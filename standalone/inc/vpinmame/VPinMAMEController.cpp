#include "stdafx.h"
#include "VPinMAMEController.h"
#include "VPinMAMEGames.h"
#include "inc/mINI/ini.h"

void CALLBACK VPinMAMEController::GetGameCallback(PinmameGame* pPinmameGame, const void* pUserData)
{
   ((VPinMAMEController*)pUserData)->GetGameCallback(pPinmameGame);
}

void CALLBACK VPinMAMEController::OnDisplayAvailable(int index, int displayCount, PinmameDisplayLayout *p_displayLayout, const void* pUserData)
{
   ((VPinMAMEController*)pUserData)->OnDisplayAvailable(index, displayCount, p_displayLayout);
}

void CALLBACK VPinMAMEController::OnDisplayUpdated(int index, void* p_displayData, PinmameDisplayLayout* p_displayLayout, const void* pUserData)
{
   ((VPinMAMEController*)pUserData)->OnDisplayUpdated(index, p_displayData, p_displayLayout);
}

int CALLBACK VPinMAMEController::OnAudioAvailable(PinmameAudioInfo* p_audioInfo, const void* pUserData)
{
   return ((VPinMAMEController*)pUserData)->OnAudioAvailable(p_audioInfo);
}

int CALLBACK VPinMAMEController::OnAudioUpdated(void* p_buffer, int samples, const void* pUserData)
{
   return ((VPinMAMEController*)pUserData)->OnAudioUpdated(p_buffer, samples);
}

void CALLBACK VPinMAMEController::OnMechAvailable(int mechNo, PinmameMechInfo* p_mechInfo, const void* pUserData)
{
   ((VPinMAMEController*)pUserData)->OnMechAvailable(mechNo, p_mechInfo);
}

void CALLBACK VPinMAMEController::OnMechUpdated(int mechNo, PinmameMechInfo* p_mechInfo, const void* pUserData)
{
   ((VPinMAMEController*)pUserData)->OnMechUpdated(mechNo, p_mechInfo);
}

void CALLBACK VPinMAMEController::OnLogMessage(const char* format, va_list args, const void* pUserData)
{
   char buffer[1024];
   vsnprintf(buffer, sizeof(buffer), format, args);

   PLOGI.printf("%s", buffer);
}

void CALLBACK VPinMAMEController::GetGameCallback(PinmameGame* pPinmameGame)
{
   if (m_pPinmameGame)
      delete m_pPinmameGame;

   m_pPinmameGame = new PinmameGame();
   memcpy(m_pPinmameGame, pPinmameGame, sizeof(PinmameGame));
}

void VPinMAMEController::OnDisplayAvailable(int index, int displayCount, PinmameDisplayLayout *p_displayLayout)
{
   PLOGI.printf("index=%d, displayCount=%d, type=%d, top=%d, left=%d, width=%d, height=%d, depth=%d, length=%d", 
      index,
      displayCount,
      p_displayLayout->type, 
      p_displayLayout->top,
      p_displayLayout->left, 
      p_displayLayout->width, 
      p_displayLayout->height, 
      p_displayLayout->depth, 
      p_displayLayout->length);

   if ((p_displayLayout->type & DMD) == DMD && index == 0) {
      m_dmdHeight = p_displayLayout->height;
      m_dmdWidth = p_displayLayout->width;

      m_pDmdData = (UINT8*)malloc(m_dmdWidth * m_dmdHeight);
      memset(m_pDmdData, 0,  m_dmdWidth * m_dmdHeight);

      if (!m_serum.Load(m_szPath + "altcolor", string(m_pPinmameGame->name))) {
         m_pDmdLevels = (UINT8*)((p_displayLayout->depth == 2) ? LEVELS_WPC :
            (PinmameGetHardwareGen() & PINMAME_HARDWARE_GEN::SAM | PINMAME_HARDWARE_GEN::SPA) ? LEVELS_SAM : LEVELS_GTS3);

         unsigned int colors = (p_displayLayout->depth == 2) ? 4 : 16;

         UINT8 r = m_dmdColor & 0xFF;
         UINT8 g = (m_dmdColor >> 8) & 0xFF;
         UINT8 b = (m_dmdColor >> 16) & 0xFF;

         for (int i = 0; i < colors; i++) {
            float perc = calc_brightness((float)i / (float)(colors - 1));
            int pos = i * 3;
            m_dmdPalette[pos] = (UINT8)r * perc;
            m_dmdPalette[pos + 1] = (UINT8)g * perc;
            m_dmdPalette[pos + 2] = (UINT8)b * perc;
         }
      }

#if !((defined(__APPLE__) && ((defined(TARGET_OS_IOS) && TARGET_OS_IOS) || (defined(TARGET_OS_TV) && TARGET_OS_TV))))
      m_zedmd.Open(m_dmdWidth, m_dmdHeight);

      if (!m_serum.IsLoaded())
         m_zedmd.SetPalette(m_dmdPalette);
#endif
   }
}

