#include "stdafx.h"
#include "olectl.h"

extern "C" int wcsicmp( LPCWSTR str1, LPCWSTR str2 );

name_disp_id_t Collection::m_nameIDMap[] = {
		{ NULL },
		{ L"Count", 8000 }
};

STDMETHODIMP Collection::GetIDsOfNames(REFIID /*riid*/, LPOLESTR* rgszNames, UINT cNames, LCID lcid, DISPID* rgDispId) {
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

STDMETHODIMP Collection::Invoke(DISPID dispIdMember, REFIID /*riid*/, LCID lcid, WORD wFlags, DISPPARAMS* pDispParams, VARIANT* pVarResult, EXCEPINFO* pExcepInfo, UINT* puArgErr) {
	HRESULT hr = DISP_E_UNKNOWNNAME;
	int index = pDispParams->cArgs;
	switch(dispIdMember) {
		case DISPID_NEWENUM: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 447: [propget, id(DISPID_NEWENUM),helpcontext(0x5009002)]HRESULT _NewEnum([out, retval] IUnknown** ppunk);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_UNKNOWN;
				hr = get__NewEnum(&V_UNKNOWN(pVarResult));
			}
			break;
		}
		case DISPID_VALUE: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 455: [propget, id(0),helpcontext(0x5009003)]HRESULT Item([in] long Index, [out, retval] IDispatch **ppobject);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_I4);
				V_VT(pVarResult) = VT_DISPATCH;
				hr = get_Item(V_I4(&var0), (IDispatch**)&V_DISPATCH(pVarResult));
				VariantClear(&var0);
			}
			break;
		}
		case 8000: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 459: [propget,helpcontext(0x5009004)]HRESULT Count([out, retval] long *Count);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_Count((long*)&V_I4(pVarResult));
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

STDMETHODIMP Collection::GetDocumentation(INT index, BSTR *pBstrName, BSTR *pBstrDocString, DWORD *pdwHelpContext, BSTR *pBstrHelpFile) {
	if (index == MEMBERID_NIL) {
		*pBstrName = SysAllocString(L"Collection");
		return S_OK;
	}
	return E_NOTIMPL;
}

disp_id_name_t Collection::m_idNameMap[] = {
		{ NULL },
		{ DISPID_GameEvents_Init, L"_Init" },
		{ DISPID_SurfaceEvents_Slingshot, L"_Slingshot" },
		{ DISPID_TimerEvents_Timer, L"_Timer" },
		{ DISPID_SpinnerEvents_Spin, L"_Spin" },
		{ DISPID_TargetEvents_Dropped, L"_Dropped" },
		{ DISPID_TargetEvents_Raised, L"_Raised" },
		{ DISPID_HitEvents_Hit, L"_Hit" },
		{ DISPID_HitEvents_Unhit, L"_Unhit" }
};

HRESULT Collection::FireDispID(const DISPID dispid, DISPPARAMS * const pdispparams) {
	static WCHAR wzName[MAXSTRING];
	CComPtr<IDispatch> disp;
	g_pplayer->m_ptable->m_pcv->m_pScript->GetScriptDispatch(nullptr, &disp);
	size_t min = 1, max = ARRAY_SIZE(m_idNameMap) - 1, i;
	int r;
	while(min <= max) {
		i = (min + max) / 2;
		if (m_idNameMap[i].dispId == dispid) {
			wcscpy(wzName, m_wzName);
			wcscat(wzName, m_idNameMap[i].name);
			LPOLESTR fnNames = (LPOLESTR)wzName;
			DISPID tDispid;
			if (SUCCEEDED(disp->GetIDsOfNames(IID_NULL, &fnNames, 1, 0, &tDispid))) {
				return disp->Invoke(tDispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, pdispparams, nullptr, nullptr, nullptr);
			}
			return DISP_E_MEMBERNOTFOUND;
		}
		else if (m_idNameMap[i].dispId < dispid)
		   min = i+1;
		else
		   max = i-1;
	}
	return DISP_E_MEMBERNOTFOUND;
}

name_disp_id_t PinTable::m_nameIDMap[] = {
		{ NULL },
		{ L"Accelerometer", 200 },
		{ L"AccelerometerAngle", 202 },
		{ L"AccelNormalMount", 201 },
		{ L"AOScale", 568 },
		{ L"BackdropColor", 5 },
		{ L"BackdropImage_DT", DISPID_Image2 },
		{ L"BackdropImage_FS", DISPID_Image6 },
		{ L"BackdropImage_FSS", DISPID_Image8 },
		{ L"BackdropImageApplyNightDay", 459 },
		{ L"BackglassMode", 1714 },
		{ L"BallDecalMode", 438 },
		{ L"BallFrontDecal", DISPID_Image4 },
		{ L"BallImage", DISPID_Image3 },
		{ L"BallPlayfieldReflectionScale", 1712 },
		{ L"BallReflection", 1700 },
		{ L"BallTrail", 1704 },
		{ L"BloomStrength", 450 },
		{ L"ColorGradeImage", DISPID_Image5 },
		{ L"DeadZone", 217 },
		{ L"DefaultBulbIntensityScale", 1713 },
		{ L"DefaultScatter", 1102 },
		{ L"DetailLevel", 420 },
		{ L"Elasticity", 1708 },
		{ L"ElasticityFalloff", 1709 },
		{ L"EnableAntialiasing", 394 },
		{ L"EnableAO", 396 },
		{ L"EnableDecals", 13433 },
		{ L"EnableEMReels", 13432 },
		{ L"EnableFXAA", 395 },
		{ L"EnableSSR", 590 },
		{ L"EnvironmentEmissionScale", 566 },
		{ L"EnvironmentImage", DISPID_Image7 },
		{ L"FieldOfView", DISPID_Table_FieldOfView },
		{ L"FileName", 1711 },
		{ L"Friction", 1101 },
		{ L"GlassHeight", 3 },
		{ L"GlobalAlphaAcc", 398 },
		{ L"GlobalDayNight", 588 },
		{ L"GlobalDifficulty", 209 },
		{ L"GlobalStereo3D", 427 },
		{ L"Gravity", 1100 },
		{ L"Height", DISPID_Table_Height },
		{ L"Image", DISPID_Image },
		{ L"Inclination", DISPID_Table_Inclination },
		{ L"Layback", DISPID_Table_Layback },
		{ L"Light0Emission", 559 },
		{ L"LightAmbient", 558 },
		{ L"LightEmissionScale", 567 },
		{ L"LightHeight", 564 },
		{ L"LightRange", 565 },
		{ L"MaxSeparation", DISPID_Table_MaxSeparation },
		{ L"Name", DISPID_Name },
		{ L"NightDay", 436 },
		{ L"NudgeTime", 1103 },
		{ L"Offset", DISPID_Table_Offset },
		{ L"OverridePhysics", DISPID_Table_OverridePhysics },
		{ L"OverridePhysicsFlippers", 584 },
		{ L"PhysicsLoopTime", 1105 },
		{ L"PlayfieldMaterial", 340 },
		{ L"PlayfieldReflectionStrength", 1707 },
		{ L"PlungerFilter", 1107 },
		{ L"PlungerNormalize", 1104 },
		{ L"ReflectElementsOnPlayfield", 431 },
		{ L"Rotation", 99 },
		{ L"Scalex", 100 },
		{ L"Scaley", 101 },
		{ L"Scalez", 108 },
		{ L"Scatter", 1710 },
		{ L"ShowDT", 13434 },
		{ L"ShowFSS", 625 },
		{ L"SlopeMax", 215 },
		{ L"SlopeMin", 6 },
		{ L"SSRScale", 569 },
		{ L"TableAdaptiveVSync", 585 },
		{ L"TableHeight", 214 },
		{ L"TableMusicVolume", 580 },
		{ L"TableSoundVolume", 579 },
		{ L"TrailStrength", 1705 },
		{ L"Version", 219 },
		{ L"VersionMajor", 38 },
		{ L"VersionMinor", 39 },
		{ L"VersionRevision", 40 },
		{ L"VPBuildVersion", 24 },
		{ L"Width", DISPID_Table_Width },
		{ L"Xlatex", 102 },
		{ L"Xlatey", 103 },
		{ L"Xlatez", 110 },
		{ L"YieldTime", 7 },
		{ L"ZPD", DISPID_Table_ZPD }
};

STDMETHODIMP PinTable::GetIDsOfNames(REFIID /*riid*/, LPOLESTR* rgszNames, UINT cNames, LCID lcid, DISPID* rgDispId) {
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

STDMETHODIMP PinTable::Invoke(DISPID dispIdMember, REFIID /*riid*/, LCID lcid, WORD wFlags, DISPPARAMS* pDispParams, VARIANT* pVarResult, EXCEPINFO* pExcepInfo, UINT* puArgErr) {
	HRESULT hr = DISP_E_UNKNOWNNAME;
	int index = pDispParams->cArgs;
	switch(dispIdMember) {
		case 3: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 494: [propget, id(3), helpstring("property GlassHeight")] HRESULT GlassHeight([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_GlassHeight(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 495: [propput, id(3), helpstring("property GlassHeight")] HRESULT GlassHeight([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_GlassHeight(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 340: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 496: [propget, id(340), helpstring("property PlayfieldMaterial")] HRESULT PlayfieldMaterial([out, retval] BSTR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_PlayfieldMaterial(&V_BSTR(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 497: [propput, id(340), helpstring("property PlayfieldMaterial")] HRESULT PlayfieldMaterial([in] BSTR newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = put_PlayfieldMaterial(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 5: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 498: [propget, id(5), helpstring("property BackdropColor")] HRESULT BackdropColor([out, retval] OLE_COLOR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_UI4;
				hr = get_BackdropColor(&V_UI4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 499: [propput, id(5), helpstring("property BackdropColor")] HRESULT BackdropColor([in] OLE_COLOR newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_UI4);
				hr = put_BackdropColor((OLE_COLOR)V_UI4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 215: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 500: [propget, id(215), helpstring("property SlopeMax")] HRESULT SlopeMax([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_SlopeMax(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 501: [propput, id(215), helpstring("property SlopeMax")] HRESULT SlopeMax([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_SlopeMax(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 6: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 502: [propget, id(6), helpstring("property SlopeMin")] HRESULT SlopeMin([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_SlopeMin(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 503: [propput, id(6), helpstring("property SlopeMin")] HRESULT SlopeMin([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_SlopeMin(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Table_Inclination: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 505: [propget, id(DISPID_Table_Inclination), helpstring("property Inclination")] HRESULT Inclination([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Inclination(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 506: [propput, id(DISPID_Table_Inclination), helpstring("property Inclination")] HRESULT Inclination([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Inclination(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Table_FieldOfView: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 507: [propget, id(DISPID_Table_FieldOfView), helpstring("property FieldOfView")] HRESULT FieldOfView([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_FieldOfView(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 508: [propput, id(DISPID_Table_FieldOfView), helpstring("property FieldOfView")] HRESULT FieldOfView([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_FieldOfView(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Table_Layback: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 509: [propget, id(DISPID_Table_Layback), helpstring("property Layback")] HRESULT Layback([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Layback(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 510: [propput, id(DISPID_Table_Layback), helpstring("property Layback")] HRESULT Layback([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Layback(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 99: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 511: [propget, id(99), helpstring("property Rotation")] HRESULT Rotation([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Rotation(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 512: [propput, id(99), helpstring("property Rotation")] HRESULT Rotation([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Rotation(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 100: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 513: [propget, id(100), helpstring("property Scalex")] HRESULT Scalex([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Scalex(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 514: [propput, id(100), helpstring("property Scalex")] HRESULT Scalex([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Scalex(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 101: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 515: [propget, id(101), helpstring("property Scaley")] HRESULT Scaley([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Scaley(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 516: [propput, id(101), helpstring("property Scaley")] HRESULT Scaley([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Scaley(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 108: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 517: [propget, id(108), helpstring("property Scalez")] HRESULT Scalez([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Scalez(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 518: [propput, id(108), helpstring("property Scalez")] HRESULT Scalez([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Scalez(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 102: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 519: [propget, id(102), helpstring("property Xlatex")] HRESULT Xlatex([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Xlatex(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 520: [propput, id(102), helpstring("property Xlatex")] HRESULT Xlatex([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Xlatex(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 103: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 521: [propget, id(103), helpstring("property Xlatey")] HRESULT Xlatey([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Xlatey(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 522: [propput, id(103), helpstring("property Xlatey")] HRESULT Xlatey([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Xlatey(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 110: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 523: [propget, id(110), helpstring("property Xlatez")] HRESULT Xlatez([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Xlatez(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 524: [propput, id(110), helpstring("property Xlatez")] HRESULT Xlatez([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Xlatez(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 1100: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 526: [propget, id(1100), helpstring("property Gravity")] HRESULT Gravity([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Gravity(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 527: [propput, id(1100), helpstring("property Gravity")] HRESULT Gravity([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Gravity(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 1101: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 528: [propget, id(1101), helpstring("property Friction")] HRESULT Friction([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Friction(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 529: [propput, id(1101), helpstring("property Friction")] HRESULT Friction([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Friction(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 1708: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 530: [propget, id(1708), helpstring("property Elasticity")] HRESULT Elasticity([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Elasticity(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 531: [propput, id(1708), helpstring("property Elasticity")] HRESULT Elasticity([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Elasticity(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 1709: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 532: [propget, id(1709), helpstring("property ElasticityFalloff")] HRESULT ElasticityFalloff([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_ElasticityFalloff(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 533: [propput, id(1709), helpstring("property ElasticityFalloff")] HRESULT ElasticityFalloff([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_ElasticityFalloff(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 1710: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 534: [propget, id(1710), helpstring("property Scatter")] HRESULT Scatter([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Scatter(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 535: [propput, id(1710), helpstring("property Scatter")] HRESULT Scatter([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Scatter(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 1102: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 536: [propget, id(1102), helpstring("property DefaultScatter")] HRESULT DefaultScatter([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_DefaultScatter(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 537: [propput, id(1102), helpstring("property DefaultScatter")] HRESULT DefaultScatter([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_DefaultScatter(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 1103: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 538: [propget, id(1103), helpstring("property NudgeTime")] HRESULT NudgeTime([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_NudgeTime(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 539: [propput, id(1103), helpstring("property NudgeTime")] HRESULT NudgeTime([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_NudgeTime(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 1104: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 540: [propget, id(1104), helpstring("property PlungerNormalize")] HRESULT PlungerNormalize([out, retval] int *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_PlungerNormalize((int*)&V_I4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 541: [propput, id(1104), helpstring("property PlungerNormalize")] HRESULT PlungerNormalize([in] int newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_I4);
				hr = put_PlungerNormalize(V_I4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 1105: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 542: [propget, id(1105), helpstring("property PhysicsLoopTime")] HRESULT PhysicsLoopTime([out, retval] int *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_PhysicsLoopTime((int*)&V_I4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 543: [propput, id(1105), helpstring("property PhysicsLoopTime")] HRESULT PhysicsLoopTime([in] int newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_I4);
				hr = put_PhysicsLoopTime(V_I4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 1107: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 545: [propget, id(1107), helpstring("property PlungerFilter")] HRESULT PlungerFilter([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_PlungerFilter(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 546: [propput, id(1107), helpstring("property PlungerFilter")] HRESULT PlungerFilter([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_PlungerFilter(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 7: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 548: [propget, id(7), helpstring("property YieldTime")] HRESULT YieldTime([out, retval] long *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_YieldTime((long*)&V_I4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 549: [propput, id(7), helpstring("property YieldTime")] HRESULT YieldTime([in] long newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_I4);
				hr = put_YieldTime(V_I4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Image3: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 550: [propget, id(DISPID_Image3), helpstring("property BallImage")] HRESULT BallImage([out, retval] BSTR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_BallImage(&V_BSTR(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 551: [propput, id(DISPID_Image3), helpstring("property BallImage")] HRESULT BallImage([in] BSTR newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = put_BallImage(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Image2: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 552: [propget, id(DISPID_Image2), helpstring("property BackdropImage_DT")] HRESULT BackdropImage_DT([out, retval] BSTR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_BackdropImage_DT(&V_BSTR(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 553: [propput, id(DISPID_Image2), helpstring("property BackdropImage_DT")] HRESULT BackdropImage_DT([in] BSTR newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = put_BackdropImage_DT(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Image6: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 554: [propget, id(DISPID_Image6), helpstring("property BackdropImage_FS")] HRESULT BackdropImage_FS([out, retval] BSTR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_BackdropImage_FS(&V_BSTR(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 555: [propput, id(DISPID_Image6), helpstring("property BackdropImage_FS")] HRESULT BackdropImage_FS([in] BSTR newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = put_BackdropImage_FS(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Image8: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 556: [propget, id(DISPID_Image8), helpstring("property BackdropImage_FSS")] HRESULT BackdropImage_FSS([out, retval] BSTR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_BackdropImage_FSS(&V_BSTR(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 557: [propput, id(DISPID_Image8), helpstring("property BackdropImage_FSS")] HRESULT BackdropImage_FSS([in] BSTR newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = put_BackdropImage_FSS(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 459: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 558: [propget, id(459), helpstring("property BackdropImageApplyNightDay")] HRESULT BackdropImageApplyNightDay([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_BackdropImageApplyNightDay(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 559: [propput, id(459), helpstring("property BackdropImageApplyNightDay")] HRESULT BackdropImageApplyNightDay([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_BackdropImageApplyNightDay(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Image5: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 560: [propget, id(DISPID_Image5), helpstring("property ColorGradeImage")] HRESULT ColorGradeImage([out, retval] BSTR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_ColorGradeImage(&V_BSTR(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 561: [propput, id(DISPID_Image5), helpstring("property ColorGradeImage")] HRESULT ColorGradeImage([in] BSTR newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = put_ColorGradeImage(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Table_Width: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 562: [propget, id(DISPID_Table_Width), helpstring("property Width")] HRESULT Width([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Width(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 563: [propput, id(DISPID_Table_Width), helpstring("property Width")] HRESULT Width([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Width(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Table_Height: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 564: [propget, id(DISPID_Table_Height), helpstring("property Height")] HRESULT Height([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Height(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 565: [propput, id(DISPID_Table_Height), helpstring("property Height")] HRESULT Height([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Height(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Table_MaxSeparation: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 566: [propget, id(DISPID_Table_MaxSeparation), helpstring("property MaxSeparation")] HRESULT MaxSeparation([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_MaxSeparation(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 567: [propput, id(DISPID_Table_MaxSeparation), helpstring("property MaxSeparation")] HRESULT MaxSeparation([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_MaxSeparation(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Table_ZPD: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 568: [propget, id(DISPID_Table_ZPD), helpstring("property ZPD")] HRESULT ZPD([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_ZPD(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 569: [propput, id(DISPID_Table_ZPD), helpstring("property ZPD")] HRESULT ZPD([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_ZPD(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Table_Offset: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 570: [propget, id(DISPID_Table_Offset), helpstring("property Offset")] HRESULT Offset([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Offset(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 571: [propput, id(DISPID_Table_Offset), helpstring("property Offset")] HRESULT Offset([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Offset(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 427: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 572: [propget, id(427), helpstring("property Overwrite3DStereo")] HRESULT GlobalStereo3D([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_GlobalStereo3D(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 573: [propput, id(427), helpstring("property Overwrite3DStereo")] HRESULT GlobalStereo3D([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_GlobalStereo3D(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 438: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 574: [propget, id(438), helpstring("property BallDecalMode")] HRESULT BallDecalMode([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_BallDecalMode(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 575: [propput, id(438), helpstring("property BallDecalMode")] HRESULT BallDecalMode([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_BallDecalMode(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Image: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 576: [propget, id(DISPID_Image), helpstring("property Image")] HRESULT Image([out, retval] BSTR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_Image(&V_BSTR(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 577: [propput, id(DISPID_Image), helpstring("property Image")] HRESULT Image([in] BSTR newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = put_Image(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 1711: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 578: [propget, id(1711), helpstring("property FileName")] HRESULT FileName([out, retval] BSTR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_FileName(&V_BSTR(pVarResult));
			}
			break;
		}
		case DISPID_Name: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 579: [propget, id(DISPID_Name), helpstring("property Name")] HRESULT Name([out, retval] BSTR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_Name(&V_BSTR(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 580: [propput, id(DISPID_Name), helpstring("property Name")] HRESULT Name([in] BSTR newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = put_Name(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 394: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 581: [propget, id(394), helpstring("property enableAA")] HRESULT EnableAntialiasing([out, retval] UserDefaultOnOff *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_EnableAntialiasing((UserDefaultOnOff*)&V_I4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 582: [propput, id(394), helpstring("property enableAA")] HRESULT EnableAntialiasing([in] UserDefaultOnOff newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_I4);
				hr = put_EnableAntialiasing((UserDefaultOnOff)V_I4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 396: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 583: [propget, id(396), helpstring("property enableAO")] HRESULT EnableAO([out, retval] UserDefaultOnOff *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_EnableAO((UserDefaultOnOff*)&V_I4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 584: [propput, id(396), helpstring("property enableAO")] HRESULT EnableAO([in] UserDefaultOnOff newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_I4);
				hr = put_EnableAO((UserDefaultOnOff)V_I4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 395: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 585: [propget, id(395), helpstring("property enableFXAA")] HRESULT EnableFXAA([out, retval] FXAASettings *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_EnableFXAA((FXAASettings*)&V_I4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 586: [propput, id(395), helpstring("property enableFXAA")] HRESULT EnableFXAA([in] FXAASettings newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_I4);
				hr = put_EnableFXAA((FXAASettings)V_I4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 590: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 587: [propget, id(590), helpstring("property enableSSR")] HRESULT EnableSSR([out, retval] UserDefaultOnOff *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_EnableSSR((UserDefaultOnOff*)&V_I4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 588: [propput, id(590), helpstring("property enableSSR")] HRESULT EnableSSR([in] UserDefaultOnOff newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_I4);
				hr = put_EnableSSR((UserDefaultOnOff)V_I4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 450: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 589: [propget, id(450), helpstring("property BloomStrength")] HRESULT BloomStrength([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_BloomStrength(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 590: [propput, id(450), helpstring("property BloomStrength")] HRESULT BloomStrength([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_BloomStrength(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Image4: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 591: [propget, id(DISPID_Image4), helpstring("property BallFrontDecal")] HRESULT BallFrontDecal([out, retval] BSTR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_BallFrontDecal(&V_BSTR(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 592: [propput, id(DISPID_Image4), helpstring("property BallFrontDecal")] HRESULT BallFrontDecal([in] BSTR newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = put_BallFrontDecal(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Table_OverridePhysics: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 593: [propget, id(DISPID_Table_OverridePhysics), helpstring("property OverridePhysics")] HRESULT OverridePhysics([out, retval] PhysicsSet *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_OverridePhysics((PhysicsSet*)&V_I4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 594: [propput, id(DISPID_Table_OverridePhysics), helpstring("property OverridePhysics")] HRESULT OverridePhysics([in] PhysicsSet newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_I4);
				hr = put_OverridePhysics((PhysicsSet)V_I4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 584: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 595: [propget, id(584), helpstring("property OverridePhysicsFlippers")] HRESULT OverridePhysicsFlippers([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_OverridePhysicsFlippers(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 596: [propput, id(584), helpstring("property OverridePhysicsFlippers")] HRESULT OverridePhysicsFlippers([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_OverridePhysicsFlippers(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 13432: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 597: [propget, id(13432), helpstring("property RenderEMReels")] HRESULT EnableEMReels([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_EnableEMReels(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 598: [propput, id(13432), helpstring("property RenderEMReels")] HRESULT EnableEMReels([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_EnableEMReels(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 13433: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 599: [propget, id(13433), helpstring("property RenderDecals")] HRESULT EnableDecals([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_EnableDecals(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 600: [propput, id(13433), helpstring("property RenderDecals")] HRESULT EnableDecals([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_EnableDecals(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 13434: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 601: [propget, id(13434), helpstring("property ShowDT")] HRESULT ShowDT([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_ShowDT(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 602: [propput, id(13434), helpstring("property ShowDT")] HRESULT ShowDT([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_ShowDT(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 625: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 603: [propget, id(625), helpstring("property ShowFSS")] HRESULT ShowFSS([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_ShowFSS(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 604: [propput, id(625), helpstring("property ShowFSS")] HRESULT ShowFSS([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_ShowFSS(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 431: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 605: [propget, id(431), helpstring("property ReflectElementsOnPlayfield")] HRESULT ReflectElementsOnPlayfield([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_ReflectElementsOnPlayfield(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 606: [propput, id(431), helpstring("property ReflectElementsOnPlayfield")] HRESULT ReflectElementsOnPlayfield([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_ReflectElementsOnPlayfield(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Image7: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 607: [propget, id(DISPID_Image7), helpstring("property EnvironmentImage")] HRESULT EnvironmentImage([out, retval] BSTR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_EnvironmentImage(&V_BSTR(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 608: [propput, id(DISPID_Image7), helpstring("property EnvironmentImage")] HRESULT EnvironmentImage([in] BSTR newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = put_EnvironmentImage(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 1714: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 609: [propget, id(1714), helpstring("property BackglassMode")] HRESULT BackglassMode([out, retval] BackglassIndex *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_BackglassMode((BackglassIndex*)&V_I4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 610: [propput, id(1714), helpstring("property BackglassMode")] HRESULT BackglassMode([in] BackglassIndex newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_I4);
				hr = put_BackglassMode((BackglassIndex)V_I4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 200: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 613: [propget, id(200), helpstring("property Accelerometer")] HRESULT Accelerometer([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_Accelerometer(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 614: [propput, id(200), helpstring("property Accelerometer")] HRESULT Accelerometer([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_Accelerometer(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 201: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 615: [propget, id(201), helpstring("property AccelNormalMount")] HRESULT AccelNormalMount([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_AccelNormalMount(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 616: [propput, id(201), helpstring("property AccelNormalMount")] HRESULT AccelNormalMount([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_AccelNormalMount(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 202: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 618: [propget, id(202), helpstring("property AccelerometerAngle")] HRESULT AccelerometerAngle([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_AccelerometerAngle(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 619: [propput, id(202), helpstring("property AccelerometerAngle")] HRESULT AccelerometerAngle([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_AccelerometerAngle(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 209: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 632: [propget, id(209), helpstring("property GlobalDifficulty")] HRESULT GlobalDifficulty([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_GlobalDifficulty(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 633: [propput, id(209), helpstring("property GlobalDifficulty")] HRESULT GlobalDifficulty([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_GlobalDifficulty(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 214: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 635: [propget, id(214), helpstring("property TableHeight")] HRESULT TableHeight([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_TableHeight(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 636: [propput, id(214), helpstring("property TableHeight")] HRESULT TableHeight([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_TableHeight(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 217: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 638: [propget, id(217), helpstring("property DeadZone")] HRESULT DeadZone([out, retval] int *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_DeadZone((int*)&V_I4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 639: [propput, id(217), helpstring("property DeadZone")] HRESULT DeadZone([in] int newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_I4);
				hr = put_DeadZone(V_I4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 558: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 641: [propget, id(558), helpstring("property LightAmbient")] HRESULT LightAmbient([out, retval] OLE_COLOR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_UI4;
				hr = get_LightAmbient(&V_UI4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 642: [propput, id(558), helpstring("property LightAmbient")] HRESULT LightAmbient([in] OLE_COLOR newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_UI4);
				hr = put_LightAmbient((OLE_COLOR)V_UI4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 559: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 643: [propget, id(559), helpstring("property Light0Emission")] HRESULT Light0Emission([out, retval] OLE_COLOR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_UI4;
				hr = get_Light0Emission(&V_UI4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 644: [propput, id(559), helpstring("property Light0Emission")] HRESULT Light0Emission([in] OLE_COLOR newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_UI4);
				hr = put_Light0Emission((OLE_COLOR)V_UI4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 564: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 645: [propget, id(564), helpstring("property LightHeight")] HRESULT LightHeight([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_LightHeight(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 646: [propput, id(564), helpstring("property LightHeight")] HRESULT LightHeight([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_LightHeight(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 565: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 647: [propget, id(565), helpstring("property LightRange")] HRESULT LightRange([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_LightRange(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 648: [propput, id(565), helpstring("property LightRange")] HRESULT LightRange([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_LightRange(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 566: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 650: [propget, id(566), helpstring("property EnvironmentEmissionScale")] HRESULT EnvironmentEmissionScale([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_EnvironmentEmissionScale(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 651: [propput, id(566), helpstring("property EnvironmentEmissionScale")] HRESULT EnvironmentEmissionScale([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_EnvironmentEmissionScale(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 567: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 652: [propget, id(567), helpstring("property LightEmissionScale")] HRESULT LightEmissionScale([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_LightEmissionScale(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 653: [propput, id(567), helpstring("property LightEmissionScale")] HRESULT LightEmissionScale([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_LightEmissionScale(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 568: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 654: [propget, id(568), helpstring("property AOScale")] HRESULT AOScale([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_AOScale(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 655: [propput, id(568), helpstring("property AOScale")] HRESULT AOScale([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_AOScale(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 569: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 656: [propget, id(569), helpstring("property SSRScale")] HRESULT SSRScale([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_SSRScale(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 657: [propput, id(569), helpstring("property SSRScale")] HRESULT SSRScale([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_SSRScale(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 579: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 659: [propget, id(579), helpstring("property TableSoundVolume")] HRESULT TableSoundVolume([out, retval] int *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_TableSoundVolume((int*)&V_I4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 660: [propput, id(579), helpstring("property TableSoundVolume")] HRESULT TableSoundVolume([in] int newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_I4);
				hr = put_TableSoundVolume(V_I4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 580: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 661: [propget, id(580), helpstring("property TableMusicVolume")] HRESULT TableMusicVolume([out, retval] int *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_TableMusicVolume((int*)&V_I4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 662: [propput, id(580), helpstring("property TableMusicVolume")] HRESULT TableMusicVolume([in] int newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_I4);
				hr = put_TableMusicVolume(V_I4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 585: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 663: [propget, id(585), helpstring("property TableAdaptiveVSync")] HRESULT TableAdaptiveVSync([out, retval] int *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_TableAdaptiveVSync((int*)&V_I4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 664: [propput, id(585), helpstring("property TableAdaptiveVSync")] HRESULT TableAdaptiveVSync([in] int newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_I4);
				hr = put_TableAdaptiveVSync(V_I4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 1700: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 665: [propget, id(1700), helpstring("property BallReflection")] HRESULT BallReflection([out, retval] UserDefaultOnOff *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_BallReflection((UserDefaultOnOff*)&V_I4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 666: [propput, id(1700), helpstring("property BallReflection")] HRESULT BallReflection([in] UserDefaultOnOff newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_I4);
				hr = put_BallReflection((UserDefaultOnOff)V_I4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 1707: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 667: [propget, id(1707), helpstring("property PlayfieldReflectionStrength")] HRESULT PlayfieldReflectionStrength([out, retval] int *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_PlayfieldReflectionStrength((int*)&V_I4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 668: [propput, id(1707), helpstring("property PlayfieldReflectionStrength")] HRESULT PlayfieldReflectionStrength([in] int newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_I4);
				hr = put_PlayfieldReflectionStrength(V_I4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 1704: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 669: [propget, id(1704), helpstring("property BallTrail")] HRESULT BallTrail([out, retval] UserDefaultOnOff *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_BallTrail((UserDefaultOnOff*)&V_I4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 670: [propput, id(1704), helpstring("property BallTrail")] HRESULT BallTrail([in] UserDefaultOnOff newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_I4);
				hr = put_BallTrail((UserDefaultOnOff)V_I4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 1705: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 671: [propget, id(1705), helpstring("property TrailStrength")] HRESULT TrailStrength([out, retval] int *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_TrailStrength((int*)&V_I4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 672: [propput, id(1705), helpstring("property TrailStrength")] HRESULT TrailStrength([in] int newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_I4);
				hr = put_TrailStrength(V_I4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 1712: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 673: [propget, id(1712), helpstring("property BallPlayfieldReflectionScale")] HRESULT BallPlayfieldReflectionScale([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_BallPlayfieldReflectionScale(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 674: [propput, id(1712), helpstring("property BallPlayfieldReflectionScale")] HRESULT BallPlayfieldReflectionScale([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_BallPlayfieldReflectionScale(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 1713: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 675: [propget, id(1713), helpstring("property DefaultBulbIntensityScale")] HRESULT DefaultBulbIntensityScale([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_DefaultBulbIntensityScale(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 676: [propput, id(1713), helpstring("property DefaultBulbIntensityScale")] HRESULT DefaultBulbIntensityScale([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_DefaultBulbIntensityScale(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 420: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 677: [propget, id(420), helpstring("property DetailLevel")] HRESULT DetailLevel([out, retval] int *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_DetailLevel((int*)&V_I4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 678: [propput, id(420), helpstring("property DetailLevel")] HRESULT DetailLevel([in] int newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_I4);
				hr = put_DetailLevel(V_I4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 436: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 679: [propget, id(436), helpstring("property NightDay")] HRESULT NightDay([out, retval] int *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_NightDay((int*)&V_I4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 680: [propput, id(436), helpstring("property NightDay")] HRESULT NightDay([in] int newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_I4);
				hr = put_NightDay(V_I4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 398: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 681: [propget, id(398), helpstring("property OverwriteAlphaAcc")] HRESULT GlobalAlphaAcc([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_GlobalAlphaAcc(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 682: [propput, id(398), helpstring("property OverwriteAlphaAcc")] HRESULT GlobalAlphaAcc([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_GlobalAlphaAcc(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 588: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 683: [propget, id(588), helpstring("property OverwriteDayNight")] HRESULT GlobalDayNight([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_GlobalDayNight(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 684: [propput, id(588), helpstring("property OverwriteDayNight")] HRESULT GlobalDayNight([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_GlobalDayNight(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 219: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 685: [propget, id(219), helpstring("property Version")] HRESULT Version([out, retval] int *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_Version((int*)&V_I4(pVarResult));
			}
			break;
		}
		case 38: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 686: [propget, id(38), helpstring("property VersionMajor")] HRESULT VersionMajor([out, retval] int *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_VersionMajor((int*)&V_I4(pVarResult));
			}
			break;
		}
		case 39: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 687: [propget, id(39), helpstring("property VersionMinor")] HRESULT VersionMinor([out, retval] int *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_VersionMinor((int*)&V_I4(pVarResult));
			}
			break;
		}
		case 40: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 688: [propget, id(40), helpstring("property VersionRevision")] HRESULT VersionRevision([out, retval] int *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_VersionRevision((int*)&V_I4(pVarResult));
			}
			break;
		}
		case 24: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 689: [propget, id(24), helpstring("property VPBuildVersion")] HRESULT VPBuildVersion([out, retval] int *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_VPBuildVersion((int*)&V_I4(pVarResult));
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

STDMETHODIMP PinTable::GetDocumentation(INT index, BSTR *pBstrName, BSTR *pBstrDocString, DWORD *pdwHelpContext, BSTR *pBstrHelpFile) {
	if (index == MEMBERID_NIL) {
		*pBstrName = SysAllocString(L"Table");
		return S_OK;
	}
	return E_NOTIMPL;
}

name_disp_id_t ScriptGlobalTable::m_nameIDMap[] = {
		{ NULL },
		{ L"ActiveBall", 19 },
		{ L"ActiveTable", 48 },
		{ L"AddCreditKey", 20 },
		{ L"AddCreditKey2", 67 },
		{ L"AutoSize", 2132 },
		{ L"AutoWidth", 2133 },
		{ L"BeginModal", 25 },
		{ L"CenterTiltKey", 8 },
		{ L"CloseSerial", 251 },
		{ L"DecalImage", 2042 },
		{ L"DecalText", 2041 },
		{ L"DMDColoredPixels", 47 },
		{ L"DMDHeight", 45 },
		{ L"DMDPixels", 46 },
		{ L"DMDWidth", 44 },
		{ L"DropTargetBeveled", 2025 },
		{ L"DropTargetFlatSimple", 2031 },
		{ L"DropTargetSimple", 2026 },
		{ L"EndModal", 26 },
		{ L"EndMusic", 11 },
		{ L"ExitGame", 34 },
		{ L"FireKnocker", 33 },
		{ L"FlushSerial", 252 },
		{ L"GameTime", 22 },
		{ L"GateLongPlate", 2037 },
		{ L"GatePlate", 2036 },
		{ L"GateWireRectangle", 2035 },
		{ L"GateWireW", 2034 },
		{ L"GetBalls", 41 },
		{ L"GetCustomParam", 823 },
		{ L"GetElementByName", 43 },
		{ L"GetElements", 42 },
		{ L"GetMaterial", 231 },
		{ L"GetMaterialPhysics", 248 },
		{ L"GetPlayerHWnd", 14 },
		{ L"GetSerialDevices", 249 },
		{ L"GetTextFile", 23 },
		{ L"HitFatTargetRectangle", 2029 },
		{ L"HitFatTargetSlim", 2032 },
		{ L"HitFatTargetSquare", 2030 },
		{ L"HitTargetRectangle", 2028 },
		{ L"HitTargetRound", 2027 },
		{ L"HitTargetSlim", 2033 },
		{ L"ImageAlignCenter", 2005 },
		{ L"ImageAlignTopLeft", 2004 },
		{ L"ImageAlignWorld", 2003 },
		{ L"ImageModeWorld", 2142 },
		{ L"ImageModeWrap", 2143 },
		{ L"KickerCup", 2137 },
		{ L"KickerCup2", 2141 },
		{ L"KickerGottlieb", 2140 },
		{ L"KickerHole", 2136 },
		{ L"KickerHoleSimple", 2138 },
		{ L"KickerInvisible", 2135 },
		{ L"KickerWilliams", 2139 },
		{ L"LeftFlipperKey", 4 },
		{ L"LeftMagnaSave", 31 },
		{ L"LeftTiltKey", 6 },
		{ L"LightStateBlinking", 2002 },
		{ L"LightStateOff", 2000 },
		{ L"LightStateOn", 2001 },
		{ L"LoadValue", 18 },
		{ L"LockbarKey", 803 },
		{ L"ManualSize", 2134 },
		{ L"MaterialColor", 224 },
		{ L"MechanicalTilt", 30 },
		{ L"MusicDirectory", 257 },
		{ L"MusicVolume", 15 },
		{ L"NightDay", 436 },
		{ L"Nudge", DISPID_Table_Nudge },
		{ L"NudgeGetCalibration", 804 },
		{ L"NudgeSensorStatus", 806 },
		{ L"NudgeSetCalibration", 805 },
		{ L"NudgeTiltStatus", 807 },
		{ L"OpenSerial", 250 },
		{ L"PlatformBits", 261 },
		{ L"PlatformCPU", 260 },
		{ L"PlatformOS", 259 },
		{ L"PlayMusic", 10 },
		{ L"PlaySound", 3 },
		{ L"PlungerKey", 9 },
		{ L"PlungerTypeCustom", 2024 },
		{ L"PlungerTypeFlat", 2023 },
		{ L"PlungerTypeModern", 2022 },
		{ L"QuitPlayer", 37 },
		{ L"RampType1Wire", 2021 },
		{ L"RampType2Wire", 2018 },
		{ L"RampType3WireLeft", 2019 },
		{ L"RampType3WireRight", 2020 },
		{ L"RampType4Wire", 2017 },
		{ L"RampTypeFlat", 2016 },
		{ L"ReadSerial", 254 },
		{ L"RenderingMode", 218 },
		{ L"RightFlipperKey", 5 },
		{ L"RightMagnaSave", 32 },
		{ L"RightTiltKey", 7 },
		{ L"SaveValue", 17 },
		{ L"ScriptsDirectory", 258 },
		{ L"SeqAllOff", 2128 },
		{ L"SeqAllOn", 2129 },
		{ L"SeqArcBottomLeftDownOff", 2110 },
		{ L"SeqArcBottomLeftDownOn", 2109 },
		{ L"SeqArcBottomLeftUpOff", 2108 },
		{ L"SeqArcBottomLeftUpOn", 2107 },
		{ L"SeqArcBottomRightDownOff", 2114 },
		{ L"SeqArcBottomRightDownOn", 2113 },
		{ L"SeqArcBottomRightUpOff", 2112 },
		{ L"SeqArcBottomRightUpOn", 2111 },
		{ L"SeqArcTopLeftDownOff", 2118 },
		{ L"SeqArcTopLeftDownOn", 2117 },
		{ L"SeqArcTopLeftUpOff", 2116 },
		{ L"SeqArcTopLeftUpOn", 2115 },
		{ L"SeqArcTopRightDownOff", 2122 },
		{ L"SeqArcTopRightDownOn", 2121 },
		{ L"SeqArcTopRightUpOff", 2120 },
		{ L"SeqArcTopRightUpOn", 2119 },
		{ L"SeqBlinking", 2130 },
		{ L"SeqCircleInOff", 2086 },
		{ L"SeqCircleInOn", 2085 },
		{ L"SeqCircleOutOff", 2084 },
		{ L"SeqCircleOutOn", 2083 },
		{ L"SeqClockLeftOff", 2090 },
		{ L"SeqClockLeftOn", 2089 },
		{ L"SeqClockRightOff", 2088 },
		{ L"SeqClockRightOn", 2087 },
		{ L"SeqDiagDownLeftOff", 2058 },
		{ L"SeqDiagDownLeftOn", 2057 },
		{ L"SeqDiagDownRightOff", 2056 },
		{ L"SeqDiagDownRightOn", 2055 },
		{ L"SeqDiagUpLeftOff", 2054 },
		{ L"SeqDiagUpLeftOn", 2053 },
		{ L"SeqDiagUpRightOff", 2052 },
		{ L"SeqDiagUpRightOn", 2051 },
		{ L"SeqDownOff", 2046 },
		{ L"SeqDownOn", 2045 },
		{ L"SeqFanLeftDownOff", 2102 },
		{ L"SeqFanLeftDownOn", 2101 },
		{ L"SeqFanLeftUpOff", 2100 },
		{ L"SeqFanLeftUpOn", 2099 },
		{ L"SeqFanRightDownOff", 2106 },
		{ L"SeqFanRightDownOn", 2105 },
		{ L"SeqFanRightUpOff", 2104 },
		{ L"SeqFanRightUpOn", 2103 },
		{ L"SeqHatch1HorizOff", 2076 },
		{ L"SeqHatch1HorizOn", 2075 },
		{ L"SeqHatch1VertOff", 2080 },
		{ L"SeqHatch1VertOn", 2079 },
		{ L"SeqHatch2HorizOff", 2078 },
		{ L"SeqHatch2HorizOn", 2077 },
		{ L"SeqHatch2VertOff", 2082 },
		{ L"SeqHatch2VertOn", 2081 },
		{ L"SeqLastDynamic", 2127 },
		{ L"SeqLeftOff", 2050 },
		{ L"SeqLeftOn", 2049 },
		{ L"SeqMiddleInHorizOff", 2062 },
		{ L"SeqMiddleInHorizOn", 2061 },
		{ L"SeqMiddleInVertOff", 2066 },
		{ L"SeqMiddleInVertOn", 2065 },
		{ L"SeqMiddleOutHorizOff", 2060 },
		{ L"SeqMiddleOutHorizOn", 2059 },
		{ L"SeqMiddleOutVertOff", 2064 },
		{ L"SeqMiddleOutVertOn", 2063 },
		{ L"SeqRadarLeftOff", 2094 },
		{ L"SeqRadarLeftOn", 2093 },
		{ L"SeqRadarRightOff", 2092 },
		{ L"SeqRadarRightOn", 2091 },
		{ L"SeqRandom", 2131 },
		{ L"SeqRightOff", 2048 },
		{ L"SeqRightOn", 2047 },
		{ L"SeqScrewLeftOff", 2126 },
		{ L"SeqScrewLeftOn", 2125 },
		{ L"SeqScrewRightOff", 2124 },
		{ L"SeqScrewRightOn", 2123 },
		{ L"SeqStripe1HorizOff", 2068 },
		{ L"SeqStripe1HorizOn", 2067 },
		{ L"SeqStripe1VertOff", 2072 },
		{ L"SeqStripe1VertOn", 2071 },
		{ L"SeqStripe2HorizOff", 2070 },
		{ L"SeqStripe2HorizOn", 2069 },
		{ L"SeqStripe2VertOff", 2074 },
		{ L"SeqStripe2VertOn", 2073 },
		{ L"SeqUpOff", 2044 },
		{ L"SeqUpOn", 2043 },
		{ L"SeqWiperLeftOff", 2098 },
		{ L"SeqWiperLeftOn", 2097 },
		{ L"SeqWiperRightOff", 2096 },
		{ L"SeqWiperRightOn", 2095 },
		{ L"SetupSerial", 253 },
		{ L"ShapeCircle", 2006 },
		{ L"ShapeCustom", 2007 },
		{ L"ShowDT", 13434 },
		{ L"ShowFSS", 625 },
		{ L"StartGameKey", 12 },
		{ L"StopSound", 16 },
		{ L"SystemTime", 225 },
		{ L"TablesDirectory", 256 },
		{ L"TextAlignCenter", 2039 },
		{ L"TextAlignLeft", 2038 },
		{ L"TextAlignRight", 2040 },
		{ L"TriggerButton", 2012 },
		{ L"TriggerInder", 2015 },
		{ L"TriggerNone", 2008 },
		{ L"TriggerStar", 2010 },
		{ L"TriggerWireA", 2009 },
		{ L"TriggerWireB", 2011 },
		{ L"TriggerWireC", 2013 },
		{ L"TriggerWireD", 2014 },
		{ L"UpdateMaterial", 230 },
		{ L"UpdateMaterialPhysics", 247 },
		{ L"UserDirectory", 13 },
		{ L"Version", 219 },
		{ L"VersionMajor", 38 },
		{ L"VersionMinor", 39 },
		{ L"VersionRevision", 40 },
		{ L"VPBuildVersion", 24 },
		{ L"WindowHeight", 227 },
		{ L"WindowWidth", 226 },
		{ L"WriteSerial", 255 }
};

STDMETHODIMP ScriptGlobalTable::GetIDsOfNames(REFIID /*riid*/, LPOLESTR* rgszNames, UINT cNames, LCID lcid, DISPID* rgDispId) {
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

STDMETHODIMP ScriptGlobalTable::Invoke(DISPID dispIdMember, REFIID /*riid*/, LCID lcid, WORD wFlags, DISPPARAMS* pDispParams, VARIANT* pVarResult, EXCEPINFO* pExcepInfo, UINT* puArgErr) {
	HRESULT hr = DISP_E_UNKNOWNNAME;
	int index = pDispParams->cArgs;
	switch(dispIdMember) {
		case 3: {
			if (wFlags & DISPATCH_METHOD) {
				// line 700: [id(3), helpstring("method PlaySound")] HRESULT PlaySound(BSTR Sound, [defaultvalue(1)] long LoopCount, [defaultvalue(1)] float Volume, [defaultvalue(0)] float pan, [defaultvalue(0)] float randompitch, [defaultvalue(0)] long pitch, [defaultvalue(0)] VARIANT_BOOL usesame, [defaultvalue(1)] VARIANT_BOOL restart, [defaultvalue(0)] float front_rear_fade);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				VARIANT var1;
				V_VT(&var1) = VT_I4;
				V_I4(&var1) = 1;
				VariantChangeType(&var1, (index > 0) ? &pDispParams->rgvarg[--index] : &var1, 0, VT_I4);
				VARIANT var2;
				V_VT(&var2) = VT_R4;
				V_R4(&var2) = 1;
				VariantChangeType(&var2, (index > 0) ? &pDispParams->rgvarg[--index] : &var2, 0, VT_R4);
				VARIANT var3;
				V_VT(&var3) = VT_R4;
				V_R4(&var3) = 0;
				VariantChangeType(&var3, (index > 0) ? &pDispParams->rgvarg[--index] : &var3, 0, VT_R4);
				VARIANT var4;
				V_VT(&var4) = VT_R4;
				V_R4(&var4) = 0;
				VariantChangeType(&var4, (index > 0) ? &pDispParams->rgvarg[--index] : &var4, 0, VT_R4);
				VARIANT var5;
				V_VT(&var5) = VT_I4;
				V_I4(&var5) = 0;
				VariantChangeType(&var5, (index > 0) ? &pDispParams->rgvarg[--index] : &var5, 0, VT_I4);
				VARIANT var6;
				V_VT(&var6) = VT_BOOL;
				V_BOOL(&var6) = 0;
				VariantChangeType(&var6, (index > 0) ? &pDispParams->rgvarg[--index] : &var6, 0, VT_BOOL);
				VARIANT var7;
				V_VT(&var7) = VT_BOOL;
				V_BOOL(&var7) = 1;
				VariantChangeType(&var7, (index > 0) ? &pDispParams->rgvarg[--index] : &var7, 0, VT_BOOL);
				VARIANT var8;
				V_VT(&var8) = VT_R4;
				V_R4(&var8) = 0;
				VariantChangeType(&var8, (index > 0) ? &pDispParams->rgvarg[--index] : &var8, 0, VT_R4);
				hr = PlaySound(V_BSTR(&var0), V_I4(&var1), V_R4(&var2), V_R4(&var3), V_R4(&var4), V_I4(&var5), V_BOOL(&var6), V_BOOL(&var7), V_R4(&var8));
				VariantClear(&var0);
				VariantClear(&var1);
				VariantClear(&var2);
				VariantClear(&var3);
				VariantClear(&var4);
				VariantClear(&var5);
				VariantClear(&var6);
				VariantClear(&var7);
				VariantClear(&var8);
			}
			break;
		}
		case 4: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 701: [propget, id(4), helpstring("property LeftFlipperKey")] HRESULT LeftFlipperKey([out, retval] long *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_LeftFlipperKey((long*)&V_I4(pVarResult));
			}
			break;
		}
		case 5: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 702: [propget, id(5), helpstring("property RightFlipperKey")] HRESULT RightFlipperKey([out, retval] long *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_RightFlipperKey((long*)&V_I4(pVarResult));
			}
			break;
		}
		case 6: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 703: [propget, id(6), helpstring("property LeftTiltKey")] HRESULT LeftTiltKey([out, retval] long *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_LeftTiltKey((long*)&V_I4(pVarResult));
			}
			break;
		}
		case 7: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 704: [propget, id(7), helpstring("property RightTiltKey")] HRESULT RightTiltKey([out, retval] long *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_RightTiltKey((long*)&V_I4(pVarResult));
			}
			break;
		}
		case 8: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 705: [propget, id(8), helpstring("property CenterTiltKey")] HRESULT CenterTiltKey([out, retval] long *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_CenterTiltKey((long*)&V_I4(pVarResult));
			}
			break;
		}
		case 9: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 706: [propget, id(9), helpstring("property PlungerKey")] HRESULT PlungerKey([out, retval] long *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_PlungerKey((long*)&V_I4(pVarResult));
			}
			break;
		}
		case 10: {
			if (wFlags & DISPATCH_METHOD) {
				// line 707: [id(10), helpstring("method PlayMusic")] HRESULT PlayMusic(BSTR str, [defaultvalue(1)] float Volume);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				VARIANT var1;
				V_VT(&var1) = VT_R4;
				V_R4(&var1) = 1;
				VariantChangeType(&var1, (index > 0) ? &pDispParams->rgvarg[--index] : &var1, 0, VT_R4);
				hr = PlayMusic(V_BSTR(&var0), V_R4(&var1));
				VariantClear(&var0);
				VariantClear(&var1);
			}
			break;
		}
		case 15: {
			if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 708: [propput, id(15), helpstring("property MusicVolume")] HRESULT MusicVolume(float Volume);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_MusicVolume(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 11: {
			if (wFlags & DISPATCH_METHOD) {
				// line 709: [id(11), helpstring("method EndMusic")] HRESULT EndMusic();
				hr = EndMusic();
			}
			break;
		}
		case 12: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 710: [propget, id(12), helpstring("property StartGameKey")] HRESULT StartGameKey([out, retval] long *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_StartGameKey((long*)&V_I4(pVarResult));
			}
			break;
		}
		case 13: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 711: [propget, id(13),  helpstring("property UserDirectory")] HRESULT UserDirectory([out, retval] BSTR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_UserDirectory(&V_BSTR(pVarResult));
			}
			break;
		}
		case 256: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 712: [propget, id(256), helpstring("property TablesDirectory")] HRESULT TablesDirectory([out, retval] BSTR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_TablesDirectory(&V_BSTR(pVarResult));
			}
			break;
		}
		case 257: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 713: [propget, id(257), helpstring("property MusicDirectory")] HRESULT MusicDirectory([out, retval] BSTR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_MusicDirectory(&V_BSTR(pVarResult));
			}
			break;
		}
		case 258: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 714: [propget, id(258), helpstring("property ScriptsDirectory")] HRESULT ScriptsDirectory([out, retval] BSTR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_ScriptsDirectory(&V_BSTR(pVarResult));
			}
			break;
		}
		case 259: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 715: [propget, id(259), helpstring("property PlatformOS")] HRESULT PlatformOS([out, retval] BSTR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_PlatformOS(&V_BSTR(pVarResult));
			}
			break;
		}
		case 260: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 716: [propget, id(260), helpstring("property PlatformCPU")] HRESULT PlatformCPU([out, retval] BSTR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_PlatformCPU(&V_BSTR(pVarResult));
			}
			break;
		}
		case 261: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 717: [propget, id(261), helpstring("property PlatformBits")] HRESULT PlatformBits([out, retval] BSTR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_PlatformBits(&V_BSTR(pVarResult));
			}
			break;
		}
		case 14: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 719: [propget, id(14), helpstring("property GetPlayerHWnd")] HRESULT GetPlayerHWnd([out, retval] SIZE_T *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_UI4;
				hr = get_GetPlayerHWnd((SIZE_T*)&V_UI4(pVarResult));
			}
			break;
		}
		case 16: {
			if (wFlags & DISPATCH_METHOD) {
				// line 724: [id(16), helpstring("method StopSound")] HRESULT StopSound(BSTR Sound);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = StopSound(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 17: {
			if (wFlags & DISPATCH_METHOD) {
				// line 725: [id(17), helpstring("method SaveValue")] HRESULT SaveValue(BSTR TableName, BSTR ValueName, VARIANT Value);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				VARIANT var1;
				V_VT(&var1) = VT_EMPTY;
				VariantChangeType(&var1, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				VARIANT var2;
				V_VT(&var2) = VT_EMPTY;
				VariantCopyInd(&var2, &pDispParams->rgvarg[--index]);
				hr = SaveValue(V_BSTR(&var0), V_BSTR(&var1), var2);
				VariantClear(&var0);
				VariantClear(&var1);
				VariantClear(&var2);
			}
			break;
		}
		case 18: {
			if (wFlags & DISPATCH_METHOD) {
				// line 726: [id(18), helpstring("method LoadValue")] HRESULT LoadValue(BSTR TableName, BSTR ValueName, [out, retval] VARIANT *Value);
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
				hr = LoadValue(V_BSTR(&var0), V_BSTR(&var1), pVarResult);
				VariantClear(&var0);
				VariantClear(&var1);
			}
			break;
		}
		case 19: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 727: [propget, id(19), helpstring("property ActiveBall")] HRESULT ActiveBall([out, retval] IBall **pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_DISPATCH;
				hr = get_ActiveBall((IBall**)&V_DISPATCH(pVarResult));
			}
			break;
		}
		case 20: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 728: [propget, id(20), helpstring("property AddCreditKey")] HRESULT AddCreditKey([out, retval] long *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_AddCreditKey((long*)&V_I4(pVarResult));
			}
			break;
		}
		case 67: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 729: [propget, id(67), helpstring("property AddCreditKey2")] HRESULT AddCreditKey2([out, retval] long *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_AddCreditKey2((long*)&V_I4(pVarResult));
			}
			break;
		}
		case 22: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 730: [propget, id(22), helpstring("property GameTime")] HRESULT GameTime([out, retval] long *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_GameTime((long*)&V_I4(pVarResult));
			}
			break;
		}
		case 225: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 731: [propget, id(225), helpstring("property SystemTime")] HRESULT SystemTime([out, retval] long *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_SystemTime((long*)&V_I4(pVarResult));
			}
			break;
		}
		case 823: {
			if (wFlags & DISPATCH_METHOD) {
				// line 732: [id(823), helpstring("method GetCustomParam")] HRESULT GetCustomParam(long index, [out, retval] BSTR *param);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_I4);
				V_VT(pVarResult) = VT_BSTR;
				hr = GetCustomParam(V_I4(&var0), &V_BSTR(pVarResult));
				VariantClear(&var0);
			}
			break;
		}
		case 23: {
			if (wFlags & DISPATCH_METHOD) {
				// line 733: [id(23), helpstring("method GetTextFile")] HRESULT GetTextFile(BSTR FileName, [out, retval] BSTR *pContents);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				V_VT(pVarResult) = VT_BSTR;
				hr = GetTextFile(V_BSTR(&var0), &V_BSTR(pVarResult));
				VariantClear(&var0);
			}
			break;
		}
		case 25: {
			if (wFlags & DISPATCH_METHOD) {
				// line 734: [id(25), helpstring("method BeginModal")] HRESULT BeginModal();
				hr = BeginModal();
			}
			break;
		}
		case 26: {
			if (wFlags & DISPATCH_METHOD) {
				// line 735: [id(26), helpstring("method EndModal")] HRESULT EndModal();
				hr = EndModal();
			}
			break;
		}
		case DISPID_Table_Nudge: {
			if (wFlags & DISPATCH_METHOD) {
				// line 736: [id(DISPID_Table_Nudge), helpstring("method Nudge")] HRESULT Nudge([in] float Angle, [in] float Force);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				VARIANT var1;
				V_VT(&var1) = VT_EMPTY;
				VariantChangeType(&var1, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = Nudge(V_R4(&var0), V_R4(&var1));
				VariantClear(&var0);
				VariantClear(&var1);
			}
			break;
		}
		case 804: {
			if (wFlags & DISPATCH_METHOD) {
				// line 737: [id(804), helpstring("method NudgeGetCalibration")] HRESULT NudgeGetCalibration([out] VARIANT *XMax, [out] VARIANT *YMax, [out] VARIANT *XGain, [out] VARIANT *YGain, [out] VARIANT *DeadZone, [out] VARIANT *TiltSensitivty);
				VARIANT* var0 = &pDispParams->rgvarg[--index];
				VARIANT* var1 = &pDispParams->rgvarg[--index];
				VARIANT* var2 = &pDispParams->rgvarg[--index];
				VARIANT* var3 = &pDispParams->rgvarg[--index];
				VARIANT* var4 = &pDispParams->rgvarg[--index];
				VARIANT* var5 = &pDispParams->rgvarg[--index];
				hr = NudgeGetCalibration(var0, var1, var2, var3, var4, var5);
			}
			break;
		}
		case 805: {
			if (wFlags & DISPATCH_METHOD) {
				// line 738: [id(805), helpstring("method NudgeSetCalibration")] HRESULT NudgeSetCalibration([in] int XMax, [in] int YMax, [in] int XGain, [in] int YGain, [in] int DeadZone, [in] int TiltSensitivty);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_I4);
				VARIANT var1;
				V_VT(&var1) = VT_EMPTY;
				VariantChangeType(&var1, &pDispParams->rgvarg[--index], 0, VT_I4);
				VARIANT var2;
				V_VT(&var2) = VT_EMPTY;
				VariantChangeType(&var2, &pDispParams->rgvarg[--index], 0, VT_I4);
				VARIANT var3;
				V_VT(&var3) = VT_EMPTY;
				VariantChangeType(&var3, &pDispParams->rgvarg[--index], 0, VT_I4);
				VARIANT var4;
				V_VT(&var4) = VT_EMPTY;
				VariantChangeType(&var4, &pDispParams->rgvarg[--index], 0, VT_I4);
				VARIANT var5;
				V_VT(&var5) = VT_EMPTY;
				VariantChangeType(&var5, &pDispParams->rgvarg[--index], 0, VT_I4);
				hr = NudgeSetCalibration(V_I4(&var0), V_I4(&var1), V_I4(&var2), V_I4(&var3), V_I4(&var4), V_I4(&var5));
				VariantClear(&var0);
				VariantClear(&var1);
				VariantClear(&var2);
				VariantClear(&var3);
				VariantClear(&var4);
				VariantClear(&var5);
			}
			break;
		}
		case 806: {
			if (wFlags & DISPATCH_METHOD) {
				// line 739: [id(806), helpstring("method NudgeSensorStatus")] HRESULT NudgeSensorStatus([out] VARIANT *XNudge, [out] VARIANT *YNudge);
				VARIANT* var0 = &pDispParams->rgvarg[--index];
				VARIANT* var1 = &pDispParams->rgvarg[--index];
				hr = NudgeSensorStatus(var0, var1);
			}
			break;
		}
		case 807: {
			if (wFlags & DISPATCH_METHOD) {
				// line 740: [id(807), helpstring("method NudgeTiltStatus")] HRESULT NudgeTiltStatus([out] VARIANT *XPlumb, [out] VARIANT *YPlumb, [out] VARIANT *TiltPercent);
				VARIANT* var0 = &pDispParams->rgvarg[--index];
				VARIANT* var1 = &pDispParams->rgvarg[--index];
				VARIANT* var2 = &pDispParams->rgvarg[--index];
				hr = NudgeTiltStatus(var0, var1, var2);
			}
			break;
		}
		case 30: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 742: [propget, id(30), helpstring("property MechanicalTilt")] HRESULT MechanicalTilt([out, retval] long *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_MechanicalTilt((long*)&V_I4(pVarResult));
			}
			break;
		}
		case 31: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 743: [propget, id(31), helpstring("property LeftMagnaSave")] HRESULT LeftMagnaSave([out, retval] long *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_LeftMagnaSave((long*)&V_I4(pVarResult));
			}
			break;
		}
		case 32: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 744: [propget, id(32), helpstring("property RightMagnaSave")] HRESULT RightMagnaSave([out, retval] long *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_RightMagnaSave((long*)&V_I4(pVarResult));
			}
			break;
		}
		case 34: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 745: [propget, id(34), helpstring("property ExitGame")] HRESULT ExitGame([out, retval] long *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_ExitGame((long*)&V_I4(pVarResult));
			}
			break;
		}
		case 803: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 746: [propget, id(803), helpstring("property LockbarKey")] HRESULT LockbarKey([out, retval] long *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_LockbarKey((long*)&V_I4(pVarResult));
			}
			break;
		}
		case 33: {
			if (wFlags & DISPATCH_METHOD) {
				// line 747: [id(33), helpstring("method FireKnocker")] HRESULT FireKnocker([defaultvalue(1)] int Count);
				VARIANT var0;
				V_VT(&var0) = VT_I4;
				V_I4(&var0) = 1;
				VariantChangeType(&var0, (index > 0) ? &pDispParams->rgvarg[--index] : &var0, 0, VT_I4);
				hr = FireKnocker(V_I4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 37: {
			if (wFlags & DISPATCH_METHOD) {
				// line 748: [id(37), helpstring("method QuitPlayer")] HRESULT QuitPlayer([defaultvalue(0)] int CloseType);
				VARIANT var0;
				V_VT(&var0) = VT_I4;
				V_I4(&var0) = 0;
				VariantChangeType(&var0, (index > 0) ? &pDispParams->rgvarg[--index] : &var0, 0, VT_I4);
				hr = QuitPlayer(V_I4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 13434: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 750: [propget, id(13434), helpstring("property ShowDT")] HRESULT ShowDT([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_ShowDT(&V_BOOL(pVarResult));
			}
			break;
		}
		case 625: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 751: [propget, id(625), helpstring("property ShowFSS")] HRESULT ShowFSS([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_ShowFSS(&V_BOOL(pVarResult));
			}
			break;
		}
		case 436: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 753: [propget, id(436), helpstring("property NightDay")] HRESULT NightDay([out, retval] int *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_NightDay((int*)&V_I4(pVarResult));
			}
			break;
		}
		case 41: {
			if (wFlags & DISPATCH_METHOD) {
				// line 755: [id(41), helpstring("method GetBalls")] HRESULT GetBalls([out, retval] SAFEARRAY(VARIANT) *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_VARIANT|VT_ARRAY;
				hr = GetBalls((SAFEARRAY**)&V_ARRAY(pVarResult));
			}
			break;
		}
		case 42: {
			if (wFlags & DISPATCH_METHOD) {
				// line 756: [id(42), helpstring("method GetElements")] HRESULT GetElements([out, retval] SAFEARRAY(VARIANT) *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_VARIANT|VT_ARRAY;
				hr = GetElements((SAFEARRAY**)&V_ARRAY(pVarResult));
			}
			break;
		}
		case 43: {
			if (wFlags & DISPATCH_METHOD) {
				// line 757: [id(43), helpstring("method GetElementByName")] HRESULT GetElementByName([in] BSTR name, [out, retval] IDispatch* *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				V_VT(pVarResult) = VT_DISPATCH;
				hr = GetElementByName(V_BSTR(&var0), (IDispatch**)&V_DISPATCH(pVarResult));
				VariantClear(&var0);
			}
			break;
		}
		case 48: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 758: [propget, id(48), helpstring("property ActiveTable")] HRESULT ActiveTable([out, retval] ITable **pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_DISPATCH;
				hr = get_ActiveTable((ITable**)&V_DISPATCH(pVarResult));
			}
			break;
		}
		case 44: {
			if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 760: [propput, id(44), helpstring("property DMDWidth")] HRESULT DMDWidth([in] int pVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_I4);
				hr = put_DMDWidth(V_I4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 45: {
			if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 761: [propput, id(45), helpstring("property DMDHeight")] HRESULT DMDHeight([in] int pVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_I4);
				hr = put_DMDHeight(V_I4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 46: {
			if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 762: [propput, id(46), helpstring("property DMDPixels")] HRESULT DMDPixels([in] VARIANT pVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantCopyInd(&var0, &pDispParams->rgvarg[--index]);
				hr = put_DMDPixels(var0);
				VariantClear(&var0);
			}
			break;
		}
		case 47: {
			if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 763: [propput, id(47), helpstring("property DMDColoredPixels")] HRESULT DMDColoredPixels([in] VARIANT pVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantCopyInd(&var0, &pDispParams->rgvarg[--index]);
				hr = put_DMDColoredPixels(var0);
				VariantClear(&var0);
			}
			break;
		}
		case 219: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 765: [propget, id(219), helpstring("property Version")] HRESULT Version([out, retval] int *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_Version((int*)&V_I4(pVarResult));
			}
			break;
		}
		case 38: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 766: [propget, id(38), helpstring("property VersionMajor")] HRESULT VersionMajor([out, retval] int *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_VersionMajor((int*)&V_I4(pVarResult));
			}
			break;
		}
		case 39: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 767: [propget, id(39), helpstring("property VersionMinor")] HRESULT VersionMinor([out, retval] int *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_VersionMinor((int*)&V_I4(pVarResult));
			}
			break;
		}
		case 40: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 768: [propget, id(40), helpstring("property VersionRevision")] HRESULT VersionRevision([out, retval] int *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_VersionRevision((int*)&V_I4(pVarResult));
			}
			break;
		}
		case 24: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 769: [propget, id(24), helpstring("property VPBuildVersion")] HRESULT VPBuildVersion([out, retval] int *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_VPBuildVersion((int*)&V_I4(pVarResult));
			}
			break;
		}
		case 249: {
			if (wFlags & DISPATCH_METHOD) {
				// line 771: [id(249), helpstring("method GetSerialDevices")] HRESULT GetSerialDevices([out, retval] VARIANT *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				hr = GetSerialDevices(pVarResult);
			}
			break;
		}
		case 250: {
			if (wFlags & DISPATCH_METHOD) {
				// line 772: [id(250), helpstring("method OpenSerial")] HRESULT OpenSerial([in] BSTR device);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = OpenSerial(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 251: {
			if (wFlags & DISPATCH_METHOD) {
				// line 773: [id(251), helpstring("method CloseSerial")] HRESULT CloseSerial();
				hr = CloseSerial();
			}
			break;
		}
		case 252: {
			if (wFlags & DISPATCH_METHOD) {
				// line 774: [id(252), helpstring("method FlushSerial")] HRESULT FlushSerial();
				hr = FlushSerial();
			}
			break;
		}
		case 253: {
			if (wFlags & DISPATCH_METHOD) {
				// line 775: [id(253), helpstring("method SetupSerial")] HRESULT SetupSerial([in] int baud, [in] int bits, [in] int parity, [in] int stopbit, [in] VARIANT_BOOL rts, [in] VARIANT_BOOL dtr);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_I4);
				VARIANT var1;
				V_VT(&var1) = VT_EMPTY;
				VariantChangeType(&var1, &pDispParams->rgvarg[--index], 0, VT_I4);
				VARIANT var2;
				V_VT(&var2) = VT_EMPTY;
				VariantChangeType(&var2, &pDispParams->rgvarg[--index], 0, VT_I4);
				VARIANT var3;
				V_VT(&var3) = VT_EMPTY;
				VariantChangeType(&var3, &pDispParams->rgvarg[--index], 0, VT_I4);
				VARIANT var4;
				V_VT(&var4) = VT_EMPTY;
				VariantChangeType(&var4, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				VARIANT var5;
				V_VT(&var5) = VT_EMPTY;
				VariantChangeType(&var5, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = SetupSerial(V_I4(&var0), V_I4(&var1), V_I4(&var2), V_I4(&var3), V_BOOL(&var4), V_BOOL(&var5));
				VariantClear(&var0);
				VariantClear(&var1);
				VariantClear(&var2);
				VariantClear(&var3);
				VariantClear(&var4);
				VariantClear(&var5);
			}
			break;
		}
		case 254: {
			if (wFlags & DISPATCH_METHOD) {
				// line 776: [id(254), helpstring("method ReadSerial")] HRESULT ReadSerial([in] int size, [out] VARIANT *pVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_I4);
				VARIANT* var1 = &pDispParams->rgvarg[--index];
				hr = ReadSerial(V_I4(&var0), var1);
				VariantClear(&var0);
			}
			break;
		}
		case 255: {
			if (wFlags & DISPATCH_METHOD) {
				// line 777: [id(255), helpstring("method WriteSerial")] HRESULT WriteSerial([in] VARIANT pVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantCopyInd(&var0, &pDispParams->rgvarg[--index]);
				hr = WriteSerial(var0);
				VariantClear(&var0);
			}
			break;
		}
		case 218: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 779: [propget, id(218), helpstring("property RenderingMode")] HRESULT RenderingMode([out, retval] int *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_RenderingMode((int*)&V_I4(pVarResult));
			}
			break;
		}
		case 230: {
			if (wFlags & DISPATCH_METHOD) {
				// line 781: [id(230), helpstring("method UpdateMaterial")] HRESULT UpdateMaterial([in] BSTR pVal, [in] float wrapLighting, [in] float roughness, [in] float glossyImageLerp, [in] float thickness, [in] float edge, [in] float edgeAlpha, [in] float opacity,[in] OLE_COLOR base, [in] OLE_COLOR glossy, [in] OLE_COLOR clearcoat, [in] VARIANT_BOOL isMetal, [in] VARIANT_BOOL opacityActive,[in] float elasticity, [in] float elasticityFalloff, [in] float friction, [in] float scatterAngle);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				VARIANT var1;
				V_VT(&var1) = VT_EMPTY;
				VariantChangeType(&var1, &pDispParams->rgvarg[--index], 0, VT_R4);
				VARIANT var2;
				V_VT(&var2) = VT_EMPTY;
				VariantChangeType(&var2, &pDispParams->rgvarg[--index], 0, VT_R4);
				VARIANT var3;
				V_VT(&var3) = VT_EMPTY;
				VariantChangeType(&var3, &pDispParams->rgvarg[--index], 0, VT_R4);
				VARIANT var4;
				V_VT(&var4) = VT_EMPTY;
				VariantChangeType(&var4, &pDispParams->rgvarg[--index], 0, VT_R4);
				VARIANT var5;
				V_VT(&var5) = VT_EMPTY;
				VariantChangeType(&var5, &pDispParams->rgvarg[--index], 0, VT_R4);
				VARIANT var6;
				V_VT(&var6) = VT_EMPTY;
				VariantChangeType(&var6, &pDispParams->rgvarg[--index], 0, VT_R4);
				VARIANT var7;
				V_VT(&var7) = VT_EMPTY;
				VariantChangeType(&var7, &pDispParams->rgvarg[--index], 0, VT_R4);
				VARIANT var8;
				V_VT(&var8) = VT_EMPTY;
				VariantChangeType(&var8, &pDispParams->rgvarg[--index], 0, VT_UI4);
				VARIANT var9;
				V_VT(&var9) = VT_EMPTY;
				VariantChangeType(&var9, &pDispParams->rgvarg[--index], 0, VT_UI4);
				VARIANT var10;
				V_VT(&var10) = VT_EMPTY;
				VariantChangeType(&var10, &pDispParams->rgvarg[--index], 0, VT_UI4);
				VARIANT var11;
				V_VT(&var11) = VT_EMPTY;
				VariantChangeType(&var11, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				VARIANT var12;
				V_VT(&var12) = VT_EMPTY;
				VariantChangeType(&var12, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				VARIANT var13;
				V_VT(&var13) = VT_EMPTY;
				VariantChangeType(&var13, &pDispParams->rgvarg[--index], 0, VT_R4);
				VARIANT var14;
				V_VT(&var14) = VT_EMPTY;
				VariantChangeType(&var14, &pDispParams->rgvarg[--index], 0, VT_R4);
				VARIANT var15;
				V_VT(&var15) = VT_EMPTY;
				VariantChangeType(&var15, &pDispParams->rgvarg[--index], 0, VT_R4);
				VARIANT var16;
				V_VT(&var16) = VT_EMPTY;
				VariantChangeType(&var16, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = UpdateMaterial(V_BSTR(&var0), V_R4(&var1), V_R4(&var2), V_R4(&var3), V_R4(&var4), V_R4(&var5), V_R4(&var6), V_R4(&var7), (OLE_COLOR)V_UI4(&var8), (OLE_COLOR)V_UI4(&var9), (OLE_COLOR)V_UI4(&var10), V_BOOL(&var11), V_BOOL(&var12), V_R4(&var13), V_R4(&var14), V_R4(&var15), V_R4(&var16));
				VariantClear(&var0);
				VariantClear(&var1);
				VariantClear(&var2);
				VariantClear(&var3);
				VariantClear(&var4);
				VariantClear(&var5);
				VariantClear(&var6);
				VariantClear(&var7);
				VariantClear(&var8);
				VariantClear(&var9);
				VariantClear(&var10);
				VariantClear(&var11);
				VariantClear(&var12);
				VariantClear(&var13);
				VariantClear(&var14);
				VariantClear(&var15);
				VariantClear(&var16);
			}
			break;
		}
		case 231: {
			if (wFlags & DISPATCH_METHOD) {
				// line 784: [id(231), helpstring("method GetMaterial")] HRESULT GetMaterial([in] BSTR pVal, [out] VARIANT *wrapLighting, [out] VARIANT *roughness, [out] VARIANT *glossyImageLerp, [out] VARIANT *thickness, [out] VARIANT *edge, [out] VARIANT *edgeAlpha, [out] VARIANT *opacity,[out] VARIANT *base, [out] VARIANT *glossy, [out] VARIANT *clearcoat, [out] VARIANT *isMetal, [out] VARIANT *opacityActive,[out] VARIANT *elasticity, [out] VARIANT *elasticityFalloff, [out] VARIANT *friction, [out] VARIANT *scatterAngle);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				VARIANT* var1 = &pDispParams->rgvarg[--index];
				VARIANT* var2 = &pDispParams->rgvarg[--index];
				VARIANT* var3 = &pDispParams->rgvarg[--index];
				VARIANT* var4 = &pDispParams->rgvarg[--index];
				VARIANT* var5 = &pDispParams->rgvarg[--index];
				VARIANT* var6 = &pDispParams->rgvarg[--index];
				VARIANT* var7 = &pDispParams->rgvarg[--index];
				VARIANT* var8 = &pDispParams->rgvarg[--index];
				VARIANT* var9 = &pDispParams->rgvarg[--index];
				VARIANT* var10 = &pDispParams->rgvarg[--index];
				VARIANT* var11 = &pDispParams->rgvarg[--index];
				VARIANT* var12 = &pDispParams->rgvarg[--index];
				VARIANT* var13 = &pDispParams->rgvarg[--index];
				VARIANT* var14 = &pDispParams->rgvarg[--index];
				VARIANT* var15 = &pDispParams->rgvarg[--index];
				VARIANT* var16 = &pDispParams->rgvarg[--index];
				hr = GetMaterial(V_BSTR(&var0), var1, var2, var3, var4, var5, var6, var7, var8, var9, var10, var11, var12, var13, var14, var15, var16);
				VariantClear(&var0);
			}
			break;
		}
		case 247: {
			if (wFlags & DISPATCH_METHOD) {
				// line 787: [id(247), helpstring("method UpdateMaterialPhysics")] HRESULT UpdateMaterialPhysics([in] BSTR pVal,[in] float elasticity, [in] float elasticityFalloff, [in] float friction, [in] float scatterAngle);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				VARIANT var1;
				V_VT(&var1) = VT_EMPTY;
				VariantChangeType(&var1, &pDispParams->rgvarg[--index], 0, VT_R4);
				VARIANT var2;
				V_VT(&var2) = VT_EMPTY;
				VariantChangeType(&var2, &pDispParams->rgvarg[--index], 0, VT_R4);
				VARIANT var3;
				V_VT(&var3) = VT_EMPTY;
				VariantChangeType(&var3, &pDispParams->rgvarg[--index], 0, VT_R4);
				VARIANT var4;
				V_VT(&var4) = VT_EMPTY;
				VariantChangeType(&var4, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = UpdateMaterialPhysics(V_BSTR(&var0), V_R4(&var1), V_R4(&var2), V_R4(&var3), V_R4(&var4));
				VariantClear(&var0);
				VariantClear(&var1);
				VariantClear(&var2);
				VariantClear(&var3);
				VariantClear(&var4);
			}
			break;
		}
		case 248: {
			if (wFlags & DISPATCH_METHOD) {
				// line 789: [id(248), helpstring("method GetMaterialPhysics")] HRESULT GetMaterialPhysics([in] BSTR pVal,[out] VARIANT *elasticity, [out] VARIANT *elasticityFalloff, [out] VARIANT *friction, [out] VARIANT *scatterAngle);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				VARIANT* var1 = &pDispParams->rgvarg[--index];
				VARIANT* var2 = &pDispParams->rgvarg[--index];
				VARIANT* var3 = &pDispParams->rgvarg[--index];
				VARIANT* var4 = &pDispParams->rgvarg[--index];
				hr = GetMaterialPhysics(V_BSTR(&var0), var1, var2, var3, var4);
				VariantClear(&var0);
			}
			break;
		}
		case 224: {
			if (wFlags & DISPATCH_METHOD) {
				// line 791: [id(224), helpstring("method MaterialColor")] HRESULT MaterialColor([in] BSTR name, [in] OLE_COLOR newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				VARIANT var1;
				V_VT(&var1) = VT_EMPTY;
				VariantChangeType(&var1, &pDispParams->rgvarg[--index], 0, VT_UI4);
				hr = MaterialColor(V_BSTR(&var0), (OLE_COLOR)V_UI4(&var1));
				VariantClear(&var0);
				VariantClear(&var1);
			}
			break;
		}
		case 226: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 793: [propget, id(226), helpstring("property WindowWidth")] HRESULT WindowWidth([out, retval] int *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_WindowWidth((int*)&V_I4(pVarResult));
			}
			break;
		}
		case 227: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 794: [propget, id(227), helpstring("property WindowHeight")] HRESULT WindowHeight([out, retval] int *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_WindowHeight((int*)&V_I4(pVarResult));
			}
			break;
		}
		case 2000: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 0;
				hr = S_OK;
			}
			break;
		}
		case 2001: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 1;
				hr = S_OK;
			}
			break;
		}
		case 2002: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 2;
				hr = S_OK;
			}
			break;
		}
		case 2003: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 0;
				hr = S_OK;
			}
			break;
		}
		case 2004: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 1;
				hr = S_OK;
			}
			break;
		}
		case 2005: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 2;
				hr = S_OK;
			}
			break;
		}
		case 2006: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 0;
				hr = S_OK;
			}
			break;
		}
		case 2007: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 1;
				hr = S_OK;
			}
			break;
		}
		case 2008: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 0;
				hr = S_OK;
			}
			break;
		}
		case 2009: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 1;
				hr = S_OK;
			}
			break;
		}
		case 2010: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 2;
				hr = S_OK;
			}
			break;
		}
		case 2011: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 3;
				hr = S_OK;
			}
			break;
		}
		case 2012: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 4;
				hr = S_OK;
			}
			break;
		}
		case 2013: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 5;
				hr = S_OK;
			}
			break;
		}
		case 2014: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 6;
				hr = S_OK;
			}
			break;
		}
		case 2015: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 7;
				hr = S_OK;
			}
			break;
		}
		case 2016: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 0;
				hr = S_OK;
			}
			break;
		}
		case 2017: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 1;
				hr = S_OK;
			}
			break;
		}
		case 2018: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 2;
				hr = S_OK;
			}
			break;
		}
		case 2019: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 3;
				hr = S_OK;
			}
			break;
		}
		case 2020: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 4;
				hr = S_OK;
			}
			break;
		}
		case 2021: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 5;
				hr = S_OK;
			}
			break;
		}
		case 2022: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 1;
				hr = S_OK;
			}
			break;
		}
		case 2023: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 2;
				hr = S_OK;
			}
			break;
		}
		case 2024: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 3;
				hr = S_OK;
			}
			break;
		}
		case 2025: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 1;
				hr = S_OK;
			}
			break;
		}
		case 2026: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 2;
				hr = S_OK;
			}
			break;
		}
		case 2027: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 3;
				hr = S_OK;
			}
			break;
		}
		case 2028: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 4;
				hr = S_OK;
			}
			break;
		}
		case 2029: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 5;
				hr = S_OK;
			}
			break;
		}
		case 2030: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 6;
				hr = S_OK;
			}
			break;
		}
		case 2031: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 7;
				hr = S_OK;
			}
			break;
		}
		case 2032: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 8;
				hr = S_OK;
			}
			break;
		}
		case 2033: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 9;
				hr = S_OK;
			}
			break;
		}
		case 2034: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 1;
				hr = S_OK;
			}
			break;
		}
		case 2035: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 2;
				hr = S_OK;
			}
			break;
		}
		case 2036: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 3;
				hr = S_OK;
			}
			break;
		}
		case 2037: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 4;
				hr = S_OK;
			}
			break;
		}
		case 2038: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 0;
				hr = S_OK;
			}
			break;
		}
		case 2039: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 1;
				hr = S_OK;
			}
			break;
		}
		case 2040: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 2;
				hr = S_OK;
			}
			break;
		}
		case 2041: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 0;
				hr = S_OK;
			}
			break;
		}
		case 2042: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 1;
				hr = S_OK;
			}
			break;
		}
		case 2043: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 1;
				hr = S_OK;
			}
			break;
		}
		case 2044: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 2;
				hr = S_OK;
			}
			break;
		}
		case 2045: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 3;
				hr = S_OK;
			}
			break;
		}
		case 2046: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 4;
				hr = S_OK;
			}
			break;
		}
		case 2047: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 5;
				hr = S_OK;
			}
			break;
		}
		case 2048: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 6;
				hr = S_OK;
			}
			break;
		}
		case 2049: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 7;
				hr = S_OK;
			}
			break;
		}
		case 2050: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 8;
				hr = S_OK;
			}
			break;
		}
		case 2051: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 9;
				hr = S_OK;
			}
			break;
		}
		case 2052: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 10;
				hr = S_OK;
			}
			break;
		}
		case 2053: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 11;
				hr = S_OK;
			}
			break;
		}
		case 2054: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 12;
				hr = S_OK;
			}
			break;
		}
		case 2055: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 13;
				hr = S_OK;
			}
			break;
		}
		case 2056: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 14;
				hr = S_OK;
			}
			break;
		}
		case 2057: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 15;
				hr = S_OK;
			}
			break;
		}
		case 2058: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 16;
				hr = S_OK;
			}
			break;
		}
		case 2059: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 17;
				hr = S_OK;
			}
			break;
		}
		case 2060: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 18;
				hr = S_OK;
			}
			break;
		}
		case 2061: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 19;
				hr = S_OK;
			}
			break;
		}
		case 2062: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 20;
				hr = S_OK;
			}
			break;
		}
		case 2063: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 21;
				hr = S_OK;
			}
			break;
		}
		case 2064: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 22;
				hr = S_OK;
			}
			break;
		}
		case 2065: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 23;
				hr = S_OK;
			}
			break;
		}
		case 2066: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 24;
				hr = S_OK;
			}
			break;
		}
		case 2067: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 25;
				hr = S_OK;
			}
			break;
		}
		case 2068: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 26;
				hr = S_OK;
			}
			break;
		}
		case 2069: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 27;
				hr = S_OK;
			}
			break;
		}
		case 2070: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 28;
				hr = S_OK;
			}
			break;
		}
		case 2071: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 29;
				hr = S_OK;
			}
			break;
		}
		case 2072: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 30;
				hr = S_OK;
			}
			break;
		}
		case 2073: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 31;
				hr = S_OK;
			}
			break;
		}
		case 2074: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 32;
				hr = S_OK;
			}
			break;
		}
		case 2075: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 33;
				hr = S_OK;
			}
			break;
		}
		case 2076: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 34;
				hr = S_OK;
			}
			break;
		}
		case 2077: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 35;
				hr = S_OK;
			}
			break;
		}
		case 2078: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 36;
				hr = S_OK;
			}
			break;
		}
		case 2079: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 37;
				hr = S_OK;
			}
			break;
		}
		case 2080: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 38;
				hr = S_OK;
			}
			break;
		}
		case 2081: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 39;
				hr = S_OK;
			}
			break;
		}
		case 2082: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 40;
				hr = S_OK;
			}
			break;
		}
		case 2083: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 41;
				hr = S_OK;
			}
			break;
		}
		case 2084: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 42;
				hr = S_OK;
			}
			break;
		}
		case 2085: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 43;
				hr = S_OK;
			}
			break;
		}
		case 2086: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 44;
				hr = S_OK;
			}
			break;
		}
		case 2087: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 45;
				hr = S_OK;
			}
			break;
		}
		case 2088: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 46;
				hr = S_OK;
			}
			break;
		}
		case 2089: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 47;
				hr = S_OK;
			}
			break;
		}
		case 2090: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 48;
				hr = S_OK;
			}
			break;
		}
		case 2091: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 49;
				hr = S_OK;
			}
			break;
		}
		case 2092: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 50;
				hr = S_OK;
			}
			break;
		}
		case 2093: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 51;
				hr = S_OK;
			}
			break;
		}
		case 2094: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 52;
				hr = S_OK;
			}
			break;
		}
		case 2095: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 53;
				hr = S_OK;
			}
			break;
		}
		case 2096: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 54;
				hr = S_OK;
			}
			break;
		}
		case 2097: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 55;
				hr = S_OK;
			}
			break;
		}
		case 2098: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 56;
				hr = S_OK;
			}
			break;
		}
		case 2099: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 57;
				hr = S_OK;
			}
			break;
		}
		case 2100: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 58;
				hr = S_OK;
			}
			break;
		}
		case 2101: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 59;
				hr = S_OK;
			}
			break;
		}
		case 2102: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 60;
				hr = S_OK;
			}
			break;
		}
		case 2103: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 61;
				hr = S_OK;
			}
			break;
		}
		case 2104: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 62;
				hr = S_OK;
			}
			break;
		}
		case 2105: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 63;
				hr = S_OK;
			}
			break;
		}
		case 2106: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 64;
				hr = S_OK;
			}
			break;
		}
		case 2107: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 65;
				hr = S_OK;
			}
			break;
		}
		case 2108: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 66;
				hr = S_OK;
			}
			break;
		}
		case 2109: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 67;
				hr = S_OK;
			}
			break;
		}
		case 2110: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 68;
				hr = S_OK;
			}
			break;
		}
		case 2111: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 69;
				hr = S_OK;
			}
			break;
		}
		case 2112: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 70;
				hr = S_OK;
			}
			break;
		}
		case 2113: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 71;
				hr = S_OK;
			}
			break;
		}
		case 2114: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 72;
				hr = S_OK;
			}
			break;
		}
		case 2115: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 73;
				hr = S_OK;
			}
			break;
		}
		case 2116: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 74;
				hr = S_OK;
			}
			break;
		}
		case 2117: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 75;
				hr = S_OK;
			}
			break;
		}
		case 2118: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 76;
				hr = S_OK;
			}
			break;
		}
		case 2119: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 77;
				hr = S_OK;
			}
			break;
		}
		case 2120: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 78;
				hr = S_OK;
			}
			break;
		}
		case 2121: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 79;
				hr = S_OK;
			}
			break;
		}
		case 2122: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 80;
				hr = S_OK;
			}
			break;
		}
		case 2123: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 81;
				hr = S_OK;
			}
			break;
		}
		case 2124: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 82;
				hr = S_OK;
			}
			break;
		}
		case 2125: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 83;
				hr = S_OK;
			}
			break;
		}
		case 2126: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 84;
				hr = S_OK;
			}
			break;
		}
		case 2127: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 85;
				hr = S_OK;
			}
			break;
		}
		case 2128: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 1000;
				hr = S_OK;
			}
			break;
		}
		case 2129: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 1001;
				hr = S_OK;
			}
			break;
		}
		case 2130: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 1002;
				hr = S_OK;
			}
			break;
		}
		case 2131: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 1003;
				hr = S_OK;
			}
			break;
		}
		case 2132: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 0;
				hr = S_OK;
			}
			break;
		}
		case 2133: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 1;
				hr = S_OK;
			}
			break;
		}
		case 2134: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 2;
				hr = S_OK;
			}
			break;
		}
		case 2135: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 0;
				hr = S_OK;
			}
			break;
		}
		case 2136: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 1;
				hr = S_OK;
			}
			break;
		}
		case 2137: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 2;
				hr = S_OK;
			}
			break;
		}
		case 2138: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 3;
				hr = S_OK;
			}
			break;
		}
		case 2139: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 4;
				hr = S_OK;
			}
			break;
		}
		case 2140: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 5;
				hr = S_OK;
			}
			break;
		}
		case 2141: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 6;
				hr = S_OK;
			}
			break;
		}
		case 2142: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 0;
				hr = S_OK;
			}
			break;
		}
		case 2143: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				V_VT(pVarResult) = VT_I2;
				V_I2(pVarResult) = 1;
				hr = S_OK;
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

STDMETHODIMP ScriptGlobalTable::GetDocumentation(INT index, BSTR *pBstrName, BSTR *pBstrDocString, DWORD *pdwHelpContext, BSTR *pBstrHelpFile) {
	if (index == MEMBERID_NIL) {
		*pBstrName = SysAllocString(L"TableGlobal");
		return S_OK;
	}
	return E_NOTIMPL;
}

disp_id_name_t PinTable::m_idNameMap[] = {
		{ NULL },
		{ DISPID_GameEvents_KeyDown, L"_KeyDown" },
		{ DISPID_GameEvents_KeyUp, L"_KeyUp" },
		{ DISPID_GameEvents_Init, L"_Init" },
		{ DISPID_GameEvents_MusicDone, L"_MusicDone" },
		{ DISPID_GameEvents_Exit, L"_Exit" },
		{ DISPID_GameEvents_Paused, L"_Paused" },
		{ DISPID_GameEvents_UnPaused, L"_UnPaused" }
};

HRESULT PinTable::FireDispID(const DISPID dispid, DISPPARAMS * const pdispparams) {
	static WCHAR wzName[MAXSTRING];
	CComPtr<IDispatch> disp;
	g_pplayer->m_ptable->m_pcv->m_pScript->GetScriptDispatch(nullptr, &disp);
	size_t min = 1, max = ARRAY_SIZE(m_idNameMap) - 1, i;
	int r;
	while(min <= max) {
		i = (min + max) / 2;
		if (m_idNameMap[i].dispId == dispid) {
			wcscpy(wzName, m_wzName);
			wcscat(wzName, m_idNameMap[i].name);
			LPOLESTR fnNames = (LPOLESTR)wzName;
			DISPID tDispid;
			if (SUCCEEDED(disp->GetIDsOfNames(IID_NULL, &fnNames, 1, 0, &tDispid))) {
				return disp->Invoke(tDispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, pdispparams, nullptr, nullptr, nullptr);
			}
			return DISP_E_MEMBERNOTFOUND;
		}
		else if (m_idNameMap[i].dispId < dispid)
		   min = i+1;
		else
		   max = i-1;
	}
	return DISP_E_MEMBERNOTFOUND;
}

name_disp_id_t DebuggerModule::m_nameIDMap[] = {
		{ NULL },
		{ L"Print", 10 }
};

STDMETHODIMP DebuggerModule::GetIDsOfNames(REFIID /*riid*/, LPOLESTR* rgszNames, UINT cNames, LCID lcid, DISPID* rgDispId) {
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

STDMETHODIMP DebuggerModule::Invoke(DISPID dispIdMember, REFIID /*riid*/, LCID lcid, WORD wFlags, DISPPARAMS* pDispParams, VARIANT* pVarResult, EXCEPINFO* pExcepInfo, UINT* puArgErr) {
	HRESULT hr = DISP_E_UNKNOWNNAME;
	int index = pDispParams->cArgs;
	switch(dispIdMember) {
		case 10: {
			if (wFlags & DISPATCH_METHOD) {
				// line 824: [id(10), helpstring("method Print")] HRESULT Print([in, optional] VARIANT* pvar);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantCopyInd(&var0, &pDispParams->rgvarg[--index]);
				hr = Print(&var0);
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

STDMETHODIMP DebuggerModule::GetDocumentation(INT index, BSTR *pBstrName, BSTR *pBstrDocString, DWORD *pdwHelpContext, BSTR *pBstrHelpFile) {
	if (index == MEMBERID_NIL) {
		*pBstrName = SysAllocString(L"VPDebug");
		return S_OK;
	}
	return E_NOTIMPL;
}

name_disp_id_t Surface::m_nameIDMap[] = {
		{ NULL },
		{ L"BlendDisableLighting", 494 },
		{ L"BlendDisableLightingFromBelow", 496 },
		{ L"CanDrop", 11 },
		{ L"Collidable", 111 },
		{ L"Disabled", 108 },
		{ L"DisableLighting", 484 },
		{ L"DisplayTexture", 13 },
		{ L"Elasticity", 110 },
		{ L"ElasticityFalloff", 120 },
		{ L"FlipbookAnimation", 113 },
		{ L"Friction", 114 },
		{ L"HasHitEvent", 34 },
		{ L"HeightBottom", 8 },
		{ L"HeightTop", 9 },
		{ L"Image", DISPID_Image },
		{ L"ImageAlignment", 7 },
		{ L"IsBottomSolid", 116 },
		{ L"IsDropped", 12 },
		{ L"Name", DISPID_Name },
		{ L"OverwritePhysics", 432 },
		{ L"PhysicsMaterial", 734 },
		{ L"PlaySlingshotHit", 999 },
		{ L"ReflectionEnabled", 431 },
		{ L"Scatter", 115 },
		{ L"SideImage", DISPID_Image2 },
		{ L"SideMaterial", 341 },
		{ L"SideVisible", 109 },
		{ L"SlingshotAnimation", 112 },
		{ L"SlingshotMaterial", 426 },
		{ L"SlingshotStrength", 14 },
		{ L"SlingshotThreshold", 427 },
		{ L"Threshold", 33 },
		{ L"TimerEnabled", DISPID_Timer_Enabled },
		{ L"TimerInterval", DISPID_Timer_Interval },
		{ L"TopMaterial", 340 },
		{ L"UserValue", DISPID_UserValue },
		{ L"Visible", 16 }
};

STDMETHODIMP Surface::GetIDsOfNames(REFIID /*riid*/, LPOLESTR* rgszNames, UINT cNames, LCID lcid, DISPID* rgDispId) {
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

STDMETHODIMP Surface::Invoke(DISPID dispIdMember, REFIID /*riid*/, LCID lcid, WORD wFlags, DISPPARAMS* pDispParams, VARIANT* pVarResult, EXCEPINFO* pExcepInfo, UINT* puArgErr) {
	HRESULT hr = DISP_E_UNKNOWNNAME;
	int index = pDispParams->cArgs;
	switch(dispIdMember) {
		case DISPID_Name: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 839: [propget, id(DISPID_Name), helpstring("property Name")] HRESULT Name([out, retval] BSTR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_Name(&V_BSTR(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 840: [propput, id(DISPID_Name), helpstring("property Name")] HRESULT Name([in] BSTR newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = put_Name(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Timer_Enabled: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 841: [propget, id(DISPID_Timer_Enabled), helpstring("property TimerEnabled")] HRESULT TimerEnabled([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_TimerEnabled(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 842: [propput, id(DISPID_Timer_Enabled), helpstring("property TimerEnabled")] HRESULT TimerEnabled([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_TimerEnabled(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Timer_Interval: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 843: [propget, id(DISPID_Timer_Interval), helpstring("property TimerInterval")] HRESULT TimerInterval([out, retval] long *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_TimerInterval((long*)&V_I4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 844: [propput, id(DISPID_Timer_Interval), helpstring("property TimerInterval")] HRESULT TimerInterval([in] long newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_I4);
				hr = put_TimerInterval(V_I4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 34: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 845: [propget, id(34), helpstring("property HasHitEvent")] HRESULT HasHitEvent([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_HasHitEvent(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 846: [propput, id(34), helpstring("property HasHitEvent")] HRESULT HasHitEvent([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_HasHitEvent(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 33: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 847: [propget, id(33), helpstring("property Threshold")] HRESULT Threshold([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Threshold(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 848: [propput, id(33), helpstring("property Threshold")] HRESULT Threshold([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Threshold(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Image: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 849: [propget, id(DISPID_Image), helpstring("property Image")] HRESULT Image([out, retval] BSTR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_Image(&V_BSTR(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 850: [propput, id(DISPID_Image), helpstring("property Image")] HRESULT Image([in] BSTR newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = put_Image(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 341: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 851: [propget, id(341), helpstring("property SideMaterial")] HRESULT SideMaterial([out, retval] BSTR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_SideMaterial(&V_BSTR(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 852: [propput, id(341), helpstring("property SideMaterial")] HRESULT SideMaterial([in] BSTR newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = put_SideMaterial(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 7: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 853: [propget, id(7), helpstring("property ImageAlignment")] HRESULT ImageAlignment([out, retval] ImageAlignment *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_ImageAlignment((ImageAlignment*)&V_I4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 854: [propput, id(7), helpstring("property ImageAlignment")] HRESULT ImageAlignment([in] ImageAlignment newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_I4);
				hr = put_ImageAlignment((ImageAlignment)V_I4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 8: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 855: [propget, id(8), helpstring("property HeightBottom")] HRESULT HeightBottom([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_HeightBottom(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 856: [propput, id(8), helpstring("property HeightBottom")] HRESULT HeightBottom([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_HeightBottom(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 9: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 857: [propget, id(9), helpstring("property HeightTop")] HRESULT HeightTop([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_HeightTop(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 858: [propput, id(9), helpstring("property HeightTop")] HRESULT HeightTop([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_HeightTop(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 340: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 859: [propget, id(340), helpstring("property TopMaterial")] HRESULT TopMaterial([out, retval] BSTR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_TopMaterial(&V_BSTR(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 860: [propput, id(340), helpstring("property TopMaterial")] HRESULT TopMaterial([in] BSTR newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = put_TopMaterial(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 11: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 861: [propget, id(11), helpstring("property CanDrop")] HRESULT CanDrop([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_CanDrop(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 862: [propput, id(11), helpstring("property CanDrop")] HRESULT CanDrop([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_CanDrop(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 111: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 864: [propget, id(111), helpstring("property Collidable")] HRESULT Collidable([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_Collidable(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 865: [propput, id(111), helpstring("property Collidable")] HRESULT Collidable([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_Collidable(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 12: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 867: [propget, id(12), helpstring("property IsDropped")] HRESULT IsDropped([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_IsDropped(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 868: [propput, id(12), helpstring("property IsDropped")] HRESULT IsDropped([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_IsDropped(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 13: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 869: [propget, id(13), helpstring("property DisplayTexture")] HRESULT DisplayTexture([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_DisplayTexture(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 870: [propput, id(13), helpstring("property DisplayTexture")] HRESULT DisplayTexture([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_DisplayTexture(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 14: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 871: [propget, id(14), helpstring("property SlingshotStrength")] HRESULT SlingshotStrength([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_SlingshotStrength(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 872: [propput, id(14), helpstring("property SlingshotStrength")] HRESULT SlingshotStrength([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_SlingshotStrength(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 110: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 873: [propget, id(110), helpstring("property Elasticity")] HRESULT Elasticity([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Elasticity(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 874: [propput, id(110), helpstring("property Elasticity")] HRESULT Elasticity([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Elasticity(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 120: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 875: [propget, id(120), helpstring("property ElasticityFalloff")] HRESULT ElasticityFalloff([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_ElasticityFalloff(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 876: [propput, id(120), helpstring("property ElasticityFalloff")] HRESULT ElasticityFalloff([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_ElasticityFalloff(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Image2: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 877: [propget, id(DISPID_Image2), helpstring("property SideImage")] HRESULT SideImage([out, retval] BSTR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_SideImage(&V_BSTR(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 878: [propput, id(DISPID_Image2), helpstring("property SideImage")] HRESULT SideImage([in] BSTR newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = put_SideImage(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 16: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 879: [propget, id(16), helpstring("property Visible")] HRESULT Visible([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_Visible(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 880: [propput, id(16), helpstring("property Visible")] HRESULT Visible([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_Visible(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 108: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 881: [propget, id(108), helpstring("property Disabled")] HRESULT Disabled([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_Disabled(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 882: [propput, id(108), helpstring("property Disabled")] HRESULT Disabled([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_Disabled(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 109: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 883: [propget, id(109), helpstring("property SideVisible")] HRESULT SideVisible([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_SideVisible(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 884: [propput, id(109), helpstring("property SideVisible")] HRESULT SideVisible([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_SideVisible(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_UserValue: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 885: [propget, id(DISPID_UserValue), helpstring("property User")] HRESULT UserValue([out, retval] VARIANT* pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				hr = get_UserValue(pVarResult);
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 886: [propput, id(DISPID_UserValue), helpstring("property User")] HRESULT UserValue([in] VARIANT* newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantCopyInd(&var0, &pDispParams->rgvarg[--index]);
				hr = put_UserValue(&var0);
				VariantClear(&var0);
			}
			break;
		}
		case 426: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 887: [propget, id(426), helpstring("property SlingshotMaterial")] HRESULT SlingshotMaterial([out, retval] BSTR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_SlingshotMaterial(&V_BSTR(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 888: [propput, id(426), helpstring("property SlingshotMaterial")] HRESULT SlingshotMaterial([in] BSTR newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = put_SlingshotMaterial(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 427: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 889: [propget, id(427), helpstring("property SlingshotThreshold")] HRESULT SlingshotThreshold([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_SlingshotThreshold(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 890: [propput, id(427), helpstring("property SlingshotThreshold")] HRESULT SlingshotThreshold([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_SlingshotThreshold(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 112: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 891: [propget, id(112), helpstring("property SlingshotAnimation")] HRESULT SlingshotAnimation([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_SlingshotAnimation(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 892: [propput, id(112), helpstring("property SlingshotAnimation")] HRESULT SlingshotAnimation([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_SlingshotAnimation(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 113: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 893: [propget, id(113), helpstring("property FlipbookAnimation")] HRESULT FlipbookAnimation([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_FlipbookAnimation(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 894: [propput, id(113), helpstring("property FlipbookAnimation")] HRESULT FlipbookAnimation([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_FlipbookAnimation(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 116: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 895: [propget, id(116), helpstring("property IsBottomSolid")] HRESULT IsBottomSolid([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_IsBottomSolid(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 896: [propput, id(116), helpstring("property IsBottomSolid")] HRESULT IsBottomSolid([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_IsBottomSolid(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 484: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 897: [propget, id(484), helpstring("property DisableLighting")] HRESULT DisableLighting([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_DisableLighting(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 898: [propput, id(484), helpstring("property DisableLighting")] HRESULT DisableLighting([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_DisableLighting(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 494: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 899: [propget, id(494), helpstring("property BlendDisableLighting")] HRESULT BlendDisableLighting([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_BlendDisableLighting(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 900: [propput, id(494), helpstring("property BlendDisableLighting")] HRESULT BlendDisableLighting([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_BlendDisableLighting(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 496: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 901: [propget, id(496), helpstring("property BlendDisableLightingFromBelow")] HRESULT BlendDisableLightingFromBelow([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_BlendDisableLightingFromBelow(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 902: [propput, id(496), helpstring("property BlendDisableLightingFromBelow")] HRESULT BlendDisableLightingFromBelow([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_BlendDisableLightingFromBelow(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 114: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 903: [propget, id(114), helpstring("property Friction")] HRESULT Friction([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Friction(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 904: [propput, id(114), helpstring("property Friction")] HRESULT Friction([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Friction(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 115: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 905: [propget, id(115), helpstring("property Scatter")] HRESULT Scatter([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Scatter(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 906: [propput, id(115), helpstring("property Scatter")] HRESULT Scatter([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Scatter(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 431: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 907: [propget, id(431), helpstring("property ReflectionEnabled")] HRESULT ReflectionEnabled([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_ReflectionEnabled(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 908: [propput, id(431), helpstring("property ReflectionEnabled")] HRESULT ReflectionEnabled([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_ReflectionEnabled(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 999: {
			if (wFlags & DISPATCH_METHOD) {
				// line 909: [id(999), helpstring("method PlaySlingshotHit")] HRESULT PlaySlingshotHit();
				hr = PlaySlingshotHit();
			}
			break;
		}
		case 734: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 910: [propget, id(734), helpstring("property PhysicsMaterial")] HRESULT PhysicsMaterial([out, retval] BSTR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_PhysicsMaterial(&V_BSTR(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 911: [propput, id(734), helpstring("property PhysicsMaterial")] HRESULT PhysicsMaterial([in] BSTR newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = put_PhysicsMaterial(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 432: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 912: [propget, id(432), helpstring("property OverwritePhysics")] HRESULT OverwritePhysics([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_OverwritePhysics(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 913: [propput, id(432), helpstring("property OverwritePhysics")] HRESULT OverwritePhysics([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_OverwritePhysics(V_BOOL(&var0));
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

STDMETHODIMP Surface::GetDocumentation(INT index, BSTR *pBstrName, BSTR *pBstrDocString, DWORD *pdwHelpContext, BSTR *pBstrHelpFile) {
	if (index == MEMBERID_NIL) {
		*pBstrName = SysAllocString(L"Wall");
		return S_OK;
	}
	return E_NOTIMPL;
}

disp_id_name_t Surface::m_idNameMap[] = {
		{ NULL },
		{ DISPID_GameEvents_Init, L"_Init" },
		{ DISPID_SurfaceEvents_Slingshot, L"_Slingshot" },
		{ DISPID_TimerEvents_Timer, L"_Timer" },
		{ DISPID_HitEvents_Hit, L"_Hit" }
};

HRESULT Surface::FireDispID(const DISPID dispid, DISPPARAMS * const pdispparams) {
	static WCHAR wzName[MAXSTRING];
	CComPtr<IDispatch> disp;
	g_pplayer->m_ptable->m_pcv->m_pScript->GetScriptDispatch(nullptr, &disp);
	size_t min = 1, max = ARRAY_SIZE(m_idNameMap) - 1, i;
	int r;
	while(min <= max) {
		i = (min + max) / 2;
		if (m_idNameMap[i].dispId == dispid) {
			wcscpy(wzName, m_wzName);
			wcscat(wzName, m_idNameMap[i].name);
			LPOLESTR fnNames = (LPOLESTR)wzName;
			DISPID tDispid;
			if (SUCCEEDED(disp->GetIDsOfNames(IID_NULL, &fnNames, 1, 0, &tDispid))) {
				return disp->Invoke(tDispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, pdispparams, nullptr, nullptr, nullptr);
			}
			return DISP_E_MEMBERNOTFOUND;
		}
		else if (m_idNameMap[i].dispId < dispid)
		   min = i+1;
		else
		   max = i-1;
	}
	return DISP_E_MEMBERNOTFOUND;
}

name_disp_id_t DragPoint::m_nameIDMap[] = {
		{ NULL },
		{ L"CalcHeight", 377 },
		{ L"IsAutoTextureCoordinate", 4 },
		{ L"Smooth", 3 },
		{ L"TextureCoordinateU", 5 },
		{ L"X", 1 },
		{ L"Y", 2 },
		{ L"Z", 6 }
};

STDMETHODIMP DragPoint::GetIDsOfNames(REFIID /*riid*/, LPOLESTR* rgszNames, UINT cNames, LCID lcid, DISPID* rgDispId) {
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

STDMETHODIMP DragPoint::Invoke(DISPID dispIdMember, REFIID /*riid*/, LCID lcid, WORD wFlags, DISPPARAMS* pDispParams, VARIANT* pVarResult, EXCEPINFO* pExcepInfo, UINT* puArgErr) {
	HRESULT hr = DISP_E_UNKNOWNNAME;
	int index = pDispParams->cArgs;
	switch(dispIdMember) {
		case 1: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 938: [propget, id(1), helpstring("property X")] HRESULT X([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_X(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 939: [propput, id(1), helpstring("property X")] HRESULT X([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_X(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 2: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 940: [propget, id(2), helpstring("property Y")] HRESULT Y([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Y(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 941: [propput, id(2), helpstring("property Y")] HRESULT Y([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Y(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 6: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 942: [propget, id(6), helpstring("property Z")] HRESULT Z([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Z(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 943: [propput, id(6), helpstring("property Z")] HRESULT Z([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Z(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 377: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 944: [propget, id(377), helpstring("property calcHeight")] HRESULT CalcHeight([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_CalcHeight(&V_R4(pVarResult));
			}
			break;
		}
		case 3: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 945: [propget, id(3), helpstring("property Smooth")] HRESULT Smooth([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_Smooth(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 946: [propput, id(3), helpstring("property Smooth")] HRESULT Smooth([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_Smooth(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 4: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 947: [propget, id(4), helpstring("property IsAutoTextureCoordinate")] HRESULT IsAutoTextureCoordinate([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_IsAutoTextureCoordinate(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 948: [propput, id(4), helpstring("property IsAutoTextureCoordinate")] HRESULT IsAutoTextureCoordinate([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_IsAutoTextureCoordinate(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 5: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 949: [propget, id(5), helpstring("property TextureCoordinateU")] HRESULT TextureCoordinateU([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_TextureCoordinateU(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 950: [propput, id(5), helpstring("property TextureCoordinateU")] HRESULT TextureCoordinateU([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_TextureCoordinateU(V_R4(&var0));
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

STDMETHODIMP DragPoint::GetDocumentation(INT index, BSTR *pBstrName, BSTR *pBstrDocString, DWORD *pdwHelpContext, BSTR *pBstrHelpFile) {
	if (index == MEMBERID_NIL) {
		*pBstrName = SysAllocString(L"ControlPoint");
		return S_OK;
	}
	return E_NOTIMPL;
}

name_disp_id_t Flipper::m_nameIDMap[] = {
		{ NULL },
		{ L"BaseRadius", 1 },
		{ L"CurrentAngle", 12 },
		{ L"Elasticity", 110 },
		{ L"ElasticityFalloff", 28 },
		{ L"Enabled", 394 },
		{ L"EndAngle", 7 },
		{ L"EndRadius", 2 },
		{ L"EOSTorque", 113 },
		{ L"EOSTorqueAngle", 189 },
		{ L"FlipperRadiusMin", 111 },
		{ L"Friction", 114 },
		{ L"Height", 107 },
		{ L"Image", DISPID_Image },
		{ L"Length", 3 },
		{ L"Mass", DISPID_Flipper_Speed },
		{ L"Material", 340 },
		{ L"Name", DISPID_Name },
		{ L"OverridePhysics", DISPID_Flipper_OverridePhysics },
		{ L"RampUp", 27 },
		{ L"ReflectionEnabled", 431 },
		{ L"Return", 23 },
		{ L"RotateToEnd", 5 },
		{ L"RotateToStart", 6 },
		{ L"RubberHeight", 24 },
		{ L"RubberMaterial", 341 },
		{ L"RubberThickness", 18 },
		{ L"RubberWidth", 25 },
		{ L"Scatter", 115 },
		{ L"StartAngle", 4 },
		{ L"Strength", 19 },
		{ L"Surface", DISPID_Surface },
		{ L"TimerEnabled", DISPID_Timer_Enabled },
		{ L"TimerInterval", DISPID_Timer_Interval },
		{ L"UserValue", DISPID_UserValue },
		{ L"Visible", 458 },
		{ L"X", 13 },
		{ L"Y", 14 }
};

STDMETHODIMP Flipper::GetIDsOfNames(REFIID /*riid*/, LPOLESTR* rgszNames, UINT cNames, LCID lcid, DISPID* rgDispId) {
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

STDMETHODIMP Flipper::Invoke(DISPID dispIdMember, REFIID /*riid*/, LCID lcid, WORD wFlags, DISPPARAMS* pDispParams, VARIANT* pVarResult, EXCEPINFO* pExcepInfo, UINT* puArgErr) {
	HRESULT hr = DISP_E_UNKNOWNNAME;
	int index = pDispParams->cArgs;
	switch(dispIdMember) {
		case 1: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 968: [propget, id(1), helpstring("property BaseRadius")] HRESULT BaseRadius([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_BaseRadius(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 969: [propput, id(1), helpstring("property BaseRadius")] HRESULT BaseRadius([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_BaseRadius(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 2: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 970: [propget, id(2), helpstring("property EndRadius")] HRESULT EndRadius([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_EndRadius(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 971: [propput, id(2), helpstring("property EndRadius")] HRESULT EndRadius([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_EndRadius(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 3: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 972: [propget, id(3), helpstring("property Length")] HRESULT Length([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Length(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 973: [propput, id(3), helpstring("property Length")] HRESULT Length([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Length(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 4: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 974: [propget, id(4), helpstring("property StartAngle")] HRESULT StartAngle([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_StartAngle(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 975: [propput, id(4), helpstring("property StartAngle")] HRESULT StartAngle([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_StartAngle(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 5: {
			if (wFlags & DISPATCH_METHOD) {
				// line 976: [id(5), helpstring("method RotateToEnd")] HRESULT RotateToEnd();
				hr = RotateToEnd();
			}
			break;
		}
		case 6: {
			if (wFlags & DISPATCH_METHOD) {
				// line 977: [id(6), helpstring("method RotateToStart")] HRESULT RotateToStart();
				hr = RotateToStart();
			}
			break;
		}
		case 7: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 978: [propget, id(7), helpstring("property EndAngle")] HRESULT EndAngle([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_EndAngle(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 979: [propput, id(7), helpstring("property EndAngle")] HRESULT EndAngle([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_EndAngle(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Timer_Enabled: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 984: [propget, id(DISPID_Timer_Enabled), helpstring("property TimerEnabled")] HRESULT TimerEnabled([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_TimerEnabled(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 985: [propput, id(DISPID_Timer_Enabled), helpstring("property TimerEnabled")] HRESULT TimerEnabled([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_TimerEnabled(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Timer_Interval: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 986: [propget, id(DISPID_Timer_Interval), helpstring("property TimerInterval")] HRESULT TimerInterval([out, retval] long *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_TimerInterval((long*)&V_I4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 987: [propput, id(DISPID_Timer_Interval), helpstring("property TimerInterval")] HRESULT TimerInterval([in] long newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_I4);
				hr = put_TimerInterval(V_I4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 12: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 988: [propget, id(12), nonbrowsable, helpstring("property CurrentAngle")] HRESULT CurrentAngle([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_CurrentAngle(&V_R4(pVarResult));
			}
			break;
		}
		case 13: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 989: [propget, id(13), helpstring("property X")] HRESULT X([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_X(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 990: [propput, id(13), helpstring("property X")] HRESULT X([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_X(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 14: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 991: [propget, id(14), helpstring("property Y")] HRESULT Y([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Y(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 992: [propput, id(14), helpstring("property Y")] HRESULT Y([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Y(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 340: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 993: [propget, id(340), helpstring("property Material")] HRESULT Material([out, retval] BSTR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_Material(&V_BSTR(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 994: [propput, id(340), helpstring("property Material")] HRESULT Material([in] BSTR newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = put_Material(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Flipper_Speed: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 995: [propget, id(DISPID_Flipper_Speed), helpstring("property Mass")] HRESULT Mass([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Mass(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 996: [propput, id(DISPID_Flipper_Speed), helpstring("property Mass")] HRESULT Mass([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Mass(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Flipper_OverridePhysics: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 997: [propget, id(DISPID_Flipper_OverridePhysics), helpstring("property OverridePhysics")] HRESULT OverridePhysics([out, retval] PhysicsSet *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_OverridePhysics((PhysicsSet*)&V_I4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 998: [propput, id(DISPID_Flipper_OverridePhysics), helpstring("property OverridePhysics")] HRESULT OverridePhysics([in] PhysicsSet newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_I4);
				hr = put_OverridePhysics((PhysicsSet)V_I4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 341: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 999: [propget, id(341), helpstring("property RubberMaterial")] HRESULT RubberMaterial([out, retval] BSTR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_RubberMaterial(&V_BSTR(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1000: [propput, id(341), helpstring("property RubberMaterial")] HRESULT RubberMaterial([in] BSTR newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = put_RubberMaterial(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 18: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1001: [propget, id(18), helpstring("property RubberThickness")] HRESULT RubberThickness([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_RubberThickness(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1002: [propput, id(18), helpstring("property RubberThickness")] HRESULT RubberThickness([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_RubberThickness(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 19: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1003: [propget, id(19), helpstring("property Strength")] HRESULT Strength([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Strength(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1004: [propput, id(19), helpstring("property Strength")] HRESULT Strength([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Strength(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 458: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1005: [propget, id(458), helpstring("property Visible")] HRESULT Visible([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_Visible(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1006: [propput, id(458), helpstring("property Visible")] HRESULT Visible([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_Visible(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 394: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1007: [propget, id(394), helpstring("property Enabled")] HRESULT Enabled([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_Enabled(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1008: [propput, id(394), helpstring("property Enabled")] HRESULT Enabled([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_Enabled(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 110: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1009: [propget, id(110), helpstring("property Elasticity")] HRESULT Elasticity([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Elasticity(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1010: [propput, id(110), helpstring("property Elasticity")] HRESULT Elasticity([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Elasticity(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 115: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1011: [propget, id(115), helpstring("property Scatter")] HRESULT Scatter([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Scatter(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1012: [propput, id(115), helpstring("property Scatter")] HRESULT Scatter([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Scatter(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 23: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1013: [propget, id(23), helpstring("property Return")] HRESULT Return([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Return(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1014: [propput, id(23), helpstring("property Return")] HRESULT Return([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Return(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 24: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1015: [propget, id(24), helpstring("property RubberHeight")] HRESULT RubberHeight([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_RubberHeight(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1016: [propput, id(24), helpstring("property RubberHeight")] HRESULT RubberHeight([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_RubberHeight(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 25: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1017: [propget, id(25), helpstring("property RubberWidth")] HRESULT RubberWidth([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_RubberWidth(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1018: [propput, id(25), helpstring("property RubberWidth")] HRESULT RubberWidth([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_RubberWidth(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 114: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1019: [propget, id(114), helpstring("property Friction")] HRESULT Friction([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Friction(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1020: [propput, id(114), helpstring("property Friction")] HRESULT Friction([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Friction(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 27: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1021: [propget, id(27), helpstring("property RampUp")] HRESULT RampUp([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_RampUp(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1022: [propput, id(27), helpstring("property RampUp")] HRESULT RampUp([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_RampUp(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 28: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1023: [propget, id(28), helpstring("property ElasticityFalloff")] HRESULT ElasticityFalloff([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_ElasticityFalloff(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1024: [propput, id(28), helpstring("property ElasticityFalloff")] HRESULT ElasticityFalloff([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_ElasticityFalloff(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Surface: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1025: [propget, id(DISPID_Surface), helpstring("property Surface")] HRESULT Surface([out, retval] BSTR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_Surface(&V_BSTR(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1026: [propput, id(DISPID_Surface), helpstring("property Surface")] HRESULT Surface([in] BSTR newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = put_Surface(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Name: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1027: [propget, id(DISPID_Name), helpstring("property Name")] HRESULT Name([out, retval] BSTR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_Name(&V_BSTR(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1028: [propput, id(DISPID_Name), helpstring("property Name")] HRESULT Name([in] BSTR newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = put_Name(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_UserValue: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1029: [propget, id(DISPID_UserValue), helpstring("property User")] HRESULT UserValue([out, retval] VARIANT* pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				hr = get_UserValue(pVarResult);
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1030: [propput, id(DISPID_UserValue), helpstring("property User")] HRESULT UserValue([in] VARIANT* newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantCopyInd(&var0, &pDispParams->rgvarg[--index]);
				hr = put_UserValue(&var0);
				VariantClear(&var0);
			}
			break;
		}
		case 107: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1031: [propget, id(107), helpstring("property Height")] HRESULT Height([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Height(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1032: [propput, id(107), helpstring("property Height")] HRESULT Height([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Height(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 113: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1033: [propget, id(113), helpstring("property EOSTorque")] HRESULT EOSTorque([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_EOSTorque(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1034: [propput, id(113), helpstring("property EOSTorque")] HRESULT EOSTorque([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_EOSTorque(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 189: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1035: [propget, id(189), helpstring("property EOSTorqueAngle")] HRESULT EOSTorqueAngle([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_EOSTorqueAngle(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1036: [propput, id(189), helpstring("property EOSTorqueAngle")] HRESULT EOSTorqueAngle([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_EOSTorqueAngle(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 111: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1038: [propget, id(111), helpstring("property FlipperRadiusMin")] HRESULT FlipperRadiusMin([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_FlipperRadiusMin(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1039: [propput, id(111), helpstring("property FlipperRadiusMin")] HRESULT FlipperRadiusMin([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_FlipperRadiusMin(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Image: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1041: [propget, id(DISPID_Image), helpstring("property Image")] HRESULT Image([out, retval] BSTR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_Image(&V_BSTR(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1042: [propput, id(DISPID_Image), helpstring("property Image")] HRESULT Image([in] BSTR newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = put_Image(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 431: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1043: [propget, id(431), helpstring("property ReflectionEnabled")] HRESULT ReflectionEnabled([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_ReflectionEnabled(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1044: [propput, id(431), helpstring("property ReflectionEnabled")] HRESULT ReflectionEnabled([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_ReflectionEnabled(V_BOOL(&var0));
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

STDMETHODIMP Flipper::GetDocumentation(INT index, BSTR *pBstrName, BSTR *pBstrDocString, DWORD *pdwHelpContext, BSTR *pBstrHelpFile) {
	if (index == MEMBERID_NIL) {
		*pBstrName = SysAllocString(L"Flipper");
		return S_OK;
	}
	return E_NOTIMPL;
}

disp_id_name_t Flipper::m_idNameMap[] = {
		{ NULL },
		{ DISPID_GameEvents_Init, L"_Init" },
		{ DISPID_FlipperEvents_Collide, L"_Collide" },
		{ DISPID_TimerEvents_Timer, L"_Timer" },
		{ DISPID_HitEvents_Hit, L"_Hit" },
		{ DISPID_LimitEvents_EOS, L"_LimitEOS" },
		{ DISPID_LimitEvents_BOS, L"_LimitBOS" },
		{ DISPID_AnimateEvents_Animate, L"_Animate" }
};

HRESULT Flipper::FireDispID(const DISPID dispid, DISPPARAMS * const pdispparams) {
	static WCHAR wzName[MAXSTRING];
	CComPtr<IDispatch> disp;
	g_pplayer->m_ptable->m_pcv->m_pScript->GetScriptDispatch(nullptr, &disp);
	size_t min = 1, max = ARRAY_SIZE(m_idNameMap) - 1, i;
	int r;
	while(min <= max) {
		i = (min + max) / 2;
		if (m_idNameMap[i].dispId == dispid) {
			wcscpy(wzName, m_wzName);
			wcscat(wzName, m_idNameMap[i].name);
			LPOLESTR fnNames = (LPOLESTR)wzName;
			DISPID tDispid;
			if (SUCCEEDED(disp->GetIDsOfNames(IID_NULL, &fnNames, 1, 0, &tDispid))) {
				return disp->Invoke(tDispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, pdispparams, nullptr, nullptr, nullptr);
			}
			return DISP_E_MEMBERNOTFOUND;
		}
		else if (m_idNameMap[i].dispId < dispid)
		   min = i+1;
		else
		   max = i-1;
	}
	return DISP_E_MEMBERNOTFOUND;
}

name_disp_id_t Timer::m_nameIDMap[] = {
		{ NULL },
		{ L"Enabled", DISPID_Timer_Enabled },
		{ L"Interval", DISPID_Timer_Interval },
		{ L"Name", DISPID_Name },
		{ L"UserValue", DISPID_UserValue }
};

STDMETHODIMP Timer::GetIDsOfNames(REFIID /*riid*/, LPOLESTR* rgszNames, UINT cNames, LCID lcid, DISPID* rgDispId) {
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

STDMETHODIMP Timer::Invoke(DISPID dispIdMember, REFIID /*riid*/, LCID lcid, WORD wFlags, DISPPARAMS* pDispParams, VARIANT* pVarResult, EXCEPINFO* pExcepInfo, UINT* puArgErr) {
	HRESULT hr = DISP_E_UNKNOWNNAME;
	int index = pDispParams->cArgs;
	switch(dispIdMember) {
		case DISPID_Timer_Enabled: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1076: [propget, id(DISPID_Timer_Enabled), helpstring("property Enabled")] HRESULT Enabled([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_Enabled(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1077: [propput, id(DISPID_Timer_Enabled), helpstring("property Enabled")] HRESULT Enabled([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_Enabled(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Timer_Interval: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1078: [propget, id(DISPID_Timer_Interval), helpstring("property Interval")] HRESULT Interval([out, retval] long *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_Interval((long*)&V_I4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1079: [propput, id(DISPID_Timer_Interval), helpstring("property Interval")] HRESULT Interval([in] long newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_I4);
				hr = put_Interval(V_I4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Name: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1080: [propget, id(DISPID_Name), helpstring("property Name")] HRESULT Name([out, retval] BSTR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_Name(&V_BSTR(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1081: [propput, id(DISPID_Name), helpstring("property Name")] HRESULT Name([in] BSTR newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = put_Name(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_UserValue: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1082: [propget, id(DISPID_UserValue), helpstring("property User")] HRESULT UserValue([out, retval] VARIANT* pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				hr = get_UserValue(pVarResult);
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1083: [propput, id(DISPID_UserValue), helpstring("property User")] HRESULT UserValue([in] VARIANT* newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantCopyInd(&var0, &pDispParams->rgvarg[--index]);
				hr = put_UserValue(&var0);
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

STDMETHODIMP Timer::GetDocumentation(INT index, BSTR *pBstrName, BSTR *pBstrDocString, DWORD *pdwHelpContext, BSTR *pBstrHelpFile) {
	if (index == MEMBERID_NIL) {
		*pBstrName = SysAllocString(L"Timer");
		return S_OK;
	}
	return E_NOTIMPL;
}

disp_id_name_t Timer::m_idNameMap[] = {
		{ NULL },
		{ DISPID_GameEvents_Init, L"_Init" },
		{ DISPID_TimerEvents_Timer, L"_Timer" }
};

HRESULT Timer::FireDispID(const DISPID dispid, DISPPARAMS * const pdispparams) {
	static WCHAR wzName[MAXSTRING];
	CComPtr<IDispatch> disp;
	g_pplayer->m_ptable->m_pcv->m_pScript->GetScriptDispatch(nullptr, &disp);
	size_t min = 1, max = ARRAY_SIZE(m_idNameMap) - 1, i;
	int r;
	while(min <= max) {
		i = (min + max) / 2;
		if (m_idNameMap[i].dispId == dispid) {
			wcscpy(wzName, m_wzName);
			wcscat(wzName, m_idNameMap[i].name);
			LPOLESTR fnNames = (LPOLESTR)wzName;
			DISPID tDispid;
			if (SUCCEEDED(disp->GetIDsOfNames(IID_NULL, &fnNames, 1, 0, &tDispid))) {
				return disp->Invoke(tDispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, pdispparams, nullptr, nullptr, nullptr);
			}
			return DISP_E_MEMBERNOTFOUND;
		}
		else if (m_idNameMap[i].dispId < dispid)
		   min = i+1;
		else
		   max = i-1;
	}
	return DISP_E_MEMBERNOTFOUND;
}

name_disp_id_t Plunger::m_nameIDMap[] = {
		{ NULL },
		{ L"AnimFrames", DISPID_PluFrames },
		{ L"AutoPlunger", 116 },
		{ L"CreateBall", 5 },
		{ L"Fire", 2 },
		{ L"FireSpeed", 4 },
		{ L"Image", DISPID_Image },
		{ L"Material", 340 },
		{ L"MechPlunger", 110 },
		{ L"MechStrength", 111 },
		{ L"MomentumXfer", 118 },
		{ L"Name", DISPID_Name },
		{ L"ParkPosition", 112 },
		{ L"Position", 6 },
		{ L"PullBack", 1 },
		{ L"PullSpeed", 3 },
		{ L"ReflectionEnabled", 431 },
		{ L"RingDiam", DISPID_RingDiam },
		{ L"RingGap", DISPID_RingGap },
		{ L"RingWidth", DISPID_RingThickness },
		{ L"RodDiam", DISPID_RodDiam },
		{ L"ScatterVelocity", 114 },
		{ L"SpringDiam", DISPID_SpringDiam },
		{ L"SpringEndLoops", DISPID_SpringEndLoops },
		{ L"SpringGauge", DISPID_SpringGauge },
		{ L"SpringLoops", DISPID_SpringLoops },
		{ L"Stroke", 113 },
		{ L"Surface", DISPID_Surface },
		{ L"TimerEnabled", DISPID_Timer_Enabled },
		{ L"TimerInterval", DISPID_Timer_Interval },
		{ L"TipShape", DISPID_TipShape },
		{ L"Type", 390 },
		{ L"UserValue", DISPID_UserValue },
		{ L"Visible", 117 },
		{ L"Width", DISPID_Width },
		{ L"X", DISPID_X },
		{ L"Y", DISPID_Y },
		{ L"ZAdjust", DISPID_ZAdjust }
};

STDMETHODIMP Plunger::GetIDsOfNames(REFIID /*riid*/, LPOLESTR* rgszNames, UINT cNames, LCID lcid, DISPID* rgDispId) {
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

STDMETHODIMP Plunger::Invoke(DISPID dispIdMember, REFIID /*riid*/, LCID lcid, WORD wFlags, DISPPARAMS* pDispParams, VARIANT* pVarResult, EXCEPINFO* pExcepInfo, UINT* puArgErr) {
	HRESULT hr = DISP_E_UNKNOWNNAME;
	int index = pDispParams->cArgs;
	switch(dispIdMember) {
		case 1: {
			if (wFlags & DISPATCH_METHOD) {
				// line 1110: [id(1), helpstring("method PullBack")] HRESULT PullBack();
				hr = PullBack();
			}
			break;
		}
		case 2: {
			if (wFlags & DISPATCH_METHOD) {
				// line 1111: [id(2), helpstring("method Fire")] HRESULT Fire();
				hr = Fire();
			}
			break;
		}
		case 3: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1112: [propget, id(3), helpstring("property PullSpeed")] HRESULT PullSpeed([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_PullSpeed(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1113: [propput, id(3), helpstring("property PullSpeed")] HRESULT PullSpeed([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_PullSpeed(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 4: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1114: [propget, id(4), helpstring("property FireSpeed")] HRESULT FireSpeed([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_FireSpeed(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1115: [propput, id(4), helpstring("property FireSpeed")] HRESULT FireSpeed([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_FireSpeed(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 5: {
			if (wFlags & DISPATCH_METHOD) {
				// line 1116: [id(5), helpstring("method CreateBall")] HRESULT CreateBall([out, retval] IBall **Ball);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_DISPATCH;
				hr = CreateBall((IBall**)&V_DISPATCH(pVarResult));
			}
			break;
		}
		case 6: {
			if (wFlags & DISPATCH_METHOD) {
				// line 1117: [id(6), helpstring("method Position")] HRESULT Position([out, retval] float *pVal); //added for mechanical plunger position, also supports normal/digital one by now[id(7), helpstring("method PullBackandRetract")] HRESULT PullBackandRetract(); //added for manual non-mech plunger[id(216), helpstring("method MotionDevice")] HRESULT MotionDevice([out, retval] int *pVal); //added for uShockType[propget, id(DISPID_Timer_Enabled), helpstring("property TimerEnabled")] HRESULT TimerEnabled([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = Position(&V_R4(pVarResult));
			}
			break;
		}
		case DISPID_Timer_Enabled: {
			if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1121: [propput, id(DISPID_Timer_Enabled), helpstring("property TimerEnabled")] HRESULT TimerEnabled([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_TimerEnabled(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Timer_Interval: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1122: [propget, id(DISPID_Timer_Interval), helpstring("property TimerInterval")] HRESULT TimerInterval([out, retval] long *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_TimerInterval((long*)&V_I4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1123: [propput, id(DISPID_Timer_Interval), helpstring("property TimerInterval")] HRESULT TimerInterval([in] long newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_I4);
				hr = put_TimerInterval(V_I4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_X: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1124: [propget, id(DISPID_X), helpstring("property X")] HRESULT X([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_X(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1125: [propput, id(DISPID_X), helpstring("property X")] HRESULT X([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_X(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Y: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1126: [propget, id(DISPID_Y), helpstring("property Y")] HRESULT Y([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Y(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1127: [propput, id(DISPID_Y), helpstring("property Y")] HRESULT Y([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Y(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Width: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1128: [propget, id(DISPID_Width), helpstring("property Width")] HRESULT Width([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Width(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1129: [propput, id(DISPID_Width), helpstring("property Width")] HRESULT Width([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Width(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_ZAdjust: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1130: [propget, id(DISPID_ZAdjust), helpstring("property ZAdjust")] HRESULT ZAdjust([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_ZAdjust(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1131: [propput, id(DISPID_ZAdjust), helpstring("property ZAdjust")] HRESULT ZAdjust([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_ZAdjust(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Surface: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1132: [propget, id(DISPID_Surface), helpstring("property Surface")] HRESULT Surface([out, retval] BSTR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_Surface(&V_BSTR(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1133: [propput, id(DISPID_Surface), helpstring("property Surface")] HRESULT Surface([in] BSTR newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = put_Surface(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Name: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1134: [propget, id(DISPID_Name), helpstring("property Name")] HRESULT Name([out, retval] BSTR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_Name(&V_BSTR(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1135: [propput, id(DISPID_Name), helpstring("property Name")] HRESULT Name([in] BSTR newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = put_Name(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_UserValue: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1136: [propget, id(DISPID_UserValue), helpstring("property User")] HRESULT UserValue([out, retval] VARIANT* pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				hr = get_UserValue(pVarResult);
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1137: [propput, id(DISPID_UserValue), helpstring("property User")] HRESULT UserValue([in] VARIANT* newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantCopyInd(&var0, &pDispParams->rgvarg[--index]);
				hr = put_UserValue(&var0);
				VariantClear(&var0);
			}
			break;
		}
		case 390: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1138: [propget, id(390), helpstring("property Type")] HRESULT Type([out, retval] PlungerType *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_Type((PlungerType*)&V_I4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1139: [propput, id(390), helpstring("property Type")] HRESULT Type([in] PlungerType newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_I4);
				hr = put_Type((PlungerType)V_I4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 340: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1140: [propget, id(340), helpstring("property Material")] HRESULT Material([out, retval] BSTR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_Material(&V_BSTR(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1141: [propput, id(340), helpstring("property Material")] HRESULT Material([in] BSTR newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = put_Material(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Image: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1142: [propget, id(DISPID_Image), helpstring("property Image")] HRESULT Image([out, retval] BSTR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_Image(&V_BSTR(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1143: [propput, id(DISPID_Image), helpstring("property Image")] HRESULT Image([in] BSTR newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = put_Image(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_PluFrames: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1144: [propget, id(DISPID_PluFrames), helpstring("property AnimFrames")] HRESULT AnimFrames([out, retval] int *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_AnimFrames((int*)&V_I4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1145: [propput, id(DISPID_PluFrames), helpstring("property AnimFrames")] HRESULT AnimFrames([in] int newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_I4);
				hr = put_AnimFrames(V_I4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_TipShape: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1146: [propget, id(DISPID_TipShape), helpstring("property Shape")] HRESULT TipShape([out, retval] BSTR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_TipShape(&V_BSTR(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1147: [propput, id(DISPID_TipShape), helpstring("property Shape")] HRESULT TipShape([in] BSTR newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = put_TipShape(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_RodDiam: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1148: [propget, id(DISPID_RodDiam), helpstring("property RodDiam")] HRESULT RodDiam([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_RodDiam(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1149: [propput, id(DISPID_RodDiam), helpstring("property RodDiam")] HRESULT RodDiam([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_RodDiam(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_RingGap: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1150: [propget, id(DISPID_RingGap), helpstring("property RingGap")] HRESULT RingGap([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_RingGap(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1151: [propput, id(DISPID_RingGap), helpstring("property RingGap")] HRESULT RingGap([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_RingGap(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_RingDiam: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1152: [propget, id(DISPID_RingDiam), helpstring("property RingDiam")] HRESULT RingDiam([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_RingDiam(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1153: [propput, id(DISPID_RingDiam), helpstring("property RingDiam")] HRESULT RingDiam([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_RingDiam(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_RingThickness: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1154: [propget, id(DISPID_RingThickness), helpstring("property RingWidth")] HRESULT RingWidth([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_RingWidth(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1155: [propput, id(DISPID_RingThickness), helpstring("property RingWidth")] HRESULT RingWidth([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_RingWidth(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_SpringDiam: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1156: [propget, id(DISPID_SpringDiam), helpstring("property SpringDiam")] HRESULT SpringDiam([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_SpringDiam(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1157: [propput, id(DISPID_SpringDiam), helpstring("property SpringDiam")] HRESULT SpringDiam([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_SpringDiam(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_SpringGauge: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1158: [propget, id(DISPID_SpringGauge), helpstring("property SpringGauge")] HRESULT SpringGauge([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_SpringGauge(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1159: [propput, id(DISPID_SpringGauge), helpstring("property SpringGauge")] HRESULT SpringGauge([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_SpringGauge(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_SpringLoops: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1160: [propget, id(DISPID_SpringLoops), helpstring("property SpringLoops")] HRESULT SpringLoops([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_SpringLoops(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1161: [propput, id(DISPID_SpringLoops), helpstring("property SpringLoops")] HRESULT SpringLoops([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_SpringLoops(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_SpringEndLoops: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1162: [propget, id(DISPID_SpringEndLoops), helpstring("property SpringEndLoops")] HRESULT SpringEndLoops([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_SpringEndLoops(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1163: [propput, id(DISPID_SpringEndLoops), helpstring("property SpringEndLoops")] HRESULT SpringEndLoops([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_SpringEndLoops(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 110: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1165: [propget, id(110), helpstring("property MechPlunger")] HRESULT MechPlunger([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_MechPlunger(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1166: [propput, id(110), helpstring("property MechPlunger")] HRESULT MechPlunger([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_MechPlunger(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 116: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1167: [propget, id(116), helpstring("property AutoPlunger")] HRESULT AutoPlunger([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_AutoPlunger(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1168: [propput, id(116), helpstring("property AutoPlunger")] HRESULT AutoPlunger([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_AutoPlunger(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 117: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1169: [propget, id(117), helpstring("property Visible")] HRESULT Visible([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_Visible(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1170: [propput, id(117), helpstring("property Visible")] HRESULT Visible([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_Visible(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 111: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1171: [propget, id(111), helpstring("property MechStrength")] HRESULT MechStrength([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_MechStrength(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1172: [propput, id(111), helpstring("property MechStrength")] HRESULT MechStrength([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_MechStrength(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 112: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1173: [propget, id(112), helpstring("property ParkPosition")] HRESULT ParkPosition([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_ParkPosition(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1174: [propput, id(112), helpstring("property ParkPosition")] HRESULT ParkPosition([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_ParkPosition(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 113: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1175: [propget, id(113), helpstring("property Stroke")] HRESULT Stroke([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Stroke(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1176: [propput, id(113), helpstring("property Stroke")] HRESULT Stroke([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Stroke(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 114: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1177: [propget, id(114), helpstring("property ScatterVelocity")] HRESULT ScatterVelocity([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_ScatterVelocity(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1178: [propput, id(114), helpstring("property ScatterVelocity")] HRESULT ScatterVelocity([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_ScatterVelocity(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 118: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1179: [propget, id(118), helpstring("property MomentumXfer")] HRESULT MomentumXfer([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_MomentumXfer(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1180: [propput, id(118), helpstring("property MomentumXfer")] HRESULT MomentumXfer([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_MomentumXfer(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 431: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1181: [propget, id(431), helpstring("property ReflectionEnabled")] HRESULT ReflectionEnabled([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_ReflectionEnabled(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1182: [propput, id(431), helpstring("property ReflectionEnabled")] HRESULT ReflectionEnabled([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_ReflectionEnabled(V_BOOL(&var0));
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

STDMETHODIMP Plunger::GetDocumentation(INT index, BSTR *pBstrName, BSTR *pBstrDocString, DWORD *pdwHelpContext, BSTR *pBstrHelpFile) {
	if (index == MEMBERID_NIL) {
		*pBstrName = SysAllocString(L"Plunger");
		return S_OK;
	}
	return E_NOTIMPL;
}

disp_id_name_t Plunger::m_idNameMap[] = {
		{ NULL },
		{ DISPID_GameEvents_Init, L"_Init" },
		{ DISPID_TimerEvents_Timer, L"_Timer" },
		{ DISPID_LimitEvents_EOS, L"_LimitEOS" },
		{ DISPID_LimitEvents_BOS, L"_LimitBOS" }
};

HRESULT Plunger::FireDispID(const DISPID dispid, DISPPARAMS * const pdispparams) {
	static WCHAR wzName[MAXSTRING];
	CComPtr<IDispatch> disp;
	g_pplayer->m_ptable->m_pcv->m_pScript->GetScriptDispatch(nullptr, &disp);
	size_t min = 1, max = ARRAY_SIZE(m_idNameMap) - 1, i;
	int r;
	while(min <= max) {
		i = (min + max) / 2;
		if (m_idNameMap[i].dispId == dispid) {
			wcscpy(wzName, m_wzName);
			wcscat(wzName, m_idNameMap[i].name);
			LPOLESTR fnNames = (LPOLESTR)wzName;
			DISPID tDispid;
			if (SUCCEEDED(disp->GetIDsOfNames(IID_NULL, &fnNames, 1, 0, &tDispid))) {
				return disp->Invoke(tDispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, pdispparams, nullptr, nullptr, nullptr);
			}
			return DISP_E_MEMBERNOTFOUND;
		}
		else if (m_idNameMap[i].dispId < dispid)
		   min = i+1;
		else
		   max = i-1;
	}
	return DISP_E_MEMBERNOTFOUND;
}

name_disp_id_t Textbox::m_nameIDMap[] = {
		{ NULL },
		{ L"Alignment", 11 },
		{ L"BackColor", DISPID_Textbox_BackColor },
		{ L"DMD", 555 },
		{ L"Font", DISPID_Textbox_Font },
		{ L"FontColor", DISPID_Textbox_FontColor },
		{ L"Height", 60003 },
		{ L"IntensityScale", 587 },
		{ L"IsTransparent", 12 },
		{ L"Name", DISPID_Name },
		{ L"Text", 3 },
		{ L"TimerEnabled", DISPID_Timer_Enabled },
		{ L"TimerInterval", DISPID_Timer_Interval },
		{ L"UserValue", DISPID_UserValue },
		{ L"Visible", 616 },
		{ L"Width", 60002 },
		{ L"X", 60000 },
		{ L"Y", 60001 }
};

STDMETHODIMP Textbox::GetIDsOfNames(REFIID /*riid*/, LPOLESTR* rgszNames, UINT cNames, LCID lcid, DISPID* rgDispId) {
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

STDMETHODIMP Textbox::Invoke(DISPID dispIdMember, REFIID /*riid*/, LCID lcid, WORD wFlags, DISPPARAMS* pDispParams, VARIANT* pVarResult, EXCEPINFO* pExcepInfo, UINT* puArgErr) {
	HRESULT hr = DISP_E_UNKNOWNNAME;
	int index = pDispParams->cArgs;
	switch(dispIdMember) {
		case DISPID_Textbox_BackColor: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1211: [propget, id(DISPID_Textbox_BackColor), helpstring("property BackColor")] HRESULT BackColor([out, retval] OLE_COLOR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_UI4;
				hr = get_BackColor(&V_UI4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1212: [propput, id(DISPID_Textbox_BackColor), helpstring("property BackColor")] HRESULT BackColor([in] OLE_COLOR newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_UI4);
				hr = put_BackColor((OLE_COLOR)V_UI4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Textbox_FontColor: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1213: [propget, id(DISPID_Textbox_FontColor), helpstring("property FontColor")] HRESULT FontColor([out, retval] OLE_COLOR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_UI4;
				hr = get_FontColor(&V_UI4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1214: [propput, id(DISPID_Textbox_FontColor), helpstring("property FontColor")] HRESULT FontColor([in] OLE_COLOR newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_UI4);
				hr = put_FontColor((OLE_COLOR)V_UI4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 3: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1215: [propget, id(3), helpstring("property Text")] HRESULT Text([out, retval] BSTR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_Text(&V_BSTR(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1216: [propput, id(3), helpstring("property Text")] HRESULT Text([in] BSTR newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = put_Text(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Textbox_Font: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1217: [propget, id(DISPID_Textbox_Font), helpstring("property Font")] HRESULT Font([out, retval] IFontDisp **pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_DISPATCH;
				hr = get_Font((IFontDisp**)&V_DISPATCH(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUTREF) {
				// line 1218: [propputref, id(DISPID_Textbox_Font), helpstring("property Font")] HRESULT Font([in] IFontDisp *newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantCopyInd(&var0, &pDispParams->rgvarg[--index]);
				hr = putref_Font((IFontDisp*)&var0);
				VariantClear(&var0);
			}
			break;
		}
		case 60002: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1219: [propget, id(60002), helpstring("property Width")] HRESULT Width([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Width(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1220: [propput, id(60002), helpstring("property Width")] HRESULT Width([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Width(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 60003: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1221: [propget, id(60003), helpstring("property Height")] HRESULT Height([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Height(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1222: [propput, id(60003), helpstring("property Height")] HRESULT Height([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Height(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 11: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1223: [propget, id(11), helpstring("property Alignment")] HRESULT Alignment([out, retval] TextAlignment *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_Alignment((TextAlignment*)&V_I4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1224: [propput, id(11), helpstring("property Alignment")] HRESULT Alignment([in] TextAlignment newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_I4);
				hr = put_Alignment((TextAlignment)V_I4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 12: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1225: [propget, id(12), helpstring("property IsTransparent")] HRESULT IsTransparent([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_IsTransparent(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1226: [propput, id(12), helpstring("property IsTransparent")] HRESULT IsTransparent([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_IsTransparent(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 555: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1227: [propget, id(555), helpstring("property DMD")] HRESULT DMD([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_DMD(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1228: [propput, id(555), helpstring("property DMD")] HRESULT DMD([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_DMD(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Timer_Enabled: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1229: [propget, id(DISPID_Timer_Enabled), helpstring("property TimerEnabled")] HRESULT TimerEnabled([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_TimerEnabled(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1230: [propput, id(DISPID_Timer_Enabled), helpstring("property TimerEnabled")] HRESULT TimerEnabled([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_TimerEnabled(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Timer_Interval: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1231: [propget, id(DISPID_Timer_Interval), helpstring("property TimerInterval")] HRESULT TimerInterval([out, retval] long *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_TimerInterval((long*)&V_I4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1232: [propput, id(DISPID_Timer_Interval), helpstring("property TimerInterval")] HRESULT TimerInterval([in] long newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_I4);
				hr = put_TimerInterval(V_I4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 60000: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1233: [propget, id(60000), helpstring("property X")] HRESULT X([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_X(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1234: [propput, id(60000), helpstring("property X")] HRESULT X([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_X(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 60001: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1235: [propget, id(60001), helpstring("property Y")] HRESULT Y([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Y(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1236: [propput, id(60001), helpstring("property Y")] HRESULT Y([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Y(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 587: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1237: [propget, id(587), helpstring("property IntensityScale")] HRESULT IntensityScale([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_IntensityScale(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1238: [propput, id(587), helpstring("property IntensityScale")] HRESULT IntensityScale([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_IntensityScale(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Name: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1239: [propget, id(DISPID_Name), helpstring("property Name")] HRESULT Name([out, retval] BSTR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_Name(&V_BSTR(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1240: [propput, id(DISPID_Name), helpstring("property Name")] HRESULT Name([in] BSTR newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = put_Name(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_UserValue: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1241: [propget, id(DISPID_UserValue), helpstring("property User")] HRESULT UserValue([out, retval] VARIANT* pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				hr = get_UserValue(pVarResult);
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1242: [propput, id(DISPID_UserValue), helpstring("property User")] HRESULT UserValue([in] VARIANT* newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantCopyInd(&var0, &pDispParams->rgvarg[--index]);
				hr = put_UserValue(&var0);
				VariantClear(&var0);
			}
			break;
		}
		case 616: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1243: [propget, id(616), helpstring("property Visible")] HRESULT Visible([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_Visible(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1244: [propput, id(616), helpstring("property Visible")] HRESULT Visible([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_Visible(V_BOOL(&var0));
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

STDMETHODIMP Textbox::GetDocumentation(INT index, BSTR *pBstrName, BSTR *pBstrDocString, DWORD *pdwHelpContext, BSTR *pBstrHelpFile) {
	if (index == MEMBERID_NIL) {
		*pBstrName = SysAllocString(L"Textbox");
		return S_OK;
	}
	return E_NOTIMPL;
}

disp_id_name_t Textbox::m_idNameMap[] = {
		{ NULL },
		{ DISPID_GameEvents_Init, L"_Init" },
		{ DISPID_TimerEvents_Timer, L"_Timer" }
};

HRESULT Textbox::FireDispID(const DISPID dispid, DISPPARAMS * const pdispparams) {
	static WCHAR wzName[MAXSTRING];
	CComPtr<IDispatch> disp;
	g_pplayer->m_ptable->m_pcv->m_pScript->GetScriptDispatch(nullptr, &disp);
	size_t min = 1, max = ARRAY_SIZE(m_idNameMap) - 1, i;
	int r;
	while(min <= max) {
		i = (min + max) / 2;
		if (m_idNameMap[i].dispId == dispid) {
			wcscpy(wzName, m_wzName);
			wcscat(wzName, m_idNameMap[i].name);
			LPOLESTR fnNames = (LPOLESTR)wzName;
			DISPID tDispid;
			if (SUCCEEDED(disp->GetIDsOfNames(IID_NULL, &fnNames, 1, 0, &tDispid))) {
				return disp->Invoke(tDispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, pdispparams, nullptr, nullptr, nullptr);
			}
			return DISP_E_MEMBERNOTFOUND;
		}
		else if (m_idNameMap[i].dispId < dispid)
		   min = i+1;
		else
		   max = i-1;
	}
	return DISP_E_MEMBERNOTFOUND;
}

name_disp_id_t Bumper::m_nameIDMap[] = {
		{ NULL },
		{ L"BaseMaterial", 341 },
		{ L"BaseVisible", 110 },
		{ L"CapMaterial", 340 },
		{ L"CapVisible", 109 },
		{ L"Collidable", 111 },
		{ L"CurrentRingOffset", 28 },
		{ L"EnableSkirtAnimation", 822 },
		{ L"Force", 2 },
		{ L"HasHitEvent", 34 },
		{ L"HeightScale", 24 },
		{ L"Name", DISPID_Name },
		{ L"Orientation", 25 },
		{ L"PlayHit", 999 },
		{ L"Radius", 8 },
		{ L"ReflectionEnabled", 431 },
		{ L"RingDropOffset", 27 },
		{ L"RingMaterial", 734 },
		{ L"RingSpeed", 26 },
		{ L"RingVisible", 735 },
		{ L"Scatter", 115 },
		{ L"SkirtMaterial", 426 },
		{ L"SkirtVisible", 736 },
		{ L"Surface", DISPID_Surface },
		{ L"Threshold", 33 },
		{ L"TimerEnabled", DISPID_Timer_Enabled },
		{ L"TimerInterval", DISPID_Timer_Interval },
		{ L"UserValue", DISPID_UserValue },
		{ L"X", DISPID_X },
		{ L"Y", DISPID_Y }
};

STDMETHODIMP Bumper::GetIDsOfNames(REFIID /*riid*/, LPOLESTR* rgszNames, UINT cNames, LCID lcid, DISPID* rgDispId) {
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

STDMETHODIMP Bumper::Invoke(DISPID dispIdMember, REFIID /*riid*/, LCID lcid, WORD wFlags, DISPPARAMS* pDispParams, VARIANT* pVarResult, EXCEPINFO* pExcepInfo, UINT* puArgErr) {
	HRESULT hr = DISP_E_UNKNOWNNAME;
	int index = pDispParams->cArgs;
	switch(dispIdMember) {
		case 8: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1271: [propget, id(8), helpstring("property Radius")] HRESULT Radius([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Radius(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1272: [propput, id(8), helpstring("property Radius")] HRESULT Radius([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Radius(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_X: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1273: [propget, id(DISPID_X), helpstring("property X")] HRESULT X([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_X(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1274: [propput, id(DISPID_X), helpstring("property X")] HRESULT X([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_X(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Y: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1275: [propget, id(DISPID_Y), helpstring("property Y")] HRESULT Y([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Y(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1276: [propput, id(DISPID_Y), helpstring("property Y")] HRESULT Y([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Y(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 341: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1277: [propget, id(341), helpstring("property BaseMaterial")] HRESULT BaseMaterial([out, retval] BSTR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_BaseMaterial(&V_BSTR(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1278: [propput, id(341), helpstring("property BaseMaterial")] HRESULT BaseMaterial([in] BSTR newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = put_BaseMaterial(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 426: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1279: [propget, id(426), helpstring("property SkirtMaterial")] HRESULT SkirtMaterial([out, retval] BSTR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_SkirtMaterial(&V_BSTR(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1280: [propput, id(426), helpstring("property SkirtMaterial")] HRESULT SkirtMaterial([in] BSTR newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = put_SkirtMaterial(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_UserValue: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1281: [propget, id(DISPID_UserValue), helpstring("property User")] HRESULT UserValue([out, retval] VARIANT* pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				hr = get_UserValue(pVarResult);
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1282: [propput, id(DISPID_UserValue), helpstring("property User")] HRESULT UserValue([in] VARIANT* newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantCopyInd(&var0, &pDispParams->rgvarg[--index]);
				hr = put_UserValue(&var0);
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Surface: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1283: [propget, id(DISPID_Surface), helpstring("property Surface")] HRESULT Surface([out, retval] BSTR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_Surface(&V_BSTR(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1284: [propput, id(DISPID_Surface), helpstring("property Surface")] HRESULT Surface([in] BSTR newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = put_Surface(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 2: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1285: [propget, id(2), helpstring("property Force")] HRESULT Force([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Force(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1286: [propput, id(2), helpstring("property Force")] HRESULT Force([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Force(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 33: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1287: [propget, id(33), helpstring("property Threshold")] HRESULT Threshold([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Threshold(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1288: [propput, id(33), helpstring("property Threshold")] HRESULT Threshold([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Threshold(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Timer_Enabled: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1289: [propget, id(DISPID_Timer_Enabled), helpstring("property TimerEnabled")] HRESULT TimerEnabled([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_TimerEnabled(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1290: [propput, id(DISPID_Timer_Enabled), helpstring("property TimerEnabled")] HRESULT TimerEnabled([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_TimerEnabled(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Timer_Interval: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1291: [propget, id(DISPID_Timer_Interval), helpstring("property TimerInterval")] HRESULT TimerInterval([out, retval] long *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_TimerInterval((long*)&V_I4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1292: [propput, id(DISPID_Timer_Interval), helpstring("property TimerInterval")] HRESULT TimerInterval([in] long newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_I4);
				hr = put_TimerInterval(V_I4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 340: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1293: [propget, id(340), helpstring("property CapMaterial")] HRESULT CapMaterial([out, retval] BSTR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_CapMaterial(&V_BSTR(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1294: [propput, id(340), helpstring("property CapMaterial")] HRESULT CapMaterial([in] BSTR newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = put_CapMaterial(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 734: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1295: [propget, id(734), helpstring("property RingMaterial")] HRESULT RingMaterial([out, retval] BSTR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_RingMaterial(&V_BSTR(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1296: [propput, id(734), helpstring("property RingMaterial")] HRESULT RingMaterial([in] BSTR newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = put_RingMaterial(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 24: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1297: [propget, id(24), helpstring("property HeightScale")] HRESULT HeightScale([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_HeightScale(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1298: [propput, id(24), helpstring("property HeightScale")] HRESULT HeightScale([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_HeightScale(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 25: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1299: [propget, id(25), helpstring("property Orientation")] HRESULT Orientation([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Orientation(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1300: [propput, id(25), helpstring("property Orientation")] HRESULT Orientation([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Orientation(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 26: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1301: [propget, id(26), helpstring("property RingSpeed")] HRESULT RingSpeed([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_RingSpeed(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1302: [propput, id(26), helpstring("property RingSpeed")] HRESULT RingSpeed([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_RingSpeed(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 27: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1303: [propget, id(27), helpstring("property RingDropOffset")] HRESULT RingDropOffset([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_RingDropOffset(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1304: [propput, id(27), helpstring("property RingDropOffset")] HRESULT RingDropOffset([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_RingDropOffset(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 28: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1305: [propget, id(28), helpstring("property CurrentRingOffset")] HRESULT CurrentRingOffset([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_CurrentRingOffset(&V_R4(pVarResult));
			}
			break;
		}
		case DISPID_Name: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1306: [propget, id(DISPID_Name), helpstring("property Name")] HRESULT Name([out, retval] BSTR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_Name(&V_BSTR(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1307: [propput, id(DISPID_Name), helpstring("property Name")] HRESULT Name([in] BSTR newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = put_Name(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 34: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1308: [propget, id(34), helpstring("property HitEvent")] HRESULT HasHitEvent([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_HasHitEvent(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1309: [propput, id(34), helpstring("property HitEvent")] HRESULT HasHitEvent([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_HasHitEvent(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 109: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1310: [propget, id(109), helpstring("property CapVisible")] HRESULT CapVisible([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_CapVisible(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1311: [propput, id(109), helpstring("property CapVisible")] HRESULT CapVisible([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_CapVisible(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 110: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1312: [propget, id(110), helpstring("property BaseVisible")] HRESULT BaseVisible([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_BaseVisible(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1313: [propput, id(110), helpstring("property BaseVisible")] HRESULT BaseVisible([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_BaseVisible(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 735: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1314: [propget, id(735), helpstring("property RingVisible")] HRESULT RingVisible([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_RingVisible(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1315: [propput, id(735), helpstring("property RingVisible")] HRESULT RingVisible([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_RingVisible(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 736: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1316: [propget, id(736), helpstring("property SkirtVisible")] HRESULT SkirtVisible([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_SkirtVisible(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1317: [propput, id(736), helpstring("property SkirtVisible")] HRESULT SkirtVisible([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_SkirtVisible(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 111: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1318: [propget, id(111), helpstring("property Collidable")] HRESULT Collidable([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_Collidable(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1319: [propput, id(111), helpstring("property Collidable")] HRESULT Collidable([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_Collidable(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 431: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1320: [propget, id(431), helpstring("property ReflectionEnabled")] HRESULT ReflectionEnabled([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_ReflectionEnabled(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1321: [propput, id(431), helpstring("property ReflectionEnabled")] HRESULT ReflectionEnabled([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_ReflectionEnabled(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 115: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1322: [propget, id(115), helpstring("property Scatter")] HRESULT Scatter([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Scatter(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1323: [propput, id(115), helpstring("property Scatter")] HRESULT Scatter([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Scatter(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 822: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1324: [propget, id(822), helpstring("property EnableSkirtAnimation")] HRESULT EnableSkirtAnimation([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_EnableSkirtAnimation(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1325: [propput, id(822), helpstring("property EnableSkirtAnimation")] HRESULT EnableSkirtAnimation([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_EnableSkirtAnimation(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 999: {
			if (wFlags & DISPATCH_METHOD) {
				// line 1326: [id(999), helpstring("method PlayHit")] HRESULT PlayHit();
				hr = PlayHit();
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

STDMETHODIMP Bumper::GetDocumentation(INT index, BSTR *pBstrName, BSTR *pBstrDocString, DWORD *pdwHelpContext, BSTR *pBstrHelpFile) {
	if (index == MEMBERID_NIL) {
		*pBstrName = SysAllocString(L"Bumper");
		return S_OK;
	}
	return E_NOTIMPL;
}

disp_id_name_t Bumper::m_idNameMap[] = {
		{ NULL },
		{ DISPID_GameEvents_Init, L"_Init" },
		{ DISPID_TimerEvents_Timer, L"_Timer" },
		{ DISPID_HitEvents_Hit, L"_Hit" },
		{ DISPID_AnimateEvents_Animate, L"_Animate" }
};

HRESULT Bumper::FireDispID(const DISPID dispid, DISPPARAMS * const pdispparams) {
	static WCHAR wzName[MAXSTRING];
	CComPtr<IDispatch> disp;
	g_pplayer->m_ptable->m_pcv->m_pScript->GetScriptDispatch(nullptr, &disp);
	size_t min = 1, max = ARRAY_SIZE(m_idNameMap) - 1, i;
	int r;
	while(min <= max) {
		i = (min + max) / 2;
		if (m_idNameMap[i].dispId == dispid) {
			wcscpy(wzName, m_wzName);
			wcscat(wzName, m_idNameMap[i].name);
			LPOLESTR fnNames = (LPOLESTR)wzName;
			DISPID tDispid;
			if (SUCCEEDED(disp->GetIDsOfNames(IID_NULL, &fnNames, 1, 0, &tDispid))) {
				return disp->Invoke(tDispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, pdispparams, nullptr, nullptr, nullptr);
			}
			return DISP_E_MEMBERNOTFOUND;
		}
		else if (m_idNameMap[i].dispId < dispid)
		   min = i+1;
		else
		   max = i-1;
	}
	return DISP_E_MEMBERNOTFOUND;
}

name_disp_id_t Trigger::m_nameIDMap[] = {
		{ NULL },
		{ L"AnimSpeed", 26 },
		{ L"BallCntOver", 312 },
		{ L"CurrentAnimOffset", 27 },
		{ L"DestroyBall", 313 },
		{ L"Enabled", DISPID_Enabled },
		{ L"HitHeight", 314 },
		{ L"Material", 340 },
		{ L"Name", DISPID_Name },
		{ L"Radius", 346 },
		{ L"ReflectionEnabled", 431 },
		{ L"Rotation", 345 },
		{ L"Surface", DISPID_Surface },
		{ L"TimerEnabled", DISPID_Timer_Enabled },
		{ L"TimerInterval", DISPID_Timer_Interval },
		{ L"TriggerShape", DISPID_Shape },
		{ L"UserValue", DISPID_UserValue },
		{ L"Visible", 458 },
		{ L"WireThickness", 347 },
		{ L"X", DISPID_X },
		{ L"Y", DISPID_Y }
};

STDMETHODIMP Trigger::GetIDsOfNames(REFIID /*riid*/, LPOLESTR* rgszNames, UINT cNames, LCID lcid, DISPID* rgDispId) {
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

STDMETHODIMP Trigger::Invoke(DISPID dispIdMember, REFIID /*riid*/, LCID lcid, WORD wFlags, DISPPARAMS* pDispParams, VARIANT* pVarResult, EXCEPINFO* pExcepInfo, UINT* puArgErr) {
	HRESULT hr = DISP_E_UNKNOWNNAME;
	int index = pDispParams->cArgs;
	switch(dispIdMember) {
		case 346: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1355: [propget, id(346), helpstring("property Radius")] HRESULT Radius([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Radius(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1356: [propput, id(346), helpstring("property Radius")] HRESULT Radius([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Radius(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Timer_Enabled: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1357: [propget, id(DISPID_Timer_Enabled), helpstring("property TimerEnabled")] HRESULT TimerEnabled([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_TimerEnabled(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1358: [propput, id(DISPID_Timer_Enabled), helpstring("property TimerEnabled")] HRESULT TimerEnabled([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_TimerEnabled(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Timer_Interval: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1359: [propget, id(DISPID_Timer_Interval), helpstring("property TimerInterval")] HRESULT TimerInterval([out, retval] long *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_TimerInterval((long*)&V_I4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1360: [propput, id(DISPID_Timer_Interval), helpstring("property TimerInterval")] HRESULT TimerInterval([in] long newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_I4);
				hr = put_TimerInterval(V_I4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_X: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1361: [propget, id(DISPID_X), helpstring("property X")] HRESULT X([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_X(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1362: [propput, id(DISPID_X), helpstring("property X")] HRESULT X([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_X(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Y: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1363: [propget, id(DISPID_Y), helpstring("property Y")] HRESULT Y([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Y(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1364: [propput, id(DISPID_Y), helpstring("property Y")] HRESULT Y([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Y(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Enabled: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1365: [propget, id(DISPID_Enabled), helpstring("property Enabled")] HRESULT Enabled([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_Enabled(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1366: [propput, id(DISPID_Enabled), helpstring("property Enabled")] HRESULT Enabled([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_Enabled(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 458: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1367: [propget, id(458), helpstring("property Visible")] HRESULT Visible([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_Visible(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1368: [propput, id(458), helpstring("property Visible")] HRESULT Visible([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_Visible(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Shape: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1369: [propget, id(DISPID_Shape), helpstring("property TriggerShape")] HRESULT TriggerShape([out, retval] TriggerShape *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_TriggerShape((TriggerShape*)&V_I4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1370: [propput, id(DISPID_Shape), helpstring("property TriggerShape")] HRESULT TriggerShape([in] TriggerShape newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_I4);
				hr = put_TriggerShape((TriggerShape)V_I4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Surface: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1371: [propget, id(DISPID_Surface), helpstring("property Surface")] HRESULT Surface([out, retval] BSTR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_Surface(&V_BSTR(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1372: [propput, id(DISPID_Surface), helpstring("property Surface")] HRESULT Surface([in] BSTR newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = put_Surface(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Name: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1373: [propget, id(DISPID_Name), helpstring("property Name")] HRESULT Name([out, retval] BSTR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_Name(&V_BSTR(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1374: [propput, id(DISPID_Name), helpstring("property Name")] HRESULT Name([in] BSTR newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = put_Name(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_UserValue: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1375: [propget, id(DISPID_UserValue), helpstring("property User")] HRESULT UserValue([out, retval] VARIANT* pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				hr = get_UserValue(pVarResult);
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1376: [propput, id(DISPID_UserValue), helpstring("property User")] HRESULT UserValue([in] VARIANT* newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantCopyInd(&var0, &pDispParams->rgvarg[--index]);
				hr = put_UserValue(&var0);
				VariantClear(&var0);
			}
			break;
		}
		case 312: {
			if (wFlags & DISPATCH_METHOD) {
				// line 1377: [id(312), helpstring("method BallCntOver")] HRESULT BallCntOver([out, retval] int *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = BallCntOver((int*)&V_I4(pVarResult));
			}
			break;
		}
		case 313: {
			if (wFlags & DISPATCH_METHOD) {
				// line 1378: [id(313), helpstring("method DestroyBall")] HRESULT DestroyBall([out, retval] int *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = DestroyBall((int*)&V_I4(pVarResult));
			}
			break;
		}
		case 314: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1379: [propget, id(314), helpstring("property HitHeight")] HRESULT HitHeight([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_HitHeight(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1380: [propput, id(314), helpstring("property HitHeight")] HRESULT HitHeight([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_HitHeight(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 340: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1381: [propget, id(340), helpstring("property Material")] HRESULT Material([out, retval] BSTR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_Material(&V_BSTR(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1382: [propput, id(340), helpstring("property Material")] HRESULT Material([in] BSTR newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = put_Material(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 345: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1383: [propget, id(345), helpstring("property Rotation")] HRESULT Rotation([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Rotation(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1384: [propput, id(345), helpstring("property Rotation")] HRESULT Rotation([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Rotation(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 347: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1385: [propget, id(347), helpstring("property WireThickness")] HRESULT WireThickness([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_WireThickness(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1386: [propput, id(347), helpstring("property WireThickness")] HRESULT WireThickness([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_WireThickness(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 26: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1387: [propget, id(26), helpstring("property AnimSpeed")] HRESULT AnimSpeed([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_AnimSpeed(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1388: [propput, id(26), helpstring("property AnimSpeed")] HRESULT AnimSpeed([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_AnimSpeed(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 27: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1389: [propget, id(27), helpstring("property CurrentAnimOffset")] HRESULT CurrentAnimOffset([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_CurrentAnimOffset(&V_R4(pVarResult));
			}
			break;
		}
		case 431: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1390: [propget, id(431), helpstring("property ReflectionEnabled")] HRESULT ReflectionEnabled([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_ReflectionEnabled(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1391: [propput, id(431), helpstring("property ReflectionEnabled")] HRESULT ReflectionEnabled([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_ReflectionEnabled(V_BOOL(&var0));
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

STDMETHODIMP Trigger::GetDocumentation(INT index, BSTR *pBstrName, BSTR *pBstrDocString, DWORD *pdwHelpContext, BSTR *pBstrHelpFile) {
	if (index == MEMBERID_NIL) {
		*pBstrName = SysAllocString(L"Trigger");
		return S_OK;
	}
	return E_NOTIMPL;
}

disp_id_name_t Trigger::m_idNameMap[] = {
		{ NULL },
		{ DISPID_GameEvents_Init, L"_Init" },
		{ DISPID_TimerEvents_Timer, L"_Timer" },
		{ DISPID_HitEvents_Hit, L"_Hit" },
		{ DISPID_HitEvents_Unhit, L"_Unhit" },
		{ DISPID_AnimateEvents_Animate, L"_Animate" }
};

HRESULT Trigger::FireDispID(const DISPID dispid, DISPPARAMS * const pdispparams) {
	static WCHAR wzName[MAXSTRING];
	CComPtr<IDispatch> disp;
	g_pplayer->m_ptable->m_pcv->m_pScript->GetScriptDispatch(nullptr, &disp);
	size_t min = 1, max = ARRAY_SIZE(m_idNameMap) - 1, i;
	int r;
	while(min <= max) {
		i = (min + max) / 2;
		if (m_idNameMap[i].dispId == dispid) {
			wcscpy(wzName, m_wzName);
			wcscat(wzName, m_idNameMap[i].name);
			LPOLESTR fnNames = (LPOLESTR)wzName;
			DISPID tDispid;
			if (SUCCEEDED(disp->GetIDsOfNames(IID_NULL, &fnNames, 1, 0, &tDispid))) {
				return disp->Invoke(tDispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, pdispparams, nullptr, nullptr, nullptr);
			}
			return DISP_E_MEMBERNOTFOUND;
		}
		else if (m_idNameMap[i].dispId < dispid)
		   min = i+1;
		else
		   max = i-1;
	}
	return DISP_E_MEMBERNOTFOUND;
}

name_disp_id_t Light::m_nameIDMap[] = {
		{ NULL },
		{ L"BlinkInterval", DISPID_Light_BlinkInterval },
		{ L"BlinkPattern", 9 },
		{ L"Bulb", 340 },
		{ L"BulbHaloHeight", 429 },
		{ L"BulbModulateVsAdd", 431 },
		{ L"Color", 3 },
		{ L"ColorFull", 457 },
		{ L"DepthBias", 397 },
		{ L"Duration", 38 },
		{ L"Fader", 458 },
		{ L"FadeSpeedDown", 437 },
		{ L"FadeSpeedUp", 377 },
		{ L"Falloff", 1 },
		{ L"FalloffPower", 432 },
		{ L"FilamentTemperature", 459 },
		{ L"GetInPlayIntensity", 597 },
		{ L"GetInPlayState", 595 },
		{ L"GetInPlayStateBool", 596 },
		{ L"Image", DISPID_Image },
		{ L"ImageMode", 453 },
		{ L"Intensity", 12 },
		{ L"IntensityScale", 434 },
		{ L"Name", DISPID_Name },
		{ L"ScaleBulbMesh", 425 },
		{ L"Shadows", 456 },
		{ L"ShowBulbMesh", 394 },
		{ L"ShowReflectionOnBall", 455 },
		{ L"State", DISPID_Light_State },
		{ L"StaticBulbMesh", 727 },
		{ L"Surface", DISPID_Surface },
		{ L"TimerEnabled", DISPID_Timer_Enabled },
		{ L"TimerInterval", DISPID_Timer_Interval },
		{ L"TransmissionScale", 617 },
		{ L"UserValue", DISPID_UserValue },
		{ L"Visible", 615 },
		{ L"X", DISPID_X },
		{ L"Y", DISPID_Y }
};

STDMETHODIMP Light::GetIDsOfNames(REFIID /*riid*/, LPOLESTR* rgszNames, UINT cNames, LCID lcid, DISPID* rgDispId) {
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

STDMETHODIMP Light::Invoke(DISPID dispIdMember, REFIID /*riid*/, LCID lcid, WORD wFlags, DISPPARAMS* pDispParams, VARIANT* pVarResult, EXCEPINFO* pExcepInfo, UINT* puArgErr) {
	HRESULT hr = DISP_E_UNKNOWNNAME;
	int index = pDispParams->cArgs;
	switch(dispIdMember) {
		case 1: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1421: [propget, id(1), helpstring("property Falloff")] HRESULT Falloff([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Falloff(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1422: [propput, id(1), helpstring("property Falloff")] HRESULT Falloff([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Falloff(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 432: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1423: [propget, id(432), helpstring("property FalloffPower")] HRESULT FalloffPower([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_FalloffPower(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1424: [propput, id(432), helpstring("property FalloffPower")] HRESULT FalloffPower([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_FalloffPower(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Light_State: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1425: [propget, id(DISPID_Light_State), helpstring("property State")] HRESULT State([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_State(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1426: [propput, id(DISPID_Light_State), helpstring("property State")] HRESULT State([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_State(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 595: {
			if (wFlags & DISPATCH_METHOD) {
				// line 1427: [id(595), helpstring("method GetInPlayState")] HRESULT GetInPlayState([out, retval] float* pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = GetInPlayState(&V_R4(pVarResult));
			}
			break;
		}
		case 596: {
			if (wFlags & DISPATCH_METHOD) {
				// line 1428: [id(596), helpstring("method GetInPlayStateBool")] HRESULT GetInPlayStateBool([out, retval] VARIANT_BOOL* pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = GetInPlayStateBool(&V_BOOL(pVarResult));
			}
			break;
		}
		case 597: {
			if (wFlags & DISPATCH_METHOD) {
				// line 1429: [id(597), helpstring("method GetInPlayIntensity")] HRESULT GetInPlayIntensity([out, retval] float* pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = GetInPlayIntensity(&V_R4(pVarResult));
			}
			break;
		}
		case 3: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1430: [propget, id(3), helpstring("property Color")] HRESULT Color([out, retval] OLE_COLOR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_UI4;
				hr = get_Color(&V_UI4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1431: [propput, id(3), helpstring("property Color")] HRESULT Color([in] OLE_COLOR newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_UI4);
				hr = put_Color((OLE_COLOR)V_UI4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 457: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1432: [propget, id(457), helpstring("property ColorFull")] HRESULT ColorFull([out, retval] OLE_COLOR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_UI4;
				hr = get_ColorFull(&V_UI4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1433: [propput, id(457), helpstring("property ColorFull")] HRESULT ColorFull([in] OLE_COLOR newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_UI4);
				hr = put_ColorFull((OLE_COLOR)V_UI4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Timer_Enabled: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1434: [propget, id(DISPID_Timer_Enabled), helpstring("property TimerEnabled")] HRESULT TimerEnabled([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_TimerEnabled(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1435: [propput, id(DISPID_Timer_Enabled), helpstring("property TimerEnabled")] HRESULT TimerEnabled([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_TimerEnabled(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Timer_Interval: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1436: [propget, id(DISPID_Timer_Interval), helpstring("property TimerInterval")] HRESULT TimerInterval([out, retval] long *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_TimerInterval((long*)&V_I4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1437: [propput, id(DISPID_Timer_Interval), helpstring("property TimerInterval")] HRESULT TimerInterval([in] long newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_I4);
				hr = put_TimerInterval(V_I4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_X: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1438: [propget, id(DISPID_X), helpstring("property X")] HRESULT X([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_X(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1439: [propput, id(DISPID_X), helpstring("property X")] HRESULT X([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_X(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Y: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1440: [propget, id(DISPID_Y), helpstring("property Y")] HRESULT Y([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Y(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1441: [propput, id(DISPID_Y), helpstring("property Y")] HRESULT Y([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Y(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 9: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1442: [propget, id(9), helpstring("property BlinkPattern")] HRESULT BlinkPattern([out, retval] BSTR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_BlinkPattern(&V_BSTR(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1443: [propput, id(9), helpstring("property BlinkPattern")] HRESULT BlinkPattern([in] BSTR newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = put_BlinkPattern(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Light_BlinkInterval: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1444: [propget, id(DISPID_Light_BlinkInterval), helpstring("property BlinkInterval")] HRESULT BlinkInterval([out, retval] long *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_BlinkInterval((long*)&V_I4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1445: [propput, id(DISPID_Light_BlinkInterval), helpstring("property BlinkInterval")] HRESULT BlinkInterval([in] long newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_I4);
				hr = put_BlinkInterval(V_I4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 38: {
			if (wFlags & DISPATCH_METHOD) {
				// line 1446: [id(38), helpstring("method Duration")] HRESULT Duration([in] float startState,[in] long newVal, [in] float endState);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				VARIANT var1;
				V_VT(&var1) = VT_EMPTY;
				VariantChangeType(&var1, &pDispParams->rgvarg[--index], 0, VT_I4);
				VARIANT var2;
				V_VT(&var2) = VT_EMPTY;
				VariantChangeType(&var2, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = Duration(V_R4(&var0), V_I4(&var1), V_R4(&var2));
				VariantClear(&var0);
				VariantClear(&var1);
				VariantClear(&var2);
			}
			break;
		}
		case 12: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1447: [propget, id(12), helpstring("property Intensity")] HRESULT Intensity([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Intensity(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1448: [propput, id(12), helpstring("property Intensity")] HRESULT Intensity([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Intensity(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 617: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1449: [propget, id(617), helpstring("property TransmissionScale")] HRESULT TransmissionScale([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_TransmissionScale(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1450: [propput, id(617), helpstring("property TransmissionScale")] HRESULT TransmissionScale([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_TransmissionScale(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 434: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1451: [propget, id(434), helpstring("property IntensityScale")] HRESULT IntensityScale([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_IntensityScale(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1452: [propput, id(434), helpstring("property IntensityScale")] HRESULT IntensityScale([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_IntensityScale(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Surface: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1453: [propget, id(DISPID_Surface), helpstring("property Surface")] HRESULT Surface([out, retval] BSTR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_Surface(&V_BSTR(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1454: [propput, id(DISPID_Surface), helpstring("property Surface")] HRESULT Surface([in] BSTR newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = put_Surface(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Name: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1455: [propget, id(DISPID_Name), helpstring("property Name")] HRESULT Name([out, retval] BSTR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_Name(&V_BSTR(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1456: [propput, id(DISPID_Name), helpstring("property Name")] HRESULT Name([in] BSTR newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = put_Name(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_UserValue: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1457: [propget, id(DISPID_UserValue), helpstring("property User")] HRESULT UserValue([out, retval] VARIANT* pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				hr = get_UserValue(pVarResult);
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1458: [propput, id(DISPID_UserValue), helpstring("property User")] HRESULT UserValue([in] VARIANT* newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantCopyInd(&var0, &pDispParams->rgvarg[--index]);
				hr = put_UserValue(&var0);
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Image: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1459: [propget, id(DISPID_Image), helpstring("property Image")] HRESULT Image([out, retval] BSTR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_Image(&V_BSTR(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1460: [propput, id(DISPID_Image), helpstring("property Image")] HRESULT Image([in] BSTR newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = put_Image(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 453: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1461: [propget, id(453), helpstring("property ImageMode")] HRESULT ImageMode([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_ImageMode(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1462: [propput, id(453), helpstring("property ImageMode")] HRESULT ImageMode([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_ImageMode(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 397: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1463: [propget, id(397), helpstring("property DepthBias")] HRESULT DepthBias([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_DepthBias(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1464: [propput, id(397), helpstring("property DepthBias")] HRESULT DepthBias([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_DepthBias(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 377: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1465: [propget, id(377), helpstring("property FadeSpeedUp")] HRESULT FadeSpeedUp([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_FadeSpeedUp(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1466: [propput, id(377), helpstring("property FadeSpeedUp")] HRESULT FadeSpeedUp([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_FadeSpeedUp(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 437: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1467: [propget, id(437), helpstring("property FadeSpeedDown")] HRESULT FadeSpeedDown([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_FadeSpeedDown(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1468: [propput, id(437), helpstring("property FadeSpeedDown")] HRESULT FadeSpeedDown([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_FadeSpeedDown(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 340: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1469: [propget, id(340), helpstring("property Bulb")] HRESULT Bulb([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_Bulb(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1470: [propput, id(340), helpstring("property Bulb")] HRESULT Bulb([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_Bulb(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 394: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1471: [propget, id(394), helpstring("property ShowBulbMesh")] HRESULT ShowBulbMesh([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_ShowBulbMesh(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1472: [propput, id(394), helpstring("property ShowBulbMesh")] HRESULT ShowBulbMesh([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_ShowBulbMesh(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 727: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1473: [propget, id(727), helpstring("property StaticBulbMesh")] HRESULT StaticBulbMesh([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_StaticBulbMesh(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1474: [propput, id(727), helpstring("property StaticBulbMesh")] HRESULT StaticBulbMesh([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_StaticBulbMesh(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 455: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1475: [propget, id(455), helpstring("property ShowReflectionOnBall")] HRESULT ShowReflectionOnBall([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_ShowReflectionOnBall(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1476: [propput, id(455), helpstring("property ShowReflectionOnBall")] HRESULT ShowReflectionOnBall([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_ShowReflectionOnBall(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 425: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1477: [propget, id(425), helpstring("property ScaleBulbMesh")] HRESULT ScaleBulbMesh([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_ScaleBulbMesh(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1478: [propput, id(425), helpstring("property ScaleBulbMesh")] HRESULT ScaleBulbMesh([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_ScaleBulbMesh(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 431: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1479: [propget, id(431), helpstring("property BulbModulateVsAdd")] HRESULT BulbModulateVsAdd([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_BulbModulateVsAdd(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1480: [propput, id(431), helpstring("property BulbModulateVsAdd")] HRESULT BulbModulateVsAdd([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_BulbModulateVsAdd(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 429: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1481: [propget, id(429), helpstring("property BulbHaloHeight")] HRESULT BulbHaloHeight([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_BulbHaloHeight(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1482: [propput, id(429), helpstring("property BulbHaloHeight")] HRESULT BulbHaloHeight([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_BulbHaloHeight(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 615: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1483: [propget, id(615), helpstring("property Visible")] HRESULT Visible([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_Visible(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1484: [propput, id(615), helpstring("property Visible")] HRESULT Visible([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_Visible(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 456: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1485: [propget, id(456), helpstring("property Shadows")] HRESULT Shadows([out, retval] long *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_Shadows((long*)&V_I4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1486: [propput, id(456), helpstring("property Shadows")] HRESULT Shadows([in] long newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_I4);
				hr = put_Shadows(V_I4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 458: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1487: [propget, id(458), helpstring("property Fader")] HRESULT Fader([out, retval] long *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_Fader((long*)&V_I4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1488: [propput, id(458), helpstring("property Fader")] HRESULT Fader([in] long newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_I4);
				hr = put_Fader(V_I4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 459: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1489: [propget, id(459), helpstring("property FilamentTemperature")] HRESULT FilamentTemperature([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_FilamentTemperature(&V_R4(pVarResult));
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

STDMETHODIMP Light::GetDocumentation(INT index, BSTR *pBstrName, BSTR *pBstrDocString, DWORD *pdwHelpContext, BSTR *pBstrHelpFile) {
	if (index == MEMBERID_NIL) {
		*pBstrName = SysAllocString(L"Light");
		return S_OK;
	}
	return E_NOTIMPL;
}

disp_id_name_t Light::m_idNameMap[] = {
		{ NULL },
		{ DISPID_GameEvents_Init, L"_Init" },
		{ DISPID_TimerEvents_Timer, L"_Timer" },
		{ DISPID_AnimateEvents_Animate, L"_Animate" }
};

HRESULT Light::FireDispID(const DISPID dispid, DISPPARAMS * const pdispparams) {
	static WCHAR wzName[MAXSTRING];
	CComPtr<IDispatch> disp;
	g_pplayer->m_ptable->m_pcv->m_pScript->GetScriptDispatch(nullptr, &disp);
	size_t min = 1, max = ARRAY_SIZE(m_idNameMap) - 1, i;
	int r;
	while(min <= max) {
		i = (min + max) / 2;
		if (m_idNameMap[i].dispId == dispid) {
			wcscpy(wzName, m_wzName);
			wcscat(wzName, m_idNameMap[i].name);
			LPOLESTR fnNames = (LPOLESTR)wzName;
			DISPID tDispid;
			if (SUCCEEDED(disp->GetIDsOfNames(IID_NULL, &fnNames, 1, 0, &tDispid))) {
				return disp->Invoke(tDispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, pdispparams, nullptr, nullptr, nullptr);
			}
			return DISP_E_MEMBERNOTFOUND;
		}
		else if (m_idNameMap[i].dispId < dispid)
		   min = i+1;
		else
		   max = i-1;
	}
	return DISP_E_MEMBERNOTFOUND;
}

name_disp_id_t Kicker::m_nameIDMap[] = {
		{ NULL },
		{ L"BallCntOver", 312 },
		{ L"CreateBall", 1 },
		{ L"CreateSizedBall", 11 },
		{ L"CreateSizedBallWithMass", 444 },
		{ L"DestroyBall", 2 },
		{ L"DrawStyle", 9 },
		{ L"Enabled", DISPID_Enabled },
		{ L"FallThrough", 394 },
		{ L"HitAccuracy", 314 },
		{ L"HitHeight", 315 },
		{ L"Kick", 5 },
		{ L"KickXYZ", 310 },
		{ L"KickZ", 311 },
		{ L"LastCapturedBall", 19 },
		{ L"Legacy", 431 },
		{ L"Material", 340 },
		{ L"Name", DISPID_Name },
		{ L"Orientation", 107 },
		{ L"Radius", 111 },
		{ L"Scatter", 115 },
		{ L"Surface", DISPID_Surface },
		{ L"TimerEnabled", DISPID_Timer_Enabled },
		{ L"TimerInterval", DISPID_Timer_Interval },
		{ L"UserValue", DISPID_UserValue },
		{ L"X", DISPID_X },
		{ L"Y", DISPID_Y }
};

STDMETHODIMP Kicker::GetIDsOfNames(REFIID /*riid*/, LPOLESTR* rgszNames, UINT cNames, LCID lcid, DISPID* rgDispId) {
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

STDMETHODIMP Kicker::Invoke(DISPID dispIdMember, REFIID /*riid*/, LCID lcid, WORD wFlags, DISPPARAMS* pDispParams, VARIANT* pVarResult, EXCEPINFO* pExcepInfo, UINT* puArgErr) {
	HRESULT hr = DISP_E_UNKNOWNNAME;
	int index = pDispParams->cArgs;
	switch(dispIdMember) {
		case DISPID_Name: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1518: [propget, id(DISPID_Name), helpstring("property Name")] HRESULT Name([out, retval] BSTR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_Name(&V_BSTR(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1519: [propput, id(DISPID_Name), helpstring("property Name")] HRESULT Name([in] BSTR newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = put_Name(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 1: {
			if (wFlags & DISPATCH_METHOD) {
				// line 1520: [id(1), helpstring("method CreateBall")] HRESULT CreateBall([out, retval] IBall **Ball);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_DISPATCH;
				hr = CreateBall((IBall**)&V_DISPATCH(pVarResult));
			}
			break;
		}
		case 11: {
			if (wFlags & DISPATCH_METHOD) {
				// line 1521: [id(11), helpstring("method CreateSizedBall")] HRESULT CreateSizedBall([in] float radius, [out, retval] IBall **Ball);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				V_VT(pVarResult) = VT_DISPATCH;
				hr = CreateSizedBall(V_R4(&var0), (IBall**)&V_DISPATCH(pVarResult));
				VariantClear(&var0);
			}
			break;
		}
		case 444: {
			if (wFlags & DISPATCH_METHOD) {
				// line 1522: [id(444), helpstring("method CreateSizedBallWithMass")] HRESULT CreateSizedBallWithMass([in] float radius, [in] float mass, [out, retval] IBall **Ball);
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
				V_VT(pVarResult) = VT_DISPATCH;
				hr = CreateSizedBallWithMass(V_R4(&var0), V_R4(&var1), (IBall**)&V_DISPATCH(pVarResult));
				VariantClear(&var0);
				VariantClear(&var1);
			}
			break;
		}
		case 2: {
			if (wFlags & DISPATCH_METHOD) {
				// line 1523: [id(2), helpstring("method DestroyBall")] HRESULT DestroyBall([out, retval] int *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = DestroyBall((int*)&V_I4(pVarResult));
			}
			break;
		}
		case DISPID_Timer_Enabled: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1524: [propget, id(DISPID_Timer_Enabled), helpstring("property TimerEnabled")] HRESULT TimerEnabled([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_TimerEnabled(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1525: [propput, id(DISPID_Timer_Enabled), helpstring("property TimerEnabled")] HRESULT TimerEnabled([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_TimerEnabled(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Timer_Interval: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1526: [propget, id(DISPID_Timer_Interval), helpstring("property TimerInterval")] HRESULT TimerInterval([out, retval] long *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_TimerInterval((long*)&V_I4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1527: [propput, id(DISPID_Timer_Interval), helpstring("property TimerInterval")] HRESULT TimerInterval([in] long newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_I4);
				hr = put_TimerInterval(V_I4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 5: {
			if (wFlags & DISPATCH_METHOD) {
				// line 1528: [id(5), helpstring("method Kick")] HRESULT Kick(float angle, float speed, [defaultvalue(0)] float inclination);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				VARIANT var1;
				V_VT(&var1) = VT_EMPTY;
				VariantChangeType(&var1, &pDispParams->rgvarg[--index], 0, VT_R4);
				VARIANT var2;
				V_VT(&var2) = VT_R4;
				V_R4(&var2) = 0;
				VariantChangeType(&var2, (index > 0) ? &pDispParams->rgvarg[--index] : &var2, 0, VT_R4);
				hr = Kick(V_R4(&var0), V_R4(&var1), V_R4(&var2));
				VariantClear(&var0);
				VariantClear(&var1);
				VariantClear(&var2);
			}
			break;
		}
		case 310: {
			if (wFlags & DISPATCH_METHOD) {
				// line 1529: [id(310), helpstring("method KickXYZ")] HRESULT KickXYZ(float angle, float speed, float inclination, float x, float y, float z);
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
				VARIANT var4;
				V_VT(&var4) = VT_EMPTY;
				VariantChangeType(&var4, &pDispParams->rgvarg[--index], 0, VT_R4);
				VARIANT var5;
				V_VT(&var5) = VT_EMPTY;
				VariantChangeType(&var5, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = KickXYZ(V_R4(&var0), V_R4(&var1), V_R4(&var2), V_R4(&var3), V_R4(&var4), V_R4(&var5));
				VariantClear(&var0);
				VariantClear(&var1);
				VariantClear(&var2);
				VariantClear(&var3);
				VariantClear(&var4);
				VariantClear(&var5);
			}
			break;
		}
		case 311: {
			if (wFlags & DISPATCH_METHOD) {
				// line 1530: [id(311), helpstring("method KickZ")] HRESULT KickZ(float angle, float speed, float inclination, float heightz);
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
				hr = KickZ(V_R4(&var0), V_R4(&var1), V_R4(&var2), V_R4(&var3));
				VariantClear(&var0);
				VariantClear(&var1);
				VariantClear(&var2);
				VariantClear(&var3);
			}
			break;
		}
		case DISPID_X: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1531: [propget, id(DISPID_X), helpstring("property X")] HRESULT X([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_X(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1532: [propput, id(DISPID_X), helpstring("property X")] HRESULT X([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_X(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Y: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1533: [propget, id(DISPID_Y), helpstring("property Y")] HRESULT Y([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Y(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1534: [propput, id(DISPID_Y), helpstring("property Y")] HRESULT Y([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Y(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Enabled: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1535: [propget, id(DISPID_Enabled), helpstring("property Enabled")] HRESULT Enabled([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_Enabled(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1536: [propput, id(DISPID_Enabled), helpstring("property Enabled")] HRESULT Enabled([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_Enabled(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 9: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1537: [propget, id(9), helpstring("property DrawStyle")] HRESULT DrawStyle([out, retval] KickerType *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_DrawStyle((KickerType*)&V_I4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1538: [propput, id(9), helpstring("property DrawStyle")] HRESULT DrawStyle([in] KickerType newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_I4);
				hr = put_DrawStyle((KickerType)V_I4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 340: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1539: [propget, id(340), helpstring("property Material")] HRESULT Material([out, retval] BSTR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_Material(&V_BSTR(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1540: [propput, id(340), helpstring("property Material")] HRESULT Material([in] BSTR newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = put_Material(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Surface: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1541: [propget, id(DISPID_Surface), helpstring("property Surface")] HRESULT Surface([out, retval] BSTR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_Surface(&V_BSTR(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1542: [propput, id(DISPID_Surface), helpstring("property Surface")] HRESULT Surface([in] BSTR newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = put_Surface(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_UserValue: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1543: [propget, id(DISPID_UserValue), helpstring("property User")] HRESULT UserValue([out, retval] VARIANT* pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				hr = get_UserValue(pVarResult);
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1544: [propput, id(DISPID_UserValue), helpstring("property User")] HRESULT UserValue([in] VARIANT* newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantCopyInd(&var0, &pDispParams->rgvarg[--index]);
				hr = put_UserValue(&var0);
				VariantClear(&var0);
			}
			break;
		}
		case 312: {
			if (wFlags & DISPATCH_METHOD) {
				// line 1545: [id(312), helpstring("method BallCntOver")] HRESULT BallCntOver([out, retval] int *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = BallCntOver((int*)&V_I4(pVarResult));
			}
			break;
		}
		case 115: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1546: [propget, id(115), helpstring("property Scatter")] HRESULT Scatter([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Scatter(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1547: [propput, id(115), helpstring("property Scatter")] HRESULT Scatter([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Scatter(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 314: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1548: [propget, id(314), helpstring("property HitAccuracy")] HRESULT HitAccuracy([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_HitAccuracy(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1549: [propput, id(314), helpstring("property HitAccuracy")] HRESULT HitAccuracy([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_HitAccuracy(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 315: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1550: [propget, id(315), helpstring("property HitHeight")] HRESULT HitHeight([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_HitHeight(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1551: [propput, id(315), helpstring("property HitHeight")] HRESULT HitHeight([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_HitHeight(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 107: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1552: [propget, id(107), helpstring("property Orientation")] HRESULT Orientation([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Orientation(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1553: [propput, id(107), helpstring("property Orientation")] HRESULT Orientation([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Orientation(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 111: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1554: [propget, id(111), helpstring("property Radius")] HRESULT Radius([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Radius(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1555: [propput, id(111), helpstring("property Radius")] HRESULT Radius([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Radius(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 394: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1556: [propget, id(394), helpstring("property FallThrough")] HRESULT FallThrough([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_FallThrough(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1557: [propput, id(394), helpstring("property FallThrough")] HRESULT FallThrough([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_FallThrough(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 431: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1558: [propget, id(431), helpstring("property Legacy")] HRESULT Legacy([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_Legacy(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1559: [propput, id(431), helpstring("property Legacy")] HRESULT Legacy([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_Legacy(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 19: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1560: [propget, id(19), helpstring("property LastCapturedBall")] HRESULT LastCapturedBall([out, retval] IBall **pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_DISPATCH;
				hr = get_LastCapturedBall((IBall**)&V_DISPATCH(pVarResult));
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

STDMETHODIMP Kicker::GetDocumentation(INT index, BSTR *pBstrName, BSTR *pBstrDocString, DWORD *pdwHelpContext, BSTR *pBstrHelpFile) {
	if (index == MEMBERID_NIL) {
		*pBstrName = SysAllocString(L"Kicker");
		return S_OK;
	}
	return E_NOTIMPL;
}

disp_id_name_t Kicker::m_idNameMap[] = {
		{ NULL },
		{ DISPID_GameEvents_Init, L"_Init" },
		{ DISPID_TimerEvents_Timer, L"_Timer" },
		{ DISPID_HitEvents_Hit, L"_Hit" },
		{ DISPID_HitEvents_Unhit, L"_Unhit" }
};

HRESULT Kicker::FireDispID(const DISPID dispid, DISPPARAMS * const pdispparams) {
	static WCHAR wzName[MAXSTRING];
	CComPtr<IDispatch> disp;
	g_pplayer->m_ptable->m_pcv->m_pScript->GetScriptDispatch(nullptr, &disp);
	size_t min = 1, max = ARRAY_SIZE(m_idNameMap) - 1, i;
	int r;
	while(min <= max) {
		i = (min + max) / 2;
		if (m_idNameMap[i].dispId == dispid) {
			wcscpy(wzName, m_wzName);
			wcscat(wzName, m_idNameMap[i].name);
			LPOLESTR fnNames = (LPOLESTR)wzName;
			DISPID tDispid;
			if (SUCCEEDED(disp->GetIDsOfNames(IID_NULL, &fnNames, 1, 0, &tDispid))) {
				return disp->Invoke(tDispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, pdispparams, nullptr, nullptr, nullptr);
			}
			return DISP_E_MEMBERNOTFOUND;
		}
		else if (m_idNameMap[i].dispId < dispid)
		   min = i+1;
		else
		   max = i-1;
	}
	return DISP_E_MEMBERNOTFOUND;
}

name_disp_id_t Decal::m_nameIDMap[] = {
		{ NULL },
		{ L"Font", DISPID_Decal_Font },
		{ L"FontColor", 11 },
		{ L"HasVerticalText", 13 },
		{ L"Height", 4 },
		{ L"Image", DISPID_Image },
		{ L"Material", 340 },
		{ L"Rotation", 1 },
		{ L"SizingType", DISPID_Decal_SizingType },
		{ L"Surface", DISPID_Surface },
		{ L"Text", 8 },
		{ L"Type", 7 },
		{ L"Width", 3 },
		{ L"X", 5 },
		{ L"Y", 6 }
};

STDMETHODIMP Decal::GetIDsOfNames(REFIID /*riid*/, LPOLESTR* rgszNames, UINT cNames, LCID lcid, DISPID* rgDispId) {
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

STDMETHODIMP Decal::Invoke(DISPID dispIdMember, REFIID /*riid*/, LCID lcid, WORD wFlags, DISPPARAMS* pDispParams, VARIANT* pVarResult, EXCEPINFO* pExcepInfo, UINT* puArgErr) {
	HRESULT hr = DISP_E_UNKNOWNNAME;
	int index = pDispParams->cArgs;
	switch(dispIdMember) {
		case 1: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1592: [propget, id(1), helpstring("property Rotation")] HRESULT Rotation([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Rotation(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1593: [propput, id(1), helpstring("property Rotation")] HRESULT Rotation([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Rotation(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Image: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1594: [propget, id(DISPID_Image), helpstring("property Image")] HRESULT Image([out, retval] BSTR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_Image(&V_BSTR(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1595: [propput, id(DISPID_Image), helpstring("property Image")] HRESULT Image([in] BSTR newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = put_Image(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 3: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1596: [propget, id(3), helpstring("property Width")] HRESULT Width([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Width(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1597: [propput, id(3), helpstring("property Width")] HRESULT Width([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Width(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 4: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1598: [propget, id(4), helpstring("property Height")] HRESULT Height([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Height(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1599: [propput, id(4), helpstring("property Height")] HRESULT Height([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Height(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 5: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1600: [propget, id(5), helpstring("property X")] HRESULT X([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_X(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1601: [propput, id(5), helpstring("property X")] HRESULT X([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_X(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 6: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1602: [propget, id(6), helpstring("property Y")] HRESULT Y([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Y(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1603: [propput, id(6), helpstring("property Y")] HRESULT Y([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Y(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 7: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1604: [propget, id(7), helpstring("property Type")] HRESULT Type([out, retval] DecalType *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_Type((DecalType*)&V_I4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1605: [propput, id(7), helpstring("property Type")] HRESULT Type([in] DecalType newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_I4);
				hr = put_Type((DecalType)V_I4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 8: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1606: [propget, id(8), helpstring("property Text")] HRESULT Text([out, retval] BSTR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_Text(&V_BSTR(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1607: [propput, id(8), helpstring("property Text")] HRESULT Text([in] BSTR newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = put_Text(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Decal_SizingType: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1608: [propget, id(DISPID_Decal_SizingType), helpstring("property SizingType")] HRESULT SizingType([out, retval] SizingType *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_SizingType((SizingType*)&V_I4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1609: [propput, id(DISPID_Decal_SizingType), helpstring("property SizingType")] HRESULT SizingType([in] SizingType newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_I4);
				hr = put_SizingType((SizingType)V_I4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 11: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1610: [propget, id(11), helpstring("property FontColor")] HRESULT FontColor([out, retval] OLE_COLOR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_UI4;
				hr = get_FontColor(&V_UI4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1611: [propput, id(11), helpstring("property FontColor")] HRESULT FontColor([in] OLE_COLOR newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_UI4);
				hr = put_FontColor((OLE_COLOR)V_UI4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 340: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1612: [propget, id(340), helpstring("property Material")] HRESULT Material([out, retval] BSTR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_Material(&V_BSTR(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1613: [propput, id(340), helpstring("property Material")] HRESULT Material([in] BSTR newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = put_Material(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Decal_Font: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1614: [propget, id(DISPID_Decal_Font), helpstring("property Font")] HRESULT Font([out, retval] IFontDisp **pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_DISPATCH;
				hr = get_Font((IFontDisp**)&V_DISPATCH(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUTREF) {
				// line 1615: [propputref, id(DISPID_Decal_Font), helpstring("property Font")] HRESULT Font([in] IFontDisp *newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantCopyInd(&var0, &pDispParams->rgvarg[--index]);
				hr = putref_Font((IFontDisp*)&var0);
				VariantClear(&var0);
			}
			break;
		}
		case 13: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1616: [propget, id(13), helpstring("property HasVerticalText")] HRESULT HasVerticalText([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_HasVerticalText(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1617: [propput, id(13), helpstring("property HasVerticalText")] HRESULT HasVerticalText([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_HasVerticalText(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Surface: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1618: [propget, id(DISPID_Surface), helpstring("property Surface")] HRESULT Surface([out, retval] BSTR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_Surface(&V_BSTR(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1619: [propput, id(DISPID_Surface), helpstring("property Surface")] HRESULT Surface([in] BSTR newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = put_Surface(V_BSTR(&var0));
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

STDMETHODIMP Decal::GetDocumentation(INT index, BSTR *pBstrName, BSTR *pBstrDocString, DWORD *pdwHelpContext, BSTR *pBstrHelpFile) {
	if (index == MEMBERID_NIL) {
		*pBstrName = SysAllocString(L"Decal");
		return S_OK;
	}
	return E_NOTIMPL;
}

name_disp_id_t Primitive::m_nameIDMap[] = {
		{ NULL },
		{ L"AddBlend", 556 },
		{ L"BackfacesEnabled", 792 },
		{ L"BlendDisableLighting", 494 },
		{ L"BlendDisableLightingFromBelow", 496 },
		{ L"Collidable", 111 },
		{ L"CollisionReductionFactor", 481 },
		{ L"Color", 557 },
		{ L"ContinueAnim", 35 },
		{ L"DepthBias", 397 },
		{ L"DisableLighting", 441 },
		{ L"DisplayTexture", 38 },
		{ L"DrawTexturesInside", 106 },
		{ L"EdgeFactorUI", 454 },
		{ L"Elasticity", 110 },
		{ L"ElasticityFalloff", 112 },
		{ L"EnableStaticRendering", 398 },
		{ L"Friction", 114 },
		{ L"HasHitEvent", 34 },
		{ L"HitThreshold", 735 },
		{ L"Image", DISPID_Image },
		{ L"IsToy", 395 },
		{ L"Material", 340 },
		{ L"Name", DISPID_Name },
		{ L"NormalMap", DISPID_Image2 },
		{ L"ObjectSpaceNormalMap", 824 },
		{ L"ObjRotX", DISPID_OBJROT_X },
		{ L"ObjRotY", DISPID_OBJROT_Y },
		{ L"ObjRotZ", DISPID_OBJROT_Z },
		{ L"Opacity", 377 },
		{ L"OverwritePhysics", 432 },
		{ L"PhysicsMaterial", 734 },
		{ L"PlayAnim", 1 },
		{ L"PlayAnimEndless", 2 },
		{ L"ReflectionEnabled", 431 },
		{ L"RotAndTra0", DISPID_ROTRA1 },
		{ L"RotAndTra1", DISPID_ROTRA2 },
		{ L"RotAndTra2", DISPID_ROTRA3 },
		{ L"RotAndTra3", DISPID_ROTRA4 },
		{ L"RotAndTra4", DISPID_ROTRA5 },
		{ L"RotAndTra5", DISPID_ROTRA6 },
		{ L"RotAndTra6", DISPID_ROTRA7 },
		{ L"RotAndTra7", DISPID_ROTRA8 },
		{ L"RotAndTra8", DISPID_ROTRA9 },
		{ L"RotX", DISPID_ROT_X },
		{ L"RotY", DISPID_ROT_Y },
		{ L"RotZ", DISPID_ROT_Z },
		{ L"Scatter", 115 },
		{ L"ShowFrame", 19 },
		{ L"SideColor", 104 },
		{ L"Sides", 101 },
		{ L"Size_X", DISPID_SIZE_X },
		{ L"Size_Y", DISPID_SIZE_Y },
		{ L"Size_Z", DISPID_SIZE_Z },
		{ L"StopAnim", 18 },
		{ L"Threshold", 33 },
		{ L"TransX", DISPID_TRANS_X },
		{ L"TransY", DISPID_TRANS_Y },
		{ L"TransZ", DISPID_TRANS_Z },
		{ L"UserValue", DISPID_UserValue },
		{ L"Visible", 458 },
		{ L"X", DISPID_POSITION_X },
		{ L"Y", DISPID_POSITION_Y },
		{ L"Z", DISPID_POSITION_Z }
};

STDMETHODIMP Primitive::GetIDsOfNames(REFIID /*riid*/, LPOLESTR* rgszNames, UINT cNames, LCID lcid, DISPID* rgDispId) {
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

STDMETHODIMP Primitive::Invoke(DISPID dispIdMember, REFIID /*riid*/, LCID lcid, WORD wFlags, DISPPARAMS* pDispParams, VARIANT* pVarResult, EXCEPINFO* pExcepInfo, UINT* puArgErr) {
	HRESULT hr = DISP_E_UNKNOWNNAME;
	int index = pDispParams->cArgs;
	switch(dispIdMember) {
		case 1: {
			if (wFlags & DISPATCH_METHOD) {
				// line 1637: [id(1), helpstring("method PlayAnim")] HRESULT PlayAnim([in]float startFrame, [in]float speed);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				VARIANT var1;
				V_VT(&var1) = VT_EMPTY;
				VariantChangeType(&var1, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = PlayAnim(V_R4(&var0), V_R4(&var1));
				VariantClear(&var0);
				VariantClear(&var1);
			}
			break;
		}
		case 2: {
			if (wFlags & DISPATCH_METHOD) {
				// line 1638: [id(2), helpstring("method PlayAnimEndless")] HRESULT PlayAnimEndless([in]float speed);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = PlayAnimEndless(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 18: {
			if (wFlags & DISPATCH_METHOD) {
				// line 1639: [id(18), helpstring("method StopAnim")] HRESULT StopAnim();
				hr = StopAnim();
			}
			break;
		}
		case 19: {
			if (wFlags & DISPATCH_METHOD) {
				// line 1640: [id(19), helpstring("method ShowFrame")] HRESULT ShowFrame([in]float frame);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = ShowFrame(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 35: {
			if (wFlags & DISPATCH_METHOD) {
				// line 1641: [id(35), helpstring("method ContinueAnim")] HRESULT ContinueAnim([in]float speed);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = ContinueAnim(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 38: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1642: [propget, id(38), helpstring("property DisplayTexture")] HRESULT DisplayTexture([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_DisplayTexture(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1643: [propput, id(38), helpstring("property DisplayTexture")] HRESULT DisplayTexture([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_DisplayTexture(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 101: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1645: [propget, id(101), helpstring("property Sides")] HRESULT Sides([out, retval] int *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_Sides((int*)&V_I4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1646: [propput, id(101), helpstring("property Sides")] HRESULT Sides([in] int newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_I4);
				hr = put_Sides(V_I4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 458: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1647: [propget, id(458), helpstring("property Top Visible")] HRESULT Visible([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_Visible(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1648: [propput, id(458), helpstring("property Top Visible")] HRESULT Visible([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_Visible(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 340: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1649: [propget, id(340), helpstring("property Material")] HRESULT Material([out, retval] BSTR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_Material(&V_BSTR(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1650: [propput, id(340), helpstring("property Material")] HRESULT Material([in] BSTR newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = put_Material(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 104: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1651: [propget, id(104), helpstring("property Side Color")] HRESULT SideColor([out, retval] OLE_COLOR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_UI4;
				hr = get_SideColor(&V_UI4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1652: [propput, id(104), helpstring("property Side Color")] HRESULT SideColor([in] OLE_COLOR newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_UI4);
				hr = put_SideColor((OLE_COLOR)V_UI4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 106: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1653: [propget, id(106), helpstring("property Draw Textures Inside")] HRESULT DrawTexturesInside([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_DrawTexturesInside(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1654: [propput, id(106), helpstring("property Draw Textures Inside")] HRESULT DrawTexturesInside([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_DrawTexturesInside(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_UserValue: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1656: [propget, id(DISPID_UserValue), helpstring("property User")] HRESULT UserValue([out, retval] VARIANT* pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				hr = get_UserValue(pVarResult);
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1657: [propput, id(DISPID_UserValue), helpstring("property User")] HRESULT UserValue([in] VARIANT* newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantCopyInd(&var0, &pDispParams->rgvarg[--index]);
				hr = put_UserValue(&var0);
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Image: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1659: [propget, id(DISPID_Image), helpstring("property Image")] HRESULT Image([out, retval] BSTR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_Image(&V_BSTR(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1660: [propput, id(DISPID_Image), helpstring("property Image")] HRESULT Image([in] BSTR newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = put_Image(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Image2: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1661: [propget, id(DISPID_Image2), helpstring("property NormalMap")] HRESULT NormalMap([out, retval] BSTR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_NormalMap(&V_BSTR(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1662: [propput, id(DISPID_Image2), helpstring("property NormalMap")] HRESULT NormalMap([in] BSTR newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = put_NormalMap(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_POSITION_X: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1664: [propget, id(DISPID_POSITION_X), helpstring("property X")] HRESULT X([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_X(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1665: [propput, id(DISPID_POSITION_X), helpstring("property X")] HRESULT X([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_X(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_POSITION_Y: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1666: [propget, id(DISPID_POSITION_Y), helpstring("property Y")] HRESULT Y([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Y(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1667: [propput, id(DISPID_POSITION_Y), helpstring("property Y")] HRESULT Y([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Y(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_POSITION_Z: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1668: [propget, id(DISPID_POSITION_Z), helpstring("property Z")] HRESULT Z([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Z(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1669: [propput, id(DISPID_POSITION_Z), helpstring("property Z")] HRESULT Z([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Z(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_SIZE_X: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1670: [propget, id(DISPID_SIZE_X), helpstring("property Size X")] HRESULT Size_X([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Size_X(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1671: [propput, id(DISPID_SIZE_X), helpstring("property Size X")] HRESULT Size_X([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Size_X(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_SIZE_Y: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1672: [propget, id(DISPID_SIZE_Y), helpstring("property Size Y")] HRESULT Size_Y([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Size_Y(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1673: [propput, id(DISPID_SIZE_Y), helpstring("property Size Y")] HRESULT Size_Y([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Size_Y(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_SIZE_Z: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1674: [propget, id(DISPID_SIZE_Z), helpstring("property Size Z")] HRESULT Size_Z([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Size_Z(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1675: [propput, id(DISPID_SIZE_Z), helpstring("property Size Z")] HRESULT Size_Z([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Size_Z(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_ROTRA1: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1677: [propget, id(DISPID_ROTRA1), helpstring("property Rot and Tra 1")] HRESULT RotAndTra0([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_RotAndTra0(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1678: [propput, id(DISPID_ROTRA1), helpstring("property Rot and Tra 1")] HRESULT RotAndTra0([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_RotAndTra0(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_ROT_X: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1679: [propget, id(DISPID_ROT_X), helpstring("property RotX")] HRESULT RotX([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_RotX(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1680: [propput, id(DISPID_ROT_X), helpstring("property RotX")] HRESULT RotX([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_RotX(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_ROTRA2: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1682: [propget, id(DISPID_ROTRA2), helpstring("property Rot and Tra 2")] HRESULT RotAndTra1([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_RotAndTra1(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1683: [propput, id(DISPID_ROTRA2), helpstring("property Rot and Tra 2")] HRESULT RotAndTra1([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_RotAndTra1(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_ROT_Y: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1684: [propget, id(DISPID_ROT_Y), helpstring("property RotY")] HRESULT RotY([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_RotY(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1685: [propput, id(DISPID_ROT_Y), helpstring("property RotY")] HRESULT RotY([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_RotY(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_ROTRA3: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1687: [propget, id(DISPID_ROTRA3), helpstring("property Rot and Tra 3")] HRESULT RotAndTra2([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_RotAndTra2(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1688: [propput, id(DISPID_ROTRA3), helpstring("property Rot and Tra 3")] HRESULT RotAndTra2([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_RotAndTra2(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_ROT_Z: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1689: [propget, id(DISPID_ROT_Z), helpstring("property RotZ")] HRESULT RotZ([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_RotZ(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1690: [propput, id(DISPID_ROT_Z), helpstring("property RotZ")] HRESULT RotZ([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_RotZ(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_ROTRA4: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1692: [propget, id(DISPID_ROTRA4), helpstring("property Rot and Tra 4")] HRESULT RotAndTra3([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_RotAndTra3(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1693: [propput, id(DISPID_ROTRA4), helpstring("property Rot and Tra 4")] HRESULT RotAndTra3([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_RotAndTra3(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_TRANS_X: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1694: [propget, id(DISPID_TRANS_X), helpstring("property TransX")] HRESULT TransX([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_TransX(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1695: [propput, id(DISPID_TRANS_X), helpstring("property TransX")] HRESULT TransX([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_TransX(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_ROTRA5: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1697: [propget, id(DISPID_ROTRA5), helpstring("property Rot and Tra 5")] HRESULT RotAndTra4([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_RotAndTra4(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1698: [propput, id(DISPID_ROTRA5), helpstring("property Rot and Tra 5")] HRESULT RotAndTra4([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_RotAndTra4(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_TRANS_Y: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1699: [propget, id(DISPID_TRANS_Y), helpstring("property TransY")] HRESULT TransY([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_TransY(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1700: [propput, id(DISPID_TRANS_Y), helpstring("property TransY")] HRESULT TransY([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_TransY(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_ROTRA6: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1702: [propget, id(DISPID_ROTRA6), helpstring("property Rot and Tra 6")] HRESULT RotAndTra5([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_RotAndTra5(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1703: [propput, id(DISPID_ROTRA6), helpstring("property Rot and Tra 6")] HRESULT RotAndTra5([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_RotAndTra5(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_TRANS_Z: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1704: [propget, id(DISPID_TRANS_Z), helpstring("property TransZ")] HRESULT TransZ([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_TransZ(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1705: [propput, id(DISPID_TRANS_Z), helpstring("property TransZ")] HRESULT TransZ([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_TransZ(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_ROTRA7: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1707: [propget, id(DISPID_ROTRA7), helpstring("property Rot and Tra 7")] HRESULT RotAndTra6([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_RotAndTra6(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1708: [propput, id(DISPID_ROTRA7), helpstring("property Rot and Tra 7")] HRESULT RotAndTra6([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_RotAndTra6(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_OBJROT_X: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1709: [propget, id(DISPID_OBJROT_X), helpstring("property ObjRotX")] HRESULT ObjRotX([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_ObjRotX(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1710: [propput, id(DISPID_OBJROT_X), helpstring("property ObjRotX")] HRESULT ObjRotX([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_ObjRotX(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_ROTRA8: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1712: [propget, id(DISPID_ROTRA8), helpstring("property Rot and Tra 8")] HRESULT RotAndTra7([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_RotAndTra7(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1713: [propput, id(DISPID_ROTRA8), helpstring("property Rot and Tra 8")] HRESULT RotAndTra7([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_RotAndTra7(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_OBJROT_Y: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1714: [propget, id(DISPID_OBJROT_Y), helpstring("property ObjRotY")] HRESULT ObjRotY([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_ObjRotY(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1715: [propput, id(DISPID_OBJROT_Y), helpstring("property ObjRotY")] HRESULT ObjRotY([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_ObjRotY(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_ROTRA9: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1717: [propget, id(DISPID_ROTRA9), helpstring("property Rot and Tra 9")] HRESULT RotAndTra8([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_RotAndTra8(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1718: [propput, id(DISPID_ROTRA9), helpstring("property Rot and Tra 9")] HRESULT RotAndTra8([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_RotAndTra8(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_OBJROT_Z: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1719: [propget, id(DISPID_OBJROT_Z), helpstring("property ObjRotZ")] HRESULT ObjRotZ([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_ObjRotZ(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1720: [propput, id(DISPID_OBJROT_Z), helpstring("property ObjRotZ")] HRESULT ObjRotZ([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_ObjRotZ(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 454: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1722: [propget, id(454), helpstring("property EdgeFactorUI")] HRESULT EdgeFactorUI([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_EdgeFactorUI(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1723: [propput, id(454), helpstring("property EdgeFactorUI")] HRESULT EdgeFactorUI([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_EdgeFactorUI(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 481: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1724: [propget, id(481), helpstring("property CollisionReductionFactor")] HRESULT CollisionReductionFactor([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_CollisionReductionFactor(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1725: [propput, id(481), helpstring("property CollisionReductionFactor")] HRESULT CollisionReductionFactor([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_CollisionReductionFactor(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Name: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1727: [propget, id(DISPID_Name), helpstring("property Name")] HRESULT Name([out, retval] BSTR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_Name(&V_BSTR(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1728: [propput, id(DISPID_Name), helpstring("property Name")] HRESULT Name([in] BSTR newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = put_Name(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 398: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1729: [propget, id(398), helpstring("property static rendering")] HRESULT EnableStaticRendering([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_EnableStaticRendering(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1730: [propput, id(398), helpstring("property static rendering")] HRESULT EnableStaticRendering([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_EnableStaticRendering(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 110: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1732: [propget, id(110), helpstring("property Elasticity")] HRESULT Elasticity([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Elasticity(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1733: [propput, id(110), helpstring("property Elasticity")] HRESULT Elasticity([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Elasticity(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 112: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1734: [propget, id(112), helpstring("property ElasticityFalloff")] HRESULT ElasticityFalloff([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_ElasticityFalloff(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1735: [propput, id(112), helpstring("property ElasticityFalloff")] HRESULT ElasticityFalloff([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_ElasticityFalloff(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 111: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1737: [propget, id(111), helpstring("property Collidable")] HRESULT Collidable([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_Collidable(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1738: [propput, id(111), helpstring("property Collidable")] HRESULT Collidable([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_Collidable(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 395: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1739: [propget, id(395), helpstring("property IsToy")] HRESULT IsToy([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_IsToy(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1740: [propput, id(395), helpstring("property IsToy")] HRESULT IsToy([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_IsToy(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 792: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1741: [propget, id(792), helpstring("property BackfacesEnabled")] HRESULT BackfacesEnabled([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_BackfacesEnabled(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1742: [propput, id(792), helpstring("property BackfacesEnabled")] HRESULT BackfacesEnabled([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_BackfacesEnabled(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 34: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1743: [propget, id(34), helpstring("property HasHitEvent")] HRESULT HasHitEvent([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_HasHitEvent(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1744: [propput, id(34), helpstring("property HasHitEvent")] HRESULT HasHitEvent([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_HasHitEvent(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 33: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1745: [propget, id(33), helpstring("property Threshold")] HRESULT Threshold([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Threshold(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1746: [propput, id(33), helpstring("property Threshold")] HRESULT Threshold([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Threshold(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 114: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1747: [propget, id(114), helpstring("property Friction")] HRESULT Friction([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Friction(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1748: [propput, id(114), helpstring("property Friction")] HRESULT Friction([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Friction(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 115: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1749: [propget, id(115), helpstring("property Scatter")] HRESULT Scatter([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Scatter(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1750: [propput, id(115), helpstring("property Scatter")] HRESULT Scatter([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Scatter(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 441: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1751: [propget, id(441), helpstring("property DisableLighting")] HRESULT DisableLighting([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_DisableLighting(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1752: [propput, id(441), helpstring("property DisableLighting")] HRESULT DisableLighting([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_DisableLighting(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 494: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1753: [propget, id(494), helpstring("property BlendDisableLighting")] HRESULT BlendDisableLighting([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_BlendDisableLighting(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1754: [propput, id(494), helpstring("property BlendDisableLighting")] HRESULT BlendDisableLighting([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_BlendDisableLighting(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 496: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1755: [propget, id(496), helpstring("property BlendDisableLightingFromBelow")] HRESULT BlendDisableLightingFromBelow([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_BlendDisableLightingFromBelow(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1756: [propput, id(496), helpstring("property BlendDisableLightingFromBelow")] HRESULT BlendDisableLightingFromBelow([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_BlendDisableLightingFromBelow(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 431: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1757: [propget, id(431), helpstring("property ReflectionEnabled")] HRESULT ReflectionEnabled([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_ReflectionEnabled(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1758: [propput, id(431), helpstring("property ReflectionEnabled")] HRESULT ReflectionEnabled([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_ReflectionEnabled(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 377: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1760: [propget, id(377), helpstring("property Opacity")] HRESULT Opacity([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Opacity(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1761: [propput, id(377), helpstring("property Opacity")] HRESULT Opacity([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Opacity(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 556: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1762: [propget, id(556), helpstring("property AddBlend")] HRESULT AddBlend([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_AddBlend(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1763: [propput, id(556), helpstring("property AddBlend")] HRESULT AddBlend([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_AddBlend(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 557: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1764: [propget, id(557), helpstring("property Color")] HRESULT Color([out, retval] OLE_COLOR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_UI4;
				hr = get_Color(&V_UI4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1765: [propput, id(557), helpstring("property Color")] HRESULT Color([in] OLE_COLOR newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_UI4);
				hr = put_Color((OLE_COLOR)V_UI4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 397: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1767: [propget, id(397), helpstring("property DepthBias")] HRESULT DepthBias([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_DepthBias(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1768: [propput, id(397), helpstring("property DepthBias")] HRESULT DepthBias([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_DepthBias(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 734: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1770: [propget, id(734), helpstring("property PhysicsMaterial")] HRESULT PhysicsMaterial([out, retval] BSTR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_PhysicsMaterial(&V_BSTR(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1771: [propput, id(734), helpstring("property PhysicsMaterial")] HRESULT PhysicsMaterial([in] BSTR newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = put_PhysicsMaterial(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 432: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1772: [propget, id(432), helpstring("property OverwritePhysics")] HRESULT OverwritePhysics([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_OverwritePhysics(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1773: [propput, id(432), helpstring("property OverwritePhysics")] HRESULT OverwritePhysics([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_OverwritePhysics(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 735: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1774: [propget, id(735), helpstring("property HitThreshold")] HRESULT HitThreshold([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_HitThreshold(&V_R4(pVarResult));
			}
			break;
		}
		case 824: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1775: [propget, id(824), helpstring("property ObjectSpaceNormalMap")] HRESULT ObjectSpaceNormalMap([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_ObjectSpaceNormalMap(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1776: [propput, id(824), helpstring("property ObjectSpaceNormalMap")] HRESULT ObjectSpaceNormalMap([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_ObjectSpaceNormalMap(V_BOOL(&var0));
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

STDMETHODIMP Primitive::GetDocumentation(INT index, BSTR *pBstrName, BSTR *pBstrDocString, DWORD *pdwHelpContext, BSTR *pBstrHelpFile) {
	if (index == MEMBERID_NIL) {
		*pBstrName = SysAllocString(L"Primitive");
		return S_OK;
	}
	return E_NOTIMPL;
}

disp_id_name_t Primitive::m_idNameMap[] = {
		{ NULL },
		{ DISPID_GameEvents_Init, L"_Init" },
		{ DISPID_HitEvents_Hit, L"_Hit" }
};

HRESULT Primitive::FireDispID(const DISPID dispid, DISPPARAMS * const pdispparams) {
	static WCHAR wzName[MAXSTRING];
	CComPtr<IDispatch> disp;
	g_pplayer->m_ptable->m_pcv->m_pScript->GetScriptDispatch(nullptr, &disp);
	size_t min = 1, max = ARRAY_SIZE(m_idNameMap) - 1, i;
	int r;
	while(min <= max) {
		i = (min + max) / 2;
		if (m_idNameMap[i].dispId == dispid) {
			wcscpy(wzName, m_wzName);
			wcscat(wzName, m_idNameMap[i].name);
			LPOLESTR fnNames = (LPOLESTR)wzName;
			DISPID tDispid;
			if (SUCCEEDED(disp->GetIDsOfNames(IID_NULL, &fnNames, 1, 0, &tDispid))) {
				return disp->Invoke(tDispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, pdispparams, nullptr, nullptr, nullptr);
			}
			return DISP_E_MEMBERNOTFOUND;
		}
		else if (m_idNameMap[i].dispId < dispid)
		   min = i+1;
		else
		   max = i-1;
	}
	return DISP_E_MEMBERNOTFOUND;
}

name_disp_id_t HitTarget::m_nameIDMap[] = {
		{ NULL },
		{ L"BlendDisableLighting", 494 },
		{ L"BlendDisableLightingFromBelow", 496 },
		{ L"Collidable", 111 },
		{ L"CurrentAnimOffset", 27 },
		{ L"DepthBias", 397 },
		{ L"DisableLighting", 483 },
		{ L"DrawStyle", 9 },
		{ L"DropSpeed", 377 },
		{ L"Elasticity", 110 },
		{ L"ElasticityFalloff", 112 },
		{ L"Friction", 114 },
		{ L"HasHitEvent", 34 },
		{ L"HitThreshold", 727 },
		{ L"Image", DISPID_Image },
		{ L"IsDropped", 435 },
		{ L"LegacyMode", 433 },
		{ L"Material", 340 },
		{ L"Name", DISPID_Name },
		{ L"Orientation", DISPID_ROT_Z },
		{ L"OverwritePhysics", 432 },
		{ L"PhysicsMaterial", 734 },
		{ L"RaiseDelay", 726 },
		{ L"ReflectionEnabled", 431 },
		{ L"ScaleX", DISPID_SIZE_X },
		{ L"ScaleY", DISPID_SIZE_Y },
		{ L"ScaleZ", DISPID_SIZE_Z },
		{ L"Scatter", 115 },
		{ L"Threshold", 33 },
		{ L"TimerEnabled", DISPID_Timer_Enabled },
		{ L"TimerInterval", DISPID_Timer_Interval },
		{ L"UserValue", DISPID_UserValue },
		{ L"Visible", 458 },
		{ L"X", DISPID_POSITION_X },
		{ L"Y", DISPID_POSITION_Y },
		{ L"Z", DISPID_POSITION_Z }
};

STDMETHODIMP HitTarget::GetIDsOfNames(REFIID /*riid*/, LPOLESTR* rgszNames, UINT cNames, LCID lcid, DISPID* rgDispId) {
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

STDMETHODIMP HitTarget::Invoke(DISPID dispIdMember, REFIID /*riid*/, LCID lcid, WORD wFlags, DISPPARAMS* pDispParams, VARIANT* pVarResult, EXCEPINFO* pExcepInfo, UINT* puArgErr) {
	HRESULT hr = DISP_E_UNKNOWNNAME;
	int index = pDispParams->cArgs;
	switch(dispIdMember) {
		case DISPID_Timer_Enabled: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1806: [propget, id(DISPID_Timer_Enabled), helpstring("property TimerEnabled")] HRESULT TimerEnabled([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_TimerEnabled(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1807: [propput, id(DISPID_Timer_Enabled), helpstring("property TimerEnabled")] HRESULT TimerEnabled([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_TimerEnabled(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Timer_Interval: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1808: [propget, id(DISPID_Timer_Interval), helpstring("property TimerInterval")] HRESULT TimerInterval([out, retval] long *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_TimerInterval((long*)&V_I4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1809: [propput, id(DISPID_Timer_Interval), helpstring("property TimerInterval")] HRESULT TimerInterval([in] long newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_I4);
				hr = put_TimerInterval(V_I4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 458: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1810: [propget, id(458), helpstring("property Top Visible")] HRESULT Visible([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_Visible(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1811: [propput, id(458), helpstring("property Top Visible")] HRESULT Visible([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_Visible(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 340: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1812: [propget, id(340), helpstring("property Material")] HRESULT Material([out, retval] BSTR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_Material(&V_BSTR(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1813: [propput, id(340), helpstring("property Material")] HRESULT Material([in] BSTR newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = put_Material(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_UserValue: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1814: [propget, id(DISPID_UserValue), helpstring("property User")] HRESULT UserValue([out, retval] VARIANT* pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				hr = get_UserValue(pVarResult);
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1815: [propput, id(DISPID_UserValue), helpstring("property User")] HRESULT UserValue([in] VARIANT* newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantCopyInd(&var0, &pDispParams->rgvarg[--index]);
				hr = put_UserValue(&var0);
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Image: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1817: [propget, id(DISPID_Image), helpstring("property Image")] HRESULT Image([out, retval] BSTR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_Image(&V_BSTR(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1818: [propput, id(DISPID_Image), helpstring("property Image")] HRESULT Image([in] BSTR newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = put_Image(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_POSITION_X: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1820: [propget, id(DISPID_POSITION_X), helpstring("property X")] HRESULT X([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_X(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1821: [propput, id(DISPID_POSITION_X), helpstring("property X")] HRESULT X([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_X(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_POSITION_Y: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1822: [propget, id(DISPID_POSITION_Y), helpstring("property Y")] HRESULT Y([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Y(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1823: [propput, id(DISPID_POSITION_Y), helpstring("property Y")] HRESULT Y([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Y(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_POSITION_Z: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1824: [propget, id(DISPID_POSITION_Z), helpstring("property Z")] HRESULT Z([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Z(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1825: [propput, id(DISPID_POSITION_Z), helpstring("property Z")] HRESULT Z([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Z(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_SIZE_X: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1826: [propget, id(DISPID_SIZE_X), helpstring("property ScaleX")] HRESULT ScaleX([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_ScaleX(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1827: [propput, id(DISPID_SIZE_X), helpstring("property ScaleX")] HRESULT ScaleX([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_ScaleX(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_SIZE_Y: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1828: [propget, id(DISPID_SIZE_Y), helpstring("property ScaleY")] HRESULT ScaleY([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_ScaleY(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1829: [propput, id(DISPID_SIZE_Y), helpstring("property ScaleY")] HRESULT ScaleY([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_ScaleY(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_SIZE_Z: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1830: [propget, id(DISPID_SIZE_Z), helpstring("property ScaleZ")] HRESULT ScaleZ([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_ScaleZ(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1831: [propput, id(DISPID_SIZE_Z), helpstring("property ScaleZ")] HRESULT ScaleZ([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_ScaleZ(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_ROT_Z: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1833: [propget, id(DISPID_ROT_Z), helpstring("property Orientation")] HRESULT Orientation([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Orientation(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1834: [propput, id(DISPID_ROT_Z), helpstring("property Orientation")] HRESULT Orientation([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Orientation(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Name: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1836: [propget, id(DISPID_Name), helpstring("property Name")] HRESULT Name([out, retval] BSTR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_Name(&V_BSTR(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1837: [propput, id(DISPID_Name), helpstring("property Name")] HRESULT Name([in] BSTR newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = put_Name(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 110: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1839: [propget, id(110), helpstring("property Elasticity")] HRESULT Elasticity([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Elasticity(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1840: [propput, id(110), helpstring("property Elasticity")] HRESULT Elasticity([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Elasticity(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 112: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1841: [propget, id(112), helpstring("property ElasticityFalloff")] HRESULT ElasticityFalloff([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_ElasticityFalloff(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1842: [propput, id(112), helpstring("property ElasticityFalloff")] HRESULT ElasticityFalloff([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_ElasticityFalloff(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 111: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1844: [propget, id(111), helpstring("property Collidable")] HRESULT Collidable([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_Collidable(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1845: [propput, id(111), helpstring("property Collidable")] HRESULT Collidable([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_Collidable(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 34: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1846: [propget, id(34), helpstring("property HasHitEvent")] HRESULT HasHitEvent([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_HasHitEvent(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1847: [propput, id(34), helpstring("property HasHitEvent")] HRESULT HasHitEvent([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_HasHitEvent(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 33: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1848: [propget, id(33), helpstring("property Threshold")] HRESULT Threshold([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Threshold(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1849: [propput, id(33), helpstring("property Threshold")] HRESULT Threshold([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Threshold(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 114: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1850: [propget, id(114), helpstring("property Friction")] HRESULT Friction([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Friction(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1851: [propput, id(114), helpstring("property Friction")] HRESULT Friction([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Friction(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 115: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1852: [propget, id(115), helpstring("property Scatter")] HRESULT Scatter([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Scatter(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1853: [propput, id(115), helpstring("property Scatter")] HRESULT Scatter([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Scatter(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 483: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1854: [propget, id(483), helpstring("property DisableLighting")] HRESULT DisableLighting([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_DisableLighting(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1855: [propput, id(483), helpstring("property DisableLighting")] HRESULT DisableLighting([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_DisableLighting(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 494: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1856: [propget, id(494), helpstring("property BlendDisableLighting")] HRESULT BlendDisableLighting([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_BlendDisableLighting(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1857: [propput, id(494), helpstring("property BlendDisableLighting")] HRESULT BlendDisableLighting([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_BlendDisableLighting(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 496: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1858: [propget, id(496), helpstring("property BlendDisableLightingFromBelow")] HRESULT BlendDisableLightingFromBelow([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_BlendDisableLightingFromBelow(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1859: [propput, id(496), helpstring("property BlendDisableLightingFromBelow")] HRESULT BlendDisableLightingFromBelow([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_BlendDisableLightingFromBelow(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 431: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1860: [propget, id(431), helpstring("property ReflectionEnabled")] HRESULT ReflectionEnabled([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_ReflectionEnabled(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1861: [propput, id(431), helpstring("property ReflectionEnabled")] HRESULT ReflectionEnabled([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_ReflectionEnabled(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 397: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1863: [propget, id(397), helpstring("property DepthBias")] HRESULT DepthBias([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_DepthBias(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1864: [propput, id(397), helpstring("property DepthBias")] HRESULT DepthBias([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_DepthBias(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 377: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1866: [propget, id(377), helpstring("property DropSpeed")] HRESULT DropSpeed([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_DropSpeed(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1867: [propput, id(377), helpstring("property DropSpeed")] HRESULT DropSpeed([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_DropSpeed(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 27: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1868: [propget, id(27), helpstring("property CurrentAnimOffset")] HRESULT CurrentAnimOffset([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_CurrentAnimOffset(&V_R4(pVarResult));
			}
			break;
		}
		case 435: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1869: [propget, id(435), helpstring("property IsDropped")] HRESULT IsDropped([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_IsDropped(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1870: [propput, id(435), helpstring("property IsDropped")] HRESULT IsDropped([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_IsDropped(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 9: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1871: [propget, id(9), helpstring("property DrawStyle")] HRESULT DrawStyle([out, retval] TargetType *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_DrawStyle((TargetType*)&V_I4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1872: [propput, id(9), helpstring("property DrawStyle")] HRESULT DrawStyle([in] TargetType newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_I4);
				hr = put_DrawStyle((TargetType)V_I4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 433: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1873: [propget, id(433), helpstring("property LegacyMode")] HRESULT LegacyMode([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_LegacyMode(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1874: [propput, id(433), helpstring("property LegacyMode")] HRESULT LegacyMode([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_LegacyMode(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 726: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1875: [propget, id(726), helpstring("property RaiseDelay")] HRESULT RaiseDelay([out, retval] long *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_RaiseDelay((long*)&V_I4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1876: [propput, id(726), helpstring("property RaiseDelay")] HRESULT RaiseDelay([in] long newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_I4);
				hr = put_RaiseDelay(V_I4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 734: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1877: [propget, id(734), helpstring("property PhysicsMaterial")] HRESULT PhysicsMaterial([out, retval] BSTR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_PhysicsMaterial(&V_BSTR(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1878: [propput, id(734), helpstring("property PhysicsMaterial")] HRESULT PhysicsMaterial([in] BSTR newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = put_PhysicsMaterial(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 432: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1879: [propget, id(432), helpstring("property OverwritePhysics")] HRESULT OverwritePhysics([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_OverwritePhysics(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1880: [propput, id(432), helpstring("property OverwritePhysics")] HRESULT OverwritePhysics([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_OverwritePhysics(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 727: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1881: [propget, id(727), helpstring("property HitThreshold")] HRESULT HitThreshold([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_HitThreshold(&V_R4(pVarResult));
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

STDMETHODIMP HitTarget::GetDocumentation(INT index, BSTR *pBstrName, BSTR *pBstrDocString, DWORD *pdwHelpContext, BSTR *pBstrHelpFile) {
	if (index == MEMBERID_NIL) {
		*pBstrName = SysAllocString(L"HitTarget");
		return S_OK;
	}
	return E_NOTIMPL;
}

disp_id_name_t HitTarget::m_idNameMap[] = {
		{ NULL },
		{ DISPID_GameEvents_Init, L"_Init" },
		{ DISPID_TimerEvents_Timer, L"_Timer" },
		{ DISPID_TargetEvents_Dropped, L"_Dropped" },
		{ DISPID_TargetEvents_Raised, L"_Raised" },
		{ DISPID_HitEvents_Hit, L"_Hit" },
		{ DISPID_AnimateEvents_Animate, L"_Animate" }
};

HRESULT HitTarget::FireDispID(const DISPID dispid, DISPPARAMS * const pdispparams) {
	static WCHAR wzName[MAXSTRING];
	CComPtr<IDispatch> disp;
	g_pplayer->m_ptable->m_pcv->m_pScript->GetScriptDispatch(nullptr, &disp);
	size_t min = 1, max = ARRAY_SIZE(m_idNameMap) - 1, i;
	int r;
	while(min <= max) {
		i = (min + max) / 2;
		if (m_idNameMap[i].dispId == dispid) {
			wcscpy(wzName, m_wzName);
			wcscat(wzName, m_idNameMap[i].name);
			LPOLESTR fnNames = (LPOLESTR)wzName;
			DISPID tDispid;
			if (SUCCEEDED(disp->GetIDsOfNames(IID_NULL, &fnNames, 1, 0, &tDispid))) {
				return disp->Invoke(tDispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, pdispparams, nullptr, nullptr, nullptr);
			}
			return DISP_E_MEMBERNOTFOUND;
		}
		else if (m_idNameMap[i].dispId < dispid)
		   min = i+1;
		else
		   max = i-1;
	}
	return DISP_E_MEMBERNOTFOUND;
}

name_disp_id_t Gate::m_nameIDMap[] = {
		{ NULL },
		{ L"CloseAngle", 2144 },
		{ L"Collidable", 111 },
		{ L"CurrentAngle", 16 },
		{ L"Damping", 13 },
		{ L"DrawStyle", 9 },
		{ L"Elasticity", 110 },
		{ L"Friction", 114 },
		{ L"GravityFactor", 17 },
		{ L"Height", 1142 },
		{ L"Length", DISPID_Gate_Length },
		{ L"Material", 340 },
		{ L"Move", 2147 },
		{ L"Name", DISPID_Name },
		{ L"Open", 7 },
		{ L"OpenAngle", 2145 },
		{ L"ReflectionEnabled", 431 },
		{ L"Rotation", DISPID_Gate_Rotation },
		{ L"ShowBracket", 15 },
		{ L"Surface", DISPID_Surface },
		{ L"TimerEnabled", DISPID_Timer_Enabled },
		{ L"TimerInterval", DISPID_Timer_Interval },
		{ L"TwoWay", 427 },
		{ L"UserValue", DISPID_UserValue },
		{ L"Visible", 458 },
		{ L"X", 5 },
		{ L"Y", 6 }
};

STDMETHODIMP Gate::GetIDsOfNames(REFIID /*riid*/, LPOLESTR* rgszNames, UINT cNames, LCID lcid, DISPID* rgDispId) {
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

STDMETHODIMP Gate::Invoke(DISPID dispIdMember, REFIID /*riid*/, LCID lcid, WORD wFlags, DISPPARAMS* pDispParams, VARIANT* pVarResult, EXCEPINFO* pExcepInfo, UINT* puArgErr) {
	HRESULT hr = DISP_E_UNKNOWNNAME;
	int index = pDispParams->cArgs;
	switch(dispIdMember) {
		case DISPID_Name: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1912: [propget, id(DISPID_Name), helpstring("property Name")] HRESULT Name([out, retval] BSTR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_Name(&V_BSTR(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1913: [propput, id(DISPID_Name), helpstring("property Name")] HRESULT Name([in] BSTR newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = put_Name(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Timer_Enabled: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1914: [propget, id(DISPID_Timer_Enabled), helpstring("property TimerEnabled")] HRESULT TimerEnabled([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_TimerEnabled(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1915: [propput, id(DISPID_Timer_Enabled), helpstring("property TimerEnabled")] HRESULT TimerEnabled([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_TimerEnabled(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Timer_Interval: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1916: [propget, id(DISPID_Timer_Interval), helpstring("property TimerInterval")] HRESULT TimerInterval([out, retval] long *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_TimerInterval((long*)&V_I4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1917: [propput, id(DISPID_Timer_Interval), helpstring("property TimerInterval")] HRESULT TimerInterval([in] long newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_I4);
				hr = put_TimerInterval(V_I4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Gate_Length: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1918: [propget, id(DISPID_Gate_Length), helpstring("property Length")] HRESULT Length([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Length(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1919: [propput, id(DISPID_Gate_Length), helpstring("property Length")] HRESULT Length([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Length(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 1142: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1920: [propget, id(1142), helpstring("property Height")] HRESULT Height([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Height(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1921: [propput, id(1142), helpstring("property Height")] HRESULT Height([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Height(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Gate_Rotation: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1922: [propget, id(DISPID_Gate_Rotation), helpstring("property Rotation")] HRESULT Rotation([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Rotation(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1923: [propput, id(DISPID_Gate_Rotation), helpstring("property Rotation")] HRESULT Rotation([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Rotation(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 5: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1924: [propget, id(5), helpstring("property X")] HRESULT X([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_X(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1925: [propput, id(5), helpstring("property X")] HRESULT X([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_X(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 6: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1926: [propget, id(6), helpstring("property Y")] HRESULT Y([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Y(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1927: [propput, id(6), helpstring("property Y")] HRESULT Y([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Y(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 7: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1928: [propget, id(7), helpstring("property Open")] HRESULT Open([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_Open(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1929: [propput, id(7), helpstring("property Open")] HRESULT Open([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_Open(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 13: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1930: [propget, id(13), helpstring("property Damping")] HRESULT Damping([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Damping(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1931: [propput, id(13), helpstring("property Damping")] HRESULT Damping([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Damping(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 17: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1932: [propget, id(17), helpstring("property GravityFactor")] HRESULT GravityFactor([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_GravityFactor(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1933: [propput, id(17), helpstring("property GravityFactor")] HRESULT GravityFactor([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_GravityFactor(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 2147: {
			if (wFlags & DISPATCH_METHOD) {
				// line 1935: [id(2147), helpstring("method Move")] HRESULT Move(int dir, [defaultvalue(0)] float speed, [defaultvalue(0)] float angle);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_I4);
				VARIANT var1;
				V_VT(&var1) = VT_R4;
				V_R4(&var1) = 0;
				VariantChangeType(&var1, (index > 0) ? &pDispParams->rgvarg[--index] : &var1, 0, VT_R4);
				VARIANT var2;
				V_VT(&var2) = VT_R4;
				V_R4(&var2) = 0;
				VariantChangeType(&var2, (index > 0) ? &pDispParams->rgvarg[--index] : &var2, 0, VT_R4);
				hr = Move(V_I4(&var0), V_R4(&var1), V_R4(&var2));
				VariantClear(&var0);
				VariantClear(&var1);
				VariantClear(&var2);
			}
			break;
		}
		case 340: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1937: [propget, id(340), helpstring("property Material")] HRESULT Material([out, retval] BSTR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_Material(&V_BSTR(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1938: [propput, id(340), helpstring("property Material")] HRESULT Material([in] BSTR newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = put_Material(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 110: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1939: [propget, id(110), helpstring("property Elasticity")] HRESULT Elasticity([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Elasticity(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1940: [propput, id(110), helpstring("property Elasticity")] HRESULT Elasticity([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Elasticity(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Surface: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1941: [propget, id(DISPID_Surface), helpstring("property Surface")] HRESULT Surface([out, retval] BSTR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_Surface(&V_BSTR(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1942: [propput, id(DISPID_Surface), helpstring("property Surface")] HRESULT Surface([in] BSTR newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = put_Surface(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_UserValue: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1943: [propget, id(DISPID_UserValue), helpstring("property User")] HRESULT UserValue([out, retval] VARIANT* pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				hr = get_UserValue(pVarResult);
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1944: [propput, id(DISPID_UserValue), helpstring("property User")] HRESULT UserValue([in] VARIANT* newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantCopyInd(&var0, &pDispParams->rgvarg[--index]);
				hr = put_UserValue(&var0);
				VariantClear(&var0);
			}
			break;
		}
		case 2144: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1946: [propget, id(2144), helpstring("property CloseAngle")] HRESULT CloseAngle([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_CloseAngle(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1947: [propput, id(2144), helpstring("property CloseAngle")] HRESULT CloseAngle([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_CloseAngle(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 2145: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1948: [propget, id(2145), helpstring("property OpenAngle")] HRESULT OpenAngle([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_OpenAngle(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1949: [propput, id(2145), helpstring("property OpenAngle")] HRESULT OpenAngle([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_OpenAngle(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 111: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1950: [propget, id(111), helpstring("property Collidable")] HRESULT Collidable([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_Collidable(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1951: [propput, id(111), helpstring("property Collidable")] HRESULT Collidable([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_Collidable(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 114: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1952: [propget, id(114), helpstring("property Friction")] HRESULT Friction([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Friction(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1953: [propput, id(114), helpstring("property Friction")] HRESULT Friction([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Friction(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 458: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1954: [propget, id(458), helpstring("property Visible")] HRESULT Visible([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_Visible(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1955: [propput, id(458), helpstring("property Visible")] HRESULT Visible([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_Visible(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 427: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1956: [propget, id(427), helpstring("property TwoWay")] HRESULT TwoWay([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_TwoWay(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1957: [propput, id(427), helpstring("property TwoWay")] HRESULT TwoWay([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_TwoWay(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 15: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1958: [propget, id(15), helpstring("property ShowBracket")] HRESULT ShowBracket([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_ShowBracket(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1959: [propput, id(15), helpstring("property ShowBracket")] HRESULT ShowBracket([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_ShowBracket(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 16: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1960: [propget, id(16), nonbrowsable, helpstring("property CurrentAngle")] HRESULT CurrentAngle([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_CurrentAngle(&V_R4(pVarResult));
			}
			break;
		}
		case 431: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1961: [propget, id(431), helpstring("property ReflectionEnabled")] HRESULT ReflectionEnabled([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_ReflectionEnabled(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1962: [propput, id(431), helpstring("property ReflectionEnabled")] HRESULT ReflectionEnabled([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_ReflectionEnabled(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 9: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1963: [propget, id(9), helpstring("property DrawStyle")] HRESULT DrawStyle([out, retval] GateType *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_DrawStyle((GateType*)&V_I4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1964: [propput, id(9), helpstring("property DrawStyle")] HRESULT DrawStyle([in] GateType newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_I4);
				hr = put_DrawStyle((GateType)V_I4(&var0));
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

STDMETHODIMP Gate::GetDocumentation(INT index, BSTR *pBstrName, BSTR *pBstrDocString, DWORD *pdwHelpContext, BSTR *pBstrHelpFile) {
	if (index == MEMBERID_NIL) {
		*pBstrName = SysAllocString(L"Gate");
		return S_OK;
	}
	return E_NOTIMPL;
}

disp_id_name_t Gate::m_idNameMap[] = {
		{ NULL },
		{ DISPID_GameEvents_Init, L"_Init" },
		{ DISPID_TimerEvents_Timer, L"_Timer" },
		{ DISPID_HitEvents_Hit, L"_Hit" },
		{ DISPID_LimitEvents_EOS, L"_LimitEOS" },
		{ DISPID_LimitEvents_BOS, L"_LimitBOS" },
		{ DISPID_AnimateEvents_Animate, L"_Animate" }
};

HRESULT Gate::FireDispID(const DISPID dispid, DISPPARAMS * const pdispparams) {
	static WCHAR wzName[MAXSTRING];
	CComPtr<IDispatch> disp;
	g_pplayer->m_ptable->m_pcv->m_pScript->GetScriptDispatch(nullptr, &disp);
	size_t min = 1, max = ARRAY_SIZE(m_idNameMap) - 1, i;
	int r;
	while(min <= max) {
		i = (min + max) / 2;
		if (m_idNameMap[i].dispId == dispid) {
			wcscpy(wzName, m_wzName);
			wcscat(wzName, m_idNameMap[i].name);
			LPOLESTR fnNames = (LPOLESTR)wzName;
			DISPID tDispid;
			if (SUCCEEDED(disp->GetIDsOfNames(IID_NULL, &fnNames, 1, 0, &tDispid))) {
				return disp->Invoke(tDispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, pdispparams, nullptr, nullptr, nullptr);
			}
			return DISP_E_MEMBERNOTFOUND;
		}
		else if (m_idNameMap[i].dispId < dispid)
		   min = i+1;
		else
		   max = i-1;
	}
	return DISP_E_MEMBERNOTFOUND;
}

name_disp_id_t Spinner::m_nameIDMap[] = {
		{ NULL },
		{ L"AngleMax", 13 },
		{ L"AngleMin", 14 },
		{ L"CurrentAngle", 18 },
		{ L"Damping", 7 },
		{ L"Elasticity", 110 },
		{ L"Height", 5 },
		{ L"Image", DISPID_Image },
		{ L"Length", DISPID_Spinner_Length },
		{ L"Material", 340 },
		{ L"Name", DISPID_Name },
		{ L"ReflectionEnabled", 431 },
		{ L"Rotation", 4 },
		{ L"ShowBracket", 108 },
		{ L"Surface", DISPID_Surface },
		{ L"TimerEnabled", DISPID_Timer_Enabled },
		{ L"TimerInterval", DISPID_Timer_Interval },
		{ L"UserValue", DISPID_UserValue },
		{ L"Visible", 458 },
		{ L"X", 11 },
		{ L"Y", 12 }
};

STDMETHODIMP Spinner::GetIDsOfNames(REFIID /*riid*/, LPOLESTR* rgszNames, UINT cNames, LCID lcid, DISPID* rgDispId) {
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

STDMETHODIMP Spinner::Invoke(DISPID dispIdMember, REFIID /*riid*/, LCID lcid, WORD wFlags, DISPPARAMS* pDispParams, VARIANT* pVarResult, EXCEPINFO* pExcepInfo, UINT* puArgErr) {
	HRESULT hr = DISP_E_UNKNOWNNAME;
	int index = pDispParams->cArgs;
	switch(dispIdMember) {
		case DISPID_Name: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1995: [propget, id(DISPID_Name), helpstring("property Name")] HRESULT Name([out, retval] BSTR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_Name(&V_BSTR(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1996: [propput, id(DISPID_Name), helpstring("property Name")] HRESULT Name([in] BSTR newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = put_Name(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Timer_Enabled: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1997: [propget, id(DISPID_Timer_Enabled), helpstring("property TimerEnabled")] HRESULT TimerEnabled([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_TimerEnabled(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 1998: [propput, id(DISPID_Timer_Enabled), helpstring("property TimerEnabled")] HRESULT TimerEnabled([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_TimerEnabled(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Timer_Interval: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 1999: [propget, id(DISPID_Timer_Interval), helpstring("property TimerInterval")] HRESULT TimerInterval([out, retval] long *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_TimerInterval((long*)&V_I4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2000: [propput, id(DISPID_Timer_Interval), helpstring("property TimerInterval")] HRESULT TimerInterval([in] long newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_I4);
				hr = put_TimerInterval(V_I4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Spinner_Length: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2001: [propget, id(DISPID_Spinner_Length), helpstring("property Length")] HRESULT Length([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Length(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2002: [propput, id(DISPID_Spinner_Length), helpstring("property Length")] HRESULT Length([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Length(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 4: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2003: [propget, id(4), helpstring("property Rotation")] HRESULT Rotation([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Rotation(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2004: [propput, id(4), helpstring("property Rotation")] HRESULT Rotation([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Rotation(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 5: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2005: [propget, id(5), helpstring("property Height")] HRESULT Height([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Height(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2006: [propput, id(5), helpstring("property Height")] HRESULT Height([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Height(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 7: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2007: [propget, id(7), helpstring("property Damping")] HRESULT Damping([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Damping(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2008: [propput, id(7), helpstring("property Damping")] HRESULT Damping([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Damping(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Image: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2009: [propget, id(DISPID_Image), helpstring("property Image")] HRESULT Image([out, retval] BSTR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_Image(&V_BSTR(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2010: [propput, id(DISPID_Image), helpstring("property Image")] HRESULT Image([in] BSTR newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = put_Image(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 340: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2011: [propget, id(340), helpstring("property Material")] HRESULT Material([out, retval] BSTR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_Material(&V_BSTR(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2012: [propput, id(340), helpstring("property Material")] HRESULT Material([in] BSTR newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = put_Material(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 11: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2013: [propget, id(11), helpstring("property X")] HRESULT X([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_X(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2014: [propput, id(11), helpstring("property X")] HRESULT X([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_X(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 12: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2015: [propget, id(12), helpstring("property Y")] HRESULT Y([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Y(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2016: [propput, id(12), helpstring("property Y")] HRESULT Y([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Y(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Surface: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2017: [propget, id(DISPID_Surface), helpstring("property Surface")] HRESULT Surface([out, retval] BSTR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_Surface(&V_BSTR(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2018: [propput, id(DISPID_Surface), helpstring("property Surface")] HRESULT Surface([in] BSTR newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = put_Surface(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_UserValue: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2019: [propget, id(DISPID_UserValue), helpstring("property User")] HRESULT UserValue([out, retval] VARIANT* pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				hr = get_UserValue(pVarResult);
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2020: [propput, id(DISPID_UserValue), helpstring("property User")] HRESULT UserValue([in] VARIANT* newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantCopyInd(&var0, &pDispParams->rgvarg[--index]);
				hr = put_UserValue(&var0);
				VariantClear(&var0);
			}
			break;
		}
		case 108: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2021: [propget, id(108), helpstring("property ShowBracket")] HRESULT ShowBracket([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_ShowBracket(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2022: [propput, id(108), helpstring("property ShowBracket")] HRESULT ShowBracket([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_ShowBracket(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 13: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2024: [propget, id(13), helpstring("property AngleMax")] HRESULT AngleMax([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_AngleMax(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2025: [propput, id(13), helpstring("property AngleMax")] HRESULT AngleMax([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_AngleMax(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 14: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2026: [propget, id(14), helpstring("property AngleMin")] HRESULT AngleMin([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_AngleMin(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2027: [propput, id(14), helpstring("property AngleMin")] HRESULT AngleMin([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_AngleMin(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 110: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2028: [propget, id(110), helpstring("property Elasticity")] HRESULT Elasticity([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Elasticity(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2029: [propput, id(110), helpstring("property Elasticity")] HRESULT Elasticity([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Elasticity(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 458: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2030: [propget, id(458), helpstring("property Visible")] HRESULT Visible([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_Visible(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2031: [propput, id(458), helpstring("property Visible")] HRESULT Visible([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_Visible(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 431: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2032: [propget, id(431), helpstring("property ReflectionEnabled")] HRESULT ReflectionEnabled([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_ReflectionEnabled(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2033: [propput, id(431), helpstring("property ReflectionEnabled")] HRESULT ReflectionEnabled([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_ReflectionEnabled(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 18: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2034: [propget, id(18), nonbrowsable, helpstring("property CurrentAngle")] HRESULT CurrentAngle([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_CurrentAngle(&V_R4(pVarResult));
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

STDMETHODIMP Spinner::GetDocumentation(INT index, BSTR *pBstrName, BSTR *pBstrDocString, DWORD *pdwHelpContext, BSTR *pBstrHelpFile) {
	if (index == MEMBERID_NIL) {
		*pBstrName = SysAllocString(L"Spinner");
		return S_OK;
	}
	return E_NOTIMPL;
}

disp_id_name_t Spinner::m_idNameMap[] = {
		{ NULL },
		{ DISPID_GameEvents_Init, L"_Init" },
		{ DISPID_TimerEvents_Timer, L"_Timer" },
		{ DISPID_SpinnerEvents_Spin, L"_Spin" },
		{ DISPID_LimitEvents_EOS, L"_LimitEOS" },
		{ DISPID_LimitEvents_BOS, L"_LimitBOS" },
		{ DISPID_AnimateEvents_Animate, L"_Animate" }
};

HRESULT Spinner::FireDispID(const DISPID dispid, DISPPARAMS * const pdispparams) {
	static WCHAR wzName[MAXSTRING];
	CComPtr<IDispatch> disp;
	g_pplayer->m_ptable->m_pcv->m_pScript->GetScriptDispatch(nullptr, &disp);
	size_t min = 1, max = ARRAY_SIZE(m_idNameMap) - 1, i;
	int r;
	while(min <= max) {
		i = (min + max) / 2;
		if (m_idNameMap[i].dispId == dispid) {
			wcscpy(wzName, m_wzName);
			wcscat(wzName, m_idNameMap[i].name);
			LPOLESTR fnNames = (LPOLESTR)wzName;
			DISPID tDispid;
			if (SUCCEEDED(disp->GetIDsOfNames(IID_NULL, &fnNames, 1, 0, &tDispid))) {
				return disp->Invoke(tDispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, pdispparams, nullptr, nullptr, nullptr);
			}
			return DISP_E_MEMBERNOTFOUND;
		}
		else if (m_idNameMap[i].dispId < dispid)
		   min = i+1;
		else
		   max = i-1;
	}
	return DISP_E_MEMBERNOTFOUND;
}

name_disp_id_t Ramp::m_nameIDMap[] = {
		{ NULL },
		{ L"Collidable", 111 },
		{ L"DepthBias", 397 },
		{ L"Elasticity", 110 },
		{ L"Friction", 114 },
		{ L"HasHitEvent", 34 },
		{ L"HasWallImage", 9 },
		{ L"HeightBottom", 1 },
		{ L"HeightTop", 2 },
		{ L"Image", DISPID_Image },
		{ L"ImageAlignment", 8 },
		{ L"LeftWallHeight", 10 },
		{ L"Material", 340 },
		{ L"Name", DISPID_Name },
		{ L"OverwritePhysics", 432 },
		{ L"PhysicsMaterial", 734 },
		{ L"ReflectionEnabled", 431 },
		{ L"RightWallHeight", 11 },
		{ L"Scatter", 115 },
		{ L"Threshold", 33 },
		{ L"TimerEnabled", DISPID_Timer_Enabled },
		{ L"TimerInterval", DISPID_Timer_Interval },
		{ L"Type", 6 },
		{ L"UserValue", DISPID_UserValue },
		{ L"Visible", 458 },
		{ L"VisibleLeftWallHeight", 108 },
		{ L"VisibleRightWallHeight", 109 },
		{ L"WidthBottom", 3 },
		{ L"WidthTop", 4 },
		{ L"WireDiameter", 377 },
		{ L"WireDistanceX", 398 },
		{ L"WireDistanceY", 425 }
};

STDMETHODIMP Ramp::GetIDsOfNames(REFIID /*riid*/, LPOLESTR* rgszNames, UINT cNames, LCID lcid, DISPID* rgDispId) {
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

STDMETHODIMP Ramp::Invoke(DISPID dispIdMember, REFIID /*riid*/, LCID lcid, WORD wFlags, DISPPARAMS* pDispParams, VARIANT* pVarResult, EXCEPINFO* pExcepInfo, UINT* puArgErr) {
	HRESULT hr = DISP_E_UNKNOWNNAME;
	int index = pDispParams->cArgs;
	switch(dispIdMember) {
		case DISPID_Name: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2065: [propget, id(DISPID_Name), helpstring("property Name")] HRESULT Name([out, retval] BSTR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_Name(&V_BSTR(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2066: [propput, id(DISPID_Name), helpstring("property Name")] HRESULT Name([in] BSTR newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = put_Name(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Timer_Enabled: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2067: [propget, id(DISPID_Timer_Enabled), helpstring("property TimerEnabled")] HRESULT TimerEnabled([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_TimerEnabled(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2068: [propput, id(DISPID_Timer_Enabled), helpstring("property TimerEnabled")] HRESULT TimerEnabled([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_TimerEnabled(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Timer_Interval: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2069: [propget, id(DISPID_Timer_Interval), helpstring("property TimerInterval")] HRESULT TimerInterval([out, retval] long *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_TimerInterval((long*)&V_I4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2070: [propput, id(DISPID_Timer_Interval), helpstring("property TimerInterval")] HRESULT TimerInterval([in] long newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_I4);
				hr = put_TimerInterval(V_I4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 1: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2071: [propget, id(1), helpstring("property HeightBottom")] HRESULT HeightBottom([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_HeightBottom(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2072: [propput, id(1), helpstring("property HeightBottom")] HRESULT HeightBottom([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_HeightBottom(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 2: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2073: [propget, id(2), helpstring("property HeightTop")] HRESULT HeightTop([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_HeightTop(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2074: [propput, id(2), helpstring("property HeightTop")] HRESULT HeightTop([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_HeightTop(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 3: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2075: [propget, id(3), helpstring("property WidthBottom")] HRESULT WidthBottom([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_WidthBottom(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2076: [propput, id(3), helpstring("property WidthBottom")] HRESULT WidthBottom([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_WidthBottom(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 4: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2077: [propget, id(4), helpstring("property WidthTop")] HRESULT WidthTop([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_WidthTop(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2078: [propput, id(4), helpstring("property WidthTop")] HRESULT WidthTop([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_WidthTop(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 340: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2079: [propget, id(340), helpstring("property Material")] HRESULT Material([out, retval] BSTR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_Material(&V_BSTR(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2080: [propput, id(340), helpstring("property Material")] HRESULT Material([in] BSTR newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = put_Material(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 6: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2081: [propget, id(6), helpstring("property Type")] HRESULT Type([out, retval] RampType *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_Type((RampType*)&V_I4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2082: [propput, id(6), helpstring("property Type")] HRESULT Type([in] RampType newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_I4);
				hr = put_Type((RampType)V_I4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Image: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2083: [propget, id(DISPID_Image), helpstring("property Image")] HRESULT Image([out, retval] BSTR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_Image(&V_BSTR(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2084: [propput, id(DISPID_Image), helpstring("property Image")] HRESULT Image([in] BSTR newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = put_Image(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 8: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2085: [propget, id(8), helpstring("property ImageAlignment")] HRESULT ImageAlignment([out, retval] RampImageAlignment *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_ImageAlignment((RampImageAlignment*)&V_I4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2086: [propput, id(8), helpstring("property ImageAlignment")] HRESULT ImageAlignment([in] RampImageAlignment newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_I4);
				hr = put_ImageAlignment((RampImageAlignment)V_I4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 9: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2087: [propget, id(9), helpstring("property HasWallImage")] HRESULT HasWallImage([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_HasWallImage(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2088: [propput, id(9), helpstring("property HasWallImage")] HRESULT HasWallImage([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_HasWallImage(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 10: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2089: [propget, id(10), helpstring("property LeftWallHeight")] HRESULT LeftWallHeight([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_LeftWallHeight(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2090: [propput, id(10), helpstring("property LeftWallHeight")] HRESULT LeftWallHeight([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_LeftWallHeight(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 11: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2091: [propget, id(11), helpstring("property RightWallHeight")] HRESULT RightWallHeight([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_RightWallHeight(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2092: [propput, id(11), helpstring("property RightWallHeight")] HRESULT RightWallHeight([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_RightWallHeight(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_UserValue: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2093: [propget, id(DISPID_UserValue), helpstring("property User")] HRESULT UserValue([out, retval] VARIANT* pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				hr = get_UserValue(pVarResult);
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2094: [propput, id(DISPID_UserValue), helpstring("property User")] HRESULT UserValue([in] VARIANT* newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantCopyInd(&var0, &pDispParams->rgvarg[--index]);
				hr = put_UserValue(&var0);
				VariantClear(&var0);
			}
			break;
		}
		case 108: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2095: [propget, id(108), helpstring("property VisibleLeftWallHeight")] HRESULT VisibleLeftWallHeight([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_VisibleLeftWallHeight(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2096: [propput, id(108), helpstring("property VisibleLeftWallHeight")] HRESULT VisibleLeftWallHeight([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_VisibleLeftWallHeight(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 109: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2097: [propget, id(109), helpstring("property VisibleRightWallHeight")] HRESULT VisibleRightWallHeight([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_VisibleRightWallHeight(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2098: [propput, id(109), helpstring("property VisibleRightWallHeight")] HRESULT VisibleRightWallHeight([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_VisibleRightWallHeight(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 110: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2099: [propget, id(110), helpstring("property Elasticity")] HRESULT Elasticity([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Elasticity(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2100: [propput, id(110), helpstring("property Elasticity")] HRESULT Elasticity([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Elasticity(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 111: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2102: [propget, id(111), helpstring("property Collidable")] HRESULT Collidable([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_Collidable(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2103: [propput, id(111), helpstring("property Collidable")] HRESULT Collidable([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_Collidable(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 34: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2104: [propget, id(34), helpstring("property HasHitEvent")] HRESULT HasHitEvent([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_HasHitEvent(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2105: [propput, id(34), helpstring("property HasHitEvent")] HRESULT HasHitEvent([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_HasHitEvent(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 33: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2106: [propget, id(33), helpstring("property Threshold")] HRESULT Threshold([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Threshold(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2107: [propput, id(33), helpstring("property Threshold")] HRESULT Threshold([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Threshold(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 458: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2109: [propget, id(458), helpstring("property Visible")] HRESULT Visible([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_Visible(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2110: [propput, id(458), helpstring("property Visible")] HRESULT Visible([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_Visible(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 114: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2112: [propget, id(114), helpstring("property Friction")] HRESULT Friction([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Friction(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2113: [propput, id(114), helpstring("property Friction")] HRESULT Friction([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Friction(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 115: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2114: [propget, id(115), helpstring("property Scatter")] HRESULT Scatter([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Scatter(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2115: [propput, id(115), helpstring("property Scatter")] HRESULT Scatter([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Scatter(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 397: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2117: [propget, id(397), helpstring("property DepthBias")] HRESULT DepthBias([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_DepthBias(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2118: [propput, id(397), helpstring("property DepthBias")] HRESULT DepthBias([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_DepthBias(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 377: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2120: [propget, id(377), helpstring("property WireDiameter")] HRESULT WireDiameter([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_WireDiameter(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2121: [propput, id(377), helpstring("property WireDiameter")] HRESULT WireDiameter([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_WireDiameter(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 398: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2123: [propget, id(398), helpstring("property WireDistanceX")] HRESULT WireDistanceX([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_WireDistanceX(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2124: [propput, id(398), helpstring("property WireDistanceX")] HRESULT WireDistanceX([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_WireDistanceX(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 425: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2126: [propget, id(425), helpstring("property WireDistanceY")] HRESULT WireDistanceY([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_WireDistanceY(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2127: [propput, id(425), helpstring("property WireDistanceY")] HRESULT WireDistanceY([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_WireDistanceY(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 431: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2129: [propget, id(431), helpstring("property ReflectionEnabled")] HRESULT ReflectionEnabled([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_ReflectionEnabled(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2130: [propput, id(431), helpstring("property ReflectionEnabled")] HRESULT ReflectionEnabled([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_ReflectionEnabled(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 734: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2132: [propget, id(734), helpstring("property PhysicsMaterial")] HRESULT PhysicsMaterial([out, retval] BSTR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_PhysicsMaterial(&V_BSTR(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2133: [propput, id(734), helpstring("property PhysicsMaterial")] HRESULT PhysicsMaterial([in] BSTR newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = put_PhysicsMaterial(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 432: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2134: [propget, id(432), helpstring("property OverwritePhysics")] HRESULT OverwritePhysics([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_OverwritePhysics(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2135: [propput, id(432), helpstring("property OverwritePhysics")] HRESULT OverwritePhysics([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_OverwritePhysics(V_BOOL(&var0));
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

STDMETHODIMP Ramp::GetDocumentation(INT index, BSTR *pBstrName, BSTR *pBstrDocString, DWORD *pdwHelpContext, BSTR *pBstrHelpFile) {
	if (index == MEMBERID_NIL) {
		*pBstrName = SysAllocString(L"Ramp");
		return S_OK;
	}
	return E_NOTIMPL;
}

name_disp_id_t Flasher::m_nameIDMap[] = {
		{ NULL },
		{ L"AddBlend", 556 },
		{ L"Amount", 379 },
		{ L"Color", 11 },
		{ L"DepthBias", 397 },
		{ L"DisplayTexture", 13 },
		{ L"DMD", 557 },
		{ L"DMDColoredPixels", 47 },
		{ L"DMDHeight", 45 },
		{ L"DMDPixels", 46 },
		{ L"DMDWidth", 44 },
		{ L"Filter", 32996 },
		{ L"Height", 378 },
		{ L"ImageA", DISPID_Image },
		{ L"ImageAlignment", 8 },
		{ L"ImageB", DISPID_Image2 },
		{ L"IntensityScale", 435 },
		{ L"ModulateVsAdd", 433 },
		{ L"Name", DISPID_Name },
		{ L"Opacity", 377 },
		{ L"RotX", 9 },
		{ L"RotY", 2 },
		{ L"RotZ", 1 },
		{ L"TimerEnabled", DISPID_Timer_Enabled },
		{ L"TimerInterval", DISPID_Timer_Interval },
		{ L"UserValue", DISPID_UserValue },
		{ L"VideoCapHeight", 561 },
		{ L"VideoCapUpdate", 562 },
		{ L"VideoCapWidth", 560 },
		{ L"Visible", 458 },
		{ L"X", 5 },
		{ L"Y", 6 }
};

STDMETHODIMP Flasher::GetIDsOfNames(REFIID /*riid*/, LPOLESTR* rgszNames, UINT cNames, LCID lcid, DISPID* rgDispId) {
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

STDMETHODIMP Flasher::Invoke(DISPID dispIdMember, REFIID /*riid*/, LCID lcid, WORD wFlags, DISPPARAMS* pDispParams, VARIANT* pVarResult, EXCEPINFO* pExcepInfo, UINT* puArgErr) {
	HRESULT hr = DISP_E_UNKNOWNNAME;
	int index = pDispParams->cArgs;
	switch(dispIdMember) {
		case DISPID_Name: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2151: [propget, id(DISPID_Name), helpstring("property Name")] HRESULT Name([out, retval] BSTR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_Name(&V_BSTR(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2152: [propput, id(DISPID_Name), helpstring("property Name")] HRESULT Name([in] BSTR newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = put_Name(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 5: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2153: [propget, id(5), helpstring("property X")] HRESULT X([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_X(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2154: [propput, id(5), helpstring("property X")] HRESULT X([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_X(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 6: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2155: [propget, id(6), helpstring("property Y")] HRESULT Y([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Y(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2156: [propput, id(6), helpstring("property Y")] HRESULT Y([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Y(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 8: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2157: [propget, id(8), helpstring("property ImageAlignment")] HRESULT ImageAlignment([out, retval] RampImageAlignment *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_ImageAlignment((RampImageAlignment*)&V_I4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2158: [propput, id(8), helpstring("property ImageAlignment")] HRESULT ImageAlignment([in] RampImageAlignment newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_I4);
				hr = put_ImageAlignment((RampImageAlignment)V_I4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 378: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2159: [propget, id(378), helpstring("property Height")] HRESULT Height([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Height(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2160: [propput, id(378), helpstring("property Height")] HRESULT Height([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Height(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 1: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2161: [propget, id(1), helpstring("property RotZ")] HRESULT RotZ([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_RotZ(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2162: [propput, id(1), helpstring("property RotZ")] HRESULT RotZ([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_RotZ(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 2: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2163: [propget, id(2), helpstring("property RotY")] HRESULT RotY([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_RotY(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2164: [propput, id(2), helpstring("property RotY")] HRESULT RotY([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_RotY(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 9: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2165: [propget, id(9), helpstring("property RotX")] HRESULT RotX([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_RotX(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2166: [propput, id(9), helpstring("property RotX")] HRESULT RotX([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_RotX(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 11: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2167: [propget, id(11), helpstring("property Color")] HRESULT Color([out, retval] OLE_COLOR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_UI4;
				hr = get_Color(&V_UI4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2168: [propput, id(11), helpstring("property Color")] HRESULT Color([in] OLE_COLOR newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_UI4);
				hr = put_Color((OLE_COLOR)V_UI4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Image: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2169: [propget, id(DISPID_Image), helpstring("property ImageA")] HRESULT ImageA([out, retval] BSTR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_ImageA(&V_BSTR(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2170: [propput, id(DISPID_Image), helpstring("property ImageA")] HRESULT ImageA([in] BSTR newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = put_ImageA(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Image2: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2171: [propget, id(DISPID_Image2), helpstring("property ImageB")] HRESULT ImageB([out, retval] BSTR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_ImageB(&V_BSTR(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2172: [propput, id(DISPID_Image2), helpstring("property ImageB")] HRESULT ImageB([in] BSTR newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = put_ImageB(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 13: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2173: [propget, id(13), helpstring("property DisplayTexture")] HRESULT DisplayTexture([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_DisplayTexture(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2174: [propput, id(13), helpstring("property DisplayTexture")] HRESULT DisplayTexture([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_DisplayTexture(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 377: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2175: [propget, id(377), helpstring("property Opacity")] HRESULT Opacity([out, retval] long *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_Opacity((long*)&V_I4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2176: [propput, id(377), helpstring("property Opacity")] HRESULT Opacity([in] long newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_I4);
				hr = put_Opacity(V_I4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 435: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2177: [propget, id(435), helpstring("property IntensityScale")] HRESULT IntensityScale([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_IntensityScale(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2178: [propput, id(435), helpstring("property IntensityScale")] HRESULT IntensityScale([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_IntensityScale(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 433: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2179: [propget, id(433), helpstring("property ModulateVsAdd")] HRESULT ModulateVsAdd([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_ModulateVsAdd(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2180: [propput, id(433), helpstring("property ModulateVsAdd")] HRESULT ModulateVsAdd([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_ModulateVsAdd(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_UserValue: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2181: [propget, id(DISPID_UserValue), helpstring("property User")] HRESULT UserValue([out, retval] VARIANT* pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				hr = get_UserValue(pVarResult);
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2182: [propput, id(DISPID_UserValue), helpstring("property User")] HRESULT UserValue([in] VARIANT* newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantCopyInd(&var0, &pDispParams->rgvarg[--index]);
				hr = put_UserValue(&var0);
				VariantClear(&var0);
			}
			break;
		}
		case 458: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2184: [propget, id(458), helpstring("property Visible")] HRESULT Visible([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_Visible(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2185: [propput, id(458), helpstring("property Visible")] HRESULT Visible([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_Visible(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 556: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2186: [propget, id(556), helpstring("property AddBlend")] HRESULT AddBlend([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_AddBlend(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2187: [propput, id(556), helpstring("property AddBlend")] HRESULT AddBlend([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_AddBlend(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 557: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2189: [propget, id(557), helpstring("property DMD")] HRESULT DMD([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_DMD(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2190: [propput, id(557), helpstring("property DMD")] HRESULT DMD([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_DMD(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 44: {
			if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2191: [propput, id(44), helpstring("property DMDWidth")] HRESULT DMDWidth([in] int pVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_I4);
				hr = put_DMDWidth(V_I4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 45: {
			if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2192: [propput, id(45), helpstring("property DMDHeight")] HRESULT DMDHeight([in] int pVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_I4);
				hr = put_DMDHeight(V_I4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 46: {
			if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2193: [propput, id(46), helpstring("property DMDPixels")] HRESULT DMDPixels([in] VARIANT pVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantCopyInd(&var0, &pDispParams->rgvarg[--index]);
				hr = put_DMDPixels(var0);
				VariantClear(&var0);
			}
			break;
		}
		case 47: {
			if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2194: [propput, id(47), helpstring("property DMDColoredPixels")] HRESULT DMDColoredPixels([in] VARIANT pVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantCopyInd(&var0, &pDispParams->rgvarg[--index]);
				hr = put_DMDColoredPixels(var0);
				VariantClear(&var0);
			}
			break;
		}
		case 560: {
			if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2196: [propput, id(560), helpstring("property VideoCapWidth")] HRESULT VideoCapWidth([in] long cWidth);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_I4);
				hr = put_VideoCapWidth(V_I4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 561: {
			if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2197: [propput, id(561), helpstring("property VideoCapHeight")] HRESULT VideoCapHeight([in] long cHeight);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_I4);
				hr = put_VideoCapHeight(V_I4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 562: {
			if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2198: [propput, id(562), helpstring("property VideoCapUpdate")] HRESULT VideoCapUpdate([in] BSTR cWinTitle);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = put_VideoCapUpdate(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 397: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2200: [propget, id(397), helpstring("property DepthBias")] HRESULT DepthBias([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_DepthBias(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2201: [propput, id(397), helpstring("property DepthBias")] HRESULT DepthBias([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_DepthBias(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 32996: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2202: [propget, id(32996), helpstring("property Filter")] HRESULT Filter([out, retval] BSTR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_Filter(&V_BSTR(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2203: [propput, id(32996), helpstring("property Filter")] HRESULT Filter([in] BSTR newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = put_Filter(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 379: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2204: [propget, id(379), helpstring("property Amount")] HRESULT Amount([out, retval] long *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_Amount((long*)&V_I4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2205: [propput, id(379), helpstring("property Amount")] HRESULT Amount([in] long newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_I4);
				hr = put_Amount(V_I4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Timer_Enabled: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2206: [propget, id(DISPID_Timer_Enabled), helpstring("property TimerEnabled")] HRESULT TimerEnabled([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_TimerEnabled(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2207: [propput, id(DISPID_Timer_Enabled), helpstring("property TimerEnabled")] HRESULT TimerEnabled([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_TimerEnabled(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Timer_Interval: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2208: [propget, id(DISPID_Timer_Interval), helpstring("property TimerInterval")] HRESULT TimerInterval([out, retval] long *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_TimerInterval((long*)&V_I4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2209: [propput, id(DISPID_Timer_Interval), helpstring("property TimerInterval")] HRESULT TimerInterval([in] long newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_I4);
				hr = put_TimerInterval(V_I4(&var0));
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

STDMETHODIMP Flasher::GetDocumentation(INT index, BSTR *pBstrName, BSTR *pBstrDocString, DWORD *pdwHelpContext, BSTR *pBstrHelpFile) {
	if (index == MEMBERID_NIL) {
		*pBstrName = SysAllocString(L"Flasher");
		return S_OK;
	}
	return E_NOTIMPL;
}

name_disp_id_t Rubber::m_nameIDMap[] = {
		{ NULL },
		{ L"Collidable", 111 },
		{ L"Elasticity", 110 },
		{ L"ElasticityFalloff", 120 },
		{ L"EnableShowInEditor", 479 },
		{ L"EnableStaticRendering", 398 },
		{ L"Friction", 114 },
		{ L"HasHitEvent", 34 },
		{ L"Height", 2 },
		{ L"HitHeight", 116 },
		{ L"Image", DISPID_Image },
		{ L"Material", 340 },
		{ L"Name", DISPID_Name },
		{ L"OverwritePhysics", 432 },
		{ L"PhysicsMaterial", 734 },
		{ L"ReflectionEnabled", 431 },
		{ L"RotX", 18 },
		{ L"RotY", 24 },
		{ L"RotZ", 25 },
		{ L"Scatter", 115 },
		{ L"Thickness", 4 },
		{ L"TimerEnabled", DISPID_Timer_Enabled },
		{ L"TimerInterval", DISPID_Timer_Interval },
		{ L"UserValue", DISPID_UserValue },
		{ L"Visible", 458 }
};

STDMETHODIMP Rubber::GetIDsOfNames(REFIID /*riid*/, LPOLESTR* rgszNames, UINT cNames, LCID lcid, DISPID* rgDispId) {
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

STDMETHODIMP Rubber::Invoke(DISPID dispIdMember, REFIID /*riid*/, LCID lcid, WORD wFlags, DISPPARAMS* pDispParams, VARIANT* pVarResult, EXCEPINFO* pExcepInfo, UINT* puArgErr) {
	HRESULT hr = DISP_E_UNKNOWNNAME;
	int index = pDispParams->cArgs;
	switch(dispIdMember) {
		case DISPID_Name: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2225: [propget, id(DISPID_Name), helpstring("property Name")] HRESULT Name([out, retval] BSTR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_Name(&V_BSTR(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2226: [propput, id(DISPID_Name), helpstring("property Name")] HRESULT Name([in] BSTR newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = put_Name(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 2: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2227: [propget, id(2), helpstring("property Height")] HRESULT Height([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Height(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2228: [propput, id(2), helpstring("property Height")] HRESULT Height([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Height(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 116: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2229: [propget, id(116), helpstring("property HitHeight")] HRESULT HitHeight([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_HitHeight(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2230: [propput, id(116), helpstring("property HitHeight")] HRESULT HitHeight([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_HitHeight(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 34: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2231: [propget, id(34), helpstring("property HitEvent")] HRESULT HasHitEvent([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_HasHitEvent(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2232: [propput, id(34), helpstring("property HitEvent")] HRESULT HasHitEvent([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_HasHitEvent(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 4: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2233: [propget, id(4), helpstring("property Thickness")] HRESULT Thickness([out, retval] int *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_Thickness((int*)&V_I4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2234: [propput, id(4), helpstring("property Thickness")] HRESULT Thickness([in] int newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_I4);
				hr = put_Thickness(V_I4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 18: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2235: [propget, id(18), helpstring("property RotX")] HRESULT RotX([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_RotX(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2236: [propput, id(18), helpstring("property RotX")] HRESULT RotX([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_RotX(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 25: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2237: [propget, id(25), helpstring("property RotZ")] HRESULT RotZ([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_RotZ(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2238: [propput, id(25), helpstring("property RotZ")] HRESULT RotZ([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_RotZ(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 24: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2239: [propget, id(24), helpstring("property RotY")] HRESULT RotY([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_RotY(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2240: [propput, id(24), helpstring("property RotY")] HRESULT RotY([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_RotY(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 340: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2241: [propget, id(340), helpstring("property Material")] HRESULT Material([out, retval] BSTR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_Material(&V_BSTR(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2242: [propput, id(340), helpstring("property Material")] HRESULT Material([in] BSTR newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = put_Material(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Image: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2243: [propget, id(DISPID_Image), helpstring("property Image")] HRESULT Image([out, retval] BSTR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_Image(&V_BSTR(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2244: [propput, id(DISPID_Image), helpstring("property Image")] HRESULT Image([in] BSTR newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = put_Image(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 110: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2245: [propget, id(110), helpstring("property Elasticity")] HRESULT Elasticity([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Elasticity(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2246: [propput, id(110), helpstring("property Elasticity")] HRESULT Elasticity([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Elasticity(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 120: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2247: [propget, id(120), helpstring("property ElasticityFalloff")] HRESULT ElasticityFalloff([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_ElasticityFalloff(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2248: [propput, id(120), helpstring("property ElasticityFalloff")] HRESULT ElasticityFalloff([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_ElasticityFalloff(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 111: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2250: [propget, id(111), helpstring("property Collidable")] HRESULT Collidable([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_Collidable(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2251: [propput, id(111), helpstring("property Collidable")] HRESULT Collidable([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_Collidable(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 458: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2253: [propget, id(458), helpstring("property Visible")] HRESULT Visible([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_Visible(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2254: [propput, id(458), helpstring("property Visible")] HRESULT Visible([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_Visible(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 114: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2256: [propget, id(114), helpstring("property Friction")] HRESULT Friction([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Friction(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2257: [propput, id(114), helpstring("property Friction")] HRESULT Friction([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Friction(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 115: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2258: [propget, id(115), helpstring("property Scatter")] HRESULT Scatter([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Scatter(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2259: [propput, id(115), helpstring("property Scatter")] HRESULT Scatter([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Scatter(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 398: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2261: [propget, id(398), helpstring("property static rendering")] HRESULT EnableStaticRendering([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_EnableStaticRendering(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2262: [propput, id(398), helpstring("property static rendering")] HRESULT EnableStaticRendering([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_EnableStaticRendering(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 479: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2264: [propget, id(479), helpstring("property show in editor")] HRESULT EnableShowInEditor([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_EnableShowInEditor(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2265: [propput, id(479), helpstring("property show in editor")] HRESULT EnableShowInEditor([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_EnableShowInEditor(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 431: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2267: [propget, id(431), helpstring("property ReflectionEnabled")] HRESULT ReflectionEnabled([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_ReflectionEnabled(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2268: [propput, id(431), helpstring("property ReflectionEnabled")] HRESULT ReflectionEnabled([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_ReflectionEnabled(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Timer_Enabled: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2270: [propget, id(DISPID_Timer_Enabled), helpstring("property TimerEnabled")] HRESULT TimerEnabled([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_TimerEnabled(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2271: [propput, id(DISPID_Timer_Enabled), helpstring("property TimerEnabled")] HRESULT TimerEnabled([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_TimerEnabled(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Timer_Interval: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2272: [propget, id(DISPID_Timer_Interval), helpstring("property TimerInterval")] HRESULT TimerInterval([out, retval] long *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_TimerInterval((long*)&V_I4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2273: [propput, id(DISPID_Timer_Interval), helpstring("property TimerInterval")] HRESULT TimerInterval([in] long newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_I4);
				hr = put_TimerInterval(V_I4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_UserValue: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2274: [propget, id(DISPID_UserValue), helpstring("property User")] HRESULT UserValue([out, retval] VARIANT* pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				hr = get_UserValue(pVarResult);
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2275: [propput, id(DISPID_UserValue), helpstring("property User")] HRESULT UserValue([in] VARIANT* newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantCopyInd(&var0, &pDispParams->rgvarg[--index]);
				hr = put_UserValue(&var0);
				VariantClear(&var0);
			}
			break;
		}
		case 734: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2277: [propget, id(734), helpstring("property PhysicsMaterial")] HRESULT PhysicsMaterial([out, retval] BSTR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_PhysicsMaterial(&V_BSTR(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2278: [propput, id(734), helpstring("property PhysicsMaterial")] HRESULT PhysicsMaterial([in] BSTR newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = put_PhysicsMaterial(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 432: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2279: [propget, id(432), helpstring("property OverwritePhysics")] HRESULT OverwritePhysics([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_OverwritePhysics(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2280: [propput, id(432), helpstring("property OverwritePhysics")] HRESULT OverwritePhysics([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_OverwritePhysics(V_BOOL(&var0));
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

STDMETHODIMP Rubber::GetDocumentation(INT index, BSTR *pBstrName, BSTR *pBstrDocString, DWORD *pdwHelpContext, BSTR *pBstrHelpFile) {
	if (index == MEMBERID_NIL) {
		*pBstrName = SysAllocString(L"Rubber");
		return S_OK;
	}
	return E_NOTIMPL;
}

name_disp_id_t BallEx::m_nameIDMap[] = {
		{ NULL },
		{ L"AngMomX", 17 },
		{ L"AngMomY", 18 },
		{ L"AngMomZ", 19 },
		{ L"AngVelX", 14 },
		{ L"AngVelY", 15 },
		{ L"AngVelZ", 16 },
		{ L"BulbIntensityScale", 451 },
		{ L"Color", 7 },
		{ L"DecalMode", 497 },
		{ L"DestroyBall", 100 },
		{ L"ForceReflection", 486 },
		{ L"FrontDecal", 9 },
		{ L"ID", 13 },
		{ L"Image", 8 },
		{ L"Mass", 11 },
		{ L"Name", DISPID_Name },
		{ L"PlayfieldReflectionScale", 485 },
		{ L"Radius", 12 },
		{ L"ReflectionEnabled", 484 },
		{ L"UserValue", DISPID_UserValue },
		{ L"VelX", 3 },
		{ L"VelY", 4 },
		{ L"VelZ", 6 },
		{ L"Visible", 487 },
		{ L"X", 1 },
		{ L"Y", 2 },
		{ L"Z", 5 }
};

STDMETHODIMP BallEx::GetIDsOfNames(REFIID /*riid*/, LPOLESTR* rgszNames, UINT cNames, LCID lcid, DISPID* rgDispId) {
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

STDMETHODIMP BallEx::Invoke(DISPID dispIdMember, REFIID /*riid*/, LCID lcid, WORD wFlags, DISPPARAMS* pDispParams, VARIANT* pVarResult, EXCEPINFO* pExcepInfo, UINT* puArgErr) {
	HRESULT hr = DISP_E_UNKNOWNNAME;
	int index = pDispParams->cArgs;
	switch(dispIdMember) {
		case 1: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2296: [propget, id(1), helpstring("property X")] HRESULT X([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_X(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2297: [propput, id(1), helpstring("property X")] HRESULT X([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_X(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 2: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2298: [propget, id(2), helpstring("property Y")] HRESULT Y([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Y(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2299: [propput, id(2), helpstring("property Y")] HRESULT Y([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Y(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 3: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2300: [propget, id(3), helpstring("property VelX")] HRESULT VelX([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_VelX(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2301: [propput, id(3), helpstring("property VelX")] HRESULT VelX([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_VelX(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 4: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2302: [propget, id(4), helpstring("property VelY")] HRESULT VelY([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_VelY(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2303: [propput, id(4), helpstring("property VelY")] HRESULT VelY([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_VelY(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 5: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2304: [propget, id(5), helpstring("property Z")] HRESULT Z([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Z(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2305: [propput, id(5), helpstring("property Z")] HRESULT Z([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Z(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 6: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2306: [propget, id(6), helpstring("property VelZ")] HRESULT VelZ([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_VelZ(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2307: [propput, id(6), helpstring("property VelZ")] HRESULT VelZ([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_VelZ(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 14: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2308: [propget, id(14), helpstring("property AngVelX")] HRESULT AngVelX([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_AngVelX(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2309: [propput, id(14), helpstring("property AngVelX")] HRESULT AngVelX([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_AngVelX(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 15: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2310: [propget, id(15), helpstring("property AngVelY")] HRESULT AngVelY([out, retval] float* pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_AngVelY(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2311: [propput, id(15), helpstring("property AngVelY")] HRESULT AngVelY([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_AngVelY(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 16: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2312: [propget, id(16), helpstring("property AngVelZ")] HRESULT AngVelZ([out, retval] float* pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_AngVelZ(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2313: [propput, id(16), helpstring("property AngVelZ")] HRESULT AngVelZ([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_AngVelZ(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 17: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2314: [propget, id(17), helpstring("property AngMomX")] HRESULT AngMomX([out, retval] float* pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_AngMomX(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2315: [propput, id(17), helpstring("property AngMomX")] HRESULT AngMomX([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_AngMomX(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 18: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2316: [propget, id(18), helpstring("property AngMomY")] HRESULT AngMomY([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_AngMomY(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2317: [propput, id(18), helpstring("property AngMomY")] HRESULT AngMomY([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_AngMomY(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 19: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2318: [propget, id(19), helpstring("property AngMomZ")] HRESULT AngMomZ([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_AngMomZ(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2319: [propput, id(19), helpstring("property AngMomZ")] HRESULT AngMomZ([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_AngMomZ(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 7: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2321: [propget, id(7), helpstring("property Color")] HRESULT Color([out, retval] OLE_COLOR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_UI4;
				hr = get_Color(&V_UI4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2322: [propput, id(7), helpstring("property Color")] HRESULT Color([in] OLE_COLOR newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_UI4);
				hr = put_Color((OLE_COLOR)V_UI4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 8: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2323: [propget, id(8), helpstring("property Image")] HRESULT Image([out, retval] BSTR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_Image(&V_BSTR(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2324: [propput, id(8), helpstring("property Image")] HRESULT Image([in] BSTR newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = put_Image(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 9: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2325: [propget, id(9), helpstring("property FrontDecal")] HRESULT FrontDecal([out, retval] BSTR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_FrontDecal(&V_BSTR(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2326: [propput, id(9), helpstring("property FrontDecal")] HRESULT FrontDecal([in] BSTR newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = put_FrontDecal(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 497: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2327: [propget, id(497), helpstring("property DecalMode")] HRESULT DecalMode([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_DecalMode(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2328: [propput, id(497), helpstring("property DecalMode")] HRESULT DecalMode([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_DecalMode(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_UserValue: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2329: [propget, id(DISPID_UserValue), helpstring("property User")] HRESULT UserValue([out, retval] VARIANT* pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				hr = get_UserValue(pVarResult);
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2330: [propput, id(DISPID_UserValue), helpstring("property User")] HRESULT UserValue([in] VARIANT* newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantCopyInd(&var0, &pDispParams->rgvarg[--index]);
				hr = put_UserValue(&var0);
				VariantClear(&var0);
			}
			break;
		}
		case 11: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2331: [propget, id(11), helpstring("property Mass")] HRESULT Mass([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Mass(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2332: [propput, id(11), helpstring("property Mass")] HRESULT Mass([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Mass(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 12: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2333: [propget, id(12), helpstring("property Radius")] HRESULT Radius([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Radius(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2334: [propput, id(12), helpstring("property Radius")] HRESULT Radius([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Radius(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 13: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2335: [propget, id(13), helpstring("property ID")] HRESULT ID([out, retval] int *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_ID((int*)&V_I4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2336: [propput, id(13), helpstring("property ID")] HRESULT ID([in] int newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_I4);
				hr = put_ID(V_I4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Name: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2337: [propget, id(DISPID_Name), helpstring("property Name")] HRESULT Name([out, retval] BSTR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_Name(&V_BSTR(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2338: [propput, id(DISPID_Name), helpstring("property Name")] HRESULT Name([in] BSTR newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = put_Name(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 451: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2339: [propget, id(451), helpstring("property BulbIntensityScale")] HRESULT BulbIntensityScale([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_BulbIntensityScale(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2340: [propput, id(451), helpstring("property BulbIntensityScale")] HRESULT BulbIntensityScale([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_BulbIntensityScale(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 484: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2341: [propget, id(484), helpstring("property ReflectionEnabled")] HRESULT ReflectionEnabled([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_ReflectionEnabled(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2342: [propput, id(484), helpstring("property ReflectionEnabled")] HRESULT ReflectionEnabled([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_ReflectionEnabled(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 485: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2343: [propget, id(485), helpstring("property PlayfieldReflectionScale")] HRESULT PlayfieldReflectionScale([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_PlayfieldReflectionScale(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2344: [propput, id(485), helpstring("property PlayfieldReflectionScale")] HRESULT PlayfieldReflectionScale([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_PlayfieldReflectionScale(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 486: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2345: [propget, id(486), helpstring("property ForceReflection")] HRESULT ForceReflection([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_ForceReflection(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2346: [propput, id(486), helpstring("property ForceReflection")] HRESULT ForceReflection([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_ForceReflection(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 487: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2347: [propget, id(487), helpstring("property Visible")] HRESULT Visible([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_Visible(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2348: [propput, id(487), helpstring("property Visible")] HRESULT Visible([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_Visible(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 100: {
			if (wFlags & DISPATCH_METHOD) {
				// line 2349: [id(100), helpstring("method DestroyBall")] HRESULT DestroyBall([out, retval] int *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = DestroyBall((int*)&V_I4(pVarResult));
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

STDMETHODIMP BallEx::GetDocumentation(INT index, BSTR *pBstrName, BSTR *pBstrDocString, DWORD *pdwHelpContext, BSTR *pBstrHelpFile) {
	if (index == MEMBERID_NIL) {
		*pBstrName = SysAllocString(L"IBall");
		return S_OK;
	}
	return E_NOTIMPL;
}

disp_id_name_t Ramp::m_idNameMap[] = {
		{ NULL },
		{ DISPID_GameEvents_Init, L"_Init" }
};

HRESULT Ramp::FireDispID(const DISPID dispid, DISPPARAMS * const pdispparams) {
	static WCHAR wzName[MAXSTRING];
	CComPtr<IDispatch> disp;
	g_pplayer->m_ptable->m_pcv->m_pScript->GetScriptDispatch(nullptr, &disp);
	size_t min = 1, max = ARRAY_SIZE(m_idNameMap) - 1, i;
	int r;
	while(min <= max) {
		i = (min + max) / 2;
		if (m_idNameMap[i].dispId == dispid) {
			wcscpy(wzName, m_wzName);
			wcscat(wzName, m_idNameMap[i].name);
			LPOLESTR fnNames = (LPOLESTR)wzName;
			DISPID tDispid;
			if (SUCCEEDED(disp->GetIDsOfNames(IID_NULL, &fnNames, 1, 0, &tDispid))) {
				return disp->Invoke(tDispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, pdispparams, nullptr, nullptr, nullptr);
			}
			return DISP_E_MEMBERNOTFOUND;
		}
		else if (m_idNameMap[i].dispId < dispid)
		   min = i+1;
		else
		   max = i-1;
	}
	return DISP_E_MEMBERNOTFOUND;
}

disp_id_name_t Flasher::m_idNameMap[] = {
		{ NULL },
		{ DISPID_GameEvents_Init, L"_Init" },
		{ DISPID_TimerEvents_Timer, L"_Timer" }
};

HRESULT Flasher::FireDispID(const DISPID dispid, DISPPARAMS * const pdispparams) {
	static WCHAR wzName[MAXSTRING];
	CComPtr<IDispatch> disp;
	g_pplayer->m_ptable->m_pcv->m_pScript->GetScriptDispatch(nullptr, &disp);
	size_t min = 1, max = ARRAY_SIZE(m_idNameMap) - 1, i;
	int r;
	while(min <= max) {
		i = (min + max) / 2;
		if (m_idNameMap[i].dispId == dispid) {
			wcscpy(wzName, m_wzName);
			wcscat(wzName, m_idNameMap[i].name);
			LPOLESTR fnNames = (LPOLESTR)wzName;
			DISPID tDispid;
			if (SUCCEEDED(disp->GetIDsOfNames(IID_NULL, &fnNames, 1, 0, &tDispid))) {
				return disp->Invoke(tDispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, pdispparams, nullptr, nullptr, nullptr);
			}
			return DISP_E_MEMBERNOTFOUND;
		}
		else if (m_idNameMap[i].dispId < dispid)
		   min = i+1;
		else
		   max = i-1;
	}
	return DISP_E_MEMBERNOTFOUND;
}

disp_id_name_t Rubber::m_idNameMap[] = {
		{ NULL },
		{ DISPID_GameEvents_Init, L"_Init" },
		{ DISPID_TimerEvents_Timer, L"_Timer" },
		{ DISPID_HitEvents_Hit, L"_Hit" }
};

HRESULT Rubber::FireDispID(const DISPID dispid, DISPPARAMS * const pdispparams) {
	static WCHAR wzName[MAXSTRING];
	CComPtr<IDispatch> disp;
	g_pplayer->m_ptable->m_pcv->m_pScript->GetScriptDispatch(nullptr, &disp);
	size_t min = 1, max = ARRAY_SIZE(m_idNameMap) - 1, i;
	int r;
	while(min <= max) {
		i = (min + max) / 2;
		if (m_idNameMap[i].dispId == dispid) {
			wcscpy(wzName, m_wzName);
			wcscat(wzName, m_idNameMap[i].name);
			LPOLESTR fnNames = (LPOLESTR)wzName;
			DISPID tDispid;
			if (SUCCEEDED(disp->GetIDsOfNames(IID_NULL, &fnNames, 1, 0, &tDispid))) {
				return disp->Invoke(tDispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, pdispparams, nullptr, nullptr, nullptr);
			}
			return DISP_E_MEMBERNOTFOUND;
		}
		else if (m_idNameMap[i].dispId < dispid)
		   min = i+1;
		else
		   max = i-1;
	}
	return DISP_E_MEMBERNOTFOUND;
}

name_disp_id_t DispReel::m_nameIDMap[] = {
		{ NULL },
		{ L"AddValue", 30 },
		{ L"BackColor", DISPID_DispReel_BackColor },
		{ L"Height", 6 },
		{ L"Image", DISPID_Image },
		{ L"ImagesPerGridRow", 18 },
		{ L"IsTransparent", 12 },
		{ L"Name", DISPID_Name },
		{ L"Range", 14 },
		{ L"Reels", 11 },
		{ L"ResetToZero", 31 },
		{ L"SetValue", 33 },
		{ L"Sound", DISPID_Sound },
		{ L"Spacing", 7 },
		{ L"SpinReel", 32 },
		{ L"Steps", 8 },
		{ L"TimerEnabled", DISPID_Timer_Enabled },
		{ L"TimerInterval", DISPID_Timer_Interval },
		{ L"UpdateInterval", 15 },
		{ L"UseImageGrid", 17 },
		{ L"UserValue", DISPID_UserValue },
		{ L"Visible", 458 },
		{ L"Width", 5 },
		{ L"X", 9 },
		{ L"Y", 10 }
};

STDMETHODIMP DispReel::GetIDsOfNames(REFIID /*riid*/, LPOLESTR* rgszNames, UINT cNames, LCID lcid, DISPID* rgDispId) {
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

STDMETHODIMP DispReel::Invoke(DISPID dispIdMember, REFIID /*riid*/, LCID lcid, WORD wFlags, DISPPARAMS* pDispParams, VARIANT* pVarResult, EXCEPINFO* pExcepInfo, UINT* puArgErr) {
	HRESULT hr = DISP_E_UNKNOWNNAME;
	int index = pDispParams->cArgs;
	switch(dispIdMember) {
		case DISPID_DispReel_BackColor: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2399: [propget, id(DISPID_DispReel_BackColor), helpstring("property BackColor")] HRESULT BackColor([out, retval] OLE_COLOR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_UI4;
				hr = get_BackColor(&V_UI4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2400: [propput, id(DISPID_DispReel_BackColor), helpstring("property BackColor")] HRESULT BackColor([in] OLE_COLOR newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_UI4);
				hr = put_BackColor((OLE_COLOR)V_UI4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Image: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2401: [propget, id(DISPID_Image), helpstring("property Image")] HRESULT Image([out, retval] BSTR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_Image(&V_BSTR(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2402: [propput, id(DISPID_Image), helpstring("property Image")] HRESULT Image([in] BSTR newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = put_Image(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 11: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2403: [propget, id(11), helpstring("property Reels")] HRESULT Reels([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Reels(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2404: [propput, id(11), helpstring("property Reels")] HRESULT Reels([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Reels(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 5: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2405: [propget, id(5), helpstring("property Width")] HRESULT Width([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Width(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2406: [propput, id(5), helpstring("property Width")] HRESULT Width([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Width(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 6: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2407: [propget, id(6), helpstring("property Height")] HRESULT Height([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Height(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2408: [propput, id(6), helpstring("property Height")] HRESULT Height([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Height(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 7: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2409: [propget, id(7), helpstring("property Spacing")] HRESULT Spacing([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Spacing(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2410: [propput, id(7), helpstring("property Spacing")] HRESULT Spacing([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Spacing(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 12: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2411: [propget, id(12), helpstring("property IsTransparent")] HRESULT IsTransparent([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_IsTransparent(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2412: [propput, id(12), helpstring("property IsTransparent")] HRESULT IsTransparent([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_IsTransparent(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Sound: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2413: [propget, id(DISPID_Sound), helpstring("property Sound")] HRESULT Sound([out, retval] BSTR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_Sound(&V_BSTR(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2414: [propput, id(DISPID_Sound), helpstring("property Sound")] HRESULT Sound([in] BSTR newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = put_Sound(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 8: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2415: [propget, id(8), helpstring("property Steps")] HRESULT Steps([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Steps(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2416: [propput, id(8), helpstring("property Steps")] HRESULT Steps([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Steps(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Timer_Enabled: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2417: [propget, id(DISPID_Timer_Enabled), helpstring("property TimerEnabled")] HRESULT TimerEnabled([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_TimerEnabled(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2418: [propput, id(DISPID_Timer_Enabled), helpstring("property TimerEnabled")] HRESULT TimerEnabled([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_TimerEnabled(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Timer_Interval: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2419: [propget, id(DISPID_Timer_Interval), helpstring("property TimerInterval")] HRESULT TimerInterval([out, retval] long *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_TimerInterval((long*)&V_I4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2420: [propput, id(DISPID_Timer_Interval), helpstring("property TimerInterval")] HRESULT TimerInterval([in] long newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_I4);
				hr = put_TimerInterval(V_I4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 9: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2421: [propget, id(9), helpstring("property X")] HRESULT X([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_X(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2422: [propput, id(9), helpstring("property X")] HRESULT X([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_X(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 10: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2423: [propget, id(10), helpstring("property Y")] HRESULT Y([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Y(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2424: [propput, id(10), helpstring("property Y")] HRESULT Y([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Y(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 14: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2425: [propget, id(14), helpstring("property Range")] HRESULT Range([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_Range(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2426: [propput, id(14), helpstring("property Range")] HRESULT Range([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_Range(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Name: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2427: [propget, id(DISPID_Name), helpstring("property Name")] HRESULT Name([out, retval] BSTR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_Name(&V_BSTR(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2428: [propput, id(DISPID_Name), helpstring("property Name")] HRESULT Name([in] BSTR newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = put_Name(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 15: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2429: [propget, id(15), helpstring("property UpdateInterval")] HRESULT UpdateInterval([out, retval] long *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_UpdateInterval((long*)&V_I4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2430: [propput, id(15), helpstring("property UpdateInterval")] HRESULT UpdateInterval([in] long newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_I4);
				hr = put_UpdateInterval(V_I4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_UserValue: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2431: [propget, id(DISPID_UserValue), helpstring("property User")] HRESULT UserValue([out, retval] VARIANT* pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				hr = get_UserValue(pVarResult);
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2432: [propput, id(DISPID_UserValue), helpstring("property User")] HRESULT UserValue([in] VARIANT* newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantCopyInd(&var0, &pDispParams->rgvarg[--index]);
				hr = put_UserValue(&var0);
				VariantClear(&var0);
			}
			break;
		}
		case 17: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2433: [propget, id(17), helpstring("property UseImageGrid")] HRESULT UseImageGrid([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_UseImageGrid(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2434: [propput, id(17), helpstring("property UseImageGrid")] HRESULT UseImageGrid([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_UseImageGrid(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 458: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2435: [propget, id(458), helpstring("property Visible")] HRESULT Visible([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_Visible(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2436: [propput, id(458), helpstring("property Visible")] HRESULT Visible([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_Visible(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 18: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2437: [propget, id(18), helpstring("property ImagesPerGridRow")] HRESULT ImagesPerGridRow([out, retval] long *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_ImagesPerGridRow((long*)&V_I4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2438: [propput, id(18), helpstring("property ImagesPerGridRow")] HRESULT ImagesPerGridRow([in] long newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_I4);
				hr = put_ImagesPerGridRow(V_I4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 30: {
			if (wFlags & DISPATCH_METHOD) {
				// line 2440: [id(30), helpstring("method AddValue")] HRESULT AddValue([in] long Value);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_I4);
				hr = AddValue(V_I4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 31: {
			if (wFlags & DISPATCH_METHOD) {
				// line 2441: [id(31), helpstring("method ResetToZero")] HRESULT ResetToZero();
				hr = ResetToZero();
			}
			break;
		}
		case 32: {
			if (wFlags & DISPATCH_METHOD) {
				// line 2442: [id(32), helpstring("method SpinReel")] HRESULT SpinReel([in] long ReelNumber, [in] long PulseCount);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_I4);
				VARIANT var1;
				V_VT(&var1) = VT_EMPTY;
				VariantChangeType(&var1, &pDispParams->rgvarg[--index], 0, VT_I4);
				hr = SpinReel(V_I4(&var0), V_I4(&var1));
				VariantClear(&var0);
				VariantClear(&var1);
			}
			break;
		}
		case 33: {
			if (wFlags & DISPATCH_METHOD) {
				// line 2443: [id(33), helpstring("method SetValue")] HRESULT SetValue([in] long Value);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_I4);
				hr = SetValue(V_I4(&var0));
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

STDMETHODIMP DispReel::GetDocumentation(INT index, BSTR *pBstrName, BSTR *pBstrDocString, DWORD *pdwHelpContext, BSTR *pBstrHelpFile) {
	if (index == MEMBERID_NIL) {
		*pBstrName = SysAllocString(L"DispReel");
		return S_OK;
	}
	return E_NOTIMPL;
}

disp_id_name_t DispReel::m_idNameMap[] = {
		{ NULL },
		{ DISPID_GameEvents_Init, L"_Init" },
		{ DISPID_TimerEvents_Timer, L"_Timer" },
		{ DISPID_AnimateEvents_Animate, L"_Animate" }
};

HRESULT DispReel::FireDispID(const DISPID dispid, DISPPARAMS * const pdispparams) {
	static WCHAR wzName[MAXSTRING];
	CComPtr<IDispatch> disp;
	g_pplayer->m_ptable->m_pcv->m_pScript->GetScriptDispatch(nullptr, &disp);
	size_t min = 1, max = ARRAY_SIZE(m_idNameMap) - 1, i;
	int r;
	while(min <= max) {
		i = (min + max) / 2;
		if (m_idNameMap[i].dispId == dispid) {
			wcscpy(wzName, m_wzName);
			wcscat(wzName, m_idNameMap[i].name);
			LPOLESTR fnNames = (LPOLESTR)wzName;
			DISPID tDispid;
			if (SUCCEEDED(disp->GetIDsOfNames(IID_NULL, &fnNames, 1, 0, &tDispid))) {
				return disp->Invoke(tDispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, pdispparams, nullptr, nullptr, nullptr);
			}
			return DISP_E_MEMBERNOTFOUND;
		}
		else if (m_idNameMap[i].dispId < dispid)
		   min = i+1;
		else
		   max = i-1;
	}
	return DISP_E_MEMBERNOTFOUND;
}

name_disp_id_t LightSeq::m_nameIDMap[] = {
		{ NULL },
		{ L"CenterX", 9 },
		{ L"CenterY", 10 },
		{ L"Collection", DISPID_Collection },
		{ L"Name", DISPID_Name },
		{ L"Play", 32 },
		{ L"StopPlay", 33 },
		{ L"TimerEnabled", DISPID_Timer_Enabled },
		{ L"TimerInterval", DISPID_Timer_Interval },
		{ L"UpdateInterval", 15 },
		{ L"UserValue", DISPID_UserValue }
};

STDMETHODIMP LightSeq::GetIDsOfNames(REFIID /*riid*/, LPOLESTR* rgszNames, UINT cNames, LCID lcid, DISPID* rgDispId) {
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

STDMETHODIMP LightSeq::Invoke(DISPID dispIdMember, REFIID /*riid*/, LCID lcid, WORD wFlags, DISPPARAMS* pDispParams, VARIANT* pVarResult, EXCEPINFO* pExcepInfo, UINT* puArgErr) {
	HRESULT hr = DISP_E_UNKNOWNNAME;
	int index = pDispParams->cArgs;
	switch(dispIdMember) {
		case DISPID_Name: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2467: [propget, id(DISPID_Name), helpstring("property Name")] HRESULT Name([out, retval] BSTR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_Name(&V_BSTR(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2468: [propput, id(DISPID_Name), helpstring("property Name")] HRESULT Name([in] BSTR newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = put_Name(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Collection: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2469: [propget, id(DISPID_Collection), helpstring("property Collection")] HRESULT Collection([out, retval] BSTR *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BSTR;
				hr = get_Collection(&V_BSTR(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2470: [propput, id(DISPID_Collection), helpstring("property Collection")] HRESULT Collection([in] BSTR newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BSTR);
				hr = put_Collection(V_BSTR(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 9: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2471: [propget, id(9), helpstring("property CenterX")] HRESULT CenterX([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_CenterX(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2472: [propput, id(9), helpstring("property CenterX")] HRESULT CenterX([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_CenterX(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 10: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2473: [propget, id(10), helpstring("property CenterY")] HRESULT CenterY([out, retval] float *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_R4;
				hr = get_CenterY(&V_R4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2474: [propput, id(10), helpstring("property CenterY")] HRESULT CenterY([in] float newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_R4);
				hr = put_CenterY(V_R4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case 15: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2475: [propget, id(15), helpstring("property UpdateInterval")] HRESULT UpdateInterval([out, retval] long *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_UpdateInterval((long*)&V_I4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2476: [propput, id(15), helpstring("property UpdateInterval")] HRESULT UpdateInterval([in] long newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_I4);
				hr = put_UpdateInterval(V_I4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Timer_Enabled: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2477: [propget, id(DISPID_Timer_Enabled), helpstring("property TimerEnabled")] HRESULT TimerEnabled([out, retval] VARIANT_BOOL *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_BOOL;
				hr = get_TimerEnabled(&V_BOOL(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2478: [propput, id(DISPID_Timer_Enabled), helpstring("property TimerEnabled")] HRESULT TimerEnabled([in] VARIANT_BOOL newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_BOOL);
				hr = put_TimerEnabled(V_BOOL(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_Timer_Interval: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2479: [propget, id(DISPID_Timer_Interval), helpstring("property TimerInterval")] HRESULT TimerInterval([out, retval] long *pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				V_VT(pVarResult) = VT_I4;
				hr = get_TimerInterval((long*)&V_I4(pVarResult));
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2480: [propput, id(DISPID_Timer_Interval), helpstring("property TimerInterval")] HRESULT TimerInterval([in] long newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_I4);
				hr = put_TimerInterval(V_I4(&var0));
				VariantClear(&var0);
			}
			break;
		}
		case DISPID_UserValue: {
			if (wFlags & DISPATCH_PROPERTYGET) {
				// line 2481: [propget, id(DISPID_UserValue), helpstring("property User")] HRESULT UserValue([out, retval] VARIANT* pVal);
				if (pVarResult == NULL) {
					VARIANT valResult;
					VariantInit(&valResult);
					pVarResult = &valResult;
				}
				hr = get_UserValue(pVarResult);
			}
			else if (wFlags & DISPATCH_PROPERTYPUT) {
				// line 2482: [propput, id(DISPID_UserValue), helpstring("property User")] HRESULT UserValue([in] VARIANT* newVal);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantCopyInd(&var0, &pDispParams->rgvarg[--index]);
				hr = put_UserValue(&var0);
				VariantClear(&var0);
			}
			break;
		}
		case 32: {
			if (wFlags & DISPATCH_METHOD) {
				// line 2483: [id(32), helpstring("method Play")] HRESULT Play([in] SequencerState Animation, [defaultvalue(0)] long TailLength, [defaultvalue(1)] long Repeat, [defaultvalue(0)] long Pause);
				VARIANT var0;
				V_VT(&var0) = VT_EMPTY;
				VariantChangeType(&var0, &pDispParams->rgvarg[--index], 0, VT_I4);
				VARIANT var1;
				V_VT(&var1) = VT_I4;
				V_I4(&var1) = 0;
				VariantChangeType(&var1, (index > 0) ? &pDispParams->rgvarg[--index] : &var1, 0, VT_I4);
				VARIANT var2;
				V_VT(&var2) = VT_I4;
				V_I4(&var2) = 1;
				VariantChangeType(&var2, (index > 0) ? &pDispParams->rgvarg[--index] : &var2, 0, VT_I4);
				VARIANT var3;
				V_VT(&var3) = VT_I4;
				V_I4(&var3) = 0;
				VariantChangeType(&var3, (index > 0) ? &pDispParams->rgvarg[--index] : &var3, 0, VT_I4);
				hr = Play((SequencerState)V_I4(&var0), V_I4(&var1), V_I4(&var2), V_I4(&var3));
				VariantClear(&var0);
				VariantClear(&var1);
				VariantClear(&var2);
				VariantClear(&var3);
			}
			break;
		}
		case 33: {
			if (wFlags & DISPATCH_METHOD) {
				// line 2484: [id(33), helpstring("method StopPlay")] HRESULT StopPlay();
				hr = StopPlay();
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

STDMETHODIMP LightSeq::GetDocumentation(INT index, BSTR *pBstrName, BSTR *pBstrDocString, DWORD *pdwHelpContext, BSTR *pBstrHelpFile) {
	if (index == MEMBERID_NIL) {
		*pBstrName = SysAllocString(L"LightSeq");
		return S_OK;
	}
	return E_NOTIMPL;
}

disp_id_name_t LightSeq::m_idNameMap[] = {
		{ NULL },
		{ DISPID_GameEvents_Init, L"_Init" },
		{ DISPID_TimerEvents_Timer, L"_Timer" },
		{ DISPID_LightSeqEvents_PlayDone, L"_PlayDone" }
};

HRESULT LightSeq::FireDispID(const DISPID dispid, DISPPARAMS * const pdispparams) {
	static WCHAR wzName[MAXSTRING];
	CComPtr<IDispatch> disp;
	g_pplayer->m_ptable->m_pcv->m_pScript->GetScriptDispatch(nullptr, &disp);
	size_t min = 1, max = ARRAY_SIZE(m_idNameMap) - 1, i;
	int r;
	while(min <= max) {
		i = (min + max) / 2;
		if (m_idNameMap[i].dispId == dispid) {
			wcscpy(wzName, m_wzName);
			wcscat(wzName, m_idNameMap[i].name);
			LPOLESTR fnNames = (LPOLESTR)wzName;
			DISPID tDispid;
			if (SUCCEEDED(disp->GetIDsOfNames(IID_NULL, &fnNames, 1, 0, &tDispid))) {
				return disp->Invoke(tDispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, pdispparams, nullptr, nullptr, nullptr);
			}
			return DISP_E_MEMBERNOTFOUND;
		}
		else if (m_idNameMap[i].dispId < dispid)
		   min = i+1;
		else
		   max = i-1;
	}
	return DISP_E_MEMBERNOTFOUND;
}

