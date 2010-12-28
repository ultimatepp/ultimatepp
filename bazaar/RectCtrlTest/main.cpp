#include "RectCtrlTest.h"



RectCtrlTest::RectCtrlTest()
{
	CtrlLayout(*this, "Window title");
	Sizeable().Zoomable();

	Rect r = rc.GetRect();
	r.Deflate(r.Width()/4, r.Height()/4);
	rc.SetData(r);
}

GUI_APP_MAIN
{
	RectCtrlTest().Run();
}

