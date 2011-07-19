#include <CtrlCore/CtrlCore.h>

#ifdef GUI_WINALT

NAMESPACE_UPP

#define LLOG(x)   //   LOG(x)
#define LOGTIMING 0

#ifdef _DEBUG
#define LOGMESSAGES 0
#endif

#define ELOGW(x)  // RLOG(GetSysTime() << ": " << x) // Only activate in MT!
#define ELOG(x)   // RLOG(GetSysTime() << ": " << x)

template<>
unsigned GetHashValue(const HWND& h)
{
	return (unsigned)(intptr_t)h;
}

bool Ctrl::PeekMsg(MSG& msg)
{
	if(!PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) return false;
	return IsWindowUnicode(msg.hwnd) ? PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)
	                                 : PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
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
#ifndef PLATFORM_WINCE
	LLOG("DumpWindowOrder" << BeginIndent);
	EnumChildWindows(NULL, &sDumpWindow, (LPARAM)(aliens ? 1 : 0));
	LLOG(EndIndent << "//DumpWindowOrder");
#endif
}

#ifndef PLATFORM_WINCE
Point GetMousePos() {
	Point p;
	return ::GetCursorPos(p) ? p : Null;
	::GetCursorPos(p);
	return p;
}
#endif

HCURSOR   Ctrl::hCursor;
HINSTANCE Ctrl::hInstance;
#ifndef flagDLL
#ifndef PLATFORM_WINCE
HANDLE    Ctrl::OverwatchThread;
HWND      Ctrl::OverwatchHWND;

Event Ctrl::OverwatchEndSession;
Event Ctrl::ExitLoopEvent;
#endif
#endif

bool Ctrl::endSession = false;

