#include "GLPainter.h"

namespace Upp {

void GLDrawArc(const GLContext2D& dd, const Rectf& rc, Pointf start, Pointf end, int width, Color color, double alpha)
{
	if(rc.Width() <= 0 || rc.Height() <= 0)
		return;
	Sizef radius = Sizef(rc.Size()) / 2.0;
	Pointf center = Pointf(rc.TopLeft()) + radius;
	double ang1 = Direction((Pointf(start) - center) / radius);
	double ang2 = Direction((Pointf(end) - center) / radius);
	if(ang1 > ang2)
		Swap(ang1, ang2);
	
	Vector<Vector<Pointf>> line;
	line.Add();
	for(double a = ang1; a <= ang2 + M_PI / 200; a += M_PI / 200) {
		line.Top().Add(radius * Polar(min(a, ang2)) + center);
	}
	
	GLVertexData l;
	GLPolylines(l, line);
	GLDrawPolylines(dd, Pointf(0, 0), l, Sizef(1, 1), width, color, alpha);
}

};