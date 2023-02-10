#include "stdafx.h"
#include "olectl.h"

extern "C" int wcsicmp( LPCWSTR str1, LPCWSTR str2 );

name_disp_id_t FlexDMDCompositeAction::m_nameIDMap[] = {
		{ NULL },
		{ L"Add", 0x60020000 }
};

STDMETHODIMP FlexDMDCompositeAction::GetIDsOfNames(REFIID /*riid*/, LPOLESTR* rgszNames, UINT cNames, LCID lcid, DISPID* rgDispId) {
	size_t min = 1, max = ARRAY_SIZE(m_nameIDMap) - 1, i;
	int r;
	while(min <= max) {
		i = (min + max) / 2;
		r = wcsicmp(m_nameIDMap[i].name, *rgszNames);
		if(!r) {
			*rgDispId = m_nameIDMap[i].dispId;
			return S_OK;
		}
		if(r < 0)
		   min = i+1;
		else
		   max = i-1;
	}
	return DISP_E_MEMBERNOTFOUND;
}

STDMETHODIMP FlexDMDCompositeAction::Invoke(DISPID dispIdMember, REFIID /*riid*/, LCID lcid, WORD wFlags, DISPPARAMS* pDispParams, VARIANT* pVarResult, EXCEPINFO* pExcepInfo, UINT* puArgErr) {
	HRESULT hr = DISP_E_UNKNOWNNAME;
	int index = pDispParams->cArgs;
	switch(dispIdMember) {
		case 0x60020000: {
			if (wFlags & DISPATCH_METHOD) {
				// line 108: [id(0x60020000)]HRESULT Add([in] IUnknown* action,[out, retval] ICompositeAction** pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantCopyInd(&var0, &pDispParams->rgvarg[--index]);
				V_VT(pVarResult) = VT_DISPATCH;
				hr = Add((IUnknown*)&var0, (ICompositeAction**)&V_DISPATCH(pVarResult));
				VariantClear(&var0);
			}
			break;
		}
		default:
		break;
	}
	if (hr != S_OK) {
		if (dispIdMember == DISPID_VALUE && wFlags == (DISPATCH_METHOD | DISPATCH_PROPERTYGET)) {
			V_VT(pVarResult) = VT_DISPATCH;
			V_DISPATCH(pVarResult) = this;
			return S_OK;
		}
		else if (hr != S_FALSE) {
			PLOGI.printf("dispId=%d (0x%08x), wFlags=%d, hr=%d", dispIdMember, dispIdMember, wFlags, hr);
		}
	}
	return hr;
}

STDMETHODIMP FlexDMDCompositeAction::GetDocumentation(INT index, BSTR *pBstrName, BSTR *pBstrDocString, DWORD *pdwHelpContext, BSTR *pBstrHelpFile) {
	if (index == MEMBERID_NIL) {
		*pBstrName = SysAllocString(L"CompositeAction");
		return S_OK;
	}
	return E_NOTIMPL;
}

name_disp_id_t FlexDMDTweenAction::m_nameIDMap[] = {
		{ NULL },
		{ L"Ease", 0x60020000 }
};

STDMETHODIMP FlexDMDTweenAction::GetIDsOfNames(REFIID /*riid*/, LPOLESTR* rgszNames, UINT cNames, LCID lcid, DISPID* rgDispId) {
	size_t min = 1, max = ARRAY_SIZE(m_nameIDMap) - 1, i;
	int r;
	while(min <= max) {
		i = (min + max) / 2;
		r = wcsicmp(m_nameIDMap[i].name, *rgszNames);
		if(!r) {
			*rgDispId = m_nameIDMap[i].dispId;
			return S_OK;
		}
		if(r < 0)
		   min = i+1;
		else
		   max = i-1;
	}
	return DISP_E_MEMBERNOTFOUND;
}

