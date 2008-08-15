#include "TestUpp.h"



TestUpp::TestUpp()
{
	CtrlLayout(*this, "Window title");
}

GUI_APP_MAIN
{
	TestUpp().Run();
}

