#include <CtrlLib/CtrlLib.h>

#include "AppWindow.h"

using namespace Upp;

#ifndef flagTESTING_GTEST_UI_EXAMPLE
	GUI_APP_MAIN
	{
		AppWindow().Run();
	}
#endif
