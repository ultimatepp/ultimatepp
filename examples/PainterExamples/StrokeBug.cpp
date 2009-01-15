#include "Examples.h"

#include "Examples.h"

void StrokeBug(Painter& sw)
{
	const char *txt = "GR";//ADIENT TEXT";
	Font fnt = Arial(100).Bold();
	Size tsz = GetTextSize(txt, fnt);
	sw.Scale(-4, 4);
	sw.Translate(-300, 0);
	sw.Text(100, 100, txt, fnt)
	  .Stroke(10, 100, 100, Blue(), 100 + tsz.cx, 100, LtRed())
	  .Stroke(0.25, White()); // _DBG_
	sw.Path("M 100 100 L 200 100 L 180 90 L 180 120").Stroke(25, Blue());
}

INITBLOCK {
	RegisterExample("Stroke bug", StrokeBug);
}
