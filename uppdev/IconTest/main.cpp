#include "IconTest.h"

IconTest::IconTest()
{
	CtrlLayout(*this, "Icon Test");
	
	String path = GetDataFile("test.png");
	img = StreamRaster::LoadFileAny(path);
	
	imgctrl.SetImage(img);
}

GUI_APP_MAIN
{
	IconTest().Run();
}

