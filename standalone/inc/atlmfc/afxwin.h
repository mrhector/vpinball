
// 560
class CDC { };

// 1135 
class CMenu { };

// 2181
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

// 3323
class CDialog : public CWnd {
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

// 3431
class CStatic : public CWnd { };

// 3462
class CButton : public CWnd { };

// 3718
class CComboBox : public CWnd { };

// 3821
class CEdit : public CWnd { };

// 5135
class CWinApp {
public:
   virtual BOOL InitInstance() = 0;
   virtual int Run() = 0;
};


