#include "stdafx.h"
#include "FlexDMDFrameActor.h"
#include "FlexDMDActionFactory.h"

FlexDMDFrameActor::FlexDMDFrameActor()
{
    CComObject<FlexDMDActionFactory>::CreateInstance(&m_pActionFactory);
    m_pActionFactory->AddRef();
}

FlexDMDFrameActor::~FlexDMDFrameActor()
{
    m_pActionFactory->Release();
}

STDMETHODIMP FlexDMDFrameActor::get_Name(BSTR *pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDFrameActor::put_Name(BSTR pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDFrameActor::get_x(single *pRetVal) { return E_NOTIMPL; }

STDMETHODIMP FlexDMDFrameActor::put_x(single pRetVal)
{
    return S_OK;
}

STDMETHODIMP FlexDMDFrameActor::get_y(single *pRetVal) { return E_NOTIMPL; }

STDMETHODIMP FlexDMDFrameActor::put_y(single pRetVal)
{
    return S_OK;
}

STDMETHODIMP FlexDMDFrameActor::get_Width(single *pRetVal) { return E_NOTIMPL; }

STDMETHODIMP FlexDMDFrameActor::put_Width(single pRetVal)
{
    return S_OK;
}
STDMETHODIMP FlexDMDFrameActor::get_Height(single *pRetVal) { return E_NOTIMPL; }

STDMETHODIMP FlexDMDFrameActor::put_Height(single pRetVal)
{
    return S_OK;
}

STDMETHODIMP FlexDMDFrameActor::get_visible(VARIANT_BOOL *pRetVal) { return E_NOTIMPL; }

STDMETHODIMP FlexDMDFrameActor::put_visible(VARIANT_BOOL pRetVal)
{
    return S_OK;
}

STDMETHODIMP FlexDMDFrameActor::get_FillParent(VARIANT_BOOL *pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDFrameActor::put_FillParent(VARIANT_BOOL pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDFrameActor::get_ClearBackground(VARIANT_BOOL *pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDFrameActor::put_ClearBackground(VARIANT_BOOL pRetVal) { return E_NOTIMPL; }

STDMETHODIMP FlexDMDFrameActor::SetBounds(single x, single y, single Width, single Height)
{
    return S_OK;
}

STDMETHODIMP FlexDMDFrameActor::SetPosition(single x, single y)
{
    return S_OK;
}

STDMETHODIMP FlexDMDFrameActor::SetAlignedPosition(single x, single y, Alignment alignment) { return E_NOTIMPL; }

STDMETHODIMP FlexDMDFrameActor::SetSize(single Width, single Height)
{
    return S_OK;
}

STDMETHODIMP FlexDMDFrameActor::get_PrefWidth(single *pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDFrameActor::get_PrefHeight(single *pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDFrameActor::Pack() { return E_NOTIMPL; }
STDMETHODIMP FlexDMDFrameActor::Remove() { return E_NOTIMPL; }

STDMETHODIMP FlexDMDFrameActor::get_ActionFactory(IActionFactory **pRetVal)
{
    return m_pActionFactory->QueryInterface(IID_IActionFactory, (void**)pRetVal);
}

STDMETHODIMP FlexDMDFrameActor::AddAction(IUnknown *action) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDFrameActor::ClearActions() { return E_NOTIMPL; }
STDMETHODIMP FlexDMDFrameActor::get_Thickness(LONG *pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDFrameActor::put_Thickness(LONG pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDFrameActor::get_BorderColor(OLE_COLOR *pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDFrameActor::put_BorderColor(OLE_COLOR pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDFrameActor::get_Fill(VARIANT_BOOL *pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDFrameActor::put_Fill(VARIANT_BOOL pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDFrameActor::get_FillColor(OLE_COLOR *pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDFrameActor::put_FillColor(OLE_COLOR pRetVal) { return E_NOTIMPL; }