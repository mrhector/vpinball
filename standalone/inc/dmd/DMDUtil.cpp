#include "stdafx.h"

#include "DMDUtil.h"

DMDUtil* DMDUtil::pInstance = NULL;

DMDUtil* DMDUtil::GetInstance()
{
   if (!pInstance)
      pInstance = new DMDUtil();

   return pInstance;
}

DMDUtil::DMDUtil()
{
   m_alphaNumeric = NumericalLayout::None;
   m_pSurface = NULL;
   m_pData = NULL;
   m_pBPData = NULL;
   m_pRGBData = NULL;
   m_pZeDMDUtil = ZeDMDUtil::GetInstance();

   m_active = false;
}

DMDUtil::~DMDUtil()
{
   Cleanup();
}

void DMDUtil::Register(int width, int height, int depth)
{
   Cleanup();

   m_active = true;

   m_width = width;
   m_height = height;
   m_depth = depth;

   m_pSurface = SDL_CreateRGBSurfaceWithFormat(0, m_width, m_height, 24, SDL_PIXELFORMAT_RGB24);

   m_pData = (UINT8*)malloc(m_width * m_height);
   memset(m_pData, 0, m_width * m_height);

   m_pBPData = (UINT8*)malloc(m_width * m_height);
   memset(m_pBPData, 0, m_width * m_height);

   m_pRGBData = (UINT8*)malloc(m_width * m_height * 3);
   memset(m_pRGBData, 0, m_width * m_height * 3);
   
   g_pplayer->SetDMDRenderCallback(std::bind(&DMDUtil::RenderCallback, this, std::placeholders::_1));

   m_pZeDMDUtil->Open(m_width, m_height);
}

void DMDUtil::Cleanup()
{
   m_pZeDMDUtil->Close();

   if (m_pSurface) {
      SDL_FreeSurface(m_pSurface);
      m_pSurface = NULL;
   }

   if (m_pData) {
      free(m_pData);
      m_pData = NULL;
   }

   if (m_pBPData) {
      free(m_pBPData);
      m_pBPData = NULL;
   }

   if (m_pRGBData) {
      free(m_pRGBData);
      m_pRGBData = NULL;
   }

   m_active = false;
}

void DMDUtil::SetSerum(const string& szPath, const string& szRom)
{
   m_serum.Load(szPath, szRom);
}

void DMDUtil::SetColor(OLE_COLOR color)
{
   if (m_serum.IsLoaded())
      return;

   m_color = color;
   RefreshPalette();
}

void DMDUtil::SetColor(OLE_COLOR color, int depth)
{
   if (m_serum.IsLoaded())
      return;

   m_color = color;
   m_depth = depth;
   RefreshPalette();
}

void DMDUtil::SetRGB24PixelData(UINT8* pPixels)
{
   if (!m_pRGBData)
      return;

   m_rgb = true;
   m_updated = true;

   memcpy(m_pRGBData, pPixels, m_width * m_height * 3);
   m_pZeDMDUtil->RenderRgb24(m_pRGBData);
}

void DMDUtil::SetPixelData(UINT8* pPixels, OLE_COLOR color, int depth)
{
   if (!m_pBPData)
      return;

   if (m_color != color || m_depth != depth)
      SetColor(color, depth);

   UINT8* ptr = pPixels;
   UINT8 r, g, b, v;
   UINT8* dst = m_pBPData;
   for (UINT32 i = 0; i < m_width * m_height; i++) {
      r = *pPixels++;
      g = *pPixels++;
      b = *pPixels++;
      v = (int)(0.2126f * r + 0.7152f * g + 0.0722f * b);
      if (v > 255) v = 255;
      *dst++ = v >> ((m_depth == 2) ? 6 : 4);
   }
   
   SetData(m_pBPData, color);
}

void DMDUtil::SetAlphaNumericData(UINT16* pData1, UINT16* pData2, bool reset, OLE_COLOR color)
{
   m_depth = 2;
   m_rgb = false;

   UINT8* pData = NULL;

   if (m_alphaNumeric != NumericalLayout::None) {
      if (pData2 != NULL)
         pData = AlphaNumeric::Render(m_alphaNumeric, pData1, pData2, reset);
      else 
         pData = AlphaNumeric::Render(m_alphaNumeric, pData1, reset);

      SetData(pData, color);
   }
}

void DMDUtil::SetData(UINT8* pData, OLE_COLOR color)
{
   if (!m_pData)
      return;

   if (m_color != color)
      SetColor(color);

   m_rgb = false;

   if (!m_serum.IsLoaded()) {
      if (pData) {
         memcpy(m_pData, pData, m_width * m_height);
         m_updated = true;

         if (m_depth == 2)
            m_pZeDMDUtil->RenderGray2(pData);
         else
            m_pZeDMDUtil->RenderGray4(pData);
      }
   }
   else if (m_serum.Convert((UINT8*)pData, m_pData, m_palette)) {
      m_updated = true;

      m_pZeDMDUtil->RenderColoredGray6(pData, m_palette, NULL);
   }
}

void DMDUtil::RenderCallback(SDL_Renderer* pRenderer)
{
   if (!m_pSurface)
      return;

   SDL_LockSurface(m_pSurface);
   UINT8* pPixels = (UINT8*)m_pSurface->pixels;

   if (m_rgb) {
      if (!m_pRGBData)
         return;

      memcpy(pPixels, m_pRGBData, m_width * m_height * 3);
   }
   else {
      if (!m_pData)
         return;

      int index;
      UINT8 r, g, b;
      for (int y = 0; y < m_pSurface->h; ++y) {
         for (int x = 0; x < m_pSurface->w; ++x) {
            index = m_pData[y * m_pSurface->w + x];
            r = m_palette[index * 3];
            g = m_palette[index * 3 + 1];
            b = m_palette[index * 3 + 2];

            pPixels[y * m_pSurface->pitch + x * 3] = r;
            pPixels[y * m_pSurface->pitch + x * 3 + 1] = g;
            pPixels[y * m_pSurface->pitch + x * 3 + 2] = b;
         }
      }
   }

   SDL_UnlockSurface(m_pSurface);

   SDL_Texture* pTexture = SDL_CreateTextureFromSurface(pRenderer, m_pSurface);

   SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, 255);
   SDL_RenderClear(pRenderer);

   if (pTexture) {
      SDL_RenderCopy(pRenderer, pTexture, NULL, NULL);
      SDL_RenderPresent(pRenderer);

      SDL_DestroyTexture(pTexture);
   }
}

float DMDUtil::CalcBrightness(float x)
{
   // function to improve the brightness with fx=ax²+bc+c, f(0)=0, f(1)=1, f'(1.1)=0
   return (-x * x + 2.1f * x) / 1.1f;
}

void DMDUtil::RefreshPalette()
{
   if (m_depth != 2 && m_depth != 4 && m_depth != 6)
      return;

   int colors = (m_depth == 2) ? 4 : (m_depth == 4) ? 16 : 64;

   UINT8 r = GetRValue(m_color);
   UINT8 g = GetGValue(m_color);
   UINT8 b = GetBValue(m_color);

   for (int i = 0; i < colors; i++) {
      float perc = CalcBrightness((float)i / (float)(colors - 1));
      int pos = i * 3;
      m_palette[pos] = (UINT8)(r * perc);
      m_palette[pos + 1] = (UINT8)(g * perc);
      m_palette[pos + 2] = (UINT8)(b * perc);
   }

   m_pZeDMDUtil->SetPalette(m_palette);
}