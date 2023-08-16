#include "stdafx.h"
#include "ShowAction.h"

ShowAction::ShowAction(FlexDMDActor* pTarget, bool visible)
{
   m_pTarget = pTarget;
   m_visible = visible;
}

bool ShowAction::Update(float secondsElapsed) 
{
   m_pTarget->SetVisible(m_visible);
   return true;
}