void VPinMAMEController::OnDisplayUpdated(int index, void* p_displayData, PinmameDisplayLayout* p_displayLayout)
{
   if ((p_displayLayout->type & DMD) == DMD && index == 0) {
      std::unique_lock<std::mutex> lock(m_dmdMutex);

      if (!m_serum.IsLoaded()) {
         if (p_displayData) {
            memcpy(m_pDmdData, p_displayData, m_dmdWidth * m_dmdHeight);
            m_dmdUpdate = true;

#if !((defined(__APPLE__) && ((defined(TARGET_OS_IOS) && TARGET_OS_IOS) || (defined(TARGET_OS_TV) && TARGET_OS_TV))))
            if (p_displayLayout->depth == 2)
               m_zedmd.RenderGray2(m_pDmdData);
            else
               m_zedmd.RenderGray4(m_pDmdData);
#endif
         }
      }
      else if (m_serum.Convert((UINT8*)p_displayData, m_pDmdData, m_dmdPalette)) {
         m_dmdUpdate = true;

#if !((defined(__APPLE__) && ((defined(TARGET_OS_IOS) && TARGET_OS_IOS) || (defined(TARGET_OS_TV) && TARGET_OS_TV))))
         m_zedmd.RenderColoredGray6(m_pDmdData, m_dmdPalette, NULL);
#endif
      }

      lock.unlock();
   }
}

int VPinMAMEController::OnAudioAvailable(PinmameAudioInfo* p_audioInfo)
{
   PLOGI.printf("format=%d, channels=%d, sampleRate=%.2f, framesPerSecond=%.2f, samplesPerFrame=%d, bufferSize=%d", 
      p_audioInfo->format,
      p_audioInfo->channels,
      p_audioInfo->sampleRate,
      p_audioInfo->framesPerSecond,
      p_audioInfo->samplesPerFrame,
      p_audioInfo->bufferSize);

   m_pAudioPlayer = new AudioPlayer();
   m_pAudioPlayer->StreamInit(p_audioInfo->sampleRate, p_audioInfo->channels, 1.);
   m_audioChannels = p_audioInfo->channels;

   return p_audioInfo->samplesPerFrame;
}

int VPinMAMEController::OnAudioUpdated(void* p_buffer, int samples)
{
   m_pAudioPlayer->StreamUpdate(p_buffer, samples * m_audioChannels * sizeof(int16_t));

   return samples;
}

void VPinMAMEController::OnMechAvailable(int mechNo, PinmameMechInfo* p_mechInfo)
{
   PLOGI.printf("mechNo=%d, type=%d, length=%d, steps=%d, pos=%d, speed=%d", 
      mechNo, 
      p_mechInfo->type, 
      p_mechInfo->length, 
      p_mechInfo->steps,
      p_mechInfo->pos, 
      p_mechInfo->speed);
}

void VPinMAMEController::OnMechUpdated(int mechNo, PinmameMechInfo* p_mechInfo)
{
   PLOGD.printf("mechNo=%d, type=%d, length=%d, steps=%d, pos=%d, speed=%d",
      mechNo, 
      p_mechInfo->type, 
      p_mechInfo->length, 
      p_mechInfo->steps, 
      p_mechInfo->pos,
      p_mechInfo->speed);

   m_mechData[mechNo] = p_mechInfo->pos;
}

VPinMAMEController::VPinMAMEController()
{
   PinmameConfig config = { 
      AUDIO_FORMAT_INT16, 
      44100, 
      "", 
      NULL,
      &VPinMAMEController::OnDisplayAvailable,
      &VPinMAMEController::OnDisplayUpdated,
      &VPinMAMEController::OnAudioAvailable,
      &VPinMAMEController::OnAudioUpdated,
      &VPinMAMEController::OnMechAvailable,
      &VPinMAMEController::OnMechUpdated,
      NULL,
      NULL,
      NULL,
      &VPinMAMEController::OnLogMessage,
   };

#if (defined(__APPLE__) && ((defined(TARGET_OS_IOS) && TARGET_OS_IOS) || (defined(TARGET_OS_TV) && TARGET_OS_TV))) || defined(__ANDROID__)
   m_szPath = g_pvp->m_szMyPath + "pinmame" + PATH_SEPARATOR_CHAR;
#else
   m_szPath = string(getenv("HOME")) + PATH_SEPARATOR_CHAR + ".pinmame" + PATH_SEPARATOR_CHAR;
#endif
   strncpy_s((char*)config.vpmPath, m_szPath.c_str(), PINMAME_MAX_PATH - 1);

   PinmameSetConfig(&config);
   PinmameSetUserData((void*)this);
   PinmameSetDmdMode(PINMAME_DMD_MODE::RAW);

   PinmameSetHandleKeyboard(0);
   PinmameSetHandleMechanics(0xFF);

   m_pSolenoidBuffer = new PinmameSolenoidState[PinmameGetMaxSolenoids()];
   m_pLampBuffer = new PinmameLampState[PinmameGetMaxLamps()];
   m_pGIBuffer = new PinmameGIState[PinmameGetMaxGIs()];
   m_pLEDBuffer = new PinmameLEDState[PinmameGetMaxLEDs()];

   m_pPinmameGame = NULL;
   m_pPinmameMechConfig = NULL;

   m_pDmdData = NULL;

   m_dmdColor = RGB(255, 88, 32);

   m_rgb = false;

   m_pGames = VPinMAMEGames::Create(this);
}

