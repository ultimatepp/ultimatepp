#include "test.h"



test::test()
{
	CtrlLayout(*this, "Window title");
}

GUI_APP_MAIN
{
	test().Run();
}
