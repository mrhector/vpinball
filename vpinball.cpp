// VPinball.cpp: implementation of the VPinball class.
//
////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "resource.h"
#include "KeysConfigDialog.h"

#if defined(IMSPANISH)
#define TOOLBAR_WIDTH 152
#elif defined(IMGERMAN)
#define TOOLBAR_WIDTH 152
#else
#define TOOLBAR_WIDTH 102 //98 //102
#endif

#define SCROLL_WIDTH GetSystemMetrics(SM_CXVSCROLL)

#define MAIN_WINDOW_WIDTH		800
#define MAIN_WINDOW_HEIGHT		550

#define DOCKER_REGISTRY_KEY     "Visual Pinball\\VP10\\Editor"

#define	RECENT_FIRST_MENU_IDM	5000	// ID of the first recent file list filename
#define	RECENT_LAST_MENU_IDM	(RECENT_FIRST_MENU_IDM+LAST_OPENED_TABLE_COUNT)

#define AUTOSAVE_DEFAULT_TIME 10

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// menu locations
enum {
   FILEMENU = 0,
   EDITMENU,
   VIEWMENU,
   INSERTMENU,
   TABLEMENU,
   LAYERMENU,
   PREFMENU,
   WINDOWMENU,
   HELPMENU,
   NUM_MENUS
};

/*
TBButton:
typedef struct {
int       iBitmap;
int       idCommand;
BYTE      fsState;
BYTE      fsStyle;
DWORD_PTR dwData;
INT_PTR   iString;
} TBBUTTON, *PTBBUTTON, *LPTBBUTTON;
*/


static const int allLayers[MAX_LAYERS] =
{
   ID_LAYER_LAYER1,
   ID_LAYER_LAYER2,
   ID_LAYER_LAYER3,
   ID_LAYER_LAYER4,
   ID_LAYER_LAYER5,
   ID_LAYER_LAYER6,
   ID_LAYER_LAYER7,
   ID_LAYER_LAYER8,
   ID_LAYER_LAYER9,
   ID_LAYER_LAYER10,
   ID_LAYER_LAYER11
};

WCHAR *VPinball::m_customParameters[MAX_CUSTOM_PARAM_INDEX] = {};

INT_PTR CALLBACK FontManagerProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK SecurityOptionsProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

typedef struct _tagSORTDATA
{
    HWND hwndList;
    int subItemIndex;
    int sortUpDown;
}SORTDATA;

SORTDATA SortData;

static bool firstRun = true;

static void AddToolTip(char *text, HWND parentHwnd, HWND toolTipHwnd, HWND controlHwnd)
{
    TOOLINFO toolInfo = { 0 };
    toolInfo.cbSize = sizeof(toolInfo);
    toolInfo.hwnd = parentHwnd;
    toolInfo.uFlags = TTF_IDISHWND | TTF_SUBCLASS;
    toolInfo.uId = (UINT_PTR)controlHwnd;
    toolInfo.lpszText = text;
    SendMessage(toolTipHwnd, TTM_ADDTOOL, 0, (LPARAM)&toolInfo);
}

///<summary>
///VPinball Constructor
///<para>Init</para>
///</summary>
VPinball::VPinball()
{
   // DLL_API void DLL_CALLCONV FreeImage_Initialise(BOOL load_local_plugins_only FI_DEFAULT(FALSE)); //add FreeImage support BDS
   m_closing = false;
   m_unloadingTable = false;
   m_toolbarDialog = NULL;
   m_propertyDialog = NULL;
   m_dockToolbar = NULL;
   m_dockProperties = NULL;
   m_layersListDialog = NULL;
   m_dockLayers = NULL;
   m_cref = 0;				//inits Reference Count for IUnknown Interface. Every com Object must 
   //implement this and StdMethods QueryInterface, AddRef and Release
   m_open_minimized = 0;
   memset(m_currentTablePath, 0, MAX_PATH);

   m_mouseCursorPosition.x = 0.0f;
   m_mouseCursorPosition.y = 0.0f;
   m_pcv = NULL;			// no currently active code window

   m_NextTableID = 1;

   m_ptableActive = NULL;

   m_workerthread = NULL;										//Workerthread - only for hanging scripts and autosave - will be created later

   m_ToolCur = IDC_SELECT;

   GetMyPath();													//Store path of vpinball.exe in m_szMyPath and m_wzMyPath

#ifdef _WIN64
   m_scintillaDll = LoadLibrary("SciLexerVP64.DLL");
#else
   m_scintillaDll = LoadLibrary("SciLexerVP.DLL");
#endif
   if (m_scintillaDll == NULL)
   {
   #ifdef _WIN64
       m_scintillaDll = LoadLibrary("SciLexer64.DLL");
   #else
       m_scintillaDll = LoadLibrary("SciLexer.DLL");
   #endif
       if (m_scintillaDll == NULL)
       #ifdef _WIN64
           ShowError("Unable to load SciLexer64.DLL");
   #else
           ShowError("Unable to load SciLexer.DLL");
   #endif
   }
}

///<summary>
///VPinball Destructor
///<para>deletes clipboard</para>
///<para>Releases Resources for Script editor</para>
///</summary>
VPinball::~VPinball()
{
   // DLL_API void DLL_CALLCONV FreeImage_DeInitialise(); //remove FreeImage support BDS
   SetClipboard(NULL);
   FreeLibrary(m_scintillaDll);
}

///<summary>
///Store path of exe (without the exe's filename) in Class Variable
///<para>Stores path as char[MAX_PATH] in m_szMyPath (8 bit ansi)</para>
///<para>Stores path as WCHAR[MAX_PATH] in m_wzMyPath (16 bit Unicode)</para>
///</summary>
void VPinball::GetMyPath()
{
   char szPath[MAX_PATH];

   GetModuleFileName(NULL, szPath, MAX_PATH);

   char *szEnd = szPath + lstrlen(szPath);

   // search for first backslash
   while (szEnd > szPath)
   {
      if (*szEnd == '\\')
         break;
      szEnd--;
   }

   // truncate the filename
   *(szEnd + 1) = '\0'; // Get rid of exe name

   // store 2x
   lstrcpy(m_szMyPath, szPath);
   MultiByteToWideChar(CP_ACP, 0, szPath, -1, m_wzMyPath, MAX_PATH);
}

// Class Variables
bool VPinball::m_open_minimized;

///<summary>
///Sets m_open_minimized to 1
///Called by CLI Option minimized 
///</summary>
void VPinball::SetOpenMinimized()
{
   m_open_minimized = 1;
}


///<summary>
///Ensure that worker thread exists
///<para>Starts worker Thread otherwise</para>
///</summary>
void VPinball::EnsureWorkerThread()
{
   if (!m_workerthread)
   {
      g_hWorkerStarted = CreateEvent(NULL, TRUE, FALSE, NULL);
      m_workerthread = (HANDLE)_beginthreadex(NULL, 0, VPWorkerThreadStart, 0, 0, &m_workerthreadid); //!! _beginthreadex is safer
      if (WaitForSingleObject(g_hWorkerStarted, 5000) == WAIT_TIMEOUT)
      {
      }
      SetThreadPriority(m_workerthread, THREAD_PRIORITY_LOWEST);
   }
}

///<summary>
///Post Work to the worker Thread
///<para>Creates Worker-Thread if not present</para>
///<para>See Worker::VPWorkerThreadStart for infos</para>
///<param name="workid">int for the type of message (COMPLETE_AUTOSAVE | HANG_SNOOP_START | HANG_SNOOP_STOP)</param>
///<param name="lParam">Second Parameter for message (AutoSavePackage (see worker.h) if COMPLETE_AUTOSAVE, otherwise NULL)</param>
///<returns>Handle to Event that get ack. If event is finished (unsure)</returns>
///</summary>
HANDLE VPinball::PostWorkToWorkerThread(int workid, LPARAM lParam)
{
   EnsureWorkerThread();										// Check if Workerthread was created once, otherwise create

   HANDLE hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

   PostThreadMessage(m_workerthreadid, workid, (WPARAM)hEvent, lParam);

   return hEvent;
}

///<summary>
///Sets m_autosavetime
///<param name="minutes">int Minutes between autosave</param>
///</summary>
void VPinball::SetAutoSaveMinutes(const int minutes)
{
   m_autosaveTime = (minutes <= 0) ? -1 : minutes * (60 * 1000); // convert to milliseconds
}

///<summary>
///Post Work to the worker Thread
///<para>Creates Worker-Thread if not present</para>
///<para>See Worker::VPWorkerThreadStart for infos</para>
///<param name="workid">int for the type of message (COMPLETE_AUTOSAVE | HANG_SNOOP_START | HANG_SNOOP_STOP)</param>
///<param name="lparam">Second Parameter for message (AutoSavePackage (see worker.h) if COMPLETE_AUTOSAVE, otherwise NULL)</param>
///<returns>Handle to Event that get ack. If event is finished (unsure)</returns>
///</summary>
void VPinball::InitTools()
{
   m_ToolCur = IDC_SELECT;
}

