#include "CtrlLib.h"

namespace Upp {

CH_VALUE(TreeDropEdge, ChBorder(BlackBorder()));

CtrlFrame& TreeDropFrame()
{
	static LookFrame m(TreeDropEdge);
	return m;
}

PopUpTree::PopUpTree() {
	SetFrame(TreeDropFrame());
	Accel();
	MouseMoveCursor();
	NoPopUpEx();
	SetDropLines(16);
	open = autosize = false;
	showpos = Null;
	WhenOpen = WhenClose = THISBACK(OpenClose);
}

PopUpTree::~PopUpTree() {}

void PopUpTree::CancelMode() {
	if(open) {
		DoClose();
		WhenCancel();
	}
	TreeCtrl::CancelMode();
}

void PopUpTree::DoClose() {
	open = false;
	Ctrl::Close();
}

void PopUpTree::Deactivate() {
	if(open) {
		DoClose();
		IgnoreMouseClick();
		WhenCancel();
	}
}

void PopUpTree::Select() {
	if(IsCursor() && !GetData().IsVoid()) {
		DoClose();
		WhenSelect();
	}
}

bool PopUpTree::Key(dword key, int n) {
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
	}
	return TreeCtrl::Key(key, n);
}

void PopUpTree::PopUp(Ctrl *owner, int x, int top, int bottom, int width) {
	DoClose();
	Rect area = Ctrl::GetWorkArea();
	int mh = min(maxheight, area.bottom - bottom);
	int h = AddFrameSize(width, maxheight).cy;
	showpos.x = x;
	showpos.y = bottom;
	showwidth = width;
	up = false;
	if(showpos.y + h > area.bottom) {
		up = true;
		showpos.y = top;
	}
	open = false;

	int ht = AddFrameSize(width, min(mh, autosize ? GetTreeSize().cy : INT_MAX)).cy;
	Rect rt = RectC(showpos.x, showpos.y - (up ? ht : 0), showwidth, ht);
	if(GUI_PopUpEffect()) {
		bool vis = sb.x.IsShown();
		bool ah = sb.x.IsAutoHide();
		sb.AutoHide(false);
		sb.Hide();
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
		CenterCursor();
		Ctrl::PopUp(owner, true, true, GUI_DropShadows());
		SetFocus();
		Ctrl::ProcessEvents();
		Animate(*this, rt, GUIEFFECT_SLIDE);
		Ctrl::Remove();
		sb.Show(vis);
		sb.AutoHide(ah);
		pb.Remove();
		open = true;
	}

	CenterCursor();
	SetRect(rt);
	if(!open)
		Ctrl::PopUp(owner, true, true, GUI_DropShadows());
	SetFocus();
	open = true;
}

void PopUpTree::OpenClose(int)
{
	if(autosize) {
		SyncTree();
		Rect area = Ctrl::GetWorkArea();
		int mh = min(maxheight, area.bottom - showpos.y);
		int ht = AddFrameSize(showwidth, min(mh, GetTreeSize().cy)).cy;
		SetRect(RectC(showpos.x, showpos.y - (up ? ht : 0), showwidth, ht));
	}
}

void PopUpTree::PopUp(Ctrl *owner, int width)
{
	Rect r = owner->GetScreenRect();
	r.right = r.left + width;
	PopUp(owner, r.left, r.top, r.bottom, width);
	if(IsNull(showpos))
		showpos = r.TopLeft();
	OpenClose(0);
}

void PopUpTree::PopUp(Ctrl *owner)
{
	Rect r = owner->GetScreenRect();
	PopUp(owner, r.left, r.top, r.bottom, r.Width());
}

void DropTree::Sync() {
	Image icon;
	if(tree.IsCursor())
		icon = tree.GetNode(tree.GetCursor()).image;
	icond.Set(valuedisplay ? *valuedisplay : tree.GetDisplay(tree.GetCursor()), icon);
	MultiButton::SetDisplay(icond);
	Set(tree.GetValue());
}

bool DropTree::Key(dword k, int) {
	if(IsReadOnly()) return false;
	if(k == K_ALT_DOWN) {
		Drop();
		return true;
	}
	return false;
}

void DropTree::Drop() {
	if(IsReadOnly()) return;
	if(dropfocus)
		SetFocus();
	WhenDrop();
	tree.PopUp(this, GetRect().GetWidth());
}

void DropTree::Select() {
	if(dropfocus)
		SetFocus();
	Sync();
	UpdateAction();
}

void DropTree::Cancel() {
	if(dropfocus)
		SetFocus();
	Sync();
}

void DropTree::Clear() {
	tree.Clear();
	tree <<= Null;
	Sync();
	Update();
}

void DropTree::SetData(const Value& data)
{
	if(tree.Get() != data) {
		tree <<= data;
		Update();
		Sync();
	}
}

Value DropTree::GetData() const
{
	return notnull && IsNull(tree.Get()) ? NotNullError() : tree.Get();
}

DropTree& DropTree::ValueDisplay(const Display& d)
{
	valuedisplay = &d;
	Sync();
	return *this;
}

DropTree::DropTree()
{
	displayall = false;
	valuedisplay = NULL;
	dropfocus = false;
	notnull = false;
	AddButton().Main().WhenPush = THISBACK(Drop);
	NoInitFocus();
	tree.WhenSelect = THISBACK(Select);
	tree.WhenCancel = THISBACK(Cancel);
	dropwidth = 0;
}

};
