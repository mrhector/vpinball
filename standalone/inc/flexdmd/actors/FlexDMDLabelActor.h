#pragma once

#include "FlexDMDActor.h"
#include "FlexDMDFont.h"

class FlexDMDLabelActor :
   public IDispatchImpl<ILabelActor, &IID_ILabelActor, &LIBID_FlexDMD>,
   public CComObjectRoot,
   public CComCoClass<FlexDMDLabelActor, &CLSID_FlexDMD>,
   public FlexDMDActor
{
public:
   STDMETHOD(GetIDsOfNames)(REFIID /*riid*/, LPOLESTR* rgszNames, UINT cNames, LCID lcid,DISPID* rgDispId);
   STDMETHOD(Invoke)(DISPID dispIdMember, REFIID /*riid*/, LCID lcid, WORD wFlags, DISPPARAMS* pDispParams, VARIANT* pVarResult, EXCEPINFO* pExcepInfo, UINT* puArgErr);
   STDMETHOD(GetDocumentation)(INT index, BSTR *pBstrName, BSTR *pBstrDocString, DWORD *pdwHelpContext, BSTR *pBstrHelpFile);

public:
   FlexDMDLabelActor();
   ~FlexDMDLabelActor();

   static CComObject<FlexDMDLabelActor>* Create(FlexDMD* pFlexDMD, Font* pFont, const string& text, const string& name);

   BEGIN_COM_MAP(FlexDMDLabelActor)
      COM_INTERFACE_ENTRY(ILabelActor)
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

   STDMETHOD(get_AutoPack)(VARIANT_BOOL *pRetVal);
   STDMETHOD(put_AutoPack)(VARIANT_BOOL pRetVal);
   void Missing36();
   void Missing37();
   STDMETHOD(get_Font)(IUnknown **pRetVal);
   STDMETHOD(putref_Font)(IUnknown *pRetVal);
   STDMETHOD(get_Text)(BSTR *pRetVal);
   STDMETHOD(put_Text)(BSTR pRetVal);

   Font* GetFont() { return m_pFont; }
   void SetFont(Font* pFont) { m_pFont = pFont; }
   void SetText(const string& szText);

   virtual single GetPrefWidth() { return m_textWidth; }
   virtual single GetPrefHeight() { return m_textHeight; }

   virtual void Draw(Graphics* graphics);

private:
   void ProcessText();
   void UpdateBounds();

   Font* m_pFont;
   string m_szText;
   bool m_autopack;
   float m_textWidth;
   float m_textHeight;
   Alignment m_alignment;
   vector<string> m_lines;
};