///<summary>
///Initializes Default Values of many variables (from Registry if keys are present). 
///<para>Registry Values under HKEY-CURRENT-USER/Software/Visual Pinball</para>
///<para>Deadzone, ShowDragPoints, DrawLightCenters,</para>
///<para>AutoSaveOn, AutoSaveTime, SecurityLevel</para>
///<para>Gets the last loaded Tables (List under File-Menu)</para>
///</summary>
void VPinball::InitRegValues()
{
   const int deadz = LoadValueIntWithDefault("Player", "DeadZone", 0);
   SaveValueInt("Player", "DeadZone", deadz);

   m_alwaysDrawDragPoints = LoadValueBoolWithDefault("Editor", "ShowDragPoints", false);
   m_alwaysDrawLightCenters = LoadValueBoolWithDefault("Editor", "DrawLightCenters", false);
   m_gridSize = LoadValueIntWithDefault("Editor", "GridSize", 50);

   const bool autoSave = LoadValueBoolWithDefault("Editor", "AutoSaveOn", true);
   if (autoSave)
   {
      m_autosaveTime = LoadValueIntWithDefault("Editor", "AutoSaveTime", AUTOSAVE_DEFAULT_TIME);
      SetAutoSaveMinutes(m_autosaveTime);
   }
   else
      m_autosaveTime = -1;

   m_securitylevel = LoadValueIntWithDefault("Player", "SecurityLevel", DEFAULT_SECURITY_LEVEL);

   m_dummyMaterial.m_cBase = LoadValueIntWithDefault("Editor", "DefaultMaterialColor", 0xB469FF);
   m_elemSelectColor = LoadValueIntWithDefault("Editor", "ElementSelectColor", 0x00FF0000);
   m_elemSelectLockedColor = LoadValueIntWithDefault("Editor", "ElementSelectLockedColor", 0x00A7726D);
   m_backgroundColor = LoadValueIntWithDefault("Editor", "BackgroundColor", 0x008D8D8D);
   m_fillColor = LoadValueIntWithDefault("Editor", "FillColor", 0x00B1CFB3);

   if (m_securitylevel < eSecurityNone || m_securitylevel > eSecurityNoControls)
      m_securitylevel = eSecurityNoControls;

   // get the list of the last n loaded tables
   for (int i = 0; i < LAST_OPENED_TABLE_COUNT; i++)
   {
      char szRegName[MAX_PATH];
      char szTableName[MAX_PATH];
      sprintf_s(szRegName, "TableFileName%d", i);
      LoadValueString("RecentDir", szRegName, szTableName, MAX_PATH);
      m_recentTableList.push_back(std::string(szTableName));
   }

   m_convertToUnit = LoadValueIntWithDefault("Editor", "Units", 0);
}

void VPinball::AddMDITable(PinTableMDI* mdiTable) 
{ 
    AddMDIChild(mdiTable); 
}

void VPinball::SetClipboard(vector<IStream*> * const pvstm)
{
   for (size_t i = 0; i < m_vstmclipboard.size(); i++)
      m_vstmclipboard[i]->Release();
   m_vstmclipboard.clear();

   if (pvstm)
      for (size_t i = 0; i < pvstm->size(); i++)
         m_vstmclipboard.push_back((*pvstm)[i]);
}

void VPinball::SetCursorCur(HINSTANCE hInstance, LPCTSTR lpCursorName)
{
   HCURSOR hcursor = LoadCursor(hInstance, lpCursorName);
   SetCursor(hcursor);
}

void VPinball::SetActionCur(const char * const szaction)
{
   SendMessage(m_hwndStatusBar, SB_SETTEXT, 3 | 0, (size_t)szaction);
}

void VPinball::SetStatusBarElementInfo(const char * const info)
{
   SendMessage(m_hwndStatusBar, SB_SETTEXT, 4 | 0, (size_t)info);
}

void VPinball::SetStatusBarUnitInfo(const char * const info, const bool isUnit)
{
    if (g_pplayer)
        return;

    char textBuf[256];

    if (info[0] != '\0')
    {
       strcpy_s(textBuf, info);
       if(isUnit)
       {
           switch(m_convertToUnit)
           {
               case 0:
               {
                   strcat_s(textBuf, " (inch)");
                   break;
               }
               case 1:
               {
                   strcat_s(textBuf, " (mm)");
                   break;
               }
               case 2:
               {
                   strcat_s(textBuf, " (VPUnits)");
                   break;
               }
               default:
               break;
           }
       }
    }
    else
       textBuf[0] = '\0';

    SendMessage(m_hwndStatusBar, SB_SETTEXT, 5 | 0, (size_t)textBuf);
}

bool VPinball::OpenFileDialog(const char *initDir, char *filename, const char *fileFilter, const char *defaultExt, DWORD flags, int &fileOffset)
{
    OPENFILENAME ofn;
    ZeroMemory(&ofn, sizeof(OPENFILENAME));
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hInstance = g_hinst;
    ofn.hwndOwner = GetHwnd();
    // TEXT
    ofn.lpstrFilter = fileFilter;
    ofn.lpstrInitialDir = initDir;
    ofn.lpstrFile = filename;
    ofn.nMaxFile = MAXSTRING;
    ofn.lpstrDefExt = defaultExt;
    ofn.Flags = OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY | flags;
    const int ret = GetOpenFileName(&ofn);
    fileOffset = ofn.nFileOffset;
    return ret != 0;
}

CDockProperty *VPinball::GetDefaultPropertiesDocker()
{
    const int dockStyle = DS_DOCKED_RIGHT | DS_CLIENTEDGE | DS_NO_CLOSE;
    m_dockProperties = (CDockProperty *)AddDockedChild(new CDockProperty, dockStyle, 280, IDD_PROPERTY_DIALOG);

    assert(m_dockProperties->GetContainer());
    m_dockProperties->GetContainer()->SetHideSingleTab(TRUE);
    m_propertyDialog = m_dockProperties->GetContainProperties()->GetPropertyDialog();
    return m_dockProperties;
}

CDockProperty *VPinball::GetPropertiesDocker()
{
    if(m_propertyDialog==NULL || !m_dockProperties->IsWindow())
        return GetDefaultPropertiesDocker();

    return m_dockProperties;
}

CDockToolbar *VPinball::GetDefaultToolbarDocker()
{
    const int dockStyle = DS_DOCKED_LEFT | DS_CLIENTEDGE | DS_NO_CLOSE;
    m_dockToolbar = (CDockToolbar *)AddDockedChild(new CDockToolbar, dockStyle, 110, IDD_TOOLBAR);
    assert(m_dockToolbar->GetContainer());
    m_dockToolbar->GetContainer()->SetHideSingleTab(TRUE);
    m_toolbarDialog = m_dockToolbar->GetContainToolbar()->GetToolbarDialog();

    return m_dockToolbar;
}

CDockToolbar *VPinball::GetToolbarDocker()
{
    if(m_dockToolbar==NULL || !m_dockToolbar->IsWindow() )
        return GetDefaultToolbarDocker();
    return m_dockToolbar;
}

CDockLayers *VPinball::GetDefaultLayersDocker()
{
    const int dockStyle = DS_DOCKED_BOTTOM | DS_CLIENTEDGE | DS_NO_CLOSE;
    m_dockLayers = (CDockLayers *)AddDockedChild(new CDockLayers, dockStyle, 380, IDD_LAYERS);

    assert(m_dockLayers->GetContainer());
    m_dockLayers->GetContainer()->SetHideSingleTab(TRUE);
    m_layersListDialog = m_dockLayers->GetContainLayers()->GetLayersDialog();
    
    return m_dockLayers;
}

CDockLayers *VPinball::GetLayersDocker()
{
    if (m_dockLayers == NULL || !m_dockLayers->IsWindow())
        return GetDefaultLayersDocker();
    return m_dockLayers;
}

void VPinball::CreateDocker()
{
    if (!LoadDockRegistrySettings(DOCKER_REGISTRY_KEY))
    {
        GetPropertiesDocker();
        GetToolbarDocker();
        GetLayersDocker();
    }
    m_dockProperties->GetContainer()->SetHideSingleTab(TRUE);
    m_dockLayers->GetContainer()->SetHideSingleTab(TRUE);
    m_dockToolbar->GetContainer()->SetHideSingleTab(TRUE);
}

void VPinball::SetPosCur(float x, float y)
{
   char szT[256];
   sprintf_s(szT, "%.4f, %.4f", x, y);
   m_mouseCursorPosition.x = x;
   m_mouseCursorPosition.y = y;
   SendMessage(m_hwndStatusBar, SB_SETTEXT, 0 | 0, (size_t)szT);
}

void VPinball::SetObjectPosCur(float x, float y)
{
   char szT[256];
   sprintf_s(szT, "%.4f, %.4f", x, y);
   SendMessage(m_hwndStatusBar, SB_SETTEXT, 1 | 0, (size_t)szT);
}

void VPinball::ClearObjectPosCur()
{
   SendMessage(m_hwndStatusBar, SB_SETTEXT, 1 | 0, (size_t)"");
}

float VPinball::ConvertToUnit(const float value)
{
   switch (m_convertToUnit)
   {
      case 0:
        return vpUnitsToInches(value);

      case 1:
        return vpUnitsToMillimeters(value);

      case 2:
        return value;
   }
   return 0;
}

void VPinball::SetPropSel(VectorProtected<ISelect> *pvsel)
{
//   m_sb.CreateFromDispatch(GetHwnd(), pvsel);
    if (m_propertyDialog && m_propertyDialog->IsWindow())
        m_propertyDialog->UpdateTabs(pvsel);
}

void VPinball::DeletePropSel()
{
//   m_sb.RemoveSelection();
}

CMenu VPinball::GetMainMenu(int id)
{
   const int count = m_mainMenu.GetMenuItemCount();
   return m_mainMenu.GetSubMenu(id + ((count > NUM_MENUS) ? 1 : 0)); // MDI has added its stuff (table icon for first menu item)
}


