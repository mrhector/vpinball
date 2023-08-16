#include "stdafx.h"
#include "RemoveFromParentAction.h"

RemoveFromParentAction::RemoveFromParentAction(FlexDMDActor* pTarget)
{
   m_pTarget = pTarget;
}

bool RemoveFromParentAction::Update(float secondsElapsed) 
{
   m_pTarget->Remove();
   return true;
}