VPinMAMEController::~VPinMAMEController()
{
   if (PinmameIsRunning())
      PinmameStop();

   if (m_pDmdData)
      free(m_pDmdData);

   if (m_pSolenoidBuffer)
      delete m_pSolenoidBuffer;

   if (m_pLampBuffer)
      delete m_pLampBuffer;

   if (m_pGIBuffer)
      delete m_pGIBuffer;

   if (m_pLEDBuffer)
      delete m_pLEDBuffer;

   if (m_pPinmameGame)
      delete m_pPinmameGame;

   if (m_pPinmameMechConfig)
      delete m_pPinmameMechConfig;

   m_pGames->Release();
}

STDMETHODIMP VPinMAMEController::Run(/*[in]*/ LONG_PTR hParentWnd, /*[in,defaultvalue(100)]*/ int nMinVersion)
{
   if (m_pPinmameGame) {
      mINI::INIStructure ini;
      mINI::INIFile file(m_szPath + "vpinball" + PATH_SEPARATOR_CHAR + m_pPinmameGame->name + ".ini");
      if (file.read(ini))
         if (ini.has("settings") && ini["settings"].has("dmd_red") && ini["settings"].has("dmd_green") && ini["settings"].has("dmd_blue"))
            m_dmdColor = RGB(atoll(ini["settings"]["dmd_red"].c_str()), atoll(ini["settings"]["dmd_green"].c_str()), atoll(ini["settings"]["dmd_blue"].c_str()));

      UINT8 r = m_dmdColor & 0xFF;
      UINT8 g = (m_dmdColor >> 8) & 0xFF;
      UINT8 b = (m_dmdColor >> 16) & 0xFF;

      ini["settings"]["dmd_red"] = std::to_string(r);
      ini["settings"]["dmd_green"] = std::to_string(g);
      ini["settings"]["dmd_blue"] = std::to_string(b);

      file.write(ini);

      PINMAME_STATUS status = PinmameRun(m_pPinmameGame->name);

      if (status == PINMAME_STATUS::OK) {
         int timeout = 0;

         while (!PinmameIsRunning() && timeout < 20) {
            SDL_Delay(75);
            timeout++;
         }

         m_rgb = (PinmameGetHardwareGen() == PINMAME_HARDWARE_GEN::SAM);

         return S_OK;
      }

      if (status == PINMAME_STATUS::GAME_ALREADY_RUNNING) {
         PLOGI.printf("Game already running.");
      }
   }

   return S_FALSE;
}

STDMETHODIMP VPinMAMEController::Stop()
{
   if (PinmameIsRunning())
      PinmameStop();

   m_rgb = false;

   if (m_pDmdData) {
      free(m_pDmdData);
      m_pDmdData = NULL;
   }

   return S_OK;
}

STDMETHODIMP VPinMAMEController::get_Lamp(int nLamp, VARIANT_BOOL *pVal)
{
   *pVal = PinmameGetLamp(nLamp) ? VARIANT_TRUE : VARIANT_FALSE;

   return S_OK;
}

STDMETHODIMP VPinMAMEController::get_Solenoid(int nSolenoid, VARIANT_BOOL *pVal)
{
   *pVal = PinmameGetSolenoid(nSolenoid) ? VARIANT_TRUE : VARIANT_FALSE;

   return S_OK;
}

STDMETHODIMP VPinMAMEController::get_Switch(int nSwitchNo, VARIANT_BOOL *pVal)
{
   *pVal = PinmameGetSwitch(nSwitchNo) ? VARIANT_TRUE : VARIANT_FALSE;

   return S_OK;
}

STDMETHODIMP VPinMAMEController::put_Switch(int nSwitchNo, VARIANT_BOOL newVal)
{
   PinmameSetSwitch(nSwitchNo, (newVal == VARIANT_TRUE) ? 1 : 0);

   PLOGD.printf("switchNo=%d, newVal=%d", nSwitchNo, newVal == VARIANT_TRUE);

   return S_OK;
}

