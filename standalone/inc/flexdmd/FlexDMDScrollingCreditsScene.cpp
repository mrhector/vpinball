#include "stdafx.h"
#include "FlexDMDScrollingCreditsScene.h"

FlexDMDScrollingCreditsScene::FlexDMDScrollingCreditsScene()
{
}

FlexDMDScrollingCreditsScene::~FlexDMDScrollingCreditsScene()
{
}      
      
CComObject<FlexDMDScrollingCreditsScene>* FlexDMDScrollingCreditsScene::Create(FlexDMD* pFlexDMD, FlexDMDActor* pBackground, vector<string> text, FlexDMDFont* pFont, AnimationType animateIn, float pauseS, AnimationType animateOut, string id)
{
   CComObject<FlexDMDScrollingCreditsScene>* obj = nullptr;
   CComObject<FlexDMDScrollingCreditsScene>::CreateInstance(&obj);
   obj->AddRef();

   obj->m_pFlexDMD = pFlexDMD;
   obj->SetName(id);

   obj->m_pContainer = FlexDMDGroupActor::Create(pFlexDMD, "");
   obj->m_length = 3.f + text.size() * 0.4f;
   obj->AddActor((FlexDMDActor*)obj->m_pContainer);

   float y = 0.f;

   for (const auto& line : text) {
      string txt = trim_string(line);
      if (txt.length() == 0) 
         txt = " ";
       FlexDMDLabelActor* pLabel = FlexDMDLabelActor::Create(pFlexDMD, "", pFont, txt);
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
   //_tweener.Tween(m_pContainer, new { SetY(-m_pContainer->GetHeight() }, m_length, 0f);
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