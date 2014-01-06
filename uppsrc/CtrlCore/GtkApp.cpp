#include <CtrlCore/CtrlCore.h>

#ifdef GUI_GTK

NAMESPACE_UPP

#define LLOG(x) // DLOG(x)

void _DBG_Ungrab(void)
{   // This is a special nasty hack to make possible to ungrab mouse by debugger (see ide/Debuggers/PrettyPrinters.py)
	gdk_pointer_ungrab(GDK_CURRENT_TIME);
}

void Ctrl::PanicMsgBox(const char *title, const char *text)
{
	LLOG("PanicMsgBox " << title << ": " << text);
	_DBG_Ungrab();
	GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR,
	                                           GTK_BUTTONS_CLOSE, "%s: %s", title, text);
	gtk_dialog_run(GTK_DIALOG (dialog));
	gtk_widget_destroy(dialog);
	__BREAK__;
}

void InitGtkApp(int argc, char **argv, const char **envptr)
{
	LLOG(rmsecs() << " InitGtkApp");
#ifdef _MULTITHREADED
#if GLIB_MAJOR_VERSION <= 2 && GLIB_MINOR_VERSION < 32
	if(!g_thread_supported())
		g_thread_init(NULL);
#endif
	gdk_threads_set_lock_functions(EnterGuiMutex, LeaveGuiMutex);
	gdk_threads_init();
	EnterGuiMutex();
#endif
	gtk_init(&argc, &argv);
	Ctrl::GlobalBackBuffer();
	Ctrl::ReSkin();
	g_timeout_add(20, (GSourceFunc) Ctrl::TimeHandler, NULL);
	InstallPanicMessageBox(Ctrl::PanicMsgBox);
	gdk_window_add_filter(NULL, Ctrl::RootKeyFilter, NULL);
}

void ExitGtkApp()
{
#ifdef _MULTITHREADED
	LeaveGuiMutex();
#endif
}

END_UPP_NAMESPACE

#endif
