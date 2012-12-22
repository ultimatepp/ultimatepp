#include <CtrlCore/CtrlCore.h>

#ifdef GUI_GTK

NAMESPACE_UPP

#define LLOG(x)   DLOG(x)

void    TopWindow::SyncSizeHints()
{
	GuiLock __;
	if(!top)
		return;
	GdkGeometry m;
	Size sz0 = GetRect().GetSize();
	Size sz = sz0;
	if(sizeable)
		sz = GetMinSize();
	m.min_width = sz.cx;
	m.min_height = sz.cy;
	sz = sz0;
	if(sizeable)
		sz = GetMaxSize();
	m.max_width = sz.cx;
	m.max_height = sz.cy;
	gtk_window_set_resizable(gtk(), sizeable);
	gtk_window_set_geometry_hints(gtk(), top->window, &m, GdkWindowHints(GDK_HINT_MIN_SIZE|GDK_HINT_MAX_SIZE));
}

void TopWindow::SyncTitle0()
{
	GuiLock __;
	if(top)
		gtk_window_set_title(gtk(), FromUnicode(title, CHARSET_UTF8));
}

void TopWindow::SyncCaption0()
{
	GuiLock __;
	SyncTitle0();
	if(top && gdk_icon.Set(icon))
		gtk_window_set_icon(gtk(), gdk_icon);
}

void TopWindow::CenterRect(Ctrl *owner)
{
	GuiLock __;
	SetupRect();
	if(owner && center == 1 || center == 2) {
		Size sz = GetRect().Size();
		Rect r, wr;
		wr = Ctrl::GetWorkArea();
		// TODO: Add Frame support
		if(center == 1)
			r = owner->GetRect();
		else
			r = wr;
		Point p = r.CenterPos(sz);
		r = RectC(p.x, p.y, sz.cx, sz.cy);
		if(r.top < wr.top) {
			r.bottom += wr.top - r.top;
			r.top = wr.top;
		}
		if(r.bottom > wr.bottom)
			r.bottom = wr.bottom;
		minsize.cx = min(minsize.cx, r.GetWidth());
		minsize.cy = min(minsize.cy, r.GetHeight());
		SetRect(r);
	}
}

void TopWindow::Open(Ctrl *owner)
{
	GuiLock __;
	LLOG("OPEN " << Name() << " owner: " << UPP::Name(owner));
	if(dokeys && (!GUI_AKD_Conservative() || GetAccessKeysDeep() <= 1))
		DistributeAccessKeys();
	if(fullscreen)
		SetRect(GetPrimaryScreenArea());
	else
		CenterRect(owner);
	DDUMP(GetRect());
	IgnoreMouseUp();
	Create(owner, false);
	SyncSizeHints();
	SyncCaption();
	PlaceFocus();
}

void TopWindow::Open()
{
	Open(GetActiveWindow()); //TODO!
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
