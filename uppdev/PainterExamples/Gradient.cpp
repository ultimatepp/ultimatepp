#include "Examples.h"

void FillGradient(Painter& sw)
{
	sw.Rectangle(10, 10, 1000, 600)
	  .Fill(200, 100, Blue(), 300, 300, LtRed())
	  .Stroke(2, Black());
}

void FillGradientReflect(Painter& sw)
{
	sw.Rectangle(10, 10, 1000, 600)
	  .Fill(200, 100, Blue(), 300, 300, LtRed(), GRADIENT_REFLECT)
	  .Stroke(2, Black());
}

void FillGradientRepeat(Painter& sw)
{
	sw.Rectangle(10, 10, 1000, 600)
	  .Fill(200, 100, Blue(), 300, 300, LtRed(), GRADIENT_REPEAT)
	  .Stroke(2, Black());
}

INITBLOCK {
	RegisterExample("Linear gradient - PAD", FillGradient);
	RegisterExample("Linear gradient - REFLECT", FillGradientReflect);
	RegisterExample("Linear gradient - REPEAT", FillGradientRepeat);
}
