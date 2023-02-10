#include "stdafx.h"
#include "olectl.h"

extern "C" int wcsicmp( LPCWSTR str1, LPCWSTR str2 );

name_disp_id_t WMPControls::m_nameIDMap[] = {
		{ NULL },
		{ L"currentItem", 0x003c },
		{ L"currentMarker", 0x003d },
		{ L"currentPosition", 0x0038 },
		{ L"currentPositionString", 0x0039 },
		{ L"fastForward", 0x0036 },
		{ L"fastReverse", 0x0037 },
		{ L"isAvailable", 0x003e },
		{ L"next", 0x003a },
		{ L"pause", 0x0035 },
		{ L"play", 0x0033 },
		{ L"playItem", 0x003f },
		{ L"previous", 0x003b },
		{ L"stop", 0x0034 }
};

STDMETHODIMP WMPControls::GetIDsOfNames(REFIID /*riid*/, LPOLESTR* rgszNames, UINT cNames, LCID lcid, DISPID* rgDispId) {
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

STDMETHODIMP WMPControls::Invoke(DISPID dispIdMember, REFIID /*riid*/, LCID lcid, WORD wFlags, DISPPARAMS* pDispParams, VARIANT* pVarResult, EXCEPINFO* pExcepInfo, UINT* puArgErr) {
	HRESULT hr = DISP_E_UNKNOWNNAME;
	int index = pDispParams->cArgs;
	switch(dispIdMember) {
		case 0x003e: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 313: [id(0x003e), propget]HRESULT isAvailable([in] BSTR bstrItem,[out, retval] VARIANT_BOOL *pIsAvailable);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				V_VT(pVarResult) = VT_BOOL;
				hr = get_isAvailable(V_BSTR(&var0), &V_BOOL(pVarResult));
				VariantClear(&var0);
			}
			break;
		}
		case 0x0033: {
			if (wFlags & DISPATCH_METHOD) {
				// line 318: [id(0x0033)]HRESULT play();
				hr = play();
			}
			break;
		}
		case 0x0034: {
			if (wFlags & DISPATCH_METHOD) {
				// line 321: [id(0x0034)]HRESULT stop();
				hr = stop();
			}
			break;
		}
		case 0x0035: {
			if (wFlags & DISPATCH_METHOD) {
				// line 324: [id(0x0035)]HRESULT pause();
				hr = pause();
			}
			break;
		}
		case 0x0036: {
			if (wFlags & DISPATCH_METHOD) {
				// line 327: [id(0x0036)]HRESULT fastForward();
				hr = fastForward();
			}
			break;
		}
		case 0x0037: {
			if (wFlags & DISPATCH_METHOD) {
				// line 330: [id(0x0037)]HRESULT fastReverse();
				hr = fastReverse();
			}
			break;
		}
		case 0x0038: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 333: [id(0x0038), propget]HRESULT currentPosition([out, retval] double *pdCurrentPosition);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R8;
				hr = get_currentPosition((double*)&V_R8(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 335: [id(0x0038), propput]HRESULT currentPosition([in] double pdCurrentPosition);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R8);
				hr = put_currentPosition(V_R8(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 0x0039: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 338: [id(0x0039), propget]HRESULT currentPositionString([out, retval] BSTR *pbstrCurrentPosition);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_currentPositionString(&V_BSTR(pVarResult));
			}
			break;
		}
		case 0x003a: {
			if (wFlags & DISPATCH_METHOD) {
				// line 341: [id(0x003a)]HRESULT next();
				hr = next();
			}
			break;
		}
		case 0x003b: {
			if (wFlags & DISPATCH_METHOD) {
				// line 344: [id(0x003b)]HRESULT previous();
				hr = previous();
			}
			break;
		}
		case 0x003c: {
			if (wFlags & DISPATCH_METHOD) {
				// line 347: [id(0x003c)]HRESULT currentItem([out, retval] IWMPMedia **ppIWMPMedia);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_DISPATCH;
				hr = currentItem((IWMPMedia**)&V_DISPATCH(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 350: [id(0x003c), propput]HRESULT currentItem([in] IWMPMedia *ppIWMPMedia);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantCopyInd(&var0, &pDispParams->rgvarg[--index]);
				hr = put_currentItem((IWMPMedia*)&var0);
				VariantClear(&var0);
			}
			break;
		}
		case 0x003d: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 353: [id(0x003d), propget]HRESULT currentMarker([out, retval] long *plMarker);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_currentMarker((long*)&V_I4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 356: [id(0x003d), propput]HRESULT currentMarker([in] long plMarker);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_I4);
				hr = put_currentMarker(V_I4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 0x003f: {
			if (wFlags & DISPATCH_METHOD) {
				// line 359: [id(0x003f)]HRESULT playItem([in] IWMPMedia *pIWMPMedia);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantCopyInd(&var0, &pDispParams->rgvarg[--index]);
				hr = playItem((IWMPMedia*)&var0);
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

STDMETHODIMP WMPControls::GetDocumentation(INT index, BSTR *pBstrName, BSTR *pBstrDocString, DWORD *pdwHelpContext, BSTR *pBstrHelpFile) {
	if (index == MEMBERID_NIL) {
		*pBstrName = SysAllocString(L"WMPControls");
		return S_OK;
	}
	return E_NOTIMPL;
}

name_disp_id_t WMPSettings::m_nameIDMap[] = {
		{ NULL },
		{ L"autoStart", 0x0065 },
		{ L"balance", 0x0066 },
		{ L"baseURL", 0x006c },
		{ L"defaultFrame", 0x006d },
		{ L"enableErrorDialogs", 0x0070 },
		{ L"getMode", 0x006e },
		{ L"invokeURLs", 0x0067 },
		{ L"isAvailable", 0x0071 },
		{ L"mute", 0x0068 },
		{ L"playCount", 0x0069 },
		{ L"rate", 0x006a },
		{ L"setMode", 0x006f },
		{ L"volume", 0x006b }
};

STDMETHODIMP WMPSettings::GetIDsOfNames(REFIID /*riid*/, LPOLESTR* rgszNames, UINT cNames, LCID lcid, DISPID* rgDispId) {
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

STDMETHODIMP WMPSettings::Invoke(DISPID dispIdMember, REFIID /*riid*/, LCID lcid, WORD wFlags, DISPPARAMS* pDispParams, VARIANT* pVarResult, EXCEPINFO* pExcepInfo, UINT* puArgErr) {
	HRESULT hr = DISP_E_UNKNOWNNAME;
	int index = pDispParams->cArgs;
	switch(dispIdMember) {
		case 0x0071: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 399: [id(0x0071), propget]HRESULT isAvailable([in] BSTR bstrItem,[out, retval] VARIANT_BOOL *pIsAvailable);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				V_VT(pVarResult) = VT_BOOL;
				hr = get_isAvailable(V_BSTR(&var0), &V_BOOL(pVarResult));
				VariantClear(&var0);
			}
			break;
		}
		case 0x0065: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 404: [id(0x0065), propget]HRESULT autoStart([out, retval] VARIANT_BOOL *pfAutoStart);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_autoStart(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 406: [id(0x0065), propput]HRESULT autoStart([in] VARIANT_BOOL pfAutoStart);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_autoStart(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 0x006c: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 409: [id(0x006c), propget]HRESULT baseURL([out, retval] BSTR *pbstrBaseURL);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_baseURL(&V_BSTR(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 411: [id(0x006c), propput]HRESULT baseURL([in] BSTR pbstrBaseURL);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = put_baseURL(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 0x006d: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 414: [id(0x006d), propget]HRESULT defaultFrame([out, retval] BSTR *pbstrDefaultFrame);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_defaultFrame(&V_BSTR(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 416: [id(0x006d), propput]HRESULT defaultFrame([in] BSTR pbstrDefaultFrame);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = put_defaultFrame(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 0x0067: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 419: [id(0x0067), propget]HRESULT invokeURLs([out, retval] VARIANT_BOOL *pfInvokeURLs);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_invokeURLs(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 421: [id(0x0067), propput]HRESULT invokeURLs([in] VARIANT_BOOL pfInvokeURLs);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_invokeURLs(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 0x0068: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 424: [id(0x0068), propget]HRESULT mute([out, retval] VARIANT_BOOL *pfMute);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_mute(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 426: [id(0x0068), propput]HRESULT mute([in] VARIANT_BOOL pfMute);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_mute(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 0x0069: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 429: [id(0x0069), propget]HRESULT playCount([out, retval] long *plCount);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_playCount((long*)&V_I4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 431: [id(0x0069), propput]HRESULT playCount([in] long plCount);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_I4);
				hr = put_playCount(V_I4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 0x006a: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 434: [id(0x006a), propget]HRESULT rate([out, retval] double *pdRate);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R8;
				hr = get_rate((double*)&V_R8(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 436: [id(0x006a), propput]HRESULT rate([in] double pdRate);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R8);
				hr = put_rate(V_R8(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 0x0066: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 439: [id(0x0066), propget]HRESULT balance([out, retval] long *plBalance);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_balance((long*)&V_I4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 441: [id(0x0066), propput]HRESULT balance([in] long plBalance);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_I4);
				hr = put_balance(V_I4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 0x006b: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 444: [id(0x006b), propget]HRESULT volume([out, retval] long *plVolume);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_volume((long*)&V_I4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 446: [id(0x006b), propput]HRESULT volume([in] long plVolume);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_I4);
				hr = put_volume(V_I4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 0x006e: {
			if (wFlags & DISPATCH_METHOD) {
				// line 449: [id(0x006e)]HRESULT getMode([in] BSTR bstrMode,[out, retval] VARIANT_BOOL *pvarfMode);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				V_VT(pVarResult) = VT_BOOL;
				hr = getMode(V_BSTR(&var0), &V_BOOL(pVarResult));
				VariantClear(&var0);
			}
			break;
		}
		case 0x006f: {
			if (wFlags & DISPATCH_METHOD) {
				// line 454: [id(0x006f)]HRESULT setMode([in] BSTR bstrMode,[in] VARIANT_BOOL varfMode);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				VARIANT var1;
				V_VT(&var1) = VT_EMPTY;
				VariantChangeType(&var1, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = setMode(V_BSTR(&var0), V_BOOL(&var1));
				VariantClear(&var0);
				VariantClear(&var1);
			}
			break;
		}
		case 0x0070: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 459: [id(0x0070), propget]HRESULT enableErrorDialogs([out, retval] VARIANT_BOOL *pfEnableErrorDialogs);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_enableErrorDialogs(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 462: [id(0x0070), propput]HRESULT enableErrorDialogs([in] VARIANT_BOOL pfEnableErrorDialogs);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_enableErrorDialogs(V_BOOL(&var0));
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

STDMETHODIMP WMPSettings::GetDocumentation(INT index, BSTR *pBstrName, BSTR *pBstrDocString, DWORD *pdwHelpContext, BSTR *pBstrHelpFile) {
	if (index == MEMBERID_NIL) {
		*pBstrName = SysAllocString(L"WMPSettings");
		return S_OK;
	}
	return E_NOTIMPL;
}

name_disp_id_t WMPCore::m_nameIDMap[] = {
		{ NULL },
		{ L"cdromCollection", 0x000e },
		{ L"close", 0x0003 },
		{ L"closedCaption", 0x000f },
		{ L"controls", 0x0004 },
		{ L"currentMedia", 0x0006 },
		{ L"currentPlaylist", 0x000d },
		{ L"Error", 0x0011 },
		{ L"isOnline", 0x0010 },
		{ L"launchURL", 0x000c },
		{ L"mediaCollection", 0x0008 },
		{ L"network", 0x0007 },
		{ L"openState", 0x0002 },
		{ L"playlistCollection", 0x0009 },
		{ L"playState", 0x000a },
		{ L"settings", 0x0005 },
		{ L"status", 0x0012 },
		{ L"URL", 0x0001 },
		{ L"versionInfo", 0x000b }
};

STDMETHODIMP WMPCore::GetIDsOfNames(REFIID /*riid*/, LPOLESTR* rgszNames, UINT cNames, LCID lcid, DISPID* rgDispId) {
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

STDMETHODIMP WMPCore::Invoke(DISPID dispIdMember, REFIID /*riid*/, LCID lcid, WORD wFlags, DISPPARAMS* pDispParams, VARIANT* pVarResult, EXCEPINFO* pExcepInfo, UINT* puArgErr) {
	HRESULT hr = DISP_E_UNKNOWNNAME;
	int index = pDispParams->cArgs;
	switch(dispIdMember) {
		case 0x0003: {
			if (wFlags & DISPATCH_METHOD) {
				// line 925: [id(0x0003)]HRESULT close();
				hr = close();
			}
			break;
		}
		case 0x0001: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 928: [id(0x0001), propget]HRESULT URL([out, retval] BSTR *pbstrURL);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_URL(&V_BSTR(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 930: [id(0x0001), propput]HRESULT URL([in] BSTR pbstrURL);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = put_URL(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 0x0002: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 933: [id(0x0002), propget]HRESULT openState([out, retval] WMPOpenState *pwmpos);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_openState((WMPOpenState*)&V_I4(pVarResult));
			}
			break;
		}
		case 0x000a: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 936: [id(0x000a), propget]HRESULT playState([out, retval] WMPPlayState *pwmpps);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_playState((WMPPlayState*)&V_I4(pVarResult));
			}
			break;
		}
		case 0x0004: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 939: [id(0x0004), propget]HRESULT controls([out, retval] IWMPControls **ppControl);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_DISPATCH;
				hr = get_controls((IWMPControls**)&V_DISPATCH(pVarResult));
			}
			break;
		}
		case 0x0005: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 942: [id(0x0005), propget]HRESULT settings([out, retval] IWMPSettings **ppSettings);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_DISPATCH;
				hr = get_settings((IWMPSettings**)&V_DISPATCH(pVarResult));
			}
			break;
		}
		case 0x0006: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 945: [id(0x0006), propget]HRESULT currentMedia([out, retval] IWMPMedia **ppMedia);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_DISPATCH;
				hr = get_currentMedia((IWMPMedia**)&V_DISPATCH(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 947: [id(0x0006), propput]HRESULT currentMedia([in] IWMPMedia *ppMedia);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantCopyInd(&var0, &pDispParams->rgvarg[--index]);
				hr = put_currentMedia((IWMPMedia*)&var0);
				VariantClear(&var0);
			}
			break;
		}
		case 0x0008: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 950: [id(0x0008), propget]HRESULT mediaCollection([out, retval] IWMPMediaCollection **ppMediaCollection);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_DISPATCH;
				hr = get_mediaCollection((IWMPMediaCollection**)&V_DISPATCH(pVarResult));
			}
			break;
		}
		case 0x0009: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 953: [id(0x0009), propget]HRESULT playlistCollection([out, retval] IWMPPlaylistCollection **ppPlaylistCollection);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_DISPATCH;
				hr = get_playlistCollection((IWMPPlaylistCollection**)&V_DISPATCH(pVarResult));
			}
			break;
		}
		case 0x000b: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 956: [id(0x000b), propget]HRESULT versionInfo([out, retval] BSTR *pbstrVersionInfo);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_versionInfo(&V_BSTR(pVarResult));
			}
			break;
		}
		case 0x000c: {
			if (wFlags & DISPATCH_METHOD) {
				// line 959: [id(0x000c)]HRESULT launchURL([in] BSTR bstrURL);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = launchURL(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 0x0007: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 962: [id(0x0007), propget]HRESULT network([out, retval] IWMPNetwork **ppQNI);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_DISPATCH;
				hr = get_network((IWMPNetwork**)&V_DISPATCH(pVarResult));
			}
			break;
		}
		case 0x000d: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 965: [id(0x000d), propget]HRESULT currentPlaylist([out, retval] IWMPPlaylist **ppPL);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_DISPATCH;
				hr = get_currentPlaylist((IWMPPlaylist**)&V_DISPATCH(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 967: [id(0x000d), propput]HRESULT currentPlaylist([in] IWMPPlaylist *ppPL);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantCopyInd(&var0, &pDispParams->rgvarg[--index]);
				hr = put_currentPlaylist((IWMPPlaylist*)&var0);
				VariantClear(&var0);
			}
			break;
		}
		case 0x000e: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 970: [id(0x000e), propget]HRESULT cdromCollection([out, retval] IWMPCdromCollection **ppCdromCollection);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_DISPATCH;
				hr = get_cdromCollection((IWMPCdromCollection**)&V_DISPATCH(pVarResult));
			}
			break;
		}
		case 0x000f: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 973: [id(0x000f), propget]HRESULT closedCaption([out, retval] IWMPClosedCaption **ppClosedCaption);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_DISPATCH;
				hr = get_closedCaption((IWMPClosedCaption**)&V_DISPATCH(pVarResult));
			}
			break;
		}
		case 0x0010: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 976: [id(0x0010), propget]HRESULT isOnline([out, retval] VARIANT_BOOL *pfOnline);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_isOnline(&V_BOOL(pVarResult));
			}
			break;
		}
		case 0x0011: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 979: [id(0x0011), propget]HRESULT Error([out, retval] IWMPError **ppError);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_DISPATCH;
				hr = get_Error((IWMPError**)&V_DISPATCH(pVarResult));
			}
			break;
		}
		case 0x0012: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 982: [id(0x0012), propget]HRESULT status([out, retval] BSTR *pbstrStatus);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_status(&V_BSTR(pVarResult));
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

STDMETHODIMP WMPCore::GetDocumentation(INT index, BSTR *pBstrName, BSTR *pBstrDocString, DWORD *pdwHelpContext, BSTR *pBstrHelpFile) {
	if (index == MEMBERID_NIL) {
		*pBstrName = SysAllocString(L"WMPCore");
		return S_OK;
	}
	return E_NOTIMPL;
}

