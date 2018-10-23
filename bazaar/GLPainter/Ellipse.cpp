#include "GLPainter.h"

namespace Upp {

void GLDrawEllipse(const GLContext2D& dd, Pointf center, Sizef radius, Color fill_color, double width, Color line_color, double alpha)
{
	static GLVertexData fill, line;
	
	ONCELOCK {
		const int N = 200;
		Vector<Vector<Pointf>> p;
		p.Add();
		for(int i = 0; i < N; i++)
			p.Top().Add(Polar(i * M_2PI / N));
		GLPolygons(fill, p);
		Pointf f = p.Top()[0];
		p.Top().Add(f);
		GLPolylines(line, p);
	}
	
	Sizef r = radius - Sizef(width, width);
	if(r.cx > 0 && r.cy > 0 && !IsNull(fill_color))
		GLDrawPolygons(dd, center, fill, r, fill_color, dd.alpha * alpha);
	r = radius - Sizef(width / 2, width / 2);
	if(width > 0 && !IsNull(line_color))
		GLDrawPolylines(dd, center, line, r, width, line_color, dd.alpha * alpha);
}

};
