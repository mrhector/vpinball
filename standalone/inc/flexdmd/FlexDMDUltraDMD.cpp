#include "stdafx.h"

#include "FlexDMDUltraDMD.h"
#include "scenes/FlexDMDSingleLineScene.h"
#include "scenes/FlexDMDTwoLineScene.h"
#include "scenes/FlexDMDBackgroundScene.h"
#include "scenes/FlexDMDScrollingCreditsScene.h"
#include "actors/FlexDMDLabelActor.h"
#include "actors/FlexDMDFrameActor.h"
#include "actors/FlexDMDGIFImage.h"
#include "actors/FlexDMDVideoActor.h"
#include "actors/FlexDMDImageSequence.h"

#include "ImageSequenceDef.h"
#include "VideoDef.h"

FlexDMDUltraDMD::FlexDMDUltraDMD()
{
   m_visible = true;
   m_stretchMode = 0;
   m_nextId = 1;

   m_pFlexDMD = FlexDMD::Create();
   m_pFlexDMD->SetRenderMode(RenderMode_DMD_RGB);

   m_pQueue = FlexDMDSequence::Create(m_pFlexDMD, "");
   m_pQueue->SetFillParent(true);

   m_pScoreFontText = new FontDef("FlexDMD.Resources.udmd-f4by5.fnt", RGB(168, 168, 168), RGB(255, 255, 255));
   m_pScoreFontNormal = new FontDef("FlexDMD.Resources.udmd-f5by7.fnt", RGB(168, 168, 168), RGB(255, 255, 255));
   m_pScoreFontHighlight = new FontDef("FlexDMD.Resources.udmd-f6by12.fnt", RGB(255, 255, 255), RGB(255, 255, 255));

   m_singleLineFonts.push_back(new FontDef("FlexDMD.Resources.udmd-f14by26.fnt", RGB(255, 255, 255), RGB(255, 255, 255)));
   m_singleLineFonts.push_back(new FontDef("FlexDMD.Resources.udmd-f12by24.fnt", RGB(255, 255, 255), RGB(255, 255, 255)));
   m_singleLineFonts.push_back(new FontDef("FlexDMD.Resources.udmd-f7by13.fnt", RGB(255, 255, 255), RGB(255, 255, 255)));

   m_pTwoLinesFontTop = new FontDef("FlexDMD.Resources.udmd-f5by7.fnt", RGB(255, 255, 255), RGB(255, 255, 255));
   m_pTwoLinesFontBottom = new FontDef("FlexDMD.Resources.udmd-f6by12.fnt", RGB(255, 255, 255), RGB(255, 255, 255));

   m_pScoreBoard = FlexDMDScoreBoard::Create(m_pFlexDMD,
      m_pFlexDMD->NewFont(m_pScoreFontText->GetPath(), m_pScoreFontText->GetTint(), m_pScoreFontText->GetBorderTint(), m_pScoreFontText->GetBorderSize()),
      m_pFlexDMD->NewFont(m_pScoreFontNormal->GetPath(), m_pScoreFontNormal->GetTint(), m_pScoreFontNormal->GetBorderTint(), m_pScoreFontNormal->GetBorderSize()),
      m_pFlexDMD->NewFont(m_pScoreFontHighlight->GetPath(), m_pScoreFontHighlight->GetTint(), m_pScoreFontHighlight->GetBorderTint(), m_pScoreFontHighlight->GetBorderSize()));

   m_pScoreBoard->SetVisible(false);

   m_pFlexDMD->GetStage()->AddActor((FlexDMDActor*)m_pScoreBoard);
   m_pFlexDMD->GetStage()->AddActor((FlexDMDActor*)m_pQueue);
}

FlexDMDUltraDMD::~FlexDMDUltraDMD()
{
   m_pFlexDMD->Release();

   delete m_pScoreFontText;
   delete m_pScoreFontNormal;
   delete m_pScoreFontHighlight;

   for (const auto& pFontDef : m_singleLineFonts)
      delete pFontDef;

   delete m_pTwoLinesFontTop;
   delete m_pTwoLinesFontBottom;
}

CComObject<FlexDMDUltraDMD>* FlexDMDUltraDMD::Create()
{
   CComObject<FlexDMDUltraDMD>* obj = nullptr;
   CComObject<FlexDMDUltraDMD>::CreateInstance(&obj);
   obj->AddRef();

   return obj;
}

