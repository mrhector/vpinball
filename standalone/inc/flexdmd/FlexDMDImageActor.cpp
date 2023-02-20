#include "stdafx.h"

FlexDMDImageActor::FlexDMDImageActor()
{
    CComObject<Bitmap>::CreateInstance(&m_pBitmap);
    m_pBitmap->AddRef();
}

FlexDMDImageActor::~FlexDMDImageActor()
{
    m_pBitmap->Release();
}

STDMETHODIMP FlexDMDImageActor::get_Name(BSTR *pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDImageActor::put_Name(BSTR pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDImageActor::get_x(single *pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDImageActor::put_x(single pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDImageActor::get_y(single *pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDImageActor::put_y(single pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDImageActor::get_Width(single *pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDImageActor::put_Width(single pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDImageActor::get_Height(single *pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDImageActor::put_Height(single pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDImageActor::get_visible(VARIANT_BOOL *pRetVal) { return E_NOTIMPL; }

STDMETHODIMP FlexDMDImageActor::put_visible(VARIANT_BOOL pRetVal)
{
    return S_OK;
}

STDMETHODIMP FlexDMDImageActor::get_FillParent(VARIANT_BOOL *pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDImageActor::put_FillParent(VARIANT_BOOL pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDImageActor::get_ClearBackground(VARIANT_BOOL *pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDImageActor::put_ClearBackground(VARIANT_BOOL pRetVal) { return E_NOTIMPL; }

STDMETHODIMP FlexDMDImageActor::SetBounds(single x, single y, single Width, single Height)
{
    return S_OK;
}

STDMETHODIMP FlexDMDImageActor::SetPosition(single x, single y)
{
    return S_OK; 
}

STDMETHODIMP FlexDMDImageActor::SetAlignedPosition(single x, single y, Alignment alignment) { return E_NOTIMPL; }

STDMETHODIMP FlexDMDImageActor::SetSize(single Width, single Height)
{
    return S_OK;
}

STDMETHODIMP FlexDMDImageActor::get_PrefWidth(single *pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDImageActor::get_PrefHeight(single *pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDImageActor::Pack() { return E_NOTIMPL; }
STDMETHODIMP FlexDMDImageActor::Remove() { return E_NOTIMPL; }
STDMETHODIMP FlexDMDImageActor::get_ActionFactory(IActionFactory **pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDImageActor::AddAction(IUnknown *action) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDImageActor::ClearActions() { return E_NOTIMPL; }

STDMETHODIMP FlexDMDImageActor::get_Bitmap(_Bitmap **pRetVal)
{
    return m_pBitmap->QueryInterface(IID__Bitmap, (void**)pRetVal);
}

STDMETHODIMP FlexDMDImageActor::putref_Bitmap(_Bitmap *pRetVal)
{
    return S_OK;
}