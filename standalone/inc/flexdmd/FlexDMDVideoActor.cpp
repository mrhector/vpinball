#include "stdafx.h"
#include "FlexDMDVideoActor.h"
#include <cmath>

FlexDMDVideoActor::FlexDMDVideoActor()
{
   SetVisible(true);

   m_seek = -1;
   m_pFrame = NULL;
}

FlexDMDVideoActor::~FlexDMDVideoActor()
{
}

CComObject<FlexDMDVideoActor>* FlexDMDVideoActor::Create(FlexDMD* pFlexDMD, string path, string name, bool loop)
{
   CComObject<FlexDMDVideoActor>* obj = nullptr;
   CComObject<FlexDMDVideoActor>::CreateInstance(&obj);
   obj->AddRef();

   obj->m_szPath = path;
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

void FlexDMDVideoActor::Draw(SDL_Surface* graphics)
{
   if (GetVisible() && m_pFrame != NULL) {
      float w = 0;
      float h = 0;
      Layout::Scale(GetScaling(), GetPrefWidth(), GetPrefHeight(), GetWidth(), GetHeight(), w, h);
      float x = 0;
      float y = 0;
      Layout::Align(GetAlignment(), w, h, GetWidth(), GetHeight(), x, y);
      SDL_Rect srcRect = { 0, 0, m_pFrame->w, m_pFrame->h };
      SDL_Rect dstRect = { (int)(GetX() + x), (int)(GetY() + y), (int)w, (int)h };
      SDL_BlitScaled(m_pFrame, &srcRect, graphics, &dstRect);
   }
}