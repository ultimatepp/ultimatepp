#include "CtrlCore.h"

#ifdef GUI_WIN

#include <imm.h>

namespace Upp {

#define LLOG(x)    //   DLOG(x)
// #define LOGTIMING 1 _DBG_

#ifdef _DEBUG
#define LOGMESSAGES 0
#endif

#define ELOGW(x)   // RLOG(GetSysTime() << ": " << x) // Only activate in MT!
#define ELOG(x)    // RLOG(GetSysTime() << ": " << x)

template<>
hash_t GetHashValue(const HWND& h)
{
	return (hash_t)(intptr_t)h;
}

bool Ctrl::GetMsg(MSG& msg)
{
	if(!PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) return false;
	return PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE);
}

static bool sFinished;

bool IsExiting()
{
	return sFinished;
}

static BOOL CALLBACK sDumpWindow(HWND hwnd, LPARAM lParam) {
	String dump;
	dump << (IsWindowEnabled(hwnd) ? "ena" : "dis") << ' '
	     << (IsWindowVisible(hwnd) ? "vis" : "hid") << ' '
	     << Sprintf("owner=0x%x ", GetWindow(hwnd, GW_OWNER));
	Ctrl *ctrl = Ctrl::CtrlFromHWND(hwnd);
	if(ctrl) {
#ifdef _DEBUG
		dump << "Ctrl: " << UPP::Name(ctrl);
#endif
	}
	else if(!lParam)
		return TRUE;
	else
	{
#ifdef PLATFORM_WINCE
		wchar clsname[256], title[1024];
#else
		char clsname[256], title[1024];
#endif
		::GetClassName(hwnd, clsname, __countof(clsname));
		::GetWindowText(hwnd, title, __countof(title));
		dump << "HWND: " << Sprintf("0x%x", hwnd) << ", class = "
		     << clsname << ", title = " << title;
	}
	LLOG(dump);
	return TRUE;
}

void DumpWindowOrder(bool aliens) {
	LLOG("DumpWindowOrder" << LOG_BEGIN);
	EnumChildWindows(NULL, &sDumpWindow, (LPARAM)(aliens ? 1 : 0));
	LLOG(LOG_END << "//DumpWindowOrder");
}

HCURSOR   Ctrl::hCursor;
HINSTANCE Ctrl::hInstance;
#ifndef flagDLL
#ifndef PLATFORM_WINCE
HANDLE    Ctrl::OverwatchThread;
HWND      Ctrl::OverwatchHWND;

Win32Event Ctrl::OverwatchEndSession;
Win32Event Ctrl::ExitLoopEvent;
#endif
#endif

bool Ctrl::endsession;

void Ctrl::EndSession()
{
	GuiLock __;
	EndSessionLoopNo = EventLoopNo;
	endsession = true;
}

#ifndef flagDLL
#ifndef PLATFORM_WINCE
LRESULT CALLBACK Ctrl::OverwatchWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if(msg == WM_USER) {
		ELOGW("WM_USER");
		PostQuitMessage(0);
	}
	if(msg == WM_QUERYENDSESSION) {
		static BOOL (WINAPI *ShutdownBlockReasonCreate)(HWND hWnd, LPCWSTR pwszReason);
		static BOOL (WINAPI *ShutdownBlockReasonDestroy)(HWND hWnd);
		ONCELOCK {
			DllFn(ShutdownBlockReasonCreate, "user32", "ShutdownBlockReasonCreate");
			DllFn(ShutdownBlockReasonDestroy, "user32", "ShutdownBlockReasonDestroy");
		}
		if(ShutdownBlockReasonCreate)
			ShutdownBlockReasonCreate(hwnd, ToSystemCharsetW(t_("waiting for user response")));
		EndSession();
		ELOGW("WM_QUERYENDSESSION 1");
		OverwatchEndSession.Wait();
		if(ShutdownBlockReasonDestroy)
			ShutdownBlockReasonDestroy(hwnd);
		ELOGW("WM_QUERYENDSESSION 2");
		return TRUE;
	}
	if(msg == WM_ENDSESSION) {
		EndSession();
		ELOGW("WM_ENDSESSION 1");
		ExitLoopEvent.Set();
		ELOGW("WM_ENDSESSION 2");
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

DWORD WINAPI Ctrl::Win32OverwatchThread(LPVOID)
{
    WNDCLASS  wc;
    Zero(wc);
	wc.style         = CS_DBLCLKS|CS_HREDRAW|CS_VREDRAW;
	wc.lpfnWndProc   = (WNDPROC)OverwatchWndProc;
	wc.hInstance     = hInstance;
	wc.hCursor       = NULL;
	wc.hbrBackground = NULL;
	wc.lpszClassName = "UPP-OVERWATCH";
	RegisterClass(&wc);

	OverwatchHWND = CreateWindowEx(0, "UPP-OVERWATCH", "", WS_OVERLAPPEDWINDOW,
	                               -1000, -1000, 50, 50, NULL, NULL, hInstance, NULL);

	ELOGW("OverWatch 1");
	ExitLoopEvent.Set();
	ELOGW("OverWatch 2");
	MSG Msg;
	while(GetMessage(&Msg, NULL, 0, 0) > 0) {
        TranslateMessage(&Msg);
		DispatchMessageW(&Msg);
    }
	ELOGW("OverWatch 3");
	return 0;
}
#endif
#endif

HWND utilityHWND = 0;

extern VectorMap<int, ClipData>& sClipMap();

INITBLOCK
{
	sClipMap();
}

EXITBLOCK
{
	if(utilityHWND) DestroyWindow(utilityHWND);
}

LRESULT CALLBACK Ctrl::UtilityProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	sClipMap();
	switch(message) {
	case WM_TIMER:
		TimerProc(msecs());
		AnimateCaret();
		SyncCustomTitleBars();
		break;
	case WM_RENDERFORMAT:
		RenderFormat((dword)wParam);
		return 0;
	case WM_DESTROYCLIPBOARD:
		DestroyClipboard();
		return 0;
	}
	return ::DefWindowProc(hWnd, message, wParam, lParam);
}

