#include "Examples.h"

#if 0
void RadialFocus(Painter& sw)
{
	sw.Circle(400.5, 400.5, 200)
	  .Fill(300, 300, White(), 400.5, 400.5, 200, LtBlue())
	;
}
#endif

void RadialFocusPad(Painter& sw)
{
	sw.Rectangle(10, 10, 1000, 600)
	  .Fill(300, 300, White(), 400.5, 400.5, 200, LtBlue(), GRADIENT_PAD)
	  .Stroke(2, Black())
	;
}

void RadialFocusReflect(Painter& sw)
{
	sw.Rectangle(10, 10, 1000, 600)
	  .Fill(300, 300, White(), 400.5, 400.5, 200, LtBlue(), GRADIENT_REFLECT)
	  .Stroke(2, Black())
	;
}

void RadialFocusRepeat(Painter& sw)
{
	sw.Rectangle(10, 10, 1000, 600)
	  .Fill(300, 300, White(), 400.5, 400.5, 200, LtBlue(), GRADIENT_REPEAT)
	  .Stroke(2, Black())
	;
}

INITBLOCK {
//	RegisterExample("Radial gradient - focus", RadialFocus);
	RegisterExample("Radial gradient - focus - PAD", RadialFocusPad);
	RegisterExample("Radial gradient - focus - REFLECT", RadialFocusReflect);
	RegisterExample("Radial gradient - focus - REPEAT", RadialFocusRepeat);
}
