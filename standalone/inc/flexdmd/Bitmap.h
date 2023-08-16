#pragma once

#include "flexdmd_i.h"

#include "AssetSrc.h"
#include <SDL2/SDL_image.h>

class Bitmap
{
public:
   Bitmap(void* pData, AssetType assetType);
   Bitmap(Bitmap* pCachedBitmap);
   ~Bitmap();

   int GetWidth();
   int GetHeight();
   float GetLength();
   float GetFrameDelay(int pos);
   int GetFrameCount();
   SDL_Surface* GetSurface();
   SDL_Surface* GetFrameSurface(int pos);
   void SetData(void* pData);

private:
   AssetType m_assetType;
   void* m_pData;
};