BOOL VPinball::ParseCommand(size_t code, size_t notify)
{
   // check if it's an Editable tool
   ItemTypeEnum type = EditableRegistry::TypeFromToolID((int)code);
   if (type != eItemInvalid)
   {
      m_ToolCur = (int)code;

      if (notify == 1) // accelerator - mouse can be over table already
      {
         POINT pt;
         GetCursorPos(&pt);
         SetCursorPos(pt.x, pt.y);
      }
      return TRUE;
   }

   /* a MDI client window starts with ID 4000 and is incremented by Windows if a new window(table) is loaded 
      if the user switches a table (multiple tables are loaded) the code is 4000+ support up to 30 loaded tables here */
   if (code >= 4000 && code < 4030)
   {
       /* close all dialogs if the table is changed to prevent further issues */
       CloseAllDialogs();
   }

   switch (code)
   {
       case IDM_NEW:
       case ID_NEW_BLANKTABLE:
       case ID_NEW_EXAMPLETABLE:
       {
          OpenNewTable(code);
          return TRUE;
       }
       case ID_DELETE:
       {
          ProcessDeleteElement();
          return TRUE;
       }
       case ID_TABLE_CAMERAMODE:
       case ID_TABLE_PLAY:
       {
           DoPlay(code == ID_TABLE_CAMERAMODE);
           return TRUE;
       }
       case ID_SCRIPT_SHOWIDE:
       case ID_EDIT_SCRIPT:
       {
          ToggleScriptEditor();
          return TRUE;
       }
       case ID_EDIT_BACKGLASSVIEW:
       {
          ToggleBackglassView();
          return TRUE;
       }
       case ID_EDIT_SEARCH:
       {
          ShowSearchSelect();
          return TRUE;
       }
       case ID_EDIT_SETDEFAULTPHYSICS:
       {
          SetDefaultPhysics();
          break;
       }
       case ID_LOCK:
       {
          CComObject<PinTable> * const ptCur = GetActiveTable();
          if (ptCur)
             ptCur->LockElements();
          return TRUE;
       }
       case ID_EDIT_DRAWINGORDER_HIT:
       {
          //DialogBoxParam(g_hinst, MAKEINTRESOURCE(IDD_DRAWING_ORDER), m_hwnd, DrawingOrderProc, 0);
          ShowDrawingOrderDialog(false);
          return TRUE;
       }
       case ID_EDIT_DRAWINGORDER_SELECT:
       {
          //DialogBoxParam(g_hinst, MAKEINTRESOURCE(IDD_DRAWING_ORDER), m_hwnd, DrawingOrderProc, 0);
          ShowDrawingOrderDialog(true);
          return TRUE;
       }
       case ID_VIEW_SOLID:
       case ID_VIEW_OUTLINE:
       {
           SetViewSolidOutline(code);
           return TRUE;
       }
       case ID_VIEW_GRID:
       {
           ShowGridView();
           return TRUE;
       }
       case ID_VIEW_BACKDROP:
       {
           ShowBackdropView();
           return TRUE;
       }
       case IDC_SELECT:
       case ID_TABLE_MAGNIFY:
       {
          m_ToolCur = (int)code;
          if (notify == 1) // accelerator - mouse can be over table already
          {
             POINT pt;
             GetCursorPos(&pt);
             SetCursorPos(pt.x, pt.y);
          }
          return TRUE;
       }
       case ID_ADD_CTRL_POINT:
       {
          AddControlPoint();
          return TRUE;
       }
       case ID_ADD_SMOOTH_CTRL_POINT:
       {
          AddSmoothControlPoint();
          return TRUE;
       }
       case IDM_SAVE:
       {
          SaveTable(false);
          return TRUE;
       }
       case IDM_SAVEAS:
       {
          SaveTable(true);
          return TRUE;
       }

       case RECENT_FIRST_MENU_IDM:
       case RECENT_FIRST_MENU_IDM + 1:
       case RECENT_FIRST_MENU_IDM + 2:
       case RECENT_FIRST_MENU_IDM + 3:
       case RECENT_FIRST_MENU_IDM + 4:
       case RECENT_FIRST_MENU_IDM + 5:
       case RECENT_FIRST_MENU_IDM + 6:
       case RECENT_FIRST_MENU_IDM + 7:
       {
          OpenRecentFile(code);
          return TRUE;
       }

       case IDM_OPEN:
       {
           LoadFile();
           return TRUE;
       }
       case IDM_CLOSE:
       {
          CComObject<PinTable> * const ptCur = GetActiveTable();
          if (ptCur)
             CloseTable(ptCur);
          return TRUE;
       }
       case IDC_COPY:
       {
          CopyPasteElement(COPY);
          return TRUE;
       }
       case IDC_PASTE:
       {
          CopyPasteElement(PASTE);
          return TRUE;
       }
       case IDC_PASTEAT:
       {
          CopyPasteElement(PASTE_AT);
          return TRUE;
       }
       case ID_EDIT_UNDO:
       {
          CComObject<PinTable> * const ptCur = GetActiveTable();
          if (ptCur)
             ptCur->Undo();
          return TRUE;
       }
       case ID_FILE_EXPORT_BLUEPRINT:
       {
          CComObject<PinTable> * const ptCur = GetActiveTable();
          if (ptCur)
             ptCur->ExportBlueprint();
          return TRUE;
       }
       case ID_EXPORT_TABLEMESH:
       {
          CComObject<PinTable> * const ptCur = GetActiveTable();
          if (ptCur)
             ptCur->ExportTableMesh();
          return TRUE;
       }
       case ID_IMPORT_BACKDROPPOV:
       {
          CComObject<PinTable> * const ptCur = GetActiveTable();
          if (ptCur)
             ptCur->ImportBackdropPOV(NULL);
          return TRUE;
       }
       case ID_EXPORT_BACKDROPPOV:
       {
          CComObject<PinTable> * const ptCur = GetActiveTable();
          if (ptCur)
             ptCur->ExportBackdropPOV(NULL);
          return TRUE;
       }
       case ID_FILE_EXIT:
       {
           PostMessage(WM_CLOSE, 0, 0);
           return TRUE;
       }
       case ID_EDIT_AUDIOOPTIONS:
       {
           m_audioOptDialog.DoModal(GetHwnd());
           return TRUE;
       }
       case ID_EDIT_PHYSICSOPTIONS:
       {
           m_physicsOptDialog.DoModal(GetHwnd());
           return TRUE;
       }
       case ID_EDIT_EDITOROPTIONS:
       {
          m_editorOptDialog.DoModal(GetHwnd());
          // refresh editor options from the registry
          InitRegValues();
          // force a screen refresh (it an active table is loaded)
          CComObject<PinTable> * const ptCur = GetActiveTable();
          if (ptCur)
             ptCur->SetDirtyDraw();
          return TRUE;
       }
       case ID_EDIT_VIDEOOPTIONS:
       {
           m_videoOptDialog.DoModal(GetHwnd());
           return TRUE;
       }
       case ID_TABLE_TABLEINFO:
       {
           CComObject<PinTable> * const ptCur = GetActiveTable();
           if (ptCur)
               m_tableInfoDialog.DoModal(GetHwnd());
           return TRUE;
       }
       case IDM_IMAGE_EDITOR:
       case ID_TABLE_IMAGEMANAGER:
       {
           CComObject<PinTable> * const ptCur = GetActiveTable();
           if (ptCur)
           {
               ShowSubDialog(m_imageMngDlg);
           }
           return TRUE;
       }
       case IDM_SOUND_EDITOR:
       case ID_TABLE_SOUNDMANAGER:
       {
           CComObject<PinTable> * const ptCur = GetActiveTable();
           if (ptCur)
           {
               if (!m_soundMngDlg.IsWindow())
               {
                   m_soundMngDlg.Create(GetHwnd());
                   m_soundMngDlg.ShowWindow();
               }
               else
                   m_soundMngDlg.SetForegroundWindow();
           }
           return TRUE;
       }
       case IDM_MATERIAL_EDITOR:
       case ID_TABLE_MATERIALMANAGER:
       {
           CComObject<PinTable> * const ptCur = GetActiveTable();
           if (ptCur)
           {
               ShowSubDialog(m_materialDialog);
           }
           return TRUE;
       }
       case ID_TABLE_FONTMANAGER:
       {
           CComObject<PinTable> * const ptCur = GetActiveTable();
           if (ptCur)
              /*const DWORD foo =*/ DialogBoxParam(g_hinst, MAKEINTRESOURCE(IDD_FONTDIALOG), GetHwnd(), FontManagerProc, (size_t)ptCur);
           return TRUE;
       }
       case ID_TABLE_DIMENSIONMANAGER:
       {
           ShowSubDialog(m_dimensionDialog);
           return TRUE;
       }
       case IDM_COLLECTION_EDITOR:
       case ID_TABLE_COLLECTIONMANAGER:
       {
           CComObject<PinTable> * const ptCur = GetActiveTable();
           if (ptCur)
           {
               ShowSubDialog(m_collectionMngDlg);
           }
           return TRUE;
       }
       case ID_PREFERENCES_SECURITYOPTIONS:
       {
          DialogBoxParam(g_hinst, MAKEINTRESOURCE(IDD_SECURITY_OPTIONS), GetHwnd(), SecurityOptionsProc, 0);

          // refresh editor options from the registry
          InitRegValues();
          return TRUE;
       }
       case ID_EDIT_KEYS:
       {
          KeysConfigDialog * const keysConfigDlg = new KeysConfigDialog();
          keysConfigDlg->DoModal();
          delete keysConfigDlg;
          return TRUE;
       }
       case ID_HELP_ABOUT:
       {
          ShowSubDialog(m_aboutDialog);
          return TRUE;
       }
       case ID_WINDOW_CASCADE:
       {
          MDICascade();
          return TRUE;
       }
       case ID_WINDOW_TILE:
       {
          MDITile();
          return TRUE;
       }
       case ID_WINDOW_ARRANGEICONS:
       {
          MDIIconArrange();
          return TRUE;
       }
   }
   return FALSE;
}

void VPinball::ReInitSound()
{
	for (size_t i = 0; i < m_vtable.size(); i++)
	{
		PinTable * const ptT = m_vtable[i];
		for (size_t j = 0; j < ptT->m_vsound.size(); j++)
			ptT->m_vsound[j]->UnInitialize();
	}
	m_ps.ReInitPinDirectSound(GetHwnd());
	for (size_t i = 0; i < m_vtable.size(); i++)
	{
		PinTable * const ptT = m_vtable[i];
		for (size_t j = 0; j < ptT->m_vsound.size(); j++)
			ptT->m_vsound[j]->ReInitialize();
	}
}


void VPinball::ToggleToolbar()
{
    if (m_toolbarDialog)
        m_toolbarDialog->EnableButtons();
}

void VPinball::DoPlay(const bool _cameraMode)
{
   NumVideoBytes = 0;
   CComObject<PinTable> * const ptCur = GetActiveTable();
   if (ptCur)
      ptCur->Play(_cameraMode);
}

bool VPinball::LoadFile()
{
   char szFileName[MAXSTRING];
   char szInitialDir[MAXSTRING];
   int fileOffset;
   szFileName[0] = '\0';

   /*const HRESULT hr =*/ LoadValueString("RecentDir", "LoadDir", szInitialDir, MAXSTRING);
   if (!OpenFileDialog(szInitialDir, szFileName, "Visual Pinball Tables (*.vpx)\0*.vpx\0Old Visual Pinball Tables(*.vpt)\0*.vpt\0", "vpx", 0, fileOffset))
      return false;

   LoadFileName(szFileName,true);

   return true;
}

