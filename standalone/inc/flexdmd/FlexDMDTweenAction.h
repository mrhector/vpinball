#pragma once
#if !defined(FLEXDMDTWEENACTION_H__INCLUDED_)
#define FLEXDMDTWEENACTION_H__INCLUDED_

#pragma GCC visibility push(hidden)

#include "flexdmd_i.h"

class FlexDMDTweenAction :
   public IDispatchImpl<ITweenAction, &IID_ITweenAction, &LIBID_FlexDMD>,
   public CComObjectRoot,
   public CComCoClass<FlexDMDTweenAction, &CLSID_FlexDMD>
{
public:
   STDMETHOD(GetIDsOfNames)(REFIID /*riid*/, LPOLESTR* rgszNames, UINT cNames, LCID lcid,DISPID* rgDispId);
   STDMETHOD(Invoke)(DISPID dispIdMember, REFIID /*riid*/, LCID lcid, WORD wFlags, DISPPARAMS* pDispParams, VARIANT* pVarResult, EXCEPINFO* pExcepInfo, UINT* puArgErr);
   STDMETHOD(GetDocumentation)(INT index, BSTR *pBstrName, BSTR *pBstrDocString, DWORD *pdwHelpContext, BSTR *pBstrHelpFile);

public:
   FlexDMDTweenAction();
   ~FlexDMDTweenAction();

   BEGIN_COM_MAP(FlexDMDTweenAction)
      COM_INTERFACE_ENTRY(ITweenAction)
      COM_INTERFACE_ENTRY(IDispatch)
   END_COM_MAP()

   STDMETHOD(get_Ease)(Interpolation *pRetVal);
   STDMETHOD(put_Ease)(Interpolation pRetVal);
};

#pragma GCC visibility pop

#endif // !defined(FLEXDMDTWEENACTION_H__INCLUDED_)
