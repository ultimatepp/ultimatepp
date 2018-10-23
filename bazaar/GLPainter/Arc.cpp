#include "GLPainter.h"

namespace Upp {

void GLArc(Vector<Vector<Pointf>>& line, const Rectf& rc, Pointf start, Pointf end)
{
	if(rc.Width() <= 0 || rc.Height() <= 0)
		return;
	Sizef radius = Sizef(rc.Size()) / 2.0;
	Pointf center = Pointf(rc.TopLeft()) + radius;
	double ang1 = Direction((Pointf(start) - center) / radius);
	double ang2 = Direction((Pointf(end) - center) / radius);
	if(ang1 < 0)
		ang1 += M_2PI;
	if(ang2 < 0)
		ang2 += M_2PI;
	if(ang1 > ang2)
		Swap(ang1, ang2);
	
	line.Add();
	for(double a = ang1; a <= ang2 + M_PI / 200; a += M_PI / 200)
		line.Top().Add(radius * Polar(min(a, ang2)) + center);
}

};