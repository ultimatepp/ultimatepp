#include <CtrlCore/CtrlCore.h>

#ifdef GUI_GTK

NAMESPACE_UPP

void InitGtkApp(int argc, char **argv, const char **envptr)
{
	gtk_init(0, NULL); // TODO?
	Ctrl::GlobalBackBuffer();
	SetStdFont(Arial(12));
	g_timeout_add(10, (GSourceFunc) Ctrl::TimeHandler, NULL);
}

END_UPP_NAMESPACE

#endif