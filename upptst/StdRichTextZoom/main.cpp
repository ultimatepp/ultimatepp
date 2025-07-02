#include "Test.h"

Test::Test()
{
	CtrlLayout(*this, "Window title");
	text <<= "[R This should be [g standard size";
}

GUI_APP_MAIN
{
	Test().Run();
}
