#include "stdafx.h"

FlexDMD::FlexDMD()
{
    CComObject<FlexDMDGroupActor>::CreateInstance(&m_pStage);
    m_pStage->AddRef();
}

FlexDMD::~FlexDMD()
{
    m_pStage->Release();
}

STDMETHODIMP FlexDMD::get_Version(LONG *pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMD::get_RuntimeVersion(LONG *pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMD::put_RuntimeVersion(LONG pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMD::get_Run(VARIANT_BOOL *pRetVal) { return E_NOTIMPL; }

STDMETHODIMP FlexDMD::put_Run(VARIANT_BOOL pRetVal)
{
    return S_OK;
}

STDMETHODIMP FlexDMD::get_Show(VARIANT_BOOL *pRetVal) { return E_NOTIMPL; }

STDMETHODIMP FlexDMD::put_Show(VARIANT_BOOL pRetVal)
{
    return S_OK; 
}

STDMETHODIMP FlexDMD::get_GameName(BSTR *pRetVal)
{
    CComBSTR Val(m_szGameName);
    *pRetVal = Val.Detach();

    return S_OK;
}

STDMETHODIMP FlexDMD::put_GameName(BSTR pRetVal)
{
    WideCharToMultiByte(CP_ACP, 0, pRetVal, -1, m_szGameName, sizeof(m_szGameName), NULL, NULL);
    PLOGI.printf("pRetVal=%s", m_szGameName);

    return S_OK;
}

STDMETHODIMP FlexDMD::get_Width(unsigned short *pRetVal) { return E_NOTIMPL; }

STDMETHODIMP FlexDMD::put_Width(unsigned short pRetVal)
{
    return S_OK;
}

STDMETHODIMP FlexDMD::get_Height(unsigned short *pRetVal) { return E_NOTIMPL; }

STDMETHODIMP FlexDMD::put_Height(unsigned short pRetVal)
{
    return S_OK;
}

STDMETHODIMP FlexDMD::get_Color(OLE_COLOR *pRetVal) { return E_NOTIMPL; }

STDMETHODIMP FlexDMD::put_Color(OLE_COLOR pRetVal)
{
    return S_OK;
}

STDMETHODIMP FlexDMD::get_RenderMode(RenderMode *pRetVal) { return E_NOTIMPL; }

STDMETHODIMP FlexDMD::put_RenderMode(RenderMode pRetVal)
{
    return S_OK;
}

STDMETHODIMP FlexDMD::get_ProjectFolder(BSTR *pRetVal) { return E_NOTIMPL; }

STDMETHODIMP FlexDMD::put_ProjectFolder(BSTR pRetVal)
{
    return S_OK;
}

STDMETHODIMP FlexDMD::get_TableFile(BSTR *pRetVal)
{
    CComBSTR Val(m_szTableFile);
    *pRetVal = Val.Detach();

    return S_OK;
}

STDMETHODIMP FlexDMD::put_TableFile(BSTR pRetVal)
{
    WideCharToMultiByte(CP_ACP, 0, pRetVal, -1, m_szTableFile, sizeof(m_szTableFile), NULL, NULL);
    PLOGI.printf("pRetVal=%s", m_szTableFile);

    return S_OK;
}

STDMETHODIMP FlexDMD::get_Clear(VARIANT_BOOL *pRetVal) { return E_NOTIMPL; }

STDMETHODIMP FlexDMD::put_Clear(VARIANT_BOOL pRetVal)
{
    return S_OK;
}

STDMETHODIMP FlexDMD::get_DmdColoredPixels(VARIANT *pRetVal) { return E_NOTIMPL; }

STDMETHODIMP FlexDMD::get_DmdPixels(VARIANT *pRetVal)
{
    return S_FALSE;
}

STDMETHODIMP FlexDMD::putref_Segments(VARIANT rhs) { return E_NOTIMPL; }

STDMETHODIMP FlexDMD::get_Stage(IGroupActor **pRetVal)
{
    return m_pStage->QueryInterface(IID_IGroupActor, (void**)pRetVal);
}

STDMETHODIMP FlexDMD::LockRenderThread()
{
    return S_OK;
}

STDMETHODIMP FlexDMD::UnlockRenderThread()
{
    return S_OK;
}

STDMETHODIMP FlexDMD::NewGroup(BSTR Name, IGroupActor **pRetVal)
{
    CComObject<FlexDMDGroupActor>* obj = nullptr;
    CComObject<FlexDMDGroupActor>::CreateInstance(&obj);
    obj->AddRef();

    return obj->QueryInterface(IID_IGroupActor, (void**)pRetVal);
}

STDMETHODIMP FlexDMD::NewFrame(BSTR Name, IFrameActor **pRetVal) { return E_NOTIMPL; }

STDMETHODIMP FlexDMD::NewLabel(BSTR Name, IUnknown *Font,BSTR Text, ILabelActor **pRetVal)
{
    CComObject<FlexDMDLabelActor>* obj = nullptr;
    CComObject<FlexDMDLabelActor>::CreateInstance(&obj);
    obj->AddRef();

    return obj->QueryInterface(IID_ILabelActor, (void**)pRetVal); 
}

STDMETHODIMP FlexDMD::NewVideo(BSTR Name, BSTR video, IVideoActor **pRetVal)
{
    CComObject<FlexDMDVideoActor>* obj = nullptr;
    CComObject<FlexDMDVideoActor>::CreateInstance(&obj);
    obj->AddRef();

    return obj->QueryInterface(IID_IVideoActor, (void**)pRetVal);
}

STDMETHODIMP FlexDMD::NewImage(BSTR Name, BSTR image, IImageActor **pRetVal)
{
    CComObject<FlexDMDImageActor>* obj = nullptr;
    CComObject<FlexDMDImageActor>::CreateInstance(&obj);
    obj->AddRef();

    return obj->QueryInterface(IID_IImageActor, (void**)pRetVal);
}

STDMETHODIMP FlexDMD::NewFont(BSTR Font, OLE_COLOR tint, OLE_COLOR borderTint, LONG borderSize, IUnknown **pRetVal)
{ 
    CComObject<FlexDMD>* obj;
    CComObject<FlexDMD>::CreateInstance(&obj);
    obj->AddRef();

    return obj->QueryInterface(IID_IFlexDMD, (void**)pRetVal);
}

STDMETHODIMP FlexDMD::NewUltraDMD(IUltraDMD **pRetVal) { return E_NOTIMPL; }