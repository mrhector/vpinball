#include "stdafx.h"

VPinMAMEController* g_pController;

void CALLBACK VPinMAMEController::GetGameCallback(PinmameGame* pPinmameGame)
{
   g_pController->m_pPinmameGame = new PinmameGame();
   memcpy(g_pController->m_pPinmameGame, pPinmameGame, sizeof(PinmameGame));
}

void CALLBACK VPinMAMEController::OnStateUpdated(int state) {
   PLOGI.printf("state=%d", state);
}

void CALLBACK VPinMAMEController::OnDisplayAvailable(int index, int displayCount, PinmameDisplayLayout *p_displayLayout)
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
      g_pController->m_dmdHeight = p_displayLayout->height;
      g_pController->m_dmdWidth = p_displayLayout->width;
      g_pController->m_dmdData = (char*)malloc(g_pController->m_dmdHeight * g_pController->m_dmdWidth);
   }
}

void CALLBACK VPinMAMEController::OnDisplayUpdated(int index, void* p_displayData, PinmameDisplayLayout* p_displayLayout)
{
   if ((p_displayLayout->type & DMD) == DMD && index == 0) {
      memcpy(g_pController->m_dmdData, p_displayData, p_displayLayout->width * p_displayLayout->height);
      g_pController->m_hasDMDUpdates = true;
   }
}

int CALLBACK VPinMAMEController::OnAudioAvailable(PinmameAudioInfo* p_audioInfo)
{
   PLOGI.printf("format=%d, channels=%d, sampleRate=%.2f, framesPerSecond=%.2f, samplesPerFrame=%d, bufferSize=%d", 
      p_audioInfo->format,
      p_audioInfo->channels,
      p_audioInfo->sampleRate,
      p_audioInfo->framesPerSecond,
      p_audioInfo->samplesPerFrame,
      p_audioInfo->bufferSize);

   g_pController->m_audio = new AudioPlayer();
   g_pController->m_audio->StreamInit(p_audioInfo->sampleRate, p_audioInfo->channels, 1.);

   g_pController->m_audioChannels = p_audioInfo->channels;

   return p_audioInfo->samplesPerFrame;
}

int CALLBACK VPinMAMEController::OnAudioUpdated(void* p_buffer, int samples)
{
   g_pController->m_audio->StreamUpdate(p_buffer, samples * g_pController->m_audioChannels * sizeof(int16_t));

   return samples;
}

void CALLBACK VPinMAMEController::OnMechAvailable(int mechNo, PinmameMechInfo* p_mechInfo)
{
   PLOGI.printf("mechNo=%d, type=%d, length=%d, steps=%d, pos=%d, speed=%d", 
      mechNo, 
      p_mechInfo->type, 
      p_mechInfo->length, 
      p_mechInfo->steps,
      p_mechInfo->pos, 
      p_mechInfo->speed);
}

void CALLBACK VPinMAMEController::OnMechUpdated(int mechNo, PinmameMechInfo* p_mechInfo)
{
   PLOGD.printf("mechNo=%d, type=%d, length=%d, steps=%d, pos=%d, speed=%d",
      mechNo, 
      p_mechInfo->type, 
      p_mechInfo->length, 
      p_mechInfo->steps, 
      p_mechInfo->pos,
      p_mechInfo->speed);

   g_pController->m_mechData[mechNo] = p_mechInfo->pos;
}

void CALLBACK VPinMAMEController::OnConsoleDataUpdated(void* p_data, int size)
{
   PLOGI.printf("size=%d", size);
}

VPinMAMEController::VPinMAMEController()
{
   PinmameConfig config = { 
      AUDIO_FORMAT_INT16, 
      44100, 
      "", 
      &VPinMAMEController::OnStateUpdated,
      &VPinMAMEController::OnDisplayAvailable,
      &VPinMAMEController::OnDisplayUpdated,
      &VPinMAMEController::OnAudioAvailable,
      &VPinMAMEController::OnAudioUpdated,
      &VPinMAMEController::OnMechAvailable,
      &VPinMAMEController::OnMechUpdated,
      nullptr,
      &VPinMAMEController::OnConsoleDataUpdated,
      nullptr 
   };

#if (defined(__APPLE__) && (TARGET_OS_IOS || TARGET_OS_TV)) || defined(__ANDROID__)
   strncpy((char*)config.vpmPath, g_pvp->m_szMyPath.c_str(), PINMAME_MAX_VPM_PATH);
   strncat((char*)config.vpmPath, "pinmame/", PINMAME_MAX_VPM_PATH);
#else
   strncpy((char*)config.vpmPath, getenv("HOME"), PINMAME_MAX_VPM_PATH);
   strncat((char*)config.vpmPath, "/.pinmame/", PINMAME_MAX_VPM_PATH);
#endif

   PinmameSetConfig(&config);

   PinmameSetHandleKeyboard(0);
   PinmameSetHandleMechanics(0xFF);

   m_pSolenoidBuffer = new PinmameSolenoidState[PinmameGetMaxSolenoids()];
   m_pLampBuffer = new PinmameLampState[PinmameGetMaxLamps()];
   m_pGIBuffer = new PinmameGIState[PinmameGetMaxGIs()];
   m_pLEDBuffer = new PinmameLEDState[PinmameGetMaxLEDs()];

   m_pPinmameGame = nullptr;
   m_pPinmameMechConfig = nullptr;

   CComObject<VPinMAMEGames>::CreateInstance(&m_pGames);
   m_pGames->AddRef();

   g_pController = this;
}

