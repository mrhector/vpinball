// VPinball.cpp : Implementation of WinMain

#include "stdafx.h"

#ifdef CRASH_HANDLER
#include "StackTrace.h"
#include "CrashHandler.h"
#include "BlackBox.h"
#endif

#include "resource.h"
#include <initguid.h>

#define  SET_CRT_DEBUG_FIELD(a)   _CrtSetDbgFlag((a) | _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG))

#ifndef __STANDALONE__
#include "vpinball_i.c"
#endif

#include <locale>
#include <codecvt>

#include <plog/Init.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Appenders/RollingFileAppender.h>
#ifdef __STANDALONE__
#ifndef __ANDROID__
#include <plog/Appenders/ColorConsoleAppender.h>
#else
#include <plog/Appenders/AndroidAppender.h>
#endif
#endif

#ifdef CRASH_HANDLER
extern "C" int __cdecl _purecall()
{
   ShowError("Pure Virtual Function Call");

   CONTEXT Context = {};
#ifdef _WIN64
   RtlCaptureContext(&Context);
#else
   Context.ContextFlags = CONTEXT_CONTROL;

   __asm
   {
   Label:
      mov[Context.Ebp], ebp;
      mov[Context.Esp], esp;
      mov eax, [Label];
      mov[Context.Eip], eax;
   }
#endif

   char callStack[2048] = {};
   rde::StackTrace::GetCallStack(&Context, true, callStack, sizeof(callStack) - 1);

   ShowError(callStack);

   return 0;
}
#endif

#ifndef __STANDALONE__
#ifndef DISABLE_FORCE_NVIDIA_OPTIMUS
extern "C" {
   __declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}
#else
extern "C" {
   __declspec(dllexport) DWORD NvOptimusEnablement = 0x00000000;
}
#endif

#ifndef DISABLE_FORCE_AMD_HIGHPERF
extern "C" { _declspec(dllexport) DWORD AmdPowerXpressRequestHighPerformance = 0x00000001; }
#endif

#if (WINVER <= 0x0601 /* _WIN32_WINNT_WIN7 */ )
typedef enum ORIENTATION_PREFERENCE {
   ORIENTATION_PREFERENCE_NONE = 0x0,
   ORIENTATION_PREFERENCE_LANDSCAPE = 0x1,
   ORIENTATION_PREFERENCE_PORTRAIT = 0x2,
   ORIENTATION_PREFERENCE_LANDSCAPE_FLIPPED = 0x4,
   ORIENTATION_PREFERENCE_PORTRAIT_FLIPPED = 0x8
} ORIENTATION_PREFERENCE;
typedef BOOL(WINAPI *pSDARP)(ORIENTATION_PREFERENCE orientation);

static pSDARP SetDisplayAutoRotationPreferences = nullptr;
#endif

#if !defined(DEBUG_XXX) && !defined(_CRTDBG_MAP_ALLOC) //&& (!defined(__STDCPP_DEFAULT_NEW_ALIGNMENT__) || (__STDCPP_DEFAULT_NEW_ALIGNMENT__ < 16))
//!! somewhat custom new/delete still needed, otherwise VPX crashes when exiting the player
// is this due to win32xx's whacky Shared_Ptr implementation?
void *operator new(const size_t size_req)
{
   void* const ptr = _aligned_malloc(size_req, 16);
   if (!ptr)
       throw std::bad_alloc{};
   return ptr;
}
void operator delete(void *address)
{
   _aligned_free(address);
}
/*void *operator new[](const size_t size_req)
{
   void* const ptr = _aligned_malloc(size_req, 16);
   if (!ptr)
       throw std::bad_alloc{};
   return ptr;
}
void operator delete[](void *address)
{
   _aligned_free(address);
}*/
#endif

CComModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
END_OBJECT_MAP()


PCHAR* CommandLineToArgvA(PCHAR CmdLine, int* _argc)
{
   PCHAR*  argv;
   PCHAR   _argv;
   int  len;
   ULONG   argc;
   CHAR    a;
   size_t  i, j;

   BOOLEAN  in_QM;
   BOOLEAN  in_TEXT;
   BOOLEAN  in_SPACE;

   len = lstrlen(CmdLine);
   i = ((len + 2) / 2) * sizeof(PVOID) + sizeof(PVOID);

   argv = (PCHAR*)malloc(i + (len + 2) * sizeof(CHAR));
   _argv = (PCHAR)(((PUCHAR)argv) + i);

   argc = 0;
   argv[argc] = _argv;
   in_QM = FALSE;
   in_TEXT = FALSE;
   in_SPACE = TRUE;
   i = 0;
   j = 0;

   while ((a = CmdLine[i])) {
      if (in_QM) {
         if (a == '\"') {
            in_QM = FALSE;
         }
         else {
            _argv[j] = a;
            j++;
         }
      }
      else {
         switch (a) {
         case '\"':
            in_QM = TRUE;
            in_TEXT = TRUE;
            if (in_SPACE) {
               argv[argc] = _argv + j;
               argc++;
            }
            in_SPACE = FALSE;
            break;
         case ' ':
         case '\t':
         case '\n':
         case '\r':
            if (in_TEXT) {
               _argv[j] = '\0';
               j++;
            }
            in_TEXT = FALSE;
            in_SPACE = TRUE;
            break;
         default:
            in_TEXT = TRUE;
            if (in_SPACE) {
               argv[argc] = _argv + j;
               argc++;
            }
            _argv[j] = a;
            j++;
            in_SPACE = FALSE;
            break;
         }
      }
      i++;
   }
   _argv[j] = '\0';
   argv[argc] = nullptr;

   (*_argc) = argc;
   return argv;
}
#endif

