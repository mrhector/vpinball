#include "stdafx.h"

#include "FlexDMDGIFImage.h"

FlexDMDGIFImage::FlexDMDGIFImage()
{
   m_pos = 0;
   m_pAssetManager = NULL;
   m_pSrc = NULL;
   m_pBitmap = NULL;
   m_pActiveFrameSurface = NULL;
}

FlexDMDGIFImage::~FlexDMDGIFImage()
{
   if (m_pSrc)
      delete m_pSrc;
}

CComObject<FlexDMDGIFImage>* FlexDMDGIFImage::Create(FlexDMD* pFlexDMD, AssetManager* pAssetManager, const string& path, const string& name)
{
   CComObject<FlexDMDGIFImage>* obj = nullptr;
   CComObject<FlexDMDGIFImage>::CreateInstance(&obj);
   obj->AddRef();

   obj->SetFlexDMD(pFlexDMD);
   obj->m_pSrc = pAssetManager->ResolveSrc(path, NULL);
   obj->m_pAssetManager = pAssetManager;
   obj->SetName(name);

   obj->m_pBitmap = pAssetManager->GetBitmap(obj->m_pSrc);
   obj->SetPrefWidth(obj->m_pBitmap->GetWidth());
   obj->SetPrefHeight(obj->m_pBitmap->GetHeight());
   obj->SetLength(obj->m_pBitmap->GetLength() / 1000.0f);

   obj->Rewind();
   obj->Pack();

   obj->m_pBitmap = NULL;

   return obj;
}

void FlexDMDGIFImage::OnStageStateChanged()
{
   m_pBitmap = GetOnStage() ? m_pAssetManager->GetBitmap(m_pSrc) : NULL;
   UpdateFrame();
}

void FlexDMDGIFImage::Rewind()
{
   FlexDMDAnimatedActor::Rewind();
   m_pos = 0;

   if (m_pBitmap)
      SetFrameDuration(m_pBitmap->GetFrameDelay(0) / 1000.0f);
}

void FlexDMDGIFImage::ReadNextFrame()
{
   if (!m_pBitmap)
      return;

   if (m_pos >= m_pBitmap->GetFrameCount() - 1)
      SetEndOfAnimation(true);
   else {
      m_pos++;
      SetFrameTime(0);
      for (int i = 0; i < m_pos; i++)
         SetFrameTime(GetFrameTime() + (m_pBitmap->GetFrameDelay(i) / 1000.0f));
        
      SetFrameDuration(m_pBitmap->GetFrameDelay(m_pos) / 1000.0f);
      UpdateFrame();
   }
}

void FlexDMDGIFImage::UpdateFrame() 
{
   if (m_pBitmap)
      m_pActiveFrameSurface = m_pBitmap->GetFrameSurface(m_pos);
}

void FlexDMDGIFImage::Draw(Graphics* graphics)
{
   if (!m_pBitmap)
      return;

   if (GetVisible() && m_pActiveFrameSurface) {
      float w = 0;
      float h = 0;
      Layout::Scale(GetScaling(), GetPrefWidth(), GetPrefHeight(), GetWidth(), GetHeight(), w, h);
      float x = 0;
      float y = 0;
      Layout::Align(GetAlignment(), w, h, GetWidth(), GetHeight(), x, y);
      graphics->DrawImage(m_pActiveFrameSurface, (int)(GetX() + x), (int)(GetY() + y), (int)w, (int)h);
   }
}