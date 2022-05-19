#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	Progress pi;
	pi.SetText("C:/Users/cxl/Downloads/CP210x_VCP_Windows/CP210x_VCP_Windows/x64/silabenm.sys");
	for(;;)
		if(pi.StepCanceled())
			break;
}