STDMETHODIMP VPinMAMEController::get_Pause(VARIANT_BOOL *pVal)
{
   *pVal = PinmameIsPaused() ? VARIANT_TRUE : VARIANT_FALSE;

   return S_OK;
}

STDMETHODIMP VPinMAMEController::put_Pause(VARIANT_BOOL newVal)
{
   PinmamePause((newVal == VARIANT_TRUE) ? 1 : 0);

   return S_OK;
}

STDMETHODIMP VPinMAMEController::get_WPCNumbering(/*[out, retval]*/ VARIANT_BOOL *pVal) { return E_NOTIMPL; }
STDMETHODIMP VPinMAMEController::get_Lamps(VARIANT *pVal) { return E_NOTIMPL; }

STDMETHODIMP VPinMAMEController::get_RawDmdWidth(int *pVal)
{
   *pVal = m_dmdWidth;

   return S_OK;
}

STDMETHODIMP VPinMAMEController::get_RawDmdHeight(int *pVal)
{
   *pVal = m_dmdHeight;

   return S_OK;
}

STDMETHODIMP VPinMAMEController::get_NVRAM(VARIANT *pVal) { return E_NOTIMPL; }
STDMETHODIMP VPinMAMEController::get_ChangedNVRAM(VARIANT *pVal) { return E_NOTIMPL; }

STDMETHODIMP VPinMAMEController::get_RawDmdPixels(VARIANT *pVal)
{
   if (!m_dmdUpdate || m_serum.IsLoaded())
      return S_FALSE;

   std::unique_lock<std::mutex> lock(m_dmdMutex);

   SAFEARRAY *psa = SafeArrayCreateVector(VT_VARIANT, 0, m_dmdWidth * m_dmdHeight);

   VARIANT *pData;
   SafeArrayAccessData(psa, (void **)&pData);
   const UINT32 end = m_dmdWidth * m_dmdHeight;
   for (UINT32 i = 0; i < end; i++) {
      V_VT(&pData[i]) = VT_UI1;
      V_UI1(&pData[i]) = m_pDmdLevels[m_pDmdData[i]];
   }
   SafeArrayUnaccessData(psa);

   V_VT(pVal) = VT_ARRAY | VT_VARIANT;
   V_ARRAY(pVal) = psa;

   m_dmdUpdate = false;

   lock.unlock();

   return S_OK;
}

STDMETHODIMP VPinMAMEController::get_RawDmdColoredPixels(VARIANT *pVal)
{
   if (!m_dmdUpdate)
      return S_FALSE;

   std::unique_lock<std::mutex> lock(m_dmdMutex);

   SAFEARRAY *psa = SafeArrayCreateVector(VT_VARIANT, 0, m_dmdWidth * m_dmdHeight);

   VARIANT *pData;
   SafeArrayAccessData(psa, (void **)&pData);
   const UINT32 end = m_dmdWidth * m_dmdHeight;
   for (UINT32 i = 0; i < end; i++) {
      int pos = m_pDmdData[i] * 3;
      int r = m_dmdPalette[pos];
      int g = m_dmdPalette[pos + 1];
      int b = m_dmdPalette[pos + 2];
      V_VT(&pData[i]) = VT_UI4;
      V_UI4(&pData[i]) = r | g << 8 | b << 16;
   }
   SafeArrayUnaccessData(psa);

   V_VT(pVal) = VT_ARRAY | VT_VARIANT;
   V_ARRAY(pVal) = psa;

   m_dmdUpdate = false;

   lock.unlock();

   return S_OK;
}

STDMETHODIMP VPinMAMEController::get_DmdWidth(int *pVal) { return E_NOTIMPL; }
STDMETHODIMP VPinMAMEController::get_DmdHeight(int *pVal) { return E_NOTIMPL; }
STDMETHODIMP VPinMAMEController::get_DmdPixel(int x, int y, int *pVal) { return E_NOTIMPL; }
STDMETHODIMP VPinMAMEController::get_updateDmdPixels(int **buf, int width, int height, int *pVal) { return E_NOTIMPL; }
STDMETHODIMP VPinMAMEController::get_ChangedLampsState(int **buf, int *pVal) { return E_NOTIMPL; }
STDMETHODIMP VPinMAMEController::get_LampsState(int **buf, int *pVal) { return E_NOTIMPL; }
STDMETHODIMP VPinMAMEController::get_ChangedSolenoidsState(int **buf, int *pVal) { return E_NOTIMPL; }
STDMETHODIMP VPinMAMEController::get_SolenoidsState(int **buf, int *pVal) { return E_NOTIMPL; }
STDMETHODIMP VPinMAMEController::get_ChangedGIsState(int **buf, int *pVal) { return E_NOTIMPL; }
STDMETHODIMP VPinMAMEController::get_Switches(VARIANT *pVal) { return E_NOTIMPL; }
STDMETHODIMP VPinMAMEController::put_Switches(VARIANT newVal) { return E_NOTIMPL; }

