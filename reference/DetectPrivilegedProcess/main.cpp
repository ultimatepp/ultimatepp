#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	// Run the example as administrator (Windows) or as root (POSIX) to get the warning.
	if(IsUserAdmin())
		Exclamation(t_("Warning: Application has administrator/root rights.&This might pose a security risk!"));
	else
		PromptOK(t_("Application has user rights."));
}
