#pragma once

#ifndef __ANDROID__
#pragma push_macro("_WIN64")
#undef _WIN64
#include "serialib.h"
#pragma pop_macro("_WIN64")
#else
#include <jni.h>
#endif

class SerialPort {
public:
   void SetReadTimeout(int timeout);
   void SetWriteTimeout(int timeout);
   bool Open(string* pDevice, int baudRate, int dataBits, int stopBits, int parity);
   bool IsOpen();
   void Close();
   int Available();
   void ClearDTR();
   void SetDTR();
   void ClearRTS();
   void SetRTS();
   int WriteBytes(UINT8* pBytes, int size);
   int WriteChar(UINT8 byte);
   int ReadBytes(UINT8* pBytes, int size);
   int ReadChar(UINT8 *pByte);
   UINT8 ReadByte();

private:
   int m_readTimeout = 0;
   int m_writeTimeout = 0;
#ifndef __ANDROID__
   serialib m_seriallib;
#endif
};
