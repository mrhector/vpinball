#pragma once

#include "FlexDMDActor.h"

class FlexDMDFadeOverlay : public FlexDMDActor
{
public:
  
public:
   FlexDMDFadeOverlay();
   ~FlexDMDFadeOverlay();

   static FlexDMDFadeOverlay* Create(FlexDMD* pFlexDMD, const string& name);

   float GetAlpha() { return m_alpha; }
   void SetAlpha(float alpha) { m_alpha = alpha; }
   void SetColor(OLE_COLOR color) { m_color = color; }
   
   virtual void Draw(Graphics* graphics);

private:
   float m_alpha;
   OLE_COLOR m_color;
};