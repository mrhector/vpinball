#pragma once

#include "flexdmd_i.h"

#include "FlexDMD.h"

DEFINE_GUID(IID_FlexDMDFontInterface, 0x01234567, 0x89AB, 0xCDEF, 0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF);

class FlexDMDFontInterface : public IUnknown {
public:
};

class FlexDMDFont : public IUnknown {
public:
   STDMETHOD(QueryInterface)(REFIID iid, void** ppv) {
       if (iid == IID_FlexDMDFontInterface) {
          *ppv = reinterpret_cast<FlexDMDFontInterface*>(this);
          AddRef();
          return S_OK;
       } else {
          *ppv = nullptr;
          return E_NOINTERFACE;
       }
   }

   STDMETHOD_(ULONG, AddRef)() { return 1; }
   STDMETHOD_(ULONG, Release)() { return 1; }

public:
    FlexDMDFont();
    ~FlexDMDFont();
};