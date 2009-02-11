#include "Examples.h"

void TextFillGradient(Painter& sw)
{
	const char *txt = "GRADIENT TEXT";
	Font fnt = Arial(100).Bold().Italic();
	Size tsz = GetTextSize(txt, fnt);
	sw.Text(100, 100, txt, fnt)
	  .Fill(100, 100, Blue(), 100 + tsz.cx, 100/* + tsz.cy*/, LtRed());
	sw.Translate(0, 200);
	sw.Rectangle(100, 100, tsz.cx, tsz.cy)
	  .Stroke(1, Blue())
	  .Fill(TestImg::test(), 100, 100, 100 + tsz.cx, 100 + tsz.cy);
	sw.Move(100, 300).RelLine(tsz.cx + 50, 0).RelLine(-30, tsz.cy).RelLine(-tsz.cx / 2, 0)
	  .Fill(100, 300, Blue(), 100 + tsz.cx, 300/* + tsz.cy*/, LtRed())
//	  .Fill(TestImg::test(), 100, 300, 100 + tsz.cx, 300 + tsz.cy)
	  .Stroke(1, Blue());
/*	sw.Text(100, 100, txt, fnt)
	  .Fill(TestImg::test(), 100, 100, 100 + tsz.cx, 100 + tsz.cy)
	  .Stroke(1, Black());*/
}

INITBLOCK {
	RegisterExample("Filling text with linear gradient", TextFillGradient);
}
