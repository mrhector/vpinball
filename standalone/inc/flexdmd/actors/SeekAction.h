#pragma once

#include "Action.h"
#include "FlexDMDAnimatedActor.h"

class SeekAction : public Action
{
public:
   SeekAction(FlexDMDAnimatedActor* pTarget, float position);
   virtual ~SeekAction() {};

   virtual bool Update(float secondsElapsed);

private:
   FlexDMDAnimatedActor* m_pTarget;
   float m_position;
};