#ifdef PLATFORM_WINCE
#define L_(x)  L##x
#else
#define L_(x)  x
#endif

static DWORD sMainThreadId;

void WakeUpGuiThread()
{
	::PostThreadMessage(sMainThreadId, WM_NULL, 0, 0);
}

void AvoidPaintingCheck__();

static void Win32PanicMessageBox(const char *title, const char *text)
{
	AvoidPaintingCheck__();
#ifdef PLATFORM_WINCE
	static wchar wtext[256], wtitle[256];
	ToUnicode(wtext, text, strlen(text), CHARSET_DEFAULT);
	ToUnicode(wtitle, title, strlen(title), CHARSET_DEFAULT);
	MessageBox(::GetActiveWindow(), wtext, wtitle, MB_ICONSTOP | MB_OK | MB_APPLMODAL);
#else
	MessageBox(::GetActiveWindow(), text, title, MB_ICONSTOP | MB_OK | MB_APPLMODAL);
#endif
}

void Ctrl::InstallPanicBox()
{
	InstallPanicMessageBox(&Win32PanicMessageBox);
}

extern bool is_custom_titlebar_available__;
extern Event<const TopWindow *, TopWindow::CustomTitleBarMetrics&> custom_titlebar_metrics__;

void Ctrl::InitWin32(HINSTANCE hInstance)
{
	GuiLock __;
	LLOG("InitWin32");
	
	InstallPanicMessageBox(&Win32PanicMessageBox);
//	RLOGBLOCK("Ctrl::InitWin32");
	sMainThreadId = GetCurrentThreadId();
#define ILOG(x) // RLOG(x)
	Ctrl::hInstance = hInstance;
	{
		ILOG("RegisterClassW");
		WNDCLASSW  wc;
		Zero(wc);
		wc.style         = CS_DBLCLKS|CS_HREDRAW|CS_VREDRAW;
		wc.lpfnWndProc   = (WNDPROC)Ctrl::WndProc;
		wc.hInstance     = hInstance;
		wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
		wc.lpszClassName = L"UPP-CLASS-W";
		RegisterClassW(&wc);
		wc.style         = 0x20000|CS_DBLCLKS|CS_HREDRAW|CS_VREDRAW;
		wc.lpszClassName = L"UPP-CLASS-DS-W";
		RegisterClassW(&wc);
		wc.style         = CS_SAVEBITS|CS_DBLCLKS|CS_HREDRAW|CS_VREDRAW;
		wc.lpszClassName = L"UPP-CLASS-SB-W";
		RegisterClassW(&wc);
		wc.style         = 0x20000|CS_DBLCLKS|CS_HREDRAW|CS_VREDRAW|CS_SAVEBITS;
		wc.lpszClassName = L"UPP-CLASS-SB-DS-W";
		RegisterClassW(&wc);
	}

	WNDCLASS  wca;
	Zero(wca);
	wca.hInstance     = hInstance;
	wca.hbrBackground = (HBRUSH)NULL;
	wca.style         = 0;
	wca.lpszClassName = L_("UPP-TIMER");
	wca.hCursor       = NULL;
	wca.lpfnWndProc   = &Ctrl::UtilityProc;
	RegisterClass(&wca);

	ILOG("InitTimer");
	InitTimer();
	ILOG("SetTimer");
	utilityHWND = CreateWindow(L_("UPP-TIMER"), L_(""), WS_OVERLAPPED,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, hInstance, NULL);
	SetTimer(utilityHWND, 1, 10, NULL);
	ILOG("Windows");
	Windows();

	ReSkin();

	OleInitialize(NULL);

	Csizeinit();
#undef ILOG

	GlobalBackPaint();

	is_custom_titlebar_available__ = IsWin11();

	custom_titlebar_metrics__ = [=](const TopWindow *tw, TopWindow::CustomTitleBarMetrics& m) {
		if(!tw->custom_titlebar)
			return;
		m.height = GetWin32TitleBarHeight(tw);
		m.lm = 0;
		Image icon = tw->GetIcon();
		if(!IsNull(icon))
			m.lm = DPI(4) + min(icon.GetWidth(), 32);
		m.rm = (tw->IsZoomable() ? 3 : 1) * GetWin32TitleBarButtonWidth();
	};
	
	EnterGuiMutex();
}

typedef DWORD (WINAPI *PSLWA)(HWND, DWORD, BYTE, DWORD);

static PSLWA SetLayeredWindowAttributes()
{
	static PSLWA pSet;
#ifndef PLATFORM_WINCE
	static bool inited = false;
	if(!inited) {
		inited = true;
		if(HMODULE hDLL = LoadLibrary ("user32"))
			pSet = (PSLWA) GetProcAddress(hDLL, "SetLayeredWindowAttributes");
	}
#endif
	return pSet;
}

