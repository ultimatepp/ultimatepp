#include "CtrlLib.h"

#ifdef GUI_GTK

#include <libnotify/notify.h>
#ifdef NOTIFY_CHECK_VERSION
    #if NOTIFY_CHECK_VERSION(0,7,0)
        #define NOTIFY_VERSION_GT_0_7_0
    #endif
#endif

G_GNUC_BEGIN_IGNORE_DEPRECATIONS

namespace Upp {

TrayIcon::TrayIcon()
{
	tray_icon = gtk_status_icon_new ();
	g_signal_connect(tray_icon, "button-press-event", G_CALLBACK(DoButtonPress), this);
	g_signal_connect(tray_icon, "button-release-event", G_CALLBACK(DoButtonRelease), this);
	g_signal_connect(tray_icon, "activate", G_CALLBACK(DoActivate), this);
	g_signal_connect(tray_icon, "popup-menu", G_CALLBACK(PopupMenu), this);
	active = true;
}

gboolean TrayIcon::DoButtonPress(GtkStatusIcon *, GdkEventButton *e, gpointer user_data)
{
	TrayIcon *q = (TrayIcon *)user_data;
	if(e->type == GDK_BUTTON_PRESS) {
		if(e->button == 1) {
			static int clicktime = msecs() - 100000;
			if(msecs(clicktime) < 250)
				q->LeftDouble();
			else
				q->LeftDown();
			clicktime = msecs();
		}
		if(e->button == 3)
			q->ExecuteMenu();
	}
	return TRUE;
}

gboolean TrayIcon::DoButtonRelease(GtkStatusIcon *, GdkEventButton *e, gpointer user_data)
{
	TrayIcon *q = (TrayIcon *)user_data;
	if(e->button == 1)
		q->LeftUp();
	return TRUE;
}

void TrayIcon::PopupMenu(GtkStatusIcon *, guint, guint32, gpointer user_data)
{
	((TrayIcon *)user_data)->ExecuteMenu();
}

void TrayIcon::DoActivate(GtkStatusIcon *icon, gpointer user_data)
{
	((TrayIcon *)user_data)->LeftDown();
	((TrayIcon *)user_data)->LeftDouble();
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

void TrayIcon::Message(int type, const char *title, const char *text, int timeout)
{
	if(!notify_is_initted() && !notify_init(title))
		return;
	GError *error = NULL;
	NotifyNotification *notification = notify_notification_new (title, text
					, type == 1 ? "gtk-dialog-info"
					: type == 2 ? "gtk-dialog-warning"
					: "gtk-dialog-error"
#ifndef NOTIFY_VERSION_GT_0_7_0
					, NULL
#endif
					);
	notify_notification_set_timeout(notification, timeout * 1000);
	notify_notification_show(notification, &error);
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

void TrayIcon::LeftDown()
{
	WhenLeftDown();
}

void TrayIcon::LeftUp()
{
	WhenLeftUp();
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

}

G_GNUC_END_IGNORE_DEPRECATIONS

#endif
