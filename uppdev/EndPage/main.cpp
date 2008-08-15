#include <CtrlLib/CtrlLib.h>
#include <Report/Report.h>

GUI_APP_MAIN
{
	Report r;
	r << "First page&[P Second page]&Anotherline^^Third page";
	Perform(r);
}
