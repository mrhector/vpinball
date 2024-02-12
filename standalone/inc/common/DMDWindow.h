#pragma once

#include "../common/Window.h"
#include "DMDUtil/DMDUtil.h"

namespace VP {

class DMDWindow : public VP::Window
{
public:
   DMDWindow(const std::string& szTitle, int x, int y, int w, int h, int z);
   ~DMDWindow();

   bool Init() override;

   void SetDMD(DMDUtil::DMD* pDMD);
   void Render() override;

private:
   DMDUtil::DMD* m_pDMD;
   DMDUtil::VirtualDMD* m_pVirtualDMD;
   int m_pitch;
   GLuint m_textureID;
   GLuint m_vao;
   GLuint m_vbo;
   GLuint m_shaderProgram;
};

}