#pragma once

#include "Action.h"
#include "FlexDMDGroupActor.h"

class AddToAction : public Action
{
public:
   AddToAction(FlexDMDActor* pTarget, FlexDMDGroupActor* pParent, bool add);
   virtual ~AddToAction() {};

   virtual bool Update(float secondsElapsed);

private:
   FlexDMDActor* m_pTarget;
   FlexDMDGroupActor* m_pParent;
   bool m_add;
};
