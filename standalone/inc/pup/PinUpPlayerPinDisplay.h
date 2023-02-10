#pragma once
#if !defined(PINUPPLAYERPINDISPLAY_C_H__INCLUDED_)
#define PINUPPLAYERPINDISPLAY_C_H__INCLUDED_

#pragma GCC visibility push(hidden)

#include "pinupplayer_i.h"

class PinUpPlayerPinDisplay :
   public IDispatchImpl<IPinDisplay, &IID_IPinDisplay, &LIBID_PinUpPlayer>,
   public CComObjectRoot,
   public CComCoClass<PinUpPlayerPinDisplay, &CLSID_PinDisplay>
{
public:
   static name_disp_id_t m_nameIDMap[];
   STDMETHOD(GetIDsOfNames)(REFIID /*riid*/, LPOLESTR* rgszNames, UINT cNames, LCID lcid,DISPID* rgDispId);
   STDMETHOD(Invoke)(DISPID dispIdMember, REFIID /*riid*/, LCID lcid, WORD wFlags, DISPPARAMS* pDispParams, VARIANT* pVarResult, EXCEPINFO* pExcepInfo, UINT* puArgErr);
   STDMETHOD(GetDocumentation)(INT index, BSTR *pBstrName, BSTR *pBstrDocString, DWORD *pdwHelpContext, BSTR *pBstrHelpFile);

public:
   PinUpPlayerPinDisplay();
   ~PinUpPlayerPinDisplay();

   BEGIN_COM_MAP(PinUpPlayerPinDisplay)
      COM_INTERFACE_ENTRY(IPinDisplay)
      COM_INTERFACE_ENTRY(IDispatch)
   END_COM_MAP()

   STDMETHOD(Init)(long ScreenNum, BSTR RootDir);
   STDMETHOD(playlistadd)(long ScreenNum, BSTR folder, long sort, long restSeconds);
   STDMETHOD(playlistplay)(long ScreenNum, BSTR playlist);
   STDMETHOD(playlistplayex)(long ScreenNum, BSTR playlist, BSTR playfilename, long volume, long forceplay);
   STDMETHOD(play)(long ScreenNum, BSTR playlist, BSTR playfilename);
   STDMETHOD(setWidth)(long ScreenNum, long width);
   STDMETHOD(setHeight)(long ScreenNum, long Height);
   STDMETHOD(setPosX)(long ScreenNum, long Posx);
   STDMETHOD(setPosY)(long ScreenNum, long PosY);
   STDMETHOD(setAspect)(long ScreenNum, long aspectWide, long aspectHigh);
   STDMETHOD(setVolume)(long ScreenNum, long vol);
   STDMETHOD(playpause)(long ScreenNum);
   STDMETHOD(playresume)(long ScreenNum);
   STDMETHOD(playstop)(long ScreenNum);
   STDMETHOD(CloseApp)();
   STDMETHOD(get_isPlaying)(long ScreenNum, long *Value);
   STDMETHOD(put_isPlaying)(long ScreenNum, long Value);
   STDMETHOD(SetLength)(long ScreenNum, long StopSecs);
   STDMETHOD(SetLoop)(long ScreenNum, long LoopState);
   STDMETHOD(SetBackGround)(long ScreenNum, long Mode);
   STDMETHOD(BlockPlay)(long ScreenNum, long Mode);
   STDMETHOD(SetScreen)(long ScreenNum);
   STDMETHOD(SetScreenEx)(long ScreenNum, long xpos, long ypos, long swidth, long sheight, long popup);
   STDMETHOD(get_SN)(long *Value);
   STDMETHOD(put_SN)(long Value);
   STDMETHOD(B2SData)(BSTR tIndex, int Value);
   STDMETHOD(get_B2SFilter)(BSTR *Value);
   STDMETHOD(put_B2SFilter)(BSTR Value);
   STDMETHOD(Show)(long ScreenNum);
   STDMETHOD(Hide)(long ScreenNum);
   STDMETHOD(B2SInit)(BSTR TName, BSTR RomName);
   STDMETHOD(SendMSG)(BSTR cMSG);
   STDMETHOD(LabelNew)(long ScreenNum, BSTR LabelName, BSTR FontName, long Size, long Color, long Angle, long xAlign, long yAlign, long xMargin, long yMargin, long PageNum, long Visible);
   STDMETHOD(LabelSet)(long ScreenNum, BSTR LabelName, BSTR Caption, long Visible, BSTR Special);
   STDMETHOD(LabelSetEx)();
   STDMETHOD(LabelShowPage)(long ScreenNum, long PageNum, long Seconds, BSTR Special);
   STDMETHOD(LabelInit)(long ScreenNum);
   STDMETHOD(get_GetGame)(BSTR *Value);
   STDMETHOD(put_GetGame)(BSTR Value);
   STDMETHOD(get_GetRoot)(BSTR *Value);
   STDMETHOD(put_GetRoot)(BSTR Value);
   STDMETHOD(SoundAdd)(BSTR sname, BSTR fname, long svol, double sX, double sy, BSTR SP);
   STDMETHOD(SoundPlay)(BSTR sname);
   STDMETHOD(PuPSound)(BSTR sname, long sX, long sy, long sz, long vol, BSTR SP);
   STDMETHOD(InitPuPMenu)(long Param1);
   STDMETHOD(get_B2SDisplays)(BSTR *Value);
   STDMETHOD(setVolumeCurrent)(long ScreenNum, long vol);
   STDMETHOD(get_GameUpdate)(BSTR GameTitle, long Func, long FuncData, BSTR Extra, long *Value);
};

#pragma GCC visibility pop

#endif // !defined(PINUPPLAYERPINDISPLAY_C_H__INCLUDED_)
