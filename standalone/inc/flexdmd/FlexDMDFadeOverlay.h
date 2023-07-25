#pragma once

#include "flexdmd_i.h"
#include "FlexDMD.h"
#include "FlexDMDActor.h"

class FlexDMDFadeOverlay : public FlexDMDActor
{
public:
  
public:
   FlexDMDFadeOverlay();
   ~FlexDMDFadeOverlay();

   static FlexDMDFadeOverlay* Create(FlexDMD* pFlexDMD, string name);

   void SetAlpha(float alpha) { m_alpha = alpha; }
   void SetColor(OLE_COLOR color) { m_color = color; }
   
   virtual void Draw(SDL_Surface* graphics);

private:
   float m_alpha;
   OLE_COLOR m_color;
};