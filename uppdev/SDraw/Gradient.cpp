#include "SDraw.h"

NAMESPACE_UPP

Image Gradient::Generate(int cx) const
{
	RTIMING("Gradient");
	ImageBuffer ib(cx, 1);
	RGBA *t = ~ib;
	cx--;
	int l = 0;
	RGBA cl = color[0];
	for(int i = 0; i < pos.GetCount() - 1; i++) {
		int h = (int)(pos[i + 1] * cx);
		RGBA ch = color[i + 1];
		int w = h - l;
		for(int j = 0; j < w; j++) {
			t->r = ((w - j) * cl.r + j * ch.r) / w;
			t->g = ((w - j) * cl.g + j * ch.g) / w;
			t->b = ((w - j) * cl.b + j * ch.b) / w;
			t->a = ((w - j) * cl.a + j * ch.a) / w;
			t++;
		}
		cl = ch;
		l = h;
	}
	*t = cl;
	return ib;
}

Gradient& Gradient::Stop(double p, RGBA c)
{
	int i = FindLowerBound(pos, p);
	pos.Insert(i, p);
	color.Insert(i, c);
	return *this;
}

Gradient::Gradient(RGBA c1, RGBA c2)
{
	pos.Add(0);
	color.Add(c1);
	pos.Add(1.0);
	color.Add(c2);
}

SDraw& SDraw::Fill(const Gradient& gradient, double x1, double y1, double x2, double y2,
                   dword flags)
{
	Image m = gradient.Generate(256);
	Fill(m, x1, y1, x2, y2, FILL_HCOPY|FILL_REPEAT);
	return *this;
}

END_UPP_NAMESPACE