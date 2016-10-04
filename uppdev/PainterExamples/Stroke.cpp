#include "Examples.h"

void Stroke(Painter& sw)
{
	const char *txt = "GRM";
	Font fnt = Arial(100).Bold();
	Size tsz = GetTextSize(txt, fnt);
	sw.Scale(3, 3);
	sw.Text(100, 100, txt, fnt)
	  .Stroke(10, 100, 100, Blue(), 100 + tsz.cx, 100, LtRed())
	  .Stroke(0.25, White());
	sw.Path("M 100 100 L 200 100 L 210 90 L 220 40 L 230 90 L 240 100 L 400 100")
	  .Stroke(24, Blue());
}

INITBLOCK {
	RegisterExample("Stroke", Stroke);
}
