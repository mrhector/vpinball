#include "stdafx.h"

#include "FlexDMDScene.h"
#include "../actors/FlexDMDFadeOverlay.h"

FlexDMDScene::FlexDMDScene()
{
   SetVisible(true);

   m_active = false;

   m_time = 0;
}

FlexDMDScene::~FlexDMDScene()
{
   for (auto it = m_tweens.begin(); it != m_tweens.end();)
      m_tweens.erase(it);
}

CComObject<FlexDMDScene>* FlexDMDScene::Create(FlexDMD* pFlexDMD, AnimationType animateIn, float pauseS, AnimationType animateOut, const string& id)
{
   CComObject<FlexDMDScene>* obj = nullptr;
   CComObject<FlexDMDScene>::CreateInstance(&obj);
   obj->AddRef();

   obj->SetFlexDMD(pFlexDMD);
   obj->SetName(id);

   obj->m_animateIn = animateIn;
   obj->m_animateOut = animateOut;
   obj->m_pause = pauseS;

   return obj;
}

void FlexDMDScene::OnStageStateChanged()
{
   FlexDMDGroupActor::OnStageStateChanged();
   UpdateState();
}

void FlexDMDScene::UpdateState()
{
   bool shouldBeActive = GetVisible() && GetOnStage();
   if (shouldBeActive && !m_active) {
      m_active = true;
      SetSize(GetParent()->GetWidth(), GetParent()->GetHeight());
      m_inAnimLength = StartAnimation(m_animateIn);
      m_outAnimLength = -1;
      Begin();
   }
   else if (!shouldBeActive && m_active) {
      m_active = false;
      for (auto it = m_tweens.begin(); it != m_tweens.end();)
         m_tweens.erase(it);
   }
}

void FlexDMDScene::Begin()
{
}

