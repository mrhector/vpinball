#pragma once

#include "FlexDMDScene.h"
#include "FlexDMDBackgroundScene.h"

class FlexDMDScrollingCreditsScene : public FlexDMDBackgroundScene
{
public:
   FlexDMDScrollingCreditsScene();
   ~FlexDMDScrollingCreditsScene();

   static CComObject<FlexDMDScrollingCreditsScene>* Create(FlexDMD* pFlexDMD, FlexDMDActor* pBackground, vector<string> text, Font* pFont, AnimationType animateIn, float pauseS, AnimationType animateOut, const string& id);

   virtual void Begin();
   virtual void Update(float delta);

private:
   FlexDMDGroupActor* m_pContainer;
   float m_length;
};