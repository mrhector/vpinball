#include "stdafx.h"

#include "FlexDMDScrollingCreditsScene.h"
#include "../actors/FlexDMDLabelActor.h"

FlexDMDScrollingCreditsScene::FlexDMDScrollingCreditsScene()
{
   m_pContainer = NULL;
}

FlexDMDScrollingCreditsScene::~FlexDMDScrollingCreditsScene()
{
}      
      
CComObject<FlexDMDScrollingCreditsScene>* FlexDMDScrollingCreditsScene::Create(FlexDMD* pFlexDMD, FlexDMDActor* pBackground, vector<string> text, Font* pFont, AnimationType animateIn, float pauseS, AnimationType animateOut, const string& id)
{
   CComObject<FlexDMDScrollingCreditsScene>* obj = nullptr;
   CComObject<FlexDMDScrollingCreditsScene>::CreateInstance(&obj);
   obj->AddRef();

   obj->SetFlexDMD(pFlexDMD);
   obj->SetName(id);

   obj->m_pContainer = FlexDMDGroupActor::Create(pFlexDMD, "");
   obj->m_length = 3.0f + text.size() * 0.4f;
   obj->AddActor((FlexDMDActor*)obj->m_pContainer);

   float y = 0.0f;

   for (const auto& line : text) {
      string txt = trim_string(line);
      if (txt.length() == 0) 
         txt = " ";
       FlexDMDLabelActor* pLabel = FlexDMDLabelActor::Create(pFlexDMD, pFont, txt, "");
       pLabel->SetY(y);
       y += pLabel->GetHeight();
       obj->m_pContainer->AddActor((FlexDMDActor*)pLabel);
   }
   obj->m_pContainer->SetHeight(y);

   return obj;
}

void FlexDMDScrollingCreditsScene::Begin()
{
   FlexDMDBackgroundScene::Begin();
   m_pContainer->SetY(GetHeight());
   AddTween(GetY(), -m_pContainer->GetHeight(), m_length, ([this](float newValue) {
      this->SetY(newValue);
      return false;
   }));
}

void FlexDMDScrollingCreditsScene::Update(float delta)
{
   FlexDMDBackgroundScene::Update(delta);
   if (m_pContainer->GetWidth() != GetWidth()) {
      m_pContainer->SetWidth(GetWidth());
      for (const auto& line : m_pContainer->GetChildren()) {
         FlexDMDLabelActor* label = (FlexDMDLabelActor*)line;
         label->SetX((GetWidth() - line->GetWidth()) / 2);
      }
   }
}