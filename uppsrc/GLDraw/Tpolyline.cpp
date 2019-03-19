#include "GLDraw.h"

namespace Upp {

void Polyline(GLTriangles& tr, const Vector<Pointf>& p, double width, Color color, double alpha, bool close)
{
	int m = 1;
	if(p.GetCount() < 2)
		return;
	if(p.GetCount() && close && p[0] != p.Top())
		m = 0;
	width *= 0.5;
	int fv11, fv12, pv21, pv22;
	for(int i = 0; i < p.GetCount() - m; i++) {
		Pointf p1 = p[i];
		Pointf p2 = p[i + 1 < p.GetCount() ? i + 1 : 0];
		Pointf un = width * Normalize(Orthogonal(p2 - p1));
		
		int v11 = tr.Vertex(p1 + un, color, alpha);
		int v12 = tr.Vertex(p1 - un, color, alpha);
		int v21 = tr.Vertex(p2 + un, color, alpha);
		int v22 = tr.Vertex(p2 - un, color, alpha);
		
		tr.Triangle(v11, v12, v21);
		tr.Triangle(v12, v22, v21);
		if(i) { // bevel join with previous
			tr.Triangle(v11, pv21, pv22);
			tr.Triangle(v12, pv21, pv22);
		}
		else {
			fv11 = v11;
			fv12 = v12;
		}
		pv21 = v21;
		pv22 = v22;
	}
	
	if(close || p[0] == p.Top()) {
		tr.Triangle(fv11, pv21, pv22);
		tr.Triangle(fv12, pv21, pv22);
	}
}

};