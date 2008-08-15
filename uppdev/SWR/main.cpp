#include "SWR.h"



SWR::SWR()
{
	CtrlLayout(*this, "Window title");
}

GUI_APP_MAIN
{
	SWR().Run();
}

