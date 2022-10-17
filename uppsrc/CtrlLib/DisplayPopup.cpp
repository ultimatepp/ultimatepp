#include "CtrlLib.h"

namespace Upp {
	
Point DisplayPopup::PopUp::Op(Point p)
{
	return p + GetScreenView().TopLeft() - ctrl->GetScreenView().TopLeft();
}

void DisplayPopup::PopUp::LeftDown(Point p, dword flags)
{
	if(ctrl) ctrl->LeftDown(Op(p), flags);
}

void DisplayPopup::PopUp::LeftDrag(Point p, dword flags)
{
	if(ctrl) ctrl->LeftDrag(Op(p), flags);
}

void DisplayPopup::PopUp::LeftDouble(Point p, dword flags)
{
	if(ctrl) ctrl->LeftDouble(Op(p), flags);
}

void DisplayPopup::PopUp::RightDown(Point p, dword flags)
{
	if(ctrl) ctrl->RightDown(Op(p), flags);
}

void DisplayPopup::PopUp::LeftUp(Point p, dword flags)
{
	if(ctrl) ctrl->LeftUp(Op(p), flags);
}

void DisplayPopup::PopUp::MouseWheel(Point p, int zdelta, dword flags)
{
	if(ctrl) ctrl->MouseWheel(Op(p), zdelta, flags);
}

void DisplayPopup::PopUp::MouseLeave()
{
	Cancel();
}

void DisplayPopup::PopUp::MouseMove(Point p, dword flags)
{
	p += GetScreenView().TopLeft();
	if(!slim.Contains(p))
		MouseLeave();
}

void DisplayPopup::PopUp::Paint(Draw& w)
{
	Rect r = GetSize();
	w.DrawRect(r, SColorPaper);
	if(display) {
		display->PaintBackground(w, r, value, ink, paper, style);
		r.left += margin;
		if(usedisplaystdsize)
			r.top += (r.Height() - display->GetStdSize(value).cy) / 2;
		display->Paint(w, r, value, ink, paper, style);
	}
}

Vector<DisplayPopup::PopUp *>& DisplayPopup::PopUp::all()
{
	static Vector<DisplayPopup::PopUp *> all;
	return all;
}

DisplayPopup::PopUp::PopUp()
{
	SetFrame(BlackFrame());
	display = NULL;
	paper = ink = Null;
	style = 0;
	item = slim = Null;
	margin = 0;
	ONCELOCK {
		InstallStateHook(StateHook);
		InstallMouseHook(MouseHook);
	}
	all().Add(this);
}

DisplayPopup::PopUp::~PopUp()
{
	int q = FindIndex(all(), this);
	if(q >= 0)
		all().Remove(q);
}

void DisplayPopup::PopUp::Sync()
{
	if(!IsMainThread()) {
		PostCallback(PTEBACK(Sync));
		return;
	}
	if(display && ctrl && !ctrl->IsDragAndDropTarget() && !IsDragAndDropTarget()) {
		Ctrl *top = ctrl->GetTopCtrl();
		if(top && top->HasFocusDeep()) {
			Size sz = display->GetStdSize(value);
			if(sz.cx + 2 * margin > item.GetWidth() || sz.cy > item.GetHeight()) {
				Rect vw = ctrl->GetScreenView();
				slim = (item + vw.TopLeft()) & vw;
				if(slim.Contains(GetMousePos())) {
					Rect r = item;
					r.right = max(r.right, r.left + sz.cx + 2 * margin);
					r.bottom = max(r.bottom, r.top + sz.cy + 2 * margin);
					r.Inflate(1, 1);
					Rect v = ctrl->GetScreenView();
					r.Offset(v.TopLeft());

					Rect wa = GetWorkArea(r.BottomLeft());
					Size sz = r.GetSize();
					if(r.left < wa.left) {
						r.left = wa.left;
						r.right = min(wa.right, r.left + sz.cx);
					}
					else
					if(r.right > wa.right) {
						r.left = max(wa.left, wa.right - sz.cx);
						r.right = wa.right;
					}
					if(r.top < wa.top) {
						r.top = wa.top;
						r.bottom = min(wa.bottom, wa.top + sz.cy);
					}
					else
					if(r.bottom > wa.bottom) {
						if(wa.bottom - r.top < r.top - wa.top) { // there is more space upside
							r.bottom = item.bottom + v.top;
							r.top = max(wa.top, r.bottom - sz.cy);
						}
						else
							r.bottom = wa.bottom;
					}
					SetRect(r);
					if(!IsOpen())
						Ctrl::PopUp(ctrl, true, false, false);
					return;
				}
			}
		}
	}
	if(IsOpen() && !GetDragAndDropSource())
		WhenClose();
}

void DisplayPopup::PopUp::SyncAll()
{
	int n = 0;
	for(DisplayPopup::PopUp *p : all())
		if(p->ctrl && p->ctrl->IsOpen()) {
			p->Sync();
			n++;
		}
}

bool DisplayPopup::PopUp::StateHook(Ctrl *, int reason)
{
	if(reason == FOCUS)
		SyncAll();
	return false;
}


bool DisplayPopup::PopUp::MouseHook(Ctrl *, bool, int, Point, int, dword)
{
	SyncAll();
	return false;
}

void DisplayPopup::PopUp::Cancel()
{
	if(GetDragAndDropSource())
		return;
	display = NULL;
	Sync();
}

bool DisplayPopup::PopUp::IsOpen()
{
	return Ctrl::IsOpen();
}

bool DisplayPopup::PopUp::HasMouse()
{
	return Ctrl::HasMouse() || ctrl && ctrl->HasMouse();
}

void DisplayPopup::PopUp::Set(Ctrl *_ctrl, const Rect& _item,
                       const Value& _value, const Display *_display,
                       Color _ink, Color _paper, dword _style, int _margin)
{
	if(!GUI_ToolTips() || GetDragAndDropSource())
		return;
	if(item != _item || ctrl != _ctrl || value != _value || display != _display || ink != _ink ||
	   paper != _paper || style != _style) {
		item = _item;
		ctrl = _ctrl;
		value = _value;
		display = _display;
		ink = _ink;
		paper = _paper;
		style = _style;
		margin = _margin;
		if(ctrl)
			Tip(ctrl->GetTip());
		Sync();
		Refresh();
	}
}


void DisplayPopup::Set(Ctrl *ctrl, const Rect& item, const Value& v, const Display *display, Color ink, Color paper, dword style, int margin)
{
	if(!popup) {
		popup.Create();
		popup->usedisplaystdsize = usedisplaystdsize;
		popup->WhenClose << [=] { PostCallback([=] { popup.Clear(); }); };
	}
	popup->Set(ctrl, item, v, display, ink, paper, style, margin);
}

void DisplayPopup::Cancel()
{
	if(popup)
		popup->Cancel();
}

bool DisplayPopup::IsOpen()
{
	return popup && popup->IsOpen();
}

bool DisplayPopup::HasMouse()
{
	return popup && popup->HasMouse();
}

void DisplayPopup::UseDisplayStdSize()
{
	usedisplaystdsize = true;
	if(popup)
		popup->usedisplaystdsize = true;
}

DisplayPopup::~DisplayPopup()
{
	if(popup)
		popup->Close();
}

}