VPinMAMEController::~VPinMAMEController()
{
   if (m_pPinmameGame)
      delete m_pPinmameGame;

   if (m_pPinmameMechConfig)
      delete m_pPinmameMechConfig;

	m_pGames->Release();
}

STDMETHODIMP VPinMAMEController::Run(/*[in]*/ LONG_PTR hParentWnd, /*[in,defaultvalue(100)]*/ int nMinVersion)
{
   PINMAME_STATUS status = PinmameRun(m_szGameName);

   if (status == PINMAME_STATUS::OK) {
        int timeout = 0;

        while (!PinmameIsRunning() && timeout < 20) {
           SDL_Delay(75);
           timeout++;
        }

        return S_OK;
   }

   if (status == PINMAME_STATUS::GAME_ALREADY_RUNNING) {
      PLOGI.printf("Game already running.");
   }

   return S_FALSE;
}

STDMETHODIMP VPinMAMEController::Stop() { return E_NOTIMPL; }

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
   PinmameSetSwitch(nSwitchNo, newVal == VARIANT_TRUE);

   PLOGD.printf("switchNo=%d, newVal=%d", nSwitchNo, newVal == VARIANT_TRUE);

   return S_OK;
}

STDMETHODIMP VPinMAMEController::get_Pause(VARIANT_BOOL *pVal) { return E_NOTIMPL; }
STDMETHODIMP VPinMAMEController::put_Pause(VARIANT_BOOL newVal) { return E_NOTIMPL; }
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
   if (m_hasDMDUpdates) {
      SAFEARRAY *psa = SafeArrayCreateVector(VT_VARIANT, 0, m_dmdWidth * m_dmdHeight);

      VARIANT *pData;
      SafeArrayAccessData(psa, (void **)&pData);
      const UINT32 end = m_dmdWidth * m_dmdHeight;
      for (UINT32 i = 0; i < end; ++i)
      {
         V_VT(&pData[i]) = VT_UI1;
         V_UI1(&pData[i]) = ((char *)m_dmdData)[i];
      }
      SafeArrayUnaccessData(psa);

      V_VT(pVal) = VT_ARRAY | VT_VARIANT;
      V_ARRAY(pVal) = psa;

      m_hasDMDUpdates = false;

      return S_OK;
   }

   return S_FALSE;
}

STDMETHODIMP VPinMAMEController::get_RawDmdColoredPixels(VARIANT *pVal) { return E_NOTIMPL; }
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
   CComBSTR Val(m_szGameName);
   *pVal = Val.Detach();

   return S_OK;
}

STDMETHODIMP VPinMAMEController::put_GameName(BSTR newVal)
{
   WideCharToMultiByte(CP_ACP, 0, newVal, -1, m_szGameName, sizeof(m_szGameName), NULL, NULL);
   PLOGI.printf("newVal=%s", m_szGameName);

   PINMAME_STATUS status = PinmameGetGame(m_szGameName, &VPinMAMEController::GetGameCallback);

   if (status == PINMAME_STATUS::OK)
      return S_OK;

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
      if (m_pLampBuffer[ix[0]].lampNo < 81)
         V_I4(&varValue) = (m_pLampBuffer[ix[0]].state) ? 1 : 0;
      else
         V_I4(&varValue) = m_pLampBuffer[ix[0]].state;
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
   if (uCount <= 0)
   {
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
         m_pPinmameMechConfig = nullptr;

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

   uint64_t mask = PinmameGetSolenoidMask();

   *pVal = (nLow == 0) ? (mask & 0xFFFFFFFF) : ((mask >> 32) & 0xFFFFFFFF);

   return S_OK;
}

STDMETHODIMP VPinMAMEController::put_SolMask(int nLow, LONG newVal)
{
   if ((nLow < 0) || (nLow > 2))
      return S_FALSE;

   if (nLow == 2)
      PinmameSetUseModulatedSolenoids(newVal);
   else {
      uint64_t mask = PinmameGetSolenoidMask();

      if (nLow == 0)
         mask = (mask & 0xFFFFFFFF00000000) | newVal; 
      else
         mask = (mask & 0xFFFFFFFF) | ((uint64_t)newVal << 32);

      PinmameSetSolenoidMask(mask);

      PLOGI.printf("newVal=0x%016llx", mask);
   }

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
STDMETHODIMP VPinMAMEController::get_NewSoundCommands(VARIANT *pVal) { return E_NOTIMPL; }
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
