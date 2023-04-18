#pragma once

#include "FlexDMDVideoActor.h"
#include <SDL2/SDL_image.h>

class FlexDMDGIFImage : public FlexDMDVideoActor 
{
public:
   FlexDMDGIFImage();
   ~FlexDMDGIFImage();

   static CComObject<FlexDMDGIFImage>* Create(FlexDMD* pFlexDMD, string name, string video);
   
   virtual void Rewind();
   virtual void ReadNextFrame();
   virtual void UpdateFrame();

   virtual void Draw(SDL_Surface* pSurface);

private:
   int m_pos;
   IMG_Animation* m_pAnimation;
   float* m_pFrameDelays;
   SDL_Surface* m_pFrameSurface;
};