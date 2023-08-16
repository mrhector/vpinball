#pragma once

#include "FlexDMDActor.h"
#include "../FlexDMDBitmap.h"
#include "../AssetManager.h"

class FlexDMDImageActor :
   public IDispatchImpl<IImageActor, &IID_IImageActor, &LIBID_FlexDMD>,
   public CComObjectRoot,
   public CComCoClass<FlexDMDImageActor, &CLSID_FlexDMD>,
   public FlexDMDActor
{
public:
   STDMETHOD(GetIDsOfNames)(REFIID /*riid*/, LPOLESTR* rgszNames, UINT cNames, LCID lcid,DISPID* rgDispId);
   STDMETHOD(Invoke)(DISPID dispIdMember, REFIID /*riid*/, LCID lcid, WORD wFlags, DISPPARAMS* pDispParams, VARIANT* pVarResult, EXCEPINFO* pExcepInfo, UINT* puArgErr);
   STDMETHOD(GetDocumentation)(INT index, BSTR *pBstrName, BSTR *pBstrDocString, DWORD *pdwHelpContext, BSTR *pBstrHelpFile);

public:
   FlexDMDImageActor();
   ~FlexDMDImageActor();

   static CComObject<FlexDMDImageActor>* Create(FlexDMD* pFlexDMD, AssetManager* pAssetManager, const string& image, const string& name);

   BEGIN_COM_MAP(FlexDMDImageActor)
      COM_INTERFACE_ENTRY(IImageActor)
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

   STDMETHOD(get_Bitmap)(_Bitmap **pRetVal);
   STDMETHOD(putref_Bitmap)(_Bitmap *pRetVal);

   void SetScaling(Scaling scaling) { m_scaling = scaling; }
   void SetAlignment(Alignment alignment) { m_alignment = alignment; }

   virtual void Draw(Graphics* graphics);
   virtual void OnStageStateChanged();

private:
   Scaling m_scaling;
   Alignment m_alignment;
   AssetManager* m_pAssetManager;
   AssetSrc* m_pSrc;
   Bitmap* m_pBitmap;
};
