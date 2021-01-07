#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct MyAppWindow : TopWindow {
	Button lt, rt, lb, rb, lv, ht, hv, cb, rc;

	MyAppWindow() {
		Title("My application with button").Sizeable();
		*this
			<< lt.SetLabel("left-top").LeftPos(10, 200).TopPos(10, 40)
			<< rt.SetLabel("right-top").RightPos(10, 200).TopPos(10, 40)
			<< lb.SetLabel("left-bottom").LeftPos(10, 200).BottomPos(10, 40)
			<< rb.SetLabel("right-bottom").RightPos(10, 200).BottomPos(10, 40)
			<< lv.SetLabel("left-vsize").LeftPos(10, 200).VSizePos(60, 60)
			<< ht.SetLabel("hsize-pos").HSizePos(220, 220).TopPos(10, 40)
			<< hv.SetLabel("hsize-vsize").HSizePos(220, 220).VSizePos(60, 60)
			<< cb.SetLabel("hcenter-bottom").HCenterPos(200).BottomPos(10, 40)
			<< rc.SetLabel("right-vcenter").RightPos(10, 200).VCenterPos(40)
		;
	}
};

GUI_APP_MAIN
{
	MyAppWindow app;
	app.Run();
}
