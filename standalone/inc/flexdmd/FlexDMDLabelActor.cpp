#include "stdafx.h"
#include "FlexDMDLabelActor.h"
#include "FlexDMDActionFactory.h"

FlexDMDLabelActor::FlexDMDLabelActor()
{
}

FlexDMDLabelActor::~FlexDMDLabelActor()
{
}

CComObject<FlexDMDLabelActor>* FlexDMDLabelActor::Create(FlexDMD* pFlexDMD, string name, void* pFont, string text)
{
    CComObject<FlexDMDLabelActor>* obj = nullptr;
    CComObject<FlexDMDLabelActor>::CreateInstance(&obj);
    obj->AddRef();

    obj->m_pFlexDMD = pFlexDMD;
    obj->m_szName = name;
    obj->m_pFont = pFont;
    obj->m_szText = text;

    return obj;
}

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