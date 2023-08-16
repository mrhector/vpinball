#pragma once

#include "FlexDMDAnimatedActor.h"

class FlexDMDVideoActor : public FlexDMDAnimatedActor 
{
public:
   FlexDMDVideoActor();
   ~FlexDMDVideoActor();

   static CComObject<FlexDMDVideoActor>* Create(FlexDMD* pFlexDMD, const string& path, const string& name, bool loop);

   STDMETHOD(Seek)(single posInSeconds);

   virtual void SetVisible(bool visible) { FlexDMDAnimatedActor::SetVisible(visible); OnStageStateChanged(); }

   virtual void Rewind();
   virtual void ReadNextFrame();
   virtual void Draw(Graphics* graphics);
   virtual void OnStageStateChanged();

private:
   float m_seek;
};