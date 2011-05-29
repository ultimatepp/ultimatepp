#ifdef PLATFORM_X11
#include "X11/keysym.h"
#endif

#define GUIPLATFORM_KEYCODES \
K_BACK       = 8,\
K_BACKSPACE  = 8,\
\
K_TAB        = 9,\
\
K_RETURN     = 13,\
K_ENTER      = 13,\
K_ESCAPE     = 27,\
\
K_SPACE      = 32,\
\
K_DELETE     = XK_Delete + K_DELTA,\
\
K_SHIFT_KEY  = XK_Shift_L + K_DELTA,\
K_CTRL_KEY   = XK_Control_L + K_DELTA,\
K_ALT_KEY    = XK_Meta_L + K_DELTA,\
K_CAPSLOCK   = XK_Caps_Lock + K_DELTA,\
K_PRIOR      = XK_Page_Up + K_DELTA,\
K_PAGEUP     = XK_Page_Up + K_DELTA,\
K_NEXT       = XK_Page_Down + K_DELTA,\
K_PAGEDOWN   = XK_Page_Down + K_DELTA,\
K_END        = XK_End + K_DELTA,\
K_HOME       = XK_Home + K_DELTA,\
K_LEFT       = XK_Left + K_DELTA,\
K_UP         = XK_Up + K_DELTA,\
K_RIGHT      = XK_Right + K_DELTA,\
K_DOWN       = XK_Down + K_DELTA,\
K_INSERT     = XK_Insert + K_DELTA,\
\
K_NUMPAD0    = XK_KP_0 + K_DELTA,\
K_NUMPAD1    = XK_KP_1 + K_DELTA,\
K_NUMPAD2    = XK_KP_2 + K_DELTA,\
K_NUMPAD3    = XK_KP_3 + K_DELTA,\
K_NUMPAD4    = XK_KP_4 + K_DELTA,\
K_NUMPAD5    = XK_KP_5 + K_DELTA,\
K_NUMPAD6    = XK_KP_6 + K_DELTA,\
K_NUMPAD7    = XK_KP_7 + K_DELTA,\
K_NUMPAD8    = XK_KP_8 + K_DELTA,\
K_NUMPAD9    = XK_KP_9 + K_DELTA,\
K_MULTIPLY   = XK_KP_Multiply + K_DELTA,\
K_ADD        = XK_KP_Add + K_DELTA,\
K_SEPARATOR  = XK_KP_Separator + K_DELTA,\
K_SUBTRACT   = XK_KP_Subtract + K_DELTA,\
K_DECIMAL    = XK_KP_Decimal + K_DELTA,\
K_DIVIDE     = XK_KP_Divide + K_DELTA,\
\
K_SCROLL     = XK_Scroll_Lock + K_DELTA,\
\
K_F1         = XK_F1 + K_DELTA,\
K_F2         = XK_F2 + K_DELTA,\
K_F3         = XK_F3 + K_DELTA,\
K_F4         = XK_F4 + K_DELTA,\
K_F5         = XK_F5 + K_DELTA,\
K_F6         = XK_F6 + K_DELTA,\
K_F7         = XK_F7 + K_DELTA,\
K_F8         = XK_F8 + K_DELTA,\
K_F9         = XK_F9 + K_DELTA,\
K_F10        = XK_F10 + K_DELTA,\
K_F11        = XK_F11 + K_DELTA,\
K_F12        = XK_F12 + K_DELTA,\
\
K_A          = 'A' + K_DELTA,\
K_B          = 'B' + K_DELTA,\
K_C          = 'C' + K_DELTA,\
K_D          = 'D' + K_DELTA,\
K_E          = 'E' + K_DELTA,\
K_F          = 'F' + K_DELTA,\
K_G          = 'G' + K_DELTA,\
K_H          = 'H' + K_DELTA,\
K_I          = 'I' + K_DELTA,\
K_J          = 'J' + K_DELTA,\
K_K          = 'K' + K_DELTA,\
K_L          = 'L' + K_DELTA,\
K_M          = 'M' + K_DELTA,\
K_N          = 'N' + K_DELTA,\
K_O          = 'O' + K_DELTA,\
K_P          = 'P' + K_DELTA,\
K_Q          = 'Q' + K_DELTA,\
K_R          = 'R' + K_DELTA,\
K_S          = 'S' + K_DELTA,\
K_T          = 'T' + K_DELTA,\
K_U          = 'U' + K_DELTA,\
K_V          = 'V' + K_DELTA,\
K_W          = 'W' + K_DELTA,\
K_X          = 'X' + K_DELTA,\
K_Y          = 'Y' + K_DELTA,\
K_Z          = 'Z' + K_DELTA,\
K_0          = '0' + 128 + K_DELTA,\
K_1          = '1' + 128 + K_DELTA,\
K_2          = '2' + 128 + K_DELTA,\
K_3          = '3' + 128 + K_DELTA,\
K_4          = '4' + 128 + K_DELTA,\
K_5          = '5' + 128 + K_DELTA,\
K_6          = '6' + 128 + K_DELTA,\
K_7          = '7' + 128 + K_DELTA,\
K_8          = '8' + 128 + K_DELTA,\
K_9          = '9' + 128 + K_DELTA,\
\
K_CTRL_LBRACKET  = K_CTRL|XK_bracketleft|K_DELTA,\
K_CTRL_RBRACKET  = K_CTRL|XK_bracketright|K_DELTA,\
K_CTRL_MINUS     = K_CTRL|0x2d|K_DELTA,\
K_CTRL_GRAVE     = K_CTRL|XK_grave|K_DELTA,\
K_CTRL_SLASH     = K_CTRL|0x5f|K_DELTA,\
K_CTRL_BACKSLASH = K_CTRL|0x5c|K_DELTA,\
K_CTRL_COMMA     = K_CTRL|0x2c|K_DELTA,\
K_CTRL_PERIOD    = K_CTRL|XK_period|K_DELTA,\
K_CTRL_SEMICOLON = K_CTRL|XK_semicolon|K_DELTA,\
K_CTRL_EQUAL     = K_CTRL|0x3d|K_DELTA,\
K_CTRL_APOSTROPHE= K_CTRL|0x27|K_DELTA,\
\
K_BREAK      = XK_Pause|K_DELTA,\


