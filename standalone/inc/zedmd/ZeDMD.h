#pragma once

#include "ZeDMDComm.h"

class ZeDMD
{
public:
   ZeDMD();
   ~ZeDMD();

   void Open();

   void SetPalette(UINT8* pPalette);
   void SetPalette(OLE_COLOR color, int colors);

   void RenderGray2(UINT8* frame, int width, int height);
   void RenderGray4(UINT8* frame, int width, int height);
   void RenderColoredGray6(UINT8* frame, UINT8* palette, UINT8* rotations, int width, int height);
   void RenderRgb24(UINT8* frame, int width, int height);

private:
   bool UpdateFrameBuffer8(UINT8* pFrame, int width, int height);
   bool UpdateFrameBuffer24(UINT8* pFrame, int width, int height);

   void Scale8(UINT8* dst, int dstWidth, int dstHeight, UINT8* src, int srcWidth, int srcHeight);
   void Scale24(UINT8* dst, int dstWidth, int dstHeight, UINT8* src, int srcWidth, int srcHeight);
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