void VPinball::LoadFileName(const char *szFileName, const bool updateEditor)
{
   if (firstRun)
      OnInitialUpdate();
   PathFromFilename(szFileName, m_currentTablePath);
   CloseAllDialogs();

   PinTableMDI *mdiTable = new PinTableMDI(this);
   CComObject<PinTable> *ppt = mdiTable->GetTable();
   m_vtable.push_back(ppt);
   const HRESULT hr = ppt->LoadGameFromFilename(szFileName);

   if (!SUCCEEDED(hr))
   {
      if (hr == E_ACCESSDENIED)
      {
         LocalString ls(IDS_CORRUPTFILE);
         ShowError(ls.m_szbuffer);
      }

      RemoveFromVectorSingle(m_vtable, ppt);
      ppt->Release();
   }
   else
   {
      TitleFromFilename(szFileName, ppt->m_szTitle);
      ppt->InitPostLoad(this);

      AddMDITable(mdiTable);
      // auto-import POV settings if exist...

      char szFileNameAuto[MAX_PATH];
      strcpy_s(szFileNameAuto, m_currentTablePath);
      strcat_s(szFileNameAuto, ppt->m_szTitle);
      strcat_s(szFileNameAuto, ".pov");
      if (Exists(szFileNameAuto)) // We check if there is a table pov settings first
      {
          ppt->ImportBackdropPOV(szFileNameAuto);
      }
      else // Otherwise, we seek for autopov settings
      {
          strcpy_s(szFileNameAuto, m_currentTablePath);
          strcat_s(szFileNameAuto, "autopov.pov");
          if (Exists(szFileNameAuto))
          {
              ppt->ImportBackdropPOV(szFileNameAuto);
          }
      }

      // auto-import VBS table script, if exist...
      strcpy_s(szFileNameAuto, m_currentTablePath);
      strcat_s(szFileNameAuto, ppt->m_szTitle);
      strcat_s(szFileNameAuto, ".vbs");
      if (Exists(szFileNameAuto)) // We check if there is a table pov settings first
      {
          ppt->m_pcv->LoadFromFile(szFileNameAuto);
      }

      // get the load path from the filename
      SaveValueString("RecentDir", "LoadDir", m_currentTablePath);

      // make sure the load directory is the active directory
      SetCurrentDirectory(m_currentTablePath);
      UpdateRecentFileList(szFileName);
      ppt->AddMultiSel(ppt, false, true, false);
      ppt->SetDirty(eSaveClean);
      if(updateEditor)
      {
          GetLayersListDialog()->CollapseLayers();
          GetLayersListDialog()->ExpandLayers();
          ToggleToolbar();
          SetFocus();
      }
   }
}

CComObject<PinTable> *VPinball::GetActiveTable()
{
    PinTableMDI *mdiTable = (PinTableMDI *)GetActiveMDIChild();
    if (mdiTable)
    {
        if (!m_unloadingTable)
            return (CComObject<PinTable>*)mdiTable->GetTable();
        else
            return nullptr;
    }
    return nullptr;
}

bool VPinball::CanClose()
{
   while (m_vtable.size())
   {
       if (!m_vtable[0]->GetMDITable()->CanClose())
           return false;

       CloseTable(m_vtable[0]);
   }

   return true;
}

bool VPinball::CloseTable(PinTable * const ppt)
{
   m_unloadingTable = true;
   ppt->GetMDITable()->SendMessage(WM_SYSCOMMAND, SC_CLOSE, 0);
   m_unloadingTable = false;

   std::vector<MDIChildPtr> allChildren = GetAllMDIChildren();
   if (allChildren.size() == 0)
   {
       ToggleToolbar();
       if (m_propertyDialog && m_propertyDialog->IsWindow())
           m_propertyDialog->DeleteAllTabs();
   }
   return true;
}

void VPinball::SetEnableMenuItems()
{
   CComObject<PinTable> * const ptCur = GetActiveTable();

   // Set menu item to the correct state
   CMenu mainMenu = GetMenu();

   mainMenu.CheckMenuItem(ID_EDIT_BACKGLASSVIEW, MF_BYCOMMAND | (m_backglassView ? MF_CHECKED : MF_UNCHECKED));

   // is there a valid table??
   if (ptCur)
   {
      mainMenu.CheckMenuItem(ID_EDIT_SCRIPT, MF_BYCOMMAND | ((ptCur->m_pcv!=nullptr && ptCur->m_pcv->m_visible && !ptCur->m_pcv->m_minimized) ? MF_CHECKED : MF_UNCHECKED));

      mainMenu.EnableMenuItem(IDM_CLOSE, MF_BYCOMMAND | MF_ENABLED);
      mainMenu.EnableMenuItem(ID_EDIT_UNDO, MF_BYCOMMAND | MF_ENABLED);
      mainMenu.EnableMenuItem(ID_EDIT_BACKGLASSVIEW, MF_BYCOMMAND | MF_ENABLED);
      mainMenu.EnableMenuItem(ID_TABLE_PLAY, MF_BYCOMMAND | MF_ENABLED);
      mainMenu.EnableMenuItem(ID_TABLE_CAMERAMODE, MF_BYCOMMAND | MF_ENABLED);
      mainMenu.EnableMenuItem(ID_TABLE_MAGNIFY, MF_BYCOMMAND | MF_ENABLED);
      mainMenu.EnableMenuItem(ID_TABLE_TABLEINFO, MF_BYCOMMAND | MF_ENABLED);
      mainMenu.EnableMenuItem(ID_EDIT_SEARCH, MF_BYCOMMAND | MF_ENABLED);
      mainMenu.EnableMenuItem(ID_EDIT_DRAWINGORDER_HIT, MF_BYCOMMAND | MF_ENABLED);
      mainMenu.EnableMenuItem(ID_EDIT_DRAWINGORDER_SELECT, MF_BYCOMMAND | MF_ENABLED);
      // enable/disable save options
      UINT flags = MF_BYCOMMAND | MF_ENABLED;
      mainMenu.EnableMenuItem(IDM_SAVE, flags);
      mainMenu.EnableMenuItem(IDM_SAVEAS, flags);
      mainMenu.EnableMenuItem(ID_FILE_EXPORT_BLUEPRINT, flags);
      mainMenu.EnableMenuItem(ID_EXPORT_TABLEMESH, flags);
      mainMenu.EnableMenuItem(ID_EXPORT_BACKDROPPOV, flags);
      mainMenu.EnableMenuItem(ID_IMPORT_BACKDROPPOV, flags);

      // enable/disable script option
      mainMenu.EnableMenuItem(ID_EDIT_SCRIPT, flags);

      // enable/disable managers options
      mainMenu.EnableMenuItem(ID_TABLE_SOUNDMANAGER, flags);
      mainMenu.EnableMenuItem(ID_TABLE_IMAGEMANAGER, flags);
      mainMenu.EnableMenuItem(ID_TABLE_FONTMANAGER, flags);
      mainMenu.EnableMenuItem(ID_TABLE_MATERIALMANAGER, flags);
      mainMenu.EnableMenuItem(ID_TABLE_COLLECTIONMANAGER, flags);

      // enable/disable editing options
      mainMenu.EnableMenuItem(IDC_COPY, flags);
      mainMenu.EnableMenuItem(IDC_PASTE, flags);
      mainMenu.EnableMenuItem(IDC_PASTEAT, flags);
      mainMenu.EnableMenuItem(ID_DELETE, flags);

      mainMenu.CheckMenuItem(ID_VIEW_SOLID, MF_BYCOMMAND | (ptCur->RenderSolid() ? MF_CHECKED : MF_UNCHECKED));
      mainMenu.CheckMenuItem(ID_VIEW_OUTLINE, MF_BYCOMMAND | (ptCur->RenderSolid() ? MF_UNCHECKED : MF_CHECKED));

      mainMenu.CheckMenuItem(ID_VIEW_GRID, MF_BYCOMMAND | (ptCur->m_grid ? MF_CHECKED : MF_UNCHECKED));
      mainMenu.CheckMenuItem(ID_VIEW_BACKDROP, MF_BYCOMMAND | (ptCur->m_backdrop ? MF_CHECKED : MF_UNCHECKED));
   }
   else
   {
      /* no valid table, disable a few items */
      mainMenu.EnableMenuItem(IDM_CLOSE, MF_BYCOMMAND | MF_GRAYED);
      mainMenu.EnableMenuItem(IDM_SAVE, MF_BYCOMMAND | MF_GRAYED);
      mainMenu.EnableMenuItem(IDM_SAVEAS, MF_BYCOMMAND | MF_GRAYED);
      mainMenu.EnableMenuItem(ID_FILE_EXPORT_BLUEPRINT, MF_BYCOMMAND | MF_GRAYED);
      mainMenu.EnableMenuItem(ID_EXPORT_TABLEMESH, MF_BYCOMMAND | MF_GRAYED);
      mainMenu.EnableMenuItem(ID_EXPORT_BACKDROPPOV, MF_BYCOMMAND | MF_GRAYED);
      mainMenu.EnableMenuItem(ID_IMPORT_BACKDROPPOV, MF_BYCOMMAND | MF_GRAYED);

      mainMenu.EnableMenuItem(ID_EDIT_UNDO, MF_BYCOMMAND | MF_GRAYED);
      mainMenu.EnableMenuItem(IDC_COPY, MF_BYCOMMAND | MF_GRAYED);
      mainMenu.EnableMenuItem(IDC_PASTE, MF_BYCOMMAND | MF_GRAYED);
      mainMenu.EnableMenuItem(IDC_PASTEAT, MF_BYCOMMAND | MF_GRAYED);
      mainMenu.EnableMenuItem(ID_DELETE, MF_BYCOMMAND | MF_GRAYED);
      mainMenu.EnableMenuItem(ID_EDIT_SCRIPT, MF_BYCOMMAND | MF_GRAYED);
      mainMenu.EnableMenuItem(ID_EDIT_BACKGLASSVIEW, MF_BYCOMMAND | MF_GRAYED);

      mainMenu.EnableMenuItem(ID_TABLE_PLAY, MF_BYCOMMAND | MF_GRAYED);
      mainMenu.EnableMenuItem(ID_TABLE_CAMERAMODE, MF_BYCOMMAND | MF_GRAYED);
      mainMenu.EnableMenuItem(ID_TABLE_SOUNDMANAGER, MF_BYCOMMAND | MF_GRAYED);
      mainMenu.EnableMenuItem(ID_TABLE_IMAGEMANAGER, MF_BYCOMMAND | MF_GRAYED);
      mainMenu.EnableMenuItem(ID_TABLE_FONTMANAGER, MF_BYCOMMAND | MF_GRAYED);
      mainMenu.EnableMenuItem(ID_TABLE_MATERIALMANAGER, MF_BYCOMMAND | MF_GRAYED);
      mainMenu.EnableMenuItem(ID_TABLE_COLLECTIONMANAGER, MF_BYCOMMAND | MF_GRAYED);
      mainMenu.EnableMenuItem(ID_TABLE_TABLEINFO, MF_BYCOMMAND | MF_GRAYED);
      mainMenu.EnableMenuItem(ID_TABLE_MAGNIFY, MF_BYCOMMAND | MF_GRAYED);
      mainMenu.EnableMenuItem(ID_EDIT_SEARCH, MF_BYCOMMAND | MF_GRAYED);
      mainMenu.EnableMenuItem(ID_EDIT_DRAWINGORDER_HIT, MF_BYCOMMAND | MF_GRAYED);
      mainMenu.EnableMenuItem(ID_EDIT_DRAWINGORDER_SELECT, MF_BYCOMMAND | MF_GRAYED);
   }
}

