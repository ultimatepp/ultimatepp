#include "Examples.h"

void Arc(Painter& sw)
{
	sw.Move(400, 200).Arc(400, 200, 200, 100, 0.0, M_PI).Stroke(4, Blue());
	sw.Move(400, 400).Arc(400, 400, 100, 100, 0.0, M_PI / 3).Fill(LtCyan()).Stroke(2, Black());
}

void SvgArc(Painter& sw)
{
	sw.Path("M300,200 h-150 a150,150 0 1,0 150,-150 z").Fill(Red()).Stroke(5, Blue());
	sw.Path("M275,175 v-150 a150,150 0 0,0 -150,150 z").Fill(Yellow()).Stroke(5, Blue());
	sw.Path(
		"M600,350 l 50,-25 "
		"a25,25 -30 0,1 50,-25 l 50,-25 "
		"a25,50 -30 0,1 50,-25 l 50,-25 "
		"a25,75 -30 0,1 50,-25 l 50,-25 "
		"a25,100 -30 0,1 50,-25 l 50,-25"
	).Stroke(5, Red());
}

INITBLOCK {
	RegisterExample("Arc example", Arc);
	RegisterExample("SvgArc example", SvgArc);
}
