#pragma once

#include "flexdmd_i.h"
#include "FlexDMDActor.h"

class FlexDMDAnimatedActor :
   public IDispatchImpl<IVideoActor, &IID_IVideoActor, &LIBID_FlexDMD>,
   public CComObjectRoot,
   public CComCoClass<FlexDMDAnimatedActor, &CLSID_FlexDMD>,
   public FlexDMDActor
{
public:
   STDMETHOD(GetIDsOfNames)(REFIID /*riid*/, LPOLESTR* rgszNames, UINT cNames, LCID lcid,DISPID* rgDispId);
   STDMETHOD(Invoke)(DISPID dispIdMember, REFIID /*riid*/, LCID lcid, WORD wFlags, DISPPARAMS* pDispParams, VARIANT* pVarResult, EXCEPINFO* pExcepInfo, UINT* puArgErr);
   STDMETHOD(GetDocumentation)(INT index, BSTR *pBstrName, BSTR *pBstrDocString, DWORD *pdwHelpContext, BSTR *pBstrHelpFile);

public:
   FlexDMDAnimatedActor();
   ~FlexDMDAnimatedActor();

   BEGIN_COM_MAP(FlexDMDAnimatedActor)
      COM_INTERFACE_ENTRY(IVideoActor)
      COM_INTERFACE_ENTRY(IDispatch)
   END_COM_MAP()

   STDMETHOD(get_Name)(BSTR *pRetVal) { return FlexDMDActor::get_Name(pRetVal); }
   STDMETHOD(put_Name)(BSTR pRetVal) { return FlexDMDActor::put_Name(pRetVal); };
   STDMETHOD(get_x)(single *pRetVal) { return FlexDMDActor::get_x(pRetVal); };
   STDMETHOD(put_x)(single pRetVal) { return FlexDMDActor::put_x(pRetVal); };
   STDMETHOD(get_y)(single *pRetVal) { return FlexDMDActor::get_y(pRetVal); };
   STDMETHOD(put_y)(single pRetVal) { return FlexDMDActor::put_y(pRetVal); };
   STDMETHOD(get_Width)(single *pRetVal) { return FlexDMDActor::get_Width(pRetVal); };
   STDMETHOD(put_Width)(single pRetVal) { return FlexDMDActor::put_Width(pRetVal); };
   STDMETHOD(get_Height)(single *pRetVal) { return FlexDMDActor::get_Height(pRetVal); };
   STDMETHOD(put_Height)(single pRetVal) { return FlexDMDActor::put_Height(pRetVal); };
   STDMETHOD(get_visible)(VARIANT_BOOL *pRetVal) { return FlexDMDActor::get_visible(pRetVal); };
   STDMETHOD(put_visible)(VARIANT_BOOL pRetVal) { return FlexDMDActor::put_visible(pRetVal); };
   STDMETHOD(get_FillParent)(VARIANT_BOOL *pRetVal) { return FlexDMDActor::get_FillParent(pRetVal); };
   STDMETHOD(put_FillParent)(VARIANT_BOOL pRetVal) { return FlexDMDActor::put_FillParent(pRetVal); };
   STDMETHOD(get_ClearBackground)(VARIANT_BOOL *pRetVal) { return FlexDMDActor::get_ClearBackground(pRetVal); };
   STDMETHOD(put_ClearBackground)(VARIANT_BOOL pRetVal) { return FlexDMDActor::put_ClearBackground(pRetVal); };
   STDMETHOD(SetBounds)(single x, single y, single Width, single Height) { return FlexDMDActor::SetBounds(x, y, Width, Height); };
   STDMETHOD(SetPosition)(single x, single y) { return FlexDMDActor::SetPosition(x, y); };
   STDMETHOD(SetAlignedPosition)(single x, single y, Alignment alignment) { return FlexDMDActor::SetAlignedPosition(x, y, alignment); };
   STDMETHOD(SetSize)(single Width, single Height) { return FlexDMDActor::SetSize(Width, Height); };
   STDMETHOD(get_PrefWidth)(single *pRetVal) { return FlexDMDActor::get_PrefWidth(pRetVal); };
   STDMETHOD(get_PrefHeight)(single *pRetVal) { return FlexDMDActor::get_PrefHeight(pRetVal); };
   STDMETHOD(Pack)() { return FlexDMDActor::Pack(); };
   STDMETHOD(Remove)() { return FlexDMDActor::Remove(); };
   STDMETHOD(get_ActionFactory)(IActionFactory **pRetVal) { return FlexDMDActor::get_ActionFactory(pRetVal); };
   STDMETHOD(AddAction)(IUnknown *action) { return FlexDMDActor::AddAction(action); };
   STDMETHOD(ClearActions)() { return FlexDMDActor::ClearActions(); };

   void Missing34();
   void Missing35();
   void Missing36();
   void Missing37();
   STDMETHOD(get_Length)(single *pRetVal);
   STDMETHOD(get_Loop)(VARIANT_BOOL *pRetVal);
   STDMETHOD(put_Loop)(VARIANT_BOOL pRetVal);
   STDMETHOD(get_Paused)(VARIANT_BOOL *pRetVal);
   STDMETHOD(put_Paused)(VARIANT_BOOL pRetVal);
   STDMETHOD(get_PlaySpeed)(single *pRetVal);
   STDMETHOD(put_PlaySpeed)(single pRetVal);
   STDMETHOD(Seek)(single posInSeconds);

   virtual void Update(float delta);
   void Seek_(float posInSeconds);
   void Advance(float delta);
   void Rewind();
   virtual void ReadNextFrame() = 0;

   float GetFrameTime() { return m_frameTime; }
   void SetFrameTime(float frameTime) { m_frameTime = frameTime; }
   float GetFrameDuration() { return m_frameDuration; }
   void SetFrameDuration(float frameDuration) { m_frameDuration = frameDuration; }
   void SetTime(float time) { m_time = time; }
   void SetEndOfAnimation(bool endOfAnimation) { m_endOfAnimation = endOfAnimation; }
   Scaling GetScaling() { return m_scaling; }
   Alignment GetAlignment() { return m_alignment; }
   float GetLength() { return m_length; }
   void SetLength(float length) { m_length = length; }
   bool GetLoop() { return m_loop; }
   void SetLoop(float loop) { m_loop = loop; }

private:
   float m_frameTime;
   float m_frameDuration;
   float m_time;
   bool m_endOfAnimation;

   Scaling m_scaling;
   Alignment m_alignment;
   bool m_paused;
   bool m_loop;
   float m_length;
   float m_playSpeed;
};