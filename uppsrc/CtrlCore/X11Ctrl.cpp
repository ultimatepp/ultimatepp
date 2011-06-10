#include "CtrlCore.h"

#ifdef GUI_X11

NAMESPACE_UPP

void Ctrl::SetCaret(int x, int y, int cx, int cy)
{
	GuiLock __;
	if(this == caretCtrl)
		RefreshCaret();
	caretx = x;
	carety = y;
	caretcx = cx;
	caretcy = cy;
	WndCaretTime = GetTickCount();
	if(this == caretCtrl)
		RefreshCaret();
}

void Ctrl::SyncCaret() {
	GuiLock __;
	if(focusCtrl != caretCtrl) {
		RefreshCaret();
		caretCtrl = focusCtrl;
		RefreshCaret();
	}
}

END_UPP_NAMESPACE

#endif