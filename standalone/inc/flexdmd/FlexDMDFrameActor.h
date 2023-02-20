#pragma once
#if !defined(FLEXDMDFRAMEACTOR_H__INCLUDED_)
#define FLEXDMDFRAMEACTOR_H__INCLUDED_

#pragma GCC visibility push(hidden)

#include "flexdmd_i.h"

class FlexDMDFrameActor :
   public IDispatchImpl<IFrameActor, &IID_IFrameActor, &LIBID_FlexDMD>,
   public CComObjectRoot,
   public CComCoClass<FlexDMDFrameActor, &CLSID_FlexDMD>
{
public:
   STDMETHOD(GetIDsOfNames)(REFIID /*riid*/, LPOLESTR* rgszNames, UINT cNames, LCID lcid,DISPID* rgDispId);
   STDMETHOD(Invoke)(DISPID dispIdMember, REFIID /*riid*/, LCID lcid, WORD wFlags, DISPPARAMS* pDispParams, VARIANT* pVarResult, EXCEPINFO* pExcepInfo, UINT* puArgErr);
   STDMETHOD(GetDocumentation)(INT index, BSTR *pBstrName, BSTR *pBstrDocString, DWORD *pdwHelpContext, BSTR *pBstrHelpFile);

public:
   FlexDMDFrameActor();
   ~FlexDMDFrameActor();

   BEGIN_COM_MAP(FlexDMDFrameActor)
      COM_INTERFACE_ENTRY(IFrameActor)
      COM_INTERFACE_ENTRY(IDispatch)
   END_COM_MAP()

   STDMETHOD(get_Name)(BSTR *pRetVal);
   STDMETHOD(put_Name)(BSTR pRetVal);
   STDMETHOD(get_x)(single *pRetVal);
   STDMETHOD(put_x)(single pRetVal);
   STDMETHOD(get_y)(single *pRetVal);
   STDMETHOD(put_y)(single pRetVal);
   STDMETHOD(get_Width)(single *pRetVal);
   STDMETHOD(put_Width)(single pRetVal);
   STDMETHOD(get_Height)(single *pRetVal);
   STDMETHOD(put_Height)(single pRetVal);
   STDMETHOD(get_visible)(VARIANT_BOOL *pRetVal);
   STDMETHOD(put_visible)(VARIANT_BOOL pRetVal);
   STDMETHOD(get_FillParent)(VARIANT_BOOL *pRetVal);
   STDMETHOD(put_FillParent)(VARIANT_BOOL pRetVal);
   STDMETHOD(get_ClearBackground)(VARIANT_BOOL *pRetVal);
   STDMETHOD(put_ClearBackground)(VARIANT_BOOL pRetVal);
   STDMETHOD(SetBounds)(single x, single y, single Width, single Height);
   STDMETHOD(SetPosition)(single x, single y);
   STDMETHOD(SetAlignedPosition)(single x, single y, Alignment alignment);
   STDMETHOD(SetSize)(single Width, single Height);
   STDMETHOD(get_PrefWidth)(single *pRetVal);
   STDMETHOD(get_PrefHeight)(single *pRetVal);
   STDMETHOD(Pack)();
   STDMETHOD(Remove)();
   STDMETHOD(get_ActionFactory)(IActionFactory **pRetVal);
   STDMETHOD(AddAction)(IUnknown *action);
   STDMETHOD(ClearActions)();
   STDMETHOD(get_Thickness)(LONG *pRetVal);
   STDMETHOD(put_Thickness)(LONG pRetVal);
   STDMETHOD(get_BorderColor)(OLE_COLOR *pRetVal);
   STDMETHOD(put_BorderColor)(OLE_COLOR pRetVal);
   STDMETHOD(get_Fill)(VARIANT_BOOL *pRetVal);
   STDMETHOD(put_Fill)(VARIANT_BOOL pRetVal);
   STDMETHOD(get_FillColor)(OLE_COLOR *pRetVal);
   STDMETHOD(put_FillColor)(OLE_COLOR pRetVal);
};

#pragma GCC visibility pop

#endif // !defined(FLEXDMDFRAMEACTOR_H__INCLUDED_)
