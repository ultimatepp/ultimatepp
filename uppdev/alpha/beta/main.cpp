#include "beta.h"

#define IMAGEFILE <beta/beta.iml>
#include <Draw/iml_source.h>

beta::beta()
{
	CtrlLayoutOKCancel(*this, "Window title");
}

GUI_APP_MAIN
{
	beta().Run();
}
