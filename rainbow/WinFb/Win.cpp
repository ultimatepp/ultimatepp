#include "WinFb.h"

NAMESPACE_UPP

HWND fbHWND;
bool fbEndSession;

bool FBEndSession()
{
	return fbEndSession;
}

bool FBIsWaitingEvent()
{
	MSG msg;
	return PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE);
}

bool FBProcessEvent(bool *quit)
{
	MSG msg;
	if(PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)) {
		if(msg.message == WM_QUIT && quit)
			*quit = true;
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
		return true;
	}
	return false;
}

void FBSleep(int ms)
{
	MsgWaitForMultipleObjects(0, NULL, FALSE, ms, QS_ALLINPUT);
}

void FBInit(HINSTANCE hInstance)
{
	GuiLock __;
	
	Ctrl::InitFB();
	
	WNDCLASSW  wc;
	Zero(wc);
	wc.style         = CS_DBLCLKS|CS_HREDRAW|CS_VREDRAW;
	wc.lpfnWndProc   = (WNDPROC)fbWindowProc;
	wc.hInstance     = hInstance;
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)NULL;
	wc.lpszClassName = L"UPP-FB-CLASS";
	RegisterClassW(&wc);
	fbHWND = CreateWindowW(L"UPP-FB-CLASS", L"", WS_OVERLAPPED|WS_VISIBLE|WS_THICKFRAME|WS_MINIMIZEBOX|WS_MAXIMIZEBOX|WS_SYSMENU,
	                       CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
	                       NULL, NULL, hInstance, NULL);
	SetTimer(fbHWND, 1, 10, NULL);

//	Csizeinit();
}

void FBUpdate(const Vector<Rect>& inv)
{
	if(fbHWND)
		for(int i = 0; i < inv.GetCount(); i++)
			::InvalidateRect(fbHWND, inv[i], false);
}

END_UPP_NAMESPACE
