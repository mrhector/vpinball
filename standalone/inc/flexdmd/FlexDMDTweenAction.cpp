#include "stdafx.h"
#include "FlexDMDTweenAction.h"

FlexDMDTweenAction::FlexDMDTweenAction()
{
}

FlexDMDTweenAction::~FlexDMDTweenAction()
{
}

CComObject<FlexDMDTweenAction>* FlexDMDTweenAction::Create()
{
   CComObject<FlexDMDTweenAction>* obj = nullptr;
   CComObject<FlexDMDTweenAction>::CreateInstance(&obj);

   obj->AddRef();

   return obj;
}


STDMETHODIMP FlexDMDTweenAction::get_Ease(Interpolation *pRetVal)
{
   PLOGW << "Not implemented";

   return S_OK;
}

STDMETHODIMP FlexDMDTweenAction::put_Ease(Interpolation pRetVal)
{
   PLOGW << "Not implemented";

   return S_OK;
}