STDMETHODIMP VPinMAMEController::get_GameName(BSTR *pVal)
{
   CComBSTR Val(m_pPinmameGame->name);
   *pVal = Val.Detach();

   return S_OK;
}

STDMETHODIMP VPinMAMEController::put_GameName(BSTR newVal)
{
   char szGameName[MAXSTRING];
   WideCharToMultiByte(CP_ACP, 0, newVal, -1, szGameName, sizeof(szGameName), NULL, NULL);
   PLOGI.printf("newVal=%s", szGameName);

   PINMAME_STATUS status = PinmameGetGame(szGameName, &VPinMAMEController::GetGameCallback, this);

   if (status == PINMAME_STATUS::OK) {
      PLOGI.printf("Game found: name=%s, description=%s, manufacturer=%s, year=%s",
         m_pPinmameGame->name, m_pPinmameGame->description, m_pPinmameGame->manufacturer, m_pPinmameGame->year);

      return S_OK;
   }

   if (status == PINMAME_STATUS::GAME_ALREADY_RUNNING) {
      PLOGI.printf("Game already running.");
   }
   else if (status == PINMAME_STATUS::GAME_NOT_FOUND) {
      PLOGI.printf("Game name not found.");
   }

   return S_OK;
}

STDMETHODIMP VPinMAMEController::get_ROMName(BSTR *pVal) { return E_NOTIMPL; }

STDMETHODIMP VPinMAMEController::get_HandleKeyboard(VARIANT_BOOL *pVal)
{
   *pVal = PinmameGetHandleKeyboard() ? VARIANT_TRUE : VARIANT_FALSE;

   return S_OK;
}

STDMETHODIMP VPinMAMEController::put_HandleKeyboard(VARIANT_BOOL newVal)
{
   PinmameSetHandleKeyboard((newVal == VARIANT_TRUE));

   return S_OK;
}

STDMETHODIMP VPinMAMEController::get_Machines(BSTR sMachine, VARIANT *pVal) { return E_NOTIMPL; }

STDMETHODIMP VPinMAMEController::get_Running(VARIANT_BOOL *pVal) { 
   return PinmameIsRunning() ? VARIANT_TRUE : VARIANT_FALSE; 
}

STDMETHODIMP VPinMAMEController::get_ChangedLamps(VARIANT *pVal)
{
   int uCount = PinmameGetChangedLamps(m_pLampBuffer);
   if (uCount <= 0) {
      V_VT(pVal) = VT_EMPTY;
      return S_OK;
   }

   SAFEARRAYBOUND Bounds[] = { { (ULONG)uCount, 0 }, { 2, 0 } };
   SAFEARRAY *psa = SafeArrayCreate(VT_VARIANT, 2, Bounds);
   LONG ix[2];
   VARIANT varValue;
   V_VT(&varValue) = VT_I4;

   for (ix[0] = 0; ix[0] < uCount; ix[0]++) {
      ix[1] = 0;
      V_I4(&varValue) = m_pLampBuffer[ix[0]].lampNo;
      SafeArrayPutElement(psa, ix, &varValue);
      ix[1] = 1;

      if (m_rgb && m_pLampBuffer[ix[0]].lampNo >= 81) {
         V_I4(&varValue) = m_pLampBuffer[ix[0]].state;
      }
      else {
         V_I4(&varValue) = (m_pLampBuffer[ix[0]].state) ? 1 : 0;
      }

      SafeArrayPutElement(psa, ix, &varValue);
   }

   V_VT(pVal) = VT_ARRAY | VT_VARIANT;
   V_ARRAY(pVal) = psa;

   return S_OK;
}

STDMETHODIMP VPinMAMEController::get_ChangedLEDs(int nHigh, int nLow, int nnHigh, int nnLow, VARIANT *pVal)
{
   uint64_t mask = ((uint64_t)nHigh << 32) | (uint64_t)nLow;
   uint64_t mask2 = ((uint64_t)nnHigh << 32) | (uint64_t)nnLow;
    
   int uCount = PinmameGetChangedLEDs(mask, mask2, m_pLEDBuffer);
   if (uCount <= 0) {
      V_VT(pVal) = VT_EMPTY;
      return S_OK;
   }

   SAFEARRAYBOUND Bounds[] = { { (ULONG)uCount, 0 }, { 3, 0 } };
   SAFEARRAY *psa = SafeArrayCreate(VT_VARIANT, 2, Bounds);
   LONG ix[3];
   VARIANT varValue;
   V_VT(&varValue) = VT_I4;

   for (ix[0] = 0; ix[0] < uCount; ix[0]++) {
      ix[1] = 0;
      V_I4(&varValue) = m_pLEDBuffer[ix[0]].ledNo;
      SafeArrayPutElement(psa, ix, &varValue);
      ix[1] = 1;
      V_I4(&varValue) = m_pLEDBuffer[ix[0]].chgSeg;
      SafeArrayPutElement(psa, ix, &varValue);
      ix[1] = 2;
      V_I4(&varValue) = m_pLEDBuffer[ix[0]].state;
      SafeArrayPutElement(psa, ix, &varValue);
   }

   V_VT(pVal) = VT_ARRAY | VT_VARIANT;
   V_ARRAY(pVal) = psa;

   return S_OK;
}

