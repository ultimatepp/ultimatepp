#include "CtrlLib.h"

namespace Upp {

Point DisplayPopup::Op(Point p)
{
	return p + GetScreenView().TopLeft() - ctrl->GetScreenView().TopLeft();
}

void DisplayPopup::LeftDown(Point p, dword flags)
{
	ctrl->LeftDown(Op(p), flags);
}

void DisplayPopup::LeftDrag(Point p, dword flags)
{
	ctrl->LeftDrag(Op(p), flags);
}

void DisplayPopup::LeftDouble(Point p, dword flags)
{
	ctrl->LeftDouble(Op(p), flags);
}

void DisplayPopup::RightDown(Point p, dword flags)
{
	ctrl->RightDown(Op(p), flags);
}

void DisplayPopup::LeftUp(Point p, dword flags)
{
	ctrl->LeftUp(Op(p), flags);
}

void DisplayPopup::MouseWheel(Point p, int zdelta, dword flags)
{
	ctrl->MouseWheel(Op(p), zdelta, flags);
}

void DisplayPopup::MouseLeave()
{
	Cancel();
}

void DisplayPopup::MouseMove(Point p, dword flags)
{
	p += GetScreenView().TopLeft();
	if(!slim.Contains(p))
		MouseLeave();
}

void DisplayPopup::Paint(Draw& w)
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

DisplayPopup::DisplayPopup()
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
	LinkBefore(all());
}

DisplayPopup::~DisplayPopup()
{
	Unlink();
}

void DisplayPopup::Sync()
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
					r.bottom = max(r.bottom, r.top + sz.cy);
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
					Refresh();
					return;
				}
			}
		}
	}
	if(IsOpen() && !GetDragAndDropSource())
		Close();
}

Link<DisplayPopup> *DisplayPopup::all()
{
	static Link<DisplayPopup> all;
	return &all;
}

void DisplayPopup::SyncAll()
{
	int n = 0;
	for(DisplayPopup *p = all()->Link<DisplayPopup>::GetNext(); p != all(); p = p->Link<DisplayPopup>::GetNext())
		if(p->ctrl && p->ctrl->IsOpen()) {
			p->Sync();
			n++;
		}
}

bool DisplayPopup::StateHook(Ctrl *, int reason)
{
	if(reason == FOCUS)
		SyncAll();
	return false;
}


bool DisplayPopup::MouseHook(Ctrl *, bool, int, Point, int, dword)
{
	SyncAll();
	return false;
}

void DisplayPopup::Cancel()
{
	if(GetDragAndDropSource())
		return;
	display = NULL;
	Sync();
}

bool DisplayPopup::IsOpen()
{
	return Ctrl::IsOpen();
}

bool DisplayPopup::HasMouse()
{
	return Ctrl::HasMouse() || ctrl && ctrl->HasMouse();
}

void DisplayPopup::Set(Ctrl *_ctrl, const Rect& _item,
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
	}
	else
		Refresh();
}

}
