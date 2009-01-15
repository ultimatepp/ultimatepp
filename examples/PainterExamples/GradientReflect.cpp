#include "Examples.h"

void FillGradientReflect(Painter& sw)
{
	sw.Rectangle(0, 0, 500, 500)
	  .Fill(200, 100, Blue(), 300, 300, LtRed(), GRADIENT_REFLECT);
}

INITBLOCK {
	RegisterExample("Linear gradient - REFLECT", FillGradientReflect);
}
