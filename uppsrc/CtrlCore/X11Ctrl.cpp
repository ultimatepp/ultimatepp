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
	return Name0();
}

bool Ctrl::GuiPlatformRefreshFrameSpecial(const Rect&)
{
	return false;
}


bool Ctrl::GuiPlatformSetFullRefreshSpecial()
{
	return false;
}

void Ctrl::GuiPlatformSelection(PasteClip& d)
{
	d.fmt.Clear();
	d.type = 2;
}

void GuiPlatformAdjustDragImage(ImageBuffer& b)
{
	if(Ctrl::IsCompositedGui()) {
		Image h = Rescale(b, 64, 64);
		b = h;
	}
}

bool GuiPlatformHasSizeGrip()
{
	return _NET_Supported().Find(XAtom("_NET_WM_MOVERESIZE")) >= 0;
}

void GuiPlatformGripResize(TopWindow *q)
{
	if(_NET_Supported().Find(XAtom("_NET_WM_MOVERESIZE")) >= 0) {
		XUngrabPointer(Xdisplay, CurrentTime); // 2008-02-25 cxl/mdelfe... compiz fix... who has grabbed it anyway?...
		XClientMessageEvent m;
		m.type = ClientMessage;
		m.serial = 0;
		m.send_event = XTrue;
		m.window = q->GetWindow();
		m.message_type = XAtom("_NET_WM_MOVERESIZE");
		m.format = 32;
		Point p = GetMousePos();
		m.data.l[0] = p.x;
		m.data.l[1] = p.y;
		m.data.l[2] = 4;
		m.data.l[3] = 0;
		m.data.l[4] = 0;
		XSendEvent(Xdisplay, Xroot, 0, SubstructureNotifyMask|SubstructureRedirectMask,
		           (XEvent*)&m);
	}
}

Color GuiPlatformGetScreenPixel(int x, int y)
{
	// TODO
	return Black;
}

void GuiPlatformAfterMenuPopUp()
{
	XSync(Xdisplay, false);
	Ctrl::ProcessEvents();
}

void Ctrl::PaintCaret(SystemDraw& w)
{
	GuiLock __;
	if(this == caretCtrl && WndCaretVisible)
		w.DrawRect(caretx, carety, caretcx, caretcy, InvertColor);
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
