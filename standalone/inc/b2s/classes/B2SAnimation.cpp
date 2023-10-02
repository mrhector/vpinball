#include "stdafx.h"

#include "B2SAnimation.h"
#include "B2SData.h"

B2SAnimation::B2SAnimation()
{
   m_pCurrentCollectionEntries = NULL;
   m_pCurrentCollectionAnimation = NULL;
   m_pSwitchTimer = NULL;
}

B2SAnimation::~B2SAnimation()
{
}

void B2SAnimation::AddAnimation(
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
   vector<SDL_Surface*> images)
{
   if (!m_timerAnimations.contains(szName))
      m_timerAnimations[szName] = new TimerAnimation(pForm, pFormDMD, szName, dualMode, interval, loops, startTimerAtVPActivate, lightsStateAtAnimationStart, lightsStateAtAnimationEnd, animationStopBehaviour, lockInvolvedLamps, hideScoreDisplays, bringToFront, randomStart, randomQuality, images);
}

void B2SAnimation::AddAnimation(
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
   vector<SDL_Surface*> images)
{
   if (!m_timerAnimations.contains(szName))
      m_timerAnimations[szName] = new TimerAnimation(pForm, pPictureBox, szName, dualMode, interval, loops, startTimerAtVPActivate, lightsStateAtAnimationStart, lightsStateAtAnimationEnd, animationStopBehaviour, lockInvolvedLamps, hideScoreDisplays, bringToFront, randomStart, randomQuality, images);
}

void B2SAnimation::AddAnimation(
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
   vector<PictureBoxAnimationEntry*> entries)
{
   if (!m_pictureBoxAnimations.contains(szName))
      m_pictureBoxAnimations[szName] = new PictureBoxAnimation(pForm, pFormDMD, szName, dualMode, interval, loops, startTimerAtVPActivate, lightsStateAtAnimationStart, lightsStateAtAnimationEnd, animationStopBehaviour, lockInvolvedLamps, hideScoreDisplays, bringToFront, randomStart, randomQuality, entries);
}

void B2SAnimation::AddCollection(const string& szName, bool startTimerAtVPActivate, vector<PictureBoxAnimationCollectionEntry*> entries)
{
   if (!m_pictureBoxAnimationCollections.contains(szName))
      m_pictureBoxAnimationCollections[szName] = new PictureBoxAnimationCollection(startTimerAtVPActivate, entries);
}

void B2SAnimation::AutoStart()
{
   // start all autostart timer animations
   for (auto& [key, pTimer] : m_timerAnimations) {
      if (pTimer->IsStartMeAtVPActivate() && !pTimer->IsEnabled())
         pTimer->Start();
   }

   // start all autostart picture box animations
   for (auto& [key, pPictimer] : m_pictureBoxAnimations) {
      if (pPictimer->IsStartMeAtVPActivate() && !pPictimer->IsEnabled())
         pPictimer->Start();
   }

   // and now start all autostart picture box animations' collections
   for (auto& [key, pPictimer] : m_pictureBoxAnimationCollections) {
      if (pPictimer->IsStartMeAtVPActivate())
         StartCollection(key);
   }
}

void B2SAnimation::StartAnimation(const string& szName, bool playReverse)
{
   if (m_timerAnimations.contains(szName) && !m_timerAnimations[szName]->IsEnabled()) {
      m_timerAnimations[szName]->SetPlayReverse(playReverse);
      m_timerAnimations[szName]->Start();
   }
   if (m_pictureBoxAnimations.contains(szName) && !m_pictureBoxAnimations[szName]->IsEnabled()) {
      m_pictureBoxAnimations[szName]->SetPlayReverse(playReverse);
      m_pictureBoxAnimations[szName]->Start();
    }
}

void B2SAnimation::RestartAnimations()
{
   for (auto& [key, pTimer] : m_timerAnimations) {
      if (pTimer->IsWouldBeStarted()) {
         pTimer->Stop();
         pTimer->Start();
      }
      else
         pTimer->Stop();
    }
    for (auto& [key, pTimer] : m_pictureBoxAnimations) {
       if (pTimer->IsWouldBeStarted()) {
          pTimer->Stop();
          pTimer->Start();
       }
       else
          pTimer->Stop();
    }
}

void B2SAnimation::StopAnimation(const string& szName)
{
   if (m_timerAnimations.contains(szName))
      m_timerAnimations[szName]->Stop();
   if (m_pictureBoxAnimations.contains(szName))
      m_pictureBoxAnimations[szName]->Stop();
}

void B2SAnimation::StopAllAnimations()
{
   for (auto& [key, pTimer] : m_timerAnimations)
      pTimer->Stop();
   for (auto& [key, pTimer] : m_pictureBoxAnimations)
      pTimer->Stop();
}

