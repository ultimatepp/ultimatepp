#include "Examples.h"

void Dash(Painter& sw)
{
	sw.Move(50, 50).Line(400, 200).Dash("2").Stroke(5, Blue());
	sw.Translate(0, 10);
	sw.Move(50, 50).Line(400, 200).Dash("10 5").Stroke(5, Blue());
	sw.Translate(0, 10);
	sw.Move(50, 50).Line(400, 200).Dash("10 5 5 5").Stroke(5, Blue());
	sw.Translate(0, 10);
	sw.Move(50, 50).Line(400, 200).Dash("1").Stroke(5, Blue());
	sw.Translate(0, 10);
	sw.Move(50, 50).Line(400, 200).Dash("1 2 3 4 5").Stroke(5, Blue());
}

INITBLOCK {
	RegisterExample("Dash test", Dash);
}
