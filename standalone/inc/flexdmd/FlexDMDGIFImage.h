#pragma once

#include "FlexDMDAnimatedActor.h"
#include <SDL2/SDL_image.h>

class FlexDMDGIFImage : public FlexDMDAnimatedActor 
{
public:
   FlexDMDGIFImage();
   ~FlexDMDGIFImage();

   static CComObject<FlexDMDGIFImage>* Create(FlexDMD* pFlexDMD, string video, string name);

   virtual void Rewind();
   virtual void ReadNextFrame();
   virtual void UpdateFrame();
   virtual void Draw(SDL_Surface* graphics);
   virtual void OnStageStateChanged();

private:
   int m_pos;
   IMG_Animation* m_pAnimation;
   SDL_Surface* m_pSurface;
   bool m_bitmap;
   string m_szVideo;
};