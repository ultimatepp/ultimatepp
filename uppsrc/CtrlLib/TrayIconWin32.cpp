#include "CtrlLib.h"

#ifdef GUI_WIN
#ifndef PLATFORM_WINCE

#include <commdlg.h>
#include <cderr.h>
#include <shellapi.h>

#endif
#endif

namespace Upp {


#ifdef GUI_WIN

#ifndef PLATFORM_WINCE
#define LLOG(x)

enum {
	UM_TASKBAR_ = WM_USER + 1024,
	NIN_BALLOONSHOW_ = WM_USER + 2,
	NIN_BALLOONHIDE_ = WM_USER + 3,
	NIN_BALLOONTIMEOUT_ = WM_USER + 4,
	NIN_BALLOONUSERCLICK_ = WM_USER + 5,
};

TrayIcon::TrayIcon()
{
	Create(NULL, WS_POPUP, 0, false, 0, 0);
	Ctrl::Hide();
	Zero(nid);
	nid.cbSize = sizeof(nid);
	nid.uCallbackMessage = UM_TASKBAR_;
	nid.hWnd = GetHWND();
	static int id;
	nid.uID = ++id;
	visible = false;
	Show();
}

TrayIcon::~TrayIcon()
{
	Hide();
	if(nid.hIcon)
		DestroyIcon(nid.hIcon);
}

void Wcpy(char16 *t, const String& s, int sz)
{
	Vector<char16> w = ToSystemCharsetW(s);
	if(w.GetCount() > sz) {
		w.SetCount(sz - 1);
		w.Add(0);
	}
	memcpy(t, w, w.GetCount() * sizeof(char16));
}

void TrayIcon::Notify(dword msg)
{
	if(visible) {
		nid.uFlags = NIF_ICON|NIF_MESSAGE|NIF_TIP;
		if(nid.hIcon)
			DestroyIcon(nid.hIcon);
		nid.hIcon = SystemDraw::IconWin32(icon);
		Wcpy(nid.szTip, tip, 128);
		BOOL Status = Shell_NotifyIconW(msg, &nid);
		// To prevent from Shell_NotifyIcon bugs...
		// discussed here : http://msdn.microsoft.com/en-us/library/bb762159(v=vs.85).aspx
		// and here : http://issuetracker.delphi-jedi.org/bug_view_advanced_page.php?bug_id=3747
		if (Status == FALSE) {
			// The status of Shell_NotifyIcon is FALSE, in the case of NIM_ADD/NIM_MODIFY, we will try to Modify
			// If the modify is OK then we cas consider that the add was worked.
			// Same, case with delete, we can try modify and if KO then we can consider that is icon
			// was deleted correctly. In other cases, we will retry after 50ms
			DWORD ErrorCode = GetLastError();
			if(ErrorCode == ERROR_SUCCESS || ErrorCode == ERROR_TIMEOUT) {
				for(int retry = 0; retry < 60; retry++) {
					Sleep(50);
					if(Shell_NotifyIconW(NIM_MODIFY, &nid) == (msg != NIM_DELETE))
						break;
				}
			}
		}
    }
}

void TrayIcon::Message(int type, const char *title, const char *text, int timeout)
{
	nid.uFlags = 0x10;
	Wcpy(nid.szInfo, text, 256);
	Wcpy(nid.szInfoTitle, title, 64);
	nid.dwInfoFlags = type;
	nid.uTimeout = minmax(timeout, 10, 30) * 1000;
	Shell_NotifyIconW(NIM_MODIFY, &nid);
}

void TrayIcon::Show(bool b)
{
	if(b == visible)
		return;
	if(visible)
		Notify(NIM_DELETE);
	visible = b;
	if(visible)
		Notify(NIM_ADD);
}

TrayIcon& TrayIcon::Icon(const Image &img)
{
	icon = img;
	Notify(NIM_MODIFY);
	return *this;
}

TrayIcon& TrayIcon::Tip(const char *text)
{
	tip = text;
	Notify(NIM_MODIFY);
	return *this;
}

void TrayIcon::Menu(Bar& bar)
{
	WhenBar(bar);
}

void TrayIcon::LeftDown()
{
	WhenLeftDown();
}

void TrayIcon::LeftUp()
{
	WhenLeftUp();
}

void TrayIcon::LeftDouble()
{
	WhenLeftDouble();
}

void TrayIcon::DoMenu(Bar& bar)
{
	Menu(bar);
}

void TrayIcon::BalloonLeftDown()
{
	WhenBalloonLeftDown();
}

void TrayIcon::BalloonShow()
{
	WhenBalloonShow();
}

void TrayIcon::BalloonHide()
{
	WhenBalloonHide();
}

void TrayIcon::BalloonTimeout()
{
	WhenBalloonTimeout();
}

LRESULT TrayIcon::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if(message == WM_QUERYENDSESSION) {
		Shutdown();
		return true;
	}
	if(message == UM_TASKBAR_)
		switch(lParam) {
		case WM_LBUTTONDOWN:
			LeftDown();
			return TRUE;
		case WM_LBUTTONUP:
			LeftUp();
			return TRUE;
		case WM_LBUTTONDBLCLK:
			::SetForegroundWindow(nid.hWnd);
			LeftDouble();
			return TRUE;
		case WM_RBUTTONDOWN:
			::SetForegroundWindow(nid.hWnd);
			MenuBar::Execute(NULL, THISBACK(DoMenu), GetMousePos());
			return TRUE;
		case NIN_BALLOONSHOW_:
			BalloonShow();
			return TRUE;
		case NIN_BALLOONHIDE_:
			BalloonHide();
			return TRUE;
		case NIN_BALLOONTIMEOUT_:
			BalloonTimeout();
			return TRUE;
		case NIN_BALLOONUSERCLICK_:
			BalloonLeftDown();
			return TRUE;
		}
	static UINT WM_TASKBARCREATED = RegisterWindowMessageA("TaskbarCreated");
	if(message == WM_TASKBARCREATED) {
		LLOG("TrayIcon::WM_TASKBARCREATED");
		visible = false;
		Show();
	}
	return Ctrl::WindowProc(message, wParam, lParam);
}

#endif
#endif

}
