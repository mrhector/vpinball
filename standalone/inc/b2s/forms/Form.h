#pragma once

#include "../controls/Control.h"
#include "../graphics/Graphics.h"

class Form : public Control
{
public:
   Form();
   ~Form();

   void Show();
   void Hide();
   void SetTopMost(bool topMost) { m_topMost = topMost; }
   bool IsTopMost() const { return m_topMost; }

   void Render(SDL_Renderer* pRenderer);

private:
   B2S::Graphics* m_pGraphics;
   bool m_topMost;
};
