#include "stdafx.h"

FlexDMDGroupActor::FlexDMDGroupActor()
{
}

FlexDMDGroupActor::~FlexDMDGroupActor()
{
}

STDMETHODIMP FlexDMDGroupActor::get_Name(BSTR *pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDGroupActor::put_Name(BSTR pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDGroupActor::get_x(single *pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDGroupActor::put_x(single pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDGroupActor::get_y(single *pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDGroupActor::put_y(single pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDGroupActor::get_Width(single *pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDGroupActor::put_Width(single pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDGroupActor::get_Height(single *pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDGroupActor::put_Height(single pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDGroupActor::get_visible(VARIANT_BOOL *pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDGroupActor::put_visible(VARIANT_BOOL pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDGroupActor::get_FillParent(VARIANT_BOOL *pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDGroupActor::put_FillParent(VARIANT_BOOL pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDGroupActor::get_ClearBackground(VARIANT_BOOL *pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDGroupActor::put_ClearBackground(VARIANT_BOOL pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDGroupActor::SetBounds(single x,single y,single Width,single Height) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDGroupActor::SetPosition(single x,single y) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDGroupActor::SetAlignedPosition(single x, single y, Alignment alignment) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDGroupActor::SetSize(single Width,single Height) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDGroupActor::get_PrefWidth(single *pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDGroupActor::get_PrefHeight(single *pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDGroupActor::Pack() { return E_NOTIMPL; }
STDMETHODIMP FlexDMDGroupActor::Remove() { return E_NOTIMPL; }
STDMETHODIMP FlexDMDGroupActor::get_ActionFactory(IActionFactory **pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDGroupActor::AddAction(IUnknown *action) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDGroupActor::ClearActions() { return E_NOTIMPL; }
STDMETHODIMP FlexDMDGroupActor::get_Clip(VARIANT_BOOL *pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDGroupActor::put_Clip(VARIANT_BOOL pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDGroupActor::get_ChildCount(LONG *pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDGroupActor::HasChild(BSTR Name,VARIANT_BOOL *pRetVal) { return E_NOTIMPL; }

STDMETHODIMP FlexDMDGroupActor::GetGroup(BSTR Name,IGroupActor **pRetVal)
{
    CComObject<FlexDMDGroupActor>* obj = nullptr;
    CComObject<FlexDMDGroupActor>::CreateInstance(&obj);
    obj->AddRef();

    return obj->QueryInterface(IID_IGroupActor, (void**)pRetVal); 
}

STDMETHODIMP FlexDMDGroupActor::GetFrame(BSTR Name,IFrameActor **pRetVal) { return E_NOTIMPL; }

STDMETHODIMP FlexDMDGroupActor::GetLabel(BSTR Name,ILabelActor **pRetVal)
{
    CComObject<FlexDMDLabelActor>* obj = nullptr;
    CComObject<FlexDMDLabelActor>::CreateInstance(&obj);
    obj->AddRef();

    return obj->QueryInterface(IID_ILabelActor, (void**)pRetVal); 
}

STDMETHODIMP FlexDMDGroupActor::GetVideo(BSTR Name,IVideoActor **pRetVal)
{
    CComObject<FlexDMDVideoActor>* obj = nullptr;
    CComObject<FlexDMDVideoActor>::CreateInstance(&obj);
    obj->AddRef();

    return obj->QueryInterface(IID_IVideoActor, (void**)pRetVal); 
}

STDMETHODIMP FlexDMDGroupActor::GetImage(BSTR Name,IImageActor **pRetVal)
{
    CComObject<FlexDMDImageActor>* obj = nullptr;
    CComObject<FlexDMDImageActor>::CreateInstance(&obj);
    obj->AddRef();

    return obj->QueryInterface(IID_IImageActor, (void**)pRetVal); 
}

STDMETHODIMP FlexDMDGroupActor::RemoveAll()
{
    return S_OK; 
}

STDMETHODIMP FlexDMDGroupActor::AddActor(IUnknown *child)
{
    return S_OK; 
}

STDMETHODIMP FlexDMDGroupActor::RemoveActor(IUnknown *child) { return E_NOTIMPL; }