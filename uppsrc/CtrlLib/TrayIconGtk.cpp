#include "CtrlLib.h"

#ifdef GUI_GTK

NAMESPACE_UPP

TrayIcon::TrayIcon()
{
	tray_icon = gtk_status_icon_new ();
	g_signal_connect(tray_icon, "activate", G_CALLBACK(DoActivate), this);
	g_signal_connect(tray_icon, "popup-menu", G_CALLBACK(PopupMenu), this);
	active = true;
}

void TrayIcon::PopupMenu(GtkStatusIcon *, guint, guint32, gpointer user_data)
{
	((TrayIcon *)user_data)->ExecuteMenu();
}

void TrayIcon::DoActivate(GtkStatusIcon *icon, gpointer user_data)
{
	((TrayIcon *)user_data)->Activate();
}

void TrayIcon::Sync()
{
	gtk_status_icon_set_from_pixbuf(tray_icon, image);
	gtk_status_icon_set_tooltip_text(tray_icon, tooltip);
}

void TrayIcon::DoMenu(Bar& bar)
{
	Menu(bar);
}

void TrayIcon::ExecuteMenu()
{
	gint x, y;
	GdkModifierType mod;
	gdk_window_get_pointer(gdk_get_default_root_window(), &x, &y, &mod);
	MenuBar::Execute(NULL, THISBACK(DoMenu), Point(x, y));
}

void TrayIcon::Run()
{
	gtk_main();
}

void TrayIcon::Break()
{
	gtk_main_quit();
}

void TrayIcon::Show(bool b)
{
	gtk_status_icon_set_visible(tray_icon, b);
}

bool TrayIcon::IsVisible() const
{
	return gtk_status_icon_get_visible(tray_icon);
}

void TrayIcon::Activate()
{
	WhenActivate();
	LeftDouble();
}


void TrayIcon::LeftDouble()
{
	WhenLeftDouble();
}

void TrayIcon::Menu(Bar& bar)
{
	WhenBar(bar);
}

TrayIcon::~TrayIcon()
{
	g_object_unref(tray_icon);
}

END_UPP_NAMESPACE

#endif
