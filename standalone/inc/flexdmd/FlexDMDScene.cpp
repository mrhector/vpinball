#include "stdafx.h"
#include "FlexDMDScene.h"
#include "FlexDMDFadeOverlay.h"

FlexDMDScene::FlexDMDScene()
{
   SetVisible(true);

   m_active = false;
}

FlexDMDScene::~FlexDMDScene()
{
}

CComObject<FlexDMDScene>* FlexDMDScene::Create(FlexDMD* pFlexDMD, AnimationType animateIn, float pauseS, AnimationType animateOut, string id)
{
   CComObject<FlexDMDScene>* obj = nullptr;
   CComObject<FlexDMDScene>::CreateInstance(&obj);
   obj->AddRef();

   obj->m_pFlexDMD = pFlexDMD;
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
      //_tweener.Cancel();
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
         FlexDMDFadeOverlay* pFade = FlexDMDFadeOverlay::Create(m_pFlexDMD, "");
         AddActor(pFade);
         pFade->SetAlpha(1.0f);
         pFade->SetColor(RGB(0, 0, 0));
         //_tweener.Tween(fade, new { Alpha = 0f }, alphaLength);
         return alphaLength;
      }
      case AnimationType_FadeOut:
      {
         FlexDMDFadeOverlay* pFade = new FlexDMDFadeOverlay();
         AddActor(pFade);
         pFade->SetAlpha(0.f);
         pFade->SetColor(RGB(0, 0, 0));
         //_tweener.Tween(fade, new { Alpha = 1f }, alphaLength);
         return alphaLength;
      }
      case AnimationType_ScrollOffLeft:
      {
         SetX(0);
         //_tweener.Tween(this, new { X = -Width }, scrollWLength);
         return scrollWLength;
      }
      case AnimationType_ScrollOffRight:
      {
         SetX(0);
         //_tweener.Tween(this, new { X = Width }, scrollWLength);
         return scrollWLength;
      }
      case AnimationType_ScrollOnLeft:
      {
         SetX(GetWidth());
         //_tweener.Tween(this, new { X = 0f }, scrollWLength);
         return scrollWLength;
      }
      case AnimationType_ScrollOnRight:
      {
         SetX(-GetWidth());
         //_tweener.Tween(this, new { X = 0f }, scrollWLength);
         return scrollWLength;
      }
      case AnimationType_ScrollOffUp:
      {
         SetY(0);
         //_tweener.Tween(this, new { Y = -Height }, scrollHLength);
         return scrollHLength;
      }
      case AnimationType_ScrollOffDown:
      {
         SetY(0);
         //_tweener.Tween(this, new { Y = Height }, scrollHLength);
         return scrollHLength;
      }
      case AnimationType_ScrollOnUp:
      {
         SetY(GetHeight());
         //_tweener.Tween(this, new { Y = 0f }, scrollHLength);
         return scrollHLength;
      }
      case AnimationType_ScrollOnDown:
      {
         SetY(-GetHeight());
         //_tweener.Tween(this, new { Y = 0f }, scrollHLength);
         return scrollHLength;
      }
      case AnimationType_FillFadeIn:
      {
         FlexDMDFadeOverlay* pFade = FlexDMDFadeOverlay::Create(m_pFlexDMD, "");
         pFade->SetAlpha(1.0f);
         pFade->SetColor(RGB(0, 0, 0));
         AddActor(pFade);
         pFade = FlexDMDFadeOverlay::Create(m_pFlexDMD, "");
         pFade->SetAlpha(0.f);
         pFade->SetColor(RGB(255, 255, 255));
         AddActor(pFade);
         //_tweener.Tween(fade, new { Alpha = 1f }, alphaLength);
         return alphaLength;
      }
      case AnimationType_FillFadeOut:
      {
         FlexDMDFadeOverlay* pFade = FlexDMDFadeOverlay::Create(m_pFlexDMD, "");
         pFade->SetAlpha(1.0f);
         pFade->SetColor(RGB(255, 255, 255));
         AddActor(pFade);
         pFade = FlexDMDFadeOverlay::Create(m_pFlexDMD, "");
         pFade->SetAlpha(0.f);
         pFade->SetColor(RGB(0, 0, 0));
         AddActor(pFade);
        //_tweener.Tween(fade, new { Alpha = 1f }, alphaLength);
        return alphaLength;
      }
      case AnimationType_None:
         return 0.f;
      default:
         PLOGE.printf("Unsupported animation in scene '%s': %d", GetName().c_str(), animation);
         return 0.f;
   }
}

void FlexDMDScene::Update(float secondsElapsed)
{
   FlexDMDGroupActor::Update(secondsElapsed);
   m_time += secondsElapsed;
   if (m_pause >= 0.f && m_outAnimLength < 0 && m_time >= m_inAnimLength + m_pause)
      m_outAnimLength = StartAnimation(m_animateOut);
   //_tweener.Update(secondsElapsed);
}

bool FlexDMDScene::IsFinished()
{
   return m_pause >= 0.f && m_outAnimLength >= 0 && m_time >= m_inAnimLength + m_pause + m_outAnimLength;
}