void VPinball::UpdateRecentFileList(const char *szfilename)
{
   // if the loaded file name is not null then add it to the top of the list
   if (szfilename != NULL)
   {
      // does this file name aready exist in the list?
      bool found = false;
      int i = 0;

      std::vector<std::string> newList;
      newList.push_back(std::string(szfilename));

      for (std::string &tableName : m_recentTableList)
      {
          if(tableName != std::string(szfilename))
              newList.push_back(tableName);
      }

      m_recentTableList = newList;

      for (std::string tableName : m_recentTableList)
      {
          char szRegName[MAX_PATH];

          // write entry to the registry
          sprintf_s(szRegName, "TableFileName%d", i++);
          SaveValueString("RecentDir", szRegName, tableName);
      }
   }

   // update the file menu to contain the last n recent loaded files
   // must be at least 1 recent file in the list
   if (!m_recentTableList.empty())
   {
      MENUITEMINFO menuInfo;

      // update the file menu to contain the last n recent loaded files
      CMenu menuFile = GetMainMenu(FILEMENU);

      // delete all the recent file IDM's from this menu
      for (int i = RECENT_FIRST_MENU_IDM; i <= RECENT_LAST_MENU_IDM; i++)
         menuFile.DeleteMenu(i, MF_BYCOMMAND);

      // get the number of entries in the file menu
      // insert the items before the EXIT menu (assuming it is the last entry)
      int count = menuFile.GetMenuItemCount() - 1;


      // set up the menu info block
      ZeroMemory(&menuInfo, sizeof(menuInfo));
      menuInfo.cbSize = GetSizeofMenuItemInfo();
      menuInfo.fMask = MIIM_ID | MIIM_TYPE | MIIM_STATE;
      menuInfo.fType = MFT_STRING;

      // add in the list of recently accessed files
      for (size_t i = 0; i < m_recentTableList.size(); i++)
      {
         // now search for filenames with & and replace with && so that these display correctly
         const char * const ns = replace(m_recentTableList[i].c_str(), "&", "&&");
         char recentMenuname[MAX_PATH];
         snprintf(recentMenuname, MAX_PATH - 1, "&%i %s", i+1, ns);
         delete[] ns;
         // set the IDM of this menu item
         menuInfo.wID = RECENT_FIRST_MENU_IDM + i;
         menuInfo.dwTypeData = recentMenuname;

         menuFile.InsertMenuItem(count, menuInfo, TRUE);
         count++;
      }

      // add a separator onto the end
      menuInfo.fType = MFT_SEPARATOR;
      menuInfo.wID = RECENT_LAST_MENU_IDM;
      menuFile.InsertMenuItem(count, menuInfo, TRUE);

      // update the menu bar
      DrawMenuBar();
   }
}

bool VPinball::processKeyInputForDialogs(MSG *pmsg)
{
    bool consumed = false;
    if (m_ptableActive)
    {
      if (m_materialDialog.IsWindow())
          consumed = !!m_materialDialog.IsDialogMessage(*pmsg);
      if (!consumed && m_imageMngDlg.IsWindow())
          consumed = !!m_imageMngDlg.IsDialogMessage(*pmsg);
      if (!consumed && m_soundMngDlg.IsWindow())
          consumed = !!m_soundMngDlg.IsDialogMessage(*pmsg);
      if (!consumed && m_collectionMngDlg.IsWindow())
          consumed = !!m_collectionMngDlg.IsDialogMessage(*pmsg);
      if (!consumed && m_dimensionDialog.IsWindow())
          consumed = !!m_dimensionDialog.IsDialogMessage(*pmsg);
      if (!consumed && m_toolbarDialog && m_toolbarDialog->IsWindow())
          consumed = !!m_toolbarDialog->IsDialogMessage(*pmsg);
      if (!consumed && m_propertyDialog && m_propertyDialog->IsWindow())
          consumed = !!m_propertyDialog->IsSubDialogMessage(*pmsg);
      if (!consumed && m_layersListDialog && m_layersListDialog->IsWindow())
          consumed = !!m_layersListDialog->IsDialogMessage(*pmsg);
    }
    return consumed;
}

bool VPinball::ApcHost_OnTranslateMessage(MSG* pmsg)
{
   bool consumed=false;

   if (g_pplayer==nullptr)
   {
      // check if message must be processed by the code editor
      if (m_pcv) 
         consumed = m_pcv->PreTranslateMessage(pmsg);

      if (m_pcv && m_pcv->m_hwndFind && ::IsDialogMessage(m_pcv->m_hwndFind, pmsg))
         consumed = true;

      if (!consumed)
         consumed = !!TranslateAccelerator(GetHwnd(), g_haccel, pmsg);

      if(!consumed)
         // check/process events for other dialogs (material/sound/image manager, toolbar, properties
         consumed = processKeyInputForDialogs(pmsg);

      if (!consumed)
         TranslateMessage(pmsg);
   }
   else
   {
      consumed = false;
      if (g_pplayer->m_debugMode)
      {
          if (g_pplayer->m_debuggerDialog.IsWindow())
            consumed = !!g_pplayer->m_debuggerDialog.IsSubDialogMessage(*pmsg);
      }
   }

   return consumed;
}

void VPinball::MainMsgLoop()
{
   for (;;)
   {
      MSG msg;
      if (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE))
      {
         if (msg.message == WM_QUIT)
            break;

         const bool consumed = ApcHost_OnTranslateMessage(&msg);
         if (!consumed)
            DispatchMessage(&msg);
      }
      else
      {
         if (g_pplayer && !g_pplayer->m_pause)
            g_pplayer->Render(); // always render on idle
         else
            WaitMessage(); // otherwise wait for input
      }
   }
}

STDMETHODIMP VPinball::QueryInterface(REFIID iid, void **ppvObjOut)
{
   if (!ppvObjOut)
      return E_INVALIDARG;

   *ppvObjOut = NULL;

   if (*ppvObjOut)
   {
      this->AddRef();
      return S_OK;
   }

   return E_NOINTERFACE;
}


STDMETHODIMP_(ULONG) VPinball::AddRef()
{
   //!! ?? ASSERT(m_cref, "bad m_cref");
   return ++m_cref;
}

STDMETHODIMP_(ULONG) VPinball::Release()
{
   assert(m_cref);
   m_cref--;

   return m_cref;
}

void VPinball::PreCreate(CREATESTRUCT& cs)
{
    // do the base class stuff
    CWnd::PreCreate(cs);
    const int screenwidth = GetSystemMetrics(SM_CXSCREEN);		// width of primary monitor
    const int screenheight = GetSystemMetrics(SM_CYSCREEN);		// height of primary monitor

    const int x = (screenwidth - MAIN_WINDOW_WIDTH) / 2;
    const int y = (screenheight - MAIN_WINDOW_HEIGHT) / 2;
    const int width = MAIN_WINDOW_WIDTH;
    const int height = MAIN_WINDOW_HEIGHT;

    cs.x = x;  // set initial window placement
    cs.y = y;
    cs.cx = width;
    cs.cy = height;
    // specify a title bar and border with a window-menu on the title bar
    cs.style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_SIZEBOX | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
    cs.dwExStyle = WS_EX_CLIENTEDGE
        | WS_EX_CONTROLPARENT       // TAB key navigation
//      | WS_EX_CONTEXTHELP     // doesn't work if WS_MINIMIZEBOX
                    // or WS_MAXIMIZEBOX is specified
        ;
}

void VPinball::PreRegisterClass(WNDCLASS& wc)
{
    wc.hIcon = LoadIcon(g_hinst, MAKEINTRESOURCE(IDI_VPINBALL));
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.style = CS_DBLCLKS;//CS_NOCLOSE | CS_OWNDC;
    wc.lpszClassName = _T("VPinball");
    //wc.lpszMenuName = MAKEINTRESOURCE(IDR_APPMENU);    
}

