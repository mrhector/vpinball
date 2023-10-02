#pragma once

#include "../b2s_i.h"
#include "B2SAnimationBase.h"

class Form;
class PictureBox;

class TimerAnimation : public B2SAnimationBase
{
public:
   TimerAnimation(
      Form* pForm,
      Form* pFormDMD,
      const string& szName,
      eDualMode dualMode,
      int interval,
      int loops,
      bool startTimerAtVPActivate,
      eLightsStateAtAnimationStart lightsStateAtAnimationStart,
      eLightsStateAtAnimationEnd lightsStateAtAnimationEnd,
      eAnimationStopBehaviour animationStopBehaviour,
      bool lockInvolvedLamps,
      bool hideScoreDisplays,
      bool bringToFront,
      bool randomStart,
      int randomQuality,
      vector<SDL_Surface*> images);

   TimerAnimation(
      Form* pForm,
      PictureBox* pPictureBox,
      const string& szName,
      eDualMode dualMode,
      int interval,
      int loops,
      bool startTimerAtVPActivate,
      eLightsStateAtAnimationStart lightsStateAtAnimationStart,
      eLightsStateAtAnimationEnd lightsStateAtAnimationEnd,
      eAnimationStopBehaviour animationStopBehaviour,
      bool lockInvolvedLamps,
      bool hideScoreDisplays,
      bool bringToFront,
      bool randomStart,
      int randomQuality,
      vector<SDL_Surface*> images);

   void Start() override;
   void Stop() override;

private:
   void TimerAnimationTick(B2S::Timer* pTimer);

   Form* m_pForm;
   Form* m_pFormDMD;
   PictureBox* m_pPictureBox;
   vector<SDL_Surface*> m_images;

   int m_loopticker;
   int m_ticker;
   bool m_reachedThe0Point;
};