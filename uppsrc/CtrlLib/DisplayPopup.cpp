#include "CtrlLib.h"

namespace Upp {

Rect           DisplayPopup::screen_rect;
Ptr<Ctrl>      DisplayPopup::ctrl;
Rect           DisplayPopup::item;
Value          DisplayPopup::value;
Color          DisplayPopup::paper;
Color          DisplayPopup::ink;
dword          DisplayPopup::style;
const Display *DisplayPopup::display;
int            DisplayPopup::margin;
bool           DisplayPopup::usedisplaystdsize_s;
	
DisplayPopup::DisplayPopup()
{
	ONCELOCK {
		Ctrl::InstallStateHook(StateHook);
		Ctrl::InstallMouseHook(MouseHook);
		Ctrl::InstallPaintHook(PaintHook);
	}
}

void DisplayPopup::PaintHook(Ctrl *tw, Draw& w, const Rect& clip)
{
	if(ctrl && !IsNull(screen_rect) && (tw == ctrl->GetTopCtrl() || tw && tw != ctrl->GetOwner())) {
		Rect r = screen_rect - tw->GetScreenRect().TopLeft();
		DrawFrame(w, r, SBlack());
		r.Deflate(1, 1);
		w.Clip(r);
		w.DrawRect(r, SColorPaper);
		if(display) {
			display->PaintBackground(w, r, value, ink, paper, style);
			r.left += margin;
			if(usedisplaystdsize_s)
				r.top += (r.Height() - display->GetStdSize(value).cy) / 2;
			display->Paint(w, r, value, ink, paper, style);
		}
	}
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

void DisplayPopup::RefreshRect()
{
	if(ctrl && !IsNull(screen_rect)) {
		Ctrl *top = ctrl->GetTopCtrl();
		top->RefreshFrame(screen_rect - top->GetScreenRect().TopLeft());
		Ctrl *owner = top->GetOwner();
		if(owner)
			owner->RefreshFrame(screen_rect - owner->GetScreenRect().TopLeft());
	}
}

void DisplayPopup::Sync()
{
	Rect r = Check(ctrl, item, value, display, margin);
	RefreshRect();
	if(IsNull(r))
		screen_rect = Null;
	else {
		Ctrl *top = ctrl->GetTopCtrl();
		Size sz = display->GetStdSize(value);
		Rect wa = top->GetScreenRect();
		r.right = min(wa.right, r.left + sz.cx + 2 * margin);
		if(wa.bottom < r.top + sz.cy)
			return;
		r.bottom = max(r.bottom, r.top + sz.cy);
		Ctrl *owner = top->GetOwner();
		if(owner) {
			Rect owa = owner->GetScreenRect();
			if(owa.bottom >= r.bottom)
				r.right = min(max(wa.right, owa.right), r.left + sz.cx + 2 * margin);
		}
		screen_rect = r.Inflated(1, 1);
		RefreshRect();
	}
}

bool DisplayPopup::StateHook(Ctrl *, int reason)
{
	if(reason == Ctrl::FOCUS)
		Sync();
	return false;
}


bool DisplayPopup::MouseHook(Ctrl *, bool, int, Point, int, dword)
{
	Sync();
	return false;
}

void DisplayPopup::Set(Ctrl *_ctrl, const Rect& _item,
                            const Value& _value, const Display *_display,
                            Color _ink, Color _paper, dword _style, int _margin)
{
	if(!GUI_ToolTips())
		return;
	
	if(item != _item || ctrl != _ctrl || value != _value || display != _display || ink != _ink ||
	   paper != _paper || style != _style) {
		RefreshRect();
		item = _item;
		ctrl = _ctrl;
		value = _value;
		display = _display;
		ink = _ink;
		paper = _paper;
		style = _style;
		margin = _margin;
		usedisplaystdsize_s = usedisplaystdsize;
		Sync();
	}
}

void DisplayPopup::Cancel()
{
	screen_rect = Null;
	Sync();
}

bool DisplayPopup::IsOpen()
{
	return ctrl && !IsNull(screen_rect);
}

bool DisplayPopup::HasMouse()
{
	return ctrl && !IsNull(screen_rect) && screen_rect.Contains(GetMousePos());
}

void DisplayPopup::UseDisplayStdSize()
{
	usedisplaystdsize = true;
}

}
