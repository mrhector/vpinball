#include "stdafx.h"
#include "AddToAction.h"

AddToAction::AddToAction(FlexDMDActor* pTarget, FlexDMDGroupActor* pParent, bool add)
{
   m_pTarget = pTarget;
   m_pParent = pParent;
   m_add = add;
}

bool AddToAction::Update(float secondsElapsed) 
{
   if (m_add)
      m_pParent->AddActor(m_pTarget);
   else
      m_pParent->RemoveActor(m_pTarget);

   return true;
}
