#include "GeomTest.h"

Pointf LineCross(Pointf a, Pointf a1, Pointf b, Pointf b1)
{
	Pointf A = a1 - a;
	Pointf B = b1 - b;
	double d = A.y * B.x - B.y * A.x;
	if(d == 0)
		return Null;
	double tb = (A.x * (b.y - a.y) + A.y * (a.x - b.x)) / d;
	if(tb <= 0 || tb >= 1)
		return Null;
	double ta = (B.x * tb + b.x - a.x) / A.x;
	return ta > 0 && ta < 1 ? B * tb + b : Null;
}

static int sCrosses(Pointf a, double y0, Pointf b, Pointf b1, double y1)
{
	double By = b1.y - b.y;
	if(By == 0)
		return	(y0 < b.y && b.y < y1 || y0 > b.y && b.y > y1) &&
				(a.y == b.y && a.x < b.x)/* || (a.y == b1.y && a.x < b1.x)*/;
	if(a.y == b.y)
		return (y0 < b.y && b.y < b1.y || y0 > b.y && b.y > b1.y) && b.x > a.x;
	if(a.y == b1.y)
		return 0;
//		return (b.y < b1.y && b1.y < y2 || b.y > b1.y && b1.y > y2) && b1.x > a.x;
	double tb = (a.y - b.y) / By;
	return tb >= 0 && tb < 1 && (b1.x - b.x) * tb + b.x - a.x > 0;
}


bool InPolygon(Pointf p, const Vector<Pointf>& polygon)
{
	int q = 0;
	if(polygon.GetCount() < 2)
		return false;
	int n = polygon.GetCount();
	for(int i = 0; i < polygon.GetCount(); i++)
		q += sCrosses(p, polygon[(i + n - 1) % n].y, polygon[i], polygon[(i + 1) % n], polygon[(i + 2) % n].y);
	return q & 1;
}

void GeomTest::LeftDown(Point p, dword flags)
{
	p = 4 * p / 4;
	if(flags & K_ALT) {
		paintin = !paintin;
		Refresh();
		return;
	}
	if(flags & K_CTRL)
		polygon.Clear();
	polygon.Add(p);
	Refresh();
}

void GeomTest::MouseMove(Point p, dword)
{
	point = p;
	Refresh();
}

void GeomTest::Paint(Draw& w)
{
	Size sz = GetSize();
	w.DrawRect(sz, SWhite);
	if(polygon.GetCount() == 0)
		return;
	if(polygon.GetCount() == 1) {
		w.DrawRect((int)polygon[0].x, (int)polygon[0].y, 1, 1, SBlack);
		return;
	}
	for(int i = 0; i < polygon.GetCount() - 1; i++)
		w.DrawLine(polygon[i], polygon[i + 1]);
	w.DrawLine(polygon[i], polygon[0]);
//	if(paintin)
		for(int i = 0; i < polygon.GetCount(); i++) {
			int y = polygon[i].y;
			for(int x = 0; x < sz.cx; x++)
				if(InPolygon(Pointf(x, y), polygon))
					w.DrawRect(x, y, 1, 1, SLtCyan);
		}
	if(InPolygon(point, polygon))
		w.DrawText(0, 0, "IN", Arial(20).Bold());
}

GeomTest::GeomTest()
{
	BackPaint();
	paintin = false;
	polygon.Add(Pointf(100, 100));
	polygon.Add(Pointf(200, 100));
	polygon.Add(Pointf(150, 150));
	polygon.Add(Pointf(250, 150));
}

GUI_APP_MAIN
{
	GeomTest().Run();
}
