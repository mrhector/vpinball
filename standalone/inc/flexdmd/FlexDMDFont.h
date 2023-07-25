#pragma once

#include "flexdmd_i.h"

#include "FlexDMD.h"
#include "bmfonts/BitmapFont.h"
#include <SDL2/SDL_image.h>

DEFINE_GUID(IID_FlexDMDFontInterface, 0x11234567, 0x89AB, 0xCDEF, 0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF);

class FlexDMDFontInterface : public IUnknown {
public:
};

class FlexDMDFont : public IUnknown {
public:
    FlexDMDFont();
    ~FlexDMDFont();

   STDMETHOD(QueryInterface)(REFIID iid, void** ppv) {
       if (iid == IID_FlexDMDFontInterface) {
          *ppv = reinterpret_cast<FlexDMDFontInterface*>(this);
          AddRef();
          return S_OK;
       } else {
          *ppv = nullptr;
          return E_NOINTERFACE;
       }
   }

   STDMETHOD_(ULONG, AddRef)() {
      m_dwRef++;
      return m_dwRef;
   }

   STDMETHOD_(ULONG, Release)() {
      m_dwRef--;

      if (m_dwRef == 0)
         delete this;

      return m_dwRef;
   }

   static FlexDMDFont* Create(FlexDMD* pFlexDMD, string font, OLE_COLOR tint, OLE_COLOR borderTint, LONG borderSize);

   void DrawCharacter(SDL_Surface* graphics, char character, char previousCharacter, float& x, float& y);
   SDL_Rect MeasureFont(string text);
   void DrawText_(SDL_Surface* graphics, float x, float y, string text);

   BitmapFont* m_bitmapFont;
   
protected:
   FlexDMD* m_pFlexDMD;
   string m_font;
   OLE_COLOR m_tint;
   OLE_COLOR m_borderTint;
   int m_borderSize;
   SDL_Surface** m_textures;

private:
   LONG m_dwRef = 0;
};