STDMETHODIMP FlexDMDUltraDMD::LoadSetup()
{
   return S_OK;
}

STDMETHODIMP FlexDMDUltraDMD::get_FloatProperty(single *pRetVal)
{
   PLOGW << "Not implemented";

   return S_OK;
}

STDMETHODIMP FlexDMDUltraDMD::put_FloatProperty(single pRetVal)
{
   PLOGW << "Not implemented";

   return S_OK;
}

STDMETHODIMP FlexDMDUltraDMD::Init()
{
   return m_pFlexDMD->put_Run(VARIANT_TRUE);
}

STDMETHODIMP FlexDMDUltraDMD::Uninit()
{
   return m_pFlexDMD->put_Run(VARIANT_FALSE);
}

STDMETHODIMP FlexDMDUltraDMD::GetMajorVersion(LONG *pRetVal)
{
   *pRetVal = 1;

   return S_OK;
}

STDMETHODIMP FlexDMDUltraDMD::GetMinorVersion(LONG *pRetVal)
{
   *pRetVal = 9999;

   return S_OK;
}

STDMETHODIMP FlexDMDUltraDMD::GetBuildNumber(LONG *pRetVal)
{
   *pRetVal = 9999;

   return S_OK;
}

STDMETHODIMP FlexDMDUltraDMD::SetVisibleVirtualDMD(VARIANT_BOOL bHide, VARIANT_BOOL *pRetVal)
{
   bool wasVisible = m_visible;

   m_visible = (bHide == VARIANT_TRUE);

   *pRetVal = wasVisible ? VARIANT_TRUE : VARIANT_FALSE;

   return S_OK;
}

STDMETHODIMP FlexDMDUltraDMD::SetFlipY(VARIANT_BOOL flipY, VARIANT_BOOL *pRetVal)
{
   PLOGW << "Not implemented";

   return S_OK;
}

STDMETHODIMP FlexDMDUltraDMD::IsRendering(VARIANT_BOOL *pRetVal)
{
   *pRetVal = !m_pQueue->IsFinished() ? VARIANT_TRUE : VARIANT_FALSE;

   return S_OK;
}

STDMETHODIMP FlexDMDUltraDMD::CancelRendering()
{
   m_pQueue->RemoveAllScenes();

   return S_OK;
}

STDMETHODIMP FlexDMDUltraDMD::CancelRenderingWithId(BSTR sceneId)
{
   m_pQueue->RemoveScene(MakeString(sceneId));

   return S_OK;
}

STDMETHODIMP FlexDMDUltraDMD::Clear()
{
   m_pFlexDMD->GetGraphics()->Clear(RGB(0, 0, 0));
   m_pScoreBoard->SetVisible(false);
   if (m_pQueue->IsFinished())
      m_pQueue->SetVisible(false);

   return S_OK;
}

STDMETHODIMP FlexDMDUltraDMD::SetProjectFolder(BSTR basePath)
{
   return m_pFlexDMD->put_ProjectFolder(basePath);
}

STDMETHODIMP FlexDMDUltraDMD::SetVideoStretchMode(LONG mode)
{
   m_stretchMode = mode;

   return S_OK;
}

FlexDMDLabelActor* FlexDMDUltraDMD::GetFittedLabel(const string& text, float fillBrightness, float outlineBrightness)
{
   for (const auto& pFontDef : m_singleLineFonts) {
      Font* pFont = GetFont(pFontDef->GetPath(), fillBrightness, outlineBrightness);
      FlexDMDLabelActor* pLabel = FlexDMDLabelActor::Create(m_pFlexDMD, pFont, text, "");
      pLabel->SetPosition((m_pFlexDMD->GetWidth() - pLabel->GetWidth()) / 2, (m_pFlexDMD->GetHeight() - pLabel->GetHeight()) / 2);
      if ((pLabel->GetX() >= 0 && pLabel->GetY() >= 0) || pFontDef == m_singleLineFonts[m_singleLineFonts.size() - 1])
         return pLabel;
    }
    return NULL;
}

