#include "stdafx.h"

#include "FlexDMDActionFactory.h"
#include "WaitAction.h"
#include "DelayedAction.h"
#include "ParallelAction.h"
#include "SequenceAction.h"
#include "RepeatAction.h"
#include "BlinkAction.h"
#include "ShowAction.h"
#include "AddToAction.h"
#include "RemoveFromParentAction.h"
#include "AddChildAction.h"
#include "SeekAction.h"
#include "MoveToAction.h"

FlexDMDActionFactory::FlexDMDActionFactory()
{
}

FlexDMDActionFactory::~FlexDMDActionFactory()
{
}

CComObject<FlexDMDActionFactory>* FlexDMDActionFactory::Create(FlexDMDActor* pTarget)
{
   CComObject<FlexDMDActionFactory>* obj = nullptr;
   CComObject<FlexDMDActionFactory>::CreateInstance(&obj);
   obj->AddRef();

   obj->m_pTarget = pTarget;

   return obj;
}

STDMETHODIMP FlexDMDActionFactory::Wait(single secondsToWait, IUnknown **pRetVal)
{
   WaitAction* obj = new WaitAction(secondsToWait);
   return obj->QueryInterface(IID_IUnknown, (void**)pRetVal);
}

STDMETHODIMP FlexDMDActionFactory::Delayed(single secondsToWait, IUnknown *action, IUnknown **pRetVal)
{  
   Action* pAction = dynamic_cast<Action*>(action);

   DelayedAction* obj = new DelayedAction(secondsToWait, pAction);
   return obj->QueryInterface(IID_IUnknown, (void**)pRetVal);
}

STDMETHODIMP FlexDMDActionFactory::Parallel(ICompositeAction **pRetVal)
{
   ParallelAction* obj = new ParallelAction();
   return obj->QueryInterface(IID_ICompositeAction, (void**)pRetVal);
}

STDMETHODIMP FlexDMDActionFactory::Sequence(ICompositeAction **pRetVal)
{
   SequenceAction* obj = new SequenceAction();
   return obj->QueryInterface(IID_ICompositeAction, (void**)pRetVal);
}

STDMETHODIMP FlexDMDActionFactory::Repeat(IUnknown *action, LONG count, IUnknown **pRetVal)
{
   Action* pAction = dynamic_cast<Action*>(action);

   RepeatAction* obj = new RepeatAction(pAction, count); 
   return obj->QueryInterface(IID_IUnknown, (void**)pRetVal);
}

STDMETHODIMP FlexDMDActionFactory::Blink(single secondsShow, single secondsHide, LONG Repeat, IUnknown **pRetVal)
{
   BlinkAction* obj = new BlinkAction(m_pTarget, secondsShow, secondsHide, Repeat);
   return obj->QueryInterface(IID_IUnknown, (void**)pRetVal);
}

STDMETHODIMP FlexDMDActionFactory::Show(VARIANT_BOOL visible, IUnknown **pRetVal)
{
   ShowAction* obj = new ShowAction(m_pTarget, visible == VARIANT_TRUE);
   return obj->QueryInterface(IID_IUnknown, (void**)pRetVal);
}

STDMETHODIMP FlexDMDActionFactory::AddTo(IGroupActor *parent, IUnknown **pRetVal)
{
   AddToAction* obj = new AddToAction(m_pTarget, (FlexDMDGroupActor*)parent, true);
   return obj->QueryInterface(IID_IUnknown, (void**)pRetVal);
}

STDMETHODIMP FlexDMDActionFactory::RemoveFromParent(IUnknown **pRetVal)
{
   RemoveFromParentAction* obj = new RemoveFromParentAction(m_pTarget);
   return obj->QueryInterface(IID_IUnknown, (void**)pRetVal);
}

STDMETHODIMP FlexDMDActionFactory::AddChild(IUnknown *child, IUnknown **pRetVal)
{
   AddChildAction* obj = new AddChildAction((FlexDMDGroupActor*)m_pTarget, (FlexDMDActor*)child, true);
   return obj->QueryInterface(IID_IUnknown, (void**)pRetVal);
}

STDMETHODIMP FlexDMDActionFactory::RemoveChild(IUnknown *child, IUnknown **pRetVal)
{
   AddChildAction* obj = new AddChildAction((FlexDMDGroupActor*)m_pTarget, (FlexDMDActor*)child, false);
   return obj->QueryInterface(IID_IUnknown, (void**)pRetVal);
}

STDMETHODIMP FlexDMDActionFactory::Seek(single pos, IUnknown **pRetVal)
{
   SeekAction* obj = new SeekAction((FlexDMDAnimatedActor*)m_pTarget, pos);
   return obj->QueryInterface(IID_IUnknown, (void**)pRetVal);
}

STDMETHODIMP FlexDMDActionFactory::MoveTo(single x, single y, single duration, ITweenAction **pRetVal)
{
   MoveToAction* obj = new MoveToAction(m_pTarget, x, y, duration);
   return obj->QueryInterface(IID_ITweenAction, (void**)pRetVal);
}