void VPinball::OnClose()
{
   CComObject<PinTable> * const ptable = GetActiveTable();
   m_closing = true;

   if (ptable)
   {
      while (ptable->m_savingActive)
         Sleep(THREADS_PAUSE);
   }
   if (g_pplayer)
       g_pplayer->SendMessage(WM_CLOSE, 0, 0);

   const bool canClose = CanClose();
   if (canClose)
   {
      WINDOWPLACEMENT winpl;
      winpl.length = sizeof(winpl);

      if (GetWindowPlacement(winpl))
      {
         SaveValueInt("Editor", "WindowLeft", winpl.rcNormalPosition.left);
         SaveValueInt("Editor", "WindowTop", winpl.rcNormalPosition.top);
         SaveValueInt("Editor", "WindowRight", winpl.rcNormalPosition.right);
         SaveValueInt("Editor", "WindowBottom", winpl.rcNormalPosition.bottom);

         SaveValueBool("Editor", "WindowMaximized", !!IsZoomed());
      }
      SaveDockRegistrySettings(DOCKER_REGISTRY_KEY);

      CWnd::OnClose();
   }
}

void VPinball::OnDestroy()
{
    PostMessage(WM_QUIT, 0, 0);
}

void VPinball::ShowSubDialog(CDialog &dlg)
{
    if (!dlg.IsWindow())
    {
       dlg.Create(GetHwnd());
       dlg.ShowWindow();
    }
    else
       dlg.SetForegroundWindow();
}

int VPinball::OnCreate(CREATESTRUCT& cs)
{
    // OnCreate controls the way the frame is created.
    // Overriding CFrame::OnCreate is optional.
    // Uncomment the lines below to change frame options.

//     UseIndicatorStatus(FALSE);    // Don't show keyboard indicators in the StatusBar
//     UseMenuStatus(FALSE);         // Don't show menu descriptions in the StatusBar
//     UseReBar(FALSE);              // Don't use a ReBar
//     UseStatusBar(FALSE);          // Don't use a StatusBar
//     UseThemes(FALSE);             // Don't use themes
//     UseToolBar(FALSE);            // Don't use a ToolBar
    m_mainMenu.LoadMenu(IDR_APPMENU);

    SetFrameMenu(m_mainMenu.GetHandle());

    LPTSTR lpCmdLine = GetCommandLine();						//this line necessary for _ATL_MIN_CRT
    if (strstr(lpCmdLine, "minimized"))
        SetOpenMinimized();

    char szName[256];
    LoadString(g_hinst, IDS_PROJNAME, szName, 256);
    
    const int result = CMDIDockFrame::OnCreate(cs);
    
    SetWindowText(szName);
    return result;
}

LRESULT VPinball::OnPaint(UINT msg, WPARAM wparam, LPARAM lparam)
{
    PAINTSTRUCT ps;
    const HDC hdc = BeginPaint(ps);
    const CRect rc = GetClientRect();
    SelectObject(hdc, GetStockObject(WHITE_BRUSH));
    PatBlt(hdc, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, PATCOPY);
    EndPaint(ps);
    return 0;
}

void VPinball::OnInitialUpdate()
{
    if (!firstRun)
        return;
    

    const int foo[6] = {120, 240, 400, 600, 800, 1400};

    m_hwndStatusBar = CreateStatusWindow(WS_CHILD | WS_VISIBLE,
                                         "",
                                         GetHwnd(),
                                         1);											// Create Status Line at the bottom

    ::SendMessage(m_hwndStatusBar, SB_SETPARTS, 6, (size_t)foo);	// Initialize Status bar with 6 empty cells

    InitRegValues();									// get default values from registry

    SendMessage(WM_SIZE, 0, 0);			// Make our window relay itself out

    m_ps.InitPinDirectSound(GetHwnd());

    m_backglassView = false;							// we are viewing Pinfield and not the backglass at first

    UpdateRecentFileList(NULL);						// update the recent loaded file list

    wintimer_init();									// calibrate the timer routines

    int left, top, right, bottom;
    BOOL maximized;

    const HRESULT hrleft = LoadValueInt("Editor", "WindowLeft", &left);
    const HRESULT hrtop = LoadValueInt("Editor", "WindowTop", &top);
    const HRESULT hrright = LoadValueInt("Editor", "WindowRight", &right);
    const HRESULT hrbottom = LoadValueInt("Editor", "WindowBottom", &bottom);

    const HRESULT hrmax = LoadValueInt("Editor", "WindowMaximized", &maximized);

    if (hrleft == S_OK && hrtop == S_OK && hrright == S_OK && hrbottom == S_OK)
    {
        WINDOWPLACEMENT winpl;
        ZeroMemory(&winpl, sizeof(WINDOWPLACEMENT));
        winpl.length = sizeof(WINDOWPLACEMENT);

        GetWindowPlacement(winpl);

        winpl.rcNormalPosition.left = left;
        winpl.rcNormalPosition.top = top;
        winpl.rcNormalPosition.right = right;
        winpl.rcNormalPosition.bottom = bottom;

        if (m_open_minimized)
            winpl.showCmd |= SW_MINIMIZE;
        else if (hrmax == S_OK && maximized)
            winpl.showCmd |= SW_MAXIMIZE;
        else
            winpl.showCmd |= SW_SHOWNORMAL;

        SetWindowPlacement(winpl);
    }
#ifdef SLINTF
    // see slintf.cpp
    slintf_init();									// initialize debug console (can be popupped by the following command)
    slintf_popup_console();
    slintf("Debug output:\n");
#endif

    CreateDocker();
    ShowWindow(SW_SHOW);
    //InitTools();
//    SetForegroundWindow();
    SetEnableMenuItems();
    firstRun = false;
}

BOOL VPinball::OnCommand(WPARAM wparam, LPARAM lparam)
{
    if (!ParseCommand(LOWORD(wparam), HIWORD(wparam)))
    {
        if(GetActiveMDIChild())
            GetActiveMDIChild()->SendMessage(WM_COMMAND, wparam, lparam);
        return FALSE;
    }
    return TRUE;
}

LRESULT VPinball::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_KEYUP:
        {
            if (wParam == VK_ESCAPE)
            {
                if (m_ToolCur != IDC_SELECT)
                    m_ToolCur=IDC_SELECT;
                CComObject<PinTable>* const ptCur = GetActiveTable();
                if (ptCur)
                    ptCur->SetMouseCursor();

            }
            return FinalWindowProc(uMsg, wParam, lParam);
        }
        case WM_TIMER:
        {
            CComObject<PinTable>* const ptCur = GetActiveTable();
            if (!ptCur)
                break;

            switch (wParam)
            {
                case TIMER_ID_AUTOSAVE:
                {
                    ptCur->AutoSave();
                    break;
                }

                case TIMER_ID_CLOSE_TABLE:
                {
                    KillTimer(TIMER_ID_CLOSE_TABLE);
                    CloseTable(ptCur);
                    return 0;
                }
            }
            return FinalWindowProc(uMsg, wParam, lParam);
        }
        case WM_SIZE:
        {
            ::SendMessage(m_hwndStatusBar, WM_SIZE, 0, 0);
            break;
        }
    }
    return WndProcDefault(uMsg, wParam, lParam);
}

LRESULT VPinball::OnMDIActivated(UINT msg, WPARAM wparam, LPARAM lparam)
{
    GetLayersListDialog()->UpdateLayerList();
    return CMDIFrameT::OnMDIActivated(msg, wparam, lparam);
}

Win32xx::CDocker *VPinball::NewDockerFromID(int id)
{
    switch (id)
    {
        case IDD_PROPERTY_DIALOG:
        {
            if (m_dockProperties == nullptr)
            {
                m_dockProperties = new CDockProperty();
                m_propertyDialog = m_dockProperties->GetContainProperties()->GetPropertyDialog();
            }
            return m_dockProperties;
        }
        case IDD_TOOLBAR:
        {
            if (m_dockToolbar == nullptr)
            {
                m_dockToolbar = new CDockToolbar();
                m_toolbarDialog = m_dockToolbar->GetContainToolbar()->GetToolbarDialog();
            }
            return m_dockToolbar;
        }
        case IDD_LAYERS:
        {
            if (m_dockLayers == nullptr)
            {
                m_dockLayers = new CDockLayers();
                m_layersListDialog = m_dockLayers->GetContainLayers()->GetLayersDialog();
            }
            return m_dockLayers;
        }
    }
    return nullptr;
}

STDMETHODIMP VPinball::PlaySound(BSTR bstr)
{
   if (g_pplayer) g_pplayer->m_ptable->PlaySound(bstr, 0, 1.f, 0.f, 0.f, 0, VARIANT_FALSE, VARIANT_TRUE, 0.f);

   return S_OK;
}

STDMETHODIMP VPinball::FireKnocker(int Count)
{
   if (g_pplayer) g_pplayer->m_ptable->FireKnocker(Count);

   return S_OK;
}

STDMETHODIMP VPinball::QuitPlayer(int CloseType)
{
   if (g_pplayer) g_pplayer->m_ptable->QuitPlayer(CloseType);

   return S_OK;
}

void VPinball::Quit()
{
   if (g_pplayer) {
      g_pplayer->m_closeDown = true;
      g_pplayer->m_closeType = 1;
   }
   else
      PostMessage(WM_CLOSE, 0, 0);
}

int CALLBACK MyCompProc(LPARAM lSortParam1, LPARAM lSortParam2, LPARAM lSortOption)
{
   LVFINDINFO lvf;
   char buf1[MAX_PATH], buf2[MAX_PATH];

   SORTDATA *lpsd = (SORTDATA *)lSortOption;

   lvf.flags = LVFI_PARAM;
   lvf.lParam = lSortParam1;
   const int nItem1 = ListView_FindItem(lpsd->hwndList, -1, &lvf);

   lvf.lParam = lSortParam2;
   const int nItem2 = ListView_FindItem(lpsd->hwndList, -1, &lvf);

   ListView_GetItemText(lpsd->hwndList, nItem1, lpsd->subItemIndex, buf1, sizeof(buf1));

   ListView_GetItemText(lpsd->hwndList, nItem2, lpsd->subItemIndex, buf2, sizeof(buf2));
   if (nItem2 == -1 || nItem1 == -1)
       return 0;
   if (lpsd->sortUpDown == 1)
      return(_stricmp(buf1, buf2));
   else
      return(_stricmp(buf1, buf2) * -1);
}

