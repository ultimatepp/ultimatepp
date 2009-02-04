#include <CtrlLib/CtrlLib.h>
#include <Painter/Painter.h>

using namespace Upp;

struct App : TopWindow {
	Pointf p1;
	Pointf p2;
	Pointf p3;

	virtual void LeftDown(Point p, dword keyflags);
	virtual void MouseMove(Point p, dword keyflags);
	virtual void Paint(Draw& w);
	
	App() { BackPaint(); p2 = p3 = Pointf(0.0, 0.0); p1 = Pointf(300.0, 300.0); }
};

Pointf Ortogonal(Pointf p)
{
	return Pointf(-p.y, p.x);
}

double Length(Pointf p)
{
	return sqrt(p.x * p.x + p.y * p.y);
}

double Intersection0(Pointf p1, Pointf v1, Pointf p2, Pointf v2)
{
// p1.x + v1.x * t1 = p2.x + v2.x * t2;
// p1.y + v1.y * t1 = p2.y + v2.y * t2;
// t2 = (p1.x + v1.x * t1 - p2.x) / v2.x
// m2 = v2.y / v2.x;
// p1.y + v1.y * t1 = p2.y + v2.y * (p1.x + v1.x * t1 - p2.x) / v2.x;
// p1.y + v1.y * t1 = p2.y + m2 * p1.x + m2 * v1.x * t1 - m2 * p2.x;

// p1.y + v1.y * t1 = ;
// v1.y * t1 - m2 * v1.x * t1 = p2.y + m2 * p1.x - m2 * p2.x - p1.y;
// t1 = (p2.y + m2 * p1.x - m2 * p2.x - p1.y) / (v1.y - m2 * v1.x)
	double m2 = v2.y / v2.x;
	return (p2.y - p1.y + m2 * (p1.x - p2.x)) / (v1.y - m2 * v1.x);
}

Pointf Swapped(Pointf p)
{
	return Pointf(p.y, p.x);
}

double Intersection(Pointf p1, Pointf v1, Pointf p2, Pointf v2)
{
	return v2.x < 1e-30 ? Intersection0(Swapped(p1), Swapped(v1), Swapped(p2), Swapped(v2))
	                    : Intersection0(p1, v1, p2, v2);
}

Pointf Mirror(Pointf c, Pointf p)
{
	return 2 * c - p;
}

double Distance(Pointf p1, Pointf p2)
{
	return Length(p1 - p2);
}

void App::LeftDown(Point p, dword keyflags)
{
	p3 = p;
	Refresh();
}

void App::MouseMove(Point p, dword keyflags)
{
	p2 = p;
	Refresh();
}

void App::Paint(Draw& w)
{
	ImageBuffer ib(GetSize());
	BufferPainter sw(ib);
	sw.Clear(White());
	sw.Move(p1.x, p1.y).Line(p2.x, p2.y).Line(p3.x, p3.y);
	sw.Stroke(40, Gray());
	sw.Stroke(2, LtRed());
	
	sw.Circle(p2.x, p2.y, 3).Fill(White()).Stroke(1, Black());
	
	double w2 = 20;
	double miterlimit = 4 * w2;
	
	Pointf v1 = p2 - p1;
	Pointf o1 = Ortogonal(v1);
	double l1 = Length(v1);
	double u1 = w2 / l1;
	
	Pointf t1 = p1 + u1 * o1;
	Pointf b1 = p1 - u1 * o1;

	Pointf v2 = p3 - p2;
	Pointf o2 = Ortogonal(v2);
	double l2 = Length(v2);
	double u2 = w2 / l2;
	
	Pointf t2 = p2 + u2 * o2;
	Pointf b2 = p2 - u2 * o2;
//	DrawPoint(w, t2);
//	DrawPoint(w, b2);
	
	double s1 = Intersection(t1, v1, t2, v2);

	Pointf ts = t1 + s1 * v1;
	Pointf bs = Mirror(p2, ts);
	
	Pointf t3 = p3 + u2 * o2;
	Pointf b3 = p3 - u2 * o2;

	if(s1 < 1) {
		Swap(t1, b1);
		Swap(t2, b2);
		Swap(ts, bs);
		Swap(t3, b3);
		u1 = -u1;
		u2 = -u2;
	}

	if(Distance(ts, p2) > miterlimit) {
		Pointf bevel1 = p2 + u1 * o1;
		Pointf bevel2 = p2 + u2 * o2;
		sw.Move(t1.x, t1.y);
		sw.Line(bevel1.x, bevel1.y);
		sw.Line(bevel2.x, bevel2.y);
		sw.Line(t3.x, t3.y);
	}
	else {
		sw.Move(t1.x, t1.y);
		sw.Line(ts.x, ts.y);
		sw.Line(t3.x, t3.y);
	}
	
	sw.Move(b1.x, b1.y);
	sw.Line(bs.x, bs.y);
	sw.Line(b3.x, b3.y);
	sw.Stroke(1, LtRed());

	w.DrawImage(0, 0, ib);
}

GUI_APP_MAIN
{
	App().Run();
}

