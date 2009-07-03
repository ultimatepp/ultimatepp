#include "CtrlCore.h"

NAMESPACE_UPP

#define LLOG(x)  // LOG(x)

#ifdef PLATFORM_WIN32

void    TopWindow::SyncSizeHints() {}

LRESULT TopWindow::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	GuiLock __;
	HWND hwnd = GetHWND();
#ifndef PLATFORM_WINCE
	bool inloop;
#endif
	switch(message) {
#ifndef PLATFORM_WINCE
	case WM_QUERYENDSESSION:
		inloop = InLoop();
		WhenClose();
		return inloop ? !InLoop() : !IsOpen();
	case WM_ENDSESSION:
		EndSession() = true;
		PostQuitMessage(0);
		return 0;
#endif
	case WM_CLOSE:
		if(IsEnabled()) {
			IgnoreMouseUp();
			WhenClose();
		}
		return 0;
	case WM_WINDOWPOSCHANGED:
#ifndef PLATFORM_WINCE
		if(IsIconic(hwnd))
			state = MINIMIZED;
		else
		if(IsZoomed(hwnd))
			state = MAXIMIZED;
		else
#endif
		{
			state = OVERLAPPED;
			overlapped = GetScreenClient(hwnd);
		}
		Layout();
		break;
	}
	return Ctrl::WindowProc(message, wParam, lParam);
}

void TopWindow::SyncTitle0()
{
	GuiLock __;
	HWND hwnd = GetHWND();
#ifndef PLATFORM_WINCE
	if(hwnd)
		if(IsWindowUnicode(hwnd))
			::SetWindowTextW(hwnd, (const WCHAR*)~title);
		else
#endif
			::SetWindowText(hwnd, ToSystemCharset(title.ToString()));
}

void TopWindow::DeleteIco0()
{
	GuiLock __;
	if(ico)
		DestroyIcon(ico);
	if(lico)
		DestroyIcon(lico);
	ico = lico = NULL;
}

void TopWindow::DeleteIco()
{
	Call(THISBACK(DeleteIco0));
}

void TopWindow::SyncCaption0()
{
	GuiLock __;
	LLOG("SyncCaption");
	if(fullscreen)
		return;
	HWND hwnd = GetHWND();
	if(hwnd) {
		style = ::GetWindowLong(hwnd, GWL_STYLE);
		exstyle = ::GetWindowLong(hwnd, GWL_EXSTYLE);
	}
	style &= ~(WS_THICKFRAME|WS_MINIMIZEBOX|WS_MAXIMIZEBOX|WS_SYSMENU|WS_POPUP|WS_DLGFRAME);
	exstyle &= ~(WS_EX_TOOLWINDOW|WS_EX_DLGMODALFRAME);
	style |= WS_CAPTION;
	if(hasdhctrl)
		style |= WS_CLIPSIBLINGS|WS_CLIPCHILDREN;
	if(minimizebox)
		style |= WS_MINIMIZEBOX;
	if(maximizebox)
		style |= WS_MAXIMIZEBOX;
	if(sizeable)
		style |= WS_THICKFRAME;
#ifndef PLATFORM_WINCE
	if(frameless)
		style = (style & ~WS_CAPTION) | WS_POPUP;
	else
	if(IsNull(icon) && !maximizebox && !minimizebox || noclosebox) {
		style |= WS_POPUPWINDOW|WS_DLGFRAME;
		exstyle |= WS_EX_DLGMODALFRAME;
		if(noclosebox)
			style &= ~WS_SYSMENU;
	}
	else
#endif
		style |= WS_SYSMENU;
	if(tool)
		exstyle |= WS_EX_TOOLWINDOW;
	if(hwnd) {
		::SetWindowLong(hwnd, GWL_STYLE, style);
		::SetWindowLong(hwnd, GWL_EXSTYLE, exstyle);
		SyncTitle();
	}
	DeleteIco();
#ifndef PLATFORM_WINCE //TODO!!!
	if(hwnd) {
		::SendMessage(hwnd, WM_SETICON, false, (LPARAM)(ico = IconWin32(icon)));
		::SendMessage(hwnd, WM_SETICON, true, (LPARAM)(lico = IconWin32(largeicon)));
	}
#endif
}

void TopWindow::CenterRect(HWND hwnd, int center)
{
	GuiLock __;
	SetupRect();
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
	UsrLogT(3, "OPEN " + Desc(this));
	LLOG("TopWindow::Open, owner HWND = " << FormatIntHex((int)hwnd, 8) << ", Active = " << FormatIntHex((int)::GetActiveWindow(), 8));
	IgnoreMouseUp();
	SyncCaption();
#ifdef PLATFORM_WINCE
	if(!GetRect().IsEmpty())
#endif
	if(fullscreen) {
		SetRect(GetScreenSize());
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
	state = MINIMIZED;
	if(IsOpen())
#ifdef PLATFORM_WINCE
		::ShowWindow(GetHWND(), SW_MINIMIZE);
#else
		::ShowWindow(GetHWND(), effect ? SW_MINIMIZE : SW_SHOWMINIMIZED);
#endif
}

void TopWindow::Maximize(bool effect)
{
	state = MAXIMIZED;
	if(IsOpen())
		::ShowWindow(GetHWND(), effect ? SW_MAXIMIZE : SW_SHOWMAXIMIZED);
}

void TopWindow::Overlap(bool effect)
{
	GuiLock __;
	state = OVERLAPPED;
	if(IsOpen())
		::ShowWindow(GetHWND(), effect ? SW_SHOWNORMAL : SW_RESTORE);
}

TopWindow& TopWindow::Style(dword _style)
{
	GuiLock __;
	style = _style;
	if(GetHWND())
		::SetWindowLong(GetHWND(), GWL_STYLE, style);
	SyncCaption();
	return *this;
}

TopWindow& TopWindow::ExStyle(dword _exstyle)
{
	GuiLock __;
	exstyle = _exstyle;
	if(GetHWND())
		::SetWindowLong(GetHWND(), GWL_EXSTYLE, exstyle);
	SyncCaption();
	return *this;
}

TopWindow& TopWindow::TopMost(bool b, bool stay_top)
{
	GuiLock __;
	HWND hwnd;
	if(hwnd = GetHWND())
		SetWindowPos(hwnd, b ? HWND_TOPMOST : (stay_top ? HWND_NOTOPMOST : HWND_BOTTOM),
		             0,0,0,0,SWP_NOMOVE|SWP_NOSIZE );
	return ExStyle(b ? GetExStyle() | WS_EX_TOPMOST : GetExStyle() & ~WS_EX_TOPMOST);
}

bool TopWindow::IsTopMost() const
{
	return GetExStyle() & WS_EX_TOPMOST;
}

#endif

END_UPP_NAMESPACE
