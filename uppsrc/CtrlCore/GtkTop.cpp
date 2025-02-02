#include <CtrlCore/CtrlCore.h>

#ifdef GUI_GTK

namespace Upp {

#define LLOG(x)  // DLOG(x)

Rect Ctrl::frameMargins;

Rect Ctrl::GetFrameMargins()
{
	GuiLock __;
	return frameMargins != Rect(0, 0, 0, 0) ? frameMargins : Rect(8, 32, 8, 8);
}

void TopWindow::SyncSizeHints()
{
	GuiLock __;
	if(!top)
		return;
	GdkGeometry m;
	Size sz0 = GetRect().GetSize();
	LLOG("SyncSizeHints sz0: " << sz0 << ", sizeable: " << sizeable << ", min: " << GetMinSize() << ", max: " << GetMaxSize());
	Size sz = sz0;
	if(sizeable)
		sz = GetMinSize();
	m.min_width = LSC(sz.cx + utop->csd.ExtraWidth());
	m.min_height = LSC(sz.cy + utop->csd.ExtraHeight());
	sz = sz0;
	if(sizeable)
		sz = GetMaxSize();
	m.max_width = LSC(sz.cx + utop->csd.ExtraWidth());
	m.max_height = LSC(sz.cy + utop->csd.ExtraHeight());
	gtk_window_set_resizable(gtk(), sizeable);
	Top *top = GetTop();
	if(top) {
		gtk_window_set_geometry_hints(gtk(), top->window, &m,
		                              GdkWindowHints(GDK_HINT_MIN_SIZE|GDK_HINT_MAX_SIZE));
		gtk_widget_set_size_request(top->window, m.min_width, m.min_height);
	}
}

void TopWindow::SyncTitle()
{
	GuiLock __;
	if(GetTop())
		gtk_window_set_title(gtk(), FromUnicode(title, CHARSET_UTF8));
}

void TopWindow::SyncCaption()
{
	GuiLock __;
	SyncTitle();
	if(top) {
		GList *icons = NULL;
		if(gdk_icon.Set(icon))
			icons = g_list_append(icons, gdk_icon);
		if(gdk_largeicon.Set(largeicon))
			icons = g_list_append(icons, gdk_largeicon);
		if(icons != NULL) {
			gtk_window_set_icon_list(gtk(), icons);
			g_list_free(icons);
		}
		gtk_window_set_decorated(gtk(), !frameless);
		gtk_window_set_urgency_hint(gtk(), urgent);
	}
}

void TopWindow::CenterRect(Ctrl *owner)
{
	GuiLock __;
	SetupRect(owner);
	if(owner && center == 1 || center == 2) {
		Size sz = GetRect().Size();
		Rect wr = owner? owner->GetWorkArea() : Ctrl::GetPrimaryWorkArea();
		Rect fm = GetFrameMargins();
		Rect r = (center == 1 && owner ? owner->GetRect() : wr)
		         .CenterRect(sz);
		wr.left += fm.left;
		wr.right -= fm.right;
		wr.top += fm.top;
		wr.bottom -= fm.bottom;
		if(r.top < wr.top) {
			r.top = wr.top;
			r.bottom = r.top + sz.cy;
		}
		if(r.left < wr.left) {
			r.left = wr.left;
			r.right = r.left + sz.cx;
		}
		if(r.bottom > wr.bottom) {
			r.bottom = wr.bottom;
			r.top = r.bottom - sz.cy;
		}
		if(r.right > wr.right) {
			r.right = wr.right;
			r.left = r.right - sz.cx;
		}
		minsize.cx = min(minsize.cx, r.GetWidth());
		minsize.cy = min(minsize.cy, r.GetHeight());
		SetRect(r);
	}
}

gboolean TopWindow::StateEvent(GtkWidget *widget, GdkEventWindowState *event, gpointer user_data)
{
	TopWindow *w = (TopWindow *)user_data;
	dword h = event->new_window_state;
	if(h & GDK_WINDOW_STATE_FULLSCREEN)
		w->state = FULLSCREEN;
	else
	if(h & GDK_WINDOW_STATE_ICONIFIED)
		w->state = MINIMIZED;
	else
	if(h & GDK_WINDOW_STATE_MAXIMIZED)
		w->state = MAXIMIZED;
	else {
		w->state = OVERLAPPED;
		w->overlapped = w->GetRect();
	}
	w->topmost = h & GDK_WINDOW_STATE_ABOVE;
	return FALSE;
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
	IgnoreMouseUp();
	Create(owner, false);
	Top *top = GetTop();
	if(top)
		g_signal_connect(top->window, "window-state-event", G_CALLBACK(StateEvent), this);
	SyncSizeHints();
	SyncCaption();
	PlaceFocus();
	int q = state;
	state = OVERLAPPED;
	SetMode(q);
	SyncTopMost();
	GdkRectangle fr;
	gdk_window_get_frame_extents(gdk(), &fr);
	Rect r = GetRect();
	frameMargins.left = max(frameMargins.left, minmax(r.left - SCL(fr.x), 0, 32));
	frameMargins.right = max(frameMargins.right, minmax(SCL(fr.x + fr.width) - r.right, 0, 32));
	frameMargins.top = max(frameMargins.top, minmax(r.top - SCL(fr.y), 0, 64));
	frameMargins.bottom = max(frameMargins.bottom, minmax(SCL(fr.y + fr.height) - r.bottom, 0, 48));
}

void TopWindow::Open()
{
	Open(GetActiveWindow());
}

void TopWindow::OpenMain()
{
	Open(NULL);
}

void TopWindow::SyncTopMost()
{
	GuiLock __;
	if(top)
		gtk_window_set_keep_above(gtk(), topmost);
}

void TopWindow::SetMode(int mode)
{
	GuiLock __;
	GtkWindow *w = gtk();
	if(w)
		switch(state) {
		case MINIMIZED:
			gtk_window_deiconify(w);
			break;
		case MAXIMIZED:
			gtk_window_unmaximize(w);
			break;
		case FULLSCREEN:
			gtk_window_unfullscreen(w);
			break;
		}
	state = mode;
	if(w)
		switch(state) {
		case MINIMIZED:
			gtk_window_iconify(w);
			break;
		case MAXIMIZED:
			gtk_window_maximize(w);
			break;
		case FULLSCREEN:
			gtk_window_fullscreen(w);
			break;
		}
	fullscreen = state == FULLSCREEN;
}

void TopWindow::Minimize(bool effect)
{
	SetMode(MINIMIZED);
}

TopWindow& TopWindow::FullScreen(bool b)
{
	SetMode(b ? (int)FULLSCREEN : (int)OVERLAPPED);
	return *this;
}

void TopWindow::Maximize(bool effect)
{
	SetMode(MAXIMIZED);
}

void TopWindow::Overlap(bool effect)
{
	SetMode(OVERLAPPED);
}

TopWindow& TopWindow::TopMost(bool b, bool)
{
	GuiLock __;
	topmost = b;
	SyncTopMost();
	return *this;
}

bool TopWindow::IsTopMost() const
{
	GuiLock __;
	return topmost;
}

void TopWindow::GuiPlatformConstruct()
{
	topmost = false;
}

void TopWindow::GuiPlatformDestruct()
{
}

void TopWindow::SerializePlacement(Stream& s, bool reminimize)
{
	GuiLock __;
	int version = 0;
	s / version;
	Rect rect = GetRect();
	s % overlapped % rect;
	bool mn = state == MINIMIZED;
	bool mx = state == MAXIMIZED;
	s.Pack(mn, mx);
	LLOG("TopWindow::SerializePlacement / " << (s.IsStoring() ? "write" : "read"));
	LLOG("minimized = " << mn << ", maximized = " << mx);
	LLOG("rect = " << rect << ", overlapped = " << overlapped);
	if(s.IsLoading()) {
		if(mn) rect = overlapped;
		Rect limit = GetVirtualWorkArea();
		Rect fm = GetFrameMargins();
		limit.left += fm.left;
		limit.right -= fm.right;
		limit.top += fm.top;
		limit.bottom -= fm.bottom;
		Size sz = min(rect.Size(), limit.Size());
		rect = RectC(
			minmax(rect.left, limit.left, limit.right - sz.cx),
			minmax(rect.top,  limit.top,  limit.bottom - sz.cy),
			sz.cx, sz.cy);
		state = OVERLAPPED;
		if(mn && reminimize)
			state = MINIMIZED;
		if(mx)
			state = MAXIMIZED;
		if(state == OVERLAPPED)
			SetRect(rect);
		if(IsOpen()) {
			if(state == MINIMIZED)
				Minimize(false);
			if(state == MAXIMIZED)
				Maximize(false);
		}
	}
}

}

#endif
