#pragma once

#include "TweenAction.h"
#include "FlexDMDActor.h"

class MoveToAction : public TweenAction
{
public:
   MoveToAction(FlexDMDActor* pTarget, float x, float y, float duration);
   virtual ~MoveToAction() {};

   void Begin();

private:
   float m_x;
   float m_y;
};
