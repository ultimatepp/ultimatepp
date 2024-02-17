#include "CtrlCore.h"

namespace Upp {

#define LLOG(x)  // DLOG(x)

Ptr<Ctrl> Ctrl::eventCtrl;
Ptr<Ctrl> Ctrl::mouseCtrl;
Ptr<Ctrl> Ctrl::captureCtrl;
Ptr<Ctrl> Ctrl::repeatTopCtrl;
Point     Ctrl::repeatMousePos;
bool      Ctrl::ignoreclick;
bool      Ctrl::ignoremouseup;
bool      Ctrl::mouseinframe;
bool      Ctrl::mouseinview;
Point     Ctrl::mousepos;
Point     Ctrl::leftmousepos = Null;
Point     Ctrl::rightmousepos = Null;
Point     Ctrl::middlemousepos = Null;

PenInfo   Ctrl::pen;
bool      Ctrl::is_pen_event;

dword GetMouseFlags() {
	dword style = 0;
	if(GetAlt()) style |= K_ALT;
	if(GetCtrl()) style |= K_CTRL;
	if(GetShift()) style |= K_SHIFT;
	if(GetMouseLeft()) style |= K_MOUSELEFT;
	if(GetMouseRight()) style |= K_MOUSERIGHT;
	if(GetMouseMiddle()) style |= K_MOUSEMIDDLE;
	return style;
}

void Ctrl::LogMouseEvent(const char *f, const Ctrl *ctrl, int event, Point p, int zdelta, dword keyflags)
{
	if(!Ini::user_log)
		return;
	String txt = f;
	txt += (event & BUTTON) == RIGHT ? "RIGHT" : (event & BUTTON) == MIDDLE ? "MIDDLE" : "LEFT";
	switch(event & ACTION) {
	case DOWN:        txt << "DOWN"; break;
	case UP:          txt << "UP"; break;
	case DOUBLE:      txt << "DOUBLE"; break;
	case MOUSEWHEEL:  txt << "WHEEL"; break;
	default:
		return;
	}
	txt << ' ' << p << " (";
	if(keyflags & K_ALT)
		txt << " ALT";
	if(keyflags & K_CTRL)
		txt << " CTRL";
	if(keyflags & K_SHIFT)
		txt << " SHIFT";
	if(keyflags & K_MOUSELEFT)
		txt << " LEFT";
	if(keyflags & K_MOUSERIGHT)
		txt << " RIGHT";
	if(keyflags & K_MOUSEMIDDLE)
		txt << " MIDDLE";
	txt << " ) " << Desc(ctrl);
	USRLOG(txt);
	LLOG(txt);
}

Image Ctrl::FrameMouseEventH(int event, Point p, int zdelta, dword keyflags)
{
	GuiLock __;
	Ptr<Ctrl> this_ = this;
	for(int i = 0; i < mousehook().GetCount(); i++)
		if(this_ && (*mousehook()[i])(this, true, event, p, zdelta, keyflags))
			return Image::Arrow();
	if(this_)
		LogMouseEvent("FRAME ", this, event, p, zdelta, keyflags);
	eventCtrl = this_;
	Ctrl *parent = GetParent();
	if(parent && this_)
		parent->ChildFrameMouseEvent(this, event, p, zdelta, keyflags);
	return this_ ? FrameMouseEvent(event, p, zdelta, keyflags) : Image();
}

Image Ctrl::FrameMouseEvent(int event, Point p, int zdelta, dword keyflags)
{
	return Image::Arrow();
}

static bool sPropagated;

Image Ctrl::MouseEvent0(int event, Point p, int zdelta, dword keyflags)
{
	GuiLock __;
	Ptr<Ctrl> this_ = this;
	bool pb = sPropagated;
	sPropagated = false;
	Image m = this_ ? MouseEvent(event, p, zdelta, keyflags) : Image();
	Ctrl *parent = this_ ? this_->GetParent() : NULL;
	if(((event == MOUSEWHEEL)||(event == MOUSEHWHEEL)) && !sPropagated && this_ && parent)
		parent->ChildMouseEvent(this, event, p, zdelta, keyflags);
	sPropagated = pb;
	return m;
}

Image Ctrl::MouseEventH(int event, Point p, int zdelta, dword keyflags)
{
	GuiLock __;
	Ptr<Ctrl> this_ = this;
	for(int i = 0; i < mousehook().GetCount(); i++)
		if(this_ && (*mousehook()[i])(this, false, event, p, zdelta, keyflags))
			return Image::Arrow();
	if(this_)
		LogMouseEvent(NULL, this, event, p, zdelta, keyflags);
	Ctrl *parent = this_ ? this_->GetParent() : NULL;
	if(this_ && parent && event != MOUSEWHEEL && event != MOUSEHWHEEL)
		parent->ChildMouseEvent(this, event, p, zdelta, keyflags);
	return MouseEvent0(event, p, zdelta, keyflags);
}

void Ctrl::MouseWheel(Point p, int zd, dword kf)
{
	Ctrl *parent = GetParent();
	if(parent) {
		p += GetScreenView().TopLeft();
		Rect r = parent->GetScreenView();
		if(r.Contains(p)) {
			parent->MouseEvent0(MOUSEWHEEL, p - r.TopLeft(), zd, kf);
			sPropagated = true;
		}
	}
}

void Ctrl::HorzMouseWheel(Point p, int zd, dword kf)
{
	Ctrl *parent = GetParent();
	if(parent) {
		p += GetScreenView().TopLeft();
		Rect r = parent->GetScreenView();
		if(r.Contains(p)) {
			parent->MouseEvent0(MOUSEHWHEEL, p - r.TopLeft(), zd, kf);
			sPropagated = true;
		}
	}
}

void Ctrl::ChildFrameMouseEvent(Ctrl *child, int event, Point p, int zdelta, dword keyflags)
{
	GuiLock __;
	Ctrl *parent = GetParent();
	if(parent)
		parent->ChildFrameMouseEvent(child, event, p, zdelta, keyflags);
}

void Ctrl::ChildMouseEvent(Ctrl *child, int event, Point p, int zdelta, dword keyflags)
{
	GuiLock __;
	Ctrl *parent = GetParent();
	if(parent)
		parent->ChildMouseEvent(child, event, p, zdelta, keyflags);
}

Image Ctrl::MouseEvent(int event, Point p, int zdelta, dword keyflags)
{
	LLOG("MouseEvent " << UPP::Name(this) << " " << FormatIntHex(event));
	switch(event) {
	case MOUSEENTER:
		MouseEnter(p, keyflags);
		break;
	case MOUSEMOVE:
		MouseMove(p, keyflags);
		break;
	case LEFTDOWN:
		LeftDown(p, keyflags);
		break;
	case LEFTDOUBLE:
		LeftDouble(p, keyflags);
		break;
	case LEFTDRAG:
		LeftDrag(p, keyflags);
		break;
	case LEFTHOLD:
		LeftHold(p, keyflags);
		break;
	case LEFTTRIPLE:
		LeftTriple(p, keyflags);
		break;
	case LEFTREPEAT:
		LeftRepeat(p, keyflags);
		break;
	case LEFTUP:
		LeftUp(p, keyflags);
		break;
	case RIGHTDRAG:
		RightDrag(p, keyflags);
		break;
	case RIGHTHOLD:
		RightHold(p, keyflags);
		break;
	case RIGHTTRIPLE:
		RightTriple(p, keyflags);
		break;
	case RIGHTDOWN:
		RightDown(p, keyflags);
		break;
	case RIGHTDOUBLE:
		RightDouble(p, keyflags);
		break;
	case RIGHTREPEAT:
		RightRepeat(p, keyflags);
		break;
	case RIGHTUP:
		RightUp(p, keyflags);
		break;
	case MIDDLEDRAG:
		MiddleDrag(p, keyflags);
		break;
	case MIDDLEHOLD:
		MiddleHold(p, keyflags);
		break;
	case MIDDLETRIPLE:
		MiddleTriple(p, keyflags);
		break;
	case MIDDLEDOWN:
		MiddleDown(p, keyflags);
		break;
	case MIDDLEDOUBLE:
		MiddleDouble(p, keyflags);
		break;
	case MIDDLEREPEAT:
		MiddleRepeat(p, keyflags);
		break;
	case MIDDLEUP:
		MiddleUp(p, keyflags);
		break;
	case MOUSELEAVE:
		MouseLeave();
		break;
	case MOUSEWHEEL:
		MouseWheel(p, zdelta, keyflags);
		break;
	case MOUSEHWHEEL:
		HorzMouseWheel(p, zdelta, keyflags);
		break;
	case CURSORIMAGE:
		return CursorImage(p, keyflags);
	}
	return Image::Arrow();
}

Image Ctrl::CursorImage(Point p, dword keyflags)
{
	return Image::Arrow();
}

void Ctrl::IgnoreMouseClick()
{
	GuiLock __;
	LLOG("IgnoreMouseClick");
	ignoreclick = true;
	KillRepeat();
}

void Ctrl::IgnoreMouseUp()
{
	GuiLock __;
	LLOG("Ctrl::IgnoreMouseUp");
	if(GetMouseLeft() || GetMouseRight() || GetMouseMiddle()) {
		IgnoreMouseClick();
		ignoremouseup = true;
	}
}

void Ctrl::UnIgnoreMouse()
{
	GuiLock __;
	LLOG("Ctrl::EndIgnore");
	KillRepeat();
	ignoreclick = false;
	ignoremouseup = false;
}

void Ctrl::EndIgnore()
{
	GuiLock __;
	LLOG("Ctrl::EndIgnore");
	if(GetMouseLeft() || GetMouseRight() || GetMouseMiddle()) return;
	KillRepeat();
	ignoreclick = false;
	ignoremouseup = false;
}

bool Ctrl::IsMouseActive() const
{
	GuiLock __;
	return IsVisible() && IsEnabled() && IsOpen() && !ignoremouse;
}

Ctrl *Ctrl::ChildFromPoint(Point& pt) const
{
	GuiLock __;
	Ctrl *q;
	Point p = pt;
	Rect view = GetView();
	if(view.Contains(p)) {
		Point vp = p - view.TopLeft();
		for(q = GetLastChild(); q; q = q->GetPrev()) {
			if(q->InView() && q->IsMouseActive()) {
				Rect r = q->GetRect();
				if(r.Contains(vp)) {
					pt = vp - r.TopLeft();
					return q;
				}
			}
		}
		return NULL;
	}
	for(q = GetLastChild(); q; q = q->GetPrev()) {
		if(q->InFrame() && q->IsMouseActive()) {
			Rect r = q->GetRect();
			if(r.Contains(p)) {
				pt = p - r.TopLeft();
				return q;
			}
		}
	}
	return NULL;
}

Image Ctrl::MEvent0(int e, Point p, int zd)
{
	GuiLock __;
	LLOG("MEvent0 " << Name() << " event: " << FormatIntHex(e, 0) << " point:" << p);
	Ptr<Ctrl> _this = this;
	mousepos = p;
	dword mm = 0;
	if((e & ACTION) == DOUBLE)
		mm |= K_MOUSEDOUBLE;
	if((e & ACTION) == TRIPLE)
		mm |= K_MOUSETRIPLE;
	if(is_pen_event)
		mm |= K_PEN;
	Rect view = GetView();
	if(mouseCtrl != this) {
		if(mouseCtrl) {
			Ptr<Ctrl> mousectrl = mouseCtrl;
			mousectrl->MouseEventH(MOUSELEAVE, Point(0, 0), zd, GetMouseFlags() | mm);
			if(mousectrl)
				mousectrl->FrameMouseEventH(MOUSELEAVE, Point(0, 0), zd, GetMouseFlags() | mm);
		}
		mouseinframe = mouseinview = false;
		if(_this) {
			mouseCtrl = _this;
			mouseinframe = true;
			FrameMouseEventH(MOUSEENTER, p, zd, GetMouseFlags() | mm);
		}
	}
	bool inview = view.Contains(p);
	if(inview != mouseinview && _this) {
		mouseinview = inview;
		MouseEventH(inview ? MOUSEENTER : MOUSELEAVE, p, zd, GetMouseFlags() | mm);
	}
	if(_this) {
		if(view.Contains(p) || HasCapture()) {
			p -= view.TopLeft();
			return MouseEventH(e, p, zd, GetMouseFlags() | mm);
		}
		else
			return FrameMouseEventH(e, p, zd, GetMouseFlags() | mm);
	}
	return Image::Arrow();
}

void    Ctrl::LRepeat() {
	GuiLock __;
	if(repeatTopCtrl && GetMouseLeft()) {
		if(repeatTopCtrl->HasFocusDeep())
			repeatTopCtrl->DispatchMouseEvent(LEFTREPEAT, repeatMousePos, 0);
	}
	else
		KillRepeat();
	LLOG("LRepeat " << UPP::Name(mouseCtrl));
}

static int sDistMax(Point a, Point b)
{
	return IsNull(a) ? INT_MAX : max(abs(a.x - b.x), abs(a.y - b.y));
}

static int sDistMin(Point a, Point b)
{
	return IsNull(a) ? -1 : max(abs(a.x - b.x), abs(a.y - b.y));
}

void    Ctrl::LRep() {
	LLOG("LRep");
	UPP::SetTimeCallback(-GetKbdSpeed(), callback(&Ctrl::LRepeat), &mousepos);
}

bool    Ctrl::NotDrag(Point p)
{
	return mouseCtrl &&
	       sDistMax(p, mousepos + mouseCtrl->GetRect().TopLeft()) < GUI_DragDistance();
}

void    Ctrl::LHold() {
	GuiLock __;
	if(NotDrag(leftmousepos) && repeatTopCtrl && GetMouseLeft())
		repeatTopCtrl->DispatchMouseEvent(LEFTHOLD, repeatMousePos, 0);
}

void    Ctrl::RRepeat() {
	GuiLock __;
	if(repeatTopCtrl && GetMouseRight()) {
		if(repeatTopCtrl->HasFocusDeep())
			repeatTopCtrl->DispatchMouseEvent(RIGHTREPEAT, repeatMousePos, 0);
	}
	else
		KillRepeat();
}

void    Ctrl::RRep() {
	UPP::SetTimeCallback(-GetKbdSpeed(), callback(&Ctrl::RRepeat), &mousepos);
}

void    Ctrl::RHold() {
	GuiLock __;
	if(NotDrag(rightmousepos) && repeatTopCtrl && GetMouseRight())
		repeatTopCtrl->DispatchMouseEvent(RIGHTHOLD, repeatMousePos, 0);
}

void    Ctrl::MRepeat() {
	GuiLock __;
	if(repeatTopCtrl && GetMouseMiddle()) {
		if(repeatTopCtrl->HasFocusDeep())
			repeatTopCtrl->DispatchMouseEvent(MIDDLEREPEAT, repeatMousePos, 0);
	}
	else
		KillRepeat();
}

void    Ctrl::MRep() {
	GuiLock __;
	UPP::SetTimeCallback(-GetKbdSpeed(), callback(&Ctrl::MRepeat), &mousepos);
}

void    Ctrl::MHold() {
	GuiLock __;
	if(NotDrag(middlemousepos) && repeatTopCtrl && GetMouseMiddle())
		repeatTopCtrl->DispatchMouseEvent(MIDDLEHOLD, repeatMousePos, 0);
}

void    Ctrl::KillRepeat() {
	GuiLock __;
	LLOG("Ctrl::KillRepeat");
	UPP::KillTimeCallback(&mousepos);
	repeatTopCtrl = NULL;
	leftmousepos = Null;
	rightmousepos = Null;
	middlemousepos = Null;
}

bool    Ctrl::HasMouse() const
{
	GuiLock __;
	return mouseCtrl == this;
}

bool    Ctrl::HasMouseDeep() const
{
	GuiLock __;
	return mouseCtrl == this || HasChildDeep(mouseCtrl);
}

Ctrl   *Ctrl::GetMouseCtrl()
{
	GuiLock __;
	return mouseCtrl;
}

bool    Ctrl::HasMouseInFrame(const Rect& r) const
{
	GuiLock __;
	if(!HasMouse())
		return false;
	Rect q = GetVisibleScreenRect();
	q = r.Offseted(q.TopLeft()) & q;
	return q.Contains(GetMousePos());
}

bool    Ctrl::HasMouseIn(const Rect& r) const
{
	GuiLock __;
	if(!HasMouse())
		return false;
	return (r.Offseted(GetScreenView().TopLeft()) & GetVisibleScreenView()).Contains(GetMousePos());
}

Point Ctrl::GetMouseViewPos() const
{
	GuiLock __;
	return GetMousePos() - GetVisibleScreenView().TopLeft();
}

void    Ctrl::DoCursorShape() {
	GuiLock __;
	Image m = CursorOverride();
	if(IsNull(m))
		if(mouseCtrl)
			SetMouseCursor(mouseCtrl->MEvent0(CURSORIMAGE, mousepos, 0));
		else
			SetMouseCursor(Image::Arrow());
	else
		SetMouseCursor(m);
}

void    Ctrl::CheckMouseCtrl() {
	LLOG("CheckMouseCtrl " << mouseCtrl);
	GuiLock __;
	Point p = GetMousePos();
	if(mouseCtrl) {
		Rect r = mouseCtrl->GetScreenRect();
		LLOG("CheckMouseCtrl mouseCtrl " << UPP::Name(mouseCtrl) << " " << r);
		if(!mouseCtrl->HasCapture() && !r.Contains(p)) {
			Ptr<Ctrl> mousectrl = mouseCtrl;
			if(mouseinview)
				mousectrl->MouseEventH(MOUSELEAVE, p - mousectrl->GetScreenView().TopLeft(),
				                       0, GetMouseFlags());
			if(mouseinframe && mousectrl)
				mousectrl->FrameMouseEventH(MOUSELEAVE, p - r.TopLeft(),
				                            0, GetMouseFlags());
			mouseinview = mouseinframe = false;
			mouseCtrl = NULL;
			leftmousepos = rightmousepos = middlemousepos = Null;
			KillRepeat();
		}
	}
	DoCursorShape();
}

Point leftdblpos = Null, rightdblpos = Null, middledblpos = Null;
int leftdbltime = Null, rightdbltime = Null, middledbltime = Null;

bool sDblTime(int time)
{
	return !IsNull(time) && (int)msecs() - time < GUI_DblClickTime();
}

Image Ctrl::DispatchMouse(int e, Point p, int zd) {
	GuiLock __;
	EventLevelDo ___;
	if(((e == MOUSEWHEEL)||(e == MOUSEHWHEEL)) && !zd) // ignore non-scroll wheel events
		return Null;
	if(e == MOUSEMOVE && repeatTopCtrl == this) {
		if(sDistMin(leftmousepos, p) > GUI_DragDistance() && GetMouseLeft()) {
			DispatchMouseEvent(LEFTDRAG, leftmousepos, 0);
			leftmousepos = Null;
		}
		if(sDistMin(rightmousepos, p) > GUI_DragDistance() && GetMouseRight()) {
			DispatchMouseEvent(RIGHTDRAG, rightmousepos, 0);
			rightmousepos = Null;
		}
		if(sDistMin(middlemousepos, p) > GUI_DragDistance() && GetMouseMiddle()) {
			DispatchMouseEvent(MIDDLEDRAG, middlemousepos, 0);
			middlemousepos = Null;
		}
	}
	repeatMousePos = p;
	if(e == LEFTDOUBLE) {
		leftdbltime = msecs();
		leftdblpos = p;
		UPP::SetTimeCallback(GetKbdDelay(), callback(&Ctrl::LRep), &mousepos);
		repeatTopCtrl = this;
	}
	if(e == RIGHTDOUBLE) {
		rightdbltime = msecs();
		rightdblpos = p;
		UPP::SetTimeCallback(GetKbdDelay(), callback(&Ctrl::RRep), &mousepos);
		repeatTopCtrl = this;
	}
	if(e == MIDDLEDOUBLE) {
		middledbltime = msecs();
		middledblpos = p;
		UPP::SetTimeCallback(GetKbdDelay(), callback(&Ctrl::MRep), &mousepos);
		repeatTopCtrl = this;
	}
	if(e == LEFTDOWN) {
		LLOG("Ctrl::DispatchMouse: init left repeat for " << UPP::Name(this) << " at " << p);
		UPP::SetTimeCallback(GetKbdDelay(), callback(&Ctrl::LRep), &mousepos);
		UPP::SetTimeCallback(2 * GetKbdDelay(), callback(&Ctrl::LHold), &mousepos);
		leftmousepos = p;
		if(sDistMax(leftdblpos, p) < GUI_DragDistance() && sDblTime(leftdbltime))
			e = LEFTTRIPLE;
		repeatTopCtrl = this;
	}
	if(e == RIGHTDOWN) {
		LLOG("Ctrl::DispatchMouse: init right repeat for " << UPP::Name(this) << " at " << p);
		UPP::SetTimeCallback(GetKbdDelay(), callback(&Ctrl::RRep), &mousepos);
		UPP::SetTimeCallback(2 * GetKbdDelay(), callback(&Ctrl::RHold), &mousepos);
		rightmousepos = p;
		if(sDistMax(rightdblpos, p) < GUI_DragDistance() && sDblTime(rightdbltime))
			e = RIGHTTRIPLE;
		repeatTopCtrl = this;
	}
	if(e == MIDDLEDOWN) {
		LLOG("Ctrl::DispatchMouse: init middle repeat for " << UPP::Name(this) << " at " << p);
		UPP::SetTimeCallback(GetKbdDelay(), callback(&Ctrl::MRep), &mousepos);
		UPP::SetTimeCallback(2 * GetKbdDelay(), callback(&Ctrl::MHold), &mousepos);
		middlemousepos = p;
		if(sDistMax(middledblpos, p) < GUI_DragDistance() && sDblTime(middledbltime))
			e = MIDDLETRIPLE;
		repeatTopCtrl = this;
	}
	if(repeatTopCtrl != this)
		repeatTopCtrl = NULL;
	if(e == LEFTUP)
		leftmousepos = Null;
	if(e == RIGHTUP)
		rightmousepos = Null;
	if(e == MIDDLEUP)
		middlemousepos = Null;
	if(findarg(e, LEFTUP, RIGHTUP, MIDDLEUP) >= 0)
		KillRepeat();
	Image result = DispatchMouseEvent(e, p, zd);
	if(!GetMouseRight() && !GetMouseMiddle() && !GetMouseLeft())
		ReleaseCtrlCapture();
	return result;
}

Image Ctrl::DispatchMouseEvent(int e, Point p, int zd) {
	GuiLock __;
	if(captureCtrl && captureCtrl != this && captureCtrl->IsMouseActive()) {
		if(captureCtrl->IsEnabled())
			return captureCtrl->MEvent0(e, p + GetScreenRect().TopLeft() -
			                            captureCtrl->GetScreenRect().TopLeft(), zd);
		else
			return Image::Arrow();
	}
	if(!IsEnabled())
		return Image::Arrow();
	Ctrl *top = this;
	if(((e == MOUSEWHEEL)||(e == MOUSEHWHEEL)) && !GetParent()) {
		Ctrl *w = GetFocusCtrl();
		if(w) {
			top = w->GetTopCtrl();
			p = GetMousePos() - top->GetScreenRect().TopLeft();
		}
	}
	Ctrl *q = top->ChildFromPoint(p);
	return q ? q->DispatchMouseEvent(e, p, zd) : top->MEvent0(e, p, zd);
}

bool Ctrl::SetCapture() {
	GuiLock __;
	ReleaseCtrlCapture();
	if(!GetTopCtrl()->SetWndCapture()) return false;
	captureCtrl = mouseCtrl = this;
	return true;
}

bool Ctrl::ReleaseCapture() {
	GuiLock __;
	return this == captureCtrl && ReleaseCtrlCapture();
}

bool Ctrl::ReleaseCtrlCapture() {
	GuiLock __;
	LLOG("ReleaseCtrlCapture");
	if(captureCtrl) {
		captureCtrl->CancelMode();
		Ctrl *w = captureCtrl->GetTopCtrl();
		captureCtrl = NULL;
		CheckMouseCtrl();
		if(w->HasWndCapture()) {
			w->ReleaseWndCapture();
			return true;
		}
	}
	return false;
}

bool Ctrl::HasCapture() const {
	GuiLock __;
	if(captureCtrl != this)
		return false;
	return captureCtrl == this && GetTopCtrl()->HasWndCapture();
}

Ctrl * Ctrl::GetCaptureCtrl()
{
	GuiLock __;
	return captureCtrl && captureCtrl->GetTopCtrl()->HasWndCapture() ? captureCtrl : NULL;
}

Ctrl *Ctrl::GetVisibleChild(Ctrl *ctrl, Point p, bool pointinframe)
{
	GuiLock __;
	if(!pointinframe)
		p += ctrl->GetView().TopLeft();
	Ctrl *q;
	Rect view = ctrl->GetView();
	if(view.Contains(p)) {
		p -= view.TopLeft();
		for(q = ctrl->GetLastChild(); q; q = q->GetPrev()) {
			if(q->InView() && q->IsVisible()) {
				Rect r = q->GetRect();
				if(r.Contains(p))
					return GetVisibleChild(q, p - r.TopLeft(), true);
			}
		}
	}
	else
		for(q = ctrl->GetLastChild(); q; q = q->GetPrev()) {
			if(q->InFrame() && q->IsVisible()) {
				Rect r = q->GetRect();
				if(r.Contains(p))
					return GetVisibleChild(q, p - r.TopLeft(), true);
			}
		}
	return ctrl;
}

AutoWaitCursor::AutoWaitCursor(int& avg) : WaitCursor(avg >= 0), avg(avg) {
	time0 = msecs();
}

AutoWaitCursor::~AutoWaitCursor() {
	if(time0) avg = msecs() - time0 - 500;
	if(avg < -10000) avg = -10000;
	if(avg >  10000) avg = 10000;
}

Image& Ctrl::CursorOverride()
{
	GuiLock __;
	static Image m;
	return m;
}

Image Ctrl::OverrideCursor(const Image& m)
{
	GuiLock __;
	Image om = CursorOverride();
	CursorOverride() = m;
	DoCursorShape();
	if(!mouseCtrl)
		SetMouseCursor(IsNull(m) ? Image::Arrow() : m);
	return om;
}

void WaitCursor::Show() {
	if(flag)
		prev = Ctrl::OverrideCursor(Image::Wait());
	flag = false;
}

WaitCursor::WaitCursor(bool show) {
	LLOG("WaitCursor");
	flag = true;
	if(show) Show();
}

WaitCursor::~WaitCursor() {
	if(!flag)
		Ctrl::OverrideCursor(prev);
}

}
