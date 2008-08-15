#include "Install.h"
#include <commctrl.h>

//////////////////////////////////////////////////////////////////////

ProgressMeter::ProgressMeter(int chunk, const char *text, int total, int flags, HWND parent)
: chunk(chunk)
, text(text)
, total(total)
, parent(parent)
, flags(flags)
, window(0)
, pos(0)
, cancel(false)
, caption("Working...")
, next_ticks(0)
{
}

//////////////////////////////////////////////////////////////////////

ProgressMeter::ProgressMeter(const char *title, int step, int total, int flags, HWND parent)
: chunk(step)
, text(title)
, total(total)
, parent(parent)
, flags(flags)
, window(0)
, pos(0)
, cancel(false)
, caption("Working...")
, next_ticks(0)
{
}

//////////////////////////////////////////////////////////////////////

ProgressMeter::~ProgressMeter()
{
	Destroy();
}

//////////////////////////////////////////////////////////////////////

LRESULT CALLBACK ProgressMeter::StaticProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if(message == WM_CREATE)
	{
		CREATESTRUCT *pp = (CREATESTRUCT *)lParam;
		ASSERT(pp -> lpCreateParams);
		SetWindowLong(hwnd, GWL_USERDATA, (long)pp -> lpCreateParams);
		((ProgressMeter *)pp -> lpCreateParams) -> window = hwnd;
	}
	ProgressMeter *pp = (ProgressMeter *)GetWindowLong(hwnd, GWL_USERDATA);
	if(pp)
		return pp -> Proc(message, wParam, lParam);
	else
		return DefWindowProc(hwnd, message, wParam, lParam);
}

//////////////////////////////////////////////////////////////////////

LRESULT ProgressMeter::Proc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_CREATE:
		cancel = false;
		break;

	case WM_KEYDOWN:
		if(IsAllowCancel() && wParam == VK_ESCAPE)
		{
			cancel = true;
			Destroy();
			return 0;
		}
		break;

	case WM_COMMAND:
		if(IsAllowCancel() && LOWORD(wParam) == IDCANCEL)
		{
			cancel = true;
			Destroy();
		}
		return 0;

	case WM_CLOSE:
		if(IsAllowCancel())
		{
			cancel = true;
			Destroy();
		}
		return 0;
	}
	return DefWindowProc(window, message, wParam, lParam);
}

//////////////////////////////////////////////////////////////////////

ProgressMeter& ProgressMeter::Caption(const char *_caption)
{
	if(caption != _caption)
	{
		caption = _caption;
		if(IsOpen())
			SetWindowText(window, caption);
	}
	return *this;
}

//////////////////////////////////////////////////////////////////////

bool ProgressMeter::Create()
{
	if(window)
		return true;

	if(cancel)
		return false;

//	if(AfxGetApp())
//		AfxGetApp() -> BeginWaitCursor();

	HINSTANCE instance = AppGetHandle();
	dword process = GetCurrentProcessId();

	static const char classname[] = "Cybex_Progress";
	static bool done = false;
	if(!done)
	{
		done = true;
		WNDCLASS wndclass =
		{
			CS_DBLCLKS | CS_SAVEBITS,        // UINT    style;
			StaticProc,                      // WNDPROC lpfnWndProc;
			0,                               // int     cbClsExtra;
			0,                               // int     cbWndExtra;
			instance,                        // HANDLE  hInstance;
			0,                               // HICON   hIcon;
			LoadCursor(0, IDC_WAIT),         // HCURSOR hCursor;
			GetSysColorBrush(COLOR_BTNFACE), // HBRUSH  hbrBackground;
			NULL,                            // LPCTSTR lpszMenuName;
			classname,                       // LPCTSTR lpszClassName;
		};
		RegisterClass(&wndclass);
	}

	enum
	{
		CXWND = 240,
		CYWND = 70,

		CXGAP = 5,
		CYGAP = 5,

		XPROGRESS  = CXGAP,
		CXPROGRESS = CXWND - 2 * XPROGRESS,
		CYPROGRESS = 10,
		YPROGRESS  = (CYWND - CYPROGRESS - 10) >> 1,

		XTEXT  = CXGAP,
		YTEXT  = CYGAP,
		CXTEXT = CXWND - 2 * XTEXT,
		CYTEXT = YPROGRESS - YTEXT,

		CXBUTTON = 60,
		CYBUTTON = 22,
		XBUTTON  = (CXWND - CXBUTTON) >> 1,
		YBUTTON  = CYWND - CYGAP - CYBUTTON,

		STYLE = WS_OVERLAPPED | WS_DLGFRAME | WS_CAPTION | WS_SYSMENU | WS_CLIPCHILDREN,
	};

	Size screen(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));

	if(parent)
		while(GetParent(parent))
			parent = GetParent(parent);

	if(!parent)
	{
		if(parent = GetActiveWindow())
			while(GetParent(parent))
				parent = GetParent(parent);
		if(parent && !(IsWindowVisible(parent) && IsWindowEnabled(parent)))
			parent = 0;
	}

	if(parent)
		enable.Add(parent);

