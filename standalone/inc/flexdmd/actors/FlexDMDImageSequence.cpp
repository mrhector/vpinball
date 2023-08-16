#include "stdafx.h"

#include "FlexDMDImageSequence.h"

FlexDMDImageSequence::FlexDMDImageSequence()
{
}

FlexDMDImageSequence::~FlexDMDImageSequence()
{
}
   
CComObject<FlexDMDImageSequence>* FlexDMDImageSequence::Create(FlexDMD* pFlexDMD, AssetManager* pAssetManager, const string& paths, const string& name, int fps, bool loop)
{
   CComObject<FlexDMDImageSequence>* obj = nullptr;
   CComObject<FlexDMDImageSequence>::CreateInstance(&obj);
   obj->AddRef();

   obj->SetFlexDMD(pFlexDMD);
   obj->m_fps = fps;
   obj->SetLoop(loop);
   obj->SetName(name);

   std::stringstream ss(paths);
   string path;
   while (std::getline(ss, path, '|'))
      obj->m_frames.push_back(FlexDMDImageActor::Create(pFlexDMD, pAssetManager, path, ""));

   obj->m_frame = 0;
   obj->SetFrameDuration(1.0f / fps);
   obj->Pack();

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

void FlexDMDImageSequence::Draw(Graphics* graphics)
{
   FlexDMDAnimatedActor::Draw(graphics);
   if (GetVisible()) {
      m_frames[m_frame]->SetScaling(GetScaling());
      m_frames[m_frame]->SetAlignment(GetAlignment());
      m_frames[m_frame]->SetBounds(GetX(), GetY(), GetWidth(), GetHeight());
      m_frames[m_frame]->Draw(graphics);
   }
}