#include "PopUpC.h"

NAMESPACE_UPP

void PopUpC::Deactivate() {
	if(IsOpen()) {
		IgnoreMouseClick();
		WhenDeactivate();
	}
}

void PopUpC::Break(int ID)
{
	EndLoop(ID);
	Close();
	WhenBreak();
}

bool PopUpC::AcceptBreak(int ID)
{
	if(Accept()) {
		WhenAccept();
		Break(ID);
		return true;
	}
	return false;
}

void PopUpC::RejectBreak(int ID)
{
	Reject();
	WhenReject();
	Break(ID);
}

void PopUpC::PopUp(Ctrl* owner)
{
	Size sz = AddFrameSize(GetSize());
	Rect wr = GetWorkArea();
	Rect r = owner->GetScreenRect();
	int x = r.left;
	int y = r.bottom;
	if(x + sz.cx >= wr.right)
		x = r.right - sz.cx;
	if(y + sz.cy >= wr.bottom)
		y = r.top - sz.cy;

	Rect rt = RectC(x, y, sz.cx, sz.cy);
	if(GUI_PopUpEffect()) {
		sPaintRedirectCtrl pb;
		pb.ctrl = this;
		Add(pb.BottomPos(0, rt.Height()).LeftPos(0, rt.Width()));
		SetRect(RectC(rt.left, rt.top, 1, 1));
		Ctrl::PopUp(owner, true, true, GUI_GlobalStyle() >= GUISTYLE_XP);
		SetFocus();
		Ctrl::ProcessEvents();
		Animate(*this, rt, GUIEFFECT_SLIDE);
		pb.Remove();
	}
	else {
		SetRect(rt);
		Ctrl::PopUp(owner, true, true, true);
	}
	SetFocus();
}

END_UPP_NAMESPACE
