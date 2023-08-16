#include "stdafx.h"
#include "FlexDMDFont.h"
#include <sstream>

FlexDMDFont::FlexDMDFont()
{
   m_pFont = NULL;
}

FlexDMDFont::~FlexDMDFont()
{
}

FlexDMDFont* FlexDMDFont::Create(Font* pFont)
{
   FlexDMDFont* obj = new FlexDMDFont();
   obj->AddRef();

   obj->m_pFont = pFont;

   return obj;
}