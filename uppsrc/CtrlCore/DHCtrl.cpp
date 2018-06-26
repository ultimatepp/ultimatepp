#include "CtrlCore.h"

#ifdef GUI_WIN

namespace Upp {

#ifndef PLATFORM_WINCE

Vector<DHCtrl *> DHCtrl::all_active;

void DHCtrl::RemoveActive()
{
	for(;;) {
		int q = FindIndex(all_active, this);
		if(q < 0)
			return;
		all_active.Remove(q);
	}
}

bool DHCtrl::PreprocessMessageAll(MSG& msg)
{
	for(auto q : all_active)
		if(q->PreprocessMessage(msg))
			return true;
	return false;
}

bool DHCtrl::PreprocessMessage(MSG& msg)
{
	return false;
}

void DHCtrl::NcCreate(HWND _hwnd)
{
	hwnd = _hwnd;
	all_active.Add(this);
}

void DHCtrl::NcDestroy()
{
	hwnd = NULL;
	RemoveActive();
}

LRESULT DHCtrl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	GuiLock __;
	return DefWindowProc(hwnd, message, wParam, lParam);
}

void DHCtrl::CloseHWND()
{
	GuiLock __;
	if(hwnd) {
		DestroyWindow(hwnd);
		hwnd = NULL;
		RemoveActive();
	}
}

void DHCtrl::OpenHWND()
{
	GuiLock __;
	CloseHWND();
	HWND phwnd = GetTopCtrl()->GetHWND();
	if(phwnd)
		CreateWindowEx(0, "UPP-CLASS-A", "",
		               WS_CHILD|WS_DISABLED|WS_VISIBLE,
		               0, 0, 20, 20,
		               phwnd, NULL, hInstance, this);
}

void DHCtrl::SyncHWND()
{
	GuiLock __;
	HWND phwnd = GetTopCtrl()->GetHWND();
	if(phwnd) {
		Rect r = GetScreenView();
		Rect pr = GetScreenClient(phwnd);
		SetWindowPos(hwnd, NULL, r.left - pr.left, r.top - pr.top, r.Width(), r.Height(),
		             SWP_NOACTIVATE|SWP_NOZORDER);
		ShowWindow(hwnd, IsVisible() ? SW_SHOW : SW_HIDE);
	}
}

void DHCtrl::State(int reason)
{
	switch(reason) {
	case OPEN:
		OpenHWND();
	default:
		SyncHWND();
		break;
	case CLOSE:
		CloseHWND();
	}
}

DHCtrl::DHCtrl()
{
	hwnd = NULL;
	isdhctrl = true;
	BackPaint(EXCLUDEPAINT);
}

DHCtrl::~DHCtrl()
{
	CloseHWND();
	BackPaint(EXCLUDEPAINT);
	RemoveActive();
}

#endif

}

#endif
