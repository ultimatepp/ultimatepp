#include <CtrlLib/CtrlLib.h>
#include <Painter/Painter.h>

using namespace Upp;

GUI_APP_MAIN
{
	PaintingPainter p(100, 100);
	p.Circle(50, 50, 40).Stroke(3, Black()).Fill(LtBlue());
	Picture pc;
	pc.Transparent();
	pc.Background(Null).Set(AsDrawing(p));
	TopWindow win;
	win.Add(pc.SizePos());
	win.Sizeable().Zoomable();
	win.Run();
}