#ifdef __STANDALONE__
int g_argc;
char **g_argv;
#endif

void SetupLogger(const bool enable);

robin_hood::unordered_map<ItemTypeEnum, EditableInfo> EditableRegistry::m_map;

static const string options[] = { // keep in sync with option_names & option_descs!
   "h"s,
   "Help"s,
   "?"s,
   "UnregServer"s,
   "RegServer"s,
   "DisableTrueFullscreen"s,
   "EnableTrueFullscreen"s,
   "Minimized"s,
   "ExtMinimized"s,
   "Primary"s,
   "GLES"s,
   "LessCPUthreads"s,
   "Edit"s,
   "Play"s,
   "PovEdit"s,
   "Pov"s,
   "ExtractVBS"s,
   "Ini"s,
   "exit"s // (ab)used by frontend, not handled by us
#ifdef __STANDALONE__
   ,
   "PrefPath"s,
   "listres"s,
   "listsnd"s
#endif
}; // + c1..c9
static const string option_descs[] =
{
   string(),
   string(),
   string(),
   "Unregister VP functions"s,
   "Register VP functions"s,
   "Force-disable True Fullscreen setting"s,
   "Force-enable True Fullscreen setting"s,
   "Start VP in the 'invisible' minimized window mode"s,
   "Start VP in the 'invisible' minimized window mode, but with enabled Pause Menu"s,
   "Force VP to render on the Primary/Pixel(0,0) Monitor"s,
   "[value]  Overrides the global emission scale (day/night setting, value range: 0.115..0.925)"s,
   "Limit the amount of parallel execution"s,
   "[filename]  Load file into VP"s,
   "[filename]  Load and play file"s,
   "[filename]  Load and run file in live editing mode, then export new pov on exit"s,
   "[filename]  Load, export pov and close"s,
   "[filename]  Load, export table script and close"s,
   "[filename]  Use a custom settings file instead of loading it from the default location"s,
   string()
#ifdef __STANDALONE__
   ,
   "[path]  Use a custom preferences path instead of $HOME/.vpinball"s,
   "List available fullscreen resolutions"s,
   "List available sound devices"s
#endif   
};
enum option_names
{
   OPTION_H,
   OPTION_HELP,
   OPTION_QMARK,
   OPTION_UNREGSERVER,
   OPTION_REGSERVER,
   OPTION_DISABLETRUEFULLSCREEN,
   OPTION_ENABLETRUEFULLSCREEN,
   OPTION_MINIMIZED,
   OPTION_EXTMINIMIZED,
   OPTION_PRIMARY,
   OPTION_GLES,
   OPTION_LESSCPUTHREADS,
   OPTION_EDIT,
   OPTION_PLAY,
   OPTION_POVEDIT,
   OPTION_POV,
   OPTION_EXTRACTVBS,
   OPTION_INI,
   OPTION_FRONTEND_EXIT
#ifdef __STANDALONE__
   ,
   OPTION_PREFPATH,
   OPTION_LISTRES,
   OPTION_LISTSND
#endif
};

static bool compare_option(const char *const arg, const option_names option)
{
   return ((lstrcmpi(arg, ('-'+options[option]).c_str()) == 0) ||
           (lstrcmpi(arg, ('/'+options[option]).c_str()) == 0));
}

class VPApp : public CWinApp
{
private:
   bool run;
   bool play;
   bool extractPov;
   bool file;
   bool loadFileResult;
   bool extractScript;
#ifdef __STANDALONE__
   bool listRes;
   bool listSnd;
#endif
   bool bgles;
   float fgles;
#ifdef __STANDALONE__
   string szPrefPath;
#endif
   string szTableFileName;
   string szIniFileName;
   VPinball m_vpinball;

public:
   VPApp(HINSTANCE hInstance)
   {
#ifndef __STANDALONE__
       m_vpinball.theInstance = GetInstanceHandle();
       SetResourceHandle(m_vpinball.theInstance);
#endif
   }

