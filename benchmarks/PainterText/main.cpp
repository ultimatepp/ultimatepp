#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	ImagePainter iw(4000, 2000);
	iw.Co();
	Image m;

	for(int i = 0; i < 20; i++) {
		{
			RTIMING("Clear");
			iw.Clear();
		}
		if(1) {
			RTIMING("Text");
			for(int y = 0; y < 2000; y += 30)
				iw.DrawText(0, y,
				            "U++ is a C++ cross-platform rapid application development framework focused on programmers productivity. It includes a set of libraries (GUI, SQL, etc.), and an integrated development environment.",
				            Arial(25));
			iw.Finish();
		}
//		m = iw;
	}

	TopWindow win;
	ImageCtrl h;
	h.SetImage(iw);
	win << h.SizePos();
	win.Sizeable().Zoomable();
	win.Run();
}
