#include "CtrlLib.h"

namespace Upp {
	
void DisplayPopup::Pop::PopCtrl::Paint(Draw& w)
{
	Rect r = GetSize();
	w.DrawRect(r, SColorPaper);
	if(!p) return;
	if(p->display) {
		p->display->PaintBackground(w, r, p->value, p->ink, p->paper, p->style);
		r.left += p->margin;
		if(p->usedisplaystdsize)
			r.top += (r.Height() - p->display->GetStdSize(p->value).cy) / 2;
		p->display->Paint(w, r, p->value, p->ink, p->paper, p->style);
	}
}

Vector<DisplayPopup::Pop *>& DisplayPopup::Pop::all()
{
	static Vector<DisplayPopup::Pop *> all;
	return all;
}

DisplayPopup::Pop::Pop()
{
	display = NULL;
	paper = ink = Null;
	style = 0;
	item = Null;
	margin = 0;
	all().Add(this);
	view.IgnoreMouse();
	view.SetFrame(BlackFrame());
	frame.IgnoreMouse();
	frame.SetFrame(BlackFrame());
	view.p = this;
	frame.p = this;
}

DisplayPopup::Pop::~Pop()
{
	view.p = nullptr;
	frame.p = nullptr;
	int q = FindIndex(all(), this);
	if(q >= 0)
		all().Remove(q);
}

Rect DisplayPopup::Check(Ctrl *ctrl, const Rect& item, const Value& value, const Display *display, int margin)
{
	if(display && ctrl && !ctrl->IsDragAndDropTarget() && !(GetMouseLeft() || GetMouseRight() || GetMouseMiddle())) {
		Ctrl *top = ctrl->GetTopCtrl();
		if(top && top->HasFocusDeep()) {
			Size sz = display->GetStdSize(value);
			if(sz.cx + 2 * margin > item.GetWidth() || sz.cy > item.GetHeight()) {
				Rect vw = ctrl->GetScreenView();
				Rect r = (item + vw.TopLeft()) & vw;
				if(r.Contains(GetMousePos()))
					return r;
			}
		}
	}
	return Null;
}

void DisplayPopup::Pop::Sync()
{
	if(!IsMainThread()) {
		Ptr<Pop> p;
		PostCallback([=] { if(p) p->Sync(); });
		return;
	}
	Rect r = Check(ctrl, item, value, display, margin);
	if(IsNull(r))
		WhenClose();
	else {
		Ctrl *top = ctrl->GetTopCtrl();
		Size sz = display->GetStdSize(value);
		Rect wa = top->GetScreenRect();
		r.right = min(wa.right, r.left + sz.cx + 2 * margin);
		r.bottom = max(r.bottom, r.top + sz.cy);
		r.Inflate(1, 1);
		view.SetRect(r - top->GetScreenView().TopLeft());
		frame.SetFrameRect(r - wa.TopLeft());
		if(!frame.GetParent())
			*top << view << frame;
	}
}

DisplayPopup::DisplayPopup()
{
	ONCELOCK {
		Ctrl::InstallStateHook(StateHook);
		Ctrl::InstallMouseHook(MouseHook);
	}
}

void DisplayPopup::SyncAll()
{
	for(DisplayPopup::Pop *p : Pop::all())
		if(p->ctrl && p->ctrl->IsOpen())
			p->Sync();
}

bool DisplayPopup::StateHook(Ctrl *, int reason)
{
	if(reason == Ctrl::FOCUS)
		SyncAll();
	return false;
}


bool DisplayPopup::MouseHook(Ctrl *, bool, int, Point, int, dword)
{
	SyncAll();
	return false;
}

void DisplayPopup::Pop::Set(Ctrl *_ctrl, const Rect& _item,
                            const Value& _value, const Display *_display,
                            Color _ink, Color _paper, dword _style, int _margin)
{
	if(!GUI_ToolTips())
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
		if(ctrl) {
			String h = ctrl->GetTip();
			view.Tip(h);
			frame.Tip(h);
		}
		Sync();
		view.Refresh();
		frame.Refresh();
	}
}

void DisplayPopup::Set(Ctrl *ctrl, const Rect& item, const Value& v, const Display *display, Color ink, Color paper, dword style, int margin)
{
	if(IsNull(Check(ctrl, item, v, display, margin)))
		return; // precheck to avoid creating / deleting popup too often, avoid flooding timer with PostCallback
	if(!popup) {
		popup.Create();
		popup->usedisplaystdsize = usedisplaystdsize;
		Ptr<DisplayPopup> pt = this;
		popup->WhenClose << [=] { PostCallback([=] { if(pt) pt->popup.Clear(); }); };
	}
	popup->Set(ctrl, item, v, display, ink, paper, style, margin);
}

void DisplayPopup::Cancel()
{
	if(popup) {
		popup->display = nullptr;
		popup->Sync();
	}
}

bool DisplayPopup::IsOpen()
{
	return popup && popup->view.GetParent();
}

bool DisplayPopup::HasMouse()
{ // TODO: remove
	return popup && (popup->view.HasMouse() || popup->frame.HasMouse());
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
		popup.Clear();
}

}
