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
/*
		sw.Rectangle(20, 20, 60, 60);


		sw.Move(0, 0);
		sw.Cubic(99, 0,  50, 50,  99, 99);
		sw.Cubic(0, 99,  50, 50,  0, 0);
		sw.EvenOdd(i).Fill(Green());
		sw.Stroke(1, Black());
		sw.Translate(120, 0);
*/
	}
}

INITBLOCK {
	RegisterExample("Thin polygon", ThinPolygon);
}
