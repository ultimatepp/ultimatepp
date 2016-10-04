#include "test4.h"



test4::test4()
{
	CtrlLayout(*this, "Window title");
	Sizeable();
	
	AddFrame(fra.Width(100));
	btn.HCenterPos(80).VSizePos(10, 30);
	
	fra.Add(btn);
	
	Add(img);
	img.Background(LtGreen());
//	img.HCenterPos(400).VCenterPos(400);
	img.SizePos().LeftPos(0, 500);
}

GUI_APP_MAIN
{
	test4().Run();
}

