#include "EnableCase.h"

EnableCase::EnableCase()
{
	CtrlLayout(*this, "Window title");
	sw.DisableCase(1);
}

GUI_APP_MAIN
{
	EnableCase().Run();
}