bool Ctrl::IsAlphaSupported()
{
	return SetLayeredWindowAttributes();
}

bool Ctrl::IsCompositedGui()
{
	return false;
}

void Ctrl::ExitWin32()
{
	RenderAllFormats();

	TopWindow::ShutdownWindows();
	CloseTopCtrls();

	OleUninitialize();

	sFinished = true;

	for(int i = 0; i < hotkey.GetCount(); i++)
		if(hotkey[i])
			UnregisterHotKey(NULL, i);
	
	for(int i = 0; i < Windows().GetCount(); i++) {
		HWND hwnd = Windows().GetKey(i);
		if(hwnd)
			::DestroyWindow(hwnd);
	}
	MSG msg;
	while(GetMsg(msg))
		if(msg.message != WM_QUIT)
			::PostQuitMessage(0);
#ifndef flagDLL
#ifndef PLATFORM_WINCE
	ELOG("ExitWin32 1");
	OverwatchEndSession.Set();
	ELOG("ExitWin32 2");
	PostMessage(OverwatchHWND, WM_USER, 0, 0);
	ELOG("ExitWin32 3");
	LLOG("Waiting for overwatch thread to finish...");
	WaitForSingleObject(OverwatchThread, INFINITE);
	ELOG("ExitWin32 4");
	LLOG("...overwatch thread finished");
#endif
#endif
	LeaveGuiMutex();
}

void Ctrl::SetTimerGranularity(int ms)
{
	if(ms > 0)
		SetTimer(utilityHWND, 1, ms, NULL);
	else
		KillTimer(utilityHWND, 1);
}

VectorMap< HWND, Ptr<Ctrl> >& Ctrl::Windows()
{
	static VectorMap< HWND, Ptr<Ctrl> > map;
	return map;
}

Vector<Ctrl *> Ctrl::GetTopCtrls()
{
	Vector<Ctrl *> v;
	VectorMap< HWND, Ptr<Ctrl> >& w = Windows();
	for(int i = 0; i < w.GetCount(); i++)
		if(w.GetKey(i) && w[i] && !w[i]->GetParent())
			v.Add(w[i]);
	return v;
}

void  Ctrl::SetMouseCursor(const Image& image)
{
	GuiLock __;
	static Image img;
	if(image.GetSerialId() != img.GetSerialId()) {
		img = image;
		HCURSOR hc = SystemDraw::IconWin32(img, true);
		SetCursor(hc);
		if(hCursor)
			DestroyCursor(hCursor);
		hCursor = hc;
	}
}

Ctrl *Ctrl::CtrlFromHWND(HWND hwnd)
{
	GuiLock __;
	return hwnd ? Windows().Get(hwnd, NULL) : NULL;
}

HWND Ctrl::GetOwnerHWND() const
{
	GuiLock __;
	HWND hwnd = GetHWND();
	if(!hwnd) return NULL;
	return GetWindow(hwnd, GW_OWNER);
}

Ctrl *Ctrl::GetOwner()
{
	GuiLock __;
	HWND hwnd = GetOwnerHWND();
	return hwnd ? CtrlFromHWND(hwnd) : NULL;
}

Ctrl *Ctrl::GetActiveCtrl()
{
	GuiLock __;
	if(focusCtrl)
		return focusCtrl->GetTopCtrl();
	HWND actwnd = ::GetActiveWindow();
	Vector<Ctrl *> top = GetTopCtrls();
	for(int i = 0; i < top.GetCount(); i++)
		if(top[i]->IsActiveX() && top[i]->GetHWND()) {
			LLOG("-> top[" << i << "] = " << FormatIntHex(top[i]->GetHWND()));
			for(HWND hwnd = top[i]->GetHWND(); hwnd; hwnd = ::GetParent(hwnd))
				if(hwnd == actwnd) {
					LLOG("-> match for " <<UPP::Name(top[i]));
					return top[i];
				}
		}
	LLOG("//Ctrl::GetActiveCtrl -> not found (NULL)");
	return NULL;
}

UDropTarget *NewUDropTarget(Ctrl *);

String WindowStyleAsString(dword style, dword exstyle);

void Ctrl::UseImmersiveDarkModeForWindowBorder()
{
	static HRESULT (WINAPI *DwmSetWindowAttribute)(HWND hwnd, DWORD dwAttribute, LPCVOID pvAttribute, DWORD cbAttribute);
	ONCELOCK {
		DllFn(DwmSetWindowAttribute, "dwmapi.dll", "DwmSetWindowAttribute");
	}
	if(DwmSetWindowAttribute) {
		const auto DWMWA_USE_IMMERSIVE_DARK_MODE = 20;
		
		BOOL useDarkTheme = IsDarkTheme();
		DwmSetWindowAttribute(
			utop->hwnd, DWMWA_USE_IMMERSIVE_DARK_MODE,
			&useDarkTheme, sizeof(useDarkTheme));
	}
}

