#include "CtrlLib.h"

NAMESPACE_UPP

CH_VALUE(DropEdge, ChBorder(BlackBorder()));

CtrlFrame& DropFrame()
{
	static LookFrame m(DropEdge);
	return m;
}

void PopUpTable::CancelMode() {
	if(open) {
		DoClose();
		WhenCancel();
	}
	ArrayCtrl::CancelMode();
}

void PopUpTable::DoClose() {
	open = false;
	Close();
}

void PopUpTable::Deactivate() {
	if(open) {
		DoClose();
		IgnoreMouseClick();
		WhenCancel();
	}
}

void PopUpTable::LeftUp(Point p, dword keyflags) {
	ArrayCtrl::LeftUp(p, keyflags);
	DoClose();
	WhenSelect();
}

bool PopUpTable::Key(dword key, int n) {
	switch(key) {
	case K_ENTER:
	case K_ALT_DOWN:
		DoClose();
		WhenSelect();
		return true;
	case K_ESCAPE:
		DoClose();
		WhenCancel();
		return true;
	case K_UP:
		if(!IsCursor()) {
			SetCursor(GetCount() - 1);
			return true;
		}
		break;
	case K_DOWN:
		if(!IsCursor()) {
			SetCursor(0);
			return true;
		}
		break;
	}
	return ArrayCtrl::Key(key, n);
}

void PopUpTable::PopUp(Ctrl *owner, int x, int top, int bottom, int width) {
	if(inpopup)
		return;
	inpopup++;
	DoClose();
	int h = AddFrameSize(width, min(droplines * GetLineCy(), GetTotalCy())).cy;
	Rect rt = RectC(x, bottom, width, h);
	Rect area = Ctrl::GetWorkArea(Point(x, top));
	bool up = false;
	if(rt.bottom > area.bottom) {
		up = true;
		rt.top = top - h;
		rt.bottom = rt.top + h;
	}
	open = false;
	if(GUI_PopUpEffect()) {
		AutoHideSb(false);
		HideSb(true);
		sPaintRedirectCtrl pb;
		pb.ctrl = this;
		if(up) {
			SetRect(Rect(rt.left, rt.bottom - 1, rt.right, rt.bottom));
			Ctrl::Add(pb.TopPos(0, rt.Height()).LeftPos(0, rt.Width()));
		}
		else {
			SetRect(Rect(rt.left, rt.top, rt.right, rt.top + 1));
			Ctrl::Add(pb.BottomPos(0, rt.Height()).LeftPos(0, rt.Width()));
		}
		Ctrl::PopUp(owner, true, true, GUI_DropShadows());
		SetFocus();
		Ctrl::ProcessEvents();
		Animate(*this, rt, GUIEFFECT_SLIDE);
		pb.Remove();
		HideSb(false);
		AutoHideSb(true);
		CenterCursor();
		open = true;
	}
	if(!open) {
		CenterCursor();
		SetRect(rt);
		Ctrl::PopUp(owner, true, true, GUI_DropShadows());
		SetFocus();
		open = true;
	}
	inpopup--;
}

void PopUpTable::PopUp(Ctrl *owner, int width)
{
	Rect r = owner->GetScreenRect();
	if(width)
		r.right = r.left + width;
	PopUp(owner, r.left, r.top, r.bottom, r.Width());
}

void PopUpTable::PopUp(Ctrl *owner)
{
	Rect r = owner->GetScreenRect();
	PopUp(owner, r.left, r.top, r.bottom, r.Width());
}

void PopUpTable::Normal()
{
	AddColumn().Accel();
	NoHeader();
	HeaderTab(0).SetMargin(0);
	MouseMoveCursor();
	NoGrid();
	AutoHideSb();
	SetLineCy(Draw::GetStdFontCy());
	NoPopUpEx();
}

PopUpTable::PopUpTable() {
	SetFrame(DropFrame());
	droplines = 16;
	open = false;
	inpopup = 0;
}

PopUpTable::~PopUpTable() {}

END_UPP_NAMESPACE
