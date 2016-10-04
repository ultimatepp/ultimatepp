#include "editint.h"



editint::editint()
{
	CtrlLayout(*this, "Window title");
}

GUI_APP_MAIN
{
	editint().Run();
}
