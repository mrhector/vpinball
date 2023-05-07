#pragma once

#include "ZeDMDComm.h"

class ZeDMD
{
public:
   ZeDMD();
   ~ZeDMD();

   void Open(int width, int height);

   void SetColor(OLE_COLOR color);

   void RenderGray2(UINT8* frame, int width, int height);
   void RenderGray4(UINT8* frame, int width, int height);
   void RenderColoredGray6(UINT8* frame, UINT8* palette, int width, int height);
   void RenderRgb24(UINT8* frame, int width, int height);

private:
   float CalcBrightness(float x);
   void Split(int width, int height, int bitlen, UINT8* frame, UINT8* planes);

   ZeDMDComm m_dmd;

   int m_width;
   int m_height;
   OLE_COLOR m_color;

   bool m_available;

   UINT8* m_pFrameBuffer;
   UINT8* m_pCommandBuffer;
   UINT8* m_pPlanes;

   UINT8* m_pPalette;
};