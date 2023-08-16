#include "stdafx.h"

#include "FlexDMDLabelActor.h"

#include <sstream>

FlexDMDLabelActor::FlexDMDLabelActor()
{
   m_pFont = NULL;
   m_autopack = false;
   m_alignment = Alignment_Center;
}

FlexDMDLabelActor::~FlexDMDLabelActor()
{
}

CComObject<FlexDMDLabelActor>* FlexDMDLabelActor::Create(FlexDMD* pFlexDMD, Font* pFont, const string& text, const string& name)
{
   CComObject<FlexDMDLabelActor>* obj = nullptr;
   CComObject<FlexDMDLabelActor>::CreateInstance(&obj);
   obj->AddRef();

   obj->SetFlexDMD(pFlexDMD);
   obj->SetName(name);

   obj->m_pFont = pFont;

   obj->m_autopack = pFlexDMD->GetRuntimeVersion() <= 1008;

   obj->SetText(text);
   obj->Pack();

   return obj;
}

STDMETHODIMP FlexDMDLabelActor::get_AutoPack(VARIANT_BOOL *pRetVal)
{
   *pRetVal = m_autopack ? VARIANT_TRUE : VARIANT_FALSE;

   return S_OK;
}

STDMETHODIMP FlexDMDLabelActor::put_AutoPack(VARIANT_BOOL pRetVal)
{
   m_autopack = (pRetVal == VARIANT_TRUE);

   return S_OK;
}

void FlexDMDLabelActor::Missing36() { }
void FlexDMDLabelActor::Missing37() { }

STDMETHODIMP FlexDMDLabelActor::get_Font(IUnknown **pRetVal)
{
   FlexDMDFont* obj = FlexDMDFont::Create(m_pFont);
   return obj->QueryInterface(IID_FlexDMDFontInterface, (void**)pRetVal);
}

STDMETHODIMP FlexDMDLabelActor::putref_Font(IUnknown *pRetVal)
{
   FlexDMDFont* obj = (FlexDMDFont*)pRetVal;
   m_pFont = obj->GetFont();

   UpdateBounds();

   return S_OK;
}

STDMETHODIMP FlexDMDLabelActor::get_Text(BSTR *pRetVal)
{
   CComBSTR Val(m_szText.c_str());
   *pRetVal = Val.Detach();

   return S_OK;
}

STDMETHODIMP FlexDMDLabelActor::put_Text(BSTR pRetVal)
{
   SetText(MakeString(pRetVal));

   return S_OK;
}

void FlexDMDLabelActor::SetText(const string& szText)
{
   string szTmp = szText;

   static string patterns[] = { "\r\n" };
   static string replaceWith = "\n";

   for (const auto& pattern : patterns) {
      size_t pos = 0;
      while((pos = szTmp.find(pattern, pos)) != string::npos) {
         szTmp.replace(pos, pattern.size(), replaceWith);
         pos += replaceWith.size();
      }
   }

   if (m_szText != szTmp) {
      m_szText = szTmp;
      m_lines.clear();
      std::stringstream ss(m_szText);
      string line;
      while (std::getline(ss, line, '\n'))
         m_lines.push_back(line);
      UpdateBounds();
   }
}

void FlexDMDLabelActor::UpdateBounds()
{
   if (!m_pFont)
      return;

   SDL_Rect size = m_pFont->MeasureFont(m_szText);
   m_textWidth = size.w;
   m_textHeight = size.h;

   if (m_autopack)
      Pack();
}

void FlexDMDLabelActor::Draw(Graphics* graphics)
{
   FlexDMDActor::Draw(graphics);
   if (GetVisible() && m_pFont) {
      if (m_lines.size() > 1 && m_alignment != Alignment_Left && m_alignment != Alignment_BottomLeft && m_alignment != Alignment_TopLeft) {
         float x = 0;
         float y = 0;
         Layout::Align(m_alignment, GetPrefWidth(), GetPrefHeight(), GetWidth(), GetHeight(), x, y);

         for (const string& line : m_lines) {
            float lx = 0;
            float ly = 0;
            Layout::Align(m_alignment, m_pFont->MeasureFont(line).w, GetPrefHeight(), GetWidth(), GetHeight(), lx, ly);
            if (GetFlexDMD()->GetRuntimeVersion() <= 1008)
               m_pFont->DrawText_(graphics, (int)(GetX() + lx), (int)(GetY() + y), line);
            else
               m_pFont->DrawText_(graphics, (int)floor(GetX() + lx), (int)floor(GetY() + y), line);
            y += m_pFont->GetBitmapFont()->GetLineHeight();
         }
      }
      else {
         float x = 0;
         float y = 0;
         Layout::Align(m_alignment, GetPrefWidth(), GetPrefHeight(), GetWidth(), GetHeight(), x, y);
         if (GetFlexDMD()->GetRuntimeVersion() <= 1008)
            m_pFont->DrawText_(graphics, (int)(GetX() + x), (int)(GetY() + y), m_szText);
         else
            m_pFont->DrawText_(graphics, (int)floor(GetX() + x), (int)floor(GetY() + y), m_szText);
      }
   }
}