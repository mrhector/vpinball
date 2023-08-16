#pragma once

#include "FlexDMDBackgroundScene.h"
#include "../actors/FlexDMDLabelActor.h"

class FlexDMDTwoLineScene : public FlexDMDBackgroundScene
{
public:
   FlexDMDTwoLineScene();
   ~FlexDMDTwoLineScene();

   static CComObject<FlexDMDTwoLineScene>* Create(FlexDMD* pFlexDMD, FlexDMDActor* pBackground, const string& topText, Font* pTopFont, const string& bottomText, Font* pBottomFont, AnimationType animateIn, float pauseS, AnimationType animateOut, const string& id);

   void SetText(string topText, string bottomText);
   virtual void Update(float delta);

private:
   CComObject<FlexDMDLabelActor>* m_pTopText;
   CComObject<FlexDMDLabelActor>* m_pBottomText;
};