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
	if(message == WM_ERASEBKGND)
		return 1L;
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
		CreateWindowEx(0, "UPP-CLASS-W", "",
		               WS_CHILD|WS_DISABLED|WS_VISIBLE,
		               0, 0, 20, 20,
		               phwnd, NULL, hInstance, this);
}

void DHCtrl::SyncHWND()
{
	GuiLock __;
	HWND phwnd = GetTopCtrl()->GetHWND();
	if(phwnd) {
		Rect r = GetScreenView() - GetScreenClient(phwnd).TopLeft();
		if(r != current_pos || IsVisible() != (bool)current_visible) {
			SetWindowPos(hwnd, NULL, r.left, r.top, r.Width(), r.Height(), SWP_NOACTIVATE|SWP_NOZORDER);
			ShowWindow(hwnd, IsVisible() ? SW_SHOW : SW_HIDE);
			Refresh();
			current_pos = r;
			current_visible = IsVisible();
		}
	}
}

void DHCtrl::State(int reason)
{
	switch(reason) {
	case OPEN:
		current_pos = Null;
		current_visible = Null;
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

void Ctrl::UpdateDHCtrls()
{ // we call this in WM_PAINT to force updating in single WM_PAINT, this makes things smoother e.g. with OpenGL in splitter
	for(Ctrl *q = GetFirstChild(); q; q = q->GetNext()) {
		DHCtrl *dh = dynamic_cast<DHCtrl *>(q);
		if(dh)
			UpdateWindow(dh->GetHWND());
		q->UpdateDHCtrls();
	}
}


#endif

}

#endif
