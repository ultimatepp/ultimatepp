#include "Examples.h"

void GradientStop(Painter& sw)
{
	sw.Rectangle(20.5, 20.5, 500, 100)
	  .ColorStop(0.25, Green())
	  .ColorStop(0.5, Yellow())
	  .ColorStop(0.75, Gray())
	  .Fill(0, 0, Blue(), 500, 0, LtRed())
	  .Stroke(1, Black());

	sw.Circle(120.5, 250.5, 100)
	  .ColorStop(0.25, Green())
	  .ColorStop(0.5, Yellow())
	  .ColorStop(0.75, Gray())
	  .Fill(120.5, 250.5, Blue(), 100, LtRed())
	  .Stroke(1, Black());

	sw.Circle(420.5, 250.5, 100)
	  .ColorStop(0.25, Green())
	  .ColorStop(0.5, Yellow())
	  .ColorStop(0.75, Gray())
	  .Fill(390, 280, Blue(), 420.5, 250.5, 100, LtRed())
	  .Stroke(1, Black());
}

INITBLOCK {
	RegisterExample("Gradient stops", GradientStop);
}
