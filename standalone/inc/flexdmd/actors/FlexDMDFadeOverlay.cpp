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

FlexDMDFadeOverlay* FlexDMDFadeOverlay::Create(FlexDMD* pFlexDMD, const string& name)
{
   FlexDMDFadeOverlay* obj = new FlexDMDFadeOverlay();
   obj->SetFlexDMD(pFlexDMD);
   obj->SetName(name);
   
   return obj;
}

void FlexDMDFadeOverlay::Draw(Graphics* graphics)
{
   FlexDMDActor::Draw(graphics);

   if (GetParent()) {
      SetX(0);
      SetY(0);
      SetWidth(GetParent()->GetWidth());
      SetHeight(GetParent()->GetHeight());
      graphics->FillRectangleAlpha(m_color, (UINT8)SDL_min((m_alpha * 255.f), 255.f), (int)GetX(), (int)GetY(), (int)GetWidth(), (int)GetHeight());
   }
}