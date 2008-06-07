#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct MyAppWindow : TopWindow {
	Button lt, rt, lb, rb, lv, ht, hv, cb, rc;

	MyAppWindow() {
		Title("My application with button").Sizeable();
		*this
			<< lt.SetLabel("left-top").LeftPos(10, 100).TopPos(10, 20)
			<< rt.SetLabel("right-top").RightPos(10, 100).TopPos(10, 20)
			<< lb.SetLabel("left-bottom").LeftPos(10, 100).BottomPos(10, 20)
			<< rb.SetLabel("right-bottom").RightPos(10, 100).BottomPos(10, 20)
			<< lv.SetLabel("left-vsize").LeftPos(10, 100).VSizePos(40, 40)
			<< ht.SetLabel("hsize-pos").HSizePos(120, 120).TopPos(10, 20)
			<< hv.SetLabel("hsize-vsize").HSizePos(120, 120).VSizePos(40, 40)
			<< cb.SetLabel("hcenter-bottom").HCenterPos(90).BottomPos(10, 20)
			<< rc.SetLabel("right-vcenter").RightPos(10, 100).VCenterPos(40)
		;
	}
};

GUI_APP_MAIN
{
	MyAppWindow app;
	app.Run();
}
