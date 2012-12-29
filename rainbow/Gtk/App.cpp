#include <CtrlCore/CtrlCore.h>

#ifdef GUI_GTK

NAMESPACE_UPP

#define LLOG(x) DLOG(x)

void _DBG_Ungrab(void)
{
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
	gtk_init(0, NULL); // TODO?
	Ctrl::GlobalBackBuffer();
	SetStdFont(Arial(12));
	g_timeout_add(20, (GSourceFunc) Ctrl::TimeHandler, NULL);
	InstallPanicMessageBox(Ctrl::PanicMsgBox);
}

END_UPP_NAMESPACE

#endif
