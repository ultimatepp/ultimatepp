#include "CtrlLib.h"

namespace Upp {

void SplitterFrame::FrameAdd(Ctrl& parent)
{
	parent.Add(*this);
}

void SplitterFrame::FrameRemove()
{
	this->Ctrl::Remove();
}

void SplitterFrame::FrameAddSize(Size& sz)
{
	if (IsShown()) (type == LEFT || type == RIGHT ? sz.cx : sz.cy) += size;
}

int  SplitterFrame::BoundSize()
{
	int maxsize = max(0, (type == LEFT || type == RIGHT ? parentsize.cx : parentsize.cy) - sizemin);
	return IsShown() ? max(4, minmax(size, minsize - 4, maxsize)) : 0;
}

void SplitterFrame::FrameLayout(Rect& r)
{
	Rect rr = r;
	parentsize = r.GetSize();
	int sz = BoundSize();
	switch(type) {
	case LEFT:
		r.left += sz;
		rr.right = r.left;
		break;
	case RIGHT:
		r.right -= sz;
		rr.left = r.right;
		break;
	case TOP:
		r.top += sz;
		rr.bottom = r.top;
		break;
	case BOTTOM:
		r.bottom -= sz;
		rr.top = r.bottom;
		break;
	}
	SetFrameRect(rr);
}

void SplitterFrame::Paint(Draw& draw)
{
	bool horz = type == LEFT || type == RIGHT;
	const Value *ch = horz ? style->vert : style->horz;
	Size sz = Ctrl::GetSize();
	Rect r(sz);
	switch(type) {
	case LEFT: r.left = r.right - style->width; break;
	case RIGHT: r.right = r.left + style->width; break;
	case TOP: r.top = r.bottom - style->width; break;
	case BOTTOM: r.bottom = r.top + style->width; break;
	}
	ChPaint(draw, r, ch[HasCapture()]);
	if(style->dots)
		Splitter::PaintDots(draw, r, !horz);
}

void SplitterFrame::LeftDown(Point p, dword)
{
	SetCapture();
	Refresh();
	ref = GetMousePos();
	size0 = BoundSize();
}

void SplitterFrame::MouseMove(Point p, dword keyflags)
{
	if(!HasCapture())
		return;
	p = GetMousePos();
	switch(type) {
	case LEFT: size = size0 + p.x - ref.x; break;
	case RIGHT: size = size0 + ref.x - p.x; break;
	case TOP: size = size0 + p.y - ref.y; break;
	case BOTTOM: size = size0 + ref.y - p.y; break;
	}
	RefreshParentLayout();
}

void SplitterFrame::LeftUp(Point p, dword keyflags)
{
    Refresh();
    ReleaseCapture();
}

Image SplitterFrame::CursorImage(Point p, dword keyflags)
{
	return type == LEFT || type == RIGHT ? Image::SizeHorz() : Image::SizeVert();
}

SplitterFrame& SplitterFrame::Set(Ctrl& c, int _size, int _type)
{
	while(GetFirstChild())
		GetFirstChild()->Remove();
	type = _type;
	size = _size;
	Add(c.SizePos());
	switch(type) {
	case LEFT: c.HSizePos(0, style->width); break;
	case RIGHT: c.HSizePos(style->width, 0); break;
	case TOP: c.VSizePos(0, style->width); break;
	case BOTTOM: c.VSizePos(style->width, 0); break;
	}
	RefreshParentLayout();
	return *this;
}

void SplitterFrame::Serialize(Stream& s)
{
	int version = 1;
	s / version;
	s % size;
	bool visible = IsShown();
	if(version >= 1)
		s % visible;
	if(s.IsLoading()) {
		Show(visible);
		RefreshParentLayout();
	}
}

SplitterFrame& SplitterFrame::SetStyle(const Splitter::Style& s)
{
	if(style != &s) {
		style = &s;
		RefreshLayout();
		Refresh();
	}
	return *this;
}

SplitterFrame::SplitterFrame()
{
	type = LEFT;
	size = size0 = 4;
	minsize = 0;
	sizemin = 0;
	style = NULL;
	SetStyle(Splitter::StyleDefault());
}

}
