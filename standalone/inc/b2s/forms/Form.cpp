#include "stdafx.h"

#include "Form.h"

Form::Form()
{
   m_pGraphics = NULL;
   m_topMost = false;
}

Form::~Form()
{
   if (m_pGraphics)
      delete m_pGraphics;
}

void Form::Show()
{
   if (m_pGraphics)
      delete m_pGraphics;

   m_pGraphics = new B2S::Graphics(GetWidth(), GetHeight());
}

void Form::Hide()
{
   if (m_pGraphics)
      delete m_pGraphics;

   m_pGraphics = NULL;
}

void Form::Render(SDL_Renderer* pRenderer)
{
   if (!m_pGraphics)
      return;

   OnPaint(m_pGraphics);

   SDL_Texture* pTexture = SDL_CreateTextureFromSurface(pRenderer, m_pGraphics->GetSurface());

   if (!pTexture)
      return;

   SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, 255);
   SDL_RenderClear(pRenderer);
   SDL_RenderCopy(pRenderer, pTexture, NULL, NULL);
   SDL_RenderPresent(pRenderer);

   SDL_DestroyTexture(pTexture);
}