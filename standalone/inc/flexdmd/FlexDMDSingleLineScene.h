#pragma once

#include "flexdmd_i.h"

#include "FlexDMDScene.h"
#include "FlexDMDLabelActor.h"
#include "FlexDMDBackgroundScene.h"
#include "FlexDMDImageActor.h"

class FlexDMDSingleLineScene : public FlexDMDBackgroundScene
{
public:
   FlexDMDSingleLineScene();
   ~FlexDMDSingleLineScene();

   static CComObject<FlexDMDSingleLineScene>* Create(FlexDMD* pFlexDMD, FlexDMDActor* pBackground, string text, FlexDMDFont* pFont, AnimationType animateIn, float pauseS, AnimationType animateOut, bool scroll, string id);

   void SetText(string text);
   virtual void Begin();
   virtual void Update(float delta);

private:
   CComObject<FlexDMDLabelActor>* m_pText;
   bool m_scroll;
   float m_scrollX;
};