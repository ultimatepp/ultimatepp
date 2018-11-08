#include "Examples.h"

#ifndef _DEBUG

void Big(Painter& sw)
{
	int n = 0;
	double sgn = 1;
	for(double r = 400; r > 5; r -= 3) {
		for(double i = 0; i < 400; i++) {
			Pointf p = Polar(sgn * i * M_2PI / 400) * r + Pointf(400, 400);
			if(i)
				sw.Line(p);
			else
				sw.Move(p);
			sw.Line(Polar(sgn * (i * M_2PI / 400 + M_2PI / 800)) * (r - 6) + Pointf(400, 400));
			n += 2;
		}
		sw.Close();
		sgn = -sgn;
	}
	sw.Fill(Black());
	sw.Text(0, 0, "Elements: " + AsString(n), Arial(20)).Fill(Blue());
}

void BigStroke(Painter& sw)
{
	int n = 0;
	double r = 400;
	int    i = 0;
	while(r > 5) {
		Pointf p = Polar(i * M_2PI / 400) * r + Pointf(400, 400);
		if(i)
			sw.Line(p);
		else
			sw.Move(p);
		sw.Line(Polar((i * M_2PI / 400 + M_2PI / 800)) * (r - 3) + Pointf(400, 400));
		n += 2;
		r = r - 0.01;
		i++;
	}
	sw.Stroke(1, Black());
	sw.Text(0, 0, "Elements: " + AsString(n), Arial(20)).Fill(Blue());
}

INITBLOCK {
	RegisterExample("Really Big Polygon", Big);
	RegisterExample("Really Big Stroke", BigStroke);
}

#endif
