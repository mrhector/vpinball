#include "stdafx.h"

FlexDMDActor::FlexDMDActor()
{
    CComObject<FlexDMDActionFactory>::CreateInstance(&m_pActionFactory);
    m_pActionFactory->AddRef();
}

FlexDMDActor::~FlexDMDActor()
{
    m_pActionFactory->Release();
}

STDMETHODIMP FlexDMDActor::get_Name(BSTR *pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDActor::put_Name(BSTR pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDActor::get_x(single *pRetVal) { return E_NOTIMPL; }

STDMETHODIMP FlexDMDActor::put_x(single pRetVal)
{
    return S_OK;
}

STDMETHODIMP FlexDMDActor::get_y(single *pRetVal) { return E_NOTIMPL; }

STDMETHODIMP FlexDMDActor::put_y(single pRetVal)
{
    return S_OK;
}

STDMETHODIMP FlexDMDActor::get_Width(single *pRetVal) { return E_NOTIMPL; }

STDMETHODIMP FlexDMDActor::put_Width(single pRetVal)
{
    return S_OK;
}

STDMETHODIMP FlexDMDActor::get_Height(single *pRetVal) { return E_NOTIMPL; }

STDMETHODIMP FlexDMDActor::put_Height(single pRetVal)
{
    return S_OK;
}

STDMETHODIMP FlexDMDActor::get_visible(VARIANT_BOOL *pRetVal) { return E_NOTIMPL; }

STDMETHODIMP FlexDMDActor::put_visible(VARIANT_BOOL pRetVal)
{
    return S_OK;
}

STDMETHODIMP FlexDMDActor::get_FillParent(VARIANT_BOOL *pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDActor::put_FillParent(VARIANT_BOOL pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDActor::get_ClearBackground(VARIANT_BOOL *pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDActor::put_ClearBackground(VARIANT_BOOL pRetVal) { return E_NOTIMPL; }

STDMETHODIMP FlexDMDActor::SetBounds(single x,single y,single Width,single Height)
{
    return S_OK;
}

STDMETHODIMP FlexDMDActor::SetPosition(single x,single y)
{
    return S_OK;
}

STDMETHODIMP FlexDMDActor::SetAlignedPosition(single x, single y, Alignment alignment) { return E_NOTIMPL; }

STDMETHODIMP FlexDMDActor::SetSize(single Width,single Height)
{
    return S_OK;
}

STDMETHODIMP FlexDMDActor::get_PrefWidth(single *pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDActor::get_PrefHeight(single *pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDActor::Pack() { return E_NOTIMPL; }
STDMETHODIMP FlexDMDActor::Remove() { return E_NOTIMPL; }

STDMETHODIMP FlexDMDActor::get_ActionFactory(IActionFactory **pRetVal)
{
    return m_pActionFactory->QueryInterface(IID_IActionFactory, (void**)pRetVal);
}

STDMETHODIMP FlexDMDActor::AddAction(IUnknown *action) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDActor::ClearActions() { return E_NOTIMPL; }