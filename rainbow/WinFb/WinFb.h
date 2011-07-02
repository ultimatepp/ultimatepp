#include <CtrlCore/CtrlCore.h>

NAMESPACE_UPP

extern bool fbEndSession;

LRESULT CALLBACK fbWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void             fbUpdate(HDC hdc, const Rect& r);

END_UPP_NAMESPACE