   virtual ~VPApp() 
   {
#ifndef __STANDALONE__
      _Module.Term();
      CoUninitialize();
#endif
      g_pvp = nullptr;

#ifdef _CRTDBG_MAP_ALLOC
#ifdef DEBUG_XXX  //disable this in perference to DevPartner
      _CrtSetDumpClient(MemLeakAlert);
#endif
      _CrtDumpMemoryLeaks();
#endif
   }
   BOOL InitInstance() override
   {
      g_pvp = &m_vpinball;

#ifdef CRASH_HANDLER
      rde::CrashHandler::Init();
#endif

#ifdef _MSC_VER
      // disable auto-rotate on tablets
#if (WINVER <= 0x0601)
      SetDisplayAutoRotationPreferences = (pSDARP)GetProcAddress(GetModuleHandle(TEXT("user32.dll")),
         "SetDisplayAutoRotationPreferences");
      if (SetDisplayAutoRotationPreferences)
         SetDisplayAutoRotationPreferences(ORIENTATION_PREFERENCE_LANDSCAPE);
#else
      SetDisplayAutoRotationPreferences(ORIENTATION_PREFERENCE_LANDSCAPE);
#endif

      //!! max(2u, std::thread::hardware_concurrency()) ??
      SYSTEM_INFO sysinfo;
      GetSystemInfo(&sysinfo);
      m_vpinball.m_logicalNumberOfProcessors = sysinfo.dwNumberOfProcessors; //!! this ignores processor groups, so if at some point we need extreme multi threading, implement this in addition!
#else
      m_vpinball.m_logicalNumberOfProcessors = SDL_GetCPUCount();
#endif

      IsOnWine(); // init static variable in there

#ifdef _MSC_VER
#if _WIN32_WINNT >= 0x0400 & defined(_ATL_FREE_THREADED)
      const HRESULT hRes = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
#else
      const HRESULT hRes = CoInitialize(nullptr);
#endif
      _ASSERTE(SUCCEEDED(hRes));
      _Module.Init(ObjectMap, m_vpinball.theInstance, &LIBID_VPinballLib);
#endif

      file = false;
      play = false;
      bool allowLoadOnStart = true;
      extractPov = false;
      run = true;
      loadFileResult = true;
      extractScript = false;
#ifdef __STANDALONE__
      listRes = false;
      listSnd = false;
#endif
      fgles = 0.f;
      bgles = false;

#ifdef __STANDALONE__
      szPrefPath.clear();
#endif

      szTableFileName.clear();

#ifndef __STANDALONE__
      int nArgs;
      LPSTR *szArglist = CommandLineToArgvA(GetCommandLine(), &nArgs);
#else
      int nArgs = g_argc;
      char**  szArglist = g_argv;
#endif

      for (int i = 1; i < nArgs; ++i) // skip szArglist[0], contains executable name
      {
         bool valid_param = false;
         if ((szArglist[i][0] != '-') && (szArglist[i][0] != '/')) // ignore stuff (directories) that is passed in via frontends
            valid_param = true;
         if (!valid_param) for (size_t i2 = 0; i2 < std::size(options); ++i2)
         {
            if (compare_option(szArglist[i], (option_names)i2))
            {
               valid_param = true;
               break;
            }
         }
         if (!valid_param) for (char t = '1'; t <= '9'; ++t)
         {
            if ((lstrcmpi(szArglist[i], ("-c"s+t).c_str()) == 0) || (lstrcmpi(szArglist[i], ("/c"s+t).c_str()) == 0))
            {
               valid_param = true;
               break;
            }
         }

         //

         if (!valid_param
            || compare_option(szArglist[i], OPTION_H)
            || compare_option(szArglist[i], OPTION_HELP)
            || compare_option(szArglist[i], OPTION_QMARK))
         {
            string output = 
#ifndef __STANDALONE__
                            "-"    +options[OPTION_UNREGSERVER]+          "  "+option_descs[OPTION_UNREGSERVER]+
                            "\n-"  +options[OPTION_REGSERVER]+            "  "+option_descs[OPTION_REGSERVER]+
                            "\n\n"+
#endif
                            "-"+options[OPTION_DISABLETRUEFULLSCREEN]+    "  "+option_descs[OPTION_DISABLETRUEFULLSCREEN]+
                            "\n-"  +options[OPTION_ENABLETRUEFULLSCREEN]+ "  "+option_descs[OPTION_ENABLETRUEFULLSCREEN]+
                            "\n-"  +options[OPTION_MINIMIZED]+            "  "+option_descs[OPTION_MINIMIZED]+
                            "\n-"  +options[OPTION_EXTMINIMIZED]+         "  "+option_descs[OPTION_EXTMINIMIZED]+
                            "\n-"  +options[OPTION_PRIMARY]+              "  "+option_descs[OPTION_PRIMARY]+
                            "\n\n-"+options[OPTION_GLES]+                 "  "+option_descs[OPTION_GLES]+
                            "\n\n-"+options[OPTION_LESSCPUTHREADS]+       "  "+option_descs[OPTION_LESSCPUTHREADS]+
                            "\n\n-"+options[OPTION_EDIT]+                 " "+option_descs[OPTION_EDIT]+
                            "\n-"  +options[OPTION_PLAY]+                 " "+option_descs[OPTION_PLAY]+
                            "\n-"  +options[OPTION_POVEDIT]+              " "+option_descs[OPTION_POVEDIT]+
                            "\n-"  +options[OPTION_POV]+                  " "+option_descs[OPTION_POV]+
                            "\n-"  +options[OPTION_EXTRACTVBS]+           " "+option_descs[OPTION_EXTRACTVBS]+
                            "\n-"  +options[OPTION_INI]+                  " "+option_descs[OPTION_INI]+
#ifdef __STANDALONE__
                            "\n\n-"+options[OPTION_PREFPATH]+             " "+option_descs[OPTION_PREFPATH]+
                            "\n-"  +options[OPTION_LISTRES]+              "  "+option_descs[OPTION_LISTRES]+
                            "\n-"  +options[OPTION_LISTSND]+              "  "+option_descs[OPTION_LISTSND]+
                            "\n"+
#endif       
                            "\n-c1 [customparam] .. -c9 [customparam]  Custom user parameters that can be accessed in the script via GetCustomParam(X)";
            if (!valid_param)
                output = "Invalid Parameter "s + szArglist[i] + "\n\nValid Parameters are:\n\n" + output;
#ifndef __STANDALONE__
            ::MessageBox(NULL, output.c_str(), "Visual Pinball Usage", valid_param ? MB_OK : MB_ICONERROR);
#else
            std::cout 
                << "Visual Pinball Usage"
                << "\n\n" 
                << output
                << "\n\n";
#endif

            //run = false;
            exit(valid_param ? 0 : 1);
         }

         //

         if (compare_option(szArglist[i], OPTION_LESSCPUTHREADS))
             m_vpinball.m_logicalNumberOfProcessors = max(min(m_vpinball.m_logicalNumberOfProcessors, 2), m_vpinball.m_logicalNumberOfProcessors/4); // only use 1/4th the threads, but at least 2 (if there are 2)

         //

#ifndef __STANDALONE__
         if (compare_option(szArglist[i], OPTION_UNREGSERVER))
         {
            _Module.UpdateRegistryFromResource(IDR_VPINBALL, FALSE);
            const HRESULT ret = _Module.UnregisterServer(TRUE);
            if (ret != S_OK)
                ShowError("Unregister VP functions failed");
            run = false;
            break;
         }
         if (compare_option(szArglist[i], OPTION_REGSERVER))
         {
            _Module.UpdateRegistryFromResource(IDR_VPINBALL, TRUE);
            const HRESULT ret = _Module.RegisterServer(TRUE);
            if (ret != S_OK)
                ShowError("Register VP functions failed");
            run = false;
            break;
         }
#endif

         //

         if (compare_option(szArglist[i], OPTION_DISABLETRUEFULLSCREEN))
         {
             m_vpinball.m_disEnableTrueFullscreen = 0;
             continue;
         }
         if (compare_option(szArglist[i], OPTION_ENABLETRUEFULLSCREEN))
         {
             m_vpinball.m_disEnableTrueFullscreen = 1;
             continue;
         }

         //

         bool useCustomParams = false;
         int customIdx = 1;
         for (char t = '1'; t <= '9'; ++t)
         {
             if (lstrcmpi(szArglist[i], ("-c"s+t).c_str()) == 0 || lstrcmpi(szArglist[i], ("/c"s+t).c_str()) == 0)
             {
                 useCustomParams = true;
                 break;
             }
             customIdx++;
         }

         if (useCustomParams && (i+1<nArgs))
         {
             const size_t len = strlen(szArglist[i + 1]);
             m_vpinball.m_customParameters[customIdx - 1] = new WCHAR[len + 1];

             MultiByteToWideCharNull(CP_ACP, 0, szArglist[i + 1], -1, m_vpinball.m_customParameters[customIdx - 1], (int)len + 1);

             ++i; // two params processed

             continue;
         }

         //

         const bool minimized = compare_option(szArglist[i], OPTION_MINIMIZED);
         if (minimized)
         {
             m_vpinball.m_open_minimized = true;
             m_vpinball.m_disable_pause_menu = true;
         }

         const bool ext_minimized = compare_option(szArglist[i], OPTION_EXTMINIMIZED);
         if (ext_minimized)
             m_vpinball.m_open_minimized = true;

         const bool editfile = compare_option(szArglist[i], OPTION_EDIT);
         const bool playfile = compare_option(szArglist[i], OPTION_PLAY);

         const bool gles = compare_option(szArglist[i], OPTION_GLES);

         const bool primaryDisplay = compare_option(szArglist[i], OPTION_PRIMARY);
         if (primaryDisplay)
             m_vpinball.m_primaryDisplay = true;

         const bool povEdit = compare_option(szArglist[i], OPTION_POVEDIT);
         if (povEdit)
             m_vpinball.m_povEdit = true;

         const bool extractpov = compare_option(szArglist[i], OPTION_POV);
         const bool extractscript = compare_option(szArglist[i], OPTION_EXTRACTVBS);

         const bool ini = compare_option(szArglist[i], OPTION_INI);

#ifdef __STANDALONE__
         const bool prefPath = compare_option(szArglist[i], OPTION_PREFPATH);

         listRes = compare_option(szArglist[i], OPTION_LISTRES);
         listSnd = compare_option(szArglist[i], OPTION_LISTSND);
#endif

         // global emission scale parameter handling
         if (gles && (i + 1 < nArgs))
         {
             char *lpszStr;
             if ((szArglist[i + 1][0] == '-') || (szArglist[i + 1][0] == '/'))
                 lpszStr = szArglist[i + 1] + 1;
             else
                 lpszStr = szArglist[i + 1];

            fgles = clamp((float)atof(lpszStr), 0.115f, 0.925f);
            bgles = true;
         }

         // user specified ini handling
         if (ini && (i + 1 < nArgs))
         {
#ifndef __STANDALONE__
            // Remove leading - or /
            if ((szArglist[i + 1][0] == '-') || (szArglist[i + 1][0] == '/'))
                 szIniFileName = szArglist[i + 1] + 1;
            else
#endif
                 szIniFileName = szArglist[i + 1];

            // Remove " "
            if (szIniFileName[0] == '"')
                 szIniFileName = szIniFileName.substr(1, szIniFileName.size() - 1);

#ifndef __STANDALONE__
            // Add current path
            if (szIniFileName[1] != ':')
            {
                 char szLoadDir[MAXSTRING];
                 GetCurrentDirectory(MAXSTRING, szLoadDir);
                 szIniFileName = string(szLoadDir) + PATH_SEPARATOR_CHAR + szIniFileName;
            }
#endif

            ++i; // two params processed
         }

#ifdef __STANDALONE__
         // user specified prefPath handling
         if (prefPath && (i + 1 < nArgs))
         {
            szPrefPath = szArglist[i + 1];

            // Remove " "
            if (szPrefPath[0] == '"')
                 szPrefPath = szPrefPath.substr(1, szPrefPath.size() - 1);

            ++i; // two params processed
         }
#endif

         // table name handling
         if ((editfile || playfile || povEdit || extractpov || extractscript) && (i + 1 < nArgs))
         {
            allowLoadOnStart = false; // Don't face the user with a load dialog since the file is provided on the command line
            file = true;
            play = playfile || povEdit;
            extractPov = extractpov;
            extractScript = extractscript;

#ifndef __STANDALONE__
            // Remove leading - or /
            if ((szArglist[i + 1][0] == '-') || (szArglist[i + 1][0] == '/'))
               szTableFileName = szArglist[i + 1] + 1;
            else
#endif
               szTableFileName = szArglist[i + 1];

            // Remove " "
            if (szTableFileName[0] == '"')
               szTableFileName = szTableFileName.substr(1, szTableFileName.size()-1);

#ifndef __STANDALONE__
            // Add current path
            if (szTableFileName[1] != ':') {
               char szLoadDir[MAXSTRING];
               GetCurrentDirectory(MAXSTRING, szLoadDir);
               szTableFileName = string(szLoadDir) + PATH_SEPARATOR_CHAR + szTableFileName;
            }
            else
               // Or set from table path
               if (play) {
                  const string dir = PathFromFilename(szTableFileName);
                  SetCurrentDirectory(dir.c_str());
               }
#endif

            ++i; // two params processed

            if (extractpov || extractscript)
               break;
            else
               continue;
         }
      }

#ifndef __STANDALONE__
      free(szArglist);
#endif

#ifdef __STANDALONE__
   if (!szPrefPath.empty()) {
      if (!szPrefPath.ends_with(PATH_SEPARATOR_CHAR))
         szPrefPath += PATH_SEPARATOR_CHAR;

      if (!DirExists(szPrefPath)) {
         try {
            std::filesystem::create_directory(szPrefPath);
         }
         catch(...) {
            std::cout << "Could not create preferences path: " << szPrefPath << "\n";
            exit(1);
         }
      }

      m_vpinball.m_szMyPrefPath = szPrefPath;
   }
#endif

#ifdef __STANDALONE__
#if (defined(__APPLE__) && ((defined(TARGET_OS_IOS) && TARGET_OS_IOS) || (defined(TARGET_OS_TV) && TARGET_OS_TV)))
      copy_folder("assets", m_vpinball.m_szMyPrefPath);
#endif
#endif

      // Default ini path (can be overriden from command line)
      if (szIniFileName.empty())
         szIniFileName = m_vpinball.m_szMyPrefPath + "VPinballX.ini"s;

      m_vpinball.m_settings.LoadFromFile(szIniFileName, true);
      
#ifndef __STANDALONE__
      SetupLogger(m_vpinball.m_settings.LoadValueWithDefault(Settings::Editor, "EnableLog"s, false));
#else
      SetupLogger(m_vpinball.m_settings.LoadValueWithDefault(Settings::Editor, "EnableLog"s, true));
#endif
      PLOGI << "Starting VPX...";

#ifdef __STANDALONE__
      PLOGI << "Settings file was loaded from " << szIniFileName;
      PLOGI << "m_logicalNumberOfProcessors=" << m_vpinball.m_logicalNumberOfProcessors;
      PLOGI << "m_szMyPath=" << m_vpinball.m_szMyPath;
      PLOGI << "m_szMyPrefPath=" << m_vpinball.m_szMyPrefPath;

      if (!DirExists(PATH_USER))
         std::filesystem::create_directory(PATH_USER);

      if (listRes) {
         PLOGI << "Available fullscreen resolutions:";
         int displays = getNumberOfDisplays();
         for (int display = 0; display < displays; display++) {
            vector<VideoMode> allVideoModes;
            EnumerateDisplayModes(display, allVideoModes);
            for (size_t i = 0; i < allVideoModes.size(); ++i) {
               VideoMode mode = allVideoModes.at(i);
               PLOGI << "display " << display << ": " << mode.width << "x" << mode.height
                     << " (depth=" << mode.depth << ", refreshRate=" << mode.refreshrate << ")";
            }
         }
      }

      if (listSnd) {
         PLOGI << "Available sound devices:";
         vector<AudioDevice> allAudioDevices;
         EnumerateAudioDevices(allAudioDevices);
         for (size_t i = 0; i < allAudioDevices.size(); ++i) {
            AudioDevice audioDevice = allAudioDevices.at(i);
            PLOGI << "id " << audioDevice.id << ": name=" << audioDevice.name << ", enabled=" << audioDevice.enabled;
         }
      }

      if (listRes || listSnd)
         exit(0);

#if (defined(__APPLE__) && ((defined(TARGET_OS_IOS) && TARGET_OS_IOS) || (defined(TARGET_OS_TV) && TARGET_OS_TV))) || defined(__ANDROID__)
      const string launchTable = g_pvp->m_settings.LoadValueWithDefault(Settings::Standalone, "LaunchTable"s, "res/exampleTable.vpx"s);
      szTableFileName = m_vpinball.m_szMyPrefPath + launchTable;
      file = true;
      play = true;
#endif

#endif

      // Start VP with file dialog open and then also playing that one?
      const bool stos = allowLoadOnStart && m_vpinball.m_settings.LoadValueWithDefault(Settings::Editor, "SelectTableOnStart"s, true);
      if (stos)
      {
         file = true;
         play = true;
      }

#ifndef __STANDALONE__
      // load and register VP type library for COM integration
      char szFileName[MAXSTRING];
      if (GetModuleFileName(m_vpinball.theInstance, szFileName, MAXSTRING))
      {
         ITypeLib *ptl = nullptr;
         MAKE_WIDEPTR_FROMANSI(wszFileName, szFileName);
         if (SUCCEEDED(LoadTypeLib(wszFileName, &ptl)))
         {
            // first try to register system-wide (if running as admin)
            HRESULT hr = RegisterTypeLib(ptl, wszFileName, nullptr);
            if (!SUCCEEDED(hr))
            {
               // if failed, register only for current user
               hr = RegisterTypeLibForUser(ptl, wszFileName, nullptr);
               if (!SUCCEEDED(hr))
                  m_vpinball.MessageBox("Could not register type library. Try running Visual Pinball as administrator.", "Error", MB_ICONERROR);
            }
            ptl->Release();
         }
         else
            m_vpinball.MessageBox("Could not load type library.", "Error", MB_ICONERROR);
      }
#endif

      InitVPX();
      //SET_CRT_DEBUG_FIELD( _CRTDBG_LEAK_CHECK_DF );
      return TRUE;
   }

