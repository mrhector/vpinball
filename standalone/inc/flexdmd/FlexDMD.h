#pragma once
#if !defined(FLEXDMD_C_H__INCLUDED_)
#define FLEXDMD_C_H__INCLUDED_

#pragma GCC visibility push(hidden)

#include "flexdmd_i.h"

#include "FlexDMDActionFactory.h"
#include "FlexDMDCompositeAction.h"
#include "FlexDMDTweenAction.h"
#include "FlexDMDActor.h"
#include "FlexDMDFrameActor.h"
#include "FlexDMDGroupActor.h"
#include "FlexDMDImageActor.h"
#include "FlexDMDLabelActor.h"
#include "FlexDMDVideoActor.h"

class FlexDMD :
   public IDispatchImpl<IFlexDMD, &IID_IFlexDMD, &LIBID_FlexDMD>,
   public CComObjectRoot,
   public CComCoClass<FlexDMD, &CLSID_FlexDMD>
{
public:
   STDMETHOD(GetIDsOfNames)(REFIID /*riid*/, LPOLESTR* rgszNames, UINT cNames, LCID lcid,DISPID* rgDispId);
   STDMETHOD(Invoke)(DISPID dispIdMember, REFIID /*riid*/, LCID lcid, WORD wFlags, DISPPARAMS* pDispParams, VARIANT* pVarResult, EXCEPINFO* pExcepInfo, UINT* puArgErr);
   STDMETHOD(GetDocumentation)(INT index, BSTR *pBstrName, BSTR *pBstrDocString, DWORD *pdwHelpContext, BSTR *pBstrHelpFile);

public:
   FlexDMD();
   ~FlexDMD();

   BEGIN_COM_MAP(FlexDMD)
      COM_INTERFACE_ENTRY(IFlexDMD)
      COM_INTERFACE_ENTRY(IDispatch)
   END_COM_MAP()

   STDMETHOD(get_Version)(LONG *pRetVal);
   STDMETHOD(get_RuntimeVersion)(LONG *pRetVal);
   STDMETHOD(put_RuntimeVersion)(LONG pRetVal);
   STDMETHOD(get_Run)(VARIANT_BOOL *pRetVal);
   STDMETHOD(put_Run)(VARIANT_BOOL pRetVal);
   STDMETHOD(get_Show)(VARIANT_BOOL *pRetVal);
   STDMETHOD(put_Show)(VARIANT_BOOL pRetVal);
   STDMETHOD(get_GameName)(BSTR *pRetVal);
   STDMETHOD(put_GameName)(BSTR pRetVal);
   STDMETHOD(get_Width)(unsigned short *pRetVal);
   STDMETHOD(put_Width)(unsigned short pRetVal);
   STDMETHOD(get_Height)(unsigned short *pRetVal);
   STDMETHOD(put_Height)(unsigned short pRetVal);
   STDMETHOD(get_Color)(OLE_COLOR *pRetVal);
   STDMETHOD(put_Color)(OLE_COLOR pRetVal);
   STDMETHOD(get_RenderMode)(RenderMode *pRetVal);
   STDMETHOD(put_RenderMode)(RenderMode pRetVal);
   STDMETHOD(get_ProjectFolder)(BSTR *pRetVal);
   STDMETHOD(put_ProjectFolder)(BSTR pRetVal);
   STDMETHOD(get_TableFile)(BSTR *pRetVal);
   STDMETHOD(put_TableFile)(BSTR pRetVal);
   STDMETHOD(get_Clear)(VARIANT_BOOL *pRetVal);
   STDMETHOD(put_Clear)(VARIANT_BOOL pRetVal);
   STDMETHOD(get_DmdColoredPixels)(VARIANT *pRetVal);
   STDMETHOD(get_DmdPixels)(VARIANT *pRetVal);
   STDMETHOD(putref_Segments)(VARIANT rhs);
   STDMETHOD(get_Stage)(IGroupActor **pRetVal);
   STDMETHOD(LockRenderThread)();
   STDMETHOD(UnlockRenderThread)();
   STDMETHOD(NewGroup)(BSTR Name, IGroupActor **pRetVal);
   STDMETHOD(NewFrame)(BSTR Name, IFrameActor **pRetVal);
   STDMETHOD(NewLabel)(BSTR Name, IUnknown *Font,BSTR Text, ILabelActor **pRetVal);
   STDMETHOD(NewVideo)(BSTR Name, BSTR video, IVideoActor **pRetVal);
   STDMETHOD(NewImage)(BSTR Name, BSTR image, IImageActor **pRetVal);
   STDMETHOD(NewFont)(BSTR Font, OLE_COLOR tint, OLE_COLOR borderTint, LONG borderSize, IUnknown **pRetVal);
   STDMETHOD(NewUltraDMD)(IUltraDMD **pRetVal);

private:
   char m_szGameName[MAXSTRING];
   char m_szTableFile[MAXSTRING];

   CComObject<FlexDMDGroupActor>* m_pStage;
};

#pragma GCC visibility pop

#endif // !defined(FLEXDMD_C_H__INCLUDED_)