float FlexDMDScene::StartAnimation(AnimationType animation)
{
   float alphaLength = 0.5f;
   float scrollWLength = 0.5f;
   float scrollHLength = scrollWLength * GetHeight() / GetWidth();
   // TODO Missing animations: ZoomIn = 2, ZoomOut = 3
   switch (animation) {
      case AnimationType_FadeIn:
      {
         FlexDMDFadeOverlay* pFade = FlexDMDFadeOverlay::Create(GetFlexDMD(), "");
         AddActor(pFade);
         pFade->SetAlpha(1.0f);
         pFade->SetColor(RGB(0, 0, 0));
         AddTween(pFade->GetAlpha(), 0, alphaLength, ([pFade](float newValue) {
            pFade->SetAlpha(newValue);
            return false;
         }));
         return alphaLength;
      }
      case AnimationType_FadeOut:
      {
         FlexDMDFadeOverlay* pFade = FlexDMDFadeOverlay::Create(GetFlexDMD(), "");
         AddActor(pFade);
         pFade->SetAlpha(0.0f);
         pFade->SetColor(RGB(0, 0, 0));
         AddTween(pFade->GetAlpha(), 1.0f, alphaLength, ([pFade](float newValue) {
            pFade->SetAlpha(newValue);
            return false;
         }));
         return alphaLength;
      }
      case AnimationType_ScrollOffLeft:
      {
         SetX(0);
         AddTween(GetX(), -GetWidth(), scrollWLength, ([this](float newValue) {
            this->SetX(newValue);
            return false;
         }));
         return scrollWLength;
      }
      case AnimationType_ScrollOffRight:
      {
         SetX(0);
         AddTween(GetX(), GetWidth(), scrollWLength, ([this](float newValue) {
            this->SetX(newValue);
            return false;
         }));
         return scrollWLength;
      }
      case AnimationType_ScrollOnLeft:
      {
         SetX(GetWidth());
         AddTween(GetX(), 0.0f, scrollWLength, ([this](float newValue) {
            this->SetX(newValue);
            return false;
         }));
         return scrollWLength;
      }
      case AnimationType_ScrollOnRight:
      {
         SetX(-GetWidth());
         AddTween(GetX(), 0.0f, scrollWLength, ([this](float newValue) {
            this->SetX(newValue);
            return false;
         }));
         return scrollWLength;
      }
      case AnimationType_ScrollOffUp:
      {
         SetY(0);
         AddTween(GetY(), -GetHeight(), scrollHLength, ([this](float newValue) {
            this->SetY(newValue);
            return false;
         }));
         return scrollHLength;
      }
      case AnimationType_ScrollOffDown:
      {
         SetY(0);
         AddTween(GetY(), GetHeight(), scrollHLength, ([this](float newValue) {
            this->SetY(newValue);
            return false;
         }));
         return scrollHLength;
      }
      case AnimationType_ScrollOnUp:
      {
         SetY(GetHeight());
         AddTween(GetY(), 0.0f, scrollHLength, ([this](float newValue) {
            this->SetY(newValue);
            return false;
         }));
         return scrollHLength;
      }
      case AnimationType_ScrollOnDown:
      {
         SetY(-GetHeight());
         AddTween(GetY(), 0.0f, scrollHLength, ([this](float newValue) {
            this->SetY(newValue);
            return false;
         }));
         return scrollHLength;
      }
      case AnimationType_FillFadeIn:
      {
         FlexDMDFadeOverlay* pFade = FlexDMDFadeOverlay::Create(GetFlexDMD(), "");
         pFade->SetAlpha(1.0f);
         pFade->SetColor(RGB(0, 0, 0));
         AddActor(pFade);
         pFade = FlexDMDFadeOverlay::Create(GetFlexDMD(), "");
         pFade->SetAlpha(0.0f);
         pFade->SetColor(RGB(255, 255, 255));
         AddActor(pFade);
         AddTween(pFade->GetAlpha(), 1.0f, alphaLength, ([pFade](float newValue) {
            pFade->SetAlpha(newValue);
            return false;
         }));
         return alphaLength;
      }
      case AnimationType_FillFadeOut:
      {
         FlexDMDFadeOverlay* pFade = FlexDMDFadeOverlay::Create(GetFlexDMD(), "");
         pFade->SetAlpha(1.0f);
         pFade->SetColor(RGB(255, 255, 255));
         AddActor(pFade);
         pFade = FlexDMDFadeOverlay::Create(GetFlexDMD(), "");
         pFade->SetAlpha(0.0f);
         pFade->SetColor(RGB(0, 0, 0));
         AddActor(pFade);
         AddTween(pFade->GetAlpha(), 1.0f, alphaLength, ([pFade](float newValue) {
            pFade->SetAlpha(newValue);
            return false;
         }));
        return alphaLength;
      }
      case AnimationType_None:
         return 0.0f;
      default:
         PLOGE.printf("Unsupported animation in scene '%s': %d", GetName().c_str(), animation);
         return 0.0f;
   }
}

void FlexDMDScene::Update(float secondsElapsed)
{
   FlexDMDGroupActor::Update(secondsElapsed);
   m_time += secondsElapsed;
   if (m_pause >= 0.0f && m_outAnimLength < 0 && m_time >= m_inAnimLength + m_pause)
      m_outAnimLength = StartAnimation(m_animateOut);

   for (auto it = m_tweens.begin(); it != m_tweens.end();) {
      it->step((int)(secondsElapsed * 1000.0f));
      if (it->progress() >= 1.0)
         m_tweens.erase(it);
      else
        ++it;
   }
}

bool FlexDMDScene::IsFinished()
{
   return m_pause >= 0.0f && m_outAnimLength >= 0 && m_time >= m_inAnimLength + m_pause + m_outAnimLength;
}

tweeny::tween<float>& FlexDMDScene::AddTween(float from, float to, float duration, std::function<bool (float)> callback)
{
   m_tweens.emplace_back(tweeny::tween<float>(tweeny::from(from).to(to).during((int)(duration * 1000.0f)).onStep(callback)));
   return m_tweens.back();
}