#include "test2.h"



test2::test2()
{
	CtrlLayout(*this, "Window title");
}

GUI_APP_MAIN
{
	test2().Run();
}
