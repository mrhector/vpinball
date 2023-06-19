#include "stdafx.h"
#include "VPinMAMEGameSettings.h"
#include "VPinMAMEGame.h"
#include "inc/mINI/ini.h"

VPinMAMEGameSettings::VPinMAMEGameSettings()
{
   m_pGame = nullptr;
}

VPinMAMEGameSettings::~VPinMAMEGameSettings()
{
   if (m_pGame)
      m_pGame->Release();
}

CComObject<VPinMAMEGameSettings>* VPinMAMEGameSettings::Create(VPinMAMEController* pController, IGame* pGame)
{
   CComObject<VPinMAMEGameSettings>* obj = nullptr;
   CComObject<VPinMAMEGameSettings>::CreateInstance(&obj);
   obj->AddRef();

   obj->m_pController = pController;
   obj->m_pGame = pGame;
   obj->m_pGame->AddRef();

   return obj;
}

STDMETHODIMP VPinMAMEGameSettings::ShowSettingsDlg(LONG_PTR hParentWnd) { return E_NOTIMPL; }
STDMETHODIMP VPinMAMEGameSettings::Clear() { return E_NOTIMPL; }

STDMETHODIMP VPinMAMEGameSettings::get_Value(BSTR sName, VARIANT *pVal)
{
   mINI::INIStructure ini;
   mINI::INIFile file(m_pController->GetPath() + "vpinball" + PATH_SEPARATOR_CHAR + ((VPinMAMEGame*)m_pGame)->GetName() + ".ini");
   file.read(ini);

   char szName[MAXSTRING];
   WideCharToMultiByteNull(CP_ACP, 0, sName, -1, szName, 256, nullptr, nullptr);

   V_VT(pVal) = VT_BSTR;

   if (ini.has("settings") && ini["settings"].has(szName)) {
      const WCHAR * const wz = MakeWide(ini["settings"][szName]);
      V_BSTR(pVal) = ::SysAllocString(wz);
      delete[] wz;
   }
   else
      V_BSTR(pVal) = ::SysAllocString(L"");

   string szValue = MakeString(V_BSTR(pVal));

   PLOGI.printf("name=%s, value=%s", szName, szValue.c_str());

   return S_OK;
}

STDMETHODIMP VPinMAMEGameSettings::put_Value(BSTR sName, VARIANT newVal)
{
   mINI::INIStructure ini;
   mINI::INIFile file(m_pController->GetPath() + "vpinball" + PATH_SEPARATOR_CHAR + ((VPinMAMEGame*)m_pGame)->GetName() + ".ini");
   file.read(ini);

   char szName[MAXSTRING];
   WideCharToMultiByteNull(CP_ACP, 0, sName, -1, szName, 256, nullptr, nullptr);

   VARIANT var0;
   V_VT(&var0) = VT_EMPTY;
   VariantChangeType(&var0, &newVal, 0, VT_BSTR);

   char szValue[MAXSTRING * 4];
   WideCharToMultiByte(CP_ACP, 0, V_BSTR(&var0), -1, szValue, sizeof(szValue), NULL, NULL);

   PLOGI.printf("name=%s, value=%s", szName, szValue);

   VariantClear(&var0);

   ini["settings"][szName] = szValue;

   file.write(ini);

   return S_OK;
}

STDMETHODIMP VPinMAMEGameSettings::SetDisplayPosition(VARIANT newValX, VARIANT newValY, LONG_PTR hWnd) { return E_NOTIMPL; }