   void InitVPX()
   {
#ifndef __STANDALONE__
#if _WIN32_WINNT >= 0x0400 & defined(_ATL_FREE_THREADED)
       const HRESULT hRes = _Module.RegisterClassObjects(CLSCTX_LOCAL_SERVER,
           REGCLS_MULTIPLEUSE | REGCLS_SUSPENDED);
       _ASSERTE(SUCCEEDED(hRes));
       hRes = CoResumeClassObjects();
#else
       const HRESULT hRes = _Module.RegisterClassObjects(CLSCTX_LOCAL_SERVER,
           REGCLS_MULTIPLEUSE);
#endif
       _ASSERTE(SUCCEEDED(hRes));

       INITCOMMONCONTROLSEX iccex;
       iccex.dwSize = sizeof(INITCOMMONCONTROLSEX);
       iccex.dwICC = ICC_COOL_CLASSES;
       InitCommonControlsEx(&iccex);
#endif

       {
           EditableRegistry::RegisterEditable<Bumper>();
           EditableRegistry::RegisterEditable<Decal>();
           EditableRegistry::RegisterEditable<DispReel>();
           EditableRegistry::RegisterEditable<Flasher>();
           EditableRegistry::RegisterEditable<Flipper>();
           EditableRegistry::RegisterEditable<Gate>();
           EditableRegistry::RegisterEditable<Kicker>();
           EditableRegistry::RegisterEditable<Light>();
           EditableRegistry::RegisterEditable<LightSeq>();
           EditableRegistry::RegisterEditable<Plunger>();
           EditableRegistry::RegisterEditable<Primitive>();
           EditableRegistry::RegisterEditable<Ramp>();
           EditableRegistry::RegisterEditable<Rubber>();
           EditableRegistry::RegisterEditable<Spinner>();
           EditableRegistry::RegisterEditable<Surface>();
           EditableRegistry::RegisterEditable<Textbox>();
           EditableRegistry::RegisterEditable<Timer>();
           EditableRegistry::RegisterEditable<Trigger>();
           EditableRegistry::RegisterEditable<HitTarget>();
       }

       m_vpinball.AddRef();
       m_vpinball.Create(nullptr);
       m_vpinball.m_bgles = bgles;
       m_vpinball.m_fgles = fgles;

#ifndef __STANDALONE__
       g_haccel = LoadAccelerators(m_vpinball.theInstance, MAKEINTRESOURCE(IDR_VPACCEL));
#endif

       if (file)
       {
           if (!szTableFileName.empty())
           {
               PLOGI << "Loading table from command line option: " << szTableFileName;
               m_vpinball.LoadFileName(szTableFileName, !play);
               m_vpinball.m_table_played_via_command_line = play;
               loadFileResult = m_vpinball.m_ptableActive != nullptr;
               if (!loadFileResult && m_vpinball.m_open_minimized)
                  m_vpinball.QuitPlayer(Player::CloseState::CS_CLOSE_APP);
           }
           else
           {
               loadFileResult = m_vpinball.LoadFile(!play);
               m_vpinball.m_table_played_via_SelectTableOnStart = m_vpinball.m_settings.LoadValueWithDefault(Settings::Editor, "SelectTableOnPlayerClose"s, true) ? loadFileResult : false;
           }

           if (extractScript && loadFileResult)
           {
               string szScriptFilename = szTableFileName;
               if(ReplaceExtensionFromFilename(szScriptFilename, "vbs"s))
                   m_vpinball.m_ptableActive->m_pcv->SaveToFile(szScriptFilename);
#ifndef __STANDALONE__
               m_vpinball.QuitPlayer(Player::CloseState::CS_CLOSE_APP);
#else
               run = false;
#endif
           }
           if (extractPov && loadFileResult)
           {
               string szPOVFilename = szTableFileName;
               if (ReplaceExtensionFromFilename(szPOVFilename, "pov"s))
                   m_vpinball.m_ptableActive->ExportBackdropPOV(szPOVFilename);
#ifndef __STANDALONE__
               m_vpinball.QuitPlayer(Player::CloseState::CS_CLOSE_APP);
#else
               run = false;
#endif
           }
       }
   }

