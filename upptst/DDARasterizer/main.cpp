#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define IMAGECLASS TestImg
#define IMAGEFILE <DDARasterizer/test.iml>
#include <Draw/iml_header.h>

#define IMAGECLASS TestImg
#define IMAGEFILE <DDARasterizer/test.iml>
#include <Draw/iml_source.h>

struct ColorRenderer : DDARasterizer {
	Draw *draw;
	Color color;

	virtual void PutHorz(int x, int y, int cx) {
		draw->DrawRect(x, y, cx, 1, color);
	}
	virtual void PutVert(int x, int y, int cy) {
		draw->DrawRect(x, y, 1, cy, color);
	}
};

struct MyApp : TopWindow {
	Point p;
	int   width;
	bool  type;
	
	virtual void Paint(Draw& w);
	virtual void MouseMove(Point p, dword keyflags);
	virtual void LeftDown(Point p, dword keyflags);
	virtual void MouseWheel(Point p, int zdelta, dword keyflags);
	virtual Image CursorImage(Point p, dword keyflags);
	
	MyApp() { width = 20; p = Point(0, 0); Sizeable().Zoomable(); type = false; }
};

Image MyApp::CursorImage(Point p, dword keyflags)
{
	return TestImg::cursor1();
}

void MyApp::MouseMove(Point p_, dword keyflags)
{
	p = p_;
	Refresh();
}

void MyApp::MouseWheel(Point p, int zdelta, dword keyflags)
{
	width = minmax(1, 1000, width + sgn(zdelta));
	Title(AsString(width));
	Refresh();
}

void MyApp::LeftDown(Point p, dword keyflags)
{
	type = !type;
	Refresh();
}


void FatLine(DDARasterizer& r, Point p1, Point p2, int width)
{
	Point v = p2 - p1;
	Point shift = (width - 0.5) * Orthogonal(Pointf(v) / Length((Pointf(v))));
	Point p = p1 + shift / 2;
	r.Move(p);
	r.Line(p += v);
	r.Line(p -= shift);
	r.Line(p - v);
	r.Close();
}

void MyApp::Paint(Draw& w)
{
	Size sz = GetSize();

	w.DrawRect(GetSize(), Gray());

	Point p0(200, 200);

	Vector< Vector<Point> > pgs;
	
	ColorRenderer r;
	r.Cy(sz.cy);

	r.draw = &w;
	r.color = Green();
/*
	r.color = Black();
	r.Polygon();
	r.Move(p0);
	r.Line(p);
	r.Line(Point(700, 400));
	r.Fill();
*/	
//	w.DrawRect(p.x, p.y, 1, 1, White());
#if 0
	r.color = LtRed();
	r.Width(width);
	r.Move(p0);
	r.Line(p);
	r.color = LtBlue();
	r.Line(Point(sz.cx / 2, 300));
	r.color = LtGreen();
	r.Close();
#endif

#if 1
	r.Width(width);
	r.Ellipse(Rect(10, 10, p.x, p.y), 10);
#endif

	r.color = Black();
/*
	if(type) {
		r.Width(width);
		r.Move(p0);
		r.Line(p);
	}
	else*/
#if 0
	if(type)
		r.Polygon();
	FatLine(r, p0, p, width);
	if(type)
		r.Fill();
#endif

#if 0
	r.color = LtRed();
	r.Width(2);
	r.Move(Point(800, 100));
	r.Line(Point(800, 200));
#endif

//	w.DrawRect(p0.x, p0.y, 1, 1, White());
	w.DrawRect(700, 400, 1, 1, White());

	r.color = LtBlue();
}

GUI_APP_MAIN
{
	MyApp().Run();
}
