#include "Examples.h"

void Arc(Painter& sw)
{
	sw.Move(400, 200).Arc(400, 200, 200, 100, 0.0, M_PI).Stroke(4, Blue());
	sw.Move(400, 400).Arc(400, 400, 100, 100, 0.0, M_PI / 3).Fill(LtCyan()).Stroke(2, Black());
}

INITBLOCK {
	RegisterExample("Arc example", Arc);
}