#define GUIPLATFORM_CTRL_TOP_DECLS \
		Window         window;


#define GUIPLATFORM_CTRL_DECLS \
	bool         ignoretakefocus:1; \
protected: \
	struct XWindow { \
		Ptr<Ctrl>    ctrl; \
		bool         exposed; \
		Vector<Rect> invalid; \
		Ptr<Ctrl>    owner; \
		Ptr<Ctrl>    last_active; \
		XIC          xic; \
	}; \
 \
private: \
	static ArrayMap<Window, XWindow>& Xwindow(); \
	static int       WndCaretTime; \
	static bool      WndCaretVisible; \
	static int       Xbuttons; \
	static int       Xbuttontime; \
	static Point     Xbuttonpos; \
	static Window    grabWindow, focusWindow; \
	static Point     mousePos; \
	static int       PopupGrab; \
	static Ptr<Ctrl> popupWnd; \
	static Index<String> sel_formats; \
	static Ptr<Ctrl>     sel_ctrl; \
	static void     ProcessEvent(XEvent *event); \
	static void     TimerAndPaint(); \
	static void     ProcessEvent(XEvent& event); \
	       void     Invalidate(XWindow& xw, const Rect& r); \
	static void     AnimateCaret(); \
	       void     DoPaint(const Vector<Rect>& invalid); \
	       void     SetLastActive(XWindow *w, Ctrl *la); \
	       XWindow *GetXWindow(); \
	static void     SyncMousePos(); \
	static void     ReleaseGrab(); \
	static Vector<Callback> hotkey; \
	static Vector<dword> modhot; \
	static Vector<dword> keyhot; \
 \
	       void  StartPopupGrab(); \
	static void  EndPopupGrab(); \
	static void  SyncIMPosition(); \
 \
	friend bool  GetMouseRight(); \
	friend bool  GetMouseLeft(); \
	friend bool  GetMouseMiddle(); \
	friend Point GetMousePos(); \
 \
