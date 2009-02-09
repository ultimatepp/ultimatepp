#include "Examples.h"

#if 0

void TextFillSolid(Painter& sw)
{
	sw.Text(100, 100, "Hello world!", Roman(120).Italic().Bold())
	  .Fill(LtBlue());
}

INITBLOCK {
	RegisterExample("Filling text with solid color", TextFillSolid);
}

#endif
