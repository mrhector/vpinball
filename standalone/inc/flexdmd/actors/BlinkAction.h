#pragma once

#include "Action.h"
#include "FlexDMDActor.h"

class BlinkAction : public Action
{
public:
   BlinkAction(FlexDMDActor* pTarget, float secondsShow, float secondsHide, int repeat);
   virtual ~BlinkAction() {};

   virtual bool Update(float secondsElapsed);

private:
   FlexDMDActor* m_pTarget;
   float m_secondsShow;
   float m_secondsHide;
   int m_repeat;

   int m_n;
   float m_time;
};
