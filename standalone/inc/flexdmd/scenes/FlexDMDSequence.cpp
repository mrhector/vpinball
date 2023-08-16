#include "stdafx.h"

#include "FlexDMDSequence.h"

FlexDMDSequence::FlexDMDSequence()
{
   m_finished = false;
   m_pActiveScene = NULL;
}

FlexDMDSequence::~FlexDMDSequence()
{
}

CComObject<FlexDMDSequence>* FlexDMDSequence::Create(FlexDMD* pFlexDMD, const string& name)
{
   CComObject<FlexDMDSequence>* obj = nullptr;
   CComObject<FlexDMDSequence>::CreateInstance(&obj);
   obj->AddRef();

   obj->SetFlexDMD(pFlexDMD);
   obj->SetName(name);

   return obj;
}

void FlexDMDSequence::Enqueue(FlexDMDScene* scene)
{
   m_pendingScenes.push_back(scene);
   m_finished = false;
}

void FlexDMDSequence::RemoveAllScenes()
{
   if (m_pActiveScene)
      m_pActiveScene->Remove();

   m_pActiveScene = NULL;
   m_pendingScenes.clear();
   m_finished = true;
}

void FlexDMDSequence::RemoveScene(string name)
{
   if (m_pActiveScene->GetName() == name) {
      m_pActiveScene->Remove();
      m_pActiveScene = NULL;
   }
   m_pendingScenes.erase(std::remove_if(m_pendingScenes.begin(), m_pendingScenes.end(), [name](FlexDMDGroupActor* p) {
      return p->GetName() == name;
   }), m_pendingScenes.end());
   m_finished = m_pActiveScene == NULL && m_pendingScenes.size() == 0;
}

bool FlexDMDSequence::IsFinished()
{
   return m_finished;
}

void FlexDMDSequence::Update(float delta)
{
   FlexDMDGroupActor::Update(delta);
   if (m_pActiveScene != NULL && m_pActiveScene->IsFinished()) {
      m_pActiveScene->Remove();
      m_pActiveScene = NULL;
   }
   if (m_pActiveScene == NULL && m_pendingScenes.size() > 0) {
      m_pActiveScene = m_pendingScenes[0];
      m_pendingScenes.erase(m_pendingScenes.begin());
      AddActor((FlexDMDActor*)m_pActiveScene);
      m_pActiveScene->Update(0);
   }
   m_finished = m_pActiveScene == NULL && m_pendingScenes.size() == 0;
}

void FlexDMDSequence::Draw(Graphics* graphics)
{
   if (GetVisible() && m_pActiveScene != NULL) {
      graphics->Clear(RGB(0, 0, 0));
      FlexDMDGroupActor::Draw(graphics);
   }
}