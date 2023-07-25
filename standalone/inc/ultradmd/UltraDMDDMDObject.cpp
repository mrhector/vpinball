#include "stdafx.h"

#include "UltraDMDDMDObject.h"

UltraDMDDMDObject::UltraDMDDMDObject()
{
   m_pFlexUDMD = FlexDMDUltraDMD::Create();
   LoadSetup();
}

UltraDMDDMDObject::~UltraDMDDMDObject()
{
   m_pFlexUDMD->Release();
}

STDMETHODIMP UltraDMDDMDObject::get_FloatProperty(single *pRetVal)
{
   return m_pFlexUDMD->get_FloatProperty(pRetVal);
}

STDMETHODIMP UltraDMDDMDObject::put_FloatProperty(single pRetVal)
{
   return m_pFlexUDMD->put_FloatProperty(pRetVal);
}

STDMETHODIMP UltraDMDDMDObject::Init()
{
   return m_pFlexUDMD->Init();
}

STDMETHODIMP UltraDMDDMDObject::Uninit()
{
   return m_pFlexUDMD->Uninit();
}

STDMETHODIMP UltraDMDDMDObject::GetMajorVersion(LONG *pRetVal)
{
   return m_pFlexUDMD->GetMajorVersion(pRetVal);
}

STDMETHODIMP UltraDMDDMDObject::GetMinorVersion(LONG *pRetVal)
{
   return m_pFlexUDMD->GetMinorVersion(pRetVal);
}

STDMETHODIMP UltraDMDDMDObject::GetBuildNumber(LONG *pRetVal)
{
   return m_pFlexUDMD->GetBuildNumber(pRetVal);
}

STDMETHODIMP UltraDMDDMDObject::SetVisibleVirtualDMD(VARIANT_BOOL bHide, VARIANT_BOOL *pRetVal)
{
   return m_pFlexUDMD->SetVisibleVirtualDMD(bHide, pRetVal);
}

STDMETHODIMP UltraDMDDMDObject::SetFlipY(VARIANT_BOOL flipY, VARIANT_BOOL *pRetVal)
{
   return m_pFlexUDMD->SetFlipY(flipY, pRetVal);
}

STDMETHODIMP UltraDMDDMDObject::IsRendering(VARIANT_BOOL *pRetVal)
{
   return m_pFlexUDMD->IsRendering(pRetVal);
}

STDMETHODIMP UltraDMDDMDObject::CancelRendering()
{
   return m_pFlexUDMD->CancelRendering();
}

STDMETHODIMP UltraDMDDMDObject::CancelRenderingWithId(BSTR sceneId)
{
   return m_pFlexUDMD->CancelRenderingWithId(sceneId);
}

STDMETHODIMP UltraDMDDMDObject::Clear()
{
   return m_pFlexUDMD->Clear();
}

STDMETHODIMP UltraDMDDMDObject::SetProjectFolder(BSTR basePath)
{
   return m_pFlexUDMD->SetProjectFolder(basePath);
}

STDMETHODIMP UltraDMDDMDObject::SetVideoStretchMode(LONG mode)
{
   return m_pFlexUDMD->SetVideoStretchMode(mode);
}

STDMETHODIMP UltraDMDDMDObject::SetScoreboardBackgroundImage(BSTR filename, LONG selectedBrightness, LONG unselectedBrightness)
{
   return m_pFlexUDMD->SetScoreboardBackgroundImage(filename, selectedBrightness, unselectedBrightness);
}

STDMETHODIMP UltraDMDDMDObject::CreateAnimationFromImages(LONG fps, VARIANT_BOOL loop, BSTR imagelist, LONG *pRetVal)
{
   return m_pFlexUDMD->CreateAnimationFromImages(fps, loop, imagelist, pRetVal);
}

STDMETHODIMP UltraDMDDMDObject::RegisterVideo(LONG videoStretchMode, VARIANT_BOOL loop, BSTR videoFilename, LONG *pRetVal)
{
   return m_pFlexUDMD->RegisterVideo(videoStretchMode, loop, videoFilename, pRetVal);
}

STDMETHODIMP UltraDMDDMDObject::RegisterFont(BSTR file, LONG *pRetVal)
{
   return m_pFlexUDMD->RegisterFont(file, pRetVal);
}

STDMETHODIMP UltraDMDDMDObject::UnregisterFont(LONG registeredFont)
{
   return m_pFlexUDMD->UnregisterFont(registeredFont);
}

STDMETHODIMP UltraDMDDMDObject::DisplayVersionInfo()
{
   return m_pFlexUDMD->DisplayVersionInfo();
}

