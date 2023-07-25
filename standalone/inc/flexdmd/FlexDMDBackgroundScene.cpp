#include "stdafx.h"
#include "FlexDMDBackgroundScene.h"

FlexDMDBackgroundScene::FlexDMDBackgroundScene()
{
}

FlexDMDBackgroundScene::~FlexDMDBackgroundScene()
{
}

CComObject<FlexDMDBackgroundScene>* FlexDMDBackgroundScene::Create(FlexDMD* pFlexDMD, FlexDMDActor* pBackground, AnimationType animateIn, float pauseS, AnimationType animateOut, string id)
{
   CComObject<FlexDMDBackgroundScene>* obj = nullptr;
   CComObject<FlexDMDBackgroundScene>::CreateInstance(&obj);
   obj->AddRef();

   obj->m_pFlexDMD = pFlexDMD;
   obj->SetName(id);

   obj->m_pBackground = pBackground;
   obj->SetAnimateIn(animateIn);
   obj->SetPause(pauseS);
   obj->SetAnimateOut(animateOut);

   obj->AddActor(obj->m_pBackground);

   return obj;
}

void FlexDMDBackgroundScene::SetBackground(FlexDMDActor* pBackground)
{
   if (m_pBackground == pBackground) return;
   if (m_pBackground != NULL)
      RemoveActor(m_pBackground);
   m_pBackground = pBackground;
   if (m_pBackground != NULL)
      AddActorAt(m_pBackground, 0);
}

FlexDMDActor* FlexDMDBackgroundScene::GetBackground()
{
   return m_pBackground;
}

void FlexDMDBackgroundScene::Update(float delta)
{
   FlexDMDScene::Update(delta);
   if (m_pBackground)
      m_pBackground->SetSize(GetWidth(), GetHeight());
}
