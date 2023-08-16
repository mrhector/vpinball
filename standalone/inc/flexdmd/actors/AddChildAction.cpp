#include "stdafx.h"
#include "AddChildAction.h"

AddChildAction::AddChildAction(FlexDMDGroupActor* pTarget, FlexDMDActor* pChild, bool add)
{
    m_pTarget = pTarget;
    m_pChild = pChild;
    m_add = add;
}

bool AddChildAction::Update(float secondsElapsed) 
{
   if (m_add)
      m_pTarget->AddActor(m_pChild);
   else
      m_pTarget->RemoveActor(m_pChild);

   return true;
}