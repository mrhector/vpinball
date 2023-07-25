#include "stdafx.h"
#include "FlexDMDGIFImage.h"
#include <cmath>

FlexDMDGIFImage::FlexDMDGIFImage()
{
   m_pAnimation = NULL;
   m_pos = 0;

   SetLength(0);

   m_pSurface = NULL;
   m_bitmap = false;
}

FlexDMDGIFImage::~FlexDMDGIFImage()
{
   if (m_pAnimation)
      IMG_FreeAnimation(m_pAnimation);
}

CComObject<FlexDMDGIFImage>* FlexDMDGIFImage::Create(FlexDMD* pFlexDMD, string video, string name)
{
   video = normalize_path_separators(video);

   string path = g_pvp->m_currentTablePath + pFlexDMD->GetProjectFolder() + video;

   IMG_Animation* pAnimation = IMG_LoadAnimation(path.c_str());

   if (!pAnimation) {
      PLOGE.printf("%s failed to load: name=%s", video.c_str(), name.c_str());
      return NULL;
   }

   CComObject<FlexDMDGIFImage>* obj = nullptr;
   CComObject<FlexDMDGIFImage>::CreateInstance(&obj);
   obj->AddRef();

   obj->m_pFlexDMD = pFlexDMD;
   obj->SetName(name);

   obj->m_szVideo = video;
   obj->m_pAnimation = pAnimation;
   obj->SetPrefWidth(pAnimation->w);
   obj->SetPrefHeight(pAnimation->h);

   for (int index = 0; index < pAnimation->count; index++)
      obj->SetLength(obj->GetLength() + pAnimation->delays[index] / 1000.f);

   obj->Rewind();
   obj->Pack();

   obj->m_bitmap = false;

   PLOGD.printf("%s loaded: name=%s, w=%d, h=%d, count=%d, length=%.2f",
      video.c_str(), name.c_str(), pAnimation->w, pAnimation->h, pAnimation->count, obj->GetLength());

   return obj;
}

void FlexDMDGIFImage::Rewind() 
{
   if (!m_pAnimation)
      return;

   FlexDMDAnimatedActor::Rewind();
   m_pos = 0;
   SetFrameDuration(m_pAnimation->delays[0] / 1000.0f);
}

void FlexDMDGIFImage::ReadNextFrame()
{
   if (!m_pAnimation)
      return;

   if (m_pos >= m_pAnimation->count - 1)
      SetEndOfAnimation(true);
   else {
      m_pos++;
      SetFrameTime(0);
      for (int i = 0; i < m_pos; i++)
         SetFrameTime(GetFrameTime() + m_pAnimation->delays[i] / 1000.0f);
        
      SetFrameDuration(m_pAnimation->delays[m_pos] / 1000.0f);
      UpdateFrame();
   }
}

void FlexDMDGIFImage::UpdateFrame() 
{
   if (m_pAnimation && m_bitmap)
      m_pSurface = m_pAnimation->frames[m_pos];
}

void FlexDMDGIFImage::Draw(SDL_Surface* graphics)
{
   if (GetVisible() && m_bitmap && m_pSurface) {
      float w = 0;
      float h = 0;
      Layout::Scale(GetScaling(), GetPrefWidth(), GetPrefHeight(), GetWidth(), GetHeight(), w, h);
      float x = 0;
      float y = 0;
      Layout::Align(GetAlignment(), w, h, GetWidth(), GetHeight(), x, y);
      SDL_Rect srcRect = { 0, 0, m_pSurface->w, m_pSurface->h };
      SDL_Rect dstRect = { (int)(GetX() + x), (int)(GetY() + y), (int)w, (int)h };
      SDL_BlitScaled(m_pSurface, &srcRect, graphics, &dstRect);
   }
}

void FlexDMDGIFImage::OnStageStateChanged()
{
   m_bitmap = GetOnStage();
   UpdateFrame();
}