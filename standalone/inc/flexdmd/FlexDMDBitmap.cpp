#include "stdafx.h"
#include "FlexDMDBitmap.h"

FlexDMDBitmap::FlexDMDBitmap()
{
}

FlexDMDBitmap::~FlexDMDBitmap()
{
}

FlexDMDBitmap* FlexDMDBitmap::Create(Bitmap* pCachedBitmap)
{
   FlexDMDBitmap* obj = new FlexDMDBitmap();
   obj->AddRef();

   obj->m_pCachedBitmap = pCachedBitmap;

   return obj;
}