#pragma once
#if !defined(FLEXDMDIMAGEACTOR_H__INCLUDED_)
#define FLEXDMDIMAGEACTOR_H__INCLUDED_

#pragma GCC visibility push(hidden)

#include "flexdmd_i.h"

class Bitmap;

class FlexDMDImageActor :
   public IDispatchImpl<IImageActor, &IID_IImageActor, &LIBID_FlexDMD>,
   public CComObjectRoot,
   public CComCoClass<FlexDMDImageActor, &CLSID_FlexDMD>
{
public:
   STDMETHOD(GetIDsOfNames)(REFIID /*riid*/, LPOLESTR* rgszNames, UINT cNames, LCID lcid,DISPID* rgDispId);
   STDMETHOD(Invoke)(DISPID dispIdMember, REFIID /*riid*/, LCID lcid, WORD wFlags, DISPPARAMS* pDispParams, VARIANT* pVarResult, EXCEPINFO* pExcepInfo, UINT* puArgErr);
   STDMETHOD(GetDocumentation)(INT index, BSTR *pBstrName, BSTR *pBstrDocString, DWORD *pdwHelpContext, BSTR *pBstrHelpFile);

public:
   FlexDMDImageActor();
   ~FlexDMDImageActor();

   BEGIN_COM_MAP(FlexDMDImageActor)
      COM_INTERFACE_ENTRY(IImageActor)
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
   STDMETHOD(get_Bitmap)(_Bitmap **pRetVal);
   STDMETHOD(putref_Bitmap)(_Bitmap *pRetVal);

private:
   CComObject<Bitmap>* m_pBitmap;
   CComObject<FlexDMDActionFactory>* m_pActionFactory;
};

#pragma GCC visibility pop

#endif // !defined(FLEXDMDIMAGEACTOR_H__INCLUDED_)
