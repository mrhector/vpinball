#pragma once

#if !((defined(__APPLE__) && ((defined(TARGET_OS_IOS) && TARGET_OS_IOS) || (defined(TARGET_OS_TV) && TARGET_OS_TV))))
#define __ZEDMD__
#endif

#ifdef __ZEDMD__
#pragma push_macro("_WIN64")
#pragma push_macro("CALLBACK")
#undef _WIN64
#undef CALLBACK
#include "ZeDMD.h"
#pragma pop_macro("CALLBACK")
#pragma pop_macro("_WIN64")
#endif

class ZeDMDUtil {
public:
   static void CALLBACK OnLogMessage(const char* format, va_list args, const void* pUserData);

   static void Init();
   static void Open(int width, int height);
   static void SetPalette(UINT8* palette);
   static void RenderGray2(UINT8* frame);
   static void RenderGray4(UINT8 *frame);
   static void RenderColoredGray6(UINT8* frame, UINT8 *palette, UINT8 *rotations);
   static void RenderRgb24(UINT8 *frame);
   static void Close();
   
private:
#ifdef __ZEDMD__
   static ZeDMD m_zedmd;
#endif
};
