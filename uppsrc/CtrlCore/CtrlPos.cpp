#include "CtrlCore.h"

namespace Upp {

#define LLOG(x)    // DLOG(x)
#define LTIMING(x) // RTIMING(x)

bool Ctrl::Logc::IsEmpty() const {
	return GetAlign() == SIZE ? GetB() <= GetA() : GetB() <= 0;
}

Size Ctrl::PosVal(int v) const {
	switch(v) {
	case MINSIZE: return GetMinSize();
	case STDSIZE: return GetStdSize();
	case MAXSIZE: return GetMaxSize();
	}
	return Size(v, v);
}

void Ctrl::Lay1(int& pos, int& r, int align, int a, int b, int sz) const
{
	pos = a;
	int size = b;
	switch(align) {
	case CENTER:
		pos = (sz - b) / 2 + a;
		break;
	case RIGHT:
		pos = sz - (a + b);
		break;
	case SIZE:
		size = sz - (a + b);
		break;
	}
	r = pos + max(size, 0);
}

Rect Ctrl::CalcRect(LogPos pos, const Rect& prect, const Rect& pview) const
{
	Rect r;
	Size sz = InFrame() ? prect.Size() : pview.Size();
	Lay1(r.left, r.right, pos.x.GetAlign(),
	     PosVal(pos.x.GetA()).cx, PosVal(pos.x.GetB()).cx, sz.cx);
	Lay1(r.top, r.bottom, pos.y.GetAlign(),
	     PosVal(pos.y.GetA()).cy, PosVal(pos.y.GetB()).cy, sz.cy);
	return r;
}

Rect Ctrl::CalcRect(const Rect& prect, const Rect& pview) const
{
	return CalcRect(pos, prect, pview);
}

Rect Ctrl::OffsetMegaRect(Rect r) const
{
	if(megarect)
		r.Offset(GetIntAttr(ATTR_MEGARECT_X, 0), GetIntAttr(ATTR_MEGARECT_Y, 0));
	return r;
}

Rect Ctrl::GetRect() const
{
	Rect r = rect;
	if(megarect)
		r = OffsetMegaRect(r);
	return r;
}

Rect Ctrl::GetView() const
{
	GuiLock __;
	int n = GetFrameCount();
	return n == 0 ? Rect(Size(rect.Size())) : Rect(GetFrame0(n - 1).GetView());
}

Size Ctrl::GetSize() const
{
	return GetView().GetSize();
}

Rect  Ctrl::GetScreenRect() const
{
	GuiLock __;
	Rect r = GetRect();
	Ctrl *parent = GetParent();
	if(parent) {
		Rect pr = inframe ? parent->GetScreenRect() : parent->GetScreenView();
		r = r + pr.TopLeft();
	}
	else
		GuiPlatformGetTopRect(r);
	return r;
}

Rect  Ctrl::GetScreenView() const
{
	Rect r = GetScreenRect();
	return GetView() + r.TopLeft();
}

Rect  Ctrl::GetVisibleScreenRect() const
{
	GuiLock __;
	Rect r = GetRect();
	Ctrl *parent = GetParent();
	if(parent) {
		Rect pr = inframe ? parent->GetVisibleScreenRect() : parent->GetVisibleScreenView();
		Rect pr1 = inframe ? parent->GetScreenRect() : parent->GetScreenView();
		r = (r + pr1.TopLeft()) & pr;
	}
	else
		GuiPlatformGetTopRect(r);
	return r;
}

Rect  Ctrl::GetVisibleScreenView() const
{
	Rect r = GetVisibleScreenRect();
	return (GetView() + r.TopLeft()) & r;
}

Size  Ctrl::AddFrameSize(int cx, int cy) const
{
	GuiLock __;
	Size sz = Size(cx, cy);
	for(int i = GetFrameCount() - 1; i >= 0; i--)
		GetFrame0(i).frame->FrameAddSize(sz);
	return sz;
}

int EditFieldIsThin();

Size Ctrl::GetMinSize() const
{
	int fcy = Draw::GetStdFontCy();
	return AddFrameSize(fcy / 2, fcy + 2 + 2 * EditFieldIsThin());
}

Size Ctrl::GetStdSize() const
{
	Size sz = GetMinSize();
	sz.cx *= 10;
	return sz;
}

Size Ctrl::GetMaxSize() const
{
	return GetVirtualScreenArea().Size();
}

void Ctrl::SyncLayout(int force)
{
	GuiLock __;
	if(destroying)
		return;
	LLOG("SyncLayout " << Name() << " size: " << GetSize());
	fullrefresh = false;
	bool refresh = false;
	Rect oview = GetView();
	Rect view = GetRect().Size();
	overpaint = OverPaint();
	int n = GetFrameCount();
	for(int i = 0; i < n; i++) {
		Frame& f = GetFrame0(i);
		f.frame->FrameLayout(view);
		if(view != f.GetView()) {
			f.SetView(view);
			refresh = true;
		}
		int q = f.frame->OverPaint();
		if(q > overpaint) overpaint = q;
	}
	if(oview.Size() != view.Size() || force > 1) {
		for(Ctrl& q : *this) {
			q.rect = q.CalcRect(rect, view);
			LLOG("Layout set rect " << q.Name() << " " << q.rect);
			q.SyncLayout(force > 1 ? force : 0);
		}
		Refresh();
	}
	if(oview != view || force) {
		State(LAYOUTPOS);
		Layout();
	}
	if(refresh)
		RefreshFrame();
}

void Ctrl::RefreshParentLayout()
{
	Ctrl *parent = GetParent();
	if(parent)
		parent->RefreshLayout();
}

void Ctrl::UpdateParentLayout()
{
	Ctrl *parent = GetParent();
	if(parent)
		parent->UpdateLayout();
}

int Ctrl::FindMoveCtrl(const VectorMap<Ctrl *, MoveCtrl>& m, Ctrl *x)
{
	int q = m.Find(x);
	return q >= 0 && m[q].ctrl ? q : -1;
}

Ctrl::MoveCtrl *Ctrl::FindMoveCtrlPtr(VectorMap<Ctrl *, MoveCtrl>& m, Ctrl *x)
{
	int q = FindMoveCtrl(m, x);
	return q >= 0 ? &m[q] : NULL;
}

void Ctrl::SetPos0(LogPos p, bool _inframe)
{
	GuiLock __;
	if(p == pos && inframe == _inframe) return;
	Ctrl *parent = GetParent();
	if(parent && !IsDHCtrl()) {
		if(!globalbackbuffer) {
			Rect from = GetRect().Size();
			Top *top = GetTopRect(from, true)->GetTop();
			if(top) {
				LTIMING("SetPos0 MoveCtrl");
				pos = p;
				inframe = _inframe;
				Rect to = GetRect().Size();
				UpdateRect0();
				GetTopRect(to, true);
				MoveCtrl *s = FindMoveCtrlPtr(top->scroll_move, this);
				if(s && s->from == from && s->to == to) {
					s->ctrl = NULL;
					LLOG("SetPos Matched " << from << " -> " << to);
				}
				else {
					MoveCtrl& m = top->move.Add(this);
					m.ctrl = this;
					m.from = from;
					m.to = to;
					LLOG("SetPos Add " << UPP::Name(this) << from << " -> " << to);
				}
				StateH(POSITION);
				return;
			}
		}
		RefreshFrame();
	}
	pos = p;
	inframe = _inframe;
	UpdateRect();
	StateH(POSITION);
}

void Ctrl::UpdateRect0(bool sync)
{
	GuiLock __;
	LTIMING("UpdateRect0");
	Ctrl *parent = GetParent();
	if(parent)
		rect = CalcRect(parent->GetRect(), parent->GetView());
	else {
		Rect pwa = GetPrimaryWorkArea();
		rect = OffsetMegaRect(CalcRect(pwa, pwa));
	}
	LLOG("UpdateRect0 " << Name() << " to " << rect);
	LTIMING("UpdateRect0 SyncLayout");
	if(sync)
		SyncLayout();
}


void Ctrl::UpdateRect(bool sync)
{
	GuiLock __;
	UpdateRect0(sync);
	if(GetParent()) RefreshFrame();
}

Ctrl& Ctrl::SetPos(LogPos p, bool _inframe)
{
	GuiLock __;
	Ctrl *parent = GetParent();
	if(p != pos || inframe != _inframe) {
		if(parent || !IsOpen())
			SetPos0(p, _inframe);
		else {
			ASSERT(p.x.GetAlign() == ALIGN_LEFT);
			ASSERT(p.y.GetAlign() == ALIGN_TOP);
			Rect pwa = GetPrimaryWorkArea();
			WndSetPos(OffsetMegaRect(CalcRect(p, pwa, pwa)));
			StateH(POSITION);
		}
	}
	return *this;
}

Ctrl& Ctrl::SetPos(LogPos p)
{
	return SetPos(p, false);
}

Ctrl& Ctrl::SetPosX(Logc x)
{
	return SetPos(LogPos(x, pos.y));
}

Ctrl& Ctrl::SetPosY(Logc y)
{
	return SetPos(LogPos(pos.x, y));
}

Ctrl& Ctrl::SetFramePos(LogPos p)
{
	return SetPos(p, true);
}

Ctrl& Ctrl::SetFramePosX(Logc x) {
	return SetPos(LogPos(x, pos.y), true);
}

Ctrl& Ctrl::SetFramePosY(Logc y) {
	return SetPos(LogPos(pos.x, y), true);
}

void  Ctrl::MegaRect(Rect& r)
{ // support 32-bit position of window (not size - just clamp it)
	Size sz = r.GetSize();
	sz.cx = clamp(sz.cx, -16000, 16000);
	sz.cy = clamp(sz.cy, -16000, 16000);
	if(abs(r.left) > 16000 || abs(r.top) > 16000) {
		megarect = true;
		SetIntAttr(ATTR_MEGARECT_X, r.left);
		SetIntAttr(ATTR_MEGARECT_Y, r.top);
		r = sz;
	}
	else {
		megarect = false;
		r.SetSize(sz);
	}
}

void  Ctrl::SetRect(int x, int y, int cx, int cy)
{
	LLOG("SetRect " << Name() << " rect: " << RectC(x, y, cx, cy));
	Rect r = RectC(x, y, cx, cy);
	MegaRect(r);
	SetPos(LogPos(PosLeft(r.left, r.Width()), PosTop(r.top, r.Height())), false);
}

void  Ctrl::SetWndRect(const Rect& r)
{
	LLOG("SetWndRect " << Name() << " rect: " << r << " (Ctrl::GetRect = " << GetRect() << ")");
	SetPos0(LogPos(PosLeft(r.left, r.Width()), PosTop(r.top, r.Height())), false);
}

void Ctrl::SetRect(const Rect& r)
{
	LLOG("SetRect " << Name() << " rect: " << r << " (Ctrl::GetRect = " << GetRect() << ")");
	SetRect(r.left, r.top, r.Width(), r.Height());
}

void Ctrl::SetRectX(int x, int cx) {
	SetPosX(PosLeft(x, cx));
}

void Ctrl::SetRectY(int y, int cy) {
	SetPosY(PosTop(y, cy));
}

void  Ctrl::SetFrameRect(int x, int y, int cx, int cy) {
	SetFramePos(PosLeft(x, cx), PosTop(y, cy));
}

void Ctrl::SetFrameRect(const Rect& r) {
	SetFrameRect(r.left, r.top, r.Width(), r.Height());
}

void Ctrl::SetFrameRectX(int x, int cx) {
	SetFramePosX(PosLeft(x, cx));
}

void Ctrl::SetFrameRectY(int y, int cy) {
	SetFramePosY(PosTop(y, cy));
}

Ctrl& Ctrl::LeftPos(int a, int size) {
	return SetPosX(PosLeft(a, size));
}

Ctrl& Ctrl::RightPos(int a, int size) {
	return SetPosX(PosRight(a, size));
}

Ctrl& Ctrl::TopPos(int a, int size) {
	return SetPosY(PosTop(a, size));
}

Ctrl& Ctrl::BottomPos(int a, int size) {
	return SetPosY(PosBottom(a, size));
}

Ctrl& Ctrl::HSizePos(int a, int b) {
	return SetPosX(PosSize(a, b));
}

Ctrl& Ctrl::VSizePos(int a, int b) {
	return SetPosY(PosSize(a, b));
}

Ctrl& Ctrl::SizePos() {
	return HSizePos().VSizePos();
}

Ctrl& Ctrl::HCenterPos(int size, int delta) {
	return SetPosX(PosCenter(size, delta));
}

Ctrl& Ctrl::VCenterPos(int size, int delta) {
	return SetPosY(PosCenter(size, delta));
}

Ctrl& Ctrl::LeftPosZ(int a, int size) {
	return LeftPos(HorzLayoutZoom(a), HorzLayoutZoom(size));
}

Ctrl& Ctrl::RightPosZ(int a, int size) {
	return RightPos(HorzLayoutZoom(a), HorzLayoutZoom(size));
}

Ctrl& Ctrl::TopPosZ(int a, int size) {
	return TopPos(VertLayoutZoom(a), VertLayoutZoom(size));
}

Ctrl& Ctrl::BottomPosZ(int a, int size) {
	return BottomPos(VertLayoutZoom(a), VertLayoutZoom(size));
}

Ctrl& Ctrl::HSizePosZ(int a, int b) {
	return HSizePos(HorzLayoutZoom(a), HorzLayoutZoom(b));
}

Ctrl& Ctrl::VSizePosZ(int a, int b) {
	return VSizePos(VertLayoutZoom(a), VertLayoutZoom(b));
}

Ctrl& Ctrl::HCenterPosZ(int size, int delta) {
	return HCenterPos(HorzLayoutZoom(size), HorzLayoutZoom(delta));
}

Ctrl& Ctrl::VCenterPosZ(int size, int delta) {
	return VCenterPos(VertLayoutZoom(size), VertLayoutZoom(delta));
}

Rect Ctrl::GetWorkArea(Point pt)
{
	GuiLock __;
	Array<Rect> rc;
	GetWorkArea(rc);
	for(int i = 0; i < rc.GetCount(); i++)
		if(rc[i].Contains(pt))
			return rc[i];
	return GetPrimaryWorkArea();
}

Rect Ctrl::StdGetWorkArea() const
{
	GuiLock __;

	const Ctrl *top = GetTopCtrl();
	if(top && top->IsOpen())
		return GetWorkArea(top->GetScreenRect().TopLeft());
	return GetPrimaryWorkArea();
}

}