STDMETHODIMP VPinMAMEController::get_ChangedLEDsState(int nHigh, int nLow, int nnHigh, int nnLow, int **buf, int *pVal) { return E_NOTIMPL; }
STDMETHODIMP VPinMAMEController::ShowAboutDialog(LONG_PTR hParentWnd) { return E_NOTIMPL; }

STDMETHODIMP VPinMAMEController::get_HandleMechanics(int *pVal)
{
   *pVal = PinmameGetHandleMechanics();

   return S_OK;
}

STDMETHODIMP VPinMAMEController::put_HandleMechanics(int newVal)
{
   PinmameSetHandleMechanics(newVal);

   return S_OK;
}

STDMETHODIMP VPinMAMEController::get_GetMech(int mechNo, int *pVal)
{
   *pVal = (m_mechData.count(mechNo) > 0) ? m_mechData[mechNo] : 0;

   return S_OK;
}

STDMETHODIMP VPinMAMEController::put_Mech(int mechNo, int newVal) {
   if (!m_pPinmameMechConfig)
      m_pPinmameMechConfig = new PinmameMechConfig();

   switch(mechNo) {
      case 0:
         PinmameSetMech(newVal, m_pPinmameMechConfig);

         delete m_pPinmameMechConfig;
         m_pPinmameMechConfig = NULL;

         break;
      case 1:
         m_pPinmameMechConfig->sol1 = newVal;
         break;
      case 2:
         m_pPinmameMechConfig->sol2 = newVal;
         break;
      case 3:
         m_pPinmameMechConfig->length = newVal;
         break;
      case 4:
         m_pPinmameMechConfig->steps = newVal;
         break;
      case 5:
         m_pPinmameMechConfig->type = newVal;
         break;
      case 6:
         m_pPinmameMechConfig->acc = newVal;
         break;
      case 7:
         m_pPinmameMechConfig->ret = newVal;
         break;
      case 8:
         m_pPinmameMechConfig->initialPos = newVal + 1;
         break;
      default:
         if (mechNo % 10 == 0)
            m_pPinmameMechConfig->sw[(mechNo/10)-1].swNo = newVal;
         else if (mechNo % 10 == 1)
            m_pPinmameMechConfig->sw[(mechNo/10)-1].startPos = newVal;
         else if (mechNo % 10 == 2)
            m_pPinmameMechConfig->sw[(mechNo/10)-1].endPos = newVal;
         else if (mechNo % 10 == 3)
            m_pPinmameMechConfig->sw[(mechNo/10)-1].pulse = newVal;
         break;
   }

   return S_OK;
}

STDMETHODIMP VPinMAMEController::get_GIString(int nString, int *pVal) { return E_NOTIMPL; }

STDMETHODIMP VPinMAMEController::get_ChangedGIStrings(VARIANT *pVal)
{
   int uCount = PinmameGetChangedGIs(m_pGIBuffer);
   if (uCount <= 0) {
      V_VT(pVal) = VT_EMPTY;
      return S_OK;
   }

   SAFEARRAYBOUND Bounds[] = { { (ULONG)uCount, 0 }, { 2, 0 } };
   SAFEARRAY *psa = SafeArrayCreate(VT_VARIANT, 2, Bounds);
   LONG ix[2];
   VARIANT varValue;
   V_VT(&varValue) = VT_I4;

   for (ix[0] = 0; ix[0] < uCount; ix[0]++) {
      ix[1] = 0;
      V_I4(&varValue) = m_pGIBuffer[ix[0]].giNo;
      SafeArrayPutElement(psa, ix, &varValue);
      ix[1] = 1;
      V_I4(&varValue) = m_pGIBuffer[ix[0]].state;
      SafeArrayPutElement(psa, ix, &varValue);
   }

   V_VT(pVal) = VT_ARRAY | VT_VARIANT;
   V_ARRAY(pVal) = psa;

   return S_OK;
}

