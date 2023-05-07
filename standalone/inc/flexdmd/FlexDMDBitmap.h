#pragma once

#include "flexdmd_i.h"

#include "FlexDMD.h"

DEFINE_GUID(IID_FlexDMDBitmapInterface, 0x01234567, 0x89AB, 0xCDEF, 0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF);

class FlexDMDBitmapInterface : public IUnknown {
public:
};

class FlexDMDBitmap : public IUnknown {
public:
    FlexDMDBitmap();
    ~FlexDMDBitmap();

   STDMETHOD(QueryInterface)(REFIID iid, void** ppv) {
       if (iid == IID_FlexDMDBitmapInterface) {
          *ppv = reinterpret_cast<FlexDMDBitmapInterface*>(this);
          AddRef();
          return S_OK;
       } else {
          *ppv = nullptr;
          return E_NOINTERFACE;
       }
   }

   STDMETHOD_(ULONG, AddRef)() { return 1; }
   STDMETHOD_(ULONG, Release)() { return 1; }
   
protected:
};