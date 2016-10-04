#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	FontInfo fi = Arial(20).Info();
	for(int i = 32; i < 65536; i++)
		LOG(i << " \'" << WString(i, 1) << "\' " << fi.HasChar(i));
}