   int Run() override
   {
      if (run)
      {
         if (play && loadFileResult)
           m_vpinball.DoPlay(m_vpinball.m_povEdit);

         // VBA APC handles message loop (bastards)
         m_vpinball.MainMsgLoop();

         m_vpinball.m_settings.Save();

         m_vpinball.Release();

#ifndef __STANDALONE__
         DestroyAcceleratorTable(g_haccel);

         _Module.RevokeClassObjects();
#endif
         Sleep(THREADS_PAUSE); //wait for any threads to finish
      }
      return 0;
   }
};

#if defined(ENABLE_SDL)
// The OpenGL implementation will fail on NVIDIA drivers when Threaded Optimization is enabled so we disable it for this app
// Note: There are quite a lot of applications doing this, but I think this may hide an incorrect OpenGL call somewhere
// that the threaded optimization of NVIDIA drivers ends up to crash. This would be good to find the root cause, if any.

#ifndef __STANDALONE__
#include "nvapi/nvapi.h"
#include "nvapi/NvApiDriverSettings.h"
#include "nvapi/NvApiDriverSettings.c"

enum NvThreadOptimization
{
   NV_THREAD_OPTIMIZATION_AUTO = 0,
   NV_THREAD_OPTIMIZATION_ENABLE = 1,
   NV_THREAD_OPTIMIZATION_DISABLE = 2,
   NV_THREAD_OPTIMIZATION_NO_SUPPORT = 3
};

