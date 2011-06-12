#include "CtrlCore.h"

#ifdef GUI_WIN

#define LLOG(x) // DLOG(x)

NAMESPACE_UPP

void Ctrl::GuiPlatformConstruct()
{
	activex = false;
	isdhctrl = false;
}

void Ctrl::GuiPlatformDestruct()
{
}

void Ctrl::GuiPlatformRemove()
{
}

void Ctrl::GuiPlatformGetTopRect(Rect& r) const
{
	if(activex)
		r = GetWndScreenRect();
}

void Ctrl::SetCaret(int x, int y, int cx, int cy)
{
	GuiLock __;
	caretx = x;
	carety = y;
	caretcx = cx;
	caretcy = cy;
	SyncCaret();
}

void Ctrl::SyncCaret() {
	GuiLock __;
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
}

String Ctrl::Name() const {
	GuiLock __;
#ifdef CPU_64
	String s = String(typeid(*this).name()) + " : 0x" + FormatIntHex(this);
#else
	String s = String(typeid(*this).name()) + " : " + Format("0x%x", (int) this);
#endif
	if(IsChild())
		s << "(parent " << String(typeid(*parent).name()) << ")";
	else
		s << Format("(hwnd 0x%x)", (int)(intptr_t) GetHWND());
	return s;
}

void Ctrl::WndCreateCaret(const Rect& cr)
{
	ICall(THISBACK1(WndCreateCaret0, cr));
}

END_UPP_NAMESPACE

#endif