int CALLBACK MyCompProcIntValues(LPARAM lSortParam1, LPARAM lSortParam2, LPARAM lSortOption)
{
    LVFINDINFO lvf;
    char buf1[MAX_PATH], buf2[MAX_PATH];

    SORTDATA * const lpsd = (SORTDATA *)lSortOption;

    lvf.flags = LVFI_PARAM;
    lvf.lParam = lSortParam1;
    const int nItem1 = ListView_FindItem(lpsd->hwndList, -1, &lvf);

    lvf.lParam = lSortParam2;
    const int nItem2 = ListView_FindItem(lpsd->hwndList, -1, &lvf);

    ListView_GetItemText(lpsd->hwndList, nItem1, lpsd->subItemIndex, buf1, sizeof(buf1));
    ListView_GetItemText(lpsd->hwndList, nItem2, lpsd->subItemIndex, buf2, sizeof(buf2));

    int value1, value2;
    sscanf_s(buf1, "%i", &value1);
    sscanf_s(buf2, "%i", &value2);

    if (lpsd->sortUpDown == 1)
        return(value1-value2);
    else
        return(value2-value1);
}

static const int rgDlgIDFromSecurityLevel[] = { IDC_ACTIVEX0, IDC_ACTIVEX1, IDC_ACTIVEX2, IDC_ACTIVEX3, IDC_ACTIVEX4 };

INT_PTR CALLBACK SecurityOptionsProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
   switch (uMsg)
   {
   case WM_INITDIALOG:
   {
      const HWND hwndParent = GetParent(hwndDlg);
      RECT rcDlg;
      RECT rcMain;
      GetWindowRect(hwndParent, &rcMain);
      GetWindowRect(hwndDlg, &rcDlg);

      SetWindowPos(hwndDlg, NULL,
         (rcMain.right + rcMain.left) / 2 - (rcDlg.right - rcDlg.left) / 2,
         (rcMain.bottom + rcMain.top) / 2 - (rcDlg.bottom - rcDlg.top) / 2,
         0, 0, SWP_NOOWNERZORDER | SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE/* | SWP_NOMOVE*/);

      int security = LoadValueIntWithDefault("Player", "SecurityLevel", DEFAULT_SECURITY_LEVEL);
      if (security < 0 || security > 4)
         security = 0;

      const int buttonid = rgDlgIDFromSecurityLevel[security];

      SendMessage(GetDlgItem(hwndDlg, buttonid), BM_SETCHECK, BST_CHECKED, 0);

      const bool hangdetect = LoadValueBoolWithDefault("Player", "DetectHang", false);
      SendMessage(GetDlgItem(hwndDlg, IDC_HANGDETECT), BM_SETCHECK, hangdetect ? BST_CHECKED : BST_UNCHECKED, 0);

      return TRUE;
   }
   break;

   case WM_COMMAND:
   {
      switch (HIWORD(wParam))
      {
      case BN_CLICKED:
         switch (LOWORD(wParam))
         {
         case IDOK:
         {
            for (int i = 0; i < 5; i++)
            {
               const size_t checked = SendMessage(GetDlgItem(hwndDlg, rgDlgIDFromSecurityLevel[i]), BM_GETCHECK, 0, 0);
               if (checked == BST_CHECKED)
                  SaveValueInt("Player", "SecurityLevel", i);
            }

            const bool hangdetect = (SendMessage(GetDlgItem(hwndDlg, IDC_HANGDETECT), BM_GETCHECK, 0, 0) != 0);
            SaveValueBool("Player", "DetectHang", hangdetect);

            EndDialog(hwndDlg, TRUE);
         }
         break;

         case IDCANCEL:
            EndDialog(hwndDlg, FALSE);
            break;
         }
      }
   }
   break;

   case WM_CLOSE:
      EndDialog(hwndDlg, FALSE);
      break;
   }

   return FALSE;
}

INT_PTR CALLBACK FontManagerProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
   CCO(PinTable) *pt = (CCO(PinTable) *)GetWindowLongPtr(hwndDlg, GWLP_USERDATA);

   switch (uMsg)
   {
   case WM_INITDIALOG:
   {
      SetWindowLongPtr(hwndDlg, GWLP_USERDATA, lParam);

      LVCOLUMN lvcol;
      lvcol.mask = LVCF_TEXT | LVCF_WIDTH;
      LocalString ls(IDS_NAME);
      lvcol.pszText = ls.m_szbuffer;// = "Name";
      lvcol.cx = 100;
      ListView_InsertColumn(GetDlgItem(hwndDlg, IDC_SOUNDLIST), 0, &lvcol);

      LocalString ls2(IDS_IMPORTPATH);
      lvcol.pszText = ls2.m_szbuffer; // = "Import Path";
      lvcol.cx = 200;
      ListView_InsertColumn(GetDlgItem(hwndDlg, IDC_SOUNDLIST), 1, &lvcol);

      pt = (CCO(PinTable) *)GetWindowLongPtr(hwndDlg, GWLP_USERDATA);

      pt->ListFonts(GetDlgItem(hwndDlg, IDC_SOUNDLIST));

      return TRUE;
   }
   break;

   case WM_CLOSE:
      EndDialog(hwndDlg, FALSE);
      break;

   case WM_COMMAND:
      switch (HIWORD(wParam))
      {
      case BN_CLICKED:
         switch (LOWORD(wParam))
         {
         case IDOK:
            EndDialog(hwndDlg, TRUE);
            break;

         case IDCANCEL:
            EndDialog(hwndDlg, FALSE);
            break;

         case IDC_IMPORT:
         {
            char szFileName[MAXSTRING];
            char szInitialDir[MAXSTRING];
            int  fileOffset;
            szFileName[0] = '\0';

            /*const HRESULT hr =*/ LoadValueString("RecentDir", "FontDir", szInitialDir, MAXSTRING);
            if (g_pvp->OpenFileDialog(szInitialDir, szFileName, "Font Files (*.ttf)\0*.ttf\0", "ttf", 0, fileOffset))
            {
               strcpy_s(szInitialDir, sizeof(szInitialDir), szFileName);
               szInitialDir[fileOffset] = 0;
               SaveValueString("RecentDir", "FontDir", szInitialDir);
               pt->ImportFont(GetDlgItem(hwndDlg, IDC_SOUNDLIST), szFileName);
            }
         }
         break;

         /*case IDC_RENAME:
         {
         int sel = ListView_GetNextItem(GetDlgItem(hwndDlg, IDC_SOUNDLIST), -1, LVNI_SELECTED);
         if (sel != -1)
         {
         SetFocus(GetDlgItem(hwndDlg, IDC_SOUNDLIST));
         HWND hwndFoo = ListView_EditLabel(GetDlgItem(hwndDlg, IDC_SOUNDLIST), sel);
         }
         }
         break;*/

         case IDC_DELETE:
         {
            const int sel = ListView_GetNextItem(GetDlgItem(hwndDlg, IDC_SOUNDLIST), -1, LVNI_SELECTED);
            if (sel != -1)
            {
               // TEXT
               const int ans = MessageBox(hwndDlg, "Are you sure you want to remove this font?", "Confirm Deletion", MB_YESNO | MB_DEFBUTTON2);
               if (ans == IDYES)
               {
                  LVITEM lvitem;
                  lvitem.mask = LVIF_PARAM;
                  lvitem.iItem = sel;
                  lvitem.iSubItem = 0;
                  ListView_GetItem(GetDlgItem(hwndDlg, IDC_SOUNDLIST), &lvitem);
                  PinFont * const ppf = (PinFont *)lvitem.lParam;
                  ListView_DeleteItem(GetDlgItem(hwndDlg, IDC_SOUNDLIST), sel);
                  pt->RemoveFont(ppf);
               }
            }
         }
         break;

         /*case IDC_REIMPORT:
         {
         const int ans = MessageBox(hwndDlg, "Are you sure you want to replace this image with a new one?", "Confirm Reimport", MB_YESNO | MB_DEFBUTTON2);
         if (ans == IDYES)
         {
         const int sel = ListView_GetNextItem(GetDlgItem(hwndDlg, IDC_SOUNDLIST), -1, LVNI_SELECTED);
         if (sel != -1)
         {
         LVITEM lvitem;
         lvitem.mask = LVIF_PARAM;
         lvitem.iItem = sel;
         lvitem.iSubItem = 0;
         ListView_GetItem(GetDlgItem(hwndDlg, IDC_SOUNDLIST), &lvitem);
         PinImage * const ppi = (PinImage *)lvitem.lParam;

         pt->ReImportImage(ppi, ppi->m_szPath);

         // Display new image
         InvalidateRect(GetDlgItem(hwndDlg, IDC_PICTUREPREVIEW), NULL, fTrue);
         }
         }
         }
         break;

         case IDC_REIMPORTFROM:
         {
         const int sel = ListView_GetNextItem(GetDlgItem(hwndDlg, IDC_SOUNDLIST), -1, LVNI_SELECTED);
         if (sel != -1)
         {
         char szFileName[MAXSTRING];

         const int ans = MessageBox(hwndDlg, "Are you sure you want to replace this image with a new one?", "Confirm Reimport", MB_YESNO | MB_DEFBUTTON2);
         if (ans == IDYES)
         {
         szFileName[0] = '\0';

         OPENFILENAME ofn;
         ZeroMemory(&ofn, sizeof(OPENFILENAME));
         ofn.lStructSize = sizeof(OPENFILENAME);
         ofn.hInstance = g_hinst;
         ofn.hwndOwner = g_pvp->m_hwnd;
         ofn.lpstrFilter = "Font Files (*.ttf)\0*.ttf\0";
         ofn.lpstrFile = szFileName;
         ofn.nMaxFile = MAXSTRING;
         ofn.lpstrDefExt = "ttf";
         ofn.Flags = OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY;

         const int ret = GetOpenFileName(&ofn);

         if (ret)
         {
         LVITEM lvitem;
         lvitem.mask = LVIF_PARAM;
         lvitem.iItem = sel;
         lvitem.iSubItem = 0;
         ListView_GetItem(GetDlgItem(hwndDlg, IDC_SOUNDLIST), &lvitem);
         PinImage * const ppi = (PinImage *)lvitem.lParam;

         pt->ReImportImage(ppi, ofn.lpstrFile);

         // Display new image
         InvalidateRect(GetDlgItem(hwndDlg, IDC_PICTUREPREVIEW), NULL, fTrue);
         }
         }
         }
         }
         break;*/
         }
         break;
      }
      break;
   }

   return FALSE;
}


