#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	TopWindow w;
	Array<LabelBox> l;
	w.Add(l.Add().SetLabel("LeftPos(10, 180).TopPos(5, 20)").LeftPos(10, 180).TopPos(5, 20));
	w.Add(l.Add().SetLabel("LeftPos(10, 180).BottomPos(5, 20)").LeftPos(10, 180).BottomPos(5, 20));
	w.Add(l.Add().SetLabel("LeftPos(10, 180).VSizePos(50, 50)").LeftPos(10, 180).VSizePos(50, 50));
	w.Add(l.Add().SetLabel("RightPos(10, 180).TopPos(5, 20)").RightPos(10, 180).TopPos(5, 20));
	w.Add(l.Add().SetLabel("RightPos(10, 180).BottomPos(5, 20)").RightPos(10, 180).BottomPos(5, 20));
	w.Add(l.Add().SetLabel("RightPos(10, 180).VSizePos(50, 50)").RightPos(10, 180).VSizePos(50, 50));
	w.Add(l.Add().SetLabel("HSizePos(200, 200).TopPos(5, 20)").HSizePos(200, 200).TopPos(5, 20));
	w.Add(l.Add().SetLabel("HSizePos(200, 200).BottomPos(5, 20)").HSizePos(200, 200).BottomPos(5, 20));
	w.Add(l.Add().SetLabel("HSizePos(200, 200).VSizePos(50, 50)").HSizePos(200, 200).VSizePos(50, 50));
	w.Add(l.Add().SetLabel("HCenterPos(180).TopPos(27, 20)").HCenterPos(180).TopPos(27, 20));
	w.Add(l.Add().SetLabel("LeftPos(15, 170).VCenterPos(20)").LeftPos(15, 170).VCenterPos(20));
	w.Add(l.Add().SetLabel("HCenterPos(180).VCenterPos(20)").HCenterPos(180).VCenterPos(20));
	w.Sizeable().Zoomable();
	w.Run();
}
