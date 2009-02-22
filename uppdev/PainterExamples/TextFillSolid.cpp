#include "Examples.h"

void TextFillSolid(Painter& sw)
{
	sw.Text(100, 100, "Hello world!", Roman(120).Italic().Bold())
	  .Fill(LtBlue());
}

const char *text = "Quick Brown Fox Jumps Over The Lazy Dog!";

void SomeTexts(Painter& sw)
{
	double y = 20;
	Font fnt[] = {
		Arial(10), Arial(16), Arial(24),
		Arial(10).Italic(), Arial(16).Italic(), Arial(24).Italic(),
		Roman(10), Roman(16), Roman(24),
		Roman(10).Italic(), Roman(16).Italic(), Roman(24).Italic(),
		Courier(10), Courier(16), Courier(24),
		Courier(10).Italic(), Courier(16).Italic(), Courier(24).Italic(),
	};
	for(int i = 0; i < __countof(fnt); i++) {
		Font f = fnt[i];
		sw.Text(20, y, text, f).Fill(Black());
		y += f.GetHeight();
	}
}

INITBLOCK {
	RegisterExample("Filling text with solid color", TextFillSolid);
	RegisterExample("Various fonts", SomeTexts);
}
