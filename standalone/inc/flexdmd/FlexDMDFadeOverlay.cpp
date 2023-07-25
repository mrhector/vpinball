#include "stdafx.h"
#include "FlexDMDFadeOverlay.h"
#include "FlexDMDGroupActor.h"

FlexDMDFadeOverlay::FlexDMDFadeOverlay()
{
   m_alpha = 1.0f;
   m_color = RGB(0, 0, 0);
}

FlexDMDFadeOverlay::~FlexDMDFadeOverlay()
{
}

FlexDMDFadeOverlay* FlexDMDFadeOverlay::Create(FlexDMD* pFlexDMD, string name)
{
   FlexDMDFadeOverlay* obj = new FlexDMDFadeOverlay();
   obj->m_pFlexDMD = pFlexDMD;
   obj->SetName(name);
   
   return obj;
}

void FlexDMDFadeOverlay::Draw(SDL_Surface* graphics)
{
   FlexDMDActor::Draw(graphics);

   if (GetParent()) {
      SetX(0);
      SetY(0);
      SetWidth(GetParent()->GetWidth());
      SetHeight(GetParent()->GetHeight());
      SDL_Rect rect = { (int)GetX(), (int)GetY(), (int)GetWidth(), (int)GetHeight() };
      Uint32 fillColor = SDL_MapRGBA(graphics->format, GetRValue(m_color), GetGValue(m_color), GetBValue(m_color), SDL_min((int)(m_alpha * 255.f), 255));
      SDL_FillRect(graphics, &rect, fillColor);
   }
}