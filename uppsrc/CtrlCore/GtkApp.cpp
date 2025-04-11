#include <CtrlCore/CtrlCore.h>

#ifdef GUI_GTK

#include <X11/Xlib.h>

#ifdef GDK_WINDOWING_X11
#include <gdk/gdkx.h>
#endif
#ifdef GDK_WINDOWING_WAYLAND
#include <gdk/gdkwayland.h>
#endif

#define CATCH_ERRORS 1

namespace Upp {

#define LLOG(x) // DLOG(x)

#if CATCH_ERRORS
void CatchError(const gchar *log_domain,
             GLogLevelFlags log_level,
             const gchar *message,
             gpointer user_data)
{
	RLOG((const char *)message);
	// __BREAK__;
}
#endif

void Ctrl::PanicMsgBox(const char *title, const char *text)
{
	LLOG("PanicMsgBox " << title << ": " << text);
	UngrabMouse();
	char m[2000];
	*m = 0;
	if(system("which gxmessage") == 0)
		strcpy(m, "gxmessage -center \"");
	else
	if(system("which kdialog") == 0)
		strcpy(m, "kdialog --error \"");
	else
	if(system("which xmessage") == 0)
		strcpy(m, "xmessage -center \"");

	if(*m) {
		strcat(m, title);
		strcat(m, "\n");
		strcat(m, text);
		strcat(m, "\"");
		IGNORE_RESULT(system(m));
	}
	else {
		GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR,
		                                           GTK_BUTTONS_CLOSE, "%s: %s", title, text);
		gtk_dialog_run(GTK_DIALOG (dialog));
		gtk_widget_destroy(dialog);
	}
	__BREAK__;
}

int Ctrl::scale;

bool Ctrl::IsWayland()
{
#ifdef GDK_WINDOWING_WAYLAND
	static bool wayland = GDK_IS_WAYLAND_DISPLAY(gdk_display_get_default());
	return wayland;
#endif
	return false;
}

bool Ctrl::IsX11()
{
#ifdef GDK_WINDOWING_WAYLAND
	static bool x11 = GDK_IS_X11_DISPLAY(gdk_display_get_default());
	return x11;
#endif
	return false;
}

bool Ctrl::IsRunningOnWayland()
{
	static bool running = GetEnv("XDG_SESSION_TYPE") == "wayland";
	return running;
}

void Ctrl::ThemeChanged(void *)
{
	PostReSkin();
}

static bool sUseWayland;

void Ctrl::UseWayland()
{
	sUseWayland = true;
}

bool InitGtkApp(int argc, char **argv, const char **envptr)
{
	LLOG(rmsecs() << " InitGtkApp");

	XInitThreads(); // otherwise there are errors despite GuiLock
	// this has to be called before gtk_init_check, otherwise it crashes with xfce (at least
	// some versions)
	
#if GTK_CHECK_VERSION(3, 10, 0)
	String backends = "x11,wayland";
#ifdef flagWAYLAND // Sets GTK to prefer Wayland backend
	backends = "wayland,x11";
#endif
	if(FileExists(ConfigFile("USE_WAYLAND")) || sUseWayland)
		backends = "wayland,x11";
	gdk_set_allowed_backends(backends);
#endif

	if (!gtk_init_check(&argc, &argv)) {
		Cerr() << t_("Failed to initialized GTK app!") << "\n";
		return false;
	}

	EnterGuiMutex();
	
	Ctrl::SetUHDEnabled(true);
	
	Ctrl::scale = 1;
#if GTK_CHECK_VERSION(3, 10, 0)
	if(Ctrl::IsWayland()) {
	    GtkWidget *window;
	    window = gtk_window_new(GTK_WINDOW_POPUP);
	    gtk_window_set_default_size (GTK_WINDOW (window), 1, 1);
	    gtk_widget_show_all (window);
		Ctrl::scale = gdk_window_get_scale_factor(gtk_widget_get_window(window));
		gtk_widget_destroy(window);
	}
	else
		Ctrl::scale = gdk_window_get_scale_factor(gdk_screen_get_root_window(gdk_screen_get_default()));
#endif

	Ctrl::GlobalBackBuffer();
	Ctrl::ReSkin();
	g_timeout_add(20, (GSourceFunc) Ctrl::TimeHandler, NULL);
	InstallPanicMessageBox(Ctrl::PanicMsgBox);
	if(Ctrl::IsX11())
		gdk_window_add_filter(NULL, Ctrl::RootKeyFilter, NULL);
#if CATCH_ERRORS
	g_log_set_default_handler (CatchError, 0);
#endif

	GtkSettings *settings = gtk_settings_get_default();
	if(settings) {
		g_signal_connect_swapped(settings, "notify::gtk-theme-name", G_CALLBACK(Ctrl::ThemeChanged), NULL);
		g_signal_connect_swapped(settings, "notify::gtk-application-prefer-dark-theme", G_CALLBACK(Ctrl::ThemeChanged), NULL);
	}
	
	return true;
}

void ExitGtkApp()
{
	LeaveGuiMutex();
}

}

#endif
