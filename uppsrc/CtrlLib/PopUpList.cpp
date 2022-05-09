#include "CtrlLib.h"
#include "CtrlLib.h"

namespace Upp {

CtrlFrame& DropFrame();

void PopUpList::Clear()
{
	if(popup)
		popup->ac.SetVirtualCount(0);
	items.Clear();
}

void PopUpList::Add(const Value& v)
{
	items.Add(v);
	if(popup)
		popup->ac.SetVirtualCount(items.GetCount());
}

void PopUpList::PopupCancelMode() {
	if(popup && !inpopup)
		DoCancel();
}

void PopUpList::DoClose() {
	if(!inpopup && popup) {
		popup->closing = true; // prevent infinite recursion
		popup.Clear();
	}
}

void PopUpList::PopupDeactivate() {
	if(popup) {
		Ctrl::IgnoreMouseClick();
		DoCancel();
	}
}

void PopUpList::PopupArrayCtrl::LeftUp(Point p, dword keyflags)
{
	ArrayCtrl::LeftUp(p, keyflags);
	list->DoSelect();
}

bool PopUpList::PopupArrayCtrl::Key(dword key, int n)
{
	switch(key) {
	case K_ENTER:
	case K_ALT_DOWN:
		list->DoSelect();
		return true;
	case K_ESCAPE:
		list->DoCancel();
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

PopUpList::Popup::Popup(PopUpList *list)
:	list(list)
{
	ac.list = list;
	ac.SetFrame(DropFrame());
	ac.AddRowNumColumn().Accel().ConvertBy([=] (const Value& i) -> Value {
		return list->items[i];
	});
	ac.NoHeader();
	ac.HeaderTab(0).SetMargin(0);
	ac.MouseMoveCursor();
	ac.NoGrid();
	ac.AutoHideSb();
	ac.SetLineCy(Draw::GetStdFontCy());
	ac.SetVirtualCount(list->items.GetCount());
}

void PopUpList::PopUp(Ctrl *owner, int x, int top, int bottom, int width) {
	if(inpopup)
		return;
	inpopup++;
	DoClose();
	popup.Create(this);
	int h = popup->ac.AddFrameSize(width, min(droplines * popup->ac.GetLineCy(), popup->ac.GetTotalCy())).cy;
	DDUMP(droplines * popup->ac.GetLineCy());
	DDUMP(popup->ac.GetTotalCy());
	DDUMP(h);
	Rect rt = RectC(x, bottom, width, h);
	DDUMP(rt);
	Rect area = Ctrl::GetWorkArea(Point(x, top));
	bool up = false;
	if(rt.bottom > area.bottom) {
		up = true;
		rt.top = top - h;
		rt.bottom = rt.top + h;
	}
	if(up) {
		popup->SetRect(Rect(rt.left, rt.bottom - 1, rt.right, rt.bottom));
		popup->Add(popup->ac.TopPos(0, rt.Height()).LeftPos(0, rt.Width()));
	}
	else {
		popup->SetRect(Rect(rt.left, rt.top, rt.right, rt.top + 1));
		popup->Add(popup->ac.BottomPos(0, rt.Height()).LeftPos(0, rt.Width()));
	}
	if(GUI_PopUpEffect()) {
		popup->ac.CenterCursor();
		popup->PopUp(owner, true, true, GUI_DropShadows());
		popup->ac.SetFocus();
		Ctrl::ProcessEvents();
		Animate(*popup, rt, GUIEFFECT_SLIDE);
	}
	if(popup && !popup->IsOpen()) {
		popup->SetRect(rt);
		if(!popup->IsOpen())
			popup->PopUp(owner, true, true, GUI_DropShadows());
		popup->ac.CenterCursor();
		popup->ac.SetFocus();
	}
	DDUMP(popup->ac.GetScreenRect());
	DDUMP(popup->GetScreenRect());
	DDUMP(popup->GetScreenView());
	DDUMP(popup->GetFrameCount());
	inpopup--;
}

void PopUpList::PopUp(Ctrl *owner, int width)
{
	Rect r = owner->GetScreenRect();
	if(width)
		r.right = r.left + width;
	PopUp(owner, r.left, r.top, r.bottom, r.Width());
}

void PopUpList::PopUp(Ctrl *owner)
{
	Rect r = owner->GetScreenRect();
	PopUp(owner, r.left, r.top, r.bottom, r.Width());
}

void PopUpList::DoSelect()
{
	DoClose();
	Select();
}

void PopUpList::DoCancel()
{
	DoClose();
	Cancel();
}

PopUpList::PopUpList() {
	droplines = 16;
	inpopup = 0;
}

PopUpList::~PopUpList() {}

}
