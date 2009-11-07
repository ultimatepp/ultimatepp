#include "CtrlTest2.h"

#define IMAGECLASS MyImg
#define IMAGEFILE <CtrlTest2/CtrlTest2.iml>
#include <Draw/iml_source.h>

CtrlTest2::CtrlTest2()
{
	CtrlLayout(*this, "Window title");
}
/*
GUI_APP_MAIN
{
	CtrlTest2().Icon(MyImg::icon()).Run();
}

*/