#pragma once

#include "system_drawing_common_i.h"

#include "Bitmap.h"

class Bitmap :
   public IDispatchImpl<_Bitmap, &IID__Bitmap, &LIBID_System_Drawing_Common>,
   public CComObjectRoot,
   public CComCoClass<Bitmap, &CLSID_Bitmap>
{
public:
   STDMETHOD(GetIDsOfNames)(REFIID /*riid*/, LPOLESTR* rgszNames, UINT cNames, LCID lcid,DISPID* rgDispId);
   STDMETHOD(Invoke)(DISPID dispIdMember, REFIID /*riid*/, LCID lcid, WORD wFlags, DISPPARAMS* pDispParams, VARIANT* pVarResult, EXCEPINFO* pExcepInfo, UINT* puArgErr);
   STDMETHOD(GetDocumentation)(INT index, BSTR *pBstrName, BSTR *pBstrDocString, DWORD *pdwHelpContext, BSTR *pBstrHelpFile);

public:
   Bitmap();
   ~Bitmap();

   BEGIN_COM_MAP(Bitmap)
      COM_INTERFACE_ENTRY(_Bitmap)
      COM_INTERFACE_ENTRY(IDispatch)
   END_COM_MAP()

};