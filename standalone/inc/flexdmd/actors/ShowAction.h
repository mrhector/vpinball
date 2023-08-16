#pragma once

#include "Action.h"
#include "FlexDMDActor.h"

class ShowAction : public Action
{
public:
   ShowAction(FlexDMDActor* pTarget, bool visible);
   virtual ~ShowAction() {};

   virtual bool Update(float secondsElapsed);

private:
   FlexDMDActor* m_pTarget;
   bool m_visible; 
};
