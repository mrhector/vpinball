#include "stdafx.h"
#include "SeekAction.h"

SeekAction::SeekAction(FlexDMDAnimatedActor* pTarget, float position)
{
   m_pTarget = pTarget;
   m_position = position;
}

bool SeekAction::Update(float secondsElapsed) 
{
   m_pTarget->Seek(m_position);
   return true;
}