#include "CtrlCore.h"

#ifdef GUI_WIN

namespace Upp {

#define LLOG(x) // DLOG(x)

#if defined(COMPILER_MINGW) && !defined(FLASHW_ALL)
	// MINGW headers don't include this in (some versions of) windows
	extern "C"{
		struct FLASHWINFO {
			UINT  cbSize;
			HWND  hwnd;
			DWORD dwFlags;
			UINT  uCount;
			DWORD dwTimeout;
		};
		WINUSERAPI BOOL WINAPI FlashWindowEx(FLASHWINFO*);
	}
	#define FLASHW_STOP         0
	#define FLASHW_CAPTION      0x00000001
	#define FLASHW_TRAY         0x00000002
	#define FLASHW_ALL          (FLASHW_CAPTION | FLASHW_TRAY)
	#define FLASHW_TIMER        0x00000004
	#define FLASHW_TIMERNOFG    0x0000000C
#endif

void    TopWindow::SyncSizeHints() {}

LRESULT TopWindow::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	GuiLock __;
	HWND hwnd = GetHWND();
#ifndef PLATFORM_WINCE
	bool inloop;
#endif
	switch(message) {
	case WM_QUERYENDSESSION:
		inloop = InLoop();
		WhenClose();
		return inloop ? !InLoop() : !IsOpen();
	case WM_CLOSE:
		if(IsEnabled()) {
			IgnoreMouseUp();
			WhenClose();
		}
		return 0;
	case WM_DESTROY:
		if(overlapped.GetWidth() && overlapped.GetHeight())
			SetRect(overlapped);
		break;
	case WM_WINDOWPOSCHANGED:
		if(!isopen)
			break;
		if(IsIconic(hwnd))
			state = MINIMIZED;
		else
		if(IsZoomed(hwnd))
			state = MAXIMIZED;
		else {
			state = OVERLAPPED;
			if(IsWindowVisible(hwnd))
				overlapped = GetScreenClient(hwnd);
		}
		LLOG("TopWindow::WindowProc::WM_WINDOWPOSCHANGED: overlapped = " << overlapped);
		Layout();
		break;
	}
	return Ctrl::WindowProc(message, wParam, lParam);
}

void TopWindow::SyncTitle()
{
	GuiLock __;
	LLOG("TopWindow::SyncTitle0 " << UPP::Name(this));
	HWND hwnd = GetHWND();
	if(hwnd)
		::SetWindowTextW(hwnd, ToSystemCharsetW(title));
}

void TopWindow::DeleteIco()
{
	GuiLock __;
	LLOG("TopWindow::DeleteIco " << UPP::Name(this));
	if(ico)
		DestroyIcon(ico);
	if(lico)
		DestroyIcon(lico);
	ico = lico = NULL;
}

String WindowStyleAsString(dword style, dword exstyle)
{
	String r1;
#define DO(x) if(style & x) { if(r1.GetCount()) r1 << "|"; r1 << #x; }
	DO(WS_OVERLAPPED)
	DO(WS_POPUP)
	DO(WS_CHILD)
	DO(WS_MINIMIZE)
	DO(WS_VISIBLE)
	DO(WS_DISABLED)
	DO(WS_CLIPSIBLINGS)
	DO(WS_CLIPCHILDREN)
	DO(WS_MAXIMIZE)
	DO(WS_CAPTION)
	DO(WS_BORDER)
	DO(WS_DLGFRAME)
	DO(WS_VSCROLL)
	DO(WS_HSCROLL)
	DO(WS_SYSMENU)
	DO(WS_THICKFRAME)
	DO(WS_GROUP)
	DO(WS_TABSTOP)
	DO(WS_MINIMIZEBOX)
	DO(WS_MAXIMIZEBOX)
#undef DO

	String r2;
#define DO(x) if(exstyle & x) { if(r2.GetCount()) r2 << "|"; r2 << #x; }
	DO(WS_EX_DLGMODALFRAME)
	DO(WS_EX_NOPARENTNOTIFY)
	DO(WS_EX_TOPMOST)
	DO(WS_EX_ACCEPTFILES)
	DO(WS_EX_TRANSPARENT)
	DO(WS_EX_MDICHILD)
	DO(WS_EX_TOOLWINDOW)
	DO(WS_EX_WINDOWEDGE)
	DO(WS_EX_CLIENTEDGE)
	DO(WS_EX_CONTEXTHELP)
	DO(WS_EX_RIGHT)
	DO(WS_EX_LEFT)
	DO(WS_EX_RTLREADING)
	DO(WS_EX_LTRREADING)
	DO(WS_EX_LEFTSCROLLBAR)
	DO(WS_EX_RIGHTSCROLLBAR)
	DO(WS_EX_CONTROLPARENT)
	DO(WS_EX_STATICEDGE)
	DO(WS_EX_APPWINDOW)
#undef DO

	return r1 + ' ' + r2;
}