Font* FlexDMDUltraDMD::GetFont(const string& path, float brightness, float outlineBrightness)
{
   brightness = brightness > 1.f ? 1.f : brightness;
   outlineBrightness = outlineBrightness > 1.f ? 1.f : outlineBrightness;
   OLE_COLOR baseColor = m_pFlexDMD->GetRenderMode() == RenderMode_DMD_RGB ? m_pFlexDMD->GetDMDColor() : RGB(255, 255, 255);

   OLE_COLOR tint = brightness >= 0.0f ? RGB(
      SDL_min((GetRValue(baseColor) * brightness), 255),
      SDL_min((GetGValue(baseColor) * brightness), 255),
      SDL_min((GetBValue(baseColor) * brightness), 255)) : RGB(0, 0, 0);

   if (outlineBrightness >= 0.0f) {
      OLE_COLOR borderTint = RGB(
         SDL_min((GetRValue(baseColor) * outlineBrightness), 255),
         SDL_min((GetGValue(baseColor) * outlineBrightness), 255),
         SDL_min((GetBValue(baseColor) * outlineBrightness), 255));

      return m_pFlexDMD->NewFont(path, tint, borderTint, 1);
   }

   return m_pFlexDMD->NewFont(path, tint, RGB(255, 255, 255), 0);
}

STDMETHODIMP FlexDMDUltraDMD::SetScoreboardBackgroundImage(BSTR filename, LONG selectedBrightness, LONG unselectedBrightness)
{
   m_pScoreBoard->SetBackground(ResolveImage(MakeString(filename), false));

   m_pScoreBoard->SetFonts(
      GetFont(m_pScoreFontNormal->GetPath(), unselectedBrightness / 15.0f, -1),
      GetFont(m_pScoreFontHighlight->GetPath(), selectedBrightness / 15.0f, -1),
      GetFont(m_pScoreFontText->GetPath(), unselectedBrightness / 15.0f, -1));

   return S_OK;
}

FlexDMDActor* FlexDMDUltraDMD::ResolveImage(const string& filename, bool useFrame)
{
   int key;
   if (try_parse_int(filename, key) && m_preloads.find(key) != m_preloads.end()) {
      VideoDef* pVideoDef = dynamic_cast<VideoDef*>(m_preloads[key]);
      if (pVideoDef) {
         FlexDMDVideoActor* pActor = FlexDMDVideoActor::Create(m_pFlexDMD, pVideoDef->GetVideoFilename(), "", pVideoDef->GetLoop());
         pActor->SetLoop(pVideoDef->GetLoop());
         pActor->SetScaling(pVideoDef->GetScaling());
         pActor->SetAlignment(pVideoDef->GetAlignment());
         return pActor;
      }
      else {
         ImageSequenceDef* pImageSequenceDef = dynamic_cast<ImageSequenceDef*>(m_preloads[key]);
         if (pImageSequenceDef) {
            FlexDMDImageSequence* pVideo = FlexDMDImageSequence::Create(m_pFlexDMD, m_pFlexDMD->GetAssetManager(), pImageSequenceDef->GetImages(), "", pImageSequenceDef->GetFPS(), pImageSequenceDef->GetLoop());
            pVideo->SetFPS(pImageSequenceDef->GetFPS());
            pVideo->SetLoop(pImageSequenceDef->GetLoop());
            pVideo->SetScaling(pImageSequenceDef->GetScaling());
            pVideo->SetAlignment(pImageSequenceDef->GetAlignment());
            return pVideo;
         }
      }
   }
   else {
       string path = filename;
       std::replace(path.begin(), path.end(), ',', '|');
       if (path.find("|") != string::npos)
          return FlexDMDImageSequence::Create(m_pFlexDMD, m_pFlexDMD->GetAssetManager(), path, "", 30, true);
       else {
          AssetSrc* pAssetSrc = m_pFlexDMD->GetAssetManager()->ResolveSrc(path, NULL);
          AssetType assetType = pAssetSrc->GetAssetType();
          delete pAssetSrc;

          if (assetType == AssetType_Image)
             return FlexDMDImageActor::Create(m_pFlexDMD, m_pFlexDMD->GetAssetManager(), path, "");
          else if (assetType == AssetType_Video || assetType == AssetType_GIF) {
             CComObject<FlexDMDAnimatedActor>* pActor = m_pFlexDMD->NewVideo(path, "");
             if (pActor != NULL) {
                switch (m_stretchMode) {
                   case 0:
                      pActor->SetScaling(Scaling_Stretch);
                      pActor->SetAlignment(Alignment_Center);
                      break;
                   case 1:
                      pActor->SetScaling(Scaling_FillX);
                      pActor->SetAlignment(Alignment_Top);
                      break;
                   case 2:
                      pActor->SetScaling(Scaling_FillX);
                      pActor->SetAlignment(Alignment_Center);
                      break;
                   case 3:
                      pActor->SetScaling(Scaling_FillX);
                      pActor->SetAlignment(Alignment_Bottom);
                      break;
                }
                return pActor;
             }
          }
       }
   }

   return useFrame ? FlexDMDFrameActor::Create(m_pFlexDMD, "") : FlexDMDActor::Create(m_pFlexDMD, "");
}

