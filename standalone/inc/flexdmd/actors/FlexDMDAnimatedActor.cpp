#include "stdafx.h"

#include "FlexDMDAnimatedActor.h"

FlexDMDAnimatedActor::FlexDMDAnimatedActor()
{
   m_scaling = Scaling_Stretch;
   m_alignment = Alignment_Center;
   m_paused = false;
   m_loop = true;
   m_playSpeed = 1.0f;
}

FlexDMDAnimatedActor::~FlexDMDAnimatedActor()
{
}

void FlexDMDAnimatedActor::Missing34() { };
void FlexDMDAnimatedActor::Missing35() { };
void FlexDMDAnimatedActor::Missing36() { };
void FlexDMDAnimatedActor::Missing37() { };

STDMETHODIMP FlexDMDAnimatedActor::get_Length(single *pRetVal)
{
   *pRetVal = m_length;

   return S_OK;
}

STDMETHODIMP FlexDMDAnimatedActor::get_Loop(VARIANT_BOOL *pRetVal)
{
   *pRetVal = m_loop ? VARIANT_TRUE : VARIANT_FALSE;

   return S_OK;
}

STDMETHODIMP FlexDMDAnimatedActor::put_Loop(VARIANT_BOOL pRetVal)
{
   m_loop = (pRetVal == VARIANT_TRUE);

   return S_OK;
}

STDMETHODIMP FlexDMDAnimatedActor::get_Paused(VARIANT_BOOL *pRetVal)
{
   *pRetVal = m_paused ? VARIANT_TRUE : VARIANT_FALSE;

   return S_OK;
}

STDMETHODIMP FlexDMDAnimatedActor::put_Paused(VARIANT_BOOL pRetVal)
{
   m_paused = (pRetVal == VARIANT_TRUE);

   return S_OK;
}

STDMETHODIMP FlexDMDAnimatedActor::get_PlaySpeed(single *pRetVal)
{
   *pRetVal = m_playSpeed;

   return S_OK;
}

STDMETHODIMP FlexDMDAnimatedActor::put_PlaySpeed(single pRetVal)
{
   m_playSpeed = pRetVal;

   return S_OK;
}

STDMETHODIMP FlexDMDAnimatedActor::Seek(single posInSeconds)
{
   Rewind();
   Advance(posInSeconds);

   return S_OK;
}

void FlexDMDAnimatedActor::Update(float delta)
{
   FlexDMDActor::Update(delta);

   if (!GetVisible())
      return;

   if (!m_paused)
      Advance(delta * m_playSpeed);
}

void FlexDMDAnimatedActor::Advance(float delta)
{
   m_time += delta;
   while (!m_endOfAnimation && m_time >= m_frameTime + m_frameDuration)
      ReadNextFrame();

   if (m_endOfAnimation && m_loop) {
      float length = m_frameTime + m_frameDuration;
      m_time = fmod(m_time, length);
      Rewind();
   }
}

void FlexDMDAnimatedActor::Rewind()
{
   m_time = 0;
   m_frameTime = 0;
   m_endOfAnimation = false;
}