protected: \
	       void   Create(Ctrl *owner, bool redirect, bool savebits); \
	       void   Create0(Ctrl *owner, bool redirect, bool savebits); \
	       void   SyncExpose(); \
	       void   TakeFocus(); \
	static Window GetXServerFocusWindow(); \
	       void   AddGlobalRepaint(); \
	static void   KillFocus(Window w); \
	static void   FocusSync(); \
 \
	       void DropEvent(XWindow& w, XEvent *event); \
	static void DropStatusEvent(XEvent *event); \
	static Index<String> drop_formats; \
	static String  Unicode(const WString& w); \
	static WString Unicode(const String& s); \
	static bool   ClipHas(int type, const char *fmt); \
	static String ClipGet(int type, const char *fmt); \
 \
	       XWindow *AddXWindow(Window &w); \
	       void RemoveXWindow(Window &w); \
	       XWindow *XWindowFromWindow(Window &w); \
 \
public: \
	struct Xclipboard { \
		Window win; \
 \
		VectorMap<int, ClipData> data; \
 \
		String Read(int fmt, int selection, int property); \
		void   Write(int fmt, const ClipData& data); \
		bool   IsAvailable(int fmt, const char *type); \
 \
		void   Clear()                     { data.Clear(); } \
		void   Request(XSelectionRequestEvent *se); \
 \
		Xclipboard(); \
		~Xclipboard(); \
	}; \
 \
	static Xclipboard& xclipboard(); \
 \
	static int  Xeventtime; \
 \
	static XIM  xim; \
 \
	void DnD(Window src, bool paste); \
 \
	virtual void    EventProc(XWindow& w, XEvent *event); \
	virtual bool    HookProc(XEvent *event); \
	Window  GetWindow() const         { return top ? top->window : None; } \
	static  Ctrl   *CtrlFromWindow(Window w); \
	static bool    TrapX11Errors(); \
	static void    UntrapX11Errors(bool b); \
 \
	Window GetParentWindow(void) const; \
	Ctrl *GetParentWindowCtrl(void) const; \
	Rect GetRectInParentWindow(void) const; \
 \
	static void SyncNativeWindows(void); \
public: \
	static void InitX11(const char *display); \
	static void ExitX11(); \
	static void GuiFlush()                              { XFlush(Xdisplay); } \



#define GUIPLATFORM_PASTECLIP_DECLS \
	int          type;


#define GUIPLATFORM_TOPWINDOW_DECLS \
protected: \
	virtual void    EventProc(XWindow& w, XEvent *event); \
private: \
	XSizeHints *size_hints; \
	XWMHints   *wm_hints; \
	XClassHint *class_hint; \
	Size        xminsize, xmaxsize; \
	bool        topmost; \
\
	void          CenterRect(Ctrl *owner); \
	void          DefSyncTitle(); \
	void          EndIgnoreTakeFocus(); \
\
	void          SyncState(); \
	void          SyncState0(); \
\
	Image         invert; \
	WString       title2; \


#define GUIPLATFORM_VIEWDRAW_DECLS \
	bool   caret; \


NAMESPACE_UPP

String XAtomName(Atom atom);
Atom   XAtom(const char *name);

String      GetProperty(Window w, Atom property, Atom rtype = AnyPropertyType);
Vector<int> GetPropertyInts(Window w, Atom property, Atom rtype = AnyPropertyType);
String      ReadPropertyData(Window w, Atom property, Atom rtype = AnyPropertyType);

Index<Atom>& _NET_Supported();

#include "stdids.h"

END_UPP_NAMESPACE
