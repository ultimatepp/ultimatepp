#include "Examples.h"

void Triangle(Painter& sw)
{
	sw.Move(100, 100).Line(300, 102).Line(200, 300).Fill(Red()).Stroke(3, Blue());
	sw.Translate(500, 0);
	sw.Move(100, 100).Line(300, 102).Line(200, 300).Fill(Red()).Dash("10").Stroke(5, Blue());
}

void Cubic(Painter& sw)
{
	sw.Move(100, 200).Cubic(100, 100, 250, 100, 250, 200).Cubic(400, 300, 400, 200)
	  .Stroke(2, Red());
}

void CubicDashed(Painter& sw)
{
	sw.Move(100, 200).Cubic(100, 100, 250, 100, 250, 200).Cubic(400, 300, 400, 200)
	  .Dash("4")
	  .Stroke(2, Red());
}

void Cubics(Painter& sw)
{
	sw.Begin();
		sw.Scale(0.5);
		sw.Path("M100,200 C100,100 400,100 400,200").Stroke(5, Red());
		sw.Path("M100,500 C25,400 475,400 400,500").Stroke(5, Red());
		sw.Path("M100,800 C175,700 325,700 400,800").Stroke(5, Red());
		sw.Path("M600,200 C675,100 975,100 900,200").Stroke(5, Red());
		sw.Path("M600,500 C600,350 900,650 900,500").Stroke(5, Red());
		sw.Path("M600,800 C625,700 725,700 750,800 S875,900 900,800").Stroke(5, Red());
	sw.End();
}

void Quadratic(Painter& sw)
{
	sw.Move(200, 300).Quadratic(400, 50, 600, 300).Quadratic(1000, 300).Stroke(10, Blue());
}

INITBLOCK {
	RegisterExample("Path: Move, Line, Fill, Stroke", Triangle);
	RegisterExample("Path: Cubic", Cubic);
	RegisterExample("Path: Cubic, Dashed stroke", CubicDashed);
	RegisterExample("Path: Cubics, using Path", Cubics);
	RegisterExample("Path: Quadratic", Quadratic);
}
