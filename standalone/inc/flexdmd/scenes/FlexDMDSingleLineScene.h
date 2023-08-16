#pragma once

#include "FlexDMDBackgroundScene.h"
#include "../actors/FlexDMDLabelActor.h"

class FlexDMDSingleLineScene : public FlexDMDBackgroundScene
{
public:
   FlexDMDSingleLineScene();
   ~FlexDMDSingleLineScene();

   static CComObject<FlexDMDSingleLineScene>* Create(FlexDMD* pFlexDMD, FlexDMDActor* pBackground, const string& text, Font* pFont, AnimationType animateIn, float pauseS, AnimationType animateOut, bool scroll, const string& id);

   void SetText(string text);
   virtual void Begin();
   virtual void Update(float delta);

private:
   CComObject<FlexDMDLabelActor>* m_pText;
   bool m_scroll;
   float m_scrollX;
};