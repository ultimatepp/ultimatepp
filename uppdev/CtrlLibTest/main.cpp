#include "CtrlLibTest.h"

#define IMAGECLASS MyIm
#define IMAGEFILE <CtrlLibTest/CtrlLibTest.iml>
#include <Draw/iml_source.h>

CtrlLibTest::CtrlLibTest()
{
	CtrlLayout(*this, "Window title");
}

GUI_APP_MAIN
{
	CtrlLibTest().Icon(MyIm::icon).Run();
}
