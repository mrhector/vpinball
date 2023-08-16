#include "stdafx.h"

#include "FlexDMDFrameActor.h"

FlexDMDFrameActor::FlexDMDFrameActor()
{
   m_thickness = 2;
   m_borderColor = RGB(255, 255, 255);
   m_fill = false;
   m_fillColor = RGB(0, 0, 0);
}

FlexDMDFrameActor::~FlexDMDFrameActor()
{
}

CComObject<FlexDMDFrameActor>* FlexDMDFrameActor::Create(FlexDMD* pFlexDMD, const string& name)
{
   CComObject<FlexDMDFrameActor>* obj = nullptr;
   CComObject<FlexDMDFrameActor>::CreateInstance(&obj);
   obj->AddRef();

   obj->SetFlexDMD(pFlexDMD);
   obj->SetName(name);

   return obj;
}

STDMETHODIMP FlexDMDFrameActor::get_Thickness(LONG *pRetVal)
{
   *pRetVal = m_thickness;

   return S_OK;
}

STDMETHODIMP FlexDMDFrameActor::put_Thickness(LONG pRetVal)
{
   m_thickness = pRetVal;

   return S_OK;
}

STDMETHODIMP FlexDMDFrameActor::get_BorderColor(OLE_COLOR *pRetVal)
{
   *pRetVal = m_borderColor;

   return S_OK;
}

STDMETHODIMP FlexDMDFrameActor::put_BorderColor(OLE_COLOR pRetVal)
{
   m_borderColor = pRetVal;

   return S_OK;
}

STDMETHODIMP FlexDMDFrameActor::get_Fill(VARIANT_BOOL *pRetVal)
{
   *pRetVal = m_fill ? VARIANT_TRUE : VARIANT_FALSE;

   return S_OK;
}

STDMETHODIMP FlexDMDFrameActor::put_Fill(VARIANT_BOOL pRetVal)
{
   m_fill = (pRetVal == VARIANT_TRUE);

   return S_OK;
}

STDMETHODIMP FlexDMDFrameActor::get_FillColor(OLE_COLOR *pRetVal)
{
   *pRetVal = m_fillColor;

   return S_OK;
}

STDMETHODIMP FlexDMDFrameActor::put_FillColor(OLE_COLOR pRetVal)
{
   m_fillColor = pRetVal;

   return S_OK;
}

void FlexDMDFrameActor::Draw(Graphics* graphics)
{
   FlexDMDActor::Draw(graphics);

   if (GetVisible()) {
      if (m_fill)
         graphics->FillRectangle(m_fillColor, (int)(GetX() + m_thickness), (int)(GetY() + m_thickness), (int)(GetWidth() - 2.0f * m_thickness), (int)(GetHeight() - 2.0f * m_thickness));
      if (m_thickness > 0.0f) {
         graphics->FillRectangle(m_borderColor, (int)GetX(), (int)GetY(), (int)GetWidth(), (int)m_thickness);
         graphics->FillRectangle(m_borderColor, (int)GetX(), (int)(GetY() + GetHeight() - m_thickness), (int)GetWidth(), (int)m_thickness);
         graphics->FillRectangle(m_borderColor, (int)GetX(), (int)(GetY() + m_thickness), (int)m_thickness, (int)(GetHeight() - 2.0f * m_thickness));
         graphics->FillRectangle(m_borderColor, (int)(GetX() + GetWidth() - m_thickness), (int)(GetY() + m_thickness), (int)m_thickness, (int)(GetHeight() - 2.0f * m_thickness));
      }
   }
}