#include "Examples.h"

#include "Examples.h"

void StrokeBug(Painter& sw)
{
	const char *txt = "GRM";//ADIENT TEXT";
	Font fnt = Arial(100).Bold();
	Size tsz = GetTextSize(txt, fnt);
	sw.Scale(3, 3);
//	sw.Translate(-300, 0);
	sw.Text(100, 100, txt, fnt)
	  .Stroke(10, 100, 100, Blue(), 100 + tsz.cx, 100, LtRed())
	  .Stroke(0.25, White());
	sw.Path("M 100 100 L 200 100 L 210 90 L 220 40 L 230 90 L 240 100 L 400 100")
	  .EvenOdd(true)
	  .Stroke(24, Blue());
}

INITBLOCK {
	RegisterExample("Stroke bug (fixed)", StrokeBug);
}
