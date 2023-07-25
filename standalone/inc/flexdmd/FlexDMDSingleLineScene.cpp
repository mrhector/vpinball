#include "stdafx.h"
#include "FlexDMDSingleLineScene.h"


FlexDMDSingleLineScene::FlexDMDSingleLineScene()
{
}

FlexDMDSingleLineScene::~FlexDMDSingleLineScene()
{
}

CComObject<FlexDMDSingleLineScene>* FlexDMDSingleLineScene::Create(FlexDMD* pFlexDMD, FlexDMDActor* pBackground, string text, FlexDMDFont* pFont, AnimationType animateIn, float pauseS, AnimationType animateOut, bool scroll, string id)
{
   CComObject<FlexDMDSingleLineScene>* obj = nullptr;
   CComObject<FlexDMDSingleLineScene>::CreateInstance(&obj);
   obj->AddRef();

   obj->m_pFlexDMD = pFlexDMD;
   obj->SetName(id);

   obj->m_pText = FlexDMDLabelActor::Create(pFlexDMD, "", pFont, text);
   obj->SetAnimateIn(animateIn);
   obj->SetPause(pauseS);
   obj->SetAnimateOut(animateOut);
   obj->m_scroll = scroll;

   obj->SetBackground(pBackground);
   obj->AddActor((FlexDMDActor*)obj->m_pText);

   return obj;
}

void FlexDMDSingleLineScene::SetText(string text)
{
   m_pText->SetText(text);
}

void FlexDMDSingleLineScene::Begin()
{
   FlexDMDBackgroundScene::Begin();
   if (m_scroll) {
      // UltraDMD moves text by 1.2 pixel per frame (no delta time) and seems to render based on a frame rate at 60FPS.
      float offset = -(GetWidth() - m_pText->GetWidth()) / 2.f;
      m_scrollX = offset + GetWidth();
      //_tweener.Tween(this, new { m_scrollX = offset - m_pText->GetWidth() }, (GetWidth() + m_pText->GetWidth()) / (60 * 1.2f), 0f);
   }
}

void FlexDMDSingleLineScene::Update(float delta)
{
   FlexDMDBackgroundScene::Update(delta);
   m_pText->SetPosition((GetWidth() - m_pText->GetWidth()) / 2 + m_scrollX, (GetHeight() - m_pText->GetHeight()) / 2);
}