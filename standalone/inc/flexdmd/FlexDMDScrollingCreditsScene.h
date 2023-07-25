#pragma once

#include "flexdmd_i.h"

#include "FlexDMDScene.h"
#include "FlexDMDLabelActor.h"
#include "FlexDMDBackgroundScene.h"
#include "FlexDMDImageActor.h"

class FlexDMDScrollingCreditsScene : public FlexDMDBackgroundScene
{
public:
   FlexDMDScrollingCreditsScene();
   ~FlexDMDScrollingCreditsScene();

   static CComObject<FlexDMDScrollingCreditsScene>* Create(FlexDMD* pFlexDMD, FlexDMDActor* pBackground, vector<string> text, FlexDMDFont* pFont, AnimationType animateIn, float pauseS, AnimationType animateOut, string id);

   virtual void Begin();
   virtual void Update(float delta);

private:
   FlexDMDGroupActor* m_pContainer;
   float m_length;
};