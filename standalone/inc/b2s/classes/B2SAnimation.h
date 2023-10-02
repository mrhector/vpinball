#pragma once

#include "../b2s_i.h"

#include "../collections/RunningAnimationsCollection.h"
#include "../collections/PictureBoxAnimationCollection.h"
#include "TimerAnimation.h"
#include "PictureBoxAnimation.h"
#include "../forms/Form.h"
#include "../controls/PictureBox.h"

class B2SAnimation
{
public:
   B2SAnimation();
   virtual ~B2SAnimation();

   void AddAnimation(
      string szName,
      Form* pForm,
      Form *pFormDMD,
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

   void AddAnimation(
      string szName,
      Form* pForm,
      PictureBox* pPictureBox,
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

   void AddAnimation(
      string szName,
      Form* pForm,
      Form* pFormDMD,
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
      vector<PictureBoxAnimationEntry*> entries);

   void AddCollection(const string& szName, bool startTimerAtVPActivate, vector<PictureBoxAnimationCollectionEntry*> entries);

   void AutoStart();
   void StartAnimation(const string& szName, bool playReverse = false);
   void RestartAnimations();
   void StopAnimation(const string& szName);
   void StopAllAnimations();
   int GetAnimationSlowDown(const string& szName);
   void SetAnimationSlowDown(const string& szName, int value);
   bool IsAnimationRunning(const string& szName);
   vector<string> GetAnimations();
   void StartCollection(const string& szName);
   void StopCollection(const string& szName);

private:
   void CurrentCollectionAnimationFinished();
   void SetSwitch(int switchid);
   void SwitchTimerElapsed(B2S::Timer* pTimer);

   RunningAnimationsCollection m_runningAnimations;
   std::map<string, TimerAnimation*> m_timerAnimations;
   std::map<string, PictureBoxAnimation*> m_pictureBoxAnimations;
   std::map<string, PictureBoxAnimationCollection*> m_pictureBoxAnimationCollections;
   vector<PictureBoxAnimationCollectionEntry*>* m_pCurrentCollectionEntries;
   int m_currentCollectionIndex;
   PictureBoxAnimation* m_pCurrentCollectionAnimation;
   std::map<int, bool> m_switches;
   B2S::Timer* m_pSwitchTimer;
};