void Ctrl::Create(HWND parent, DWORD style, DWORD exstyle, bool savebits, int show, bool dropshadow)
{
	GuiLock __;
	ASSERT_(IsMainThread(), "Window creation can only happen in the main thread");
	LLOG("Ctrl::Create(parent = " << (void *)parent << ") in " <<UPP::Name(this) << LOG_BEGIN);
	ASSERT(!IsChild() && !IsOpen());
	Rect r = GetRect();
	AdjustWindowRectEx(r, style, FALSE, exstyle);
	isopen = true;
	Top *top = new Top;
	SetTop(top);
	ASSERT(!parent || IsWindow(parent));
	style &= ~WS_VISIBLE;
	dropshadow = false;
	top->hwnd = CreateWindowEx(exstyle,
	                           savebits ? dropshadow ? "UPP-CLASS-SB-DS-W" : "UPP-CLASS-SB-W"
	                                    : dropshadow ? "UPP-CLASS-DS-W" : "UPP-CLASS-W",
	                           "", style, 0, 0, 0, 0,
	                           parent, NULL, hInstance, this);

	inloop = false;
	erasebg = true; // avoid flickering

	ASSERT(top->hwnd);
	
	UseImmersiveDarkModeForWindowBorder();
	
	::MoveWindow(top->hwnd, r.left, r.top, r.Width(), r.Height(), false); // To avoid "black corners" artifact effect
	::ShowWindow(top->hwnd, visible ? show : SW_HIDE);
	
	StateH(OPEN);
	LLOG(LOG_END << "//Ctrl::Create in " <<UPP::Name(this));
	RegisterDragDrop(top->hwnd, (LPDROPTARGET) (top->dndtgt = NewUDropTarget(this)));
	::ImmAssociateContextEx(top->hwnd, NULL, 0);
	CancelMode();
	RefreshLayoutDeep();
}

void ReleaseUDropTarget(UDropTarget *dt);

void Ctrl::WndFree()
{
	GuiLock __;
	Top *top = GetTop();
	RevokeDragDrop(GetHWND());
	if(!top) return;
	ReleaseUDropTarget(top->dndtgt);
	isopen = false;
	HWND owner = GetWindow(top->hwnd, GW_OWNER);// CXL 31.10.2003 z DoRemove
	bool focus = ::GetFocus() == top->hwnd;
	LLOG("Ctrl::WndDestroy owner " << (void *)owner
	     << " focus " << focus
	     << " ::GetFocus() " << (void *)::GetFocus());
	if(owner && focus) {
		LLOG("Ctrl::WndFree->SetFocus " << UPP::Name(Ctrl::CtrlFromHWND(owner)));
		Ctrl *o = GetOwner();
		if(o && !o->IsEnabled()) // owner needs to be enabled, otherwise SetFocus bounces back
			o->Enable();
		::SetFocus(owner);
	}
	LLOG(LOG_END << "//Ctrl::WndFree() in " <<UPP::Name(this));
	DeleteTop();
}

void Ctrl::WndDestroy()
{
	GuiLock __;
	Top *top = GetTop();
	if(top && top->hwnd) {
		HWND hwnd = top->hwnd;
		WndFree();
		::DestroyWindow(hwnd);
	}
}

Image Ctrl::DoMouse(int e, Point p, int zd)
{
//	LLOG("Ctrl::DoMouse(" << p << ", " << e << ")");
	GuiLock __;
	eventCtrl = this;
	Image img = DispatchMouse(e, p, zd);
	SyncCaret();
	return img;
}

#ifdef _DEBUG

bool Ctrl::LogMessages;

#define x_MSG(x)       { x, #x },

struct WinMsg {
	int         ID;
	const char *name;
}
sWinMsg[] = {
#include "Win32Msg.i"
	{0, NULL}
};

#endif

void Ctrl::NcCreate(HWND hwnd)
{
	GuiLock __;
	Top *top = GetTop();
	if(top)
		top->hwnd = hwnd;
}

void Ctrl::NcDestroy()
{
	GuiLock __;
	Top *top = GetTop();
	if(top)
		WndFree();
}

bool Ctrl::PreprocessMessage(MSG& msg)
{
	return false;
}

LRESULT CALLBACK Ctrl::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	GuiLock __;
	if(sFinished)
		return DefWindowProc(hWnd, message, wParam, lParam);
#ifdef PLATFORM_WINCE
	if(message == WM_CREATE)
#else
	if(message == WM_NCCREATE)
#endif
	{
		Ctrl *w = (Ctrl *)((LPCREATESTRUCT) lParam)->lpCreateParams;
		if(w) {
			w->NcCreate(hWnd);
			int i = Windows().Find(NULL);
			if(i >= 0) {
				Windows().SetKey(i, hWnd);
				Windows()[i] = w;
			}
			else
				Windows().Add(hWnd) = w;
		}
	}
	Ctrl *w = Windows().Get(hWnd, NULL);
#ifdef PLATFORM_WINCE
	if(message == WM_DESTROY)
#else
	if(message == WM_NCDESTROY)
#endif
	{
		if(w) w->NcDestroy();
		int i = Windows().Find(hWnd);
		if(i >= 0)
			Windows().SetKey(i, NULL);
	}
#if LOGMESSAGES
	bool logblk = false;
	if(message != WM_SETCURSOR && message != WM_CTLCOLORBTN && message != WM_TIMER &&
	   message != WM_NCHITTEST  &&  message != WM_ENTERIDLE &&
	   message != WM_CTLCOLORDLG && message != WM_CTLCOLOREDIT && message != WM_CTLCOLORLISTBOX &&
	   message != WM_CTLCOLORMSGBOX && message != WM_CTLCOLORSCROLLBAR &&
	   message != WM_CTLCOLORSTATIC && message != WM_CANCELMODE &&
	   message != 0x0118)
		for(WinMsg *m = sWinMsg; m->ID; m++)
			if(m->ID == message) {
				RLOG(m->name << ' ' << UPP::Name(w) <<
					Sprintf(", wParam = %d (0x%x), lParam = %d (0x%x)",
					       wParam, wParam, lParam, lParam));
				VppLog() << LOG_BEGIN;
				logblk = true;
				break;
			}
