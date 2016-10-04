#include "Examples.h"
	
void ThinPolygon(Painter& sw)
{
	sw.Translate(52, 52);
	for(int i = 0; i < 2; i++) {
		sw.Move(36.000000, 142.000000);
		sw.Line(480.000000, 148.000000);
		sw.Line(429.000000, 148.000000);
		sw.Line(36.000000, 142.000000);
		sw.Fill(Black());
	}
}

INITBLOCK {
	RegisterExample("Thin polygon", ThinPolygon);
}
