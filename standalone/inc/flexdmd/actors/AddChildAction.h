#pragma once

#include "Action.h"
#include "FlexDMDGroupActor.h"

class AddChildAction : public Action
{
public:
   AddChildAction(FlexDMDGroupActor* pTarget, FlexDMDActor* pChild, bool add);
   virtual ~AddChildAction() {};

   virtual bool Update(float secondsElapsed);

private:
   FlexDMDGroupActor* m_pTarget;
   FlexDMDActor* m_pChild;
   bool m_add;
};
