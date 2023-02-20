#include "stdafx.h"

FlexDMDVideoActor::FlexDMDVideoActor()
{
}

FlexDMDVideoActor::~FlexDMDVideoActor()
{
}

STDMETHODIMP FlexDMDVideoActor::get_Name(BSTR *pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDVideoActor::put_Name(BSTR pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDVideoActor::get_x(single *pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDVideoActor::put_x(single pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDVideoActor::get_y(single *pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDVideoActor::put_y(single pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDVideoActor::get_Width(single *pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDVideoActor::put_Width(single pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDVideoActor::get_Height(single *pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDVideoActor::put_Height(single pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDVideoActor::get_visible(VARIANT_BOOL *pRetVal) { return E_NOTIMPL; }

STDMETHODIMP FlexDMDVideoActor::put_visible(VARIANT_BOOL pRetVal)
{
    return S_OK;
}

STDMETHODIMP FlexDMDVideoActor::get_FillParent(VARIANT_BOOL *pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDVideoActor::put_FillParent(VARIANT_BOOL pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDVideoActor::get_ClearBackground(VARIANT_BOOL *pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDVideoActor::put_ClearBackground(VARIANT_BOOL pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDVideoActor::SetBounds(single x, single y, single Width, single Height) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDVideoActor::SetPosition(single x, single y) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDVideoActor::SetAlignedPosition(single x, single y, Alignment alignment) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDVideoActor::SetSize(single Width, single Height) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDVideoActor::get_PrefWidth(single *pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDVideoActor::get_PrefHeight(single *pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDVideoActor::Pack() { return E_NOTIMPL; }

STDMETHODIMP FlexDMDVideoActor::Remove()
{
    return S_OK;
}

STDMETHODIMP FlexDMDVideoActor::get_ActionFactory(IActionFactory **pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDVideoActor::AddAction(IUnknown *action) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDVideoActor::ClearActions() { return E_NOTIMPL; }
void FlexDMDVideoActor::Missing34() { };
void FlexDMDVideoActor::Missing35() { };
void FlexDMDVideoActor::Missing36() { };
void FlexDMDVideoActor::Missing37() { };
STDMETHODIMP FlexDMDVideoActor::get_Length(single *pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDVideoActor::get_Loop(VARIANT_BOOL *pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDVideoActor::put_Loop(VARIANT_BOOL pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDVideoActor::get_Paused(VARIANT_BOOL *pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDVideoActor::put_Paused(VARIANT_BOOL pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDVideoActor::get_PlaySpeed(single *pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDVideoActor::put_PlaySpeed(single pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDVideoActor::Seek(single posInSeconds) { return E_NOTIMPL; }