#ifndef flagDLL
#ifndef PLATFORM_WINCE
LRESULT CALLBACK Ctrl::OverwatchWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if(msg == WM_USER) {
		ELOGW("WM_USER");
		PostQuitMessage(0);
	}
	if(msg == WM_ENDSESSION) {
		EndSession();
		ELOGW("WM_ENDSESSION 1");
		ExitLoopEvent.Set();
		ELOGW("WM_ENDSESSION 2");
		OverwatchEndSession.Wait();
		ELOGW("WM_ENDSESSION 3");
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
		if(IsWindowUnicode(Msg.hwnd))
			DispatchMessageW(&Msg);
		else
			DispatchMessage(&Msg);
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

static void Win32PanicMessageBox(const char *title, const char *text)
{
#ifdef PLATFORM_WINCE
	static wchar wtext[256], wtitle[256];
	ToUnicode(wtext, text, strlen(text), CHARSET_DEFAULT);
	ToUnicode(wtitle, title, strlen(title), CHARSET_DEFAULT);
	MessageBox(::GetActiveWindow(), wtext, wtitle, MB_ICONSTOP | MB_OK | MB_APPLMODAL);
#else
	MessageBox(::GetActiveWindow(), text, title, MB_ICONSTOP | MB_OK | MB_APPLMODAL);
#endif
}

void Ctrl::InitWin32(HINSTANCE hInstance)
{
	GuiLock __;
	LLOG("InitWin32");
	InstallPanicMessageBox(&Win32PanicMessageBox);
//	RLOGBLOCK("Ctrl::InitWin32");
	sMainThreadId = GetCurrentThreadId();
#define ILOG(x) // RLOG(x)
	Ctrl::hInstance = hInstance;
	ILOG("RegisterClassW");
#ifndef PLATFORM_WINCE
	if(IsWinNT())
#endif
	{
		WNDCLASSW  wc;
		Zero(wc);
		wc.style         = CS_DBLCLKS|CS_HREDRAW|CS_VREDRAW;
		wc.lpfnWndProc   = (WNDPROC)Ctrl::WndProc;
		wc.hInstance     = hInstance;
		wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = IsWinVista() ? (HBRUSH)(COLOR_WINDOW+1) : (HBRUSH)NULL;
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

	ILOG("RegisterClassA");
	WNDCLASS  wc;
	Zero(wc);
	wc.style         = CS_DBLCLKS|CS_HREDRAW|CS_VREDRAW;
	wc.lpfnWndProc   = (WNDPROC)Ctrl::WndProc;
	wc.hInstance     = hInstance;
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = IsWinVista() ? (HBRUSH)(COLOR_WINDOW+1) : (HBRUSH)NULL;
	wc.lpszClassName = L_("UPP-CLASS-A");
	RegisterClass(&wc);
	if(IsWinXP()) {
		wc.style         = 0x20000|CS_DBLCLKS|CS_HREDRAW|CS_VREDRAW;
		wc.lpszClassName = L_("UPP-CLASS-DS-A");
		RegisterClass(&wc);
	}
	wc.style         = CS_SAVEBITS|CS_DBLCLKS|CS_HREDRAW|CS_VREDRAW;
	wc.lpszClassName = L_("UPP-CLASS-SB-A");
	RegisterClass(&wc);
	if(IsWinXP()) {
		wc.style         = 0x20000|CS_DBLCLKS|CS_HREDRAW|CS_VREDRAW|CS_SAVEBITS;
		wc.lpszClassName = L_("UPP-CLASS-SB-DS-A");
		RegisterClass(&wc);
	}
	wc.style         = 0;
	wc.lpszClassName = L_("UPP-TIMER");
	wc.hCursor       = NULL;
	wc.lpfnWndProc   = &Ctrl::UtilityProc;
	RegisterClass(&wc);

	ILOG("InitTimer");
	InitTimer();
	ILOG("SetTimer");
	utilityHWND = CreateWindow(L_("UPP-TIMER"), L_(""), WS_OVERLAPPED,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, hInstance, NULL);
	SetTimer(utilityHWND, 1, 10, NULL);
	ILOG("Windows");
	Windows(); //?? TRC: what's the use of this?

	ChSync();

	OleInitialize(NULL);

/* TRC 05/11/14: moved to GuiSleep to avoid thread creation in OCX DllMain
	DWORD dummy;
	OverwatchThread = CreateThread(NULL, 0x100000, Win32OverwatchThread, NULL, 0, &dummy);
	ExitLoopEvent().Wait();
*/

// TRC 05/11/18: pSetLayeredWindowAttributes moved to GLOBAL_VAR (see below) to make OCX initialization simpler

	Csizeinit();
#undef ILOG

	if(IsWin7())
		GlobalBackPaint();
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
	while(PeekMsg(msg))
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
		if(w.GetKey(i) && w[i] && !w[i]->parent)
			v.Add(w[i]);
	return v;
}

void  Ctrl::SetMouseCursor(const Image& image)
{
	GuiLock __;
#ifndef PLATFORM_WINCE
	static Image img;
	if(image.GetSerialId() != img.GetSerialId()) {
		img = image;
		HCURSOR hc = IconWin32(img, true);
		SetCursor(hc);
		if(hCursor)
			DestroyCursor(hCursor);
		hCursor = hc;
	}
#endif
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

struct Ctrl::CreateBox {
	HWND  parent;
	DWORD style;
	DWORD exstyle;
	bool  savebits;
	int   show;
	bool  dropshadow;
};

void Ctrl::Create(HWND parent, DWORD style, DWORD exstyle, bool savebits, int show, bool dropshadow)
{
	CreateBox cr;
	cr.parent = parent;
	cr.style = style;
	cr.exstyle = exstyle;
	cr.savebits = savebits;
	cr.show = show;
	cr.dropshadow = dropshadow;
	ICall(callback1(this, &Ctrl::Create0, &cr));
}

#if 0
void Ctrl::Create0(Ctrl::CreateBox *cr)
{
	GuiLock __;
	ASSERT(IsMainThread());
	LLOG("Ctrl::Create(parent = " << (void *)parent << ") in " <<UPP::Name(this) << BeginIndent);
	ASSERT(!IsChild() && !IsOpen());
	Rect r = GetRect();
	AdjustWindowRectEx(r, cr->style, FALSE, cr->exstyle);
	isopen = true;
	top = new Top;
	ASSERT(!cr->parent || IsWindow(cr->parent));
	if(!IsWinXP())
		cr->dropshadow = false;
#ifdef PLATFORM_WINCE
		if(parent)
			top->hwnd = CreateWindowExW(cr->exstyle,
			                            cr->savebits ? cr->dropshadow ? L"UPP-CLASS-SB-DS-W" : L"UPP-CLASS-SB-W"
			                                         : cr->dropshadow ? L"UPP-CLASS-DS-W"    : L"UPP-CLASS-W",
			                            L"", cr->style, r.left, r.top, r.Width(), r.Height(),
			                            cr->parent, NULL, hInstance, this);
		else
			top->hwnd = CreateWindowW(L"UPP-CLASS-W",
			                          L"", WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
			                          cr->parent, NULL, hInstance, this);
#else
	if(IsWinNT() && (!cr->parent || IsWindowUnicode(cr->parent)))
		top->hwnd = CreateWindowExW(cr->exstyle,
		                            cr->savebits ? cr->dropshadow ? L"UPP-CLASS-SB-DS-W" : L"UPP-CLASS-SB-W"
		                                         : cr->dropshadow ? L"UPP-CLASS-DS-W"    : L"UPP-CLASS-W",
		                            L"", cr->style, r.left, r.top, r.Width(), r.Height(),
		                            cr->parent, NULL, hInstance, this);
	else
		top->hwnd = CreateWindowEx(cr->exstyle,
		                           cr->savebits ? cr->dropshadow ? "UPP-CLASS-SB-DS-A" : "UPP-CLASS-SB-A"
		                                        : cr->dropshadow ? "UPP-CLASS-DS-A"    : "UPP-CLASS-A",
		                           "", cr->style, r.left, r.top, r.Width(), r.Height(),
		                           cr->parent, NULL, hInstance, this);
#endif

	inloop = false;

	ASSERT(top->hwnd);

	::ShowWindow(top->hwnd, visible ? cr->show : SW_HIDE);
//	::UpdateWindow(hwnd);
	StateH(OPEN);
	LLOG(EndIndent << "//Ctrl::Create in " <<UPP::Name(this));
	RegisterDragDrop(top->hwnd, (LPDROPTARGET) (top->dndtgt = NewUDropTarget(this)));
	CancelMode();
	RefreshLayoutDeep();
}
#else
// Fix to avoid black corners temorarily artifact
void Ctrl::Create0(Ctrl::CreateBox *cr)
{
	GuiLock __;
	ASSERT(IsMainThread());
	LLOG("Ctrl::Create(parent = " << (void *)parent << ") in " <<UPP::Name(this) << BeginIndent);
	ASSERT(!IsChild() && !IsOpen());
	Rect r = GetRect();
	AdjustWindowRectEx(r, cr->style, FALSE, cr->exstyle);
	isopen = true;
	top = new Top;
	ASSERT(!cr->parent || IsWindow(cr->parent));
	cr->style &= ~WS_VISIBLE;
	if(!IsWinXP())
		cr->dropshadow = false;
#ifdef PLATFORM_WINCE
		if(parent)
			top->hwnd = CreateWindowExW(cr->exstyle,
			                            cr->savebits ? cr->dropshadow ? L"UPP-CLASS-SB-DS-W" : L"UPP-CLASS-SB-W"
			                                         : cr->dropshadow ? L"UPP-CLASS-DS-W"    : L"UPP-CLASS-W",
			                            L"", cr->style, 0, 0, 0, 0,
			                            cr->parent, NULL, hInstance, this);
		else
			top->hwnd = CreateWindowW(L"UPP-CLASS-W",
			                          L"", WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
			                          cr->parent, NULL, hInstance, this);
#else
	if(IsWinNT() && (!cr->parent || IsWindowUnicode(cr->parent)))
		top->hwnd = CreateWindowExW(cr->exstyle,
		                            cr->savebits ? cr->dropshadow ? L"UPP-CLASS-SB-DS-W" : L"UPP-CLASS-SB-W"
		                                         : cr->dropshadow ? L"UPP-CLASS-DS-W"    : L"UPP-CLASS-W",
		                            L"", cr->style, 0, 0, 0, 0,
		                            cr->parent, NULL, hInstance, this);
	else
		top->hwnd = CreateWindowEx(cr->exstyle,
		                           cr->savebits ? cr->dropshadow ? "UPP-CLASS-SB-DS-A" : "UPP-CLASS-SB-A"
		                                        : cr->dropshadow ? "UPP-CLASS-DS-A"    : "UPP-CLASS-A",
		                           "", cr->style, 0, 0, 0, 0,
		                           cr->parent, NULL, hInstance, this);
#endif

	inloop = false;

	ASSERT(top->hwnd);

	::MoveWindow(top->hwnd, r.left, r.top, r.Width(), r.Height(), false); // To avoid "black corners" artifact effect
	::ShowWindow(top->hwnd, visible ? cr->show : SW_HIDE);
//	::UpdateWindow(hwnd);
	StateH(OPEN);
	LLOG(EndIndent << "//Ctrl::Create in " <<UPP::Name(this));
	RegisterDragDrop(top->hwnd, (LPDROPTARGET) (top->dndtgt = NewUDropTarget(this)));
	CancelMode();
	RefreshLayoutDeep();
}

#endif

void ReleaseUDropTarget(UDropTarget *dt);

void Ctrl::WndFree()
{
	GuiLock __;
	if(!top) return;
	RevokeDragDrop(GetHWND());
	ReleaseUDropTarget(top->dndtgt);
	isopen = false;
	if(!top) return;
	HWND owner = GetWindow(top->hwnd, GW_OWNER);// CXL 31.10.2003 z DoRemove
	bool focus = ::GetFocus() == top->hwnd;
	LLOG("Ctrl::WndDestroy owner " << (void *)owner
	     << " focus " << focus
	     << " ::GetFocus() " << (void *)::GetFocus());
	if(owner && focus) { // CXL 7.11.2003 presun - melo by to fungovat take a neblikat...
		LLOG("Ctrl::WndFree->SetFocus " << UPP::Name(Ctrl::CtrlFromHWND(owner)));
		::SetFocus(owner);
	}
	LLOG(EndIndent << "//Ctrl::WndFree() in " <<UPP::Name(this));
	delete top;
	top = NULL;
}

void Ctrl::WndDestroy0()
{
	GuiLock __;
	LLOG("Ctrl::WndDestroy() in " <<UPP::Name(this) << BeginIndent);
	LLOG((DumpWindowOrder(false), ""));
	if(top && top->hwnd) {
		HWND hwnd = top->hwnd;
		WndFree(); // CXL 2007-06-04 to avoid loosing focus with maximize box owned dialogs
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
#include "WinAltMsg.i"
	{0, NULL}
};

#endif

void Ctrl::NcCreate(HWND hwnd)
{
	GuiLock __;
	if(!parent)
		top->hwnd = hwnd;
}

void Ctrl::NcDestroy()
{
	GuiLock __;
	if(!parent)
		WndFree();
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
#ifndef PLATFORM_WINCE
	   message != WM_NCHITTEST  && message != WM_ENTERIDLE &&
#endif
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
	if(w) {
		try
		{
#if defined(_DEBUG) && LOGTIMING
			int ticks = msecs();
			String wname = w->Name();
#endif
			Ptr<Ctrl> pw = w;
			l = w->WindowProc(message, wParam, lParam);
			if(pw)
				pw->SyncMoves();
#if defined(_DEBUG) && LOGTIMING
			String msgname;
			for(WinMsg *m = sWinMsg; m->ID; m++)
				if(m->ID == message) {
					msgname = m->name;
					break;
				}
			if(IsNull(msgname))
				msgname = NFormat("0x%04x", (int)message);
			LLOG(NFormat("T+%d %s 0x%08x 0x%08x -> %s", msecs(ticks), msgname, (int)wParam, (int)lParam, wname));
#endif
		}
		catch(Exc e)
		{
			LOG("Ctrl::WindowProc -> Exc: " << e);
			NEVER();
		}
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

Vector<Callback> Ctrl::hotkey;

int Ctrl::RegisterSystemHotKey(dword key, Callback cb)
{
	ASSERT(key >= K_DELTA);
	int q = hotkey.GetCount();
	for(int i = 0; i < hotkey.GetCount(); i++)
		if(!hotkey[i]) {
			q = i;
			break;
		}
	hotkey.At(q) = cb;
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
		if(msg.wParam >= 0 && (int)msg.wParam < Ctrl::hotkey.GetCount())
			Ctrl::hotkey[(int)msg.wParam]();
		return;
	}
	if(msg.message != WM_SYSKEYDOWN && msg.message != WM_SYSKEYUP
	|| PassWindowsKey((dword)msg.wParam) || msg.wParam == VK_MENU) //17.11 Mirek - fix to get windows menu invoked on Alt+Space
		TranslateMessage(&msg); // 04/09/07: TRC fix to make barcode reader going better
	if(IsWindowUnicode(msg.hwnd))
		DispatchMessageW(&msg);
	else
		DispatchMessage(&msg);
}

bool Ctrl::IsWaitingEvent()
{
	MSG msg;
	return PeekMsg(msg);
}

bool Ctrl::ProcessEvent(bool *quit)
{
	ASSERT(IsMainThread());
	if(DoCall())
		return false;
	if(IsEndSession())
		return false;
	if(!GetMouseLeft() && !GetMouseRight() && !GetMouseMiddle())
		ReleaseCtrlCapture();
	MSG msg;
	if(PeekMsg(msg)) {
		if(msg.message == WM_QUIT && quit)
			*quit = true;
//		LLOG(GetSysTime() << " % " << (unsigned)msecs() % 10000 << ": sProcessMSG " << FormatIntHex(msg.message));
		sProcessMSG(msg);
//		LLOG(GetSysTime() << " % " << (unsigned)msecs() % 10000 << ": //sProcessMSG " << FormatIntHex(msg.message));
		DefferedFocusSync();
		SyncCaret();
		return true;
	}
	return false;
}

void SweepMkImageCache();

bool Ctrl::ProcessEvents(bool *quit)
{
	if(ProcessEvent(quit)) {
		while(ProcessEvent(quit) && (!LoopCtrl || LoopCtrl->InLoop())); // LoopCtrl-MF 071008
		TimerProc(GetTickCount());
		SweepMkImageCache();
		return true;
	}
	SweepMkImageCache();
	TimerProc(GetTickCount());
	return false;
}

void Ctrl::EventLoop0(Ctrl *ctrl)
{
	GuiLock __;
	ASSERT(IsMainThread());
	ASSERT(LoopLevel == 0 || ctrl);
	LoopLevel++;
	LLOG("Entering event loop at level " << LoopLevel << BeginIndent);
	Ptr<Ctrl> ploop;
	if(ctrl) {
		ploop = LoopCtrl;
		LoopCtrl = ctrl;
		ctrl->inloop = true;
	}

	bool quit = false;
	ProcessEvents(&quit);
	while(!IsEndSession() && !quit && (ctrl ? ctrl->IsOpen() && ctrl->InLoop() : GetTopCtrls().GetCount()))
	{
//		LLOG(GetSysTime() << " % " << (unsigned)msecs() % 10000 << ": EventLoop / GuiSleep");
		SyncCaret();
		GuiSleep(1000);
		if(IsEndSession()) break;
//		LLOG(GetSysTime() << " % " << (unsigned)msecs() % 10000 << ": EventLoop / ProcessEvents");
		ProcessEvents(&quit);
//		LLOG(GetSysTime() << " % " << (unsigned)msecs() % 10000 << ": EventLoop / after ProcessEvents");
	}

	if(ctrl)
		LoopCtrl = ploop;
	LoopLevel--;
	LLOG(EndIndent << "Leaving event loop ");
}

void Ctrl::GuiSleep0(int ms)
{
	GuiLock __;
	ASSERT(IsMainThread());
	ELOG("GuiSleep");
	if(IsEndSession())
		return;
	ELOG("GuiSleep 2");
	int level = LeaveGMutexAll();
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
	EnterGMutex(level);
}

void Ctrl::WndDestroyCaret()
{
	LLOG("Ctrl::WndDestroyCaret()");
	::DestroyCaret();
}

void Ctrl::WndCreateCaret0(const Rect& cr)
{
	GuiLock __;
	LLOG("Ctrl::WndCreateCaret(" << cr << ") in " << UPP::Name(this));
	HWND hwnd = GetHWND();
	if(!hwnd) return;
	Rect r;
	::GetClientRect(hwnd, r);
	Point p = r.TopLeft();
	::ClientToScreen(hwnd, p);
	::CreateCaret(hwnd, NULL, cr.Width(), cr.Height());
	::SetCaretPos(cr.left - p.x, cr.top - p.y);
	::ShowCaret(hwnd);
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

void Ctrl::WndShow0(bool b)
{
	GuiLock __;
	HWND hwnd = GetHWND();
	if(hwnd)
		::ShowWindow(hwnd, b ? SW_SHOW : SW_HIDE);
}

void Ctrl::WndUpdate0()
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
	if(!IsAlphaSupported() || parent || !top || !hwnd)
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

Rect Ctrl::GetWorkArea(Point pt)
{
	Array<Rect> rc;
	GetWorkArea(rc);
	for(int i = 0; i < rc.GetCount(); i++)
		if(rc[i].Contains(pt))
			return rc[i];
	return GetPrimaryWorkArea();
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
#ifdef PLATFORM_WINCE
	return 500;
#else
	int a;
	SystemParametersInfo(SPI_GETKEYBOARDDELAY, 0, &a, 0);
	return 250 + a * 750 / 4;
#endif
}

int Ctrl::GetKbdSpeed()
{
	GuiLock __;
#ifdef PLATFORM_WINCE
	return 1000 / 32;
#else
	int a;
	SystemParametersInfo(SPI_GETKEYBOARDSPEED, 0, &a, 0);
	return 1000 / (a + 2);
#endif
}

void Ctrl::SetWndForeground0()
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

void Ctrl::WndEnable0(bool *b)
{
	GuiLock __;
	LLOG("Ctrl::WndEnable(" << b << ") in " << UPP::Name(this) << ", focusCtrlWnd = " << UPP::Name(focusCtrlWnd) << ", raw = " << (void *)::GetFocus());
	if(*b)
		ReleaseCapture();
	LLOG("//Ctrl::WndEnable(" << b << ") -> false " <<UPP::Name(this) << ", focusCtrlWnd = " <<UPP::Name(focusCtrlWnd) << ", raw = " << (void *)::GetFocus());
}

void Ctrl::SetWndFocus0(bool *b)
{
	GuiLock __;
	LLOG("Ctrl::SetWndFocus() in " << UPP::Name(this));
	HWND hwnd = GetHWND();
	if(hwnd) {
		LLOG("Ctrl::SetWndFocus() -> ::SetFocus(" << (void *)hwnd << ")");
//		::SetActiveWindow(hwnd);
		::SetFocus(hwnd);
		*b = true;
		return;
	}
	LLOG("//Ctrl::SetWndFocus() in " <<UPP::Name(this) << ", active window = " << (void *)::GetActiveWindow());
	*b = false;
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
	ASSERT(IsMainThread());
	HWND hwnd = GetHWND();
	if(hwnd) {
		::SetCapture(hwnd);
		return true;
	}
	return false;
}

bool Ctrl::ReleaseWndCapture()
{
	GuiLock __;
	ASSERT(IsMainThread());
	HWND hwnd = GetHWND();
	if(hwnd && HasWndCapture())
	{
		::ReleaseCapture();
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

void Ctrl::WndInvalidateRect0(const Rect& r)
{
	GuiLock __;
	HWND hwnd = GetHWND();
	if(hwnd)
		::InvalidateRect(hwnd, r, false);
}

void Ctrl::WndSetPos0(const Rect& rect)
{
	GuiLock __;
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

void Ctrl::WndUpdate0r(const Rect& r)
{
	GuiLock __;
	Ctrl *top = GetTopCtrl();
	if(top->IsOpen()) {
		HWND hwnd = top->GetHWND();
		HDC hdc = GetDC(hwnd);
		HRGN hrgn = CreateRectRgn(0, 0, 0, 0);
		if(GetUpdateRgn(hwnd, hrgn, FALSE) != NULLREGION) {
			SelectClipRgn(hdc, hrgn);
			SystemDraw draw(hdc);
			bool hcr = focusCtrl && focusCtrl->GetTopCtrl() == top &&
			           caretRect.Intersects(r + top->GetRect().TopLeft());
			if(hcr) ::HideCaret(hwnd);
			draw.Clip(r);
			top->UpdateArea(draw, r);
			ValidateRect(hwnd, r);
			SelectClipRgn(hdc, NULL);
			if(hcr) ::ShowCaret(hwnd);
		}
		ReleaseDC(hwnd, hdc);
		DeleteObject(hrgn);
	}
}

void  Ctrl::WndScrollView0(const Rect& r, int dx, int dy)
{
	GuiLock __;
	if(caretCtrl && caretCtrl->GetTopCtrl() == this) {
		WndDestroyCaret();
		caretRect.Clear();
	}
#ifdef PLATFORM_WINCE
	::ScrollWindowEx(GetHWND(), dx, dy, r, r, NULL, NULL, 0);
#else
	::ScrollWindow(GetHWND(), dx, dy, r, r);
#endif
	SyncCaret();
}

void Ctrl::PopUpHWND(HWND owner, bool savebits, bool activate, bool dropshadow, bool topmost)
{
	LLOG("POPUP");
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
#ifdef PLATFORM_WINCE
	return Rect(0, 0, 100, 100);
#else
	HWND hwnd = ::CreateWindow("UPP-CLASS-A", "", WS_OVERLAPPED,
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
#endif
}

ViewDraw::ViewDraw(Ctrl *ctrl)
{
	EnterGuiMutex();
	Ctrl *top = ctrl->GetTopCtrl();
	hwnd = top->GetHWND();
	ASSERT(hwnd);
	Attach(GetDC(hwnd));
	Clipoff(ctrl->GetScreenView() - top->GetScreenRect().TopLeft());
}

ViewDraw::~ViewDraw()
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

END_UPP_NAMESPACE

#endif
