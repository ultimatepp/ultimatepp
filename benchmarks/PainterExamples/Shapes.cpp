#include "Examples.h"

void RoundedRect(Painter& sw)
{
	sw.RoundedRectangle(100, 100, 400, 200, 20).Stroke(2, Black());
}

INITBLOCK {
	RegisterExample("RoundedRectangle", RoundedRect);
}
