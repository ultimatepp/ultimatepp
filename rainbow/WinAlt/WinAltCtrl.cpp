#include <CtrlCore/CtrlCore.h>

#ifdef GUI_WINALT

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

bool Ctrl::GuiPlatformRefreshFrameSpecial(const Rect& r)
{
	if(isdhctrl) {
		InvalidateRect(((DHCtrl *)this)->GetHWND(), r, false);
		return true;
	}
	return false;
}

bool Ctrl::GuiPlatformSetFullRefreshSpecial()
{
	return isdhctrl;
}

void Ctrl::PaintCaret(SystemDraw& w)
{
}

String GuiPlatformGetKeyDesc(dword key)
{
	static struct {
		dword key;
		const char *name;
	} nkey[] = {
		{ 0x100c0, "[`]" }, { 0x100bd, "[-]" }, { 0x100bb, "[=]" }, { 0x100dc, "[\\]" },
		{ 0x100db, "[[]" }, { 0x100dd, "[]]" },
		{ 0x100ba, "[;]" }, { 0x100de, "[']" },
		{ 0x100bc, "[,]" }, { 0x100be, "[.]" }, { 0x100bf, "[/]" },
		{ 0, NULL }
	};
	for(int i = 0; nkey[i].key; i++)
		if(nkey[i].key == key)
			return nkey[i].name;
	return Null;
}

void Ctrl::GuiPlatformSelection(PasteClip&)
{
}

void GuiPlatformAdjustDragImage(ImageBuffer&)
{
}

bool GuiPlatformHasSizeGrip()
{
	return true;
}

void GuiPlatformGripResize(TopWindow *q)
{
	HWND hwnd = q->GetHWND();
	Point p = GetMousePos() - q->GetRect().TopLeft();
	if(hwnd) {
		::SendMessage(hwnd, WM_SYSCOMMAND, 0xf008, MAKELONG(p.x, p.y));
		::SendMessage(hwnd, WM_LBUTTONUP, 0, MAKELONG(p.x, p.y));
	}
}

Color GuiPlatformGetScreenPixel(int x, int y)
{
	HDC sdc = GetWindowDC(0);
	Color c = Color::FromCR(GetPixel(sdc, x, y));
	ReleaseDC(0, sdc);
	return c;
}

void GuiPlatformAfterMenuPopUp()
{
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