#endif
	LRESULT l = 0;
	if(w && (w->GetHWND() || w->isdhctrl)) {
#if LOGTIMING
			int ticks = msecs();
			String wname = w->Name();
#endif
			Ptr<Ctrl> pw = w;
			l = w->WindowProc(message, wParam, lParam);
			if(pw)
				pw->SyncMoves();
#if LOGTIMING
			String msgname;
			for(WinMsg *m = sWinMsg; m->ID; m++)
				if(m->ID == message) {
					msgname = m->name;
					break;
				}
			if(IsNull(msgname))
				msgname = NFormat("0x%04x", (int)message);
			RLOG(NFormat("T+%d %s 0x%08x 0x%08x -> %s", msecs(ticks), msgname, (int)wParam, (int)lParam, wname));
#endif
	}
	else
		l = DefWindowProc(hWnd, message, wParam, lParam);
#if LOGMESSAGES
	if(logblk)
		VppLog() << LOG_END;
#endif
	return l;
}

bool PassWindowsKey(int wParam)
{
	return wParam >= VK_NUMPAD0 && wParam <= VK_NUMPAD9
	||     wParam == VK_INSERT || wParam == VK_DELETE
	||     wParam == VK_HOME || wParam == VK_END
	||     wParam == VK_PRIOR || wParam == VK_NEXT
	||     wParam == VK_UP || wParam == VK_DOWN
	||     wParam == VK_LEFT || wParam == VK_RIGHT
	||     wParam == VK_CLEAR || wParam == VK_SPACE
	||     wParam >= 0x90; // OEM keys
}

Vector<Event<> > Ctrl::hotkey;

int Ctrl::RegisterSystemHotKey(dword key, Function<void ()> cb)
{
	ASSERT(key >= K_DELTA);
	int q = hotkey.GetCount();
	for(int i = 0; i < hotkey.GetCount(); i++)
		if(!hotkey[i]) {
			q = i;
			break;
		}
	hotkey.At(q) = Event<> () << cb;
	dword mod = 0;
	if(key & K_ALT)
		mod |= MOD_ALT;
	if(key & K_SHIFT)
		mod |= MOD_SHIFT;
	if(key & K_CTRL)
		mod |= MOD_CONTROL;
	
	return RegisterHotKey(NULL, q, mod, key & 0xffff) ? q : -1;
}

void Ctrl::UnregisterSystemHotKey(int id)
{
	if(id >= 0 && id < hotkey.GetCount()) {
		UnregisterHotKey(NULL, id);
		hotkey[id].Clear();
	}
}

void Ctrl::sProcessMSG(MSG& msg)
{
	if (msg.message == WM_HOTKEY) {
		if((int)msg.wParam >= 0 && (int)msg.wParam < Ctrl::hotkey.GetCount())
			Ctrl::hotkey[(int)msg.wParam]();
		return;
	}
	
	if(!DHCtrl::PreprocessMessageAll(msg))
		if(msg.message != WM_SYSKEYDOWN && msg.message != WM_SYSKEYUP
		|| PassWindowsKey((dword)msg.wParam) || msg.wParam == VK_MENU) //17.11 Mirek - fix to get windows menu invoked on Alt+Space
			TranslateMessage(&msg); // 04/09/07: TRC fix to make barcode reader going better

#if 0
	DDUMP(msg.hwnd);
		for(WinMsg *m = sWinMsg; m->ID; m++)
			if(m->ID == msg.message) {
				RLOG(m->name << ' ' <<
					Sprintf(", wParam = %d (0x%x), lParam = %d (0x%x)",
					        msg.wParam, msg.wParam, msg.lParam, msg.lParam));
				break;
			}

	char cls[200];
	GetClassName(msg.hwnd, cls, 200);
	DDUMP(cls);
#endif

	DispatchMessageW(&msg);
}

bool Ctrl::IsWaitingEvent()
{
	ASSERT_(IsMainThread(), "IsWaitingEvent can only run in the main thread");
	MSG msg;
	return PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE);
}

bool Ctrl::ProcessEvent(bool *quit)
{
	ASSERT_(IsMainThread(), "ProcessEvent can only run in the main thread");
	if(!GetMouseLeft() && !GetMouseRight() && !GetMouseMiddle())
		ReleaseCtrlCapture();
	MSG msg;
	if(GetMsg(msg)) {
		if(msg.message == WM_QUIT && quit)
			*quit = true;
//		LLOG(GetSysTime() << " % " << (unsigned)msecs() % 10000 << ": sProcessMSG " << FormatIntHex(msg.message));
		sProcessMSG(msg);
//		LLOG(GetSysTime() << " % " << (unsigned)msecs() % 10000 << ": //sProcessMSG " << FormatIntHex(msg.message));
		DefferedFocusSync();
		SyncCaret();
		PreeditSync([](Ctrl *top, bool enable) {
			if(HWND hwnd = top->GetHWND())
			  ::ImmAssociateContextEx(hwnd, NULL, enable * IACE_DEFAULT);
		});
		return true;
	}
	return false;
}

