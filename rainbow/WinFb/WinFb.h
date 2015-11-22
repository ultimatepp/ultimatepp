#ifndef _WinFb_WinFb_h
#define _WinFb_WinFb_h

#include <CtrlCore/CtrlCore.h>

NAMESPACE_UPP

LRESULT CALLBACK fbWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void             fbUpdate(HDC hdc, const Rect& r);

END_UPP_NAMESPACE

#endif
