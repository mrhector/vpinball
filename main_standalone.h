#ifndef __MAIN_STANDALONE_H__
#define __MAIN_STANDALONE_H__

#if defined(__clang__)
#pragma clang diagnostic ignored "-Winconsistent-missing-override"
#pragma clang diagnostic ignored "-Wmissing-exception-spec"
#endif

#define ONLY_USE_BASS

extern "C" {
#include <atlbase.h>
}

#include <oleauto.h>
#include <winbase.h>
#include <wincrypt.h>
#include <dinput.h>

#undef strncpy
#include <wchar.h>

#define PATH_SEPARATOR_CHAR '/'
#define PATH_SEPARATOR_WCHAR L'/'

#ifdef _MSC_VER
#define PATH_TABLES  ("c:"s + PATH_SEPARATOR_CHAR + "Visual Pinball" + PATH_SEPARATOR_CHAR + "tables"  + PATH_SEPARATOR_CHAR)
#define PATH_SCRIPTS ("c:"s + PATH_SEPARATOR_CHAR + "Visual Pinball" + PATH_SEPARATOR_CHAR + "scripts" + PATH_SEPARATOR_CHAR)
#define PATH_MUSIC   ("c:"s + PATH_SEPARATOR_CHAR + "Visual Pinball" + PATH_SEPARATOR_CHAR + "music"   + PATH_SEPARATOR_CHAR)
#define PATH_USER    ("c:"s + PATH_SEPARATOR_CHAR + "Visual Pinball" + PATH_SEPARATOR_CHAR + "user"    + PATH_SEPARATOR_CHAR)
#else
#define PATH_TABLES  (string(getenv("HOME")) + PATH_SEPARATOR_CHAR + ".vpinball" + PATH_SEPARATOR_CHAR + "tables"  + PATH_SEPARATOR_CHAR)
#define PATH_SCRIPTS (string(getenv("HOME")) + PATH_SEPARATOR_CHAR + ".vpinball" + PATH_SEPARATOR_CHAR + "scripts" + PATH_SEPARATOR_CHAR)
#define PATH_MUSIC   (string(getenv("HOME")) + PATH_SEPARATOR_CHAR + ".vpinball" + PATH_SEPARATOR_CHAR + "music"   + PATH_SEPARATOR_CHAR)
#define PATH_USER    (string(getenv("HOME")) + PATH_SEPARATOR_CHAR + ".vpinball" + PATH_SEPARATOR_CHAR + "user"    + PATH_SEPARATOR_CHAR)
#endif

#if defined(_M_IX86) || defined(_M_X64) || defined(__i386__) || defined(__i386) || defined(__i486__) || defined(__i486) || defined(i386) || defined(__ia64__) || defined(__x86_64__)
 #include <xmmintrin.h>
#endif

#include <vector>
#include <string>
#include <algorithm>
#include <commdlg.h>

using std::string;
using std::wstring;
using std::vector;

#define fopen_s(pFile, filename, mode) ((*(pFile)) = fopen((filename), (mode))) == NULL
#define fprintf_s fprintf
#define fread_s(buffer, bufferSize, elementSize, count, stream) fread(buffer, bufferSize, count, stream)
#define fscanf_s fscanf

#define sscanf_s sscanf

#define localtime_s(x, y) localtime_r(y, x)
#define gmtime_s(x, y) gmtime_r(y, x)

#define _aligned_malloc(size, align) aligned_alloc(align, size)
#define _aligned_free free

#undef lstrlen
#define lstrlen lstrlenA

#undef lstrcmpi
#define lstrcmpi lstrcmpiA

#undef lstrcmp
#define lstrcmp lstrcmpA

#define strcpy_s(A, B, C) strncpy(A, C, B)
#define strnlen_s strnlen
#define sprintf_s snprintf
#define _snprintf_s snprintf
#define swprintf_s swprintf
#define StrStrI strcasestr

