#include <CtrlLib/CtrlLib.h>
#include <Painter/Painter.h>

using namespace Upp;

struct App : TopWindow {
	void Paint(Draw& w) {
		DrawPainter sw(w, GetSize());
		sw.Clear(White());
		double width2=200;
		double width1=100;
		double h1=50.5;
		double length2=100;
		sw.Offset(200, 200);
		sw
			.Move(-.5*width2, h1)
			.Line(-.5*width1, 0)
			.Line( .5*width1, 0)
			.Line( .5*width2, h1)
			.Line(-.5*width2, h1)
			.Fill(Green())
//			.Fill(-.25*width2, h1, White(), -.15*width2, .4*h1, .3*width2, Green())
		;
		sw
			.Rectangle(-.5*width2,h1,width2,length2)
			.ColorStop(0.10, Green())
			.ColorStop(0.25, White())
			.ColorStop(0.55, Green())
			.Fill(Green())
//			.Fill(-.5*width2, 0., Green(), .5*width2, 0., Green())
		;
	}
};

GUI_APP_MAIN
{
	App().Run();
}