void Ctrl::SysEndLoop()
{
}

bool Ctrl::ProcessEvents(bool *quit)
{
	ASSERT_(IsMainThread(), "ProcessEvents can only run in the main thread");
	if(ProcessEvent(quit)) {
		while(ProcessEvent(quit) && (!LoopCtrl || LoopCtrl->InLoop())); // LoopCtrl-MF 071008
		SweepMkImageCache();
		return true;
	}
	SweepMkImageCache();
	return false;
}

void Ctrl::EventLoop(Ctrl *ctrl)
{
	GuiLock __;
	ASSERT_(IsMainThread(), "EventLoop can only run in the main thread");
	ASSERT(LoopLevel == 0 || ctrl);
	LoopLevel++;
	LLOG("Entering event loop at level " << LoopLevel << LOG_BEGIN);
	Ptr<Ctrl> ploop;
	if(ctrl) {
		ploop = LoopCtrl;
		LoopCtrl = ctrl;
		ctrl->inloop = true;
	}

	bool quit = false;
	int64 loopno = ++EventLoopNo;
	ProcessEvents(&quit);
	while(loopno > EndSessionLoopNo && !quit && (ctrl ? ctrl->IsOpen() && ctrl->InLoop() : GetTopCtrls().GetCount()))
	{
//		LLOG(GetSysTime() << " % " << (unsigned)msecs() % 10000 << ": EventLoop / GuiSleep");
		SyncCaret();
		GuiSleep(1000);
//		LLOG(GetSysTime() << " % " << (unsigned)msecs() % 10000 << ": EventLoop / ProcessEvents");
		ProcessEvents(&quit);
//		LLOG(GetSysTime() << " % " << (unsigned)msecs() % 10000 << ": EventLoop / after ProcessEvents");
	}

	if(ctrl)
		LoopCtrl = ploop;
	LoopLevel--;
	LLOG(LOG_END << "Leaving event loop ");
}

void Ctrl::GuiSleep(int ms)
{
	GuiLock __;
	ASSERT_(IsMainThread(), "Only the main thread can perform GuiSleep");
	ELOG("GuiSleep");
	int level = LeaveGuiMutexAll();
#if !defined(flagDLL) && !defined(PLATFORM_WINCE)
	if(!OverwatchThread) {
		DWORD dummy;
		OverwatchThread = CreateThread(NULL, 0x100000, Win32OverwatchThread, NULL, 0, &dummy);
		ELOG("ExitLoopEventWait 1");
		ExitLoopEvent.Wait();
	}
	HANDLE h[1];
	*h = ExitLoopEvent.GetHandle();
	ELOG("ExitLoopEventWait 2 " << (void *)*h);
	MsgWaitForMultipleObjects(1, h, FALSE, ms, QS_ALLINPUT);
#else
	MsgWaitForMultipleObjects(0, NULL, FALSE, ms, QS_ALLINPUT);
#endif
	EnterGuiMutex(level);
}

Rect Ctrl::GetWndScreenRect() const
{
	GuiLock __;
	HWND hwnd = GetHWND();
	if(!hwnd) return Null;
	Rect r;
	::GetWindowRect(hwnd, r);
	return r;
}

void Ctrl::WndShow(bool b)
{
	GuiLock __;
	HWND hwnd = GetHWND();
	if(hwnd)
		::ShowWindow(hwnd, b ? SW_SHOW : SW_HIDE);
}

void Ctrl::WndUpdate()
{
	GuiLock __;
	HWND hwnd = GetHWND();
	if(hwnd) ::UpdateWindow(hwnd);
}

bool Ctrl::IsWndOpen() const {
	GuiLock __;
	return GetHWND();
}

void Ctrl::SetAlpha(byte alpha)
{
	GuiLock __;
	HWND hwnd = GetHWND();
	if(!IsAlphaSupported() || GetParent() || !top || !hwnd)
		return;
	if(alpha == 255) {
		SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) & ~0x80000);
		return;
	}
	SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | 0x80000);
    SetLayeredWindowAttributes() (hwnd, 0, alpha, 2);
}

#define DLLFILENAME "User32.dll"
#define DLIMODULE   MultiMon
#define DLIHEADER   <CtrlCore/MultiMon.dli>
#include <Core/dli.h>

Rect MonitorRectForHWND(HWND hwnd)
{
	if(hwnd && MultiMon())
		if(HMONITOR monitor = MultiMon().MonitorFromWindow(hwnd, 2/*MONITOR_DEFAULTTONEAREST*/)) {
			MONITORINFO moninfo;
			Zero(moninfo);
			moninfo.cbSize = sizeof(moninfo);
			MultiMon().GetMonitorInfo(monitor, &moninfo);
			return Rect(moninfo.rcWork);
		}
	return Ctrl::GetPrimaryWorkArea();
}

Rect Ctrl::GetWorkArea() const
{
// return MonitorRectForHWND(GetHWND());
// mst:2008-12-08, hack for better multimonitor support.
	GuiLock __;
	const Ctrl *topctl = GetTopCtrl();
	HWND hwnd = topctl->GetHWND();
	if(!hwnd && !((topctl = topctl->GetOwnerCtrl()) && (hwnd = topctl->GetHWND())))
		hwnd = ::GetFocus();
	return MonitorRectForHWND(hwnd);
}

