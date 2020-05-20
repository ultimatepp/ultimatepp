#include "LionBenchmark.h"

void TextRotated(Painter& sw)
{
	for(int angle = 0; angle < 360; angle += 30)
		sw.DrawText(250, 250, angle * 10, "x Text with angle " + AsString(angle));
}
