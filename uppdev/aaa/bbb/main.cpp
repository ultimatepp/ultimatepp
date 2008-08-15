#include "bbb.h"



bbb::bbb()
{
	CtrlLayout(*this, "Window title");
}

GUI_APP_MAIN
{
	bbb().Run();
}