static bool NvAPI_OK_Verify(NvAPI_Status status)
{
   if (status == NVAPI_OK)
      return true;

   NvAPI_ShortString szDesc = { 0 };
   NvAPI_GetErrorMessage(status, szDesc);

   PLOGI << "NVAPI error: " << szDesc;

   return false;
}

static NvThreadOptimization GetNVIDIAThreadOptimization()
{
   NvThreadOptimization threadOptimization = NV_THREAD_OPTIMIZATION_NO_SUPPORT;

   NvAPI_Status status;
   status = NvAPI_Initialize();
   if (!NvAPI_OK_Verify(status))
      return threadOptimization;

   NvDRSSessionHandle hSession;
   status = NvAPI_DRS_CreateSession(&hSession);
   if (!NvAPI_OK_Verify(status))
      return threadOptimization;

   status = NvAPI_DRS_LoadSettings(hSession);
   if (!NvAPI_OK_Verify(status))
   {
      NvAPI_DRS_DestroySession(hSession);
      return threadOptimization;
   }

   NvDRSProfileHandle hProfile;
   status = NvAPI_DRS_GetBaseProfile(hSession, &hProfile);
   if (!NvAPI_OK_Verify(status))
   {
      NvAPI_DRS_DestroySession(hSession);
      return threadOptimization;
   }

   NVDRS_SETTING originalSetting = {};
   originalSetting.version = NVDRS_SETTING_VER;
   status = NvAPI_DRS_GetSetting(hSession, hProfile, OGL_THREAD_CONTROL_ID, &originalSetting);
   if (NvAPI_OK_Verify(status))
   {
      threadOptimization = (NvThreadOptimization)originalSetting.u32CurrentValue;
   }

   NvAPI_DRS_DestroySession(hSession);

   return threadOptimization;
}

