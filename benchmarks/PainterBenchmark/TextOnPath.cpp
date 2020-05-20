#include "LionBenchmark.h"

void TextOnPath(Painter& sw)
{
	Font fnt = Roman(100);
	FontInfo fi = fnt.Info();
	double pos = 0;
	const char *s = "Hello world, this is text on path!";
	int l = GetTextSize(s, fnt).cx;
	double r = l / (2 * M_PI);
	sw.Circle(300, 300, r).Stroke(1, Red());
	while(*s) {
		double w = fi[*s];
		sw.BeginOnPath(pos + w / 2, true);
		sw.Character(-w / 2, -fi.GetAscent(), *s++, fnt)
	      .Fill(0, -fi.GetAscent(), Yellow(), 0, fi.GetDescent(), Blue())
		  .Stroke(1, Black());
		sw.End();
		pos += w;
	}
}