void TopWindow::SyncCaption()
{
	GuiLock __;
	LLOG("TopWindow::SyncCaption " << UPP::Name(this));
	if(fullscreen)
		return;
	HWND hwnd = GetHWND();
	if(hwnd) {
		style = ::GetWindowLong(hwnd, GWL_STYLE);
		exstyle = ::GetWindowLong(hwnd, GWL_EXSTYLE);
	}
	else style = exstyle = 0;
	style &= ~(WS_THICKFRAME|WS_MINIMIZEBOX|WS_MAXIMIZEBOX|WS_SYSMENU|WS_POPUP|WS_DLGFRAME);
	exstyle &= ~(WS_EX_TOOLWINDOW|WS_EX_DLGMODALFRAME);
	style |= WS_CAPTION|WS_CLIPSIBLINGS|WS_CLIPCHILDREN;
	if(minimizebox && !GetOwner())
		style |= WS_MINIMIZEBOX;
	if(maximizebox)
		style |= WS_MAXIMIZEBOX;
	if(sizeable)
		style |= WS_THICKFRAME;
	if(frameless)
		style = (style & ~WS_CAPTION) | WS_POPUP;
	else
	if(!maximizebox && !minimizebox || noclosebox) { // icon ignored because of FixIcons
		style |= WS_POPUPWINDOW|WS_DLGFRAME;
		exstyle |= WS_EX_DLGMODALFRAME;
		if(noclosebox)
			style &= ~WS_SYSMENU;
	}
	else
		style |= WS_SYSMENU;
	if(tool)
		exstyle |= WS_EX_TOOLWINDOW;
	if(fullscreen)
		style = WS_POPUP;
	if(hwnd) {
		::SetWindowLong(hwnd, GWL_STYLE, style);
		::SetWindowLong(hwnd, GWL_EXSTYLE, exstyle);
		SyncTitle();
		if(urgent) {
			if(IsForeground()) urgent = false;
			FLASHWINFO fi;
			memset(&fi, 0, sizeof(fi));
			fi.cbSize = sizeof(fi);
			fi.hwnd = hwnd;
			fi.dwFlags = urgent ? FLASHW_TIMER|FLASHW_ALL : FLASHW_STOP;
			FlashWindowEx(&fi);
		}
	}
	DeleteIco();

	if(hwnd) {
		::SendMessage(hwnd, WM_SETICON, false, (LPARAM)(ico = SystemDraw::IconWin32(icon)));
		::SendMessage(hwnd, WM_SETICON, true, (LPARAM)(lico = SystemDraw::IconWin32(largeicon)));
	}
}

void TopWindow::CenterRect(HWND hwnd, int center)
{
	GuiLock __;
	SetupRect(CtrlFromHWND(hwnd));
	if(hwnd && center == 1 || center == 2) {
		Size sz = GetRect().Size();
		Rect frmrc(sz);
	#ifndef PLATFORM_WINCE
		::AdjustWindowRect(frmrc, WS_OVERLAPPEDWINDOW, FALSE);
	#endif
		Rect r, wr;
		wr = Ctrl::GetWorkArea().Deflated(-frmrc.left, -frmrc.top,
			frmrc.right - sz.cx, frmrc.bottom - sz.cy);
		sz.cx = min(sz.cx, wr.Width());
		sz.cy = min(sz.cy, wr.Height());
		if(center == 1) {
			::GetClientRect(hwnd, r);
			if(r.IsEmpty())
				r = wr;
			else {
				Point p = r.TopLeft();
				::ClientToScreen(hwnd, p);
				r.Offset(p);
			}
		}
		else
			r = wr;
		Point p = r.CenterPos(sz);
		if(p.x + sz.cx > wr.right) p.x = wr.right - sz.cx;
		if(p.y + sz.cy > wr.bottom) p.y = wr.bottom - sz.cy;
		if(p.x < wr.left) p.x = wr.left;
		if(p.y < wr.top) p.y = wr.top;
		SetRect(p.x, p.y, sz.cx, sz.cy);
	}
}

