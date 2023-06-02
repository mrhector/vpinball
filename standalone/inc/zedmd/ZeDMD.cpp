#include "stdafx.h"
#include "ZeDMD.h"

/**
 * Derived from https://github.com/freezy/dmd-extensions/blob/master/LibDmd/Output/ZeDMD/ZeDMD.cs
 */

ZeDMD::ZeDMD()
{
   m_width = 0;
   m_height = 0;

   m_numColors = 0;
   m_color = RGB(0, 0, 0);

   memset(&m_palette, 0, sizeof(m_palette));

   m_pFrameBuffer = NULL;
   m_pCommandBuffer = NULL;
   m_pPlanes = NULL;
}

ZeDMD::~ZeDMD()
{
   m_zedmdComm.Disconnect();

   if (m_pFrameBuffer)
      delete m_pFrameBuffer;

   if (m_pCommandBuffer)
      delete m_pCommandBuffer;

   if (m_pPlanes)
      delete m_pPlanes;
}

void ZeDMD::Open(int width, int height)
{
   m_available = m_zedmdComm.Connect();

   if (m_available) {
      m_width = width;
      m_height = height;

      m_pFrameBuffer = (UINT8*)malloc(m_width * m_height * 3);
      m_pCommandBuffer = (UINT8*)malloc(m_width * m_height * 3);
      m_pPlanes = (UINT8*)malloc(m_width * m_height * 3);

      UINT8 size[4] = { (UINT8)(width & 0xFF), (UINT8)((width >> 8) & 0xFF),
                        (UINT8)(height & 0xFF), (UINT8)((height >> 8) & 0xFF) };

      m_zedmdComm.QueueCommand(ZEDMD_COMMAND::FrameSize, size, 4);

      const bool debug = LoadValueWithDefault(regKey[RegName::Standalone], "ZeDMDDebug", false);

      if (debug) {
         m_zedmdComm.QueueCommand(ZEDMD_COMMAND::EnableDebug);
         PLOGI.printf("ZeDMD debug enabled");
      }

      const int rgbOrder = (int)LoadValueWithDefault(regKey[RegName::Standalone], "ZeDMDRGBOrder"s, -1);

      if (rgbOrder != -1) {
         m_zedmdComm.QueueCommand(ZEDMD_COMMAND::RGBOrder, rgbOrder);
         PLOGI.printf("ZeDMD RGB order override: rgbOrder=%d", rgbOrder);
      }

      const int brightness = (int)LoadValueWithDefault(regKey[RegName::Standalone], "ZeDMDBrightness"s, -1);

      if (brightness != -1) {
         m_zedmdComm.QueueCommand(ZEDMD_COMMAND::Brightness, brightness);
         PLOGI.printf("ZeDMD brightness override: brightness=%d", brightness);
      }

      const bool save = LoadValueWithDefault(regKey[RegName::Standalone], "ZeDMDSave", false);

      if (save) {
         m_zedmdComm.QueueCommand(ZEDMD_COMMAND::SaveSettings);
         PLOGI.printf("ZeDMD save enabled");
      }

      m_zedmdComm.Run();
   }
}

void ZeDMD::SetPalette(UINT8* pPalette)
{
   if (!m_available)
      return;

   m_color = RGB(0, 0, 0);
   m_numColors = 0;

   memcpy(&m_palette, pPalette, sizeof(m_palette));
}

void ZeDMD::SetPalette(OLE_COLOR color, int numColors)
{
   if (!m_available || (m_color == color && m_numColors == numColors))
      return;

   m_color = color;
   m_numColors = numColors;

   UINT8 r = color & 0xFF;
   UINT8 g = (color >> 8) & 0xFF;
   UINT8 b = (color >> 16) & 0xFF;

   for (int i = 0; i < numColors; i++) {
      float perc = calc_brightness((float)i / (float)(numColors - 1));
      int pos = i * 3;
      m_palette[pos] = (UINT8)r * perc;
      m_palette[pos + 1] = (UINT8)g * perc;
      m_palette[pos + 2] = (UINT8)b * perc;
   }
}

