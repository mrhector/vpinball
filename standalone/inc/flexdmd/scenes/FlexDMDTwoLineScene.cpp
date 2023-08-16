#include "stdafx.h"

#include "FlexDMDTwoLineScene.h"

FlexDMDTwoLineScene::FlexDMDTwoLineScene()
{
   m_pTopText = NULL;
   m_pBottomText = NULL;
}

FlexDMDTwoLineScene::~FlexDMDTwoLineScene()
{
}

CComObject<FlexDMDTwoLineScene>* FlexDMDTwoLineScene::Create(FlexDMD* pFlexDMD, FlexDMDActor* pBackground, const string& topText, Font* pTopFont, const string& bottomText, Font* pBottomFont, AnimationType animateIn, float pauseS, AnimationType animateOut, const string& id)
{
   CComObject<FlexDMDTwoLineScene>* obj = nullptr;
   CComObject<FlexDMDTwoLineScene>::CreateInstance(&obj);
   obj->AddRef();

   obj->SetFlexDMD(pFlexDMD);
   obj->SetName(id);

   obj->m_pTopText = FlexDMDLabelActor::Create(pFlexDMD, pTopFont, topText, "");
   obj->m_pBottomText = FlexDMDLabelActor::Create(pFlexDMD, pBottomFont, bottomText, "");

   obj->SetAnimateIn(animateIn);
   obj->SetPause(pauseS);
   obj->SetAnimateOut(animateOut);
   obj->SetBackground(pBackground);

   obj->AddActor((FlexDMDActor*)obj->m_pTopText);
   obj->AddActor((FlexDMDActor*)obj->m_pBottomText);

   return obj;
}

void FlexDMDTwoLineScene::SetText(string topText, string bottomText)
{
   m_pTopText->SetText(topText);
   m_pBottomText->SetText(bottomText);
}

void FlexDMDTwoLineScene::Update(float delta)
{
   FlexDMDBackgroundScene::Update(delta);
   m_pTopText->SetPosition((GetWidth() - m_pTopText->GetWidth()) / 2.0f, 4.0f);
   m_pBottomText->SetPosition((GetWidth() - m_pBottomText->GetWidth()) / 2.0f, 15.0f);
}