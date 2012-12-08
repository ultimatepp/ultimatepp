#include <CtrlCore/CtrlCore.h>

#ifdef GUI_GTK

NAMESPACE_UPP

void InitGtkApp(int argc, char **argv, const char **envptr)
{
	gtk_init(0, NULL);
	Ctrl::GlobalBackBuffer();
	SetStdFont(Arial(12));
}

END_UPP_NAMESPACE

#endif