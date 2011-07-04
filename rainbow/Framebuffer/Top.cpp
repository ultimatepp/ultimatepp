#include <CtrlCore/CtrlCore.h>

#ifdef GUI_FB

NAMESPACE_UPP

#define LLOG(x)  // LOG(x)

void TopWindow::Frame::Paint(Draw& w)
{
	Size sz = GetSize();
	Rect m = ChMargins(FBImg::border());
	ChPaintEdge(w, sz, FBImg::border());
	ChPaint(w, m.left, m.top, sz.cx - m.left - m.right, GetStdFontCy() + 4, FBImg::title());
	w.DrawText(m.left + 2, m.top + 2, title, StdFont(), White());
}

Rect TopWindow::Frame::GetClient() const
{
	Rect r = GetRect();
	Rect m = ChMargins(FBImg::border());
	r.left += m.left;
	r.right -= m.right;
	r.top += m.top;
	r.bottom -= m.bottom;
	r.top += GetStdFontCy() + 4;
	return r;
}

void TopWindow::Frame::SetClient(Rect r)
{
	Rect m = ChMargins(FBImg::border());
	r.left -= m.left;
	r.right += m.right;
	r.top -= m.top;
	r.bottom += m.bottom;
	r.top -= GetStdFontCy() + 4;
	SetRect(r);
}

Point TopWindow::Frame::GetDragMode(Point p)
{
	Size sz = GetSize();
	Rect m = ChMargins(FBImg::border());
	Point dir;
	dir.y = p.y < m.top ? -1 : p.y > sz.cy - m.top ? 1 : 0;
	dir.x = p.x < m.left ? -1 : p.x > sz.cx - m.right ? 1 : 0;
	return dir;
}

void TopWindow::Frame::GripResize()
{
	SetCapture();
	dir = Point(1, 1);
	startrect = GetRect();
	startpos = GetMousePos();
}

void TopWindow::Frame::LeftDown(Point p, dword keyflags)
{
	SetCapture();
	dir = GetDragMode(p);
	startrect = GetRect();
	startpos = GetMousePos();
}

void TopWindow::Frame::MouseMove(Point, dword)
{
	if(!HasCapture())
		return;
	Point p = GetMousePos() - startpos;
	Rect r = startrect;
	if(dir.x == -1)
		r.left += p.x;
	if(dir.x == 1)
		r.right += p.x;
	if(dir.y == -1)
		r.top += p.y;
	if(dir.y == 1)
		r.bottom += p.y;
	if(dir.y == 0 && dir.x == 0)
		r.Offset(p);
	SetRect(r);
	WhenLayout();
}

void TopWindow::Frame::Layout()
{
	WhenLayout();
}

Image TopWindow::Frame::CursorImage(Point p, dword)
{
	p = HasCapture() ? dir : GetDragMode(p);
	static Image (*im[9])() = {
		Image::SizeTopLeft, Image::SizeLeft, Image::SizeBottomLeft,
		Image::SizeTop, Image::Arrow, Image::SizeBottom,
		Image::SizeTopRight, Image::SizeRight, Image::SizeBottomRight,
	};
	return (*im[(p.x + 1) * 3 + (p.y + 1)])();
}

void TopWindow::SyncSizeHints() {}

void TopWindow::SyncTitle0()
{
	SyncCaption0();
}

void TopWindow::SyncCaption0()
{
	GuiLock __;
	frame.SetTitle(title.ToString());
}

void TopWindow::SyncRect()
{
	Rect r = frame.GetClient();
	DLOG("SyncRect to " << r);
	if(r != GetRect())
		SetRect(r);
}

void TopWindow::Open(Ctrl *owner)
{
	GuiLock __;
	Rect r = GetRect();
	if(r.IsEmpty())
		SetRect(GetDefaultWindowRect());
	else
	if(r.left == 0 && r.top == 0)
		if(owner && center == 1)
			SetRect(owner->GetRect().CenterRect(r.GetSize()));
		else
		if(center)
			SetRect(GetWorkArea().CenterRect(r.GetSize()));
	frame.SetClient(GetRect());
	frame.PopUp(owner, false, true);
	PopUp(&frame, false, true);
	popup = false;
	SetRect(frame.GetClient());
}

void TopWindow::Open()
{
	Open(GetActiveCtrl());
}

void TopWindow::OpenMain()
{
	Open(NULL);
}

void TopWindow::Minimize(bool effect)
{
	state = MINIMIZED;
}

TopWindow& TopWindow::FullScreen(bool b)
{
	return *this;
}

void TopWindow::Maximize(bool effect)
{
	state = MAXIMIZED;
}

void TopWindow::Overlap(bool effect)
{
	GuiLock __;
	state = OVERLAPPED;
}

TopWindow& TopWindow::TopMost(bool b, bool stay_top)
{
	GuiLock __;
	return *this;
}

bool TopWindow::IsTopMost() const
{
	return true;
}

void TopWindow::GuiPlatformConstruct()
{
}

void TopWindow::GuiPlatformDestruct()
{
}

void TopWindow::SerializePlacement(Stream& s, bool reminimize)
{
	GuiLock __;
}

END_UPP_NAMESPACE

#endif
