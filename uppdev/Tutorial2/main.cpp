#include "Tutorial2.h"



Tutorial2::Tutorial2()
{
	CtrlLayout(*this, "Window title");
}

GUI_APP_MAIN
{
	Tutorial2().Run();
}
