#include <CtrlLib/CtrlLib.h>
#include <Report/Report.h>

using namespace Upp;

GUI_APP_MAIN
{
	String qtf = "[R5 {{1:1:1:1:1:1:1:1:1:1<500>500 ";
	for(int i = 1; i <= 10; i++)
		for(int j = 1; j <= 10; j++) {
			if(i * j > 1)
				qtf << ":: ";
			if(i * j == 100)
				qtf << "[R4 ";
			qtf << i * j;
		}
	Report r;
	r << qtf;
	Perform(r);
}

