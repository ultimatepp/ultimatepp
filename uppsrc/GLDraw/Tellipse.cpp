#include "GLDraw.h"

namespace Upp {

void Ellipse(GLTriangles& tr, Pointf center, Sizef radius, Color color,
             double width, Color line_color, double alpha)
{
	static Pointf e[256];
	static byte   step[65];
	ONCELOCK {
		for(int i = 0; i < 256; i++)
			e[i] = Polar(i * M_2PI / 256.0);
		for(int i = 0; i < 65; i++) {
			int h = 2 * i;
			int s = 4;
			while(s < h)
				s += s;
			step[i] = 256 / min(s, 256);
		}
	}
	double rmin = min(radius.cx, radius.cy);
	if(rmin <= 1e-100)
		return;

	int st = (int)max(radius.cx, radius.cy);
	st = st < 65 ? step[st] : 1;
	
	if(width >= rmin) {
		width = 0;
		color = line_color;
	}
	
	bool circle = radius.cx == radius.cy;
	if(width >= 0 && circle)
		width = width / radius.cx;
	
	int c = tr.Vertex(center, color, alpha);
	int a, a0;
	int la, lao, la0, lao0;
	bool has_line = width > 0 && !IsNull(line_color);
	bool has_interior = !IsNull(color);
	for(int i = 0; i < 256; i += st) {
		Pointf dir = radius * e[i];
		int b, lb, lbo;
		if(has_line) { // we need to draw border line
			lbo = tr.Vertex(center + dir, line_color, alpha); // outer point
			dir -= circle ? width * dir : width * Normalize(dir);
			lb = tr.Vertex(center + dir, line_color, alpha); // inner point
		}
		b = tr.Vertex(center + dir, color, alpha);
		if(i) {
			if(has_line) {
				tr.Triangle(la, lao, lbo);
				tr.Triangle(lbo, lb, la);
			}
			if(has_interior)
				tr.Triangle(c, a, b);
		}
		else {
			a0 = b;
			la0 = lb;
			lao0 = lbo;
		}
		a = b;
		la = lb;
		lao = lbo;
	}
	if(has_interior)
		tr.Triangle(c, a, a0);
	if(has_line) {
		tr.Triangle(la, lao, lao0);
		tr.Triangle(lao0, la0, la);
	}
}

};