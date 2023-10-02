#pragma once

#include "../b2s_i.h"

#include "../classes/PictureBoxAnimationCollectionEntry.h"

class PictureBoxAnimationCollection
{
public:
   PictureBoxAnimationCollection(bool startTimerAtVPActivate, vector<PictureBoxAnimationCollectionEntry*> entries);

   bool IsStartMeAtVPActivate() const { return m_startMeAtVPActivate; }
   vector<PictureBoxAnimationCollectionEntry*>* GetEntries() { return &m_entries; }

private:
   bool m_startMeAtVPActivate;
   vector<PictureBoxAnimationCollectionEntry*> m_entries;
};