#include "CtrlCore.h"

#ifdef GUI_WIN

NAMESPACE_UPP

void Ctrl::SetCaret(int x, int y, int cx, int cy)
{
	GuiLock __;
#ifdef PLATFORM_X11
	if(this == caretCtrl)
		RefreshCaret();
#endif
	caretx = x;
	carety = y;
	caretcx = cx;
	caretcy = cy;
#ifdef PLATFORM_WIN32
	SyncCaret();
#endif
#ifdef PLATFORM_X11
	WndCaretTime = GetTickCount();
	if(this == caretCtrl)
		RefreshCaret();
#endif
}

void Ctrl::SyncCaret() {
	GuiLock __;
#ifdef PLATFORM_X11
	if(focusCtrl != caretCtrl) {
		RefreshCaret();
		caretCtrl = focusCtrl;
		RefreshCaret();
	}
#else
	Rect cr;
	cr.Clear();
	if(focusCtrl && focusCtrl->IsVisible()) {
		bool inframe = focusCtrl->InFrame();
		cr = focusCtrl->GetScreenView();
		cr = RectC(focusCtrl->caretx + cr.left, focusCtrl->carety + cr.top,
				   focusCtrl->caretcx, focusCtrl->caretcy) & cr;
		for(Ctrl *q = focusCtrl->GetParent(); q; q = q->GetParent()) {
			cr &= inframe ? q->GetScreenRect() : q->GetScreenView();
			inframe = q->InFrame();
		}
	}
	if(focusCtrl != caretCtrl || cr != caretRect) {
		LLOG("Do SyncCaret focusCtrl: " << UPP::Name(focusCtrl)
		     << ", caretCtrl: " << UPP::Name(caretCtrl)
		     << ", cr: " << cr);
		WndDestroyCaret();
		if(focusCtrl && !cr.IsEmpty())
			focusCtrl->GetTopCtrl()->WndCreateCaret(cr);
		caretCtrl = focusCtrl;
		caretRect = cr;
	}
#endif
}

END_UPP_NAMESPACE

#endif