int B2SAnimation::GetAnimationSlowDown(const string& szName)
{
   if (m_timerAnimations.contains(szName))
      return m_timerAnimations[szName]->GetSlowDown();
   if (m_pictureBoxAnimations.contains(szName))
      return m_pictureBoxAnimations[szName]->GetSlowDown();
   return 1;
}

void B2SAnimation::SetAnimationSlowDown(const string& szName, int value)
{
   if (m_timerAnimations.contains(szName))
      m_timerAnimations[szName]->SetSlowDown(value);
   if (m_pictureBoxAnimations.contains(szName))
      m_pictureBoxAnimations[szName]->SetSlowDown(value);
}

bool B2SAnimation::IsAnimationRunning(const string& szName)
{
   if (m_timerAnimations.contains(szName))
      return m_timerAnimations[szName]->IsEnabled();
   if (m_pictureBoxAnimations.contains(szName))
      return m_pictureBoxAnimations[szName]->IsEnabled();
   return false;
}

vector<string> B2SAnimation::GetAnimations()
{
   vector<string> ret;
   for (auto& [key, pTimer] : m_timerAnimations) {
      if (std::find(ret.begin(), ret.end(), key) == ret.end())
         ret.push_back(key);
   }
   for (auto& [key, pTimer] : m_pictureBoxAnimations) {
      if (std::find(ret.begin(), ret.end(), key) == ret.end())
         ret.push_back(key);
   }
   return ret;
}

void B2SAnimation::StartCollection(const string& szName)
{
   if (m_pictureBoxAnimationCollections.contains(szName)) {
      m_pCurrentCollectionEntries = m_pictureBoxAnimationCollections[szName]->GetEntries();
      m_currentCollectionIndex = 0;
      if (m_pCurrentCollectionEntries->size() > 0) {
         PictureBoxAnimationCollectionEntry* firstEntry = (*m_pCurrentCollectionEntries)[0];
         if (!firstEntry->GetAnimationName().empty() && m_pictureBoxAnimations.contains(firstEntry->GetAnimationName()) && firstEntry->GetLoops() > 0) {
            m_pCurrentCollectionAnimation = m_pictureBoxAnimations[firstEntry->GetAnimationName()];
            m_pCurrentCollectionAnimation->SetLoops(firstEntry->GetLoops());
            m_pCurrentCollectionAnimation->SetPlayReverse(firstEntry->IsPlayReverse());
            m_pCurrentCollectionAnimation->SetFinishedListener(std::bind(&B2SAnimation::CurrentCollectionAnimationFinished, this));
            m_pCurrentCollectionAnimation->Start();
         }
      }
   }
}

void B2SAnimation::StopCollection(const string& szName)
{
   if (m_pCurrentCollectionAnimation)
      m_pCurrentCollectionAnimation->Stop();
}

void B2SAnimation::CurrentCollectionAnimationFinished()
{
   m_currentCollectionIndex++;
   if (m_pCurrentCollectionEntries->size() > m_currentCollectionIndex) {
      PictureBoxAnimationCollectionEntry* nextEntry = (*m_pCurrentCollectionEntries)[m_currentCollectionIndex];
      if (!nextEntry->GetAnimationName().empty() && m_pictureBoxAnimations.contains(nextEntry->GetAnimationName()) && nextEntry->GetLoops() > 0) {
         m_pCurrentCollectionAnimation = m_pictureBoxAnimations[nextEntry->GetAnimationName()];
         m_pCurrentCollectionAnimation->SetLoops(nextEntry->GetLoops());
         m_pCurrentCollectionAnimation->SetPlayReverse(nextEntry->IsPlayReverse());
         m_pCurrentCollectionAnimation->SetFinishedListener(std::bind(&B2SAnimation::CurrentCollectionAnimationFinished, this));
         m_pCurrentCollectionAnimation->Start();
      }
   }
}

void B2SAnimation::SetSwitch(int switchid)
{
   B2SData* pB2SData = B2SData::GetInstance();

   pB2SData->GetVPinMAME()->put_Switch(switchid, VARIANT_TRUE);

   if (!m_switches.contains(switchid))
      m_switches[switchid] = true;

   if (!m_pSwitchTimer) {
      m_pSwitchTimer = new B2S::Timer();
      m_pSwitchTimer->SetInterval(200);
      m_pSwitchTimer->SetElapsedListener(std::bind(&B2SAnimation::SwitchTimerElapsed, this, std::placeholders::_1));
   }

   m_pSwitchTimer->Stop();
   m_pSwitchTimer->Start();
}

void B2SAnimation::SwitchTimerElapsed(B2S::Timer* pTimer)
{
   m_pSwitchTimer->Stop();

   B2SData* pB2SData = B2SData::GetInstance();

   for (auto& [switchid, value] : m_switches)
      pB2SData->GetVPinMAME()->put_Switch(switchid, VARIANT_FALSE);

   m_switches.clear();
}