STDMETHODIMP FlexDMDUltraDMD::CreateAnimationFromImages(LONG fps, VARIANT_BOOL loop, BSTR imagelist, LONG *pRetVal)
{
   int id = m_nextId;

   string szImagelist = MakeString(imagelist);
   std::replace(szImagelist.begin(), szImagelist.end(), ',', '|');

   m_preloads[id] = new ImageSequenceDef(szImagelist, fps, loop == VARIANT_TRUE);

   m_nextId++;

   *pRetVal = id;

   return S_OK;
}

STDMETHODIMP FlexDMDUltraDMD::RegisterVideo(LONG videoStretchMode, VARIANT_BOOL loop, BSTR videoFilename, LONG *pRetVal)
{
   VideoDef* pVideoDef = new VideoDef(MakeString(videoFilename), loop == VARIANT_TRUE);

   switch (videoStretchMode) {
      case 0:
         pVideoDef->SetScaling(Scaling_Stretch);
         pVideoDef->SetAlignment(Alignment_Center);
         break;
      case 1:
         pVideoDef->SetScaling(Scaling_FillX);
         pVideoDef->SetAlignment(Alignment_Top);
         break;
      case 2:
         pVideoDef->SetScaling(Scaling_FillX);
         pVideoDef->SetAlignment(Alignment_Center);
         break;
      case 3:
         pVideoDef->SetScaling(Scaling_FillX);
         pVideoDef->SetAlignment(Alignment_Bottom);
         break;
   }

   for (const auto& kv : m_preloads) {
      VideoDef* pObject = dynamic_cast<VideoDef*>(kv.second);
      if (pObject) {
         if (*pObject == *pVideoDef) {
            *pRetVal = kv.first;
            return S_OK;
         }
      }
   }

   int id = m_nextId;

   m_preloads[id] = pVideoDef;

   m_nextId++;

   *pRetVal = id;

   return S_OK;
}

STDMETHODIMP FlexDMDUltraDMD::RegisterFont(BSTR file, LONG *pRetVal)
{
   PLOGW << "Not implemented";

   return S_OK;
}

STDMETHODIMP FlexDMDUltraDMD::UnregisterFont(LONG registeredFont)
{
   PLOGW << "Not implemented";

   return S_OK;
}

STDMETHODIMP FlexDMDUltraDMD::DisplayVersionInfo()
{
   m_pScoreBoard->SetVisible(false);
   m_pQueue->SetVisible(false);

   return S_OK;
}

STDMETHODIMP FlexDMDUltraDMD::DisplayScoreboard(LONG cPlayers, LONG highlightedPlayer, LONG score1, LONG score2, LONG score3, LONG score4, BSTR lowerLeft, BSTR lowerRight)
{
   m_pScoreBoard->SetNPlayers(cPlayers);
   m_pScoreBoard->SetHighlightedPlayer(highlightedPlayer);
   m_pScoreBoard->SetScore(score1, score2, score3, score4);
   m_pScoreBoard->m_pLowerLeft->SetText(MakeString(lowerLeft));
   m_pScoreBoard->m_pLowerRight->SetText(MakeString(lowerRight));
   if (m_pQueue->IsFinished()) {
      m_pQueue->SetVisible(false);
      m_pScoreBoard->SetVisible(true);
   }

   return S_OK;
}

STDMETHODIMP FlexDMDUltraDMD::DisplayScoreboard00(LONG cPlayers, LONG highlightedPlayer, LONG score1, LONG score2, LONG score3, LONG score4, BSTR lowerLeft, BSTR lowerRight)
{
   return DisplayScoreboard(cPlayers, highlightedPlayer, score1, score2, score3, score4, lowerLeft, lowerRight);
}

