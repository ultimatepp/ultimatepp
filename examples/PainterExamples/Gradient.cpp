#include "Examples.h"

void FillGradient(Painter& sw)
{
	sw.Rectangle(0, 0, 500, 500)
	  .Fill(200, 100, Blue(), 300, 300, LtRed());
}

INITBLOCK {
	RegisterExample("Linear gradient - PAD", FillGradient);
}
