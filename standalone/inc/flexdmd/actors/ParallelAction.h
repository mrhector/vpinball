#pragma once

#include "Action.h"

class ParallelAction : public Action
{
public:
   ParallelAction();
   virtual ~ParallelAction() {};

   STDMETHOD(GetIDsOfNames)(REFIID /*riid*/, LPOLESTR* rgszNames, UINT cNames, LCID lcid,DISPID* rgDispId);
   STDMETHOD(Invoke)(DISPID dispIdMember, REFIID /*riid*/, LCID lcid, WORD wFlags, DISPPARAMS* pDispParams, VARIANT* pVarResult, EXCEPINFO* pExcepInfo, UINT* puArgErr);
   STDMETHOD(Add)(IUnknown *action, ICompositeAction **pRetVal);

   virtual bool Update(float secondsElapsed);

private:
   vector<Action*> m_actions;
   vector<bool> m_runMask;
};
