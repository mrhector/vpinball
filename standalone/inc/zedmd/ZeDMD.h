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
   void RenderRgb24(UINT8* frame, int width, int height);

private:
   float CalcBrightness(float x);
   void ConvertFrameToPlanes(UINT32 width, UINT32 height, UINT8* frame, UINT8* planes, int bitDepth);

   ZeDMDComm m_dmd;

   int m_width;
   int m_height;

   UINT8* m_pCache;
   UINT8* m_pTemp;
   UINT8* m_pPlanes;
   
   UINT8 m_palette2[12];
   UINT8 m_palette4[48];
};