#include "Examples.h"

void LineAttrs(Painter& sw)
{
	sw.Begin();
	for(int i = 0; i < 3; i++) {
		sw.DrawText(20, 20, get_i(i, "LINECAP_BUTT", "LINECAP_SQUARE", "LINECAP_ROUND"), Arial(28));
		sw.Move(50, 100).Line(200, 100)
		  .LineCap(get_i(i, LINECAP_BUTT, LINECAP_SQUARE, LINECAP_ROUND)).Stroke(30, Black())
		  .LineCap(LINECAP_BUTT).Stroke(1, LtRed());
		sw.Translate(300, 0);
	}
	sw.End();
	sw.Translate(0, 200);
	sw.Begin();
	for(int i = 0; i < 3; i++) {
		sw.DrawText(20, 20, get_i(i, "LINEJOIN_MITER", "LINEJOIN_ROUND", "LINEJOIN_BEVEL"), Arial(28));
		sw.Move(50, 240).Line(60, 100).Line(150, 220)
		  .LineJoin(get_i(i, LINEJOIN_MITER, LINEJOIN_ROUND, LINEJOIN_BEVEL)).Stroke(20, Black());
		sw.Translate(300, 0);
	}
	sw.End();
	sw.Translate(0, 300);
	sw.Begin();
	for(int i = 0; i < 3; i++) {
		sw.DrawText(20, 20, get_i(i, "MiterLimit(1)", "MiterLimit(4)", "MiterLimit(20)"), Arial(28));
		sw.Move(50, 240).Line(60, 100).Line(150, 220).Line(110, 100)
		  .MiterLimit(get_i(i, 1, 4, 20)).Stroke(20, Black());
		sw.Translate(300, 0);
	}
	sw.End();
	sw.Translate(0, 300);
	sw.Begin();
	for(int i = 0; i < 3; i++) {
		sw.DrawText(20, 20, get_i(i, "Dash(\"10\")", "Dash(\"10 10 30 10\")", "Dash(\"10 10 30 10\", 40)"), Arial(28));
		sw.Move(20, 100).Line(280, 100)
		  .Dash(i ? "10 10 30 10" : "10", i == 2 ? 25 : 0).Stroke(10, Black());
		sw.Translate(300, 0);
	}
	sw.End();
}

INITBLOCK {
	RegisterExample("Line attributes", LineAttrs);
}
