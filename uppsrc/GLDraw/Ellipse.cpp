#include "GLDraw.h"

namespace Upp {

void GLDrawEllipse(const GLContext2D& dd, Pointf center, Sizef radius, Color fill_color, double width,
                   Color line_color, const Vector<double>& dash, double distance)
{
	GL_TIMING("Ellipse");
	static GLVertexData fill, line;
	const int N = 200;
	
	ONCELOCK {
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
		GLDrawPolygons(dd, center, fill, r, fill_color);
	r = radius - Sizef(width / 2, width / 2);
	if(width && !IsNull(line_color)) {
		if(dash.GetCount()) {
			GL_TIMING("Ellipse Dash");
			Vector<Pointf> line;
			for(int i = 0; i < N; i++)
				line.Add(r * Polar(i * M_2PI / N) + center);
			Vector<Vector<Pointf>> ll;
			DashPolyline(ll, line, dash, 0);
			GLVertexData data;
			GLPolylines(data, ll);
			GLDrawPolylines(dd, Pointf(0, 0), data, Sizef(1, 1), width, line_color);
		}
		else
			GLDrawPolylines(dd, center, line, r, width, line_color);
	}
}

void GLDrawEllipse(const GLContext2D& dd, Pointf center, Sizef radius, Color fill_color, double width,
                   Color line_color)
{
	static Vector<double> empty;
	GLDrawEllipse(dd, center, radius, fill_color, width, line_color, empty, 0);
}

};
