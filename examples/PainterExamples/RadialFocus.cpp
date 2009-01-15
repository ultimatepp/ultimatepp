#include "Examples.h"

void RadialFocus(Painter& sw)
{
	sw.Circle(400.5, 400.5, 200)
	  .ColorStop(0.7, LtBlue())
	  .Fill(300, 300, White(), 400.5, 400.5, 200, Black())
	;
}

INITBLOCK {
	RegisterExample("Radial gradient with focus", RadialFocus);
}
