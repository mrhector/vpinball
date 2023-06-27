#include "stdafx.h"
#include "FlexDMDImageActor.h"
#include "FlexDMDActionFactory.h"
#include "FlexDMDBitmap.h"

FlexDMDImageActor::FlexDMDImageActor()
{
    m_alignment = Alignment_Center;
    m_scaling = Scaling_Stretch;
    m_bitmap = false;
    m_pBitmap = NULL;
}

FlexDMDImageActor::~FlexDMDImageActor()
{
    if (m_pBitmap)
        m_pBitmap->Release();
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
      string path;

      if (image.starts_with("FlexDMD.Resources.")) {
         path = g_pvp->m_szMyPath + "flexdmd" + PATH_SEPARATOR_CHAR;

         image = image.substr(18);

         if (image.starts_with("dmds.")) {
            image = image.substr(5);

            path += string("dmds") + PATH_SEPARATOR_CHAR;
         }

         path += image;
      }
      else {
         image = normalize_path_separators(image);

         path = g_pvp->m_currentTablePath + pFlexDMD->GetProjectFolder() + image;
      }

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

   obj->m_pBitmap = FlexDMDBitmap::Create(pFlexDMD, pSurface);

   obj->m_prefWidth = obj->m_pBitmap->GetWidth();
   obj->m_prefHeight = obj->m_pBitmap->GetHeight();

   obj->Pack();

   obj->m_bitmap = false;

   PLOGD.printf("%s loaded: name=%s, w=%d, h=%d", image.c_str(), name.c_str(), obj->m_pBitmap->GetWidth(), obj->m_pBitmap->GetHeight());

   return obj;
}

STDMETHODIMP FlexDMDImageActor::get_Bitmap(_Bitmap **pRetVal)
{
   return m_pBitmap->QueryInterface(IID_FlexDMDBitmapInterface, (void**)pRetVal);
}

STDMETHODIMP FlexDMDImageActor::putref_Bitmap(_Bitmap *pRetVal)
{
   if (m_pBitmap)
      m_pBitmap->Release();

   m_pBitmap = (FlexDMDBitmap*)pRetVal;

   if (m_pBitmap)
      m_pBitmap->AddRef();

   return S_OK;
}

void FlexDMDImageActor::Draw(SDL_Surface* graphics)
{
    if (m_visible && m_bitmap && m_pBitmap && m_pBitmap->m_pSurface) {
       float w = 0;
       float h = 0;
       Layout::Scale(m_scaling, m_prefWidth, m_prefHeight, m_width, m_height, w, h);
       float x = 0;
       float y = 0;
       Layout::Align(m_alignment, w, h, m_width, m_height, x, y);
       SDL_Rect srcRect = { 0, 0, m_pBitmap->GetWidth(), m_pBitmap->GetHeight() };
       SDL_Rect dstRect = { (int)(m_x + x), (int)(m_y + y), (int)w, (int)h };
       SDL_BlitScaled(m_pBitmap->m_pSurface, &srcRect, graphics, &dstRect);
    }
}

void FlexDMDImageActor::OnStageStateChanged()
{
   m_bitmap = m_onStage;
}