#pragma once
#if !defined(FLEXDMDVIDEOACTOR_H__INCLUDED_)
#define FLEXDMDVIDEOACTOR_H__INCLUDED_

#pragma GCC visibility push(hidden)

#include "flexdmd_i.h"

class FlexDMDVideoActor :
   public IDispatchImpl<IVideoActor, &IID_IVideoActor, &LIBID_FlexDMD>,
   public CComObjectRoot,
   public CComCoClass<FlexDMDVideoActor, &CLSID_FlexDMD>
{
public:
   STDMETHOD(GetIDsOfNames)(REFIID /*riid*/, LPOLESTR* rgszNames, UINT cNames, LCID lcid,DISPID* rgDispId);
   STDMETHOD(Invoke)(DISPID dispIdMember, REFIID /*riid*/, LCID lcid, WORD wFlags, DISPPARAMS* pDispParams, VARIANT* pVarResult, EXCEPINFO* pExcepInfo, UINT* puArgErr);
   STDMETHOD(GetDocumentation)(INT index, BSTR *pBstrName, BSTR *pBstrDocString, DWORD *pdwHelpContext, BSTR *pBstrHelpFile);

public:
   FlexDMDVideoActor();
   ~FlexDMDVideoActor();

   BEGIN_COM_MAP(FlexDMDVideoActor)
      COM_INTERFACE_ENTRY(IVideoActor)
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
   void Missing34();
   void Missing35();
   void Missing36();
   void Missing37();
   STDMETHOD(get_Length)(single *pRetVal);
   STDMETHOD(get_Loop)(VARIANT_BOOL *pRetVal);
   STDMETHOD(put_Loop)(VARIANT_BOOL pRetVal);
   STDMETHOD(get_Paused)(VARIANT_BOOL *pRetVal);
   STDMETHOD(put_Paused)(VARIANT_BOOL pRetVal);
   STDMETHOD(get_PlaySpeed)(single *pRetVal);
   STDMETHOD(put_PlaySpeed)(single pRetVal);
   STDMETHOD(Seek)(single posInSeconds);
};

#pragma GCC visibility pop

#endif // !defined(FLEXDMDVIDEOACTOR_H__INCLUDED_)