STDMETHODIMP VPinMAMEController::get_ChangedSolenoids(VARIANT *pVal)
{
   int uCount = PinmameGetChangedSolenoids(m_pSolenoidBuffer);
   if (uCount <= 0)
   {
      V_VT(pVal) = VT_EMPTY;
      return S_OK;
   }

   SAFEARRAYBOUND Bounds[] = { { (ULONG)uCount, 0 }, { 2, 0 } };
   SAFEARRAY *psa = SafeArrayCreate(VT_VARIANT, 2, Bounds);
   LONG ix[2];
   VARIANT varValue;
   V_VT(&varValue) = VT_I4;

   for (ix[0] = 0; ix[0] < uCount; ix[0]++) {
      ix[1] = 0;
      V_I4(&varValue) = m_pSolenoidBuffer[ix[0]].solNo;
      SafeArrayPutElement(psa, ix, &varValue);
      ix[1] = 1;
      V_I4(&varValue) = m_pSolenoidBuffer[ix[0]].state;
      SafeArrayPutElement(psa, ix, &varValue);
   }

   V_VT(pVal) = VT_ARRAY | VT_VARIANT;
   V_ARRAY(pVal) = psa;

   return S_OK;
}

STDMETHODIMP VPinMAMEController::get_SplashInfoLine(BSTR *pVal)
{
   CComBSTR strValue(m_szSplashInfoLine);
   *pVal = strValue.Detach();

   return S_OK;
}

STDMETHODIMP VPinMAMEController::put_SplashInfoLine(BSTR newVal)
{
   WideCharToMultiByte(CP_ACP, 0, newVal, -1, m_szSplashInfoLine, sizeof m_szSplashInfoLine, NULL, NULL);
   PLOGI.printf("newVal=%s", m_szSplashInfoLine);

   return S_OK;
}

STDMETHODIMP VPinMAMEController::get_Solenoids(VARIANT *pVal) { return E_NOTIMPL; }

STDMETHODIMP VPinMAMEController::get_Dip(int nNo, int *pVal)
{
   *pVal = PinmameGetDIP(nNo);

   return S_OK;
}

STDMETHODIMP VPinMAMEController::put_Dip(int nNo, int newVal)
{
    PinmameSetDIP(nNo, newVal);

    return S_OK;
}

STDMETHODIMP VPinMAMEController::get_GIStrings(VARIANT *pVal) { return E_NOTIMPL; }
STDMETHODIMP VPinMAMEController::get_LockDisplay(VARIANT_BOOL *pVal) { return E_NOTIMPL; }
STDMETHODIMP VPinMAMEController::put_LockDisplay(VARIANT_BOOL newVal) { return E_NOTIMPL; }

STDMETHODIMP VPinMAMEController::get_SolMask(int nLow, LONG *pVal)
{
   if (!pVal || (nLow < 0) || (nLow > 1))
      return S_FALSE;

   *pVal = PinmameGetSolenoidMask(nLow);

   return S_OK;
}

STDMETHODIMP VPinMAMEController::put_SolMask(int nLow, LONG newVal)
{
   if (!((0 <= nLow && nLow <= 2) || (1000 <= nLow && nLow < 1999)))
      return S_FALSE;

   PinmameSetSolenoidMask(nLow, newVal);

   return S_OK;
}

STDMETHODIMP VPinMAMEController::get_Version(BSTR *pVal)
{
   int nVersionNo0 = 99;
   int nVersionNo1 = 99;
   int nVersionNo2 = 99;
   int nVersionNo3 = 99;

   char szVersion[MAXTOKEN];
   sprintf_s(szVersion, sizeof(szVersion), "%02i%02i%02i%02i", nVersionNo0, nVersionNo1, nVersionNo2, nVersionNo3);

   WCHAR wzVersion[MAXTOKEN];
   MultiByteToWideCharNull(CP_ACP, 0, szVersion, -1, wzVersion, MAXTOKEN);

   *pVal = SysAllocString(wzVersion);

   return S_OK;
}

STDMETHODIMP VPinMAMEController::get_Games(IGames **pVal) 
{ 
   return m_pGames->QueryInterface(IID_IGames, (void**) pVal);
}

STDMETHODIMP VPinMAMEController::get_Settings(IControllerSettings **pVal) { return E_NOTIMPL; }

STDMETHODIMP VPinMAMEController::get_NewSoundCommands(VARIANT *pVal)
{
    return S_OK;
}

STDMETHODIMP VPinMAMEController::SetDisplayPosition(int x, int y, LONG_PTR hParentWindow) { return E_NOTIMPL; }
STDMETHODIMP VPinMAMEController::ShowOptsDialog(LONG_PTR hParentWnd) { return E_NOTIMPL; }

STDMETHODIMP VPinMAMEController::get_ShowDMDOnly(VARIANT_BOOL *pVal)
{
   *pVal = VARIANT_FALSE;

   return S_OK;
}

STDMETHODIMP VPinMAMEController::put_ShowDMDOnly(VARIANT_BOOL newVal)
{
   return S_OK;
}

STDMETHODIMP VPinMAMEController::get_ShowTitle(VARIANT_BOOL *pVal)
{
   *pVal = VARIANT_FALSE;

   return S_OK;
}

