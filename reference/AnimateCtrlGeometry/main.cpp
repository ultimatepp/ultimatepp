#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct AnimateCtrlGeometry : TopWindow {
	Array<ColorPusher> pushers;
	Vector<Ptr<Ctrl>>  ctrls;
	Rect               ctrls_start_rect;
	Vector<Rect>       targets;
	Button             reset;
	
	AnimateCtrlGeometry()
		: ctrls_start_rect(Rect(0, 0, 100, 100))
	{
		Title("Ctrl geometry animation");
		
		CenterScreen().SetRect(0,0, 400, 230);
		
		Add(reset.SetLabel("Reset").HSizePosZ(4, 4).BottomPosZ(4, 20));
		reset.WhenAction = [&] { Reset(); };

		for(int row = 0; row < 2; row++) {
			for(int col = 0; col < 4; col++) {
				ColorPusher& bt = pushers.Add();
				bt <<= Color(Random(256), Random(256), Random(256));
				bt.SetRect(ctrls_start_rect);
				Add(bt);
				ctrls.Add(&bt);
				targets.Add(RectC(col * 100, row * 100, 100, 100).Deflated(2));
			}
		}
		
		ScheduleAnimation();
	}
	
	void ScheduleAnimation()
	{
		constexpr int LAUNCH_DELAY_MS = 1000;
		constexpr int DURATION_MS = 300;
		
		reset.Disable();
		SetTimeCallback(LAUNCH_DELAY_MS, [=] {
			Animate(ctrls, targets, DURATION_MS); reset.Enable();
		});
	}
	
	void Reset()
	{
		for (ColorPusher& cp : pushers)
			cp.SetRect(ctrls_start_rect);
		ScheduleAnimation();
	}
};

GUI_APP_MAIN
{
	AnimateCtrlGeometry().Run();
}