static HWND trayHWND__;
HWND   GetTrayHWND__()          { return trayHWND__; }
void   SetTrayHWND__(HWND hwnd) { trayHWND__ = hwnd; }

void TopWindow::Open(HWND hwnd)
{
	GuiLock __;
	if(dokeys && (!GUI_AKD_Conservative() || GetAccessKeysDeep() <= 1))
		DistributeAccessKeys();
	USRLOG("   OPEN " << Desc(this));
	LLOG("TopWindow::Open, owner HWND = " << hwnd << ", Active = " << hwnd);
	IgnoreMouseUp();
	SyncCaption();
	LLOG("WindowStyles: " << WindowStyleAsString(style, exstyle));
#ifdef PLATFORM_WINCE
	if(!GetRect().IsEmpty())
#endif
	if(fullscreen) {
		SetRect(GetScreenRect()); // 12-05-23 Tom changed from GetScreenSize() to GetScreenRect() in order to get full screen on correct display
		Create(hwnd, WS_POPUP, 0, false, SW_SHOWMAXIMIZED, false);
	}
	else {
		CenterRect(hwnd, hwnd && hwnd == GetTrayHWND__() ? center ? 2 : 0 : center);
		Create(hwnd, style, exstyle, false, state == OVERLAPPED ? SW_SHOWNORMAL :
		                                    state == MINIMIZED  ? SW_MINIMIZE :
		                                                          SW_MAXIMIZE, false);
	}
	PlaceFocus();
	SyncCaption();
	FixIcons();
}

void TopWindow::Open(Ctrl *owner)
{
	GuiLock __;
	LLOG("TopWindow::Open(Ctrl) -> " << UPP::Name(owner));
	Open(owner ? owner->GetTopCtrl()->GetHWND() : NULL);
	Top *top = GetTop();
	if(IsOpen() && top)
		top->owner = owner;
}

void TopWindow::Open()
{
	Open(::GetActiveWindow()); // :: needed because of ActiveX controls (to create modal dlgs owned by a HWND)
}

void TopWindow::OpenMain()
{
	Open((HWND) NULL);
}

void TopWindow::Minimize(bool effect)
{
	LLOG("TopWindow::Minimize " << UPP::Name(this));
	state = MINIMIZED;
	if(IsOpen())
#ifdef PLATFORM_WINCE
		::ShowWindow(GetHWND(), SW_MINIMIZE);
#else
		::ShowWindow(GetHWND(), effect ? SW_MINIMIZE : SW_SHOWMINIMIZED);
#endif
}

TopWindow& TopWindow::FullScreen(bool b)
{
	LLOG("TopWindow::FullScreen " << UPP::Name(this));
	fullscreen = b;
	HWND hwnd = GetOwnerHWND();
	bool pinloop = inloop;
	bool open = IsOpen();
	WndDestroy();
	Overlap();
	Rect r = GetRect();
	if(open)
		SetRect(overlapped); // restore to the same screen as before
	else
	if(r.left + r.top == 0)
		SetRect(GetDefaultWindowRect()); // open in primary screen
	Open(hwnd);
	inloop = pinloop;
	return *this;
}

void TopWindow::Maximize(bool effect)
{
	LLOG("TopWindow::Maximize " << UPP::Name(this));
	state = MAXIMIZED;
	if(IsOpen()) {
		::ShowWindow(GetHWND(), effect ? SW_MAXIMIZE : SW_SHOWMAXIMIZED);
		SyncCaption();
	}
}

