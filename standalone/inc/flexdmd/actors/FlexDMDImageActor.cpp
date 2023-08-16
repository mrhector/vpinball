#include "stdafx.h"

#include "FlexDMDImageActor.h"
#include "../FlexDMDBitmap.h"

FlexDMDImageActor::FlexDMDImageActor()
{
   m_alignment = Alignment_Center;
   m_scaling = Scaling_Stretch;
   m_pAssetManager = NULL;
   m_pSrc = NULL;
   m_pBitmap = NULL;
}

FlexDMDImageActor::~FlexDMDImageActor()
{
   if (m_pSrc)
      delete m_pSrc;
}

CComObject<FlexDMDImageActor>* FlexDMDImageActor::Create(FlexDMD* pFlexDMD, AssetManager* pAssetManager, const string& image, const string& name)
{
   CComObject<FlexDMDImageActor>* obj = nullptr;
   CComObject<FlexDMDImageActor>::CreateInstance(&obj);
   obj->AddRef();

   obj->SetFlexDMD(pFlexDMD);
   obj->m_pSrc = pAssetManager->ResolveSrc(image, NULL);
   obj->m_pAssetManager = pAssetManager;
   obj->SetName(name);

   obj->m_pBitmap = pAssetManager->GetBitmap(obj->m_pSrc);
   obj->SetPrefWidth(obj->m_pBitmap->GetWidth());
   obj->SetPrefHeight(obj->m_pBitmap->GetHeight());
   obj->Pack();
   obj->m_pBitmap = NULL;

   return obj;
}

STDMETHODIMP FlexDMDImageActor::get_Bitmap(_Bitmap **pRetVal)
{
   FlexDMDBitmap* obj = FlexDMDBitmap::Create(m_pBitmap ? m_pBitmap : m_pAssetManager->GetBitmap(m_pSrc));
   return obj->QueryInterface(IID_FlexDMDBitmapInterface, (void**)pRetVal);
}

STDMETHODIMP FlexDMDImageActor::putref_Bitmap(_Bitmap *pRetVal)
{
   FlexDMDBitmap* obj = (FlexDMDBitmap*)pRetVal;
   m_pBitmap = obj->GetCachedBitmap();

   return S_OK;
}

void FlexDMDImageActor::Draw(Graphics* graphics)
{
   if (!m_pBitmap)
      return;

   if (GetVisible() && m_pBitmap->GetSurface()) {
      float w = 0;
      float h = 0;
      Layout::Scale(m_scaling, GetPrefWidth(), GetPrefHeight(), GetWidth(), GetHeight(), w, h);
      float x = 0;
      float y = 0;
      Layout::Align(m_alignment, w, h, GetWidth(), GetHeight(), x, y);
      graphics->DrawImage(m_pBitmap->GetSurface(), (int)(GetX() + x), (int)(GetY() + y), (int)w, (int)h);
   }
}

void FlexDMDImageActor::OnStageStateChanged()
{
   m_pBitmap = GetOnStage() ? m_pAssetManager->GetBitmap(m_pSrc) : NULL;
}