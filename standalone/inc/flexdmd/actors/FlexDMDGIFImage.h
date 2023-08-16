#pragma once

#include "FlexDMDAnimatedActor.h"
#include "../AssetManager.h"

class FlexDMDGIFImage : public FlexDMDAnimatedActor 
{
public:
   FlexDMDGIFImage();
   ~FlexDMDGIFImage();

   static CComObject<FlexDMDGIFImage>* Create(FlexDMD* pFlexDMD, AssetManager* pAssetManager, const string& path, const string& name);

   virtual void Rewind();
   virtual void ReadNextFrame();
   virtual void UpdateFrame();
   virtual void Draw(Graphics* graphics);
   virtual void OnStageStateChanged();

private:
   int m_pos;
   AssetManager* m_pAssetManager;
   AssetSrc* m_pSrc;
   Bitmap* m_pBitmap;
   SDL_Surface* m_pActiveFrameSurface;
};