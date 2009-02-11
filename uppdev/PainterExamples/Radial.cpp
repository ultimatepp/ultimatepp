#include "Examples.h"

void Radial(Painter& sw)
{
	sw.Circle(400.5, 400.5, 200)
	  .Fill(400.5, 400.5, Blue(), 200, LtRed())
	  .Stroke(1, Black());
}

INITBLOCK {
	RegisterExample("Radial gradient", Radial);
}