#define STRNCPY_S3(a, b, c) strncpy(a, b, c)
#define STRNCPY_S4(a, b, c, d) strncpy(a, c, d)
#define GET_MACRO(_1, _2, _3, _4, NAME, ...) NAME
#define strncpy_s(...) \
  GET_MACRO(__VA_ARGS__, STRNCPY_S4, STRNCPY_S3)(__VA_ARGS__)

#undef GetCurrentDirectory
#define GetCurrentDirectory GetCurrentDirectoryA

#undef GetModuleFileName
#define GetModuleFileName GetModuleFileNameA

#undef MessageBox
#define MessageBox MessageBoxA

#undef OutputDebugString
#define OutputDebugString OutputDebugStringA

#undef CharLowerBuff
#define CharLowerBuff CharLowerBuffA

#define FINDREPLACE FINDREPLACEA
#define CREATESTRUCT CREATESTRUCTA
#define WNDCLASS WNDCLASSA
#define LOGFONT LOGFONTA
#define MONITORINFOEX MONITORINFOEXA

#define LPDIRECTSOUND void*
#define LPDIRECTSOUNDBUFFER void*
#define LPDIRECTSOUND3DBUFFER void*
#define LPDIRECTSOUND3DLISTENER void*

#define _T(x) x
#define AtoT(x) x
#define CString string

#define DSBPLAY_LOOPING 0x00000001
#define DSBVOLUME_MIN -10000

typedef LPSTR LPTSTR;
typedef LPCSTR LPCTSTR;
typedef DWORD D3DCOLOR;
typedef float D3DVALUE;

#define _ASSERTE(expr) ((void)0)

#include "standalone/inc/atl/atldef.h"
#include "standalone/inc/atl/atlbase.h"
#include "standalone/inc/atl/atlcom.h"
#include "standalone/inc/atl/atlcomcli.h"
#include "standalone/inc/atl/atlsafe.h"

#define PLOG_OMIT_LOG_DEFINES
#define PLOG_NO_DBG_OUT_INSTANCE_ID 1
#include <plog/Log.h>

#include "helpers.h"

#include "def.h"

class IProcessDebugManager {};
  
class CException : public std::exception {
 public:
  LPCTSTR GetText() const { return ""; };
};

class CWinApp {
    virtual BOOL InitInstance() = 0;
    virtual int Run() = 0;
};

class CDC {
};

class CWnd {
 public:
  virtual void PreRegisterClass(WNDCLASS& wc){};
  virtual void PreCreate(CREATESTRUCT& cs) {}
  virtual int OnCreate(CREATESTRUCT& cs) { return 0; };
  virtual void OnClose(){};
  virtual LRESULT OnMDIActivate(UINT msg, WPARAM wparam, LPARAM lparam) { return 0; };
  virtual BOOL OnEraseBkgnd(CDC& dc) { return false; };
  virtual void OnInitialUpdate(){};
  virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam) { return 0; };
  virtual BOOL OnCommand(WPARAM wparam, LPARAM lparam) { return false; };
  virtual LRESULT OnNotify(WPARAM wparam, LPARAM lparam) { return 0; };
  virtual void Destroy() { };

  BOOL Create(CWnd* pParentWnd) { return true; }
  int MessageBox(LPCTSTR lpText, LPCTSTR lpCaption, UINT uType);
  HWND GetHwnd() { return 0L; }
};

class CMenu {};

class CDialog {
 public:
  CDialog(int id){};

 protected:
  virtual BOOL OnInitDialog() { return true; };
  virtual void OnOK(){};
  virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam) { return false; };
  virtual void OnClose(){};
  virtual INT_PTR DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam) {return 0; };
  virtual LRESULT OnNotify(WPARAM wparam, LPARAM lparam) { return 0; };
};

class CDocker : public CWnd {};
class CMDIDockFrame : public CDocker {};
class CMDIChild : public CWnd {};
class CDockProperty {};
class CDockNotes {};
class CDockLayers {};
class CDockToolbar {};
class CComboBox {};
class CEdit {};
class CButton {};
class CColorDialog {};
class CResizer {};
class CRect {};
class CProgressBar {};
class CStatic {};

