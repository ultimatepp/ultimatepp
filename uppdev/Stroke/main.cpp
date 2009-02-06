#include <CtrlLib/CtrlLib.h>
#include <Painter/Painter.h>

using namespace Upp;

struct App : TopWindow {
	Pointf p1;
	Pointf p2;
	Pointf p3;
	bool   round;

	virtual void LeftDown(Point p, dword keyflags);
	virtual void MouseMove(Point p, dword keyflags);
	virtual void Paint(Draw& w);
	virtual Image CursorImage(Point p, dword keyflags) { return Image::Cross(); }
	
	App() { BackPaint(); p2 = Pointf(1195, 300); p3 = Pointf(500.0, 300.0); p1 = Pointf(300.0, 300.0); round = true; }
};

Pointf Ortogonal(Pointf p)
{
	return Pointf(-p.y, p.x);
}

double SquareLength(Pointf p)
{
	return p.x * p.x + p.y * p.y;
}

double Length(Pointf p)
{
	return sqrt(SquareLength(p));
}

double Bearing(Pointf p)
{
	if(p.y == 0)
		return (p.x >= 0 ? 0 : M_PI);
	if(p.x == 0)
		return (p.y >= 0 ? M_PI_2 : 3 * M_PI_2);
	double b = atan2(p.y, p.x);
	if(b < 0)
		b += 2 * M_PI;
	return b;
}

double Distance(Pointf p1, Pointf p2)
{
	return Length(p1 - p2);
}

double SquareDistance(Pointf p1, Pointf p2)
{
	return SquareLength(p1 - p2);
}

Pointf PolarPointf(double a)                         { return Pointf(cos(a), sin(a)); }
Pointf Polar(Pointf p, double r, double a)           { return p + r * PolarPointf(a) ; }

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

void Round(Painter& sw, Pointf p, Pointf v1, Pointf v2, double r)
{
	double tolerance = 0.3;
	double a1 = Bearing(v1);
	double a2 = Bearing(v2);
	double df = acos(1 - tolerance / r);
	if(a1 < a2)
		a1 += 2 * M_PI;
	while(a1 > a2) {
		sw.Line(Polar(p, r, a1));
		a1 -= df;
	}
}

void App::Paint(Draw& w)
{
	ImageBuffer ib(GetSize());
	BufferPainter sw(ib);
	sw.Clear(White());
	
	int join = 0;
//	sw.Move(p1.x, p1.y).Line(p2.x, p2.y).Line(p3.x, p3.y);
//	sw.Stroke(40, Gray());
//	sw.Stroke(2, LtRed());
	
	sw.Circle(p2.x, p2.y, 3).Fill(White()).Stroke(1, Black());

	double w2 = 20;
	double qmiterlimit = 16 * w2 * w2;
	
	Pointf v1 = p2 - p1;
	Pointf o1 = Ortogonal(v1) * w2 / Length(v1);
	
	Pointf t1 = p1 + o1;
	Pointf b1 = p1 - o1;

	Pointf v2 = p3 - p2;
	Pointf o2 = Ortogonal(v2) * w2 / Length(v2);
	
	Pointf t2 = p2 + o2;
	Pointf b2 = p2 - o2;

	Color c;
	double d = join == 1 ? 0 : v1.y * v2.x - v2.y * v1.x;
	if(d > 1e-30) {
		Pointf ts = t1 + v1 * (v2.y * (t1.x - t2.x) - v2.x * (t1.y - t2.y)) / d;
		sw.Move(t1);
		if(join || SquareDistance(ts, p2) > qmiterlimit) {
			sw.Line(t1 + v1);
			if(join == 2)
				Round(sw, p2, o1, o2, w2);
			sw.Line(t2);
		}
		else
			sw.Line(ts);
		sw.Line(t2 + v2);
		sw.Move(b2 + v2);
		sw.Line(b2);
		sw.Line(p2);
		sw.Line(b1 + v1);
		sw.Line(b1);
		c = LtBlue();
	}
	else
	if(d < -1e-30) {
		Pointf ts = b2 + v2 * (v1.y * (t2.x - t1.x) - v1.x * (t2.y - t1.y)) / d;
		sw.Move(b2 + v2);
		if(join || SquareDistance(ts, p2) > qmiterlimit) {
			sw.Line(b2);
			if(join == 2)
				Round(sw, p2, -o2, -o1, w2);
			sw.Line(b1 + v1);
		}
		else
			sw.Line(ts);
		sw.Line(b1);
		sw.Move(t1);
		sw.Line(t1 + v1);
		sw.Line(p2);
		sw.Line(t2);
		sw.Line(t2 + v2);
		c = LtGreen();
	}
	else {
		sw.Move(t1);
		sw.Line(t1 + v1);
		if(round)
			Round(sw, p2, o1, o2, w2);
		sw.Line(t2);
		sw.Line(t2 + v2);
		sw.Move(b2 + v2);
		sw.Line(b2);
		sw.Line(b1 + v1);
		sw.Line(b1);
		c = LtRed();
	}


	sw.Move(b1);
	sw.Line(t1);

	sw.Move(p3 - o2);
	sw.Line(p3 + o2);

#if 0
	sw.EvenOdd(true);
	sw.Fill(c);
#else
	sw.Stroke(1, c);
#endif

	w.DrawImage(0, 0, ib);
	
	String s;
	s << p1 << " - " << p2 << " - " << p3 << ", D=" << d;
	w.DrawText(0, 0, s);
}

GUI_APP_MAIN
{
	App().Run();
}
