#include "Examples.h"

void Div(Painter& sw)
{
	sw.Character(100, 100, 'O', Arial(100))
	  .Character(100, 150, 'O', Arial(100))
	  .Fill(Black());
	sw.Translate(200, 0);
	sw.Character(100, 100, 'O', Arial(100))
	  .Div()
	  .Character(100, 150, 'O', Arial(100))
	  .Fill(Black());
}

INITBLOCK {
	RegisterExample("Div", Div);
}
