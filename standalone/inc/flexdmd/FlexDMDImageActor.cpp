#include "stdafx.h"
#include "FlexDMDImageActor.h"
#include "FlexDMDActionFactory.h"

FlexDMDImageActor::FlexDMDImageActor()
{
}

FlexDMDImageActor::~FlexDMDImageActor()
{
}

CComObject<FlexDMDImageActor>* FlexDMDImageActor::Create(FlexDMD* pFlexDMD, string name, string image)
{
    string path = g_pvp->m_currentTablePath + pFlexDMD->GetProjectFolder() + image;

    SDL_Surface* pSurface = IMG_Load(path.c_str());

    if (pSurface) {
        CComObject<FlexDMDImageActor>* obj = nullptr;
        CComObject<FlexDMDImageActor>::CreateInstance(&obj);
        obj->AddRef();

        obj->m_pFlexDMD = pFlexDMD;
        obj->m_szName = name;
        obj->m_szImage = image;

        obj->m_prefWidth = pSurface->w;
        obj->m_prefHeight = pSurface->h;

        return obj;
    }

    return NULL;
}

STDMETHODIMP FlexDMDImageActor::get_Bitmap(_Bitmap **pRetVal)
{
    return S_OK; //return m_pBitmap->QueryInterface(IID__Bitmap, (void**)pRetVal);
}

STDMETHODIMP FlexDMDImageActor::putref_Bitmap(_Bitmap *pRetVal)
{
    return S_OK;
}

void FlexDMDImageActor::Draw(SDL_Surface* pSurface)
{
    if (m_visible && m_pSurface) {
        SDL_Rect srcRect = { 0, 0, m_pSurface->w, m_pSurface->h };
        SDL_Rect dstRect = { 0, 0, pSurface->w, pSurface->h };
        SDL_BlitScaled(m_pSurface, &srcRect, pSurface, &dstRect);
    }
}