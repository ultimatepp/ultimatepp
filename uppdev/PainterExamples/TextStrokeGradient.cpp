#include "Examples.h"

void TextStrokeGradient(Painter& sw)
{
	const char *txt = "GRADIENT TEXT";
	Font fnt = Arial(100).Bold();
	Size tsz = GetTextSize(txt, fnt);
	sw.Text(100, 100, txt, fnt)
	  .Stroke(4, 100, 100, Blue(), 100 + tsz.cx, 100, LtRed());
}

INITBLOCK {
	RegisterExample("Stroking text with linear gradient", TextStrokeGradient);
}
