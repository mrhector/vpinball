#include "stdafx.h"

#include "PictureBoxAnimationCollection.h"

PictureBoxAnimationCollection::PictureBoxAnimationCollection(bool startTimerAtVPActivate, vector<PictureBoxAnimationCollectionEntry*> entries)
{
   m_startMeAtVPActivate = startTimerAtVPActivate;
   m_entries = entries;
}