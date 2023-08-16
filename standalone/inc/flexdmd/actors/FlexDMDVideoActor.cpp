#include "stdafx.h"

#include "FlexDMDVideoActor.h"

FlexDMDVideoActor::FlexDMDVideoActor()
{
   SetVisible(true);

   m_seek = -1;
}

FlexDMDVideoActor::~FlexDMDVideoActor()
{
}

CComObject<FlexDMDVideoActor>* FlexDMDVideoActor::Create(FlexDMD* pFlexDMD, const string& path, const string& name, bool loop)
{
   CComObject<FlexDMDVideoActor>* obj = nullptr;
   CComObject<FlexDMDVideoActor>::CreateInstance(&obj);
   obj->AddRef();

   obj->SetFlexDMD(pFlexDMD);
   obj->SetName(name);
   obj->SetLoop(loop);

   PLOGW.printf("Video not supported %s", path.c_str());

   return obj;
}

void FlexDMDVideoActor::OnStageStateChanged()
{
}

STDMETHODIMP FlexDMDVideoActor::Seek(single posInSeconds)
{
   m_seek = posInSeconds;
   SetTime(posInSeconds);
   //if (_audioReader != null)
   //{
   //   _audioReader.CurrentTime = TimeSpan.FromSeconds(position);
   //}
   //if (_videoReader != null)
   //{
      ReadNextFrame();
      SetTime(GetFrameTime());
   //}

   return S_OK;
}

void FlexDMDVideoActor::Rewind()
{
   FlexDMDAnimatedActor::Rewind();
   Seek(0);
}

void FlexDMDVideoActor::ReadNextFrame()
{
}

void FlexDMDVideoActor::Draw(Graphics* graphics)
{
   if (GetVisible()) {
      float w = 0;
      float h = 0;
      Layout::Scale(GetScaling(), GetPrefWidth(), GetPrefHeight(), GetWidth(), GetHeight(), w, h);
      float x = 0;
      float y = 0;
      Layout::Align(GetAlignment(), w, h, GetWidth(), GetHeight(), x, y);
      //graphics->DrawImage(m_pFrame, (int)(GetX() + x), (int)(GetY() + y), (int)w, (int)h);
   }
}