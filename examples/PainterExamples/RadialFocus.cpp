#include "Examples.h"

void RadialFocus(Painter& sw)
{
	sw.Circle(400.5, 400.5, 200)
	  .Fill(300, 300, White(), 400.5, 400.5, 200, LtBlue())
	;
}

INITBLOCK {
	RegisterExample("Radial gradient with focus", RadialFocus);
}
