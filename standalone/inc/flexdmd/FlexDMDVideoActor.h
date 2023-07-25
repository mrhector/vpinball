#pragma once

#include "FlexDMDAnimatedActor.h"
#include "Layout.h"

class FlexDMDVideoActor : public FlexDMDAnimatedActor 
{
public:
   FlexDMDVideoActor();
   ~FlexDMDVideoActor();

   static CComObject<FlexDMDVideoActor>* Create(FlexDMD* pFlexDMD, string path, string name, bool loop);

   STDMETHOD(Seek)(single posInSeconds);

   virtual void SetVisible(bool visible) { FlexDMDAnimatedActor::SetVisible(visible); OnStageStateChanged(); }

   virtual void Rewind();
   virtual void ReadNextFrame();
   virtual void Draw(SDL_Surface* graphics);
   virtual void OnStageStateChanged();

private:
   string m_szPath;
   float m_seek;
   SDL_Surface* m_pFrame;
};