STDMETHODIMP UltraDMDDMDObject::DisplayScoreboard(LONG cPlayers, LONG highlightedPlayer, LONG score1, LONG score2, LONG score3, LONG score4, BSTR lowerLeft, BSTR lowerRight)
{
   return m_pFlexUDMD->DisplayScoreboard(cPlayers, highlightedPlayer, score1, score2, score3, score4, lowerLeft, lowerRight);
}

STDMETHODIMP UltraDMDDMDObject::DisplayScoreboard00(LONG cPlayers, LONG highlightedPlayer, LONG score1, LONG score2, LONG score3, LONG score4, BSTR lowerLeft, BSTR lowerRight)
{
   return m_pFlexUDMD->DisplayScoreboard00(cPlayers, highlightedPlayer, score1, score2, score3, score4, lowerLeft, lowerRight);
}

STDMETHODIMP UltraDMDDMDObject::DisplayScene00(BSTR background, BSTR toptext, LONG topBrightness, BSTR bottomtext, LONG bottomBrightness, LONG animateIn, LONG pauseTime, LONG animateOut)
{
   return m_pFlexUDMD->DisplayScene00(background, toptext, topBrightness, bottomtext, bottomBrightness, animateIn, pauseTime, animateOut);
}

STDMETHODIMP UltraDMDDMDObject::DisplayScene00Ex(BSTR background, BSTR toptext, LONG topBrightness, LONG topOutlineBrightness, BSTR bottomtext, LONG bottomBrightness, LONG bottomOutlineBrightness, LONG animateIn, LONG pauseTime, LONG animateOut)
{
   return m_pFlexUDMD->DisplayScene00Ex(background, toptext, topBrightness, topOutlineBrightness, bottomtext, bottomBrightness, bottomOutlineBrightness, animateIn, pauseTime, animateOut);
}

STDMETHODIMP UltraDMDDMDObject::DisplayScene00ExWithId(BSTR sceneId, VARIANT_BOOL cancelPrevious, BSTR background, BSTR toptext, LONG topBrightness, LONG topOutlineBrightness, BSTR bottomtext, LONG bottomBrightness, LONG bottomOutlineBrightness, LONG animateIn, LONG pauseTime, LONG animateOut)
{
   return m_pFlexUDMD->DisplayScene00ExWithId(sceneId, cancelPrevious, background, toptext, topBrightness, topOutlineBrightness, bottomtext, bottomBrightness, bottomOutlineBrightness, animateIn, pauseTime, animateOut);
}

STDMETHODIMP UltraDMDDMDObject::ModifyScene00(BSTR id, BSTR toptext, BSTR bottomtext)
{
   return m_pFlexUDMD->ModifyScene00(id, toptext, bottomtext);
}

STDMETHODIMP UltraDMDDMDObject::ModifyScene00Ex(BSTR id, BSTR toptext, BSTR bottomtext, LONG pauseTime)
{
   return m_pFlexUDMD->ModifyScene00Ex(id, toptext, bottomtext, pauseTime);
}

STDMETHODIMP UltraDMDDMDObject::DisplayScene01(BSTR sceneId, BSTR background, BSTR text, LONG textBrightness, LONG textOutlineBrightness, LONG animateIn, LONG pauseTime, LONG animateOut)
{
   return m_pFlexUDMD->DisplayScene01(sceneId, background, text, textBrightness, textOutlineBrightness, animateIn, pauseTime, animateOut);
}

STDMETHODIMP UltraDMDDMDObject::DisplayText(BSTR text, LONG textBrightness, LONG textOutlineBrightness)
{
   return m_pFlexUDMD->DisplayText(text, textBrightness, textOutlineBrightness);
}

STDMETHODIMP UltraDMDDMDObject::ScrollingCredits(BSTR background, BSTR text, LONG textBrightness, LONG animateIn, LONG pauseTime, LONG animateOut)
{
   return m_pFlexUDMD->ScrollingCredits(background, text, textBrightness, animateIn, pauseTime, animateOut);
}

STDMETHODIMP UltraDMDDMDObject::LoadSetup()
{
   return m_pFlexUDMD->LoadSetup();
}

STDMETHODIMP UltraDMDDMDObject::get_DmdColoredPixels(VARIANT *pRetVal)
{
   return m_pFlexUDMD->GetFlexDMD()->get_DmdColoredPixels(pRetVal);
}

STDMETHODIMP UltraDMDDMDObject::get_DmdPixels(VARIANT *pRetVal)
{
   return m_pFlexUDMD->GetFlexDMD()->get_DmdPixels(pRetVal);
}

STDMETHODIMP UltraDMDDMDObject::get_Width(unsigned short *pRetVal)
{
   return m_pFlexUDMD->GetFlexDMD()->get_Width(pRetVal);
}

STDMETHODIMP UltraDMDDMDObject::get_Height(unsigned short *pRetVal)
{
   return m_pFlexUDMD->GetFlexDMD()->get_Height(pRetVal);
}
