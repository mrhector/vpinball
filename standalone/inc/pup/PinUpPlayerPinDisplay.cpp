#include "stdafx.h"

PinUpPlayerPinDisplay::PinUpPlayerPinDisplay()
{
}

PinUpPlayerPinDisplay::~PinUpPlayerPinDisplay()
{
}

STDMETHODIMP PinUpPlayerPinDisplay::Init(long ScreenNum, BSTR RootDir) { return E_NOTIMPL; }
STDMETHODIMP PinUpPlayerPinDisplay::playlistadd(long ScreenNum, BSTR folder, long sort, long restSeconds) { return E_NOTIMPL; }
STDMETHODIMP PinUpPlayerPinDisplay::playlistplay(long ScreenNum, BSTR playlist) { return E_NOTIMPL; }

STDMETHODIMP PinUpPlayerPinDisplay::playlistplayex(long ScreenNum, BSTR playlist, BSTR playfilename, long volume, long forceplay)
{
    return S_OK;
}

STDMETHODIMP PinUpPlayerPinDisplay::play(long ScreenNum, BSTR playlist, BSTR playfilename) { return E_NOTIMPL; }
STDMETHODIMP PinUpPlayerPinDisplay::setWidth(long ScreenNum, long width) { return E_NOTIMPL; }
STDMETHODIMP PinUpPlayerPinDisplay::setHeight(long ScreenNum, long Height) { return E_NOTIMPL; }
STDMETHODIMP PinUpPlayerPinDisplay::setPosX(long ScreenNum, long Posx) { return E_NOTIMPL; }
STDMETHODIMP PinUpPlayerPinDisplay::setPosY(long ScreenNum, long PosY) { return E_NOTIMPL; }
STDMETHODIMP PinUpPlayerPinDisplay::setAspect(long ScreenNum, long aspectWide, long aspectHigh) { return E_NOTIMPL; }
STDMETHODIMP PinUpPlayerPinDisplay::setVolume(long ScreenNum, long vol) { return E_NOTIMPL; }
STDMETHODIMP PinUpPlayerPinDisplay::playpause(long ScreenNum) { return E_NOTIMPL; }
STDMETHODIMP PinUpPlayerPinDisplay::playresume(long ScreenNum) { return E_NOTIMPL; }
STDMETHODIMP PinUpPlayerPinDisplay::playstop(long ScreenNum) { return E_NOTIMPL; }
STDMETHODIMP PinUpPlayerPinDisplay::CloseApp() { return E_NOTIMPL; }
STDMETHODIMP PinUpPlayerPinDisplay::get_isPlaying(long ScreenNum, long *Value) { return E_NOTIMPL; }
STDMETHODIMP PinUpPlayerPinDisplay::put_isPlaying(long ScreenNum, long Value) { return E_NOTIMPL; }
STDMETHODIMP PinUpPlayerPinDisplay::SetLength(long ScreenNum, long StopSecs) { return E_NOTIMPL; }
STDMETHODIMP PinUpPlayerPinDisplay::SetLoop(long ScreenNum, long LoopState) { return E_NOTIMPL; }
STDMETHODIMP PinUpPlayerPinDisplay::SetBackGround(long ScreenNum, long Mode) { return E_NOTIMPL; }
STDMETHODIMP PinUpPlayerPinDisplay::BlockPlay(long ScreenNum, long Mode) { return E_NOTIMPL; }
STDMETHODIMP PinUpPlayerPinDisplay::SetScreen(long ScreenNum) { return E_NOTIMPL; }

STDMETHODIMP PinUpPlayerPinDisplay::SetScreenEx(long ScreenNum, long xpos, long ypos, long swidth, long sheight, long popup) 
{
    return S_OK;
}

STDMETHODIMP PinUpPlayerPinDisplay::get_SN(long *Value) { return E_NOTIMPL; }
STDMETHODIMP PinUpPlayerPinDisplay::put_SN(long Value) { return E_NOTIMPL; }

STDMETHODIMP PinUpPlayerPinDisplay::B2SData(BSTR tIndex, int Value)
{
    return S_OK;
}

STDMETHODIMP PinUpPlayerPinDisplay::get_B2SFilter(BSTR *Value) { return E_NOTIMPL; }
STDMETHODIMP PinUpPlayerPinDisplay::put_B2SFilter(BSTR Value) { return E_NOTIMPL; }
STDMETHODIMP PinUpPlayerPinDisplay::Show(long ScreenNum) { return E_NOTIMPL; }
STDMETHODIMP PinUpPlayerPinDisplay::Hide(long ScreenNum) { return E_NOTIMPL; }

STDMETHODIMP PinUpPlayerPinDisplay::B2SInit(BSTR TName, BSTR RomName)
{
    return S_OK;
}

STDMETHODIMP PinUpPlayerPinDisplay::SendMSG(BSTR cMSG) { return E_NOTIMPL; }

STDMETHODIMP PinUpPlayerPinDisplay::LabelNew(long ScreenNum, BSTR LabelName, BSTR FontName, long Size, long Color, long Angle, long xAlign, long yAlign, long xMargin, long yMargin, long PageNum, long Visible)
{
    return S_OK;
}

STDMETHODIMP PinUpPlayerPinDisplay::LabelSet(long ScreenNum, BSTR LabelName, BSTR Caption, long Visible, BSTR Special)
{
    return S_OK;
}

STDMETHODIMP PinUpPlayerPinDisplay::LabelSetEx() { return E_NOTIMPL; }

STDMETHODIMP PinUpPlayerPinDisplay::LabelShowPage(long ScreenNum, long PageNum, long Seconds, BSTR Special)
{
    return S_OK;
}

STDMETHODIMP PinUpPlayerPinDisplay::LabelInit(long ScreenNum)
{
    return S_OK;
}

STDMETHODIMP PinUpPlayerPinDisplay::get_GetGame(BSTR *Value) { return E_NOTIMPL; }
STDMETHODIMP PinUpPlayerPinDisplay::put_GetGame(BSTR Value) { return E_NOTIMPL; }
STDMETHODIMP PinUpPlayerPinDisplay::get_GetRoot(BSTR *Value) { return E_NOTIMPL; }
STDMETHODIMP PinUpPlayerPinDisplay::put_GetRoot(BSTR Value) { return E_NOTIMPL; }
STDMETHODIMP PinUpPlayerPinDisplay::SoundAdd(BSTR sname, BSTR fname, long svol, double sX, double sy, BSTR SP) { return E_NOTIMPL; }
STDMETHODIMP PinUpPlayerPinDisplay::SoundPlay(BSTR sname) { return E_NOTIMPL; }
STDMETHODIMP PinUpPlayerPinDisplay::PuPSound(BSTR sname, long sX, long sy, long sz, long vol, BSTR SP) { return E_NOTIMPL; }
STDMETHODIMP PinUpPlayerPinDisplay::InitPuPMenu(long Param1) { return E_NOTIMPL; }
STDMETHODIMP PinUpPlayerPinDisplay::get_B2SDisplays(BSTR *Value) { return E_NOTIMPL; }
STDMETHODIMP PinUpPlayerPinDisplay::setVolumeCurrent(long ScreenNum, long vol) { return E_NOTIMPL; }
STDMETHODIMP PinUpPlayerPinDisplay::get_GameUpdate(BSTR GameTitle, long Func, long FuncData, BSTR Extra, long *Value) { return E_NOTIMPL; }
