#pragma once

#include "../actors/FlexDMDGroupActor.h"
#include "FlexDMDScene.h"

class FlexDMDSequence : public FlexDMDGroupActor
{
public:
   FlexDMDSequence();
   ~FlexDMDSequence();

   static CComObject<FlexDMDSequence>* Create(FlexDMD* pFlexDMD, const string& name);

   void Enqueue(FlexDMDScene* scene);
   void RemoveAllScenes();
   void RemoveScene(string name);
   bool IsFinished();

   FlexDMDScene* GetActiveScene() { return m_pActiveScene; };

   virtual void Update(float delta);
   virtual void Draw(Graphics* graphics);

private:
   vector<FlexDMDScene*> m_pendingScenes;
   bool m_finished;
   FlexDMDScene* m_pActiveScene;
};