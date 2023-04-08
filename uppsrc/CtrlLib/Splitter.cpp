#include "CtrlLib.h"

namespace Upp {

CH_STYLE(Splitter, Style, StyleDefault)
{
	width = Ctrl::HorzLayoutZoom(4);
	vert[0] = horz[0] = SColorFace();
	vert[1] = horz[1] = GUI_GlobalStyle() >= GUISTYLE_XP ? Blend(SColorHighlight, SColorFace)
	                                                     : SColorShadow();
	dots = true;
}

int Splitter::ClientToPos(Point p) const
{
	return minmax(vert ? 10000 * p.y / GetSize().cy : 10000 * p.x / GetSize().cx, 0, 9999);
}

int Splitter::PosToClient(int pos) const
{
	Size sz = GetSize();
	int sa = vert ? sz.cy : sz.cx;
	int a = sa * pos / 10000;
#ifdef PLATFORM_COCOA // workaround issue with splitter bar completely in "resize arrow" area
	a = clamp(a, 0, sa - chstyle->width);
#endif
	return a;
}

void Splitter::Layout() {
	Size sz = GetSize();
	int count = GetViewChildCount();
	if(count == 0)
		return;
	mins.SetCount(count, 0);
	minpx.SetCount(count, 0);
	count--;
	if(pos.GetCount() < count) {
		pos.SetCount(count, 0);
		for(int i = 0; i < count; i++)
			pos[i] = (i + 1) * 10000 / (count + 1);
	}
	if(style >= 0) {
		int i = 0;
		for(Ctrl *q = GetFirstChild(); q; q = q->GetNext()) {
			if(!q->InFrame()) {
				if(style == i)
					q->SizePos().Show();
				else
					q->Hide();
				i++;
			}
		}
		return;
	}

	int lw = chstyle->width >> 1;
	int rw = chstyle->width - lw;
	
	int i = 0;
	for(Ctrl *q = GetFirstChild(); q; q = q->GetNext()) {
		if(!q->InFrame()) {
			int lo = i > 0 ? PosToClient(pos[i - 1]) + rw : 0;
			int hi = i < count ? PosToClient(pos[i]) - lw : vert ? sz.cy : sz.cx;
			q->Show();
			if(vert)
				q->SetRect(0, lo, sz.cx, hi - lo);
			else
				q->SetRect(lo, 0, hi - lo, sz.cy);
			i++;
		}
	}
}

int Splitter::GetSplitWidth() const
{
	return chstyle->width;
}

void Splitter::PaintDots(Draw& w, const Rect& r, bool vert)
{
	int x = r.left + r.GetWidth() / 2;
	int y = r.top + r.GetHeight() / 2;
	(vert ? x : y) -= 8 * 4;
	for(int i = 0; i < 16; i++) {
		w.DrawRect(x - 1, y - 1, 1, 1, SColorShadow());
		w.DrawRect(x, y, 1, 1, SColorShadow());
		(vert ? x : y) += 4;
	}
}

void Splitter::Paint(Draw& w) {
	Size sz = GetSize();
	if(!IsTransparent())
		w.DrawRect(sz, SColorFace);
	const Value *ch = vert ? chstyle->vert : chstyle->horz;
	if(style < 0)
		for(int i = 0; i < pos.GetCount(); i++) {
			int p = PosToClient(pos[i]) - (chstyle->width >> 1);
			Rect r = vert ? RectC(0, p, sz.cx, chstyle->width) : RectC(p, 0, chstyle->width, sz.cy);
			if(HasCapture() && i == mouseindex)
				ChPaint(w, r, ch[1]);
			else
			if(!IsTransparent())
				ChPaint(w, r, ch[0]);
			if(chstyle->dots)
				PaintDots(w, r, vert);
		}
}

void   Splitter::MouseMove(Point p, dword) {
	if(HasCapture() && mouseindex >= 0 && mouseindex < pos.GetCount()) {
		SetPos(ClientToPos(p), mouseindex);
		Refresh();
		WhenAction();
	}
}

void   Splitter::LeftDown(Point p, dword) {
	SetCapture();
	Refresh();
	mouseindex = FindIndex(p);
}

int Splitter::FindIndex(Point client) const {
	int best = -1;
	int maxdist = chstyle->width;
	for(int i = 0; i < pos.GetCount(); i++) {
		int dist = abs((vert ? client.y : client.x) - PosToClient(pos[i]));
		if(dist <= maxdist) {
			best = i;
			maxdist = abs(dist);
		}
	}
	return best;
}

void   Splitter::LeftUp(Point p, dword keyflags) {
	if(HasCapture())
		WhenSplitFinish();
	ReleaseCapture();
	Refresh();
}

Image Splitter::CursorImage(Point p, dword) {
	return FindIndex(p) < 0 ? Image::Arrow() : vert ? Image::SizeVert() : Image::SizeHorz();
}

int Splitter::GetMins(int i) const
{
	int min1 = (i < mins.GetCount() ? mins[i] : 0);
	int min2 = 0;
	int cx = IsVert() ? GetSize().cy : GetSize().cx;
	if(cx)
		min2 = (i < minpx.GetCount() ? minpx[i] : 0) * 10000 / cx;
	return max(min1, min2);
}

Splitter& Splitter::SetPos(int p, int i) {
	int l = (i > 0 && i - 1 < pos.GetCount() ? pos[i - 1] : 0) + GetMins(i);
	int h = (i + 1 < pos.GetCount() ? pos[i + 1] : 10000) - GetMins(i + 1);
	pos.At(i) = minmax(p, l, h);
	Layout();
	return *this;
}

void   Splitter::SyncMin()
{
	for(int i = 0; i < pos.GetCount(); i++)
		SetPos(pos[i], i);
}

void   Splitter::Zoom(int i)
{
	style = i;
	Layout();
}

Splitter& Splitter::Vert(Ctrl& top, Ctrl& bottom)
{
	vert = true;
	Set(top, bottom);
	return *this;
}

Splitter& Splitter::Horz(Ctrl& left, Ctrl& right)
{
	vert = false;
	Set(left, right);
	return *this;
}

void Splitter::Set(Ctrl& l, Ctrl& r)
{
	Clear();
	*this << l << r;
	Layout();
}

void Splitter::Add(Ctrl& pane)
{
	Ctrl::Add(pane);
	Layout();
}

void Splitter::Insert(int ii, Ctrl& pane)
{
	if(ii >= GetCount())
		Add(pane);
	else {	
		Ctrl::AddChildBefore(&pane, GetIndexChild(ii));
		pos.Clear();
		Layout();
	}
}

void Splitter::Swap(Ctrl& child, Ctrl& newctrl)
{
	newctrl.SetRect(child.GetRect());
	Ctrl::AddChildBefore(&newctrl, &child);
	Ctrl::RemoveChild(&child);
}

void Splitter::Serialize(Stream& s) {
	int version = 0x02;
	s / version;
	if(version <= 1) {
		int p = GetPos(0);
		s % p;
		SetPos(p, 0);
	}
	else
		s % pos;
	if(version >= 0x01) {
		s / style;
		if(version == 0x01 && style == 3)
			style = -1;
	}
	if(s.IsLoading()) {
		for(int i = 0; i < pos.GetCount(); i++)
			if(pos[i] < 0 || pos[i] > 10000) {
				pos.Clear();
				s.LoadError();
			}
		if(style >= GetViewChildCount()) {
			style = -1;
			s.LoadError();
		}
		Layout();
	}
}

void Splitter::Remove(Ctrl& ctrl)
{
	int n = 0;
	Ctrl *c = GetFirstChild();
	while(c) {
		if(c == &ctrl){
			if(c->GetNext())
				pos.Remove(n);
			else
			if(n >= 1)
				pos.Remove(n-1);
			mins.Remove(n);
			minpx.Remove(n);
			RemoveChild(c);
			break;
		}
		c = c->GetNext();
		n++;
	}
}

void Splitter::Clear() {
	while(GetFirstChild())
		RemoveChild(GetFirstChild());
	pos.Clear();
	mins.Clear();
	minpx.Clear();
}

void Splitter::Reset() {
	Clear();
	style = -1;
	vert = false;
}

Splitter& Splitter::SetStyle(const Style& s)
{
	if(chstyle != &s) {
		chstyle = &s;
		RefreshLayout();
		Refresh();
	}
	return *this;
}

Splitter::Splitter() {
	chstyle = NULL;
	style = -1;
	vert = false;
	mouseindex = -1;
	pos.Add(5000);
	SetFrame(NullFrame());
	NoWantFocus();
	VSizePos(0, 0).HSizePos(0, 0);
	SetStyle(StyleDefault());
}

Splitter::~Splitter() {}

}
