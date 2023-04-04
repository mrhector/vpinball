#include "stdafx.h"
#include "FlexDMDLabelActor.h"
#include "FlexDMDActionFactory.h"

FlexDMDLabelActor::FlexDMDLabelActor()
{
    CComObject<FlexDMDActionFactory>::CreateInstance(&m_pActionFactory);
    m_pActionFactory->AddRef();
}

FlexDMDLabelActor::~FlexDMDLabelActor()
{
    m_pActionFactory->Release();
}

STDMETHODIMP FlexDMDLabelActor::get_Name(BSTR *pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDLabelActor::put_Name(BSTR pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDLabelActor::get_x(single *pRetVal) { return E_NOTIMPL; }

STDMETHODIMP FlexDMDLabelActor::put_x(single pRetVal)
{
    return S_OK;
}

STDMETHODIMP FlexDMDLabelActor::get_y(single *pRetVal) { return E_NOTIMPL; }

STDMETHODIMP FlexDMDLabelActor::put_y(single pRetVal)
{
    return S_OK;
}

STDMETHODIMP FlexDMDLabelActor::get_Width(single *pRetVal) { return E_NOTIMPL; }

STDMETHODIMP FlexDMDLabelActor::put_Width(single pRetVal)
{
    return S_OK;
}

STDMETHODIMP FlexDMDLabelActor::get_Height(single *pRetVal) { return E_NOTIMPL; }

STDMETHODIMP FlexDMDLabelActor::put_Height(single pRetVal)
{
    return S_OK;
}

STDMETHODIMP FlexDMDLabelActor::get_visible(VARIANT_BOOL *pRetVal) { return E_NOTIMPL; }

STDMETHODIMP FlexDMDLabelActor::put_visible(VARIANT_BOOL pRetVal)
{
    return S_OK;
}

STDMETHODIMP FlexDMDLabelActor::get_FillParent(VARIANT_BOOL *pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDLabelActor::put_FillParent(VARIANT_BOOL pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDLabelActor::get_ClearBackground(VARIANT_BOOL *pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDLabelActor::put_ClearBackground(VARIANT_BOOL pRetVal) { return E_NOTIMPL; }

STDMETHODIMP FlexDMDLabelActor::SetBounds(single x, single y, single Width, single Height)
{
    return S_OK;
}

STDMETHODIMP FlexDMDLabelActor::SetPosition(single x, single y)
{
    return S_OK;
}

STDMETHODIMP FlexDMDLabelActor::SetAlignedPosition(single x, single y, Alignment alignment)
{
    return S_OK;
}

STDMETHODIMP FlexDMDLabelActor::SetSize(single Width, single Height)
{
    return S_OK;
}

STDMETHODIMP FlexDMDLabelActor::get_PrefWidth(single *pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDLabelActor::get_PrefHeight(single *pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDLabelActor::Pack() { return E_NOTIMPL; }

STDMETHODIMP FlexDMDLabelActor::Remove()
{
    return S_OK;
}

STDMETHODIMP FlexDMDLabelActor::get_ActionFactory(IActionFactory **pRetVal)
{
    return m_pActionFactory->QueryInterface(IID_IActionFactory, (void**)pRetVal);
}

STDMETHODIMP FlexDMDLabelActor::AddAction(IUnknown *action) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDLabelActor::ClearActions() { return E_NOTIMPL; }
STDMETHODIMP FlexDMDLabelActor::get_AutoPack(VARIANT_BOOL *pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDLabelActor::put_AutoPack(VARIANT_BOOL pRetVal) { return E_NOTIMPL; }
void FlexDMDLabelActor::Missing36() { }
void FlexDMDLabelActor::Missing37() { }
STDMETHODIMP FlexDMDLabelActor::get_Font(IUnknown **pRetVal) { return E_NOTIMPL; }

STDMETHODIMP FlexDMDLabelActor::putref_Font(IUnknown *pRetVal)
{
    return S_OK;
}

STDMETHODIMP FlexDMDLabelActor::get_Text(BSTR *pRetVal) { return E_NOTIMPL; }

STDMETHODIMP FlexDMDLabelActor::put_Text(BSTR pRetVal)
{
    return S_OK;
}