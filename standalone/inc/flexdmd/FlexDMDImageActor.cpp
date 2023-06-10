#include "stdafx.h"
#include "FlexDMDImageActor.h"
#include "FlexDMDActionFactory.h"

FlexDMDImageActor::FlexDMDImageActor()
{
    m_alignment = Alignment_Center;
    m_scaling = Scaling_Stretch;
    m_bitmap = false;
}

FlexDMDImageActor::~FlexDMDImageActor()
{
}

CComObject<FlexDMDImageActor>* FlexDMDImageActor::Create(FlexDMD* pFlexDMD, string name, string image)
{
   SDL_Surface* pSurface = NULL;

   if (image.starts_with("VPX.")) {
      string asset = image.substr(4);

      size_t pos = asset.find_first_of("&");
      if (pos != string::npos) {
         asset = asset.substr(0, pos);
      }

      for (Texture* texturePtr : g_pvp->m_ptableActive->m_vimage) {
         if (asset == texturePtr->m_szName) {
            SDL_RWops* rwops = SDL_RWFromConstMem(texturePtr->m_ppb->m_pdata, texturePtr->m_ppb->m_cdata);
            pSurface = IMG_Load_RW(rwops, 0);
            break;
         }
      }
   }
   else {
      string path = g_pvp->m_currentTablePath + pFlexDMD->GetProjectFolder() + image;
      pSurface = IMG_Load(path.c_str());
   }

   if (!pSurface) {
      PLOGE.printf("%s failed to load: name=%s", image.c_str(), name.c_str());
      return NULL;
   }

   CComObject<FlexDMDImageActor>* obj = nullptr;
   CComObject<FlexDMDImageActor>::CreateInstance(&obj);

   obj->m_pFlexDMD = pFlexDMD;
   obj->m_szName = name;
   obj->m_szImage = image;

   obj->AddRef();

   obj->m_pSurface = pSurface;

   obj->m_prefWidth = pSurface->w;
   obj->m_prefHeight = pSurface->h;

   obj->Pack();

   obj->m_bitmap = false;

   PLOGI.printf("%s loaded: name=%s, w=%d, h=%d", image.c_str(), name.c_str(), pSurface->w, pSurface->h);

   return obj;
}

STDMETHODIMP FlexDMDImageActor::get_Bitmap(_Bitmap **pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDImageActor::putref_Bitmap(_Bitmap *pRetVal) { return E_NOTIMPL; }

void FlexDMDImageActor::Draw(SDL_Surface* graphics)
{
    if (m_visible && m_bitmap && m_pSurface) {
       float w = 0;
       float h = 0;
       Layout::Scale(m_scaling, m_prefWidth, m_prefHeight, m_width, m_height, w, h);
       float x = 0;
       float y = 0;
       Layout::Align(m_alignment, w, h, m_width, m_height, x, y);
       SDL_Rect srcRect = { 0, 0, m_pSurface->w, m_pSurface->h };
       SDL_Rect dstRect = { (int)(m_x + x), (int)(m_y + y), (int)w, (int)h };
       SDL_BlitScaled(m_pSurface, &srcRect, graphics, &dstRect);
    }
}

void FlexDMDImageActor::OnStageStateChanged()
{
   m_bitmap = m_onStage;
}