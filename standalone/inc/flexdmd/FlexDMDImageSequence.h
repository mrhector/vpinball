#pragma once

#include "FlexDMDAnimatedActor.h"
#include "FlexDMDImageActor.h"

class FlexDMDImageSequence : public FlexDMDAnimatedActor 
{
public:
   FlexDMDImageSequence();
   ~FlexDMDImageSequence();

   static CComObject<FlexDMDImageSequence>* Create(FlexDMD* pFlexDMD, string paths, string name, int fps, bool loop);

   virtual single GetPrefWidth() { return m_frames[0]->GetWidth(); };
   virtual single GetPrefHeight() { return m_frames[0]->GetHeight(); };
   virtual float GetLength() { return m_frames.size() * GetFrameDuration(); };

   virtual void Rewind();
   virtual void ReadNextFrame();
   virtual void Draw(SDL_Surface* graphics);
   virtual void OnStageStateChanged();

private:
   int m_fps;
   int m_frame;
   vector<FlexDMDImageActor*> m_frames;
};