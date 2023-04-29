#include "stdafx.h"
#include "ZeDMD.h"

ZeDMD::ZeDMD()
{
   m_width = 0;
   m_height = 0;
   
   m_pCache = NULL;
   m_pPlanes = NULL;
   m_pTemp = NULL;
}

ZeDMD::~ZeDMD()
{
   m_dmd.Disconnect();

   if (m_pCache)
      delete m_pCache;

   if (m_pPlanes)
      delete m_pPlanes;
      
   if (m_pTemp)
      delete m_pTemp;
}

void ZeDMD::Open(int width, int height)
{
   if (m_dmd.Connect()) {
      m_pCache = (UINT8*)malloc(width * height * 3);
      m_pPlanes = (UINT8*)malloc(width * height * 3);
      m_pTemp = (UINT8*)malloc(width * height * 3);
     
      m_width = width;
      m_height = height;   

      // m_dmd.QueueCommand(ZEDMD_COMMAND::DebugEnable);

      SetColor(RGB(200, 10, 10));

      m_dmd.Run();
   }
}

void ZeDMD::SetColor(OLE_COLOR color)
{
   UINT8 r = color & 0xFF;
   UINT8 g = (color >> 8) & 0xFF;
   UINT8 b = (color >> 16) & 0xFF;

   int pos = 0;
   for (int i = 0; i < 4; i++) {
      float perc = CalcBrightness(i / 3.0f);
      m_palette2[pos++] = (UINT8)(g * perc);
      m_palette2[pos++] = (UINT8)(r * perc);
      m_palette2[pos++] = (UINT8)(b * perc);
   }

   pos = 0;
   for (int i = 0; i < 16; i++) {
      float perc = CalcBrightness(i / 15.0f);
      m_palette4[pos++] = (UINT8)(g * perc);
      m_palette4[pos++] = (UINT8)(r * perc);
      m_palette4[pos++] = (UINT8)(b * perc);
   }
}

void ZeDMD::RenderGray2(UINT8* frame, int width, int height)
{
   if (!m_pCache)
      return;

   int size = width * height;

   if (memcmp(m_pCache, frame, size)) {
      memcpy(m_pCache, frame, size);

      int palette_size = 12;
      int buffer_size = (size / 8 * 2);

      ConvertFrameToPlanes(width, height, frame, m_pPlanes, 2);

      memcpy(m_pTemp, m_palette2, palette_size);
      memcpy(m_pTemp + palette_size, m_pPlanes, buffer_size);

      m_dmd.QueueCommand(ZEDMD_COMMAND::Gray2, m_pTemp, buffer_size + palette_size);
   }
}

void ZeDMD::RenderGray4(UINT8* frame, int width, int height)
{
   if (!m_pCache)
      return;

   int size = width * height;

   if (memcmp(m_pCache, frame, size)) {
      memcpy(m_pCache, frame, size);

      int palette_size = 48;
      int buffer_size = (size / 8 * 4);

      ConvertFrameToPlanes(width, height, frame, m_pPlanes, 4);

      memcpy(m_pTemp, m_palette4, palette_size);
      memcpy(m_pTemp + palette_size, m_pPlanes, buffer_size);

      m_dmd.QueueCommand(ZEDMD_COMMAND::ColGray4, m_pTemp, buffer_size + palette_size);
   }
}

void ZeDMD::RenderRgb24(UINT8* frame, int width, int height)
{
   if (!m_pCache)
      return;

   int size = width * height * 3;
   
   if (memcmp(m_pCache, frame, size)) {
      memcpy(m_pCache, frame, size);
      UINT8* ptr = (UINT8*)frame;
      int pos = 0;
      for (int y = 0; y < height; y++) {
         for (int x = 0; x < width; x++) {
            UINT8 r = *ptr++;
            UINT8 g = *ptr++;
            UINT8 b = *ptr++;
            m_pTemp[pos++] = g; 
            m_pTemp[pos++] = r;
            m_pTemp[pos++] = b;
         }
      }
      m_dmd.QueueCommand(ZEDMD_COMMAND::RGB24, m_pTemp, size);
   }
}

float ZeDMD::CalcBrightness(float x)
{
   // function to improve the brightness with fx=ax²+bc+c, f(0)=0, f(1)=1, f'(1.1)=0
   return (-x * x + 2.1f * x) / 1.1f;
}

void ZeDMD::ConvertFrameToPlanes(UINT32 width, UINT32 height, UINT8* frame, UINT8* planes, int bitDepth)
{
   UINT8 bitMask = 1;
   UINT32 tj = 0;
   const UINT32 frameSize = height * width;
   const UINT32 planeOffset = frameSize / 8;

   for (UINT8 tk = 0; tk < bitDepth; tk++)
      planes[tk * planeOffset + tj] = 0;

   for (UINT32 ti = 0; ti < frameSize; ti++) {
      UINT8 tl = 1;
      for (UINT8 tk = 0; tk < bitDepth; tk++) {
         if ((frame[ti] & tl) > 0)
            planes[tk * planeOffset + tj] |= bitMask;
            
         tl <<= 1;
      }

      if (bitMask == 0x80) {
         bitMask = 1;
         tj++;
         if (tj < planeOffset) {
            for (UINT8 tk = 0; tk < bitDepth; tk++) {
               planes[tk * planeOffset + tj] = 0;
            }
         }
      }
      else
         bitMask <<= 1;
    }
}

