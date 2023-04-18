#include "stdafx.h"
#include "FlexDMDGIFImage.h"
#include <cmath>

FlexDMDGIFImage::FlexDMDGIFImage()
{
    m_pos = 0;
    m_pAnimation = NULL;
    m_pFrameDelays = NULL;
    m_length = 0;

    m_pFrameSurface = NULL;
}

FlexDMDGIFImage::~FlexDMDGIFImage()
{
    if (m_pAnimation)
       IMG_FreeAnimation(m_pAnimation);

    if (m_pFrameDelays)
       delete[] m_pFrameDelays;
}

CComObject<FlexDMDGIFImage>* FlexDMDGIFImage::Create(FlexDMD* pFlexDMD, string name, string video)
{
    string path = g_pvp->m_currentTablePath + pFlexDMD->GetProjectFolder() + video;

    IMG_Animation* pAnimation = IMG_LoadAnimation(path.c_str());

    if (pAnimation) {
        CComObject<FlexDMDGIFImage>* obj = nullptr;
        CComObject<FlexDMDGIFImage>::CreateInstance(&obj);
        obj->AddRef();

        obj->m_pFlexDMD = pFlexDMD;

        obj->m_szName = name;
        obj->m_szVideo = video;

        obj->m_pAnimation = pAnimation;

        obj->m_prefWidth = pAnimation->w;
        obj->m_prefHeight = pAnimation->h;

        obj->m_pFrameDelays = new float[pAnimation->count];

        for (int index = 0; index < pAnimation->count; index++) {
           obj->m_pFrameDelays[index] = pAnimation->delays[index] / 1000.0f;
           obj->m_length += obj->m_pFrameDelays[index];
        }

        obj->Rewind();
        obj->Pack();

        return obj;
    }
    
    return NULL;
}

void FlexDMDGIFImage::Rewind() 
{
   FlexDMDVideoActor::Rewind();
   m_pos = 0;
   m_frameDuration = m_pFrameDelays[0];
}

void FlexDMDGIFImage::ReadNextFrame()
{
    if (m_pos >= m_pAnimation->count - 1)
        m_endOfAnimation = true;
    else {
        m_pos++;
        m_frameTime = 0;
        for (int i = 0; i < m_pos; i++)
            m_frameTime += m_pFrameDelays[i];
        
        m_frameDuration = m_pFrameDelays[m_pos];
        UpdateFrame();
    }
}

void FlexDMDGIFImage::UpdateFrame() 
{
    m_pFrameSurface = m_pAnimation->frames[m_pos];
}

void FlexDMDGIFImage::Draw(SDL_Surface* pSurface)
{
    if (m_visible && m_pFrameSurface) {
        SDL_Rect srcRect = { 0, 0, m_pFrameSurface->w, m_pFrameSurface->h };
        SDL_Rect dstRect = { 0, 0, pSurface->w, pSurface->h };
        SDL_BlitScaled(m_pFrameSurface, &srcRect, pSurface, &dstRect);
    }
}