#pragma once

#include "ZeDMDComm.h"

class ZeDMD
{
public:
   ZeDMD();
   ~ZeDMD();

   void Open(int width, int height);

   void SetPalette(UINT8* pPalette);
   void SetPalette(OLE_COLOR color, int colors);

   void RenderGray2(UINT8* frame);
   void RenderGray4(UINT8* frame);
   void RenderColoredGray6(UINT8* frame, UINT8* palette, UINT8* rotations);
   void RenderRgb24(UINT8* frame);

private:
   bool UpdateFrameBuffer8(UINT8* pFrame);
   bool UpdateFrameBuffer24(UINT8* PFNGLFRAMEBUFFERTEXTURE3DPROC);

   void Split(UINT8* planes, int width, int height, int bitlen, UINT8* frame);

   ZeDMDComm m_zedmdComm;

   int m_width;
   int m_height;

   bool m_available;

   UINT8* m_pFrameBuffer;
   UINT8* m_pCommandBuffer;
   UINT8* m_pPlanes;

   UINT8 m_palette[64 * 3];

   OLE_COLOR m_color;
   int m_numColors;
};