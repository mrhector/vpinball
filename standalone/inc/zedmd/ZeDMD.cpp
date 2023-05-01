#include "stdafx.h"
#include "ZeDMD.h"

/**
 * Derived from https://github.com/freezy/dmd-extensions/blob/master/LibDmd/Output/ZeDMD/ZeDMD.cs
 */

ZeDMD::ZeDMD()
{
   m_width = 0;
   m_height = 0;
   
   m_pFrameBuffer = NULL;
   m_pCommandBuffer = NULL;
   m_pPalette = NULL;

   m_pPlanes = NULL;
}

ZeDMD::~ZeDMD()
{
   m_dmd.Disconnect();

   if (m_pFrameBuffer)
      delete m_pFrameBuffer;

   if (m_pCommandBuffer)
      delete m_pCommandBuffer;

   if (m_pPalette)
      delete m_pPalette;

   if (m_pPlanes)
      delete m_pPlanes;
}

void ZeDMD::Open(int width, int height)
{
   m_available = m_dmd.Connect();

   if (m_available) {
      m_pFrameBuffer = (UINT8*)malloc(width * height * 3);
      m_pCommandBuffer = (UINT8*)malloc(width * height * 3);
      m_pPlanes = (UINT8*)malloc(width * height * 3);

      m_width = width;
      m_height = height;   

      // m_dmd.QueueCommand(ZEDMD_COMMAND::DebugEnable);

      m_color = RGB(255, 88, 32);

      m_dmd.Run();
   }
}

void ZeDMD::SetColor(OLE_COLOR color)
{
   m_color = color;

   if (m_pPalette) {
      delete m_pPalette;
      m_pPalette = NULL;
   }
}

void ZeDMD::RenderGray2(UINT8* frame, int width, int height)
{
   if (!m_available)
      return;

   int colors = 4;
   int paletteSize = colors * 3;

   if (!m_pPalette) {
      m_pPalette = (UINT8*)malloc(paletteSize);

      UINT8 r = m_color & 0xFF;
      UINT8 g = (m_color >> 8) & 0xFF;
      UINT8 b = (m_color >> 16) & 0xFF;

      for (int i = 0; i < colors; i++) {
         float perc = calc_brightness((float)i / (float)(colors - 1));
         int pos = i * 3;
         m_pPalette[pos] = (UINT8)(r * perc);
         m_pPalette[pos + 1] = (UINT8)(g * perc);
         m_pPalette[pos + 2] = (UINT8)(b * perc);
      }
   }

   int size = width * height;

   if (memcmp(m_pFrameBuffer, frame, size)) {
      memcpy(m_pFrameBuffer, frame, size);

      int bufferSize = size / 8 * 2;

      Split(width, height, 2, m_pFrameBuffer, m_pPlanes);

      memcpy(m_pCommandBuffer, m_pPalette, paletteSize);
      memcpy(m_pCommandBuffer + paletteSize, m_pPlanes, bufferSize);

      m_dmd.QueueCommand(ZEDMD_COMMAND::Gray2, m_pCommandBuffer, paletteSize + bufferSize);
   }
}

void ZeDMD::RenderGray4(UINT8* frame, int width, int height)
{
   if (!m_available)
      return;

   int colors = 16;
   int paletteSize = colors * 3;

   if (!m_pPalette) {
      m_pPalette = (UINT8*)malloc(paletteSize);

      UINT8 r = m_color & 0xFF;
      UINT8 g = (m_color >> 8) & 0xFF;
      UINT8 b = (m_color >> 16) & 0xFF;

      for (int i = 0; i < colors; i++) {
         float perc = calc_brightness((float)i / (float)(colors - 1));
         int pos = i * 3;
         m_pPalette[pos] = (UINT8)(r * perc);
         m_pPalette[pos + 1] = (UINT8)(g * perc);
         m_pPalette[pos + 2] = (UINT8)(b * perc);
      }
   }

   int size = width * height;

   if (memcmp(m_pFrameBuffer, frame, size)) {
      memcpy(m_pFrameBuffer, frame, size);

      int bufferSize = size / 8 * 4;

      Split(width, height, 4, m_pFrameBuffer, m_pPlanes);

      memcpy(m_pCommandBuffer, m_pPalette, paletteSize);
      memcpy(m_pCommandBuffer + paletteSize, m_pPlanes, bufferSize);

      m_dmd.QueueCommand(ZEDMD_COMMAND::ColGray4, m_pCommandBuffer, paletteSize + bufferSize);
   }
}

void ZeDMD::RenderColoredGray6(UINT8* frame, UINT8* palette, int width, int height)
{
   if (!m_available)
      return;

   int paletteSize = 64 * 3;

   if (!m_pPalette)
      m_pPalette = (UINT8*)malloc(paletteSize);

   int size = width * height;

   if (memcmp(m_pFrameBuffer, frame, size) ||
       memcmp(m_pPalette, palette, paletteSize)) {
      memcpy(m_pFrameBuffer, frame, size);
      memcpy(m_pPalette, palette, paletteSize);

      int bufferSize = size / 8 * 6;

      Split(width, height, 6, m_pFrameBuffer, m_pPlanes);

      memcpy(m_pCommandBuffer, palette, paletteSize);
      memcpy(m_pCommandBuffer + paletteSize, m_pPlanes, bufferSize);
      memset(m_pCommandBuffer + paletteSize + bufferSize, 0, 24);

      m_dmd.QueueCommand(ZEDMD_COMMAND::ColGray6, m_pCommandBuffer, paletteSize + bufferSize + 24);
   }
}

void ZeDMD::RenderRgb24(UINT8* frame, int width, int height)
{
   if (!m_available)
      return;

   int size = width * height * 3;

   if (memcmp(m_pFrameBuffer, frame, size)) {
      memcpy(m_pFrameBuffer, frame, size);

      m_dmd.QueueCommand(ZEDMD_COMMAND::RGB24, m_pFrameBuffer, size);
   }
}

/**
 * Derived from https://github.com/freezy/dmd-extensions/blob/master/LibDmd/Common/FrameUtil.cs
 */

void ZeDMD::Split(int width, int height, int bitlen, UINT8* frame, UINT8* planes)
{
   int planeSize = width * height / 8;
   int pos = 0;
   UINT8 bd[bitlen];

   for (int y = 0; y < height; y++) {
      for (int x = 0; x < width; x += 8) {
          memset(bd, 0, bitlen * sizeof(UINT8));

          for (int v = 7; v >= 0; v--) {
             UINT8 pixel = frame[(y * width) + (x + v)];
             for (int i = 0; i < bitlen; i++) {
                bd[i] <<= 1;
                if ((pixel & (1 << i)) != 0)
                   bd[i] |= 1;
             }
          }

          for (int i = 0; i < bitlen; i++)
             planes[i * planeSize + pos] = bd[i];

          pos++;
       }
    }
}