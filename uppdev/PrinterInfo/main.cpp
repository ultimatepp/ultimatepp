#include <CtrlLib/CtrlLib.h>
#include <TDraw/TDraw.h>
#include <Report/Report.h>

using namespace Upp;

GUI_APP_MAIN
{
	String qtf;
	qtf << "{{1 " << String('&', 100) << "}}";
	
	Report r;
//	r.ChooseDefaultPrinter();
	r << qtf;
	Perform(r);
}
