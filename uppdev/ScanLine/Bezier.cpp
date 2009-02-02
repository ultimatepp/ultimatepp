#include "ScanLine.h"

double SquareDist(Pointf p1, Pointf p2)
{
	return (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
}

static void sQuadratic(VertexTarget& t, Pointf p1, Pointf p2, Pointf p3, double qt, int lvl)
{
	if(lvl > 32) {
		t.Line(p3);
		return;
	}
	Pointf p12 = (p1 + p2) / 2;
	Pointf p23 = (p2 + p3) / 2;
	Pointf div = (p12 + p23) / 2;
	Pointf p13 = (p1 + p3) / 2;
	if(SquareDist(p13, div) < qt)
		t.Line(p3);
	else {
		sQuadratic(t, p1, p12, div, qt, lvl + 1);
		sQuadratic(t, div, p23, p3, qt, lvl + 1);
	}
}

void ApproximateQuadratic(VertexTarget& t, Pointf p1, Pointf p2, Pointf p3, double tolerance)
{
	sQuadratic(t, p1, p2, p3, tolerance * tolerance, 0);
	t.Line(p3);
}

static void sCubic(VertexTarget& t, Pointf p1, Pointf p2, Pointf p3, Pointf p4, double qt,
                   int lvl)
{
	if(lvl > 32) {
		t.Line(p4);
		return;
	}
	Pointf p12 = (p1 + p2) / 2;
	Pointf p23 = (p2 + p3) / 2;
	Pointf p34 = (p3 + p4) / 2;
	Pointf p123 = (p12 + p23) / 2;
	Pointf p234 = (p23 + p34) / 2;
	Pointf div = (p123 + p234) / 2;
	Pointf p14 = (p1 + p4) / 2;
	if(SquareDist(p14, div) < qt)
		t.Line(p4);
	else {
		sCubic(t, p1, p12, p123, div, qt, lvl + 1);
		sCubic(t, div, p234, p34, p4, qt, lvl + 1);
	}
}

void ApproximateCubic(VertexTarget& t, Pointf p1, Pointf p2, Pointf p3, Pointf p4, double tolerance)
{
	sCubic(t, p1, p2, p3, p4, tolerance * tolerance, 0);
	t.Line(p4);
}
