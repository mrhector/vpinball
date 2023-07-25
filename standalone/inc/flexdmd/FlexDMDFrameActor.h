#pragma once

#include "flexdmd_i.h"
#include "FlexDMD.h"
#include "FlexDMDActor.h"

class FlexDMDFrameActor :
   public IDispatchImpl<IFrameActor, &IID_IFrameActor, &LIBID_FlexDMD>,
   public CComObjectRoot,
   public CComCoClass<FlexDMDFrameActor, &CLSID_FlexDMD>,
   public FlexDMDActor
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

   STDMETHOD(get_Name)(BSTR *pRetVal) { return FlexDMDActor::get_Name(pRetVal); }
   STDMETHOD(put_Name)(BSTR pRetVal) { return FlexDMDActor::put_Name(pRetVal); };
   STDMETHOD(get_x)(single *pRetVal) { return FlexDMDActor::get_x(pRetVal); };
   STDMETHOD(put_x)(single pRetVal) { return FlexDMDActor::put_x(pRetVal); };
   STDMETHOD(get_y)(single *pRetVal) { return FlexDMDActor::get_y(pRetVal); };
   STDMETHOD(put_y)(single pRetVal) { return FlexDMDActor::put_y(pRetVal); };
   STDMETHOD(get_Width)(single *pRetVal) { return FlexDMDActor::get_Width(pRetVal); };
   STDMETHOD(put_Width)(single pRetVal) { return FlexDMDActor::put_Width(pRetVal); };
   STDMETHOD(get_Height)(single *pRetVal) { return FlexDMDActor::get_Height(pRetVal); };
   STDMETHOD(put_Height)(single pRetVal) { return FlexDMDActor::put_Height(pRetVal); };
   STDMETHOD(get_visible)(VARIANT_BOOL *pRetVal) { return FlexDMDActor::get_visible(pRetVal); };
   STDMETHOD(put_visible)(VARIANT_BOOL pRetVal) { return FlexDMDActor::put_visible(pRetVal); };
   STDMETHOD(get_FillParent)(VARIANT_BOOL *pRetVal) { return FlexDMDActor::get_FillParent(pRetVal); };
   STDMETHOD(put_FillParent)(VARIANT_BOOL pRetVal) { return FlexDMDActor::put_FillParent(pRetVal); };
   STDMETHOD(get_ClearBackground)(VARIANT_BOOL *pRetVal) { return FlexDMDActor::get_ClearBackground(pRetVal); };
   STDMETHOD(put_ClearBackground)(VARIANT_BOOL pRetVal) { return FlexDMDActor::put_ClearBackground(pRetVal); };
   STDMETHOD(SetBounds)(single x, single y, single Width, single Height) { return FlexDMDActor::SetBounds(x, y, Width, Height); };
   STDMETHOD(SetPosition)(single x, single y) { return FlexDMDActor::SetPosition(x, y); };
   STDMETHOD(SetAlignedPosition)(single x, single y, Alignment alignment) { return FlexDMDActor::SetAlignedPosition(x, y, alignment); };
   STDMETHOD(SetSize)(single Width, single Height) { return FlexDMDActor::SetSize(Width, Height); };
   STDMETHOD(get_PrefWidth)(single *pRetVal) { return FlexDMDActor::get_PrefWidth(pRetVal); };
   STDMETHOD(get_PrefHeight)(single *pRetVal) { return FlexDMDActor::get_PrefHeight(pRetVal); };
   STDMETHOD(Pack)() { return FlexDMDActor::Pack(); };
   STDMETHOD(Remove)() { return FlexDMDActor::Remove(); };
   STDMETHOD(get_ActionFactory)(IActionFactory **pRetVal) { return FlexDMDActor::get_ActionFactory(pRetVal); };
   STDMETHOD(AddAction)(IUnknown *action) { return FlexDMDActor::AddAction(action); };
   STDMETHOD(ClearActions)() { return FlexDMDActor::ClearActions(); };

   STDMETHOD(get_Thickness)(LONG *pRetVal);
   STDMETHOD(put_Thickness)(LONG pRetVal);
   STDMETHOD(get_BorderColor)(OLE_COLOR *pRetVal);
   STDMETHOD(put_BorderColor)(OLE_COLOR pRetVal);
   STDMETHOD(get_Fill)(VARIANT_BOOL *pRetVal);
   STDMETHOD(put_Fill)(VARIANT_BOOL pRetVal);
   STDMETHOD(get_FillColor)(OLE_COLOR *pRetVal);
   STDMETHOD(put_FillColor)(OLE_COLOR pRetVal);

   static CComObject<FlexDMDFrameActor>* Create(FlexDMD* pFlexDMD, string name);

   virtual void Draw(SDL_Surface* graphics);

private:
   FlexDMD* m_pFlexDMD;
   int m_thickness;
   OLE_COLOR m_borderColor;
   bool m_fill;
   OLE_COLOR m_fillColor;
};