void VPinball::ShowDrawingOrderDialog(bool select)
{
   DrawingOrderDialog *orderDlg = new DrawingOrderDialog(select);
   orderDlg->DoModal();
   delete orderDlg;
}

void VPinball::CloseAllDialogs()
{
   if (m_imageMngDlg.IsWindow())
      m_imageMngDlg.Destroy();
   if (m_soundMngDlg.IsWindow())
      m_soundMngDlg.Destroy();
   if (m_audioOptDialog.IsWindow())
      m_audioOptDialog.Destroy();
   if (m_editorOptDialog.IsWindow())
      m_editorOptDialog.Destroy();
   if (m_videoOptDialog.IsWindow())
      m_videoOptDialog.Destroy();
   if (m_collectionMngDlg.IsWindow())
      m_collectionMngDlg.Destroy();
   if (m_physicsOptDialog.IsWindow())
      m_physicsOptDialog.Destroy();
   if (m_tableInfoDialog.IsWindow())
      m_tableInfoDialog.Destroy();
   if (m_dimensionDialog.IsWindow())
      m_dimensionDialog.Destroy();
   if (m_materialDialog.IsWindow())
      m_materialDialog.Destroy();
   if (m_aboutDialog.IsWindow())
      m_aboutDialog.Destroy();
}

void VPinball::ToggleBackglassView()
{
    const bool show = !m_backglassView;
    m_backglassView = show;

    for (size_t i = 0; i < m_vtable.size(); i++)
    {
        PinTable * const ptT = m_vtable[i];
        ptT->SetDefaultView();
        ptT->SetDirtyDraw();
    }

    CComObject<PinTable> * const ptCur = GetActiveTable();
    if (ptCur)
       // Set selection to something in the new view (unless hiding table elements)
        ptCur->AddMultiSel((ISelect *)ptCur, false, true, false);

    ToggleToolbar();
}

void VPinball::ToggleScriptEditor()
{
    CComObject<PinTable> * const ptCur = GetActiveTable();
    if (ptCur)
    {
        const bool alwaysViewScript = LoadValueBoolWithDefault("Editor", "AlwaysViewScript", false);

        ptCur->m_pcv->SetVisible(alwaysViewScript || !(ptCur->m_pcv->m_visible && !ptCur->m_pcv->m_minimized));

        //SendMessage(m_hwndToolbarMain, TB_CHECKBUTTON, ID_EDIT_SCRIPT, MAKELONG(ptCur->m_pcv->m_visible && !ptCur->m_pcv->m_minimized, 0));
    }
}

void VPinball::ShowSearchSelect()
{
    CComObject<PinTable> * const ptCur = GetActiveTable();
    if (ptCur)
    {
        if (!ptCur->m_searchSelectDlg.IsWindow())
        {
            ptCur->m_searchSelectDlg.Create(GetHwnd());

            char windowName[256];
            strcpy_s(windowName, "Search/Select Element - ");
            strncat_s(windowName, ptCur->m_szFileName, 255);
            ptCur->m_searchSelectDlg.SetWindowText(windowName);

            ptCur->m_searchSelectDlg.ShowWindow();
        }
        else
        {
            ptCur->m_searchSelectDlg.ShowWindow();
            ptCur->m_searchSelectDlg.SetForegroundWindow();
        }
    }
}

void VPinball::SetDefaultPhysics()
{
    CComObject<PinTable> * const ptCur = GetActiveTable();
    if (ptCur)
    {
        LocalString ls(IDS_DEFAULTPHYSICS);
        const int answ = MessageBox(ls.m_szbuffer, "Continue?", MB_YESNO | MB_ICONWARNING);
        if (answ == IDYES)
        {
            ptCur->BeginUndo();
            for (int i = 0; i < ptCur->m_vmultisel.size(); i++)
                ptCur->m_vmultisel.ElementAt(i)->SetDefaultPhysics(true);
            ptCur->EndUndo();
        }
    }
}

void VPinball::SetViewSolidOutline(size_t viewId)
{
    CComObject<PinTable> * const ptCur = GetActiveTable();
    if (ptCur)
    {
        ptCur->m_renderSolid = (viewId == ID_VIEW_SOLID);
        GetMenu().CheckMenuItem(ID_VIEW_SOLID, MF_BYCOMMAND | (ptCur->RenderSolid() ? MF_CHECKED : MF_UNCHECKED));
        GetMenu().CheckMenuItem(ID_VIEW_OUTLINE, MF_BYCOMMAND | (ptCur->RenderSolid() ? MF_UNCHECKED : MF_CHECKED));

        ptCur->SetDirtyDraw();
        SaveValueBool("Editor", "RenderSolid", ptCur->m_renderSolid);
    }
}

void VPinball::ShowGridView()
{
    CComObject<PinTable> * const ptCur = GetActiveTable();
    if (ptCur)
    {
        ptCur->put_DisplayGrid(!ptCur->m_grid);
        GetMenu().CheckMenuItem(ID_VIEW_GRID, MF_BYCOMMAND | (ptCur->m_grid ? MF_CHECKED : MF_UNCHECKED));
    }
}

void VPinball::ShowBackdropView()
{
    CComObject<PinTable> * const ptCur = GetActiveTable();
    if (ptCur)
    {
        ptCur->put_DisplayBackdrop(!ptCur->m_backdrop);
        GetMenu().CheckMenuItem(ID_VIEW_BACKDROP, MF_BYCOMMAND | (ptCur->m_backdrop ? MF_CHECKED : MF_UNCHECKED));
    }
}

void VPinball::AddControlPoint()
{
    CComObject<PinTable> * const ptCur = GetActiveTable();
    if (ptCur == NULL)
        return;

    if (ptCur->m_vmultisel.Size() > 0)
    {
        ISelect * const psel = ptCur->m_vmultisel.ElementAt(0);
        if (psel != NULL)
        {
            const POINT pt = ptCur->GetScreenPoint();
            switch (psel->GetItemType())
            {
                case eItemRamp:
                {
                    Ramp * const pRamp = (Ramp*)psel;
                    pRamp->AddPoint(pt.x, pt.y, false);
                    break;
                }
                case eItemLight:
                {
                    Light * const pLight = (Light*)psel;
                    pLight->AddPoint(pt.x, pt.y, false);
                    break;
                }
                case eItemSurface:
                {
                    Surface * const pSurf = (Surface*)psel;
                    pSurf->AddPoint(pt.x, pt.y, false);
                    break;
                }
                case eItemRubber:
                {
                    Rubber *const pRub = (Rubber*)psel;
                    pRub->AddPoint(pt.x, pt.y, false);
                    break;
                }
                default:
                    break;
            }
        }//if (psel != NULL)
    }
}

void VPinball::AddSmoothControlPoint()
{
    CComObject<PinTable> * const ptCur = GetActiveTable();
    if (ptCur == NULL)
        return;

    if (ptCur->m_vmultisel.Size() > 0)
    {
        ISelect * const psel = ptCur->m_vmultisel.ElementAt(0);
        if (psel != NULL)
        {
            const POINT pt = ptCur->GetScreenPoint();
            switch (psel->GetItemType())
            {
                case eItemRamp:
                {
                    Ramp * const pRamp = (Ramp*)psel;
                    pRamp->AddPoint(pt.x, pt.y, true);
                    break;
                }
                case eItemLight:
                {
                    Light * const pLight = (Light*)psel;
                    pLight->AddPoint(pt.x, pt.y, true);
                    break;
                }
                case eItemSurface:
                {
                    Surface * const pSurf = (Surface*)psel;
                    pSurf->AddPoint(pt.x, pt.y, true);
                    break;
                }
                case eItemRubber:
                {
                    Rubber *const pRub = (Rubber*)psel;
                    pRub->AddPoint(pt.x, pt.y, true);
                    break;
                }
                default:
                    break;
            }
        }
    }
}

void VPinball::SaveTable(const bool saveAs)
{
    CComObject<PinTable> * const ptCur = GetActiveTable();
    if (ptCur)
    {
        HRESULT hr;
        if(!saveAs)
            hr = ptCur->TableSave();
        else
            hr = ptCur->SaveAs();

        if (hr == S_OK)
            UpdateRecentFileList(ptCur->m_szFileName);
    }
}

void VPinball::OpenNewTable(size_t tableId)
{
    PinTableMDI *mdiTable = new PinTableMDI(this);

    mdiTable->GetTable()->InitBuiltinTable(this, tableId != ID_NEW_EXAMPLETABLE);
    m_vtable.push_back(mdiTable->GetTable());

    AddMDITable(mdiTable);
    mdiTable->GetTable()->AddMultiSel(mdiTable->GetTable(), false, true, false);
    GetLayersListDialog()->CollapseLayers();
    GetLayersListDialog()->ExpandLayers();
    ToggleToolbar();
    SetFocus();
}

void VPinball::ProcessDeleteElement()
{
    CComObject<PinTable> * const ptCur = GetActiveTable();
    if (ptCur)
        ptCur->OnDelete();
}

void VPinball::OpenRecentFile(const size_t menuId)
{
    // get the index into the recent list menu
    const size_t Index = menuId - RECENT_FIRST_MENU_IDM;
    // copy it into a temporary string so it can be correctly processed
    LoadFileName(m_recentTableList[Index].c_str(),true);
}

void VPinball::CopyPasteElement(const CopyPasteModes mode)
{
    CComObject<PinTable> * const ptCur = GetActiveTable();
    if (ptCur)
    {
        const POINT ptCursor = ptCur->GetScreenPoint();
        switch (mode)
        {
            case COPY:
            {
                ptCur->Copy(ptCursor.x, ptCursor.y);
                break;
            }
            case PASTE:
            {
                ptCur->Paste(false, ptCursor.x, ptCursor.y);
                break;
            }
            case PASTE_AT:
            {
                ptCur->Paste(true, ptCursor.x, ptCursor.y);
                break;
            }
            default:
                break;
        }
    }
}
