#pragma once

#include "flexdmd_i.h"

#include "FlexDMDScene.h"
#include "FlexDMDImageActor.h"

class FlexDMDBackgroundScene : public FlexDMDScene
{
public:
   FlexDMDBackgroundScene();
   ~FlexDMDBackgroundScene();

   static CComObject<FlexDMDBackgroundScene>* Create(FlexDMD* pFlexDMD, FlexDMDActor* pBackground, AnimationType animateIn, float pauseS, AnimationType animateOut, string id);

   void SetBackground(FlexDMDActor* background);
   FlexDMDActor* GetBackground();

   virtual void Update(float delta);

private:
   FlexDMDActor* m_pBackground;
};