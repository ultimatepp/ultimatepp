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

void Div2(Painter& sw)
{
	sw.Circle(200, 200, 100).Circle(270, 200, 100).EvenOdd().Fill(Black());
	sw.Translate(300, 0);
	sw.Circle(200, 200, 100).Div().Circle(270, 200, 100).EvenOdd().Fill(Black());
}

INITBLOCK {
	RegisterExample("Div", Div);
	RegisterExample("Div2", Div2);
}
