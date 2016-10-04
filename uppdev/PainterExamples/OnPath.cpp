#include "Examples.h"

void OnPath(Painter& sw)
{
	sw.Move(200, 300).Quadratic(400, 50, 600, 300).Quadratic(1000, 300).Stroke(1, Blue());
	for(int i = -1; i <= 16; i++) {
		sw.BeginOnPath(i / 15.0);
		sw.Move(0, -6).Line(6, 0).Line(0, 6).Close().Stroke(1, Black());
		sw.End();
	}
}

void OnTextPath(Painter& sw)
{
	sw.Text(50, 50, "Hello!", Roman(350)).Stroke(1, Black());
	for(int i = 0; i <= 100; i++) {
		sw.BeginOnPath(i / 100.0);
		sw.Move(0, -6).Line(6, 0).Line(0, 6).Fill(Red());
		sw.End();
	}
}

void OnPathBee(Painter& sw)
{
	sw.Move(200, 300).Quadratic(400, 50, 600, 300).Quadratic(1000, 300).Stroke(1, Blue());
	for(int i = -1; i <= 16; i++) {
		sw.BeginOnPath(i / 15.0);
		sw.DrawImage(0, -TestImg::Bee().GetHeight() / 2, TestImg::Bee());
		sw.End();
	}
}

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

INITBLOCK {
	RegisterExample("OnPath", OnPath);
	RegisterExample("OnTextPath", OnTextPath);
	RegisterExample("OnPathBee", OnPathBee);
	RegisterExample("TextOnPath", TextOnPath);
}
