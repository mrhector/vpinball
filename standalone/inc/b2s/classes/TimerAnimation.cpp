#include "stdafx.h"

#include "TimerAnimation.h"
#include "../collections/RunningAnimationsCollection.h"
#include "../forms/Form.h"
#include "../controls/PictureBox.h"
#include "B2SData.h"
#include "B2SSettings.h"
#include "B2SAnimation.h"

TimerAnimation::TimerAnimation(
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
   vector<SDL_Surface*> images) : B2SAnimationBase(dualMode, interval, eType_ImageCollectionAtForm, loops, false,
      startTimerAtVPActivate, lightsStateAtAnimationStart, lightsStateAtAnimationEnd, animationStopBehaviour,
      lockInvolvedLamps, hideScoreDisplays, bringToFront, randomStart, randomQuality)
{
   //Me.SynchronizingObject = _Form
   m_pForm = pForm;
   m_pFormDMD = pFormDMD;
   m_pPictureBox = NULL;
   SetName(szName);
   //Me.ImageCollection = _ImageCollection

   m_loopticker = 0;
   m_ticker = 0;
   m_reachedThe0Point = false;

   SetElapsedListener(std::bind(&TimerAnimation::TimerAnimationTick, this, std::placeholders::_1));
}

TimerAnimation::TimerAnimation(
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
    vector<SDL_Surface*> images) : B2SAnimationBase(dualMode, interval, eType_ImageCollectionAtForm, loops, false,
       startTimerAtVPActivate, lightsStateAtAnimationStart, lightsStateAtAnimationEnd, animationStopBehaviour, 
       lockInvolvedLamps, hideScoreDisplays, bringToFront, randomStart, randomQuality)
{
   //Me.SynchronizingObject = _Form
   m_pForm = pForm;
   m_pFormDMD = NULL;
   m_pPictureBox = pPictureBox;
   SetName(szName);
   //Me.ImageCollection = _ImageCollection

   m_loopticker = 0;
   m_ticker = 0;
   m_reachedThe0Point = false;

   SetElapsedListener(std::bind(&TimerAnimation::TimerAnimationTick, this, std::placeholders::_1));
}

void TimerAnimation::Start()
{
   //set the start flag
   SetWouldBeStarted(true);
   SetStopMeLater(false);

   B2SSettings* pB2SSettings = B2SSettings::GetInstance();
   B2SData* pB2SData = B2SData::GetInstance();

   // maybe get out here because animation is not allowed to start
   if (pB2SSettings->GetAllAnimationSlowDown() == 0 || GetSlowDown() == 0)
      return;

   // maybe get out here because of not matching dual mode
   if (pB2SData->IsDualBackglass()) {
       if (GetDualMode() == eDualMode_Both || GetDualMode() == pB2SSettings->GetCurrentDualMode())
          return;
   }

   // start the base timer
   B2S::Timer::Start();
   GetRunningAnimations()->Add(GetName());
}

void TimerAnimation::Stop()
{
   // remove the start flag
   SetWouldBeStarted(false);

   // maybe stop the animation a bit later to do the last animation steps
   if (GetAnimationStopBehaviour() == eAnimationStopBehaviour_RunAnimationTillEnd || GetAnimationStopBehaviour() == eAnimationStopBehaviour_RunAnimationToFirstStep) {
      if (!IsStopMeLater())
         m_reachedThe0Point = false;
      SetStopMeLater(true);
      return;
   }

   // stop the base timer
   GetRunningAnimations()->Remove(GetName());
   B2S::Timer::Stop();
}

void TimerAnimation::TimerAnimationTick(B2S::Timer* pTimer)
{
   // first of all stop the timer
   B2S::Timer::Stop();

   // maybe get out since the timer is already stopped
   if (!GetRunningAnimations()->Contains(GetName()))
      return;

   B2SSettings* pB2SSettings = B2SSettings::GetInstance();

   // show image
   if (GetType() == eType_ImageCollectionAtForm)
       m_pForm->SetBackgroundImage(m_images[m_ticker]);
   else if (GetType() == eType_ImageCollectionAtPictureBox)
       m_pPictureBox->SetImage(m_images[m_ticker]);

   // count on and maybe restart the timer
   bool restart = true;
   m_ticker++;
   if (m_ticker >= m_images.size()) {
      m_reachedThe0Point = true;
      m_loopticker++;
      m_ticker = 0;
      if (GetLoops() > 0 && m_loopticker >= GetLoops()) {
         m_loopticker = 0;
         restart = false;
         RaiseFinishedEvent();
      }
   }
   if (restart) {
      if (pB2SSettings->GetAllAnimationSlowDown() == 0 || GetSlowDown() == 0)
         Stop();
      else if (IsStopMeLater() && GetAnimationStopBehaviour() == eAnimationStopBehaviour_RunAnimationTillEnd && m_ticker == 0)
         Stop();
      else if (IsStopMeLater() && GetAnimationStopBehaviour() == eAnimationStopBehaviour_RunAnimationToFirstStep && (m_ticker == 1 || m_ticker == 2) && m_reachedThe0Point)
         Stop();
      else
         B2S::Timer::Start();
   }
   else
      Stop();
}