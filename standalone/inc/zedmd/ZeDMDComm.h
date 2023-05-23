#pragma once

#include "SerialPort.h"

#include <thread>
#include <queue>

typedef enum {
    Handshake = 0x0c,
    Compression = 0x0e,
    Chunk = 0x0d,
    DebugEnable = 0x63,
    RGB24 = 0x03,
    Gray2 = 0x08,
    ColGray4 = 0x09,
    ColGray6 = 0x0b,
} ZEDMD_COMMAND;

struct ZeDMDFrame {
   int size;
   UINT8* data;
};

#define ZEDMD_BAUD_RATE 921600
#define ZEDMD_SERIAL_TIMEOUT 200

#ifndef __APPLE__
#define ZEDMD_MAX_SERIAL_WRITE_AT_ONCE 2048
#else
#define ZEDMD_MAX_SERIAL_WRITE_AT_ONCE 256
#endif

#define ZEDMD_SLOW_FRAMES_THRESHOLD 4096
#define ZEDMD_FRAME_BUFFER_SIZE 128
#define ZEDMD_COMMAND_SIZE_LIMIT 100

class ZeDMDComm
{
public:
   static const int CTRL_CHARS_HEADER_SIZE = 6;
   static constexpr UINT8 CTRL_CHARS_HEADER[] = { 0x5a, 0x65, 0x64, 0x72, 0x75, 0x6d };

public:
   ZeDMDComm();
   ~ZeDMDComm();

   bool Connect();
   int GetWidth() { return m_width; }
   int GetHeight() { return m_height; }
   void Disconnect();

   void Run(); 
   void QueueFrame(char command, UINT8* buffer, int size);
   void QueueCommand(char command);

private:
   bool Connect(string* pName);
   void Reset();
   bool StreamBytes(ZeDMDFrame* pFrame);

   SerialPort m_serialPort;
   std::queue<ZeDMDFrame> m_frames;
   std::thread* m_pThread;
   int m_width;
   int m_height;
};