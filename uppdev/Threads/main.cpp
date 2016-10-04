#include "Threads.h"

String ToString(const Complex & c) { return Format("%g+%g i",c.Re(), c.Im());}

GUI_APP_MAIN
{
	Threads().Run();
}

