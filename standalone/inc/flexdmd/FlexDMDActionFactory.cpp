#include "stdafx.h"

FlexDMDActionFactory::FlexDMDActionFactory()
{
}

FlexDMDActionFactory::~FlexDMDActionFactory()
{
}

STDMETHODIMP FlexDMDActionFactory::Wait(single secondsToWait, IUnknown **pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDActionFactory::Delayed(single secondsToWait, IUnknown *action, IUnknown **pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDActionFactory::Parallel(ICompositeAction **pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDActionFactory::Sequence(ICompositeAction **pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDActionFactory::Repeat(IUnknown *action, LONG count, IUnknown **pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDActionFactory::Blink(single secondsShow, single secondsHide, LONG Repeat, IUnknown **pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDActionFactory::Show(VARIANT_BOOL visible, IUnknown **pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDActionFactory::AddTo(IGroupActor *parent, IUnknown **pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDActionFactory::RemoveFromParent(IUnknown **pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDActionFactory::AddChild(IUnknown *child, IUnknown **pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDActionFactory::RemoveChild(IUnknown *child, IUnknown **pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDActionFactory::Seek(single pos, IUnknown **pRetVal) { return E_NOTIMPL; }
STDMETHODIMP FlexDMDActionFactory::MoveTo(single x, single y, single duration, ITweenAction **pRetVal) { return E_NOTIMPL; }