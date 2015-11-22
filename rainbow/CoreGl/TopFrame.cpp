#if defined(flagWINGL) || defined(flagLINUXGL)

#include "TopFrame.h"

NAMESPACE_UPP

#define LLOG(x)  // LOG(x)

TopWindowFrame::TopWindowFrame()
{
	close.SetImage(CoreGlImg::close());
	close.EdgeStyle();
	Add(close);
	maximize.SetImage(CoreGlImg::maximize());
	maximize.EdgeStyle();
	Add(maximize);
	maximize <<= THISBACK(ToggleMaximize);
	maximized = false;
	sizeable = false;
	holding = false;
}

void TopWindowFrame::SyncRect()
{
	if(maximized) {
		Size sz = screenRect.GetSize();
		if(GetRect().GetSize() != sz)
			SetRect(sz);
	}
}

void TopWindowFrame::Maximize()
{
	if(!maximized && maximize.IsShown()) {
		maximized = true;
		overlapped = GetRect();
		SetRect(screenRect);
		maximize.SetImage(CoreGlImg::overlap());
	}
}

void TopWindowFrame::Overlap()
{
	if(maximized && maximize.IsShown()) {
		maximized = false;
		SetRect(overlapped);
		maximize.SetImage(CoreGlImg::maximize());
	}
}

void TopWindowFrame::ToggleMaximize()
{
	if(maximized)
		Overlap();
	else
		Maximize();
}

Rect TopWindowFrame::Margins() const
{
	return maximized ? Rect(0, 0, 0, 0) : ChMargins(CoreGlImg::border());
}

void TopWindowFrame::Paint(Draw& w)
{
	Size sz = GetSize();
	Rect m = Margins();
	int c = GetStdFontCy() + 4;
	ChPaintEdge(w, sz, CoreGlImg::border());
	Image img = window->IsForeground() ? CoreGlImg::title() : CoreGlImg::bgtitle();
	resources.Bind(img, Resources::FORCE_FILTERING | Resources::LINEAR_FILTERING);
	ChPaint(w, m.left, m.top, sz.cx - m.left - m.right, GetStdFontCy() + 4, img);
	int tx = m.left + 2;
	int tcx = sz.cx - m.left - m.right - 4 - c * (close.IsShown() + maximize.IsShown());
	if(!IsNull(icon)) {
		Image h = icon;
		if(h.GetWidth() > c || h.GetHeight() > c)
			h = Rescale(h, GetFitSize(h.GetSize(), Size(c)));
		w.DrawImage(tx, m.top + 2, h);
		tx += c;
		tcx -= c;
	}
	DrawTextEllipsis(w, tx, m.top + 2, tcx, title, "..", StdFont(), SColorHighlightText());
}

void TopWindowFrame::Layout()
{
	Size sz = GetSize();
	Rect m = Margins();
	int c = GetStdFontCy() + 4;
	int x = sz.cx - m.right;
	if(close.IsShown())
		close.SetRect(x -= c, m.top, c, c);
	if(maximize.IsShown())
		maximize.SetRect(x -= c, m.top, c, c);
}

Rect TopWindowFrame::GetClient() const
{
	Rect r = GetRect();
	Rect m = Margins();
	r.left += m.left;
	r.right -= m.right;
	r.top += m.top;
	r.bottom -= m.bottom;
	r.top += GetStdFontCy() + 4;
	return r;
}

Rect TopWindowFrame::ComputeClient(Rect r)
{
	Rect m = Margins();
	r.left -= m.left;
	r.right += m.right;
	r.top -= m.top;
	r.bottom += m.bottom;
	r.top -= GetStdFontCy() + 4;
	return r;
}

void TopWindowFrame::SetClient(Rect r)
{
	SetRect(ComputeClient(r));	
}

Point TopWindowFrame::GetDragMode(Point p)
{
	Size sz = GetSize();
	Rect m = ChMargins(CoreGlImg::border());
	Point dir;
	dir.y = p.y < m.top ? -1 : p.y > sz.cy - m.top ? 1 : 0;
	dir.x = p.x < m.left ? -1 : p.x > sz.cx - m.right ? 1 : 0;
	return dir;
}

void TopWindowFrame::StartDrag()
{
	if(maximized)
		return;
	if(!sizeable && (dir.x || dir.y))
		return;
	SetCapture();
	startrect = GetRect();
	startpos = GetMousePos();
}

void TopWindowFrame::GripResize()
{
	dir = Point(1, 1);
	StartDrag();
}

void TopWindowFrame::LeftDown(Point p, dword keyflags)
{
	dir = GetDragMode(p);
	StartDrag();
}

void TopWindowFrame::LeftDouble(Point p, dword keyflags)
{
	ToggleMaximize();
	IgnoreMouseUp();
}

void TopWindowFrame::LeftUp(Point p, dword keyflags)
{
	ReleaseCapture();
}

void TopWindowFrame::MouseMove(Point, dword)
{
	if(!HasCapture())
		return;
	Size msz = ComputeClient(minsize).GetSize();
	Point p = GetMousePos() - startpos;
	Rect r = startrect;
	if(dir.x == -1)
		r.left = min(r.left + p.x, startrect.right - msz.cx);
	if(dir.x == 1)
		r.right = max(r.right + p.x, startrect.left + msz.cx);
	if(dir.y == -1)
		r.top = min(r.top + p.y, startrect.bottom - msz.cy);
	if(dir.y == 1)
		r.bottom = max(r.bottom + p.y, startrect.top + msz.cy);
	if(dir.y == 0 && dir.x == 0)
		r.Offset(p);
	SetRect(r);
}

Image TopWindowFrame::GetDragImage(Point dir)
{
	static Image (*im[9])() = {
		Image::SizeTopLeft, Image::SizeLeft, Image::SizeBottomLeft,
		Image::SizeTop, Image::Arrow, Image::SizeBottom,
		Image::SizeTopRight, Image::SizeRight, Image::SizeBottomRight,
	};
	return (*im[(dir.x + 1) * 3 + (dir.y + 1)])();
}

Image TopWindowFrame::CursorImage(Point p, dword)
{
	if(!sizeable)
		return Image::Arrow();
	return GetDragImage(HasCapture() ? dir : GetDragMode(p));
}

END_UPP_NAMESPACE

#endif
