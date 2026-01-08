#include <CtrlCore/CtrlCore.h>

#ifdef GUI_GTK

namespace Upp {

extern Event<const TopWindow *, TopWindow::CustomTitleBarMetrics&> custom_titlebar_metrics__;
extern Function<bool (const TopWindow *)> is_custom_titlebar__;
extern Function<Ctrl *(TopWindow *, Color, int)> custom_titlebar_make__;

void TopWindow::CustomBarIcon::Paint(Draw& w)
{
	w.DrawImage(0, 0, HasMouse() ? CtrlCoreImg::GtkBarButtonH() : CtrlCoreImg::GtkBarButton());
	w.DrawImage(0, 0, img);
}

void TopWindow::CustomBarIcon::MouseMove(Point p, dword keyflags)
{
	Refresh();
}

void TopWindow::CustomBarIcon::LeftDown(Point p, dword keyflags)
{
	SetCapture();
}

void TopWindow::CustomBarIcon::LeftUp(Point, dword keyflags)
{
	if(HasMouse())
		Action();
	Refresh();
}

void TopWindow::CustomBarIcon::MouseLeave()
{
	Refresh();
}

void TopWindow::BarCtrl::LeftDouble(Point p, dword keyflags)
{
	WhenAction();
}

void TopWindow::Init()
{
	custom_titlebar_metrics__ = [](const TopWindow *tw, TopWindow::CustomTitleBarMetrics& m) {
		if(!tw->custom_bar)
			return;
		m.height = GetGtkTitleBarHeight(tw);
		m.lm = 0;
		m.rm = 0;
		/*
		Image icon = tw->GetIcon();
		if(!IsNull(icon))
			m.lm = DPI(4) + min(icon.GetWidth(), 32);
		m.rm = (tw->IsZoomable() ? 3 : 1) * GetGtkTitleBarButtonWidth();
		*/
	};

	is_custom_titlebar__ = [](const TopWindow *win) {
		return win->IsCustomTitleBar__();
	};
	
	custom_titlebar_make__ = [=](TopWindow *win, Color bk, int mincy) -> Ctrl * {
		return win->MakeCustomTitleBar__(bk, mincy);
	};
}

int Ctrl::GetGtkTitleBarHeight(const TopWindow *tw)
{
	return max(tw->custom_titlebar_cy, IsUHDMode() ? 60 : 31);
}

int Ctrl::GetGtkTitleBarButtonWidth()
{
	return IsUHDMode() ? 94 : 47;
}

void TopWindow::SyncIcons()
{
	minicon.Set(CtrlCoreImg::GtkBarMinimize());
	GtkWindow *w = gtk();
	maxicon.Set(w && gtk_window_is_maximized(w) ? CtrlCoreImg::GtkBarOverlap()
	                                            : CtrlCoreImg::GtkBarMaximize());
	closeicon.Set(CtrlCoreImg::GtkBarClose());
}

void TopWindow::SyncCustomBar()
{
	if(custom_bar_frame) {
	//	custom_bar_frame->Height(GetCustomTitleBarMetrics().height);
		SetCustomBarColor(Nvl(custom_titlebar_bk, SColorFace()));
		auto cm = GetCustomTitleBarMetrics();
		custom_bar->VSizePos().HSizePos(DPI(10), DPI(6));
		RefreshFrame(0, 0, GetRect().Width(), cm.height);
		Size isz = CtrlCoreImg::GtkBarButton().GetSize();
		int y = (cm.height - isz.cy) / 2;
		custom_bar_icons->Width((isz.cx + DPI(8)) * (IsZoomable() ? 3 : 1));
		*custom_bar_icons << closeicon.RightPos(DPI(8), isz.cx).TopPos(y, isz.cy);
		if(IsZoomable()) {
			*custom_bar_icons << minicon.LeftPos(0, isz.cx).TopPos(y, isz.cy);
			*custom_bar_icons << maxicon.LeftPos(isz.cx + DPI(8), isz.cx).TopPos(y, isz.cy);
		}
		else {
			minicon.Remove();
			maxicon.Remove();
		}
		SyncIcons();
	}
}

bool Ctrl::prevent_custombar_drag;

void Ctrl::SetCustomBarDragPrevention()
{ // ugly as hell, but seems to be the only simple way how to prevent window drag in custom titlebar
	static gint p1, p2;
	GtkSettings *gs = gtk_settings_get_default();
	ONCELOCK {
		g_object_get(gs, "gtk-double-click-distance", &p1, "gtk-dnd-drag-threshold", &p2, NULL);
	};
	gint s1 = p1;
	gint s2 = p2;
	if(prevent_custombar_drag)
		s1 = s2 = 1000000; // no drags for you, GtkWindow...
	g_object_set (gs, "gtk-double-click-distance", s1, "gtk-dnd-drag-threshold", s2, NULL);
}

void Ctrl::SyncPreventCustomBarDragPrevention()
{
	TopWindow *tw = dynamic_cast<TopWindow *>(this);
	prevent_custombar_drag = tw && tw->custom_bar_frame && tw->GetScreenRect().Contains(GetMousePos());
	SetCustomBarDragPrevention();
}

bool TopWindow::IsCustomTitleBar__() const
{
	return custom_bar;
}

void  TopWindow::DoZoom()
{
	GtkWindow *w = gtk();
	if(w) {
		if(gtk_window_is_maximized(w))
			gtk_window_unmaximize(w);
		else
			gtk_window_maximize(w);
	}
}

void  TopWindow::DoMoveWindow()
{
	DLOG("============= MOVE");
#if GTK_CHECK_VERSION(3, 22, 0)
	if(CurrentEvent.device)
		gdk_window_begin_move_drag_for_device(gdk(), CurrentEvent.device,
		                                      1, CurrentEvent.x_root, CurrentEvent.y_root, CurrentEvent.time);
	else
#endif
		gdk_window_begin_move_drag(gdk(), 1, CurrentEvent.x_root, CurrentEvent.y_root, CurrentEvent.time);
}

Ctrl *TopWindow::MakeCustomTitleBar__(Color bk, int mincy)
{
	if(!custom_bar) {
		custom_bar_frame.Create();
		custom_bar_frame->Transparent();
		custom_bar_icons.Create();
		custom_bar_icons->Transparent();
		custom_bar_frame->AddFrame(*custom_bar_icons);
		custom_bar.Create();
		
		minicon << [=] {
			GtkWindow *w = gtk();
			if(w)
				gtk_window_iconify(w);
		};
		maxicon << [=] {
			DoZoom();
		};
		closeicon << [=] {
			if(IsEnabled()) {
				IgnoreMouseUp();
				WhenClose();
			}
		};
	}
	if(custom_bar) {
		if(&GetFrame(0) != ~custom_bar_frame) {
			RemoveFrame(*custom_bar_frame);
			if(&GetFrame(0) == &NullFrame())
				SetFrame(0, *custom_bar_frame);
			else
				InsertFrame(0, *custom_bar_frame);
		}
		custom_bar_frame->Add(*custom_bar);
	}
	custom_titlebar_bk = bk;
	if(custom_titlebar_cy < 0)
		custom_titlebar_cy = mincy;
	SyncCustomBar();
	return ~custom_bar;
}

}

#endif