static void SetNVIDIAThreadOptimization(NvThreadOptimization threadedOptimization)
{
   if (threadedOptimization == NV_THREAD_OPTIMIZATION_NO_SUPPORT)
      return;

   NvAPI_Status status;
   status = NvAPI_Initialize();
   if (!NvAPI_OK_Verify(status))
      return;

   NvDRSSessionHandle hSession;
   status = NvAPI_DRS_CreateSession(&hSession);
   if (!NvAPI_OK_Verify(status))
      return;

   status = NvAPI_DRS_LoadSettings(hSession);
   if (!NvAPI_OK_Verify(status))
   {
      NvAPI_DRS_DestroySession(hSession);
      return;
   }

   NvDRSProfileHandle hProfile;
   status = NvAPI_DRS_GetBaseProfile(hSession, &hProfile);
   if (!NvAPI_OK_Verify(status))
   {
      NvAPI_DRS_DestroySession(hSession);
      return;
   }

   NVDRS_SETTING setting = {};
   setting.version = NVDRS_SETTING_VER;
   setting.settingId = OGL_THREAD_CONTROL_ID;
   setting.settingType = NVDRS_DWORD_TYPE;
   setting.u32CurrentValue = (EValues_OGL_THREAD_CONTROL)threadedOptimization;

   status = NvAPI_DRS_SetSetting(hSession, hProfile, &setting);
   if (!NvAPI_OK_Verify(status))
   {
      NvAPI_DRS_DestroySession(hSession);
      return;
   }

   status = NvAPI_DRS_SaveSettings(hSession);
   NvAPI_OK_Verify(status);

   NvAPI_DRS_DestroySession(hSession);
}
#endif
#endif

class DebugAppender : public plog::IAppender
{
public:
   virtual void write(const plog::Record &record) PLOG_OVERRIDE
   {
      if (g_pvp == nullptr || g_pplayer == nullptr)
         return;
      auto table = g_pvp->GetActiveTable();
      if (table == nullptr)
         return;
      #ifdef _WIN32
      // Convert from wchar* to char* on Win32
      auto msg = record.getMessage();
      const int len = (int)lstrlenW(msg);
      char *const szT = new char[len + 1];
      WideCharToMultiByteNull(CP_ACP, 0, msg, -1, szT, len + 1, nullptr, nullptr);
      table->m_pcv->AddToDebugOutput(szT);
      delete [] szT;
      #else
      table->m_pcv->AddToDebugOutput(record.getMessage());
      #endif
   }
};

