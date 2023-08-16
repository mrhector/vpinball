#pragma once

#include "../flexdmd_i.h"

class Action : public IDispatch
{
public:
   Action();
   virtual ~Action() {};

   STDMETHOD(QueryInterface)(REFIID iid, void** ppv) {
      if (iid == IID_ICompositeAction) {
         *ppv = reinterpret_cast<Action*>(this);
         AddRef();
         return S_OK;
      } else if (iid == IID_ITweenAction) {
         *ppv = reinterpret_cast<Action*>(this);
         AddRef();
         return S_OK;
      } else if (iid == IID_IUnknown) {
         *ppv = reinterpret_cast<Action*>(this);
         AddRef();
         return S_OK;
      } else {
         *ppv = nullptr;
         return E_NOINTERFACE;
      }
   }

   STDMETHOD_(ULONG, AddRef)() {
      m_dwRef++;
      return m_dwRef;
   }

   STDMETHOD_(ULONG, Release)() {
      m_dwRef--;

      //if (m_dwRef == 0)
         //delete this;

      return m_dwRef;
   }

   STDMETHOD(GetTypeInfoCount)(UINT *pCountTypeInfo) { *pCountTypeInfo = 0; return S_OK;  }
   STDMETHOD(GetTypeInfo)(UINT iInfo, LCID lcid, ITypeInfo **ppTInfo) { return E_NOTIMPL; }
   STDMETHOD(GetIDsOfNames)(REFIID /*riid*/, LPOLESTR* rgszNames, UINT cNames, LCID lcid,DISPID* rgDispId) { return E_NOTIMPL; }
   STDMETHOD(Invoke)(DISPID dispIdMember, REFIID /*riid*/, LCID lcid, WORD wFlags, DISPPARAMS* pDispParams, VARIANT* pVarResult, EXCEPINFO* pExcepInfo, UINT* puArgErr) { return E_NOTIMPL; }

   virtual bool Update(float secondsElapsed) = 0;

private:
   LONG m_dwRef = 0;
};
