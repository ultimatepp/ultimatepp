#include "CtrlCore.h"

#ifdef GUI_X11

NAMESPACE_UPP

void Ctrl::GuiPlatformConstruct()
{
}

void Ctrl::GuiPlatformDestruct()
{
}

void Ctrl::GuiPlatformRemove()
{
	if(popupgrab) {
		EndPopupGrab();
		popupgrab = false;
	}
}

void Ctrl::GuiPlatformGetTopRect(Rect& r) const
{
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
		s << Format("(window 0x%x)", (int)(intptr_t) GetWindow());
	return s;
}


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