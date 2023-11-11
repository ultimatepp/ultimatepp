#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	ImagePainter iw(4000, 2000);
	iw.Co();

	for(int i = 0; i < 1000; i++) {
		{
			RTIMING("Clear");
			iw.Clear();
		}
		{
			RTIMING("Clear 2");
			iw.Clear();
		}
		{
			RTIMING("Rect");
			iw.DrawRect(0, 0, 4000, 2000, White());
			iw.Finish();
		}
		{
			RTIMING("Fill");
			iw.Circle(2000, 1000, 900).Fill(Blue());
			iw.Finish();
		}
		{
			RTIMING("Stroke");
			iw.Circle(2000, 1000, 900).Stroke(2, LtRed());
			iw.Finish();
		}
		{
			RTIMING("Blend");
			iw.Circle(2000, 1000, 900).Fill(200 * Blue());
			iw.Finish();
		}
	}
	
	TopWindow win;
	ImageCtrl h;
	h.SetImage(iw);
	win << h.SizePos();
	win.Run();
}
