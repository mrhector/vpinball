#include "stdafx.h"
#include "VPinMAMEGames.h"
#include "VPinMAMEGame.h"

void CALLBACK VPinMAMEGames::GetGameCallback(PinmameGame* pPinmameGame, const void* pUserData)
{
   VPinMAMEGames* pGames = (VPinMAMEGames*)pUserData;

   memcpy(&pGames->m_pinmameGame, pPinmameGame, sizeof(PinmameGame));
}

VPinMAMEGames::VPinMAMEGames()
{
}

VPinMAMEGames::~VPinMAMEGames()
{
}

CComObject<VPinMAMEGames>* VPinMAMEGames::Create(VPinMAMEController* pController)
{
    CComObject<VPinMAMEGames>* obj = nullptr;
    CComObject<VPinMAMEGames>::CreateInstance(&obj);
    obj->AddRef();

    obj->m_pController = pController;

    return obj;
}

STDMETHODIMP VPinMAMEGames::get_Count(LONG* pnCount) { return E_NOTIMPL; }

STDMETHODIMP VPinMAMEGames::get_Item(VARIANT *pKey, IGame **pGame)
{
   if (V_VT(pKey) & VT_BSTR) {
      string szName = MakeString(V_BSTR(pKey));

      if (PinmameGetGame(szName.c_str(), &GetGameCallback, this) == PINMAME_STATUS::OK) {
         *pGame = VPinMAMEGame::Create(m_pController, &m_pinmameGame);

         return S_OK;
      }
   }

   return E_INVALIDARG;
}

STDMETHODIMP VPinMAMEGames::get__NewEnum(IUnknown** ppunkEnum) { return E_NOTIMPL; }
