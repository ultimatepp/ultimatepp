#include "Examples.h"

void TextFillSolid(Painter& sw)
{
	sw.Text(100, 100, "Hello world!", Roman(120).Italic().Bold())
	  .Fill(LtBlue());
}

const char *text = "Quick Brown Fox Jumps Over The Lazy Dog!";

void SomeTexts(Painter& sw)
{
	int face[] = {
		Font::ARIAL, Font::ROMAN, Font::COURIER
	};
	int height[] = {
		10, 16, 20
	};
	sw.Rectangle(600, 0, 1000, 1000).Fill(Blue());
	for(int x = 20; x < 1000; x += 600) {
		double y = 20;
		for(int h = 0; h < __countof(height); h++)
			for(int italic = 0; italic < 2; italic++)
				for(int bold = 0; bold < 2; bold++)
					for(int c = 0; c < __countof(face); c++) {
						sw.Text(x, y, text, Font().Face(face[c]).Height(height[h]).Italic(italic).Bold(bold))
						  .Fill(x == 20 ? Black() : White());
						y += height[h] + 2;
					}
	}
}

INITBLOCK {
	RegisterExample("Filling text with solid color", TextFillSolid);
	RegisterExample("Various fonts", SomeTexts);
}
