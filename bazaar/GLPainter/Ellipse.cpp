#include "GLPainter.h"

namespace Upp {

void GLDrawEllipse(const GLContext2D& dd, Pointf center, Sizef radius, Color color, double alpha)
{
	static GLVertexData ellipse;
	
	ONCELOCK {
		const int N = 200;
		Vector<Vector<Pointf>> p;
		p.Add();
		for(int i = 0; i < N; i++) {
			p.Top().Add(Polar(i * M_2PI / N));
		}
		GLPolygon(ellipse, p);
	}
	
	GLDrawPolygon(dd, center, ellipse, radius, color, alpha);
}

};
