#pragma once

#include "RendererGraphics.h"
#include "Timer.h"

#include "DMDUtil/DMDUtil.h"

namespace VP {

class Window
{
public:
   Window(const string& szTitle, int x, int y, int w, int h, int z);
   ~Window();

   virtual bool Init();
   Uint32 GetId() { return m_id; }
   int GetZ() { return m_z; }
   void Show();
   void Hide();
   void OnUpdate();
   void OnRender();
   virtual void Render() { };

private:
   string m_szTitle;
   int m_id;
   int m_x;
   int m_y;
   int m_w;
   int m_h;
   int m_z;
   bool m_visible;
   bool m_init;

protected:
   SDL_Window* m_pWindow;
   SDL_GLContext m_glContext;
   SDL_Renderer* m_pRenderer;
};

}