static BOOL CALLBACK sMonEnumProc(HMONITOR monitor, HDC hdc, LPRECT lprcMonitor, LPARAM data)
{
	MONITORINFO moninfo;
	Zero(moninfo);
	moninfo.cbSize = sizeof(moninfo);
	MultiMon().GetMonitorInfo(monitor, &moninfo);
	((Array<Rect> *)data)->Add(Rect(moninfo.rcWork));
	return TRUE;
}

void Ctrl::GetWorkArea(Array<Rect>& rc)
{
	GuiLock __;
	MultiMon().EnumDisplayMonitors(NULL, NULL, &sMonEnumProc, (LPARAM)&rc);
}

Rect Ctrl::GetVirtualWorkArea()
{
	Rect out = GetPrimaryWorkArea();
	Array<Rect> rc;
	GetWorkArea(rc);
	for(int i = 0; i < rc.GetCount(); i++)
		out |= rc[i];
	return out;
}

Rect Ctrl::GetVirtualScreenArea()
{
	GuiLock __;
	return RectC(GetSystemMetrics(SM_XVIRTUALSCREEN),
		GetSystemMetrics(SM_YVIRTUALSCREEN),
		GetSystemMetrics(SM_CXVIRTUALSCREEN),
		GetSystemMetrics(SM_CYVIRTUALSCREEN));
}

Rect Ctrl::GetPrimaryWorkArea()
{
	Rect r;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &r, 0);
	LLOG("Ctrl::GetWorkArea -> " << r);
	return r;
}

