#include "stdafx.h"
#include "ZeDMDComm.h"
#include "inc/miniz/miniz.h"

/**
 * Derived from https://github.com/freezy/dmd-extensions/blob/master/LibDmd/Output/ZeDMD/ZeDMDComm.cs
 */

ZeDMDComm::ZeDMDComm()
{
   m_pThread = NULL;
}

ZeDMDComm::~ZeDMDComm()
{
   if (m_pThread) {
      m_pThread->join();

      delete m_pThread;
   }
}

void ZeDMDComm::Run()
{
   m_pThread = new std::thread([this](){
      PLOGI.printf("ZeDMD run thread starting");

      bool sleep = false;
	  int maxQueuedFrames = ZEDMD_FRAME_QUEUE_SIZE_MAX;

      while (m_serialPort.IsOpen()) {
         if (m_frames.empty()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(8));
            continue;
         }

         ZeDMDFrame frame = m_frames.front();
         m_frames.pop();

         if (frame.size < ZEDMD_FRAME_SIZE_COMMAND_LIMIT) {
              sleep = false;
              maxQueuedFrames = ZEDMD_FRAME_QUEUE_SIZE_MAX;
         }
         else if (frame.size > ZEDMD_FRAME_SIZE_SLOW_THRESHOLD) {
            sleep = false;
            maxQueuedFrames = ZEDMD_FRAME_QUEUE_SIZE_SLOW;
         }
         else
            maxQueuedFrames = ZEDMD_FRAME_QUEUE_SIZE_DEFAULT;

         sleep = !StreamBytes(&frame) || sleep;

         if (sleep)
            std::this_thread::sleep_for(std::chrono::milliseconds(8));

         if (frame.data)
            free(frame.data);

         while (m_frames.size() >= maxQueuedFrames) {
            frame = m_frames.front();
            m_frames.pop();
            if (frame.size < ZEDMD_FRAME_SIZE_COMMAND_LIMIT) {
                while (m_frames.size() > 0) {
                    ZeDMDFrame tmpFrame = m_frames.front();
                    m_frames.pop();

                    if (tmpFrame.data)
                       free(tmpFrame.data);
                }
                m_frames.push(frame);
            }
            else
                if (frame.data)
                   free(frame.data);
         }
      }

      PLOGI.printf("ZeDMD run thread finished");
   });
}

void ZeDMDComm::QueueCommand(char command, UINT8* data, int size)
{
   if (!m_serialPort.IsOpen())
      return;

   ZeDMDFrame frame = { 0 };
   frame.command = command;
   frame.size = size;

   if (data && size > 0) {
      frame.data = (UINT8*)malloc(size);
      memcpy(frame.data, data, size);
   }

   m_frames.push(frame);
}

void ZeDMDComm::QueueCommand(char command, UINT8 value)
{
   QueueCommand(command, &value, 1);
}

void ZeDMDComm::QueueCommand(char command)
{
   QueueCommand(command, NULL, 0);
}

bool ZeDMDComm::Connect()
{
#ifndef __ANDROID__
   char szDevice[32];

   for (int i = 0; i < 5; i++) {
#ifdef __APPLE__
      sprintf_s(szDevice, sizeof(szDevice), "/dev/cu.usbserial-%04d", i);
#else
      sprintf_s(szDevice, sizeof(szDevice), "/dev/ttyUSB%d", i);
#endif

      string device = szDevice;

      if (Connect(&device))
         return true;
   }

   return false;
#else
   return Connect(NULL);
#endif
}

void ZeDMDComm::Disconnect()
{
   if (!m_serialPort.IsOpen())
      return;

   Reset();

   m_serialPort.Close();
}

bool ZeDMDComm::Connect(string* pDevice) {
   m_serialPort.SetReadTimeout(ZEDMD_SERIAL_TIMEOUT);
   m_serialPort.SetWriteTimeout(ZEDMD_SERIAL_TIMEOUT);

   if (m_serialPort.Open(pDevice, ZEDMD_BAUD_RATE, 8, 1, 0) != 1)
      return false;

   Reset();

   UINT8 data[8] = { 0 };

   while (m_serialPort.Available() > 0)
      m_serialPort.ReadBytes(data, 8);

#ifdef __ANDROID__
   std::this_thread::sleep_for(std::chrono::milliseconds(1000));

   while (m_serialPort.Available() > 0)
      m_serialPort.ReadBytes(data, 8);
#endif

   m_serialPort.WriteBytes((UINT8*)CTRL_CHARS_HEADER, CTRL_CHARS_HEADER_SIZE);
   m_serialPort.WriteChar(ZEDMD_COMMAND::Handshake);
   std::this_thread::sleep_for(std::chrono::milliseconds(200));

   if (m_serialPort.ReadBytes(data, 8)) {
      if (!memcmp(data, CTRL_CHARS_HEADER, 4)) {
         int width = data[4] + data[5] * 256;
         int height = data[6] + data[7] * 256;

         UINT8 response;

         if (m_serialPort.ReadByte() == 'R') {
            m_serialPort.WriteBytes((UINT8*)CTRL_CHARS_HEADER, 6);
            m_serialPort.WriteChar(ZEDMD_COMMAND::Compression);
            std::this_thread::sleep_for(std::chrono::milliseconds(4));

            if (m_serialPort.ReadByte() == 'A' && m_serialPort.ReadByte() == 'R') {
               m_serialPort.WriteBytes((UINT8*)CTRL_CHARS_HEADER, 6);
               m_serialPort.WriteChar(ZEDMD_COMMAND::Chunk);
               m_serialPort.WriteChar(ZEDMD_MAX_SERIAL_WRITE_AT_ONCE / 256);
               std::this_thread::sleep_for(std::chrono::milliseconds(4));

               if (m_serialPort.ReadByte() == 'A') {
                  if (pDevice) {
                     PLOGI.printf("ZeDMD found: device=%s, width=%d, height=%d", pDevice->c_str(), width, height);
                  }
                  else {
                     PLOGI.printf("ZeDMD found: width=%d, height=%d", width, height);
                  }

                  return true;
               }
            }
         }
      }
   }

   Disconnect();

   return false;
}

void ZeDMDComm::Reset()
{
   m_serialPort.ClearDTR();
   m_serialPort.SetRTS();
   std::this_thread::sleep_for(std::chrono::milliseconds(200));

   m_serialPort.ClearRTS();
   m_serialPort.ClearDTR();
   std::this_thread::sleep_for(std::chrono::milliseconds(200));
}

bool ZeDMDComm::StreamBytes(ZeDMDFrame* pFrame)
{
   UINT8* data;
   int size;

   if (pFrame->size == 0) {
      size = CTRL_CHARS_HEADER_SIZE + 1;
      data = (UINT8*)malloc(size);
      memcpy(data, CTRL_CHARS_HEADER, CTRL_CHARS_HEADER_SIZE);
      data[CTRL_CHARS_HEADER_SIZE] = pFrame->command;
   }
   else {
      mz_ulong compressedSize = mz_compressBound(pFrame->size);
      data = (UINT8*)malloc(CTRL_CHARS_HEADER_SIZE + 3 + compressedSize);
      memcpy(data, CTRL_CHARS_HEADER, CTRL_CHARS_HEADER_SIZE);
      data[CTRL_CHARS_HEADER_SIZE] = pFrame->command;
      mz_compress(data + CTRL_CHARS_HEADER_SIZE + 3, &compressedSize, pFrame->data, pFrame->size);
      size = CTRL_CHARS_HEADER_SIZE + 3 + compressedSize;
      data[CTRL_CHARS_HEADER_SIZE + 1] = (UINT8)(compressedSize >> 8 & 0xFF);
      data[CTRL_CHARS_HEADER_SIZE + 2] = (UINT8)(compressedSize & 0xFF);
   }

   bool success = false;

   if (m_serialPort.ReadByte() == 'R') {
      int position = 0;
      success = true;

      while (position < size && success) {
         m_serialPort.WriteBytes(data + position, ((size - position) < ZEDMD_MAX_SERIAL_WRITE_AT_ONCE) ? (size - position) : ZEDMD_MAX_SERIAL_WRITE_AT_ONCE);

         char response = m_serialPort.ReadByte();

         if (response == 'A')
            position += ZEDMD_MAX_SERIAL_WRITE_AT_ONCE;
         else {
            success = false;

            PLOGE.printf("Write bytes failure: response=%c", response);
         }
      }
   }

   free(data);

   return success;
}