#include "stdafx.h"

#include "PictureBox.h"

PictureBox::PictureBox() : Control()
{
}

void PictureBox::OnPaint(B2S::Graphics* pGraphics)
{
   pGraphics->DrawImage(m_pImage, NULL, NULL);
}
   