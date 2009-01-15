#include "Examples.h"

#include "Examples.h"

void FillGradientRepeat(Painter& sw)
{
	sw.Rectangle(0, 0, 500, 500)
	  .Fill(200, 100, Blue(), 300, 300, LtRed(), GRADIENT_REPEAT);
}

INITBLOCK {
	RegisterExample("Linear gradient - REPEAT", FillGradientRepeat);
}
