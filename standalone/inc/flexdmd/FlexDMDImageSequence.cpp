#include "stdafx.h"
#include "FlexDMDImageSequence.h"
#include <cmath>

FlexDMDImageSequence::FlexDMDImageSequence()
{
}

FlexDMDImageSequence::~FlexDMDImageSequence()
{
}
   
CComObject<FlexDMDImageSequence>* FlexDMDImageSequence::Create(FlexDMD* pFlexDMD, string paths, string name, int fps, bool loop)
{
   CComObject<FlexDMDImageSequence>* obj = nullptr;
   CComObject<FlexDMDImageSequence>::CreateInstance(&obj);
   obj->AddRef();

   obj->m_pFlexDMD = pFlexDMD;
   obj->SetName(name);
   obj->m_fps = fps;
   obj->SetFrameDuration(1.0f / fps);
   obj->SetLoop(loop);

   std::stringstream ss(paths);
   string path;

   while (std::getline(ss, path, '|'))
      obj->m_frames.push_back(FlexDMDImageActor::Create(pFlexDMD, path, ""));

   return obj;
}

void FlexDMDImageSequence::OnStageStateChanged()
{
    for (FlexDMDImageActor* pFrame : m_frames)
       pFrame->SetOnStage(GetOnStage());
}

void FlexDMDImageSequence::Rewind() 
{
    FlexDMDAnimatedActor::Rewind();
    m_frame = 0;
}

void FlexDMDImageSequence::ReadNextFrame()
{
    if (m_frame == m_frames.size() - 1)
       SetEndOfAnimation(true);
    else {
       m_frame++;
       SetFrameTime(m_frame * GetFrameDuration());
    }
}

void FlexDMDImageSequence::Draw(SDL_Surface* graphics)
{
   FlexDMDAnimatedActor::Draw(graphics);
   if (GetVisible()) {
      m_frames[m_frame]->SetScaling(GetScaling());
      m_frames[m_frame]->SetAlignment(GetAlignment());
      m_frames[m_frame]->SetBounds(GetX(), GetY(), GetWidth(), GetHeight());
      m_frames[m_frame]->Draw(graphics);
   }
}