class SearchSelectDialog {};
class LayersListDialog {};
class ImageDialog {};
class SoundDialog {};
class AudioOptionsDialog {};
class VideoOptionsDialog {};
class VROptionsDialog {};
class EditorOptionsDialog {};
class CollectionManagerDialog {};
class PhysicsOptionsDialog {};
class TableInfoDialog {};
class DimensionDialog {};
class RenderProbeDialog {};
class MaterialDialog {};
class AboutDialog {};
class ToolbarDialog {};
class PropertyDialog {};
class NotesDialog {};

class ColorButton {};

class SCNotification {};

inline int GetWinVersion() { return 2601; }

#include "math/math.h"
#include "math/vector.h"
#include "math/matrix.h"
#include "math/bbox.h"

#include "dispid.h"

#include "variant.h"
#include "vector.h"
#include "vectorsort.h"
#include "vpinball_standalone_i.h"
#include "regutil.h"

#include "idebug.h"

#include "eventproxy.h"

#include "audioplayer.h"
#include "media/fileio.h"
#include "pinundo.h"
#include "iselect.h"

#include "ieditable.h"
#include "codeview.h"

#include "media/lzwreader.h"
#include "media/lzwwriter.h"

#include "pinsound.h"
#include "pinbinary.h"

#include "extern.h"

#include "vpinball_h.h"
#include "pintable.h"

#include "mesh.h"
#include "pin/collide.h"
#include "pin3d.h"

#include "sur.h"
#include "paintsur.h"
#include "hitsur.h"
#include "hitrectsur.h"

#include "ballex.h"

#include "pin/collideex.h"
#include "pin/ball.h"
#include "pin/hittimer.h"
#include "pin/hitable.h"
#include "pin/hitflipper.h"
#include "pin/hitplunger.h"
#include "pin/player.h"

#include "color.h"

#include "dragpoint.h"
#include "timer.h"
#include "flipper.h"
#include "plunger.h"
#include "textbox.h"
#include "surface.h"
#include "dispreel.h"
#include "lightseq.h"
#include "bumper.h"
#include "trigger.h"
#include "light.h"
#include "kicker.h"
#include "decal.h"
#include "primitive.h"
#include "hittarget.h"
#include "gate.h"
#include "spinner.h"
#include "ramp.h"
#include "flasher.h"
#include "rubber.h"
#include "mixer.h"
#include "hid.h"
#include "plumb.h"

#include "kdtree.h"

#include "wintimer.h"

#include "trace.h"

inline void ShowError(const char* const sz) { 
  PLOGI.printf("%s", sz);
}

inline void ShowError(const string& sz) { 
  PLOGI.printf("%s", sz.c_str());
}

#include "editablereg.h"
#include "typedefs3D.h"

__forceinline float getBGxmult() { return g_pplayer->m_AAfactor; }

__forceinline float getBGymult() { return g_pplayer->m_AAfactor; }

#include "standalone/inc/system_drawing_common/Bitmap.h"
#include "standalone/inc/vpinmame/VPinMAMEController.h"
#include "standalone/inc/wmp/WMPCore.h"
#include "standalone/inc/flexdmd/FlexDMD.h"
#include "standalone/inc/pup/PinUpPlayerPinDisplay.h"

const char* glToString(GLuint val);

static CMDIChild* g_pActiveMDIChild = nullptr;

inline CMDIChild* GetActiveMDIChild() { return g_pActiveMDIChild; }

inline void AddMDIChild(CMDIChild* pChild) { g_pActiveMDIChild = pChild; }

extern "C" HRESULT vpinball_create_object(const WCHAR *progid, IClassFactory* cf, IUnknown* obj);

extern "C" void vpinball_log_debug(const char* log_buffer);
extern "C" void vpinball_log_info(const char* log_buffer);
#endif