void SetupLogger(const bool enable)
{
   plog::Severity maxLogSeverity = plog::none;
   if (enable)
   {
      static bool initialized = false;
      if (!initialized)
      {
         initialized = true;
         string szLogPath = g_pvp->m_szMyPrefPath + "vpinball.log";
         static plog::RollingFileAppender<plog::TxtFormatter> fileAppender(szLogPath.c_str(), 1024 * 1024 * 5, 1);
         static DebugAppender debugAppender;
         plog::Logger<PLOG_DEFAULT_INSTANCE_ID>::getInstance()->addAppender(&debugAppender);
         plog::Logger<PLOG_DEFAULT_INSTANCE_ID>::getInstance()->addAppender(&fileAppender);
         plog::Logger<PLOG_NO_DBG_OUT_INSTANCE_ID>::getInstance()->addAppender(&fileAppender);

#ifdef __STANDALONE__
#ifndef __ANDROID__
         static plog::ColorConsoleAppender<plog::TxtFormatter> consoleAppender;
         plog::Logger<PLOG_DEFAULT_INSTANCE_ID>::getInstance()->addAppender(&consoleAppender);
         plog::Logger<PLOG_NO_DBG_OUT_INSTANCE_ID>::getInstance()->addAppender(&consoleAppender);
#else
         static plog::AndroidAppender<plog::TxtFormatter> androidAppender("vpinball");
         plog::Logger<PLOG_DEFAULT_INSTANCE_ID>::getInstance()->addAppender(&androidAppender);
         plog::Logger<PLOG_NO_DBG_OUT_INSTANCE_ID>::getInstance()->addAppender(&androidAppender);
#endif
#endif
      }
      #ifdef _DEBUG
      maxLogSeverity = plog::debug;
      #else
      maxLogSeverity = plog::info;
      #endif
   }
   plog::Logger<PLOG_DEFAULT_INSTANCE_ID>::getInstance()->setMaxSeverity(maxLogSeverity);
   plog::Logger<PLOG_NO_DBG_OUT_INSTANCE_ID>::getInstance()->setMaxSeverity(maxLogSeverity);
}

extern "C" int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR /*lpCmdLine*/, int /*nShowCmd*/)
{
#ifndef __STANDALONE__
   #if defined(ENABLE_SDL)
   static NvThreadOptimization s_OriginalNVidiaThreadOptimization = NV_THREAD_OPTIMIZATION_NO_SUPPORT;
   #endif
#endif

   int retval;
   try
   {
#ifndef __STANDALONE__
      #if defined(ENABLE_SDL)
      s_OriginalNVidiaThreadOptimization = GetNVIDIAThreadOptimization();
      if (s_OriginalNVidiaThreadOptimization != NV_THREAD_OPTIMIZATION_NO_SUPPORT && s_OriginalNVidiaThreadOptimization != NV_THREAD_OPTIMIZATION_DISABLE)
      {
         PLOGI << "Disabling NVIDIA Threaded Optimization";
         SetNVIDIAThreadOptimization(NV_THREAD_OPTIMIZATION_DISABLE);
      }
      #endif
#endif

      #if defined(ENABLE_SDL) || defined(ENABLE_SDL_INPUT)
      SDL_Init(0
         #ifdef ENABLE_SDL
            | SDL_INIT_VIDEO
         #endif
         #ifdef ENABLE_SDL_INPUT
            | SDL_INIT_JOYSTICK
         #endif
#ifdef __STANDALONE__
            | SDL_INIT_TIMER
#endif
      );
      #endif

      plog::init<PLOG_DEFAULT_INSTANCE_ID>();
      plog::init<PLOG_NO_DBG_OUT_INSTANCE_ID>(); // Logger that do not show in the debug window to avoid duplicated messages

      // Start Win32++
      VPApp theApp(hInstance);
      theApp.InitInstance();
      // Run the application
      retval = theApp.Run();
   }

   // catch all CException types
   catch (const CException &e)
   {
      // Display the exception and quit
      MessageBox(nullptr, e.GetText(), AtoT(e.what()), MB_ICONERROR);

      retval = -1;
   }
   #if defined(ENABLE_SDL) || defined(ENABLE_SDL_INPUT)
   SDL_Quit();
   #endif

#ifndef __STANDALONE__
   #if defined(ENABLE_SDL)
   if (s_OriginalNVidiaThreadOptimization != NV_THREAD_OPTIMIZATION_NO_SUPPORT && s_OriginalNVidiaThreadOptimization != NV_THREAD_OPTIMIZATION_DISABLE)
   {
      PLOGI << "Restoring NVIDIA Threaded Optimization";
      SetNVIDIAThreadOptimization(s_OriginalNVidiaThreadOptimization);
   };
   #endif
#endif

   PLOGI << "Closing VPX...";
#ifdef __STANDALONE__
#if (defined(__APPLE__) && ((defined(TARGET_OS_IOS) && TARGET_OS_IOS) || (defined(TARGET_OS_TV) && TARGET_OS_TV))) || defined(__ANDROID__)
   exit(retval);
#endif
#endif
   return retval;
}

#ifdef __STANDALONE__
int main(int argc, char** argv) {
   g_argc = argc;
   g_argv = argv;

   return WinMain(nullptr, nullptr, (LPTSTR)"", 0);
}
#endif
