#include <CtrlLib/CtrlLib.h>

using namespace Upp;

void RoundedRectangle(Painter& p, double x, double y, double cx, double cy, double rx, double ry)
{
	ASSERT(rx >= 0 && ry >= 0);
	if (cx < 0) { x += cx; cx = -cx;}
	if (cy < 0) { y += cy; cy = -cy;}
	p.Move(x + rx, y).Arc(x + rx, y + ry, rx, ry, -M_PI / 2, -M_PI / 2);
	return;
	p.Line(x, y + cy - ry)//.Arc(x + rx, y + cy - ry, rx, ry, M_PI, -M_PI / 2)
//	 .Line(x + cx - rx, y + cy)//.Arc(x + cx - rx, y + cy - ry, rx, ry, M_PI / 2, -M_PI / 2)
	 .Line(x + cx, y + ry)//.Arc(x + cx - rx, y + ry, rx, ry, 0, -M_PI / 2)
	 .Line(x + rx, y);

}

struct MyApp : public TopWindow {
	void Paint(Draw& w) override {
		DrawPainter p(w, GetSize());
		p.Clear(White());
		p.Scale(22);
		p.Translate(5, 5);
		DLOG("===============");
//		p.LineJoin(LINEJOIN_ROUND);
#if 1
		p.RoundedRectangle(-2, -2, 100, 100, 5, 5);
		// RoundedRectangle(p, -2, -2, 100, 100, 5, 5);
		p.Stroke(5, Blue());
#endif
#if 1
		DLOG("---------------");
		p.Translate(20, 0);
		p
			.Move(3, -2)
			.Line(3, -2)
			.Line(2.4887790452444, -1.97418556365037)
			.Line(1.99232560327025, -1.89841777096139)
			.Line(1.51315294861739, -1.77520989647291)
			.Line(1.05377435582567, -1.60707521472478)
			.Line(0.616703099434949, -1.39652700025684)
			.Line(0.204452453985074, -1.14607852760896)
			.Line(-0.180464305984105, -0.858243071320971)
			.Line(-0.535533905932737, -0.535533905932738)
			.Line(-0.858243071320971, -0.180464305984106)
			.Line(-1.14607852760896, 0.204452453985073)
			.Line(-1.39652700025684, 0.616703099434948)
			.Line(-1.60707521472478, 1.05377435582567)
			.Line(-1.77520989647291, 1.51315294861739)
			.Line(-1.89841777096139, 1.99232560327025)
			.Line(-1.97418556365037, 2.4887790452444)
			.Line(-2, 3)
			.Line(-2, 3)
		;
		p.Stroke(5, Blue());
#endif
	}
};

GUI_APP_MAIN
{
	MyApp().Run();
}
