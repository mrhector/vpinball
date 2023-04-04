#include "stdafx.h"
#include "olectl.h"

#include "Bitmap.h"

STDMETHODIMP Bitmap::GetIDsOfNames(REFIID /*riid*/, LPOLESTR* rgszNames, UINT cNames, LCID lcid, DISPID* rgDispId) {
	static struct {
		const WCHAR *name;
		DISPID dispId;
	} namesIdsList[] = {
			{ NULL }
	};

	size_t min = 1, max = ARRAY_SIZE(namesIdsList) - 1, i;
	int r;
	while(min <= max) {
		i = (min + max) / 2;
		r = wcsicmp(namesIdsList[i].name, *rgszNames);
		if(!r) {
			*rgDispId = namesIdsList[i].dispId;
			return S_OK;
		}
		if(r < 0)
		   min = i+1;
		else
		   max = i-1;
	}
	return DISP_E_MEMBERNOTFOUND;
}

STDMETHODIMP Bitmap::Invoke(DISPID dispIdMember, REFIID /*riid*/, LCID lcid, WORD wFlags, DISPPARAMS* pDispParams, VARIANT* pVarResult, EXCEPINFO* pExcepInfo, UINT* puArgErr) {
	int index = pDispParams->cArgs;
	VARIANT res;
	HRESULT hres = DISP_E_UNKNOWNNAME;

	V_VT(&res) = VT_EMPTY;

	switch(dispIdMember) {
		case DISPID_VALUE: {
			if (wFlags == (DISPATCH_METHOD | DISPATCH_PROPERTYGET)) {
				V_VT(&res) = VT_DISPATCH;
				V_DISPATCH(&res) = this;
				hres = S_OK;
			}
			break;
		}
		default:
		break;
	}
	if (hres == S_OK) {
		if (pVarResult)
			*pVarResult = res;
		else
			VariantClear(&res);
	}
	else if (hres != S_FALSE) {
		PLOGI.printf("dispId=%d (0x%08x), wFlags=%d, hres=%d", dispIdMember, dispIdMember, wFlags, hres);
	}
	return hres;
}

STDMETHODIMP Bitmap::GetDocumentation(INT index, BSTR *pBstrName, BSTR *pBstrDocString, DWORD *pdwHelpContext, BSTR *pBstrHelpFile) {
	if (index == MEMBERID_NIL) {
		*pBstrName = SysAllocString(L"Bitmap");
		return S_OK;
	}
	return E_NOTIMPL;
}

