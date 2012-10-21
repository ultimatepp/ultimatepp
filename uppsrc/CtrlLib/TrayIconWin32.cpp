#include "CtrlLib.h"

#ifdef GUI_WIN
#ifndef PLATFORM_WINCE

#include <commdlg.h>
#include <cderr.h>
#include <ShellApi.h>

#endif
#endif

NAMESPACE_UPP


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
	nid.sz = IsWin2K() ? sizeof(NotifyIconNew) : sizeof(NotifyIconOld);
	nid.message = UM_TASKBAR_;
	nid.hwnd = GetHWND();
	static int id;
	nid.id = ++id;
	visible = false;
	Show();
}

TrayIcon::~TrayIcon()
{
	Hide();
	if(nid.icon)
		DestroyIcon(nid.icon);
}

void TrayIcon::Notify(dword msg)
{
	if(visible) {
		nid.flags = NIF_ICON|NIF_MESSAGE|NIF_TIP;
		if(nid.icon)
			DestroyIcon(nid.icon);
		nid.icon = SystemDraw::IconWin32(icon);
		String stip = ToSystemCharset(tip);
		int len = min(stip.GetLength(), 125);
		memcpy(nid.tip, stip, len);
		nid.tip[len] = 0;
		BOOL Status = Shell_NotifyIcon(msg, (NOTIFYICONDATA *)&nid);		
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
					if(Shell_NotifyIcon(NIM_MODIFY, (NOTIFYICONDATA *)&nid) == (msg != NIM_DELETE))
						break;
				}
			}
		}
    }
}

void TrayIcon::Message(int type, const char *title, const char *text, int timeout)
{
	if(!IsWin2K())
	    return;
	nid.flags = 0x10;
	*nid.info = *nid.title = 0;
	if(text) {
		String h = ToSystemCharset(text);
		memcpy(nid.info, h, min(h.GetLength(), 255) + 1);
		nid.info[255] = 0;
	}
	if(title) {
		String h = ToSystemCharset(title);
		memcpy(nid.title, h, min(h.GetLength(), 63) + 1);
		nid.title[63] = 0;
	}
	nid.infoflags = type;
	nid.timeout = minmax(timeout, 10, 30) * 1000;
	Shell_NotifyIcon(NIM_MODIFY, (NOTIFYICONDATA *)&nid);
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
	if(message == UM_TASKBAR_)
		switch(lParam) {
		case WM_LBUTTONDOWN:
			LeftDown();
			return TRUE;
		case WM_LBUTTONUP:
			LeftUp();
			return TRUE;
		case WM_LBUTTONDBLCLK:
			::SetForegroundWindow(nid.hwnd);
			LeftDouble();
			return TRUE;
		case WM_RBUTTONDOWN:
			::SetForegroundWindow(nid.hwnd);
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

END_UPP_NAMESPACE
