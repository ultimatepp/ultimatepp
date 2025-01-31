#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct AnimateCtrlGeometry : TopWindow {
	Array<ColorPusher> pushers;
	Vector<Ptr<Ctrl>>  ctrls;
	Vector<Rect>       targets;

	AnimateCtrlGeometry()
	{
		Title("Ctrl geometry animation");
		
		CenterScreen().SetRect(0,0, 400, 200);

		for(int row = 0; row < 2; row++) {
			for(int col = 0; col < 4; col++) {
				ColorPusher& bt = pushers.Add();
				bt <<= Color(Random(256), Random(256), Random(256));
				bt.SetRect(0, 0, 100, 100);
				Add(bt);
				ctrls.Add(&bt);
				targets.Add(RectC(col * 100, row * 100, 100, 100).Deflated(2));
			}
		}
		
		SetTimeCallback(1000, [&]{ Animate(ctrls, targets, 300); });
	}
};

GUI_APP_MAIN
{
	AnimateCtrlGeometry().Run();
}