//*
	for(HWND hwnd = GetWindow(GetDesktopWindow(), GW_CHILD); hwnd; hwnd = GetWindow(hwnd, GW_HWNDNEXT))
	{
		dword wprocess;
		GetWindowThreadProcessId(hwnd, &wprocess);
		if(wprocess == process && IsWindowVisible(hwnd) && IsWindowEnabled(hwnd))
			enable.Add(hwnd);
	}

	if(!parent && !enable.IsEmpty())
		parent = enable[0];
//*/

	if(!parent)
		parent = GetDesktopWindow();

	Rect pane(0, 0, CXWND, CYWND);
	AdjustWindowRect(pane, STYLE, false);

	pane.Offset((screen >> 1) - pane.CenterPoint());

	CreateWindowEx(
		WS_EX_DLGMODALFRAME | WS_EX_WINDOWEDGE,
		classname, caption,
		STYLE,
		pane.left, pane.top, pane.Width(), pane.Height(),
		parent, 0, instance, this);

	if(!window)
		return false;

	SendMessage(window, WM_SETICON, ICON_BIG, 0);
	SendMessage(window, WM_SETICON, ICON_SMALL, 0);

	VERIFY(CreateWindow("static", NULL,
		WS_CHILD | WS_VISIBLE | SS_CENTER | SS_NOPREFIX,
		XTEXT, YTEXT, CXTEXT, CYTEXT,
		window, (HMENU)IDOK, instance, 0));

//	INITCOMMONCONTROLSEX icc;
//	icc.dwSize = sizeof(icc);
//	icc.dwICC = ICC_PROGRESS_CLASS;
//	InitCommonControlsEx(&icc);

	InitCommonControls();

	VERIFY(progress = CreateWindow("msctls_progress32", NULL,
		WS_CHILD | WS_DISABLED | (total ? WS_VISIBLE : 0),
		XPROGRESS, YPROGRESS, CXPROGRESS, CYPROGRESS,
		window, 0, instance, 0));

	VERIFY(CreateWindow("button", "Cancel",
		(flags & NOCANCEL ? 0 : WS_VISIBLE) | WS_CHILD | BS_PUSHBUTTON,
		XBUTTON, YBUTTON, CXBUTTON, CYBUTTON,
		window, (HMENU)IDCANCEL, instance, 0));

	HFONT font = (HFONT)GetStockObject(ANSI_VAR_FONT);
	SendDlgItemMessage(window, IDOK, WM_SETFONT, (WPARAM)font, true);
	SendDlgItemMessage(window, IDCANCEL, WM_SETFONT, (WPARAM)font, true);

	ShowWindow(window, SW_SHOW);
	Enable(false);

	old_text = Null;
	Set(pos, total);

	return true;
}

//////////////////////////////////////////////////////////////////////

ProgressMeter& ProgressMeter::SetText(const char *s, bool reset)
{
	if(reset)
		Reset();
	text = s;
	return Set(pos, total);
}

//////////////////////////////////////////////////////////////////////

