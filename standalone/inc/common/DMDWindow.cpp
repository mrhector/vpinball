#include "stdafx.h"

#include "DMDWindow.h"

namespace VP {

DMDWindow::DMDWindow(const std::string& szTitle, int x, int y, int w, int h, int z)
    : VP::Window(szTitle, x, y, w, h, z)
{
   m_pDMD = nullptr;
   m_pVirtualDMD = nullptr;
   m_pitch = 0;
   m_textureID = 0;
   m_vao = 0;
   m_vbo = 0;
   m_shaderProgram = 0;
}

DMDWindow::~DMDWindow()
{
   if (m_textureID)
      glDeleteTextures(1, &m_textureID);

   if (m_vao)
      glDeleteVertexArrays(1, &m_vao);

   if (m_vbo)
      glDeleteBuffers(1, &m_vbo);

   if (m_shaderProgram)
      glDeleteProgram(m_shaderProgram);
}

bool DMDWindow::Init()
{
   if (!Window::Init())
      return false;

   glGenVertexArrays(1, &m_vao);
   glBindVertexArray(m_vao);

   glGenBuffers(1, &m_vbo);
   glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

   float vertices[] = {
      -1.0f, -1.0f, 0.0f,  0.0f, 1.0f,
       1.0f, -1.0f, 0.0f,  1.0f, 1.0f,
       1.0f,  1.0f, 0.0f,  1.0f, 0.0f,
      -1.0f,  1.0f, 0.0f,  0.0f, 0.0f,
   };

   glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
   glEnableVertexAttribArray(0);

   glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
   glEnableVertexAttribArray(1);

   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindVertexArray(0);

#ifndef __OPENGLES__
   const GLchar* vertexShaderSource =
R"glsl(
    #version 330 core
    layout (location = 0) in vec3 position;
    layout (location = 1) in vec2 texCoords;

    out vec2 TexCoords;

    void main()
    {
        gl_Position = vec4(position, 1.0f);
        TexCoords = texCoords;
    }
)glsl";

const GLchar* fragmentShaderSource =
R"glsl(
    #version 330 core
    in vec2 TexCoords;
    out vec4 color;

    uniform sampler2D texture1;

    void main()
    {
        color = texture(texture1, TexCoords);
    }
)glsl";
#else
   const GLchar* vertexShaderSource =
R"glsl(
    #version 300 es
    layout (location = 0) in vec3 position;
    layout (location = 1) in vec2 texCoords;

    out vec2 TexCoords;

    void main()
    {
        gl_Position = vec4(position, 1.0);
        TexCoords = texCoords;
    }
)glsl";

const GLchar* fragmentShaderSource =
R"glsl(
    #version 300 es
    precision mediump float;
    in vec2 TexCoords;
    out vec4 color;

    uniform sampler2D texture1;

    void main()
    {
        color = texture(texture1, TexCoords);
    }
)glsl";
#endif

   GLint success;
   GLchar infoLog[512];

   GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
   glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
   glCompileShader(vertexShader);

   glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
   if (!success) {
      glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
      PLOGE.printf("Unable to compile vertex shader\n%s", infoLog);
   }

   GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
   glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
   glCompileShader(fragmentShader);

   glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
   if (!success) {
      glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
      PLOGE.printf("Unable to compile fragment shader\n%s", infoLog);
   }

   m_shaderProgram = glCreateProgram();
   glAttachShader(m_shaderProgram, vertexShader);
   glAttachShader(m_shaderProgram, fragmentShader);
   glLinkProgram(m_shaderProgram);

   glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &success);
   if (!success) {
      glGetProgramInfoLog(m_shaderProgram, 512, NULL, infoLog);
      PLOGE.printf("Unable to link shader program\n%s", infoLog);
   }

   glDeleteShader(vertexShader);
   glDeleteShader(fragmentShader);

   return true;
}

void DMDWindow::SetDMD(DMDUtil::DMD* pDMD)
{
   if (m_pDMD) {
      if (m_pVirtualDMD) {
         m_pDMD->DestroyVirtualDMD(m_pVirtualDMD);
         m_pVirtualDMD = nullptr;
      }
      if (m_textureID) {
         glDeleteTextures(1, &m_textureID);
         m_textureID = 0;
      }
   }

   if (pDMD) {
      m_pVirtualDMD = pDMD->CreateVirtualDMD();
      m_pitch = m_pVirtualDMD->GetPitch();
   }

   m_pDMD = pDMD;
}

void DMDWindow::Render()
{
   const UINT8* pRGB24Data = m_pVirtualDMD ? m_pVirtualDMD->GetRGB24Data() : nullptr;

   if (!pRGB24Data)
      return;

   SDL_GL_MakeCurrent(m_pWindow, m_glContext);

   if (!m_textureID) {
      glGenTextures(1, &m_textureID);
      glBindTexture(GL_TEXTURE_2D, m_textureID);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_pVirtualDMD->GetWidth(), m_pVirtualDMD->GetHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, pRGB24Data);

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glBindTexture(GL_TEXTURE_2D, 0);
   }
   else {
      glBindTexture(GL_TEXTURE_2D, m_textureID);
      glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_pVirtualDMD->GetWidth(), m_pVirtualDMD->GetHeight(), GL_RGB, GL_UNSIGNED_BYTE, pRGB24Data);
      glBindTexture(GL_TEXTURE_2D, 0);
   }

   glUseProgram(m_shaderProgram);

   glBindTexture(GL_TEXTURE_2D, m_textureID);
   glUniform1i(glGetUniformLocation(m_shaderProgram, "texture1"), 0);

   glBindVertexArray(m_vao);
   glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
   glBindVertexArray(0);

   SDL_GL_SwapWindow(m_pWindow);
}

}