STDMETHODIMP VPinMAMEController::put_ShowTitle(VARIANT_BOOL newVal)
{
   return S_OK;
}

STDMETHODIMP VPinMAMEController::get_ShowFrame(VARIANT_BOOL *pVal)
{
   *pVal = VARIANT_FALSE;

   return S_OK;
}

STDMETHODIMP VPinMAMEController::put_ShowFrame(VARIANT_BOOL newVal)
{
   return S_OK;
}

STDMETHODIMP VPinMAMEController::get_DoubleSize(VARIANT_BOOL *pVal) { return E_NOTIMPL; }
STDMETHODIMP VPinMAMEController::put_DoubleSize(VARIANT_BOOL newVal) { return E_NOTIMPL; }
STDMETHODIMP VPinMAMEController::CheckROMS(/*[in,defaultvalue(0)]*/ int nShowOptions, /*[in,defaultvalue(0)]*/ LONG_PTR hParentWnd, /*[out, retval]*/ VARIANT_BOOL *pVal) { return E_NOTIMPL; }
STDMETHODIMP VPinMAMEController::ShowPathesDialog(LONG_PTR hParentWnd) { return E_NOTIMPL; }

STDMETHODIMP VPinMAMEController::get_Hidden(VARIANT_BOOL *pVal)
{
   *pVal = VARIANT_FALSE;

   return S_OK;
}

STDMETHODIMP VPinMAMEController::put_Hidden(VARIANT_BOOL newVal)
{
   return S_OK;
}

STDMETHODIMP VPinMAMEController::get_Game(IGame **pVal) { return E_NOTIMPL; }
STDMETHODIMP VPinMAMEController::get_MechSamples(VARIANT_BOOL *pVal) { return E_NOTIMPL; }
STDMETHODIMP VPinMAMEController::put_MechSamples(VARIANT_BOOL newVal) { return E_NOTIMPL; }
STDMETHODIMP VPinMAMEController::GetWindowRect(LONG_PTR hWnd, VARIANT *pVal) { return E_NOTIMPL; }
STDMETHODIMP VPinMAMEController::GetClientRect(LONG_PTR hWnd, VARIANT *pVal) { return E_NOTIMPL; }
STDMETHODIMP VPinMAMEController::get_MasterVolume(int *pVal) { return E_NOTIMPL; }
STDMETHODIMP VPinMAMEController::put_MasterVolume(int newVal) { return E_NOTIMPL; }
STDMETHODIMP VPinMAMEController::get_EnumAudioDevices(int *pVal) { return E_NOTIMPL; }
STDMETHODIMP VPinMAMEController::get_AudioDevicesCount(int *pVal) { return E_NOTIMPL; }
STDMETHODIMP VPinMAMEController::get_AudioDeviceDescription(int num, BSTR *pVal) { return E_NOTIMPL; }
STDMETHODIMP VPinMAMEController::get_AudioDeviceModule(int num, BSTR *pVal) { return E_NOTIMPL; }
STDMETHODIMP VPinMAMEController::get_CurrentAudioDevice(int *pVal) { return E_NOTIMPL; }
STDMETHODIMP VPinMAMEController::put_CurrentAudioDevice(int newVal) { return E_NOTIMPL; }
STDMETHODIMP VPinMAMEController::get_FastFrames(int *pVal) { return E_NOTIMPL; }
STDMETHODIMP VPinMAMEController::put_FastFrames(int newVal) { return E_NOTIMPL; }
STDMETHODIMP VPinMAMEController::get_IgnoreRomCrc(VARIANT_BOOL *pVal) { return E_NOTIMPL; }
STDMETHODIMP VPinMAMEController::put_IgnoreRomCrc(VARIANT_BOOL newVal) { return E_NOTIMPL; }
STDMETHODIMP VPinMAMEController::get_SoundMode(int *pVal) { return E_NOTIMPL; }
STDMETHODIMP VPinMAMEController::put_SoundMode(int newVal) { return E_NOTIMPL; }
STDMETHODIMP VPinMAMEController::get_CabinetMode(VARIANT_BOOL *pVal) { return E_NOTIMPL; }
STDMETHODIMP VPinMAMEController::put_CabinetMode(VARIANT_BOOL newVal) { return E_NOTIMPL; }
STDMETHODIMP VPinMAMEController::get_ShowPinDMD(VARIANT_BOOL *pVal) { return E_NOTIMPL; }
STDMETHODIMP VPinMAMEController::put_ShowPinDMD(VARIANT_BOOL newVal) { return E_NOTIMPL; }
STDMETHODIMP VPinMAMEController::get_ShowWinDMD(VARIANT_BOOL *pVal) { return E_NOTIMPL; }
STDMETHODIMP VPinMAMEController::put_ShowWinDMD(VARIANT_BOOL newVal) { return E_NOTIMPL; }