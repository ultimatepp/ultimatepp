#include "Examples.h"

void DrawArc(Painter& sw)
{
	Pointf center(350, 300);
	for(int i = 0; i < 13; i++) {
		double angle = (i - 3) * M_PI / 6;
		Pointf box_center = Polar(center, 250, angle);
		Size radius(60, 20);
		Rect box(Point(box_center), Size(1, 1));
		box.Inflate(radius);
		sw.DrawRect(box, Yellow());
		for(int j = 0; j < 12; j++) {
			double ang2 = j * M_PI / 6;
			Sizef hand = Polar(Pointf(0, 0), 1, ang2);
			Point h1(box_center + Sizef(radius) * hand);
			Point h2(box_center + Sizef(radius) * 0.7 * hand);
			sw.DrawLine(h1, h2, 1, LtBlue());
		}
		Sizef hand = Polar(Pointf(0, 0), 1, angle);
		sw.DrawArc(box, Point(box_center + radius * hand), box.TopCenter(), PEN_DASHDOT, LtRed());
	}
}

INITBLOCK {
	RegisterExample("Draw::DrawArc", DrawArc);
}