Rect Ctrl::GetPrimaryScreenArea()
{
	return Size(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
}

int Ctrl::GetKbdDelay()
{
	GuiLock __;
	int a;
	SystemParametersInfo(SPI_GETKEYBOARDDELAY, 0, &a, 0);
	return 250 + a * 750 / 4;
}

int Ctrl::GetKbdSpeed()
{
	GuiLock __;
	int a;
	SystemParametersInfo(SPI_GETKEYBOARDSPEED, 0, &a, 0);
	return 1000 / (a + 2);
}

void Ctrl::SetWndForeground()
{
	GuiLock __;
	LLOG("Ctrl::SetWndForeground() in " << UPP::Name(this));
	HWND hwnd = GetHWND();
	if(hwnd)
		::SetForegroundWindow(hwnd);
}

bool Ctrl::IsWndForeground() const
{
	GuiLock __;
	HWND hwnd = GetHWND();
	if(!hwnd)
		return false;
	HWND fore = ::GetForegroundWindow();
	LLOG("Ctrl::IsWndForeground(): hwnd = " << (void *)hwnd
	     << ", fore = " << (void *)fore << " - " << UPP::Name(CtrlFromHWND(fore)));
	if(IsActiveX()) {
		while(hwnd && hwnd != fore && !!::GetParent(hwnd))
			hwnd = ::GetParent(hwnd);
	}
	return hwnd == fore;
}

void Ctrl::WndEnable(bool b)
{
	GuiLock __;
	LLOG("Ctrl::WndEnable(" << b << ") in " << UPP::Name(this) << ", focusCtrlWnd = " << UPP::Name(~focusCtrlWnd) << ", raw = " << (void *)::GetFocus());
	if(b)
		ReleaseCapture();
	LLOG("//Ctrl::WndEnable(" << b << ") -> false " <<UPP::Name(this) << ", focusCtrlWnd = " <<UPP::Name(~focusCtrlWnd) << ", raw = " << (void *)::GetFocus());
}

bool Ctrl::SetWndFocus()
{
	GuiLock __;
	LLOG("Ctrl::SetWndFocus() in " << UPP::Name(this));
	HWND hwnd = GetHWND();
	if(hwnd) {
		LLOG("Ctrl::SetWndFocus() -> ::SetFocus(" << (void *)hwnd << ")");
//		::SetActiveWindow(hwnd);
		::SetFocus(hwnd);
		return true;
	}
	LLOG("//Ctrl::SetWndFocus() in " <<UPP::Name(this) << ", active window = " << (void *)::GetActiveWindow());
	return false;
}

bool Ctrl::HasWndFocus() const
{
	GuiLock __;
	HWND hwnd = GetHWND();
	return hwnd && ::GetFocus() == hwnd;
}

bool Ctrl::SetWndCapture()
{
	GuiLock __;
	LLOG("Ctrl::SetWndCapture() in " << UPP::Name(this));
	ASSERT(IsMainThread());
	HWND hwnd = GetHWND();
	if(hwnd) {
		::SetCapture(hwnd);
		LLOG("SetCapture succeeded");
		return true;
	}
	return false;
}

bool Ctrl::ReleaseWndCapture()
{
	GuiLock __;
	LLOG("Ctrl::ReleaseWndCapture() in " << UPP::Name(this));
	ASSERT(IsMainThread());
	HWND hwnd = GetHWND();
	if(hwnd && HasWndCapture())
	{
		::ReleaseCapture();
		LLOG("ReleaseCapture succeeded");
		return true;
	}
	return false;
}

bool Ctrl::HasWndCapture() const
{
	GuiLock __;
	HWND hwnd = GetHWND();
	return hwnd && hwnd == ::GetCapture();
}

void Ctrl::WndInvalidateRect(const Rect& r)
{
	GuiLock __;
	LLOG("WndInvalidateRect " << UPP::Name(this));
	HWND hwnd = GetHWND();
	if(hwnd)
		::InvalidateRect(hwnd, r, false);
}

void Ctrl::WndSetPos(const Rect& rect)
{
	GuiLock __;
	LLOG("WndSetPos " << UPP::Name(this) << " " << rect);
	HWND hwnd = GetHWND();
	if(hwnd) {
		Rect r = rect;
		AdjustWindowRectEx(r, ::GetWindowLong(hwnd, GWL_STYLE), FALSE,
		                   ::GetWindowLong(hwnd, GWL_EXSTYLE));
		SetWindowPos(hwnd, NULL, r.left, r.top, r.Width(), r.Height(),
		             SWP_NOACTIVATE|SWP_NOZORDER);
		if(HasFocusDeep()) {
			caretCtrl = NULL;
			SyncCaret();
		}
	}
	fullrefresh = false;
}

void Ctrl::WndUpdate(const Rect& r)
{
	GuiLock __;
	LLOG("WndUpdate " << UPP::Name(this));
	Ctrl *top = GetTopCtrl();
	if(top->IsOpen()) {
		HWND hwnd = top->GetHWND();
		HDC hdc = GetDC(hwnd);
		HRGN hrgn = CreateRectRgn(0, 0, 0, 0);
		if(GetUpdateRgn(hwnd, hrgn, FALSE) != NULLREGION) {
			SelectClipRgn(hdc, hrgn);
			SystemDraw draw(hdc);
			draw.Clip(r);
			top->UpdateArea(draw, r);
			ValidateRect(hwnd, r);
			SelectClipRgn(hdc, NULL);
		}
		ReleaseDC(hwnd, hdc);
		DeleteObject(hrgn);
	}
}

void  Ctrl::WndScrollView(const Rect& r, int dx, int dy)
{
	GuiLock __;
	LLOG("WndScrollView " << UPP::Name(this));
	if(caretCtrl && caretCtrl->GetTopCtrl() == this)
		RefreshCaret();
#ifdef PLATFORM_WINCE
	::ScrollWindowEx(GetHWND(), dx, dy, r, r, NULL, NULL, 0);
#else
	::ScrollWindow(GetHWND(), dx, dy, r, r);
#endif
	SyncCaret();
}

void Ctrl::PopUpHWND(HWND owner, bool savebits, bool activate, bool dropshadow, bool topmost)
{
	LLOG("PopUpHWND " << UPP::Name(this) << ", owner: " << owner << ", activate: " << activate);
	popup = false;
	Create(owner, WS_POPUP, topmost ? WS_EX_TOPMOST : 0, savebits,
	       owner || !activate ? SW_SHOWNOACTIVATE : SW_SHOW,
	       dropshadow);
	HWND hwnd = GetHWND();
	if(hwnd) popup = true;
	if(activate) SetFocus();
}

void Ctrl::PopUp(Ctrl *owner, bool savebits, bool activate, bool dropshadow, bool topmost)
{
	popup = false;
	Ctrl *q = owner ? owner->GetTopCtrl() : GetActiveCtrl();
	PopUpHWND(q ? q->GetHWND() : NULL, savebits, activate, dropshadow, topmost);
	Top *top = GetTop();
	if(top) top->owner = owner;
}

Rect Ctrl::GetScreenClient(HWND hwnd)
{
	Rect r;
	::GetClientRect(hwnd, r);
	Point tl = r.TopLeft();
	Point br = r.BottomRight();
	::ClientToScreen(hwnd, tl);
	::ClientToScreen(hwnd, br);
	LLOG("Ctrl::GetScreenClient: hwnd = " << FormatPtr(hwnd) << ", client = " << r
	     << ", screen(tl) = " << tl << ", screen(br) = " << br);
	return Rect(tl, br);
}

Rect Ctrl::GetDefaultWindowRect() {
	HWND hwnd = ::CreateWindow("UPP-CLASS-W", "", WS_OVERLAPPED,
		                       CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
							   NULL, NULL, NULL, NULL);
	Rect sr;
	if(hwnd) {
		::ShowWindow(hwnd, SW_HIDE);
		sr = GetScreenClient(hwnd);
		::DestroyWindow(hwnd);
	}
	else
		sr = RectC(20, 20, 500, 350);
	return sr;
}

TopFrameDraw::TopFrameDraw(Ctrl *ctrl, const Rect& r)
{
	EnterGuiMutex();
	Ctrl *top = ctrl->GetTopCtrl();
	hwnd = top->GetHWND();
	ASSERT(hwnd);
	Attach(GetDC(hwnd));
	Clipoff(r);
}

TopFrameDraw::~TopFrameDraw()
{
	End();
	HDC hdc = Detach();
	if(hwnd && hdc)
		ReleaseDC(hwnd, hdc);
	LeaveGuiMutex();
}

Vector<WString> SplitCmdLine__(const char *cmd)
{
	Vector<WString> out;
	while(*cmd)
		if((byte)*cmd <= ' ')
			cmd++;
		else if(*cmd == '\"') {
			WString quoted;
			while(*++cmd && (*cmd != '\"' || *++cmd == '\"'))
				quoted.Cat(FromSystemCharset(String(cmd, 1)).ToWString());
			out.Add(quoted);
		}
		else {
			const char *begin = cmd;
			while((byte)*cmd > ' ')
				cmd++;
			out.Add(String(begin, cmd).ToWString());
		}
	return out;
}

}

#endif
