#include <CtrlCore/CtrlCore.h>

#ifdef GUI_GTK

NAMESPACE_UPP

#define LLOG(x)  // LOG(x)

void    TopWindow::SyncSizeHints()
{
	GuiLock __;
	if(!top)
		return;
	GdkGeometry m;
	Size sz0 = GetRect().GetSize();
	DDUMP(sz0);
	Size sz = sz0;
	if(sizeable)
		sz = GetMinSize();
//		sz = Size(50, 50);
	DDUMP(sz);
	m.min_width = sz.cx;
	m.min_height = sz.cy;
	sz = sz0;
	if(sizeable)
		sz = GetMaxSize();
	DDUMP(sz);
	m.max_width = sz.cx;
	m.max_height = sz.cy;
	// TODO!!!
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

void TopWindow::Open(Ctrl *owner)
{
	GuiLock __;
	if(GetSize().cx == 0)
		SetRect(10, 10, 400, 400);
	Create(owner, false);
	SyncSizeHints();
	SyncCaption();
}

void TopWindow::Open()
{
	Open(NULL); //TODO!
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
