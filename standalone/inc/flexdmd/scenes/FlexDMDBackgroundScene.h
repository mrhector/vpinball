#pragma once

#include "FlexDMDScene.h"

class FlexDMDBackgroundScene : public FlexDMDScene
{
public:
   FlexDMDBackgroundScene();
   ~FlexDMDBackgroundScene();

   static CComObject<FlexDMDBackgroundScene>* Create(FlexDMD* pFlexDMD, FlexDMDActor* pBackground, AnimationType animateIn, float pauseS, AnimationType animateOut, string id);

   void SetBackground(FlexDMDActor* background);
   FlexDMDActor* GetBackground() { return m_pBackground; }

   virtual void Update(float delta);

private:
   FlexDMDActor* m_pBackground;
};