#include "stdafx.h"
#include "FlexDMDBitmap.h"

FlexDMDBitmap::FlexDMDBitmap()
{
   m_pSurface = NULL;
}

FlexDMDBitmap::~FlexDMDBitmap()
{
   if (m_pSurface)
      SDL_FreeSurface(m_pSurface);
}

FlexDMDBitmap* FlexDMDBitmap::Create(FlexDMD* pFlexDMD, SDL_Surface* pSurface)
{
   FlexDMDBitmap* obj = new FlexDMDBitmap();
   obj->AddRef();

   obj->m_pFlexDMD = pFlexDMD;
   obj->m_pSurface = pSurface;

   return obj;
}