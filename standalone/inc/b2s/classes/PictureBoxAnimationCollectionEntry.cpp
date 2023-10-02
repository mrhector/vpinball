#include "stdafx.h"

#include "PictureBoxAnimationCollectionEntry.h"

PictureBoxAnimationCollectionEntry::PictureBoxAnimationCollectionEntry(const string& szAnimationName, int loops, bool playReverse)
{
   m_szAnimationName = szAnimationName;
   m_loops = loops;
   m_playReverse = playReverse;
}