void TopWindow::Overlap(bool effect)
{
	GuiLock __;
	LLOG("TopWindow::Overlap " << UPP::Name(this));
	state = OVERLAPPED;
	if(IsOpen()) {
		::ShowWindow(GetHWND(), effect ? SW_SHOWNORMAL : SW_RESTORE);
		SyncCaption();
	}
}

TopWindow& TopWindow::Style(dword _style)
{
	GuiLock __;
	LLOG("TopWindow::Style " << UPP::Name(this));
	style = _style;
	if(GetHWND())
		::SetWindowLong(GetHWND(), GWL_STYLE, style);
	SyncCaption();
	return *this;
}

TopWindow& TopWindow::ExStyle(dword _exstyle)
{
	GuiLock __;
	LLOG("TopWindow::ExStyle " << UPP::Name(this));
	exstyle = _exstyle;
	if(GetHWND())
		::SetWindowLong(GetHWND(), GWL_EXSTYLE, exstyle);
	SyncCaption();
	return *this;
}

TopWindow& TopWindow::TopMost(bool b, bool stay_top)
{
	GuiLock __;
	LLOG("TopWindow::TopMost " << UPP::Name(this));
	HWND hwnd = GetHWND();
	if(hwnd)
		SetWindowPos(hwnd, b ? HWND_TOPMOST : (stay_top ? HWND_NOTOPMOST : HWND_BOTTOM),
		             0,0,0,0,SWP_NOMOVE|SWP_NOSIZE );
	return ExStyle(b ? GetExStyle() | WS_EX_TOPMOST : GetExStyle() & ~WS_EX_TOPMOST);
}

bool TopWindow::IsTopMost() const
{
	return GetExStyle() & WS_EX_TOPMOST;
}

void TopWindow::GuiPlatformConstruct()
{
	style = 0;
	exstyle = 0;
	ico = lico = NULL;
}

void TopWindow::GuiPlatformDestruct()
{
	DeleteIco();
}

void TopWindow::SerializePlacement(Stream& s, bool reminimize)
{
	GuiLock __;
	int version = 1;
	s / version;
	Rect rect = GetRect();
	s % overlapped % rect;
	bool mn = state == MINIMIZED;
	bool mx = state == MAXIMIZED;
	bool fs = fullscreen;
	if(version >= 1)
		s.Pack(mn, mx, fs);
	else
		s.Pack(mn, mx);
	LLOG(Name(this) << "::SerializePlacement / " << (s.IsStoring() ? "write" : "read"));
	LLOG("minimized = " << mn << ", maximized = " << mx << ", fullscreen = " << fs);
	LLOG("rect = " << rect << ", overlapped = " << overlapped);
	if(s.IsLoading()) {
		rect = overlapped;
		Rect limit = GetVirtualWorkArea();
		Rect outer = rect;
		::AdjustWindowRect(outer, WS_OVERLAPPEDWINDOW, FALSE);
		limit.left   += rect.left   - outer.left;
		limit.top    += rect.top    - outer.top;
		limit.right  += rect.right  - outer.right;
		limit.bottom += rect.bottom - outer.bottom;
		Size sz = min(rect.Size(), limit.Size());
		rect = RectC(
			minmax(rect.left, limit.left, limit.right - sz.cx),
			minmax(rect.top,  limit.top,  limit.bottom - sz.cy),
			sz.cx, sz.cy);

		Overlap();
		SetRect(rect);
		
		if(mn && reminimize)
			state = MINIMIZED;
		if(mx)
			state = MAXIMIZED;
		if(min(sz.cx, sz.cy) < 50 && mn && !reminimize)
			state = MAXIMIZED; // Minimized tends to have invalid size, somewhat ugly patch here
		if(IsOpen()) {
			switch(state) {
			case MINIMIZED:
				Minimize();
				break;
			case MAXIMIZED:
				Maximize();
				break;
			}
			if(fs) {
				Overlap(); // Needed to restore normal position before fullscreen mode
				FullScreen();
			}
		}
		else
			fullscreen = fs;
	}
}

}

#endif
