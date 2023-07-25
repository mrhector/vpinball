#pragma once

#include "flexdmd_i.h"

#include "FlexDMDScene.h"
#include "FlexDMDBackgroundScene.h"
#include "FlexDMDLabelActor.h"
#include "FlexDMDImageActor.h"

class FlexDMDTwoLineScene : public FlexDMDBackgroundScene
{
public:
   FlexDMDTwoLineScene();
   ~FlexDMDTwoLineScene();

   static CComObject<FlexDMDTwoLineScene>* Create(FlexDMD* pFlexDMD, FlexDMDActor* pBackground, string topText, FlexDMDFont* pTopFont, string bottomText, FlexDMDFont* pBottomFont, AnimationType animateIn, float pauseS, AnimationType animateOut, string id);

   void SetText(string topText, string bottomText);
   virtual void Update(float delta);

private:
   CComObject<FlexDMDLabelActor>* m_pTopText;
   CComObject<FlexDMDLabelActor>* m_pBottomText;
};