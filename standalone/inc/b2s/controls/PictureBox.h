#pragma once

#include "../b2s_i.h"

#include "Control.h"

class PictureBox : public Control
{
public:
   PictureBox();

   void OnPaint(B2S::Graphics* pGraphics) override;

   void SetImage(SDL_Surface* pImage) { m_pImage = pImage; }
   SDL_Surface* GetImage() const { return m_pImage; }

private:
   SDL_Surface* m_pImage;
};