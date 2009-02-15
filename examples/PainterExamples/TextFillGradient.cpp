#include "Examples.h"

void TextFillGradient(Painter& sw)
{
	const char *txt = "GRADIENT TEXT";
	Font fnt = Arial(100).Bold().Italic();
	Size tsz = GetTextSize(txt, fnt);
	sw.Text(100, 100, txt, fnt)
	  .Fill(100, 100, Blue(), 100 + tsz.cx, 100 + tsz.cy, LtRed());
	sw.Translate(0, 200);
}

INITBLOCK {
	RegisterExample("Filling text with linear gradient", TextFillGradient);
}