ProgressMeter& ProgressMeter::Set(int apos, int atotal)
{
	if(cancel && flags & THROW)
		throw AbortExc();

	if(atotal >= 0)
		total = atotal;
	if(apos >= 0)
		pos = apos;
	if(IgnoreStep(apos))
		return *this;
	next_ticks = GetTickCount() + 500;
	if(!Create())
		return *this;

	String new_text = Format(text, pos);
	if(new_text.Compare(old_text))
		SetDlgItemText(window, IDOK, old_text = new_text);

	atotal = total;
	apos = pos;
	while(atotal > 30000)
	{
		atotal >>= 8;
		apos >>= 8;
	}

	ShowWindow(progress, atotal > 0 ? SW_SHOW : SW_HIDE);
	if(atotal > 0)
	{
		SendMessage(progress, PBM_SETRANGE, 0, MAKELPARAM(0, atotal));
		SendMessage(progress, PBM_SETPOS, apos, 0);
	}

	return *this;
}

//////////////////////////////////////////////////////////////////////

ProgressMeter& ProgressMeter::Step()
{
	return Set(++pos, total);
}

//////////////////////////////////////////////////////////////////////

ProgressMeter& ProgressMeter::SetRange(int atotal)
{
	return Set(pos, atotal);
}

//////////////////////////////////////////////////////////////////////

ProgressMeter& ProgressMeter::SetPos(int apos)
{
	return Set(apos, total);
}

//////////////////////////////////////////////////////////////////////

bool ProgressMeter::Cancelled()
{
	Create();
	MSG msg;
	while(::PeekMessage(&msg, flags & FILTER ? window : 0, 0, 0, PM_REMOVE))
		::DispatchMessage(&msg);
	if(cancel && flags & THROW)
		throw AbortExc();
	return cancel;
}

//////////////////////////////////////////////////////////////////////

bool ProgressMeter::StepCancelled()
{
	SetPos(pos + 1);
	return IgnoreStep(pos) ? cancel : Cancelled();
}

//////////////////////////////////////////////////////////////////////

bool ProgressMeter::SetPosCancelled(int apos)
{
	SetPos(apos);
	return IgnoreStep(apos) ? cancel : Cancelled();
}

//////////////////////////////////////////////////////////////////////

bool ProgressMeter::IgnoreStep(int pos) const
{
	return (int)GetTickCount() < next_ticks && pos % (chunk ? chunk : 1);
}

//////////////////////////////////////////////////////////////////////

bool ProgressMeter::OnStep(int apos, int atotal)
{
	if(atotal >= 0)
		total = atotal;
	SetPos(apos);
	return IgnoreStep(apos) ? cancel : Cancelled();
}

//////////////////////////////////////////////////////////////////////

void ProgressMeter::Destroy()
{
	if(window)
	{
		Enable(true);
		if(!enable.IsEmpty())
			SetActiveWindow(enable[0]);
		enable.Clear();
		DestroyWindow(window);
		window = NULL;
		progress = NULL;
//		if(AfxGetApp())
//			AfxGetApp() -> EndWaitCursor();
	}
}

//////////////////////////////////////////////////////////////////////

void ProgressMeter::Enable(bool en)
{
	for(const HWND *p = enable.Begin(), *e = enable.End(); p < e; p++)
		EnableWindow(*p, en);
}

//////////////////////////////////////////////////////////////////////

ProgressMeter& ProgressMeter::Filter(bool _fm)
{
	flags = _fm ? flags | FILTER : flags & ~FILTER;
	return *this;
}

//////////////////////////////////////////////////////////////////////

ProgressMeter& ProgressMeter::Throw(bool _throw)
{
	flags = _throw ? flags | THROW : flags & ~THROW;
	return *this;
}

//////////////////////////////////////////////////////////////////////

ProgressMeter& ProgressMeter::AllowCancel(bool _ac)
{
	flags = _ac ? flags & ~NOCANCEL : flags | NOCANCEL;
	if(window)
		ShowWindow(GetDlgItem(window, IDCANCEL), _ac ? SW_SHOW : SW_HIDE);
	return *this;
}

//////////////////////////////////////////////////////////////////////

ProgressMeter& ProgressMeter::Show(bool _show)
{
	flags = _show ? flags & ~HIDDEN : flags | HIDDEN;
	if(window)
		ShowWindow(window, _show ? SW_SHOW : SW_HIDE);
	return *this;
}

//////////////////////////////////////////////////////////////////////
