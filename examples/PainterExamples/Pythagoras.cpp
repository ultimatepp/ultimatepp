#include "Examples.h"

void DoRect(Painter &sw, double size)
{
	if(size < 1)
		return;
	
	sw.Move(0, 0);
	sw.Line(0, size);
	sw.Line(size, size);
	sw.Line(size, 0);
	sw.Fill(Blue());
	
	sw.Begin();
	sw.Translate(0, size);
	sw.Rotate(M_PI/4.);
	DoRect(sw, size / M_SQRT2);
	sw.End();

	sw.Begin();
	sw.Translate(size / 2, 1.5 * size);
	sw.Rotate(-M_PI/4.);
	DoRect(sw, size / M_SQRT2);
	sw.End();
}

void PythagorasTree(Painter& sw)
{
	sw.Begin();
	sw.Translate(450, 700);
	sw.Scale(1, -1);
	DoRect(sw, 160);
	sw.End();
}

INITBLOCK {
	RegisterExample("Pythagoras tree", PythagorasTree);
}
