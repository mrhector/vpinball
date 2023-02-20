#pragma once
#if !defined(FLEXDMDGROUPACTOR_H__INCLUDED_)
#define FLEXDMDGROUPACTOR_H__INCLUDED_

#pragma GCC visibility push(hidden)

#include "flexdmd_i.h"

class FlexDMDGroupActor :
   public IDispatchImpl<IGroupActor, &IID_IGroupActor, &LIBID_FlexDMD>,
   public CComObjectRoot,
   public CComCoClass<FlexDMDGroupActor, &CLSID_FlexDMD>
{
public:
   STDMETHOD(GetIDsOfNames)(REFIID /*riid*/, LPOLESTR* rgszNames, UINT cNames, LCID lcid,DISPID* rgDispId);
   STDMETHOD(Invoke)(DISPID dispIdMember, REFIID /*riid*/, LCID lcid, WORD wFlags, DISPPARAMS* pDispParams, VARIANT* pVarResult, EXCEPINFO* pExcepInfo, UINT* puArgErr);
   STDMETHOD(GetDocumentation)(INT index, BSTR *pBstrName, BSTR *pBstrDocString, DWORD *pdwHelpContext, BSTR *pBstrHelpFile);

public:
   FlexDMDGroupActor();
   ~FlexDMDGroupActor();

   BEGIN_COM_MAP(FlexDMDGroupActor)
      COM_INTERFACE_ENTRY(IGroupActor)
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
   STDMETHOD(get_Clip)(VARIANT_BOOL *pRetVal);
   STDMETHOD(put_Clip)(VARIANT_BOOL pRetVal);
   STDMETHOD(get_ChildCount)(LONG *pRetVal);
   STDMETHOD(HasChild)(BSTR Name, VARIANT_BOOL *pRetVal);
   STDMETHOD(GetGroup)(BSTR Name, IGroupActor **pRetVal);
   STDMETHOD(GetFrame)(BSTR Name, IFrameActor **pRetVal);
   STDMETHOD(GetLabel)(BSTR Name, ILabelActor **pRetVal);
   STDMETHOD(GetVideo)(BSTR Name, IVideoActor **pRetVal);
   STDMETHOD(GetImage)(BSTR Name, IImageActor **pRetVal);
   STDMETHOD(RemoveAll)();
   STDMETHOD(AddActor)(IUnknown *child);
   STDMETHOD(RemoveActor)(IUnknown *child);
};

#pragma GCC visibility pop

#endif // !defined(FLEXDMDGROUPACTOR_H__INCLUDED_)
