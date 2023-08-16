#pragma once

#include "ultradmd_i.h"
#include "../flexdmd/FlexDMDUltraDMD.h"

class UltraDMDDMDObject :
   public IDispatchImpl<IDMDObject, &IID_IDMDObject, &LIBID_UltraDMD>,
   public CComObjectRoot,
   public CComCoClass<UltraDMDDMDObject, &CLSID_DMDObject>
{
public:
   STDMETHOD(GetIDsOfNames)(REFIID /*riid*/, LPOLESTR* rgszNames, UINT cNames, LCID lcid,DISPID* rgDispId);
   STDMETHOD(Invoke)(DISPID dispIdMember, REFIID /*riid*/, LCID lcid, WORD wFlags, DISPPARAMS* pDispParams, VARIANT* pVarResult, EXCEPINFO* pExcepInfo, UINT* puArgErr);
   STDMETHOD(GetDocumentation)(INT index, BSTR *pBstrName, BSTR *pBstrDocString, DWORD *pdwHelpContext, BSTR *pBstrHelpFile);

public:
   UltraDMDDMDObject();
   ~UltraDMDDMDObject();

   BEGIN_COM_MAP(UltraDMDDMDObject)
      COM_INTERFACE_ENTRY(IDMDObject)
      COM_INTERFACE_ENTRY(IDispatch)
   END_COM_MAP()

   STDMETHOD(get_FloatProperty)(single *pRetVal);
   STDMETHOD(put_FloatProperty)(single pRetVal);
   STDMETHOD(Init)();
   STDMETHOD(Uninit)();
   STDMETHOD(GetMajorVersion)(LONG *pRetVal);
   STDMETHOD(GetMinorVersion)(LONG *pRetVal);
   STDMETHOD(GetBuildNumber)(LONG *pRetVal);
   STDMETHOD(SetVisibleVirtualDMD)(VARIANT_BOOL bHide, VARIANT_BOOL *pRetVal);
   STDMETHOD(SetFlipY)(VARIANT_BOOL flipY, VARIANT_BOOL *pRetVal);
   STDMETHOD(IsRendering)(VARIANT_BOOL *pRetVal);
   STDMETHOD(CancelRendering)();
   STDMETHOD(CancelRenderingWithId)(BSTR sceneId);
   STDMETHOD(Clear)();
   STDMETHOD(SetProjectFolder)(BSTR basePath);
   STDMETHOD(SetVideoStretchMode)(LONG mode);
   STDMETHOD(SetScoreboardBackgroundImage)(BSTR filename, LONG selectedBrightness, LONG unselectedBrightness);
   STDMETHOD(CreateAnimationFromImages)(LONG fps, VARIANT_BOOL loop, BSTR imagelist, LONG *pRetVal);
   STDMETHOD(RegisterVideo)(LONG videoStretchMode, VARIANT_BOOL loop, BSTR videoFilename, LONG *pRetVal);
   STDMETHOD(RegisterFont)(BSTR file, LONG *pRetVal);
   STDMETHOD(UnregisterFont)(LONG registeredFont);
   STDMETHOD(DisplayVersionInfo)();
   STDMETHOD(DisplayScoreboard)(LONG cPlayers, LONG highlightedPlayer, LONG score1, LONG score2, LONG score3, LONG score4, BSTR lowerLeft, BSTR lowerRight);
   STDMETHOD(DisplayScoreboard00)(LONG cPlayers, LONG highlightedPlayer, LONG score1, LONG score2, LONG score3, LONG score4, BSTR lowerLeft, BSTR lowerRight);
   STDMETHOD(DisplayScene00)(BSTR background, BSTR toptext, LONG topBrightness, BSTR bottomtext, LONG bottomBrightness, LONG animateIn, LONG pauseTime, LONG animateOut);
   STDMETHOD(DisplayScene00Ex)(BSTR background, BSTR toptext, LONG topBrightness, LONG topOutlineBrightness, BSTR bottomtext, LONG bottomBrightness, LONG bottomOutlineBrightness, LONG animateIn, LONG pauseTime, LONG animateOut);
   STDMETHOD(DisplayScene00ExWithId)(BSTR sceneId, VARIANT_BOOL cancelPrevious, BSTR background, BSTR toptext, LONG topBrightness, LONG topOutlineBrightness, BSTR bottomtext, LONG bottomBrightness, LONG bottomOutlineBrightness, LONG animateIn, LONG pauseTime, LONG animateOut);
   STDMETHOD(ModifyScene00)(BSTR id, BSTR toptext, BSTR bottomtext);
   STDMETHOD(ModifyScene00Ex)(BSTR id, BSTR toptext, BSTR bottomtext, LONG pauseTime);
   STDMETHOD(DisplayScene01)(BSTR sceneId, BSTR background, BSTR text, LONG textBrightness, LONG textOutlineBrightness, LONG animateIn, LONG pauseTime, LONG animateOut);
   STDMETHOD(DisplayText)(BSTR text, LONG textBrightness, LONG textOutlineBrightness);
   STDMETHOD(ScrollingCredits)(BSTR background, BSTR text, LONG textBrightness, LONG animateIn, LONG pauseTime, LONG animateOut);
   STDMETHOD(LoadSetup)();
   STDMETHOD(get_DmdColoredPixels)(VARIANT *pRetVal);
   STDMETHOD(get_DmdPixels)(VARIANT *pRetVal);
   STDMETHOD(get_Width)(unsigned short *pRetVal);
   STDMETHOD(get_Height)(unsigned short *pRetVal);

   FlexDMDUltraDMD* m_pFlexUDMD;
};