#include "Docking.h"

namespace Upp {

int ToolWin::GetTitleCy() const
{
	return max(GetStdFontCy() + DPI(4), DPI(16));
}

int ToolWin::GetBorder() const
{
	return DPI(2);
}

Rect ToolWin::GetMargins() const
{
	Rect r;
	r.left = r.right = r.top = r.bottom = GetBorder();
	r.top += GetTitleCy();
	return r;
}

Size ToolWin::AddMargins(Size sz) const
{
	Rect m = GetMargins();
	sz += Size(m.left + m.right, m.top + m.bottom);
	return sz;
}

void ToolWin::FrameAddSize(Size& sz)
{
	sz = AddMargins(sz);
}

void ToolWin::FrameLayout(Rect& r)
{
	if(GetParent()) {
		close.Hide();
		return;
	}
	close.Show();
	int c = GetTitleCy();
	int b = GetBorder();
	close.SetFrameRect(r.right - c - b + 1, r.top + b, c - 1, c - 1);
	Rect m = GetMargins();
	r.left += m.left;
	r.right -= m.right;
	r.top += m.top;
	r.bottom -= m.bottom;
}

void ToolWin::FramePaint(Draw& w, const Rect& rr)
{
	if(GetParent())
		return;
	int bn = GetBorder();
	Rect r = rr;
	for(int i = 0; i < bn; i++) {
		DrawFrame(w, r, decode(i, 0, SColorShadow(), 1, SColorLight(), SColorFace()));
		r.Deflate(1);
	}
	int fcy = GetStdFontCy();
	int titlecy = GetTitleCy();
	w.DrawRect(r.left, r.top, r.GetWidth(), titlecy, Blend(SColorFace(), SColorShadow()));
	DrawTextEllipsis(w, r.left + fcy / 4, r.top + (titlecy - fcy) / 2,
	                 r.GetWidth() - fcy / 2 - DPI(16), GetTitle(), "...", StdFont(), SColorText());
}

void ToolWin::StartMouseDrag0()
{
	p0 = GetMousePos();
	rect0 = GetRect();
	UnIgnoreMouse();
	SetCapture();
}

void ToolWin::StartMouseDrag()
{
	if(HasCapture() || !IsNull(dragdir))
		return;
	dragdir = Point(0, 0);
	StartMouseDrag0();
}

void ToolWin::MouseMove(Point, dword)
{
	if(!HasCapture() || GetParent())
		return;
	Rect r = rect0;
	Point off = GetMousePos() - p0;
	Ptr<Ctrl> _this = this;
	if(dragdir == Point(0, 0)) {
		r.Offset(off);
		Moving();
	}
	else {
		Size minsz = GetMinSize();
		Size maxsz = GetMaxSize();
		if(dragdir.x == -1)
			r.left = minmax(r.left + off.x, r.right - maxsz.cx, r.right - minsz.cx);
		if(dragdir.x == 1)
			r.right = minmax(r.right + off.x, r.left + minsz.cx, r.left + maxsz.cx);
		if(dragdir.y == -1)
			r.top = minmax(r.top + off.y, r.bottom - maxsz.cy, r.bottom - minsz.cy);
		if(dragdir.y == 1)
			r.bottom = minmax(r.bottom + off.y, r.top + minsz.cy, r.top + maxsz.cy);
	}
	if(_this)
		SetRect(r);
}

void ToolWin::LeftUp(Point, dword)
{
	Ptr<Ctrl> _this = this;
	if(dragdir == Point(0, 0))
		MoveEnd();
	if(_this)
		dragdir = Null;
}

Image ToolWin::CursorImage(Point, dword)
{
	Point dir = HasCapture() ? dragdir : GetDir(GetMousePos() - GetScreenRect().TopLeft());
	if(IsNull(dir))
		return Image::Arrow();
	static Image (*im[9])() = {
		Image::SizeTopLeft, Image::SizeLeft, Image::SizeBottomLeft,
		Image::SizeTop, Image::Arrow, Image::SizeBottom,
		Image::SizeTopRight, Image::SizeRight, Image::SizeBottomRight,
	};
	return (*im[(dir.x + 1) * 3 + (dir.y + 1)])();
}

Image ToolWin::FrameMouseEvent(int event, Point p, int zdelta, dword keyflags)
{
	if(GetParent())
		return Image::Arrow();
	switch(event) {
	case LEFTDOWN:
		StartMouseDrag0();
		dragdir = GetDir(p);
		if(IsNull(dragdir))
			break;
		if(dragdir == Point(0, 0))
			MoveBegin();
		break;
	case MOUSEMOVE:
		MouseMove(p, keyflags);
		break;
	case LEFTUP:
		LeftUp(p, keyflags);
		break;
	case CURSORIMAGE:
		return CursorImage(p, keyflags);
	}
	return Image::Arrow();
}

Point ToolWin::GetDir(Point p) const
{
	Size sz = GetSize();
	int b = GetBorder();
	if(p.x >= b && p.y > b && p.x < sz.cx - b && p.y < b + GetTitleCy())
		return Point(0, 0);
	if(Rect(sz).Deflated(b).Contains(p))
		return Null;
	b *= 4;
	Point r(0, 0);
	if(p.x < b)
		r.x = -1;
	if(p.x >= sz.cx - b)
		r.x = 1;
	if(p.y < b)
		r.y = -1;
	if(p.y >= sz.cy - b)
		r.y = 1;
	return r;
}

void ToolWin::SetClientRect(Rect r)
{
	Rect m = GetMargins();
	r.left += m.left;
	r.right += m.right;
	r.top += m.top;
	r.bottom += m.bottom;
	SetRect(r);
}

void ToolWin::PlaceClientRect(Rect r)
{
	Size sz = r.GetSize();
	Point pt = GetMousePos();
	int b = GetBorder();
	int t = GetTitleCy();
	if(!(pt.x >= r.left + b && pt.x < r.right - b))
		r.left = pt.x - sz.cx / 2;
	if(!(pt.y >= r.top + b && pt.y < r.top + b + t))
		r.top = pt.y - b - t / 2;
	r.SetSize(sz);
	Rect m = GetMargins();
	r.left -= m.left;
	r.right += m.right;
	r.top -= m.top;
	r.bottom += m.bottom;
	SetRect(r);
}

void ToolWin::DoClose()
{
	WhenClose();
}

void ToolWin::MoveBegin() {}

void ToolWin::Moving() {}

void ToolWin::MoveEnd() {}

ToolWin::ToolWin()
{
	dragdir = Null;
	Add(close);
	close.Image(CtrlImg::cross());
	close <<= THISBACK(DoClose);
	close.Tip(t_("Close"));
	AddFrame(*this);
	FrameLess();
}

}
