#include <CtrlLib/CtrlLib.h>
using namespace Upp;
#include "TaskbarButton.h"

TaskbarButton::TaskbarButton()
{
    SetRect(0, 0, 16, 16);
    Zero(nid);
    nid.cbSize = sizeof(nid);
    nid.uCallbackMessage = UM_TASKBAR;
    NoWantFocus();
}

TaskbarButton::~TaskbarButton()
{
    Flush(NIM_DELETE);
    nid.uID = 0;
}

TaskbarButton& TaskbarButton::Icon(const Image &img)
{
	icon = img;
	Flush(NIM_MODIFY);
    return *this;
}

TaskbarButton& TaskbarButton::Tip(const char *text)
{
    tip = text;
	Flush(NIM_MODIFY);
    return *this;
}

void TaskbarButton::Init(HWND hwnd)
{
    nid.hWnd = hwnd;
    nid.uID = 1;
    Flush(NIM_ADD);
}

void TaskbarButton::PassNotification(UINT lParam)
{
	switch(lParam) 
	{
		case WM_LBUTTONDOWN:
			Action();
			return;
		case WM_LBUTTONUP:
			WhenLeftUp();
			return;
		case WM_LBUTTONDBLCLK:
			WhenLeftDouble();
			return;
		case WM_RBUTTONDOWN:
			MenuBar::Execute(this, WhenBar, GetMousePos());
			return;
	}
}

void TaskbarButton::Flush(dword msg)
{
    if(nid.uID)
    {
        nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
        nid.hIcon = IconWin32(icon);
        int len = tip.GetLength();
        memcpy(nid.szTip, tip, len);
        nid.szTip[len] = 0;
        VERIFY(Shell_NotifyIcon(msg, &nid));
    }
}
