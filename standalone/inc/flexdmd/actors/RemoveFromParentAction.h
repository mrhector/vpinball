#pragma once

#include "Action.h"
#include "FlexDMDActor.h"

class RemoveFromParentAction : public Action
{
public:
   RemoveFromParentAction(FlexDMDActor* pTarget);
   virtual ~RemoveFromParentAction() {};

   virtual bool Update(float secondsElapsed);

private:
   FlexDMDActor* m_pTarget;
};