STDMETHODIMP FlexDMDTweenAction::Invoke(DISPID dispIdMember, REFIID /*riid*/, LCID lcid, WORD wFlags, DISPPARAMS* pDispParams, VARIANT* pVarResult, EXCEPINFO* pExcepInfo, UINT* puArgErr) {
	HRESULT hr = DISP_E_UNKNOWNNAME;
	int index = pDispParams->cArgs;
	switch(dispIdMember) {
		case 0x60020000: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 124: [id(0x60020000), propget]HRESULT Ease([out, retval] Interpolation* pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_Ease((Interpolation*)&V_I4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 126: [id(0x60020000), propput]HRESULT Ease([in] Interpolation pRetVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_I4);
				hr = put_Ease((Interpolation)V_I4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		default:
		break;
	}
	if (hr != S_OK) {
		if (dispIdMember == DISPID_VALUE && wFlags == (DISPATCH_METHOD | DISPATCH_PROPERTYGET)) {
			V_VT(pVarResult) = VT_DISPATCH;
			V_DISPATCH(pVarResult) = this;
			return S_OK;
		}
		else if (hr != S_FALSE) {
			PLOGI.printf("dispId=%d (0x%08x), wFlags=%d, hr=%d", dispIdMember, dispIdMember, wFlags, hr);
		}
	}
	return hr;
}

STDMETHODIMP FlexDMDTweenAction::GetDocumentation(INT index, BSTR *pBstrName, BSTR *pBstrDocString, DWORD *pdwHelpContext, BSTR *pBstrHelpFile) {
	if (index == MEMBERID_NIL) {
		*pBstrName = SysAllocString(L"TweenAction");
		return S_OK;
	}
	return E_NOTIMPL;
}

name_disp_id_t FlexDMDActionFactory::m_nameIDMap[] = {
		{ NULL },
		{ L"AddChild", 0x60020009 },
		{ L"AddTo", 0x60020007 },
		{ L"Blink", 0x60020005 },
		{ L"Delayed", 0x60020001 },
		{ L"MoveTo", 0x6002000c },
		{ L"Parallel", 0x60020002 },
		{ L"RemoveChild", 0x6002000a },
		{ L"RemoveFromParent", 0x60020008 },
		{ L"Repeat", 0x60020004 },
		{ L"Seek", 0x6002000b },
		{ L"Sequence", 0x60020003 },
		{ L"Show", 0x60020006 },
		{ L"Wait", 0x60020000 }
};

STDMETHODIMP FlexDMDActionFactory::GetIDsOfNames(REFIID /*riid*/, LPOLESTR* rgszNames, UINT cNames, LCID lcid, DISPID* rgDispId) {
	size_t min = 1, max = ARRAY_SIZE(m_nameIDMap) - 1, i;
	int r;
	while(min <= max) {
		i = (min + max) / 2;
		r = wcsicmp(m_nameIDMap[i].name, *rgszNames);
		if(!r) {
			*rgDispId = m_nameIDMap[i].dispId;
			return S_OK;
		}
		if(r < 0)
		   min = i+1;
		else
		   max = i-1;
	}
	return DISP_E_MEMBERNOTFOUND;
}

STDMETHODIMP FlexDMDActionFactory::Invoke(DISPID dispIdMember, REFIID /*riid*/, LCID lcid, WORD wFlags, DISPPARAMS* pDispParams, VARIANT* pVarResult, EXCEPINFO* pExcepInfo, UINT* puArgErr) {
	HRESULT hr = DISP_E_UNKNOWNNAME;
	int index = pDispParams->cArgs;
	switch(dispIdMember) {
		case 0x60020000: {
			if (wFlags & DISPATCH_METHOD) {
				// line 140: [id(0x60020000)]HRESULT Wait([in] single secondsToWait,[out, retval] IUnknown** pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				V_VT(pVarResult) = VT_UNKNOWN;
				hr = Wait(V_R4(&var0), &V_UNKNOWN(pVarResult));
				VariantClear(&var0);
			}
			break;
		}
		case 0x60020001: {
			if (wFlags & DISPATCH_METHOD) {
				// line 144: [id(0x60020001)]HRESULT Delayed([in] single secondsToWait,[in] IUnknown* action,[out, retval] IUnknown** pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				VARIANT var1;
				V_VT(&var1) = VT_EMPTY;
				VariantCopyInd(&var1, &pDispParams->rgvarg[--index]);
				V_VT(pVarResult) = VT_UNKNOWN;
				hr = Delayed(V_R4(&var0), (IUnknown*)&var1, &V_UNKNOWN(pVarResult));
				VariantClear(&var0);
				VariantClear(&var1);
			}
			break;
		}
		case 0x60020002: {
			if (wFlags & DISPATCH_METHOD) {
				// line 149: [id(0x60020002)]HRESULT Parallel([out, retval] ICompositeAction** pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_DISPATCH;
				hr = Parallel((ICompositeAction**)&V_DISPATCH(pVarResult));
			}
			break;
		}
		case 0x60020003: {
			if (wFlags & DISPATCH_METHOD) {
				// line 151: [id(0x60020003)]HRESULT Sequence([out, retval] ICompositeAction** pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_DISPATCH;
				hr = Sequence((ICompositeAction**)&V_DISPATCH(pVarResult));
			}
			break;
		}
		case 0x60020004: {
			if (wFlags & DISPATCH_METHOD) {
				// line 153: [id(0x60020004)]HRESULT Repeat([in] IUnknown* action,[in] long count,[out, retval] IUnknown** pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantCopyInd(&var0, &pDispParams->rgvarg[--index]);
				VARIANT var1;
				V_VT(&var1) = VT_EMPTY;
				VariantChangeType(&var1, &pDispParams->rgvarg[--index], 0, VT_I4);
				V_VT(pVarResult) = VT_UNKNOWN;
				hr = Repeat((IUnknown*)&var0, V_I4(&var1), &V_UNKNOWN(pVarResult));
				VariantClear(&var0);
				VariantClear(&var1);
			}
			break;
		}
		case 0x60020005: {
			if (wFlags & DISPATCH_METHOD) {
				// line 158: [id(0x60020005)]HRESULT Blink([in] single secondsShow,[in] single secondsHide,[in] long Repeat,[out, retval] IUnknown** pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				VARIANT var1;
				V_VT(&var1) = VT_EMPTY;
				VariantChangeType(&var1, &pDispParams->rgvarg[--index], 0, VT_R4);
				VARIANT var2;
				V_VT(&var2) = VT_EMPTY;
				VariantChangeType(&var2, &pDispParams->rgvarg[--index], 0, VT_I4);
				V_VT(pVarResult) = VT_UNKNOWN;
				hr = Blink(V_R4(&var0), V_R4(&var1), V_I4(&var2), &V_UNKNOWN(pVarResult));
				VariantClear(&var0);
				VariantClear(&var1);
				VariantClear(&var2);
			}
			break;
		}
		case 0x60020006: {
			if (wFlags & DISPATCH_METHOD) {
				// line 164: [id(0x60020006)]HRESULT Show([in] VARIANT_BOOL visible,[out, retval] IUnknown** pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				V_VT(pVarResult) = VT_UNKNOWN;
				hr = Show(V_BOOL(&var0), &V_UNKNOWN(pVarResult));
				VariantClear(&var0);
			}
			break;
		}
		case 0x60020007: {
			if (wFlags & DISPATCH_METHOD) {
				// line 168: [id(0x60020007)]HRESULT AddTo([in] IGroupActor* parent,[out, retval] IUnknown** pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantCopyInd(&var0, &pDispParams->rgvarg[--index]);
				V_VT(pVarResult) = VT_UNKNOWN;
				hr = AddTo((IGroupActor*)&var0, &V_UNKNOWN(pVarResult));
				VariantClear(&var0);
			}
			break;
		}
		case 0x60020008: {
			if (wFlags & DISPATCH_METHOD) {
				// line 172: [id(0x60020008)]HRESULT RemoveFromParent([out, retval] IUnknown** pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_UNKNOWN;
				hr = RemoveFromParent(&V_UNKNOWN(pVarResult));
			}
			break;
		}
		case 0x60020009: {
			if (wFlags & DISPATCH_METHOD) {
				// line 174: [id(0x60020009)]HRESULT AddChild([in] IUnknown* child,[out, retval] IUnknown** pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantCopyInd(&var0, &pDispParams->rgvarg[--index]);
				V_VT(pVarResult) = VT_UNKNOWN;
				hr = AddChild((IUnknown*)&var0, &V_UNKNOWN(pVarResult));
				VariantClear(&var0);
			}
			break;
		}
		case 0x6002000a: {
			if (wFlags & DISPATCH_METHOD) {
				// line 178: [id(0x6002000a)]HRESULT RemoveChild([in] IUnknown* child,[out, retval] IUnknown** pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantCopyInd(&var0, &pDispParams->rgvarg[--index]);
				V_VT(pVarResult) = VT_UNKNOWN;
				hr = RemoveChild((IUnknown*)&var0, &V_UNKNOWN(pVarResult));
				VariantClear(&var0);
			}
			break;
		}
		case 0x6002000b: {
			if (wFlags & DISPATCH_METHOD) {
				// line 182: [id(0x6002000b)]HRESULT Seek([in] single pos,[out, retval] IUnknown** pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				V_VT(pVarResult) = VT_UNKNOWN;
				hr = Seek(V_R4(&var0), &V_UNKNOWN(pVarResult));
				VariantClear(&var0);
			}
			break;
		}
		case 0x6002000c: {
			if (wFlags & DISPATCH_METHOD) {
				// line 186: [id(0x6002000c)]HRESULT MoveTo([in] single x,[in] single y,[in] single duration,[out, retval] ITweenAction** pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				VARIANT var1;
				V_VT(&var1) = VT_EMPTY;
				VariantChangeType(&var1, &pDispParams->rgvarg[--index], 0, VT_R4);
				VARIANT var2;
				V_VT(&var2) = VT_EMPTY;
				VariantChangeType(&var2, &pDispParams->rgvarg[--index], 0, VT_R4);
				V_VT(pVarResult) = VT_DISPATCH;
				hr = MoveTo(V_R4(&var0), V_R4(&var1), V_R4(&var2), (ITweenAction**)&V_DISPATCH(pVarResult));
				VariantClear(&var0);
				VariantClear(&var1);
				VariantClear(&var2);
			}
			break;
		}
		default:
		break;
	}
	if (hr != S_OK) {
		if (dispIdMember == DISPID_VALUE && wFlags == (DISPATCH_METHOD | DISPATCH_PROPERTYGET)) {
			V_VT(pVarResult) = VT_DISPATCH;
			V_DISPATCH(pVarResult) = this;
			return S_OK;
		}
		else if (hr != S_FALSE) {
			PLOGI.printf("dispId=%d (0x%08x), wFlags=%d, hr=%d", dispIdMember, dispIdMember, wFlags, hr);
		}
	}
	return hr;
}

STDMETHODIMP FlexDMDActionFactory::GetDocumentation(INT index, BSTR *pBstrName, BSTR *pBstrDocString, DWORD *pdwHelpContext, BSTR *pBstrHelpFile) {
	if (index == MEMBERID_NIL) {
		*pBstrName = SysAllocString(L"ActionFactory");
		return S_OK;
	}
	return E_NOTIMPL;
}

name_disp_id_t FlexDMDActor::m_nameIDMap[] = {
		{ NULL },
		{ L"ActionFactory", 0x60020018 },
		{ L"AddAction", 0x60020019 },
		{ L"ClearActions", 0x6002001a },
		{ L"ClearBackground", 0x6002000e },
		{ L"FillParent", 0x6002000c },
		{ L"Height", 0x60020008 },
		{ L"Name", 0x60020000 },
		{ L"Pack", 0x60020016 },
		{ L"PrefHeight", 0x60020015 },
		{ L"PrefWidth", 0x60020014 },
		{ L"Remove", 0x60020017 },
		{ L"SetAlignedPosition", 0x60020012 },
		{ L"SetBounds", 0x60020010 },
		{ L"SetPosition", 0x60020011 },
		{ L"SetSize", 0x60020013 },
		{ L"visible", 0x6002000a },
		{ L"Width", 0x60020006 },
		{ L"x", 0x60020002 },
		{ L"y", 0x60020004 }
};

STDMETHODIMP FlexDMDActor::GetIDsOfNames(REFIID /*riid*/, LPOLESTR* rgszNames, UINT cNames, LCID lcid, DISPID* rgDispId) {
	size_t min = 1, max = ARRAY_SIZE(m_nameIDMap) - 1, i;
	int r;
	while(min <= max) {
		i = (min + max) / 2;
		r = wcsicmp(m_nameIDMap[i].name, *rgszNames);
		if(!r) {
			*rgDispId = m_nameIDMap[i].dispId;
			return S_OK;
		}
		if(r < 0)
		   min = i+1;
		else
		   max = i-1;
	}
	return DISP_E_MEMBERNOTFOUND;
}

STDMETHODIMP FlexDMDActor::Invoke(DISPID dispIdMember, REFIID /*riid*/, LCID lcid, WORD wFlags, DISPPARAMS* pDispParams, VARIANT* pVarResult, EXCEPINFO* pExcepInfo, UINT* puArgErr) {
	HRESULT hr = DISP_E_UNKNOWNNAME;
	int index = pDispParams->cArgs;
	switch(dispIdMember) {
		case 0x60020000: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 204: [id(0x60020000), propget]HRESULT Name([out, retval] BSTR* pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_Name(&V_BSTR(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 206: [id(0x60020000), propput]HRESULT Name([in] BSTR pRetVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = put_Name(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 0x60020002: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 208: [id(0x60020002), propget]HRESULT x([out, retval] single* pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_x(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 210: [id(0x60020002), propput]HRESULT x([in] single pRetVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_x(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 0x60020004: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 212: [id(0x60020004), propget]HRESULT y([out, retval] single* pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_y(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 214: [id(0x60020004), propput]HRESULT y([in] single pRetVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_y(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 0x60020006: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 216: [id(0x60020006), propget]HRESULT Width([out, retval] single* pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Width(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 218: [id(0x60020006), propput]HRESULT Width([in] single pRetVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Width(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 0x60020008: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 220: [id(0x60020008), propget]HRESULT Height([out, retval] single* pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Height(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 222: [id(0x60020008), propput]HRESULT Height([in] single pRetVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Height(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 0x6002000a: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 224: [id(0x6002000a), propget]HRESULT visible([out, retval] VARIANT_BOOL* pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_visible(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 226: [id(0x6002000a), propput]HRESULT visible([in] VARIANT_BOOL pRetVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_visible(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 0x6002000c: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 228: [id(0x6002000c), propget]HRESULT FillParent([out, retval] VARIANT_BOOL* pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_FillParent(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 230: [id(0x6002000c), propput]HRESULT FillParent([in] VARIANT_BOOL pRetVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_FillParent(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 0x6002000e: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 232: [id(0x6002000e), propget]HRESULT ClearBackground([out, retval] VARIANT_BOOL* pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_ClearBackground(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 234: [id(0x6002000e), propput]HRESULT ClearBackground([in] VARIANT_BOOL pRetVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_ClearBackground(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 0x60020010: {
			if (wFlags & DISPATCH_METHOD) {
				// line 236: [id(0x60020010)]HRESULT SetBounds([in] single x,[in] single y,[in] single Width,[in] single Height);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				VARIANT var1;
				V_VT(&var1) = VT_EMPTY;
				VariantChangeType(&var1, &pDispParams->rgvarg[--index], 0, VT_R4);
				VARIANT var2;
				V_VT(&var2) = VT_EMPTY;
				VariantChangeType(&var2, &pDispParams->rgvarg[--index], 0, VT_R4);
				VARIANT var3;
				V_VT(&var3) = VT_EMPTY;
				VariantChangeType(&var3, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = SetBounds(V_R4(&var0), V_R4(&var1), V_R4(&var2), V_R4(&var3));
				VariantClear(&var0);
				VariantClear(&var1);
				VariantClear(&var2);
				VariantClear(&var3);
			}
			break;
		}
		case 0x60020011: {
			if (wFlags & DISPATCH_METHOD) {
				// line 242: [id(0x60020011)]HRESULT SetPosition([in] single x,[in] single y);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				VARIANT var1;
				V_VT(&var1) = VT_EMPTY;
				VariantChangeType(&var1, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = SetPosition(V_R4(&var0), V_R4(&var1));
				VariantClear(&var0);
				VariantClear(&var1);
			}
			break;
		}
		case 0x60020012: {
			if (wFlags & DISPATCH_METHOD) {
				// line 246: [id(0x60020012)]HRESULT SetAlignedPosition([in] single x,[in] single y,[in] Alignment alignment);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				VARIANT var1;
				V_VT(&var1) = VT_EMPTY;
				VariantChangeType(&var1, &pDispParams->rgvarg[--index], 0, VT_R4);
				VARIANT var2;
				V_VT(&var2) = VT_EMPTY;
				VariantChangeType(&var2, &pDispParams->rgvarg[--index], 0, VT_I4);
				hr = SetAlignedPosition(V_R4(&var0), V_R4(&var1), (Alignment)V_I4(&var2));
				VariantClear(&var0);
				VariantClear(&var1);
				VariantClear(&var2);
			}
			break;
		}
		case 0x60020013: {
			if (wFlags & DISPATCH_METHOD) {
				// line 251: [id(0x60020013)]HRESULT SetSize([in] single Width,[in] single Height);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				VARIANT var1;
				V_VT(&var1) = VT_EMPTY;
				VariantChangeType(&var1, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = SetSize(V_R4(&var0), V_R4(&var1));
				VariantClear(&var0);
				VariantClear(&var1);
			}
			break;
		}
		case 0x60020014: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 255: [id(0x60020014), propget]HRESULT PrefWidth([out, retval] single* pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_PrefWidth(&V_R4(pVarResult));
			}
			break;
		}
		case 0x60020015: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 257: [id(0x60020015), propget]HRESULT PrefHeight([out, retval] single* pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_PrefHeight(&V_R4(pVarResult));
			}
			break;
		}
		case 0x60020016: {
			if (wFlags & DISPATCH_METHOD) {
				// line 259: [id(0x60020016)]HRESULT Pack();
				hr = Pack();
			}
			break;
		}
		case 0x60020017: {
			if (wFlags & DISPATCH_METHOD) {
				// line 261: [id(0x60020017)]HRESULT Remove();
				hr = Remove();
			}
			break;
		}
		case 0x60020018: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 263: [id(0x60020018), propget]HRESULT ActionFactory([out, retval] IActionFactory** pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_DISPATCH;
				hr = get_ActionFactory((IActionFactory**)&V_DISPATCH(pVarResult));
			}
			break;
		}
		case 0x60020019: {
			if (wFlags & DISPATCH_METHOD) {
				// line 265: [id(0x60020019)]HRESULT AddAction([in] IUnknown* action);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantCopyInd(&var0, &pDispParams->rgvarg[--index]);
				hr = AddAction((IUnknown*)&var0);
				VariantClear(&var0);
			}
			break;
		}
		case 0x6002001a: {
			if (wFlags & DISPATCH_METHOD) {
				// line 267: [id(0x6002001a)]HRESULT ClearActions();
				hr = ClearActions();
			}
			break;
		}
		default:
		break;
	}
	if (hr != S_OK) {
		if (dispIdMember == DISPID_VALUE && wFlags == (DISPATCH_METHOD | DISPATCH_PROPERTYGET)) {
			V_VT(pVarResult) = VT_DISPATCH;
			V_DISPATCH(pVarResult) = this;
			return S_OK;
		}
		else if (hr != S_FALSE) {
			PLOGI.printf("dispId=%d (0x%08x), wFlags=%d, hr=%d", dispIdMember, dispIdMember, wFlags, hr);
		}
	}
	return hr;
}

STDMETHODIMP FlexDMDActor::GetDocumentation(INT index, BSTR *pBstrName, BSTR *pBstrDocString, DWORD *pdwHelpContext, BSTR *pBstrHelpFile) {
	if (index == MEMBERID_NIL) {
		*pBstrName = SysAllocString(L"Actor");
		return S_OK;
	}
	return E_NOTIMPL;
}

name_disp_id_t FlexDMDGroupActor::m_nameIDMap[] = {
		{ NULL },
		{ L"ActionFactory", 0x60020018 },
		{ L"AddAction", 0x60020019 },
		{ L"AddActor", 0x60020025 },
		{ L"ChildCount", 0x6002001d },
		{ L"ClearActions", 0x6002001a },
		{ L"ClearBackground", 0x6002000e },
		{ L"Clip", 0x6002001b },
		{ L"FillParent", 0x6002000c },
		{ L"GetFrame", 0x60020020 },
		{ L"GetGroup", 0x6002001f },
		{ L"GetImage", 0x60020023 },
		{ L"GetLabel", 0x60020021 },
		{ L"GetVideo", 0x60020022 },
		{ L"HasChild", 0x6002001e },
		{ L"Height", 0x60020008 },
		{ L"Name", 0x60020000 },
		{ L"Pack", 0x60020016 },
		{ L"PrefHeight", 0x60020015 },
		{ L"PrefWidth", 0x60020014 },
		{ L"Remove", 0x60020017 },
		{ L"RemoveActor", 0x60020026 },
		{ L"RemoveAll", 0x60020024 },
		{ L"SetAlignedPosition", 0x60020012 },
		{ L"SetBounds", 0x60020010 },
		{ L"SetPosition", 0x60020011 },
		{ L"SetSize", 0x60020013 },
		{ L"visible", 0x6002000a },
		{ L"Width", 0x60020006 },
		{ L"x", 0x60020002 },
		{ L"y", 0x60020004 }
};

STDMETHODIMP FlexDMDGroupActor::GetIDsOfNames(REFIID /*riid*/, LPOLESTR* rgszNames, UINT cNames, LCID lcid, DISPID* rgDispId) {
	size_t min = 1, max = ARRAY_SIZE(m_nameIDMap) - 1, i;
	int r;
	while(min <= max) {
		i = (min + max) / 2;
		r = wcsicmp(m_nameIDMap[i].name, *rgszNames);
		if(!r) {
			*rgDispId = m_nameIDMap[i].dispId;
			return S_OK;
		}
		if(r < 0)
		   min = i+1;
		else
		   max = i-1;
	}
	return DISP_E_MEMBERNOTFOUND;
}

STDMETHODIMP FlexDMDGroupActor::Invoke(DISPID dispIdMember, REFIID /*riid*/, LCID lcid, WORD wFlags, DISPPARAMS* pDispParams, VARIANT* pVarResult, EXCEPINFO* pExcepInfo, UINT* puArgErr) {
	HRESULT hr = DISP_E_UNKNOWNNAME;
	int index = pDispParams->cArgs;
	switch(dispIdMember) {
		case 0x60020000: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 281: [id(0x60020000), propget]HRESULT Name([out, retval] BSTR* pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_Name(&V_BSTR(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 283: [id(0x60020000), propput]HRESULT Name([in] BSTR pRetVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = put_Name(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 0x60020002: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 285: [id(0x60020002), propget]HRESULT x([out, retval] single* pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_x(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 287: [id(0x60020002), propput]HRESULT x([in] single pRetVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_x(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 0x60020004: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 289: [id(0x60020004), propget]HRESULT y([out, retval] single* pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_y(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 291: [id(0x60020004), propput]HRESULT y([in] single pRetVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_y(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 0x60020006: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 293: [id(0x60020006), propget]HRESULT Width([out, retval] single* pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Width(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 295: [id(0x60020006), propput]HRESULT Width([in] single pRetVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Width(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 0x60020008: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 297: [id(0x60020008), propget]HRESULT Height([out, retval] single* pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Height(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 299: [id(0x60020008), propput]HRESULT Height([in] single pRetVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Height(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 0x6002000a: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 301: [id(0x6002000a), propget]HRESULT visible([out, retval] VARIANT_BOOL* pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_visible(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 303: [id(0x6002000a), propput]HRESULT visible([in] VARIANT_BOOL pRetVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_visible(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 0x6002000c: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 305: [id(0x6002000c), propget]HRESULT FillParent([out, retval] VARIANT_BOOL* pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_FillParent(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 307: [id(0x6002000c), propput]HRESULT FillParent([in] VARIANT_BOOL pRetVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_FillParent(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 0x6002000e: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 309: [id(0x6002000e), propget]HRESULT ClearBackground([out, retval] VARIANT_BOOL* pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_ClearBackground(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 311: [id(0x6002000e), propput]HRESULT ClearBackground([in] VARIANT_BOOL pRetVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_ClearBackground(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 0x60020010: {
			if (wFlags & DISPATCH_METHOD) {
				// line 313: [id(0x60020010)]HRESULT SetBounds([in] single x,[in] single y,[in] single Width,[in] single Height);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				VARIANT var1;
				V_VT(&var1) = VT_EMPTY;
				VariantChangeType(&var1, &pDispParams->rgvarg[--index], 0, VT_R4);
				VARIANT var2;
				V_VT(&var2) = VT_EMPTY;
				VariantChangeType(&var2, &pDispParams->rgvarg[--index], 0, VT_R4);
				VARIANT var3;
				V_VT(&var3) = VT_EMPTY;
				VariantChangeType(&var3, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = SetBounds(V_R4(&var0), V_R4(&var1), V_R4(&var2), V_R4(&var3));
				VariantClear(&var0);
				VariantClear(&var1);
				VariantClear(&var2);
				VariantClear(&var3);
			}
			break;
		}
		case 0x60020011: {
			if (wFlags & DISPATCH_METHOD) {
				// line 319: [id(0x60020011)]HRESULT SetPosition([in] single x,[in] single y);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				VARIANT var1;
				V_VT(&var1) = VT_EMPTY;
				VariantChangeType(&var1, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = SetPosition(V_R4(&var0), V_R4(&var1));
				VariantClear(&var0);
				VariantClear(&var1);
			}
			break;
		}
		case 0x60020012: {
			if (wFlags & DISPATCH_METHOD) {
				// line 323: [id(0x60020012)]HRESULT SetAlignedPosition([in] single x,[in] single y,[in] Alignment alignment);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				VARIANT var1;
				V_VT(&var1) = VT_EMPTY;
				VariantChangeType(&var1, &pDispParams->rgvarg[--index], 0, VT_R4);
				VARIANT var2;
				V_VT(&var2) = VT_EMPTY;
				VariantChangeType(&var2, &pDispParams->rgvarg[--index], 0, VT_I4);
				hr = SetAlignedPosition(V_R4(&var0), V_R4(&var1), (Alignment)V_I4(&var2));
				VariantClear(&var0);
				VariantClear(&var1);
				VariantClear(&var2);
			}
			break;
		}
		case 0x60020013: {
			if (wFlags & DISPATCH_METHOD) {
				// line 328: [id(0x60020013)]HRESULT SetSize([in] single Width,[in] single Height);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				VARIANT var1;
				V_VT(&var1) = VT_EMPTY;
				VariantChangeType(&var1, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = SetSize(V_R4(&var0), V_R4(&var1));
				VariantClear(&var0);
				VariantClear(&var1);
			}
			break;
		}
		case 0x60020014: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 332: [id(0x60020014), propget]HRESULT PrefWidth([out, retval] single* pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_PrefWidth(&V_R4(pVarResult));
			}
			break;
		}
		case 0x60020015: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 334: [id(0x60020015), propget]HRESULT PrefHeight([out, retval] single* pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_PrefHeight(&V_R4(pVarResult));
			}
			break;
		}
		case 0x60020016: {
			if (wFlags & DISPATCH_METHOD) {
				// line 336: [id(0x60020016)]HRESULT Pack();
				hr = Pack();
			}
			break;
		}
		case 0x60020017: {
			if (wFlags & DISPATCH_METHOD) {
				// line 338: [id(0x60020017)]HRESULT Remove();
				hr = Remove();
			}
			break;
		}
		case 0x60020018: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 340: [id(0x60020018), propget]HRESULT ActionFactory([out, retval] IActionFactory** pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_DISPATCH;
				hr = get_ActionFactory((IActionFactory**)&V_DISPATCH(pVarResult));
			}
			break;
		}
		case 0x60020019: {
			if (wFlags & DISPATCH_METHOD) {
				// line 342: [id(0x60020019)]HRESULT AddAction([in] IUnknown* action);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantCopyInd(&var0, &pDispParams->rgvarg[--index]);
				hr = AddAction((IUnknown*)&var0);
				VariantClear(&var0);
			}
			break;
		}
		case 0x6002001a: {
			if (wFlags & DISPATCH_METHOD) {
				// line 344: [id(0x6002001a)]HRESULT ClearActions();
				hr = ClearActions();
			}
			break;
		}
		case 0x6002001b: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 346: [id(0x6002001b), propget]HRESULT Clip([out, retval] VARIANT_BOOL* pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_Clip(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 348: [id(0x6002001b), propput]HRESULT Clip([in] VARIANT_BOOL pRetVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_Clip(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 0x6002001d: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 350: [id(0x6002001d), propget]HRESULT ChildCount([out, retval] long* pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_ChildCount((long*)&V_I4(pVarResult));
			}
			break;
		}
		case 0x6002001e: {
			if (wFlags & DISPATCH_METHOD) {
				// line 352: [id(0x6002001e)]HRESULT HasChild([in] BSTR Name,[out, retval] VARIANT_BOOL* pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				V_VT(pVarResult) = VT_BOOL;
				hr = HasChild(V_BSTR(&var0), &V_BOOL(pVarResult));
				VariantClear(&var0);
			}
			break;
		}
		case 0x6002001f: {
			if (wFlags & DISPATCH_METHOD) {
				// line 356: [id(0x6002001f)]HRESULT GetGroup([in] BSTR Name,[out, retval] IGroupActor** pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				V_VT(pVarResult) = VT_DISPATCH;
				hr = GetGroup(V_BSTR(&var0), (IGroupActor**)&V_DISPATCH(pVarResult));
				VariantClear(&var0);
			}
			break;
		}
		case 0x60020020: {
			if (wFlags & DISPATCH_METHOD) {
				// line 360: [id(0x60020020)]HRESULT GetFrame([in] BSTR Name,[out, retval] IFrameActor** pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				V_VT(pVarResult) = VT_DISPATCH;
				hr = GetFrame(V_BSTR(&var0), (IFrameActor**)&V_DISPATCH(pVarResult));
				VariantClear(&var0);
			}
			break;
		}
		case 0x60020021: {
			if (wFlags & DISPATCH_METHOD) {
				// line 364: [id(0x60020021)]HRESULT GetLabel([in] BSTR Name,[out, retval] ILabelActor** pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				V_VT(pVarResult) = VT_DISPATCH;
				hr = GetLabel(V_BSTR(&var0), (ILabelActor**)&V_DISPATCH(pVarResult));
				VariantClear(&var0);
			}
			break;
		}
		case 0x60020022: {
			if (wFlags & DISPATCH_METHOD) {
				// line 368: [id(0x60020022)]HRESULT GetVideo([in] BSTR Name,[out, retval] IVideoActor** pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				V_VT(pVarResult) = VT_DISPATCH;
				hr = GetVideo(V_BSTR(&var0), (IVideoActor**)&V_DISPATCH(pVarResult));
				VariantClear(&var0);
			}
			break;
		}
		case 0x60020023: {
			if (wFlags & DISPATCH_METHOD) {
				// line 372: [id(0x60020023)]HRESULT GetImage([in] BSTR Name,[out, retval] IImageActor** pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				V_VT(pVarResult) = VT_DISPATCH;
				hr = GetImage(V_BSTR(&var0), (IImageActor**)&V_DISPATCH(pVarResult));
				VariantClear(&var0);
			}
			break;
		}
		case 0x60020024: {
			if (wFlags & DISPATCH_METHOD) {
				// line 376: [id(0x60020024)]HRESULT RemoveAll();
				hr = RemoveAll();
			}
			break;
		}
		case 0x60020025: {
			if (wFlags & DISPATCH_METHOD) {
				// line 378: [id(0x60020025)]HRESULT AddActor([in] IUnknown* child);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantCopyInd(&var0, &pDispParams->rgvarg[--index]);
				hr = AddActor((IUnknown*)&var0);
				VariantClear(&var0);
			}
			break;
		}
		case 0x60020026: {
			if (wFlags & DISPATCH_METHOD) {
				// line 380: [id(0x60020026)]HRESULT RemoveActor([in] IUnknown* child);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantCopyInd(&var0, &pDispParams->rgvarg[--index]);
				hr = RemoveActor((IUnknown*)&var0);
				VariantClear(&var0);
			}
			break;
		}
		default:
		break;
	}
	if (hr != S_OK) {
		if (dispIdMember == DISPID_VALUE && wFlags == (DISPATCH_METHOD | DISPATCH_PROPERTYGET)) {
			V_VT(pVarResult) = VT_DISPATCH;
			V_DISPATCH(pVarResult) = this;
			return S_OK;
		}
		else if (hr != S_FALSE) {
			PLOGI.printf("dispId=%d (0x%08x), wFlags=%d, hr=%d", dispIdMember, dispIdMember, wFlags, hr);
		}
	}
	return hr;
}

STDMETHODIMP FlexDMDGroupActor::GetDocumentation(INT index, BSTR *pBstrName, BSTR *pBstrDocString, DWORD *pdwHelpContext, BSTR *pBstrHelpFile) {
	if (index == MEMBERID_NIL) {
		*pBstrName = SysAllocString(L"GroupActor");
		return S_OK;
	}
	return E_NOTIMPL;
}

name_disp_id_t FlexDMDFrameActor::m_nameIDMap[] = {
		{ NULL },
		{ L"ActionFactory", 0x60020018 },
		{ L"AddAction", 0x60020019 },
		{ L"BorderColor", 0x6002001d },
		{ L"ClearActions", 0x6002001a },
		{ L"ClearBackground", 0x6002000e },
		{ L"Fill", 0x6002001f },
		{ L"FillColor", 0x60020021 },
		{ L"FillParent", 0x6002000c },
		{ L"Height", 0x60020008 },
		{ L"Name", 0x60020000 },
		{ L"Pack", 0x60020016 },
		{ L"PrefHeight", 0x60020015 },
		{ L"PrefWidth", 0x60020014 },
		{ L"Remove", 0x60020017 },
		{ L"SetAlignedPosition", 0x60020012 },
		{ L"SetBounds", 0x60020010 },
		{ L"SetPosition", 0x60020011 },
		{ L"SetSize", 0x60020013 },
		{ L"Thickness", 0x6002001b },
		{ L"visible", 0x6002000a },
		{ L"Width", 0x60020006 },
		{ L"x", 0x60020002 },
		{ L"y", 0x60020004 }
};

STDMETHODIMP FlexDMDFrameActor::GetIDsOfNames(REFIID /*riid*/, LPOLESTR* rgszNames, UINT cNames, LCID lcid, DISPID* rgDispId) {
	size_t min = 1, max = ARRAY_SIZE(m_nameIDMap) - 1, i;
	int r;
	while(min <= max) {
		i = (min + max) / 2;
		r = wcsicmp(m_nameIDMap[i].name, *rgszNames);
		if(!r) {
			*rgDispId = m_nameIDMap[i].dispId;
			return S_OK;
		}
		if(r < 0)
		   min = i+1;
		else
		   max = i-1;
	}
	return DISP_E_MEMBERNOTFOUND;
}

STDMETHODIMP FlexDMDFrameActor::Invoke(DISPID dispIdMember, REFIID /*riid*/, LCID lcid, WORD wFlags, DISPPARAMS* pDispParams, VARIANT* pVarResult, EXCEPINFO* pExcepInfo, UINT* puArgErr) {
	HRESULT hr = DISP_E_UNKNOWNNAME;
	int index = pDispParams->cArgs;
	switch(dispIdMember) {
		case 0x60020000: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 394: [id(0x60020000), propget]HRESULT Name([out, retval] BSTR* pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_Name(&V_BSTR(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 396: [id(0x60020000), propput]HRESULT Name([in] BSTR pRetVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = put_Name(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 0x60020002: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 398: [id(0x60020002), propget]HRESULT x([out, retval] single* pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_x(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 400: [id(0x60020002), propput]HRESULT x([in] single pRetVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_x(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 0x60020004: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 402: [id(0x60020004), propget]HRESULT y([out, retval] single* pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_y(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 404: [id(0x60020004), propput]HRESULT y([in] single pRetVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_y(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 0x60020006: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 406: [id(0x60020006), propget]HRESULT Width([out, retval] single* pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Width(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 408: [id(0x60020006), propput]HRESULT Width([in] single pRetVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Width(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 0x60020008: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 410: [id(0x60020008), propget]HRESULT Height([out, retval] single* pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Height(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 412: [id(0x60020008), propput]HRESULT Height([in] single pRetVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Height(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 0x6002000a: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 414: [id(0x6002000a), propget]HRESULT visible([out, retval] VARIANT_BOOL* pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_visible(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 416: [id(0x6002000a), propput]HRESULT visible([in] VARIANT_BOOL pRetVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_visible(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 0x6002000c: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 418: [id(0x6002000c), propget]HRESULT FillParent([out, retval] VARIANT_BOOL* pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_FillParent(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 420: [id(0x6002000c), propput]HRESULT FillParent([in] VARIANT_BOOL pRetVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_FillParent(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 0x6002000e: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 422: [id(0x6002000e), propget]HRESULT ClearBackground([out, retval] VARIANT_BOOL* pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_ClearBackground(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 424: [id(0x6002000e), propput]HRESULT ClearBackground([in] VARIANT_BOOL pRetVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_ClearBackground(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 0x60020010: {
			if (wFlags & DISPATCH_METHOD) {
				// line 426: [id(0x60020010)]HRESULT SetBounds([in] single x,[in] single y,[in] single Width,[in] single Height);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				VARIANT var1;
				V_VT(&var1) = VT_EMPTY;
				VariantChangeType(&var1, &pDispParams->rgvarg[--index], 0, VT_R4);
				VARIANT var2;
				V_VT(&var2) = VT_EMPTY;
				VariantChangeType(&var2, &pDispParams->rgvarg[--index], 0, VT_R4);
				VARIANT var3;
				V_VT(&var3) = VT_EMPTY;
				VariantChangeType(&var3, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = SetBounds(V_R4(&var0), V_R4(&var1), V_R4(&var2), V_R4(&var3));
				VariantClear(&var0);
				VariantClear(&var1);
				VariantClear(&var2);
				VariantClear(&var3);
			}
			break;
		}
		case 0x60020011: {
			if (wFlags & DISPATCH_METHOD) {
				// line 432: [id(0x60020011)]HRESULT SetPosition([in] single x,[in] single y);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				VARIANT var1;
				V_VT(&var1) = VT_EMPTY;
				VariantChangeType(&var1, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = SetPosition(V_R4(&var0), V_R4(&var1));
				VariantClear(&var0);
				VariantClear(&var1);
			}
			break;
		}
		case 0x60020012: {
			if (wFlags & DISPATCH_METHOD) {
				// line 436: [id(0x60020012)]HRESULT SetAlignedPosition([in] single x,[in] single y,[in] Alignment alignment);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				VARIANT var1;
				V_VT(&var1) = VT_EMPTY;
				VariantChangeType(&var1, &pDispParams->rgvarg[--index], 0, VT_R4);
				VARIANT var2;
				V_VT(&var2) = VT_EMPTY;
				VariantChangeType(&var2, &pDispParams->rgvarg[--index], 0, VT_I4);
				hr = SetAlignedPosition(V_R4(&var0), V_R4(&var1), (Alignment)V_I4(&var2));
				VariantClear(&var0);
				VariantClear(&var1);
				VariantClear(&var2);
			}
			break;
		}
		case 0x60020013: {
			if (wFlags & DISPATCH_METHOD) {
				// line 441: [id(0x60020013)]HRESULT SetSize([in] single Width,[in] single Height);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				VARIANT var1;
				V_VT(&var1) = VT_EMPTY;
				VariantChangeType(&var1, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = SetSize(V_R4(&var0), V_R4(&var1));
				VariantClear(&var0);
				VariantClear(&var1);
			}
			break;
		}
		case 0x60020014: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 445: [id(0x60020014), propget]HRESULT PrefWidth([out, retval] single* pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_PrefWidth(&V_R4(pVarResult));
			}
			break;
		}
		case 0x60020015: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 447: [id(0x60020015), propget]HRESULT PrefHeight([out, retval] single* pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_PrefHeight(&V_R4(pVarResult));
			}
			break;
		}
		case 0x60020016: {
			if (wFlags & DISPATCH_METHOD) {
				// line 449: [id(0x60020016)]HRESULT Pack();
				hr = Pack();
			}
			break;
		}
		case 0x60020017: {
			if (wFlags & DISPATCH_METHOD) {
				// line 451: [id(0x60020017)]HRESULT Remove();
				hr = Remove();
			}
			break;
		}
		case 0x60020018: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 453: [id(0x60020018), propget]HRESULT ActionFactory([out, retval] IActionFactory** pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_DISPATCH;
				hr = get_ActionFactory((IActionFactory**)&V_DISPATCH(pVarResult));
			}
			break;
		}
		case 0x60020019: {
			if (wFlags & DISPATCH_METHOD) {
				// line 455: [id(0x60020019)]HRESULT AddAction([in] IUnknown* action);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantCopyInd(&var0, &pDispParams->rgvarg[--index]);
				hr = AddAction((IUnknown*)&var0);
				VariantClear(&var0);
			}
			break;
		}
		case 0x6002001a: {
			if (wFlags & DISPATCH_METHOD) {
				// line 457: [id(0x6002001a)]HRESULT ClearActions();
				hr = ClearActions();
			}
			break;
		}
		case 0x6002001b: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 459: [id(0x6002001b), propget]HRESULT Thickness([out, retval] long* pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_Thickness((long*)&V_I4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 461: [id(0x6002001b), propput]HRESULT Thickness([in] long pRetVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_I4);
				hr = put_Thickness(V_I4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 0x6002001d: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 463: [id(0x6002001d), propget]HRESULT BorderColor([out, retval] OLE_COLOR* pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_UI4;
				hr = get_BorderColor(&V_UI4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 465: [id(0x6002001d), propput]HRESULT BorderColor([in] OLE_COLOR pRetVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_UI4);
				hr = put_BorderColor((OLE_COLOR)V_UI4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 0x6002001f: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 467: [id(0x6002001f), propget]HRESULT Fill([out, retval] VARIANT_BOOL* pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_Fill(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 469: [id(0x6002001f), propput]HRESULT Fill([in] VARIANT_BOOL pRetVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_Fill(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 0x60020021: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 471: [id(0x60020021), propget]HRESULT FillColor([out, retval] OLE_COLOR* pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_UI4;
				hr = get_FillColor(&V_UI4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 473: [id(0x60020021), propput]HRESULT FillColor([in] OLE_COLOR pRetVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_UI4);
				hr = put_FillColor((OLE_COLOR)V_UI4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		default:
		break;
	}
	if (hr != S_OK) {
		if (dispIdMember == DISPID_VALUE && wFlags == (DISPATCH_METHOD | DISPATCH_PROPERTYGET)) {
			V_VT(pVarResult) = VT_DISPATCH;
			V_DISPATCH(pVarResult) = this;
			return S_OK;
		}
		else if (hr != S_FALSE) {
			PLOGI.printf("dispId=%d (0x%08x), wFlags=%d, hr=%d", dispIdMember, dispIdMember, wFlags, hr);
		}
	}
	return hr;
}

STDMETHODIMP FlexDMDFrameActor::GetDocumentation(INT index, BSTR *pBstrName, BSTR *pBstrDocString, DWORD *pdwHelpContext, BSTR *pBstrHelpFile) {
	if (index == MEMBERID_NIL) {
		*pBstrName = SysAllocString(L"FrameActor");
		return S_OK;
	}
	return E_NOTIMPL;
}

name_disp_id_t FlexDMDImageActor::m_nameIDMap[] = {
		{ NULL },
		{ L"ActionFactory", 0x60020018 },
		{ L"AddAction", 0x60020019 },
		{ L"Bitmap", 0x6002001b },
		{ L"ClearActions", 0x6002001a },
		{ L"ClearBackground", 0x6002000e },
		{ L"FillParent", 0x6002000c },
		{ L"Height", 0x60020008 },
		{ L"Name", 0x60020000 },
		{ L"Pack", 0x60020016 },
		{ L"PrefHeight", 0x60020015 },
		{ L"PrefWidth", 0x60020014 },
		{ L"Remove", 0x60020017 },
		{ L"SetAlignedPosition", 0x60020012 },
		{ L"SetBounds", 0x60020010 },
		{ L"SetPosition", 0x60020011 },
		{ L"SetSize", 0x60020013 },
		{ L"visible", 0x6002000a },
		{ L"Width", 0x60020006 },
		{ L"x", 0x60020002 },
		{ L"y", 0x60020004 }
};

STDMETHODIMP FlexDMDImageActor::GetIDsOfNames(REFIID /*riid*/, LPOLESTR* rgszNames, UINT cNames, LCID lcid, DISPID* rgDispId) {
	size_t min = 1, max = ARRAY_SIZE(m_nameIDMap) - 1, i;
	int r;
	while(min <= max) {
		i = (min + max) / 2;
		r = wcsicmp(m_nameIDMap[i].name, *rgszNames);
		if(!r) {
			*rgDispId = m_nameIDMap[i].dispId;
			return S_OK;
		}
		if(r < 0)
		   min = i+1;
		else
		   max = i-1;
	}
	return DISP_E_MEMBERNOTFOUND;
}

STDMETHODIMP FlexDMDImageActor::Invoke(DISPID dispIdMember, REFIID /*riid*/, LCID lcid, WORD wFlags, DISPPARAMS* pDispParams, VARIANT* pVarResult, EXCEPINFO* pExcepInfo, UINT* puArgErr) {
	HRESULT hr = DISP_E_UNKNOWNNAME;
	int index = pDispParams->cArgs;
	switch(dispIdMember) {
		case 0x60020000: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 487: [id(0x60020000), propget]HRESULT Name([out, retval] BSTR* pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_Name(&V_BSTR(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 489: [id(0x60020000), propput]HRESULT Name([in] BSTR pRetVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = put_Name(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 0x60020002: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 491: [id(0x60020002), propget]HRESULT x([out, retval] single* pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_x(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 493: [id(0x60020002), propput]HRESULT x([in] single pRetVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_x(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 0x60020004: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 495: [id(0x60020004), propget]HRESULT y([out, retval] single* pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_y(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 497: [id(0x60020004), propput]HRESULT y([in] single pRetVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_y(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 0x60020006: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 499: [id(0x60020006), propget]HRESULT Width([out, retval] single* pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Width(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 501: [id(0x60020006), propput]HRESULT Width([in] single pRetVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Width(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 0x60020008: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 503: [id(0x60020008), propget]HRESULT Height([out, retval] single* pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Height(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 505: [id(0x60020008), propput]HRESULT Height([in] single pRetVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Height(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 0x6002000a: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 507: [id(0x6002000a), propget]HRESULT visible([out, retval] VARIANT_BOOL* pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_visible(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 509: [id(0x6002000a), propput]HRESULT visible([in] VARIANT_BOOL pRetVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_visible(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 0x6002000c: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 511: [id(0x6002000c), propget]HRESULT FillParent([out, retval] VARIANT_BOOL* pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_FillParent(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 513: [id(0x6002000c), propput]HRESULT FillParent([in] VARIANT_BOOL pRetVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_FillParent(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 0x6002000e: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 515: [id(0x6002000e), propget]HRESULT ClearBackground([out, retval] VARIANT_BOOL* pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_ClearBackground(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 517: [id(0x6002000e), propput]HRESULT ClearBackground([in] VARIANT_BOOL pRetVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_ClearBackground(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 0x60020010: {
			if (wFlags & DISPATCH_METHOD) {
				// line 519: [id(0x60020010)]HRESULT SetBounds([in] single x,[in] single y,[in] single Width,[in] single Height);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				VARIANT var1;
				V_VT(&var1) = VT_EMPTY;
				VariantChangeType(&var1, &pDispParams->rgvarg[--index], 0, VT_R4);
				VARIANT var2;
				V_VT(&var2) = VT_EMPTY;
				VariantChangeType(&var2, &pDispParams->rgvarg[--index], 0, VT_R4);
				VARIANT var3;
				V_VT(&var3) = VT_EMPTY;
				VariantChangeType(&var3, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = SetBounds(V_R4(&var0), V_R4(&var1), V_R4(&var2), V_R4(&var3));
				VariantClear(&var0);
				VariantClear(&var1);
				VariantClear(&var2);
				VariantClear(&var3);
			}
			break;
		}
		case 0x60020011: {
			if (wFlags & DISPATCH_METHOD) {
				// line 525: [id(0x60020011)]HRESULT SetPosition([in] single x,[in] single y);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				VARIANT var1;
				V_VT(&var1) = VT_EMPTY;
				VariantChangeType(&var1, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = SetPosition(V_R4(&var0), V_R4(&var1));
				VariantClear(&var0);
				VariantClear(&var1);
			}
			break;
		}
		case 0x60020012: {
			if (wFlags & DISPATCH_METHOD) {
				// line 529: [id(0x60020012)]HRESULT SetAlignedPosition([in] single x,[in] single y,[in] Alignment alignment);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				VARIANT var1;
				V_VT(&var1) = VT_EMPTY;
				VariantChangeType(&var1, &pDispParams->rgvarg[--index], 0, VT_R4);
				VARIANT var2;
				V_VT(&var2) = VT_EMPTY;
				VariantChangeType(&var2, &pDispParams->rgvarg[--index], 0, VT_I4);
				hr = SetAlignedPosition(V_R4(&var0), V_R4(&var1), (Alignment)V_I4(&var2));
				VariantClear(&var0);
				VariantClear(&var1);
				VariantClear(&var2);
			}
			break;
		}
		case 0x60020013: {
			if (wFlags & DISPATCH_METHOD) {
				// line 534: [id(0x60020013)]HRESULT SetSize([in] single Width,[in] single Height);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				VARIANT var1;
				V_VT(&var1) = VT_EMPTY;
				VariantChangeType(&var1, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = SetSize(V_R4(&var0), V_R4(&var1));
				VariantClear(&var0);
				VariantClear(&var1);
			}
			break;
		}
		case 0x60020014: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 538: [id(0x60020014), propget]HRESULT PrefWidth([out, retval] single* pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_PrefWidth(&V_R4(pVarResult));
			}
			break;
		}
		case 0x60020015: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 540: [id(0x60020015), propget]HRESULT PrefHeight([out, retval] single* pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_PrefHeight(&V_R4(pVarResult));
			}
			break;
		}
		case 0x60020016: {
			if (wFlags & DISPATCH_METHOD) {
				// line 542: [id(0x60020016)]HRESULT Pack();
				hr = Pack();
			}
			break;
		}
		case 0x60020017: {
			if (wFlags & DISPATCH_METHOD) {
				// line 544: [id(0x60020017)]HRESULT Remove();
				hr = Remove();
			}
			break;
		}
		case 0x60020018: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 546: [id(0x60020018), propget]HRESULT ActionFactory([out, retval] IActionFactory** pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_DISPATCH;
				hr = get_ActionFactory((IActionFactory**)&V_DISPATCH(pVarResult));
			}
			break;
		}
		case 0x60020019: {
			if (wFlags & DISPATCH_METHOD) {
				// line 548: [id(0x60020019)]HRESULT AddAction([in] IUnknown* action);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantCopyInd(&var0, &pDispParams->rgvarg[--index]);
				hr = AddAction((IUnknown*)&var0);
				VariantClear(&var0);
			}
			break;
		}
		case 0x6002001a: {
			if (wFlags & DISPATCH_METHOD) {
				// line 550: [id(0x6002001a)]HRESULT ClearActions();
				hr = ClearActions();
			}
			break;
		}
		case 0x6002001b: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 552: [id(0x6002001b), propget]HRESULT Bitmap([out, retval] _Bitmap** pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_DISPATCH;
				hr = get_Bitmap((_Bitmap**)&V_DISPATCH(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUTREF) {
				// line 554: [id(0x6002001b), propputref]HRESULT Bitmap([in] _Bitmap* pRetVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantCopyInd(&var0, &pDispParams->rgvarg[--index]);
				hr = putref_Bitmap((_Bitmap*)&var0);
				VariantClear(&var0);
			}
			break;
		}
		default:
		break;
	}
	if (hr != S_OK) {
		if (dispIdMember == DISPID_VALUE && wFlags == (DISPATCH_METHOD | DISPATCH_PROPERTYGET)) {
			V_VT(pVarResult) = VT_DISPATCH;
			V_DISPATCH(pVarResult) = this;
			return S_OK;
		}
		else if (hr != S_FALSE) {
			PLOGI.printf("dispId=%d (0x%08x), wFlags=%d, hr=%d", dispIdMember, dispIdMember, wFlags, hr);
		}
	}
	return hr;
}

STDMETHODIMP FlexDMDImageActor::GetDocumentation(INT index, BSTR *pBstrName, BSTR *pBstrDocString, DWORD *pdwHelpContext, BSTR *pBstrHelpFile) {
	if (index == MEMBERID_NIL) {
		*pBstrName = SysAllocString(L"ImageActor");
		return S_OK;
	}
	return E_NOTIMPL;
}

name_disp_id_t FlexDMDVideoActor::m_nameIDMap[] = {
		{ NULL },
		{ L"ActionFactory", 0x60020018 },
		{ L"AddAction", 0x60020019 },
		{ L"ClearActions", 0x6002001a },
		{ L"ClearBackground", 0x6002000e },
		{ L"FillParent", 0x6002000c },
		{ L"Height", 0x60020008 },
		{ L"Length", 0x6002001f },
		{ L"Loop", 0x60020020 },
		{ L"Name", 0x60020000 },
		{ L"Pack", 0x60020016 },
		{ L"Paused", 0x60020022 },
		{ L"PlaySpeed", 0x60020024 },
		{ L"PrefHeight", 0x60020015 },
		{ L"PrefWidth", 0x60020014 },
		{ L"Remove", 0x60020017 },
		{ L"Seek", 0x60020026 },
		{ L"SetAlignedPosition", 0x60020012 },
		{ L"SetBounds", 0x60020010 },
		{ L"SetPosition", 0x60020011 },
		{ L"SetSize", 0x60020013 },
		{ L"visible", 0x6002000a },
		{ L"Width", 0x60020006 },
		{ L"x", 0x60020002 },
		{ L"y", 0x60020004 }
};

STDMETHODIMP FlexDMDVideoActor::GetIDsOfNames(REFIID /*riid*/, LPOLESTR* rgszNames, UINT cNames, LCID lcid, DISPID* rgDispId) {
	size_t min = 1, max = ARRAY_SIZE(m_nameIDMap) - 1, i;
	int r;
	while(min <= max) {
		i = (min + max) / 2;
		r = wcsicmp(m_nameIDMap[i].name, *rgszNames);
		if(!r) {
			*rgDispId = m_nameIDMap[i].dispId;
			return S_OK;
		}
		if(r < 0)
		   min = i+1;
		else
		   max = i-1;
	}
	return DISP_E_MEMBERNOTFOUND;
}

STDMETHODIMP FlexDMDVideoActor::Invoke(DISPID dispIdMember, REFIID /*riid*/, LCID lcid, WORD wFlags, DISPPARAMS* pDispParams, VARIANT* pVarResult, EXCEPINFO* pExcepInfo, UINT* puArgErr) {
	HRESULT hr = DISP_E_UNKNOWNNAME;
	int index = pDispParams->cArgs;
	switch(dispIdMember) {
		case 0x60020000: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 568: [id(0x60020000), propget]HRESULT Name([out, retval] BSTR* pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_Name(&V_BSTR(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 570: [id(0x60020000), propput]HRESULT Name([in] BSTR pRetVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = put_Name(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 0x60020002: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 572: [id(0x60020002), propget]HRESULT x([out, retval] single* pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_x(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 574: [id(0x60020002), propput]HRESULT x([in] single pRetVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_x(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 0x60020004: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 576: [id(0x60020004), propget]HRESULT y([out, retval] single* pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_y(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 578: [id(0x60020004), propput]HRESULT y([in] single pRetVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_y(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 0x60020006: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 580: [id(0x60020006), propget]HRESULT Width([out, retval] single* pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Width(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 582: [id(0x60020006), propput]HRESULT Width([in] single pRetVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Width(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 0x60020008: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 584: [id(0x60020008), propget]HRESULT Height([out, retval] single* pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Height(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 586: [id(0x60020008), propput]HRESULT Height([in] single pRetVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Height(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 0x6002000a: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 588: [id(0x6002000a), propget]HRESULT visible([out, retval] VARIANT_BOOL* pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_visible(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 590: [id(0x6002000a), propput]HRESULT visible([in] VARIANT_BOOL pRetVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_visible(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 0x6002000c: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 592: [id(0x6002000c), propget]HRESULT FillParent([out, retval] VARIANT_BOOL* pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_FillParent(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 594: [id(0x6002000c), propput]HRESULT FillParent([in] VARIANT_BOOL pRetVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_FillParent(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 0x6002000e: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 596: [id(0x6002000e), propget]HRESULT ClearBackground([out, retval] VARIANT_BOOL* pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_ClearBackground(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 598: [id(0x6002000e), propput]HRESULT ClearBackground([in] VARIANT_BOOL pRetVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_ClearBackground(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 0x60020010: {
			if (wFlags & DISPATCH_METHOD) {
				// line 600: [id(0x60020010)]HRESULT SetBounds([in] single x,[in] single y,[in] single Width,[in] single Height);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				VARIANT var1;
				V_VT(&var1) = VT_EMPTY;
				VariantChangeType(&var1, &pDispParams->rgvarg[--index], 0, VT_R4);
				VARIANT var2;
				V_VT(&var2) = VT_EMPTY;
				VariantChangeType(&var2, &pDispParams->rgvarg[--index], 0, VT_R4);
				VARIANT var3;
				V_VT(&var3) = VT_EMPTY;
				VariantChangeType(&var3, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = SetBounds(V_R4(&var0), V_R4(&var1), V_R4(&var2), V_R4(&var3));
				VariantClear(&var0);
				VariantClear(&var1);
				VariantClear(&var2);
				VariantClear(&var3);
			}
			break;
		}
		case 0x60020011: {
			if (wFlags & DISPATCH_METHOD) {
				// line 606: [id(0x60020011)]HRESULT SetPosition([in] single x,[in] single y);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				VARIANT var1;
				V_VT(&var1) = VT_EMPTY;
				VariantChangeType(&var1, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = SetPosition(V_R4(&var0), V_R4(&var1));
				VariantClear(&var0);
				VariantClear(&var1);
			}
			break;
		}
		case 0x60020012: {
			if (wFlags & DISPATCH_METHOD) {
				// line 610: [id(0x60020012)]HRESULT SetAlignedPosition([in] single x,[in] single y,[in] Alignment alignment);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				VARIANT var1;
				V_VT(&var1) = VT_EMPTY;
				VariantChangeType(&var1, &pDispParams->rgvarg[--index], 0, VT_R4);
				VARIANT var2;
				V_VT(&var2) = VT_EMPTY;
				VariantChangeType(&var2, &pDispParams->rgvarg[--index], 0, VT_I4);
				hr = SetAlignedPosition(V_R4(&var0), V_R4(&var1), (Alignment)V_I4(&var2));
				VariantClear(&var0);
				VariantClear(&var1);
				VariantClear(&var2);
			}
			break;
		}
		case 0x60020013: {
			if (wFlags & DISPATCH_METHOD) {
				// line 615: [id(0x60020013)]HRESULT SetSize([in] single Width,[in] single Height);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				VARIANT var1;
				V_VT(&var1) = VT_EMPTY;
				VariantChangeType(&var1, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = SetSize(V_R4(&var0), V_R4(&var1));
				VariantClear(&var0);
				VariantClear(&var1);
			}
			break;
		}
		case 0x60020014: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 619: [id(0x60020014), propget]HRESULT PrefWidth([out, retval] single* pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_PrefWidth(&V_R4(pVarResult));
			}
			break;
		}
		case 0x60020015: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 621: [id(0x60020015), propget]HRESULT PrefHeight([out, retval] single* pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_PrefHeight(&V_R4(pVarResult));
			}
			break;
		}
		case 0x60020016: {
			if (wFlags & DISPATCH_METHOD) {
				// line 623: [id(0x60020016)]HRESULT Pack();
				hr = Pack();
			}
			break;
		}
		case 0x60020017: {
			if (wFlags & DISPATCH_METHOD) {
				// line 625: [id(0x60020017)]HRESULT Remove();
				hr = Remove();
			}
			break;
		}
		case 0x60020018: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 627: [id(0x60020018), propget]HRESULT ActionFactory([out, retval] IActionFactory** pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_DISPATCH;
				hr = get_ActionFactory((IActionFactory**)&V_DISPATCH(pVarResult));
			}
			break;
		}
		case 0x60020019: {
			if (wFlags & DISPATCH_METHOD) {
				// line 629: [id(0x60020019)]HRESULT AddAction([in] IUnknown* action);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantCopyInd(&var0, &pDispParams->rgvarg[--index]);
				hr = AddAction((IUnknown*)&var0);
				VariantClear(&var0);
			}
			break;
		}
		case 0x6002001a: {
			if (wFlags & DISPATCH_METHOD) {
				// line 631: [id(0x6002001a)]HRESULT ClearActions();
				hr = ClearActions();
			}
			break;
		}
		case 0x6002001f: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 637: [id(0x6002001f), propget]HRESULT Length([out, retval] single* pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Length(&V_R4(pVarResult));
			}
			break;
		}
		case 0x60020020: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 639: [id(0x60020020), propget]HRESULT Loop([out, retval] VARIANT_BOOL* pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_Loop(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 641: [id(0x60020020), propput]HRESULT Loop([in] VARIANT_BOOL pRetVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_Loop(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 0x60020022: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 643: [id(0x60020022), propget]HRESULT Paused([out, retval] VARIANT_BOOL* pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_Paused(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 645: [id(0x60020022), propput]HRESULT Paused([in] VARIANT_BOOL pRetVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_Paused(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 0x60020024: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 647: [id(0x60020024), propget]HRESULT PlaySpeed([out, retval] single* pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_PlaySpeed(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 649: [id(0x60020024), propput]HRESULT PlaySpeed([in] single pRetVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_PlaySpeed(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 0x60020026: {
			if (wFlags & DISPATCH_METHOD) {
				// line 651: [id(0x60020026)]HRESULT Seek([in] single posInSeconds);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = Seek(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		default:
		break;
	}
	if (hr != S_OK) {
		if (dispIdMember == DISPID_VALUE && wFlags == (DISPATCH_METHOD | DISPATCH_PROPERTYGET)) {
			V_VT(pVarResult) = VT_DISPATCH;
			V_DISPATCH(pVarResult) = this;
			return S_OK;
		}
		else if (hr != S_FALSE) {
			PLOGI.printf("dispId=%d (0x%08x), wFlags=%d, hr=%d", dispIdMember, dispIdMember, wFlags, hr);
		}
	}
	return hr;
}

STDMETHODIMP FlexDMDVideoActor::GetDocumentation(INT index, BSTR *pBstrName, BSTR *pBstrDocString, DWORD *pdwHelpContext, BSTR *pBstrHelpFile) {
	if (index == MEMBERID_NIL) {
		*pBstrName = SysAllocString(L"VideoActor");
		return S_OK;
	}
	return E_NOTIMPL;
}

name_disp_id_t FlexDMDLabelActor::m_nameIDMap[] = {
		{ NULL },
		{ L"ActionFactory", 0x60020018 },
		{ L"AddAction", 0x60020019 },
		{ L"AutoPack", 0x6002001b },
		{ L"ClearActions", 0x6002001a },
		{ L"ClearBackground", 0x6002000e },
		{ L"FillParent", 0x6002000c },
		{ L"Font", 0x6002001f },
		{ L"Height", 0x60020008 },
		{ L"Name", 0x60020000 },
		{ L"Pack", 0x60020016 },
		{ L"PrefHeight", 0x60020015 },
		{ L"PrefWidth", 0x60020014 },
		{ L"Remove", 0x60020017 },
		{ L"SetAlignedPosition", 0x60020012 },
		{ L"SetBounds", 0x60020010 },
		{ L"SetPosition", 0x60020011 },
		{ L"SetSize", 0x60020013 },
		{ L"Text", 0x60020021 },
		{ L"visible", 0x6002000a },
		{ L"Width", 0x60020006 },
		{ L"x", 0x60020002 },
		{ L"y", 0x60020004 }
};

STDMETHODIMP FlexDMDLabelActor::GetIDsOfNames(REFIID /*riid*/, LPOLESTR* rgszNames, UINT cNames, LCID lcid, DISPID* rgDispId) {
	size_t min = 1, max = ARRAY_SIZE(m_nameIDMap) - 1, i;
	int r;
	while(min <= max) {
		i = (min + max) / 2;
		r = wcsicmp(m_nameIDMap[i].name, *rgszNames);
		if(!r) {
			*rgDispId = m_nameIDMap[i].dispId;
			return S_OK;
		}
		if(r < 0)
		   min = i+1;
		else
		   max = i-1;
	}
	return DISP_E_MEMBERNOTFOUND;
}

STDMETHODIMP FlexDMDLabelActor::Invoke(DISPID dispIdMember, REFIID /*riid*/, LCID lcid, WORD wFlags, DISPPARAMS* pDispParams, VARIANT* pVarResult, EXCEPINFO* pExcepInfo, UINT* puArgErr) {
	HRESULT hr = DISP_E_UNKNOWNNAME;
	int index = pDispParams->cArgs;
	switch(dispIdMember) {
		case 0x60020000: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 665: [id(0x60020000), propget]HRESULT Name([out, retval] BSTR* pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_Name(&V_BSTR(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 667: [id(0x60020000), propput]HRESULT Name([in] BSTR pRetVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = put_Name(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 0x60020002: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 669: [id(0x60020002), propget]HRESULT x([out, retval] single* pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_x(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 671: [id(0x60020002), propput]HRESULT x([in] single pRetVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_x(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 0x60020004: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 673: [id(0x60020004), propget]HRESULT y([out, retval] single* pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_y(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 675: [id(0x60020004), propput]HRESULT y([in] single pRetVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_y(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 0x60020006: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 677: [id(0x60020006), propget]HRESULT Width([out, retval] single* pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Width(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 679: [id(0x60020006), propput]HRESULT Width([in] single pRetVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Width(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 0x60020008: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 681: [id(0x60020008), propget]HRESULT Height([out, retval] single* pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Height(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 683: [id(0x60020008), propput]HRESULT Height([in] single pRetVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Height(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 0x6002000a: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 685: [id(0x6002000a), propget]HRESULT visible([out, retval] VARIANT_BOOL* pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_visible(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 687: [id(0x6002000a), propput]HRESULT visible([in] VARIANT_BOOL pRetVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_visible(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 0x6002000c: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 689: [id(0x6002000c), propget]HRESULT FillParent([out, retval] VARIANT_BOOL* pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_FillParent(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 691: [id(0x6002000c), propput]HRESULT FillParent([in] VARIANT_BOOL pRetVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_FillParent(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 0x6002000e: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 693: [id(0x6002000e), propget]HRESULT ClearBackground([out, retval] VARIANT_BOOL* pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_ClearBackground(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 695: [id(0x6002000e), propput]HRESULT ClearBackground([in] VARIANT_BOOL pRetVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_ClearBackground(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 0x60020010: {
			if (wFlags & DISPATCH_METHOD) {
				// line 697: [id(0x60020010)]HRESULT SetBounds([in] single x,[in] single y,[in] single Width,[in] single Height);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				VARIANT var1;
				V_VT(&var1) = VT_EMPTY;
				VariantChangeType(&var1, &pDispParams->rgvarg[--index], 0, VT_R4);
				VARIANT var2;
				V_VT(&var2) = VT_EMPTY;
				VariantChangeType(&var2, &pDispParams->rgvarg[--index], 0, VT_R4);
				VARIANT var3;
				V_VT(&var3) = VT_EMPTY;
				VariantChangeType(&var3, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = SetBounds(V_R4(&var0), V_R4(&var1), V_R4(&var2), V_R4(&var3));
				VariantClear(&var0);
				VariantClear(&var1);
				VariantClear(&var2);
				VariantClear(&var3);
			}
			break;
		}
		case 0x60020011: {
			if (wFlags & DISPATCH_METHOD) {
				// line 703: [id(0x60020011)]HRESULT SetPosition([in] single x,[in] single y);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				VARIANT var1;
				V_VT(&var1) = VT_EMPTY;
				VariantChangeType(&var1, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = SetPosition(V_R4(&var0), V_R4(&var1));
				VariantClear(&var0);
				VariantClear(&var1);
			}
			break;
		}
		case 0x60020012: {
			if (wFlags & DISPATCH_METHOD) {
				// line 707: [id(0x60020012)]HRESULT SetAlignedPosition([in] single x,[in] single y,[in] Alignment alignment);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				VARIANT var1;
				V_VT(&var1) = VT_EMPTY;
				VariantChangeType(&var1, &pDispParams->rgvarg[--index], 0, VT_R4);
				VARIANT var2;
				V_VT(&var2) = VT_EMPTY;
				VariantChangeType(&var2, &pDispParams->rgvarg[--index], 0, VT_I4);
				hr = SetAlignedPosition(V_R4(&var0), V_R4(&var1), (Alignment)V_I4(&var2));
				VariantClear(&var0);
				VariantClear(&var1);
				VariantClear(&var2);
			}
			break;
		}
		case 0x60020013: {
			if (wFlags & DISPATCH_METHOD) {
				// line 712: [id(0x60020013)]HRESULT SetSize([in] single Width,[in] single Height);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				VARIANT var1;
				V_VT(&var1) = VT_EMPTY;
				VariantChangeType(&var1, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = SetSize(V_R4(&var0), V_R4(&var1));
				VariantClear(&var0);
				VariantClear(&var1);
			}
			break;
		}
		case 0x60020014: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 716: [id(0x60020014), propget]HRESULT PrefWidth([out, retval] single* pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_PrefWidth(&V_R4(pVarResult));
			}
			break;
		}
		case 0x60020015: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 718: [id(0x60020015), propget]HRESULT PrefHeight([out, retval] single* pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_PrefHeight(&V_R4(pVarResult));
			}
			break;
		}
		case 0x60020016: {
			if (wFlags & DISPATCH_METHOD) {
				// line 720: [id(0x60020016)]HRESULT Pack();
				hr = Pack();
			}
			break;
		}
		case 0x60020017: {
			if (wFlags & DISPATCH_METHOD) {
				// line 722: [id(0x60020017)]HRESULT Remove();
				hr = Remove();
			}
			break;
		}
		case 0x60020018: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 724: [id(0x60020018), propget]HRESULT ActionFactory([out, retval] IActionFactory** pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_DISPATCH;
				hr = get_ActionFactory((IActionFactory**)&V_DISPATCH(pVarResult));
			}
			break;
		}
		case 0x60020019: {
			if (wFlags & DISPATCH_METHOD) {
				// line 726: [id(0x60020019)]HRESULT AddAction([in] IUnknown* action);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantCopyInd(&var0, &pDispParams->rgvarg[--index]);
				hr = AddAction((IUnknown*)&var0);
				VariantClear(&var0);
			}
			break;
		}
		case 0x6002001a: {
			if (wFlags & DISPATCH_METHOD) {
				// line 728: [id(0x6002001a)]HRESULT ClearActions();
				hr = ClearActions();
			}
			break;
		}
		case 0x6002001b: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 730: [id(0x6002001b), propget]HRESULT AutoPack([out, retval] VARIANT_BOOL* pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_AutoPack(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 732: [id(0x6002001b), propput]HRESULT AutoPack([in] VARIANT_BOOL pRetVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_AutoPack(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 0x6002001f: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 736: [id(0x6002001f), propget]HRESULT Font([out, retval] IUnknown** pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_UNKNOWN;
				hr = get_Font(&V_UNKNOWN(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 738: [id(0x6002001f), propputref]HRESULT Font([in] IUnknown* pRetVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantCopyInd(&var0, &pDispParams->rgvarg[--index]);
				hr = putref_Font((IUnknown*)&var0);
				VariantClear(&var0);
			}
			else if (wFlags & DISPATCH_PROPERTYPUTREF) {
				// line 738: [id(0x6002001f), propputref]HRESULT Font([in] IUnknown* pRetVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantCopyInd(&var0, &pDispParams->rgvarg[--index]);
				hr = putref_Font((IUnknown*)&var0);
				VariantClear(&var0);
			}
			break;
		}
		case 0x60020021: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 740: [id(0x60020021), propget]HRESULT Text([out, retval] BSTR* pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_Text(&V_BSTR(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 742: [id(0x60020021), propput]HRESULT Text([in] BSTR pRetVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = put_Text(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		default:
		break;
	}
	if (hr != S_OK) {
		if (dispIdMember == DISPID_VALUE && wFlags == (DISPATCH_METHOD | DISPATCH_PROPERTYGET)) {
			V_VT(pVarResult) = VT_DISPATCH;
			V_DISPATCH(pVarResult) = this;
			return S_OK;
		}
		else if (hr != S_FALSE) {
			PLOGI.printf("dispId=%d (0x%08x), wFlags=%d, hr=%d", dispIdMember, dispIdMember, wFlags, hr);
		}
	}
	return hr;
}

STDMETHODIMP FlexDMDLabelActor::GetDocumentation(INT index, BSTR *pBstrName, BSTR *pBstrDocString, DWORD *pdwHelpContext, BSTR *pBstrHelpFile) {
	if (index == MEMBERID_NIL) {
		*pBstrName = SysAllocString(L"LabelActor");
		return S_OK;
	}
	return E_NOTIMPL;
}

name_disp_id_t FlexDMD::m_nameIDMap[] = {
		{ NULL },
		{ L"Clear", 0x60020015 },
		{ L"Color", 0x6002000d },
		{ L"DmdColoredPixels", 0x60020017 },
		{ L"DmdPixels", 0x60020018 },
		{ L"GameName", 0x60020007 },
		{ L"Height", 0x6002000b },
		{ L"LockRenderThread", 0x6002001b },
		{ L"NewFont", 0x60020022 },
		{ L"NewFrame", 0x6002001e },
		{ L"NewGroup", 0x6002001d },
		{ L"NewImage", 0x60020021 },
		{ L"NewLabel", 0x6002001f },
		{ L"NewUltraDMD", 0x60020023 },
		{ L"NewVideo", 0x60020020 },
		{ L"ProjectFolder", 0x60020011 },
		{ L"RenderMode", 0x6002000f },
		{ L"Run", 0x60020003 },
		{ L"RuntimeVersion", 0x60020001 },
		{ L"Segments", 0x60020019 },
		{ L"Show", 0x60020005 },
		{ L"Stage", 0x6002001a },
		{ L"TableFile", 0x60020013 },
		{ L"UnlockRenderThread", 0x6002001c },
		{ L"Version", 0x60020000 },
		{ L"Width", 0x60020009 }
};

STDMETHODIMP FlexDMD::GetIDsOfNames(REFIID /*riid*/, LPOLESTR* rgszNames, UINT cNames, LCID lcid, DISPID* rgDispId) {
	size_t min = 1, max = ARRAY_SIZE(m_nameIDMap) - 1, i;
	int r;
	while(min <= max) {
		i = (min + max) / 2;
		r = wcsicmp(m_nameIDMap[i].name, *rgszNames);
		if(!r) {
			*rgDispId = m_nameIDMap[i].dispId;
			return S_OK;
		}
		if(r < 0)
		   min = i+1;
		else
		   max = i-1;
	}
	return DISP_E_MEMBERNOTFOUND;
}

STDMETHODIMP FlexDMD::Invoke(DISPID dispIdMember, REFIID /*riid*/, LCID lcid, WORD wFlags, DISPPARAMS* pDispParams, VARIANT* pVarResult, EXCEPINFO* pExcepInfo, UINT* puArgErr) {
	HRESULT hr = DISP_E_UNKNOWNNAME;
	int index = pDispParams->cArgs;
	switch(dispIdMember) {
		case 0x60020000: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 779: [id(0x60020000), propget]HRESULT Version([out, retval] long* pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_Version((long*)&V_I4(pVarResult));
			}
			break;
		}
		case 0x60020001: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 781: [id(0x60020001), propget]HRESULT RuntimeVersion([out, retval] long* pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_RuntimeVersion((long*)&V_I4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 783: [id(0x60020001), propput]HRESULT RuntimeVersion([in] long pRetVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_I4);
				hr = put_RuntimeVersion(V_I4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 0x60020003: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 785: [id(0x60020003), propget]HRESULT Run([out, retval] VARIANT_BOOL* pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_Run(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 787: [id(0x60020003), propput]HRESULT Run([in] VARIANT_BOOL pRetVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_Run(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 0x60020005: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 789: [id(0x60020005), propget]HRESULT Show([out, retval] VARIANT_BOOL* pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_Show(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 791: [id(0x60020005), propput]HRESULT Show([in] VARIANT_BOOL pRetVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_Show(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 0x60020007: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 793: [id(0x60020007), propget]HRESULT GameName([out, retval] BSTR* pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_GameName(&V_BSTR(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 795: [id(0x60020007), propput]HRESULT GameName([in] BSTR pRetVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = put_GameName(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 0x60020009: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 797: [id(0x60020009), propget]HRESULT Width([out, retval] unsigned short* pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_UI2;
				hr = get_Width((unsigned short*)&V_UI2(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 799: [id(0x60020009), propput]HRESULT Width([in] unsigned short pRetVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_UI2);
				hr = put_Width(V_UI2(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 0x6002000b: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 801: [id(0x6002000b), propget]HRESULT Height([out, retval] unsigned short* pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_UI2;
				hr = get_Height((unsigned short*)&V_UI2(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 803: [id(0x6002000b), propput]HRESULT Height([in] unsigned short pRetVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_UI2);
				hr = put_Height(V_UI2(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 0x6002000d: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 805: [id(0x6002000d), propget]HRESULT Color([out, retval] OLE_COLOR* pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_UI4;
				hr = get_Color(&V_UI4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 807: [id(0x6002000d), propput]HRESULT Color([in] OLE_COLOR pRetVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_UI4);
				hr = put_Color((OLE_COLOR)V_UI4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 0x6002000f: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 809: [id(0x6002000f), propget]HRESULT RenderMode([out, retval] RenderMode* pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_RenderMode((RenderMode*)&V_I4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 811: [id(0x6002000f), propput]HRESULT RenderMode([in] RenderMode pRetVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_I4);
				hr = put_RenderMode((RenderMode)V_I4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 0x60020011: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 813: [id(0x60020011), propget]HRESULT ProjectFolder([out, retval] BSTR* pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_ProjectFolder(&V_BSTR(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 815: [id(0x60020011), propput]HRESULT ProjectFolder([in] BSTR pRetVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = put_ProjectFolder(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 0x60020013: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 817: [id(0x60020013), propget]HRESULT TableFile([out, retval] BSTR* pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_TableFile(&V_BSTR(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 819: [id(0x60020013), propput]HRESULT TableFile([in] BSTR pRetVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = put_TableFile(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 0x60020015: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 821: [id(0x60020015), propget]HRESULT Clear([out, retval] VARIANT_BOOL* pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_Clear(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 823: [id(0x60020015), propput]HRESULT Clear([in] VARIANT_BOOL pRetVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_Clear(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 0x60020017: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 825: [id(0x60020017), propget]HRESULT DmdColoredPixels([out, retval] VARIANT* pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				hr = get_DmdColoredPixels(pVarResult);
			}
			break;
		}
		case 0x60020018: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 827: [id(0x60020018), propget]HRESULT DmdPixels([out, retval] VARIANT* pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				hr = get_DmdPixels(pVarResult);
			}
			break;
		}
		case 0x60020019: {
			if (wFlags & DISPATCH_PROPERTYPUTREF) {
				// line 829: [id(0x60020019), propputref]HRESULT Segments([in] VARIANT rhs);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantCopyInd(&var0, &pDispParams->rgvarg[--index]);
				hr = putref_Segments(var0);
				VariantClear(&var0);
			}
			break;
		}
		case 0x6002001a: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 831: [id(0x6002001a), propget]HRESULT Stage([out, retval] IGroupActor** pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_DISPATCH;
				hr = get_Stage((IGroupActor**)&V_DISPATCH(pVarResult));
			}
			break;
		}
		case 0x6002001b: {
			if (wFlags & DISPATCH_METHOD) {
				// line 833: [id(0x6002001b)]HRESULT LockRenderThread();
				hr = LockRenderThread();
			}
			break;
		}
		case 0x6002001c: {
			if (wFlags & DISPATCH_METHOD) {
				// line 835: [id(0x6002001c)]HRESULT UnlockRenderThread();
				hr = UnlockRenderThread();
			}
			break;
		}
		case 0x6002001d: {
			if (wFlags & DISPATCH_METHOD) {
				// line 837: [id(0x6002001d)]HRESULT NewGroup([in] BSTR Name,[out, retval] IGroupActor** pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				V_VT(pVarResult) = VT_DISPATCH;
				hr = NewGroup(V_BSTR(&var0), (IGroupActor**)&V_DISPATCH(pVarResult));
				VariantClear(&var0);
			}
			break;
		}
		case 0x6002001e: {
			if (wFlags & DISPATCH_METHOD) {
				// line 841: [id(0x6002001e)]HRESULT NewFrame([in] BSTR Name,[out, retval] IFrameActor** pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				V_VT(pVarResult) = VT_DISPATCH;
				hr = NewFrame(V_BSTR(&var0), (IFrameActor**)&V_DISPATCH(pVarResult));
				VariantClear(&var0);
			}
			break;
		}
		case 0x6002001f: {
			if (wFlags & DISPATCH_METHOD) {
				// line 845: [id(0x6002001f)]HRESULT NewLabel([in] BSTR Name,[in] IUnknown* Font,[in] BSTR Text,[out, retval] ILabelActor** pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				VARIANT var1;
				V_VT(&var1) = VT_EMPTY;
				VariantCopyInd(&var1, &pDispParams->rgvarg[--index]);
				VARIANT var2;
				V_VT(&var2) = VT_EMPTY;
				VariantChangeType(&var2, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				V_VT(pVarResult) = VT_DISPATCH;
				hr = NewLabel(V_BSTR(&var0), (IUnknown*)&var1, V_BSTR(&var2), (ILabelActor**)&V_DISPATCH(pVarResult));
				VariantClear(&var0);
				VariantClear(&var1);
				VariantClear(&var2);
			}
			break;
		}
		case 0x60020020: {
			if (wFlags & DISPATCH_METHOD) {
				// line 851: [id(0x60020020)]HRESULT NewVideo([in] BSTR Name,[in] BSTR video,[out, retval] IVideoActor** pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				VARIANT var1;
				V_VT(&var1) = VT_EMPTY;
				VariantChangeType(&var1, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				V_VT(pVarResult) = VT_DISPATCH;
				hr = NewVideo(V_BSTR(&var0), V_BSTR(&var1), (IVideoActor**)&V_DISPATCH(pVarResult));
				VariantClear(&var0);
				VariantClear(&var1);
			}
			break;
		}
		case 0x60020021: {
			if (wFlags & DISPATCH_METHOD) {
				// line 856: [id(0x60020021)]HRESULT NewImage([in] BSTR Name,[in] BSTR image,[out, retval] IImageActor** pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				VARIANT var1;
				V_VT(&var1) = VT_EMPTY;
				VariantChangeType(&var1, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				V_VT(pVarResult) = VT_DISPATCH;
				hr = NewImage(V_BSTR(&var0), V_BSTR(&var1), (IImageActor**)&V_DISPATCH(pVarResult));
				VariantClear(&var0);
				VariantClear(&var1);
			}
			break;
		}
		case 0x60020022: {
			if (wFlags & DISPATCH_METHOD) {
				// line 861: [id(0x60020022)]HRESULT NewFont([in] BSTR Font,[in] OLE_COLOR tint,[in] OLE_COLOR borderTint,[in] long borderSize,[out, retval] IUnknown** pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				VARIANT var1;
				V_VT(&var1) = VT_EMPTY;
				VariantChangeType(&var1, &pDispParams->rgvarg[--index], 0, VT_UI4);
				VARIANT var2;
				V_VT(&var2) = VT_EMPTY;
				VariantChangeType(&var2, &pDispParams->rgvarg[--index], 0, VT_UI4);
				VARIANT var3;
				V_VT(&var3) = VT_EMPTY;
				VariantChangeType(&var3, &pDispParams->rgvarg[--index], 0, VT_I4);
				V_VT(pVarResult) = VT_UNKNOWN;
				hr = NewFont(V_BSTR(&var0), (OLE_COLOR)V_UI4(&var1), (OLE_COLOR)V_UI4(&var2), V_I4(&var3), &V_UNKNOWN(pVarResult));
				VariantClear(&var0);
				VariantClear(&var1);
				VariantClear(&var2);
				VariantClear(&var3);
			}
			break;
		}
		case 0x60020023: {
			if (wFlags & DISPATCH_METHOD) {
				// line 868: [id(0x60020023)]HRESULT NewUltraDMD([out, retval] IUltraDMD** pRetVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_DISPATCH;
				hr = NewUltraDMD((IUltraDMD**)&V_DISPATCH(pVarResult));
			}
			break;
		}
		default:
		break;
	}
	if (hr != S_OK) {
		if (dispIdMember == DISPID_VALUE && wFlags == (DISPATCH_METHOD | DISPATCH_PROPERTYGET)) {
			V_VT(pVarResult) = VT_DISPATCH;
			V_DISPATCH(pVarResult) = this;
			return S_OK;
		}
		else if (hr != S_FALSE) {
			PLOGI.printf("dispId=%d (0x%08x), wFlags=%d, hr=%d", dispIdMember, dispIdMember, wFlags, hr);
		}
	}
	return hr;
}

STDMETHODIMP FlexDMD::GetDocumentation(INT index, BSTR *pBstrName, BSTR *pBstrDocString, DWORD *pdwHelpContext, BSTR *pBstrHelpFile) {
	if (index == MEMBERID_NIL) {
		*pBstrName = SysAllocString(L"FlexDMD");
		return S_OK;
	}
	return E_NOTIMPL;
}

