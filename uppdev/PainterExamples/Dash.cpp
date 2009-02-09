#include "Examples.h"

void Dash(Painter& sw)
{
	sw.Move(50, 50).Line(400, 200).Dash("2").Stroke(5, Blue());
}

INITBLOCK {
	RegisterExample("Dash test", Dash);
}
