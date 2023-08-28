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
   mINI::INIFile file(m_pController->GetIniPath() + ((VPinMAMEGame*)m_pGame)->GetName() + ".ini");
   file.read(ini);

   string szName = MakeString(sName);
   if (ini.has("settings") && ini["settings"].has(szName)) {
      const WCHAR * const wz = MakeWide(ini["settings"][szName]);
      SetVarBstr(pVal, SysAllocString(wz));
      delete[] wz;
   }
   else
      SetVarBstr(pVal, SysAllocString(L""));

   string szValue = MakeString(V_BSTR(pVal));

   PLOGD.printf("name=%s, value=%s", szName.c_str(), szValue.c_str());

   return S_OK;
}

STDMETHODIMP VPinMAMEGameSettings::put_Value(BSTR sName, VARIANT newVal)
{
   mINI::INIStructure ini;
   mINI::INIFile file(m_pController->GetIniPath() + ((VPinMAMEGame*)m_pGame)->GetName() + ".ini");
   file.read(ini);

   string szName = MakeString(sName);
   BSTR bstr = BstrFromVariant(&newVal, 0x409);
   string szValue = MakeString(bstr);
   ini["settings"][szName] = MakeString(bstr);
   SysFreeString(bstr);

   file.write(ini);

   PLOGD.printf("name=%s, value=%s", szName.c_str(), szValue.c_str());

   return S_OK;
}

STDMETHODIMP VPinMAMEGameSettings::SetDisplayPosition(VARIANT newValX, VARIANT newValY, LONG_PTR hWnd) { return E_NOTIMPL; }