void ZeDMD::RenderGray2(UINT8* pFrame)
{
   if (!m_available || !UpdateFrameBuffer8(pFrame))
      return;

   int bufferSize = (m_width * m_height) / 8 * 2;

   Split(m_pPlanes, m_width, m_height, 2, m_pFrameBuffer);

   memcpy(m_pCommandBuffer, &m_palette, 12);
   memcpy(m_pCommandBuffer + 12, m_pPlanes, bufferSize);

   m_zedmdComm.QueueCommand(ZEDMD_COMMAND::Gray2, m_pCommandBuffer, 12 + bufferSize);
}

void ZeDMD::RenderGray4(UINT8* pFrame)
{
   if (!m_available || !UpdateFrameBuffer8(pFrame))
      return;

   int bufferSize = (m_width * m_height) / 8 * 4;

   Split(m_pPlanes, m_width, m_height, 4, m_pFrameBuffer);

   memcpy(m_pCommandBuffer, m_palette, 48);
   memcpy(m_pCommandBuffer + 48, m_pPlanes, bufferSize);

   m_zedmdComm.QueueCommand(ZEDMD_COMMAND::ColGray4, m_pCommandBuffer, 48 + bufferSize);
}

void ZeDMD::RenderColoredGray6(UINT8* pFrame, UINT8* pPalette, UINT8* pRotations)
{
   if (!m_available)
      return;

   bool change = UpdateFrameBuffer8(pFrame);

   if (memcmp(&m_palette, pPalette, 192)) {
      memcpy(&m_palette, pPalette, 192);
      change = true;
   }

   if (!change)
      return;

   int bufferSize = (m_width * m_height) / 8 * 6;

   Split(m_pPlanes, m_width, m_height, 6, m_pFrameBuffer);

   memcpy(m_pCommandBuffer, pPalette, 192);
   memcpy(m_pCommandBuffer + 192, m_pPlanes, bufferSize);

   if (pRotations)
      memcpy(m_pCommandBuffer + 192 + bufferSize, pRotations, 24);
   else
      memset(m_pCommandBuffer + 192 + bufferSize, 255, 24);

   m_zedmdComm.QueueCommand(ZEDMD_COMMAND::ColGray6, m_pCommandBuffer, 192 + bufferSize + 24);
}

void ZeDMD::RenderRgb24(UINT8* pFrame)
{
   if (!m_available || !UpdateFrameBuffer24(pFrame))
      return;

   m_zedmdComm.QueueCommand(ZEDMD_COMMAND::RGB24, m_pFrameBuffer, m_width * m_height * 3);
}

bool ZeDMD::UpdateFrameBuffer8(UINT8* pFrame)
{
   if (!memcmp(m_pFrameBuffer, pFrame, m_width * m_height))
      return false;

   memcpy(m_pFrameBuffer, pFrame, m_width * m_height);
   return true;
}

bool ZeDMD::UpdateFrameBuffer24(UINT8* pFrame)
{
   if (!memcmp(m_pFrameBuffer, pFrame, m_width * m_height * 3))
      return false;

   memcpy(m_pFrameBuffer, pFrame, m_width * m_height * 3);
   return true;
}

/**
 * Derived from https://github.com/freezy/dmd-extensions/blob/master/LibDmd/Common/FrameUtil.cs
 */

void ZeDMD::Split(UINT8* pPlanes, int width, int height, int bitlen, UINT8* pFrame)
{
   int planeSize = width * height / 8;
   int pos = 0;
   UINT8 bd[bitlen];

   for (int y = 0; y < height; y++) {
      for (int x = 0; x < width; x += 8) {
          memset(bd, 0, bitlen * sizeof(UINT8));

          for (int v = 7; v >= 0; v--) {
             UINT8 pixel = pFrame[(y * width) + (x + v)];
             for (int i = 0; i < bitlen; i++) {
                bd[i] <<= 1;
                if ((pixel & (1 << i)) != 0)
                   bd[i] |= 1;
             }
          }

          for (int i = 0; i < bitlen; i++)
             pPlanes[i * planeSize + pos] = bd[i];

          pos++;
       }
    }
}