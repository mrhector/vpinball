#include "stdafx.h"

FlexDMDCompositeAction::FlexDMDCompositeAction()
{
}

FlexDMDCompositeAction::~FlexDMDCompositeAction()
{
}

STDMETHODIMP FlexDMDCompositeAction::Add(IUnknown *action, ICompositeAction **pRetVal)
{
    return this->QueryInterface(IID_ICompositeAction, (void**)pRetVal);
}