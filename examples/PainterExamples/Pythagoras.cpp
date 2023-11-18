#include "Examples.h"

void DoRect(Painter &sw, double size, bool image)
{
	if(size < 1)
		return;

	sw.Move(0, 0);
	sw.Line(0, size);
	sw.Line(size, size);
	sw.Line(size, 0);
	if(image)
		sw.Fill(TestImg::test(), 0, 0, size, 0);
	else
		sw.Fill(Blue());

	sw.Begin();
	sw.Opacity(0.9);
	
	sw.Begin();
	sw.Translate(0, size);
	sw.Rotate(M_PI/4.0);
	DoRect(sw, size / M_SQRT2, image);
	sw.End();

	sw.Begin();
	sw.Translate(size / 2, 1.5 * size);
	sw.Rotate(-M_PI/4.0);
	DoRect(sw, size / M_SQRT2, image);
	sw.End();
	
	sw.End();
}

void PythagorasTree(Painter& sw)
{
	sw.Begin();
	sw.Translate(450, 700);
	sw.Scale(1, -1);
	DoRect(sw, 160, false);
	sw.End();
}

void PythagorasTreeImage(Painter& sw)
{
	sw.Begin();
	sw.Translate(450, 700);
	sw.Scale(1, -1);
	DoRect(sw, 160, true);
	sw.End();
}

INITBLOCK {
	RegisterExample("Pythagoras tree", PythagorasTree);
	RegisterExample("Pythagoras tree image", PythagorasTreeImage);
}
