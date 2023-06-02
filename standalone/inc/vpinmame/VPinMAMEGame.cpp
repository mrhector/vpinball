#include "stdafx.h"
#include "VPinMAMEGame.h"
#include "VPinMAMEGameSettings.h"

VPinMAMEGame::VPinMAMEGame()
{
}

VPinMAMEGame::~VPinMAMEGame()
{
}

CComObject<VPinMAMEGame>* VPinMAMEGame::Create(VPinMAMEController* pController, PinmameGame* pPinmameGame) 
{
   CComObject<VPinMAMEGame>* obj = nullptr;
   CComObject<VPinMAMEGame>::CreateInstance(&obj);
   obj->AddRef();

   obj->m_pController = pController;
   memcpy(&obj->m_pinmameGame, pPinmameGame, sizeof(PinmameGame));

   return obj;
}

STDMETHODIMP VPinMAMEGame::get_Name(BSTR *pVal)
{
   CComBSTR Name(m_pinmameGame.name);
   *pVal = Name.Detach();
   return S_OK;
}

STDMETHODIMP VPinMAMEGame::get_Description(BSTR *pVal)
{
   CComBSTR Description(m_pinmameGame.description);
   *pVal = Description.Detach();
   return S_OK;
}

STDMETHODIMP VPinMAMEGame::get_Year(BSTR *pVal)
{
   CComBSTR Year(m_pinmameGame.year);
   *pVal = Year.Detach();
   return S_OK;
}

STDMETHODIMP VPinMAMEGame::get_Manufacturer(BSTR *pVal)
{
   CComBSTR Manufacturer(m_pinmameGame.manufacturer);
   *pVal = Manufacturer.Detach();
   return S_OK;
}

STDMETHODIMP VPinMAMEGame::get_CloneOf(BSTR *pVal)
{
   CComBSTR CloneOf(m_pinmameGame.clone_of);
   *pVal = CloneOf.Detach();
   return S_OK;
}

STDMETHODIMP VPinMAMEGame::get_Roms(IRoms** pVal) { return E_NOTIMPL; }

STDMETHODIMP VPinMAMEGame::get_Settings(IGameSettings** pVal)
{
   *pVal = VPinMAMEGameSettings::Create(m_pController, this);

   return S_OK;
}

STDMETHODIMP VPinMAMEGame::ShowInfoDlg(int nShowOptions, LONG_PTR hParentWnd, int *pVal) { return E_NOTIMPL; }
STDMETHODIMP VPinMAMEGame::get_IsSupported(VARIANT_BOOL *pVal) { return E_NOTIMPL; }