STDMETHODIMP FlexDMDUltraDMD::DisplayScene00(BSTR background, BSTR toptext, LONG topBrightness, BSTR bottomtext, LONG bottomBrightness, LONG animateIn, LONG pauseTime, LONG animateOut)
{
   BSTR sceneId = SysAllocString(L"");
   HRESULT hres = DisplayScene00ExWithId(sceneId, false, background, toptext, topBrightness, -15, bottomtext, bottomBrightness, -15, animateIn, pauseTime, animateOut);
   SysFreeString(sceneId);

   return hres;
}

STDMETHODIMP FlexDMDUltraDMD::DisplayScene00Ex(BSTR background, BSTR toptext, LONG topBrightness, LONG topOutlineBrightness, BSTR bottomtext, LONG bottomBrightness, LONG bottomOutlineBrightness, LONG animateIn, LONG pauseTime, LONG animateOut)
{
   BSTR sceneId = SysAllocString(L"");
   HRESULT hres = DisplayScene00ExWithId(sceneId, false, background, toptext, topBrightness, topOutlineBrightness, bottomtext, bottomBrightness, bottomOutlineBrightness, animateIn, pauseTime, animateOut);
   SysFreeString(sceneId);

   return hres;
}

STDMETHODIMP FlexDMDUltraDMD::DisplayScene00ExWithId(BSTR sceneId, VARIANT_BOOL cancelPrevious, BSTR background, BSTR toptext, LONG topBrightness, LONG topOutlineBrightness, BSTR bottomtext, LONG bottomBrightness, LONG bottomOutlineBrightness, LONG animateIn, LONG pauseTime, LONG animateOut)
{
   string szSceneId = (sceneId != NULL) ? MakeString(sceneId) : "";

   if (cancelPrevious == VARIANT_TRUE && szSceneId.length() > 0) {
      FlexDMDScene* pScene = m_pQueue->GetActiveScene();
      if (pScene != NULL && pScene->GetName() == szSceneId)
         m_pQueue->RemoveScene(szSceneId);
   }
   m_pScoreBoard->SetVisible(false);
   m_pQueue->SetVisible(true);

   string szTopText = (toptext != NULL) ? MakeString(toptext) : "";
   string szBottomText = (bottomtext != NULL) ? MakeString(bottomtext) : "";

   if (szTopText.length() > 0 && szBottomText.length() > 0) {
      Font* pFontTop = GetFont(m_pTwoLinesFontTop->GetPath(), topBrightness / 15.0f, topOutlineBrightness / 15.0f);
      Font* pFontBottom = GetFont(m_pTwoLinesFontBottom->GetPath(), bottomBrightness / 15.0f, bottomOutlineBrightness / 15.0f);
      FlexDMDTwoLineScene* pScene = FlexDMDTwoLineScene::Create(m_pFlexDMD, ResolveImage(MakeString(background), true), szTopText, pFontTop, szBottomText, pFontBottom, (AnimationType)animateIn, pauseTime / 1000.0f, (AnimationType)animateOut, szSceneId);
      m_pQueue->Enqueue(pScene);
   }
   else if (szTopText.length() > 0) {
      Font* pFont = GetFittedLabel(szTopText, topBrightness / 15.0f, topOutlineBrightness / 15.0f)->GetFont();
      FlexDMDSingleLineScene* pScene = FlexDMDSingleLineScene::Create(m_pFlexDMD, ResolveImage(MakeString(background), true), szTopText, pFont, (AnimationType)animateIn, pauseTime / 1000.0f, (AnimationType)animateOut, false, szSceneId);
      m_pQueue->Enqueue(pScene);
   }
   else if (szBottomText.length() > 0) {
      Font* pFont = GetFittedLabel(szBottomText, bottomBrightness / 15.0f, bottomOutlineBrightness / 15.0f)->GetFont();
      FlexDMDSingleLineScene* pScene = FlexDMDSingleLineScene::Create(m_pFlexDMD, ResolveImage(MakeString(background), true), szBottomText, pFont, (AnimationType)animateIn, pauseTime / 1000.0f, (AnimationType)animateOut, false, szSceneId);
      m_pQueue->Enqueue(pScene);
   }
   else {
      FlexDMDBackgroundScene* pScene = FlexDMDBackgroundScene::Create(m_pFlexDMD, ResolveImage(MakeString(background), true), (AnimationType)animateIn, pauseTime / 1000.0f, (AnimationType)animateOut, szSceneId);
      m_pQueue->Enqueue(pScene);
   }

   return S_OK;
}

STDMETHODIMP FlexDMDUltraDMD::ModifyScene00(BSTR id, BSTR toptext, BSTR bottomtext)
{
   string szId = (id != NULL) ? MakeString(id) : "";

   FlexDMDScene* pScene = m_pQueue->GetActiveScene();
   if (pScene != NULL && szId.length() > 0 && pScene->GetName() == szId) {
      FlexDMDTwoLineScene* pScene2 = dynamic_cast<FlexDMDTwoLineScene*>(pScene);
      if (pScene2 != NULL)
         pScene2->SetText(MakeString(toptext), MakeString(bottomtext));
      FlexDMDSingleLineScene* pScene1 = dynamic_cast<FlexDMDSingleLineScene*>(pScene);
      if (pScene1 != NULL)
         pScene1->SetText(MakeString(toptext));
   }

   return S_OK;
}

STDMETHODIMP FlexDMDUltraDMD::ModifyScene00Ex(BSTR id, BSTR toptext, BSTR bottomtext, LONG pauseTime)
{
   string szId = (id != NULL) ? MakeString(id) : "";

   FlexDMDScene* pScene = m_pQueue->GetActiveScene();
   if (pScene != NULL && szId.length() > 0 && pScene->GetName() == szId) {
      FlexDMDTwoLineScene* pScene2 = dynamic_cast<FlexDMDTwoLineScene*>(pScene);
      if (pScene2 != NULL)
         pScene2->SetText(MakeString(toptext), MakeString(bottomtext));
      FlexDMDSingleLineScene* pScene1 = dynamic_cast<FlexDMDSingleLineScene*>(pScene);
      if (pScene1 != NULL)
         pScene1->SetText(MakeString(toptext));
      pScene->SetPause(pScene->GetTime() + pauseTime / 1000.0f);
   }

   return S_OK;
}

STDMETHODIMP FlexDMDUltraDMD::DisplayScene01(BSTR sceneId, BSTR background, BSTR text, LONG textBrightness, LONG textOutlineBrightness, LONG animateIn, LONG pauseTime, LONG animateOut)
{
   string szSceneId = (sceneId != NULL) ? MakeString(sceneId) : "";

   Font* pFont = GetFont(m_singleLineFonts[0]->GetPath(), textBrightness / 15.0f, textOutlineBrightness / 15.0f);
   FlexDMDSingleLineScene* pScene = FlexDMDSingleLineScene::Create(m_pFlexDMD, ResolveImage(MakeString(background), false), MakeString(text), pFont, (AnimationType)animateIn, pauseTime / 1000.0f, (AnimationType)animateOut, true, szSceneId);
   m_pScoreBoard->SetVisible(false);
   m_pQueue->SetVisible(true);
   m_pQueue->Enqueue(pScene);

   return S_OK;
}

STDMETHODIMP FlexDMDUltraDMD::DisplayText(BSTR text, LONG textBrightness, LONG textOutlineBrightness)
{
   m_pScoreBoard->SetVisible(false);
   if (m_pQueue->IsFinished()) {
      m_pQueue->SetVisible(false);
      GetFittedLabel(MakeString(text), textBrightness / 15.0f, textOutlineBrightness / 15.0f)->Draw(m_pFlexDMD->GetGraphics());
   }

   return S_OK;
}

STDMETHODIMP FlexDMDUltraDMD::ScrollingCredits(BSTR background, BSTR text, LONG textBrightness, LONG animateIn, LONG pauseTime, LONG animateOut)
{
   m_pScoreBoard->SetVisible(false);

   vector<string> lines;

   string szText = MakeString(text);
   std::replace(szText.begin(), szText.end(), '|', '\n');

   std::stringstream ss(szText);
   string line;

   while (std::getline(ss, line, '\n'))
      lines.push_back(line);

   Font* pFont = GetFont(m_pScoreFontText->GetPath(), textBrightness / 15.0f, -1);
   FlexDMDScrollingCreditsScene* pScene = FlexDMDScrollingCreditsScene::Create(m_pFlexDMD, ResolveImage(MakeString(background), false), lines, pFont, (AnimationType)animateIn, pauseTime / 1000.0f, (AnimationType)animateOut, "");
   m_pQueue->SetVisible(true);
   m_pQueue->Enqueue(pScene);

   return S_OK;
}