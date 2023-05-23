#include "stdafx.h"
#include "ZeDMDComm.h"
#include "inc/miniz/miniz.h"

/**
 * Derived from https://github.com/freezy/dmd-extensions/blob/master/LibDmd/Output/ZeDMD/ZeDMDComm.cs
 */

ZeDMDComm::ZeDMDComm()
{
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
	  int bufferedFramesThreshold = ZEDMD_FRAME_BUFFER_SIZE;

      while (m_serialPort.IsOpen()) {
         if (m_frames.empty()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(8));
            continue;
         }

         ZeDMDFrame frame = m_frames.front();
         m_frames.pop();

         if (frame.size < ZEDMD_COMMAND_SIZE_LIMIT) {
              sleep = false;
              bufferedFramesThreshold = ZEDMD_FRAME_BUFFER_SIZE;
         }
         else if (frame.size > ZEDMD_SLOW_FRAMES_THRESHOLD) {
            sleep = false;
            bufferedFramesThreshold = 4;
         }
         else
            bufferedFramesThreshold = 32;

         sleep = !StreamBytes(&frame) || sleep;

         if (sleep)
            std::this_thread::sleep_for(std::chrono::milliseconds(8));

         free(frame.data);

         while (m_frames.size() >= bufferedFramesThreshold) {
            frame = m_frames.front();
            m_frames.pop();
            if (frame.size < ZEDMD_COMMAND_SIZE_LIMIT) {
                while (m_frames.size() > 0) {
                    ZeDMDFrame tmpFrame = m_frames.front();
                    m_frames.pop();
                    free(tmpFrame.data);
                }
                m_frames.push(frame);
            }
            else
                free(frame.data);
         }
      }

      PLOGI.printf("ZeDMD run thread finished");
   });
}

void ZeDMDComm::QueueFrame(char command, UINT8* data, int size)
{
   if (!m_serialPort.IsOpen())
      return;

   if (data && size > 0) {
      UINT8* buffer = (UINT8*)malloc(size);
      memcpy(buffer, data, size);
      new std::thread([this, command, buffer, size](){
          ZeDMDFrame frame;
          mz_ulong compressedSize = mz_compressBound(size);
          frame.data = (UINT8*)malloc(CTRL_CHARS_HEADER_SIZE + 3 + compressedSize);
          memcpy(frame.data, CTRL_CHARS_HEADER, CTRL_CHARS_HEADER_SIZE);
          frame.data[CTRL_CHARS_HEADER_SIZE] = command;
          mz_compress(frame.data + CTRL_CHARS_HEADER_SIZE + 3, &compressedSize, buffer, size);
          frame.data[CTRL_CHARS_HEADER_SIZE + 1] = (UINT8)(compressedSize >> 8 & 0xFF);
          frame.data[CTRL_CHARS_HEADER_SIZE + 2] = (UINT8)(compressedSize & 0xFF);
          frame.size = CTRL_CHARS_HEADER_SIZE + 3 + compressedSize;
          m_frames.push(frame);
          free(buffer);
      });
   }
}

void ZeDMDComm::QueueCommand(char command)
{
   if (!m_serialPort.IsOpen())
      return;

   ZeDMDFrame frame;
   frame.data = (UINT8*)malloc(CTRL_CHARS_HEADER_SIZE + 1);
   memcpy(frame.data, CTRL_CHARS_HEADER, CTRL_CHARS_HEADER_SIZE);
   frame.data[CTRL_CHARS_HEADER_SIZE] = command;
   frame.size = CTRL_CHARS_HEADER_SIZE + 1;
   m_frames.push(frame);
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
         m_width = data[4] + data[5] * 256;
         m_height = data[6] + data[7] * 256;

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
                     PLOGI.printf("ZeDMD found: device=%s, width=%d, height=%d", pDevice->c_str(), m_width, m_height);
                  }
                  else {
                     PLOGI.printf("ZeDMD found: width=%d, height=%d", m_width, m_height);
                  }

#ifdef ZEDMD_DEBUG
                  QueueCommand(ZEDMD_COMMAND::DebugEnable);
#endif

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
   if (m_serialPort.ReadByte() == 'R') {
      int position = 0;
 
      while (position < pFrame->size) {
         m_serialPort.WriteBytes(pFrame->data + position, ((pFrame->size - position) < ZEDMD_MAX_SERIAL_WRITE_AT_ONCE) ? (pFrame->size - position) : ZEDMD_MAX_SERIAL_WRITE_AT_ONCE);
         if (m_serialPort.ReadByte() == 'A')
            position += ZEDMD_MAX_SERIAL_WRITE_AT_ONCE;
         else
            return false;
      }

      return true;
   }

   return false;
}