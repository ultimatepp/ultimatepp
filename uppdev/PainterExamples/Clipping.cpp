#include "Examples.h"

void ClippingExample(Painter& sw)
{
	sw.Rectangle(0, 0, 100000, 100000).Clip();
	
	Font fnt = Roman(150).Bold();
	String txt = "CLIPPED!";
	Size tsz = GetTextSize(txt, fnt);
	sw.Begin();
		sw.Ellipse(tsz.cx / 2, tsz.cy / 2, tsz.cx / 2, tsz.cy / 4).Clip();
		sw.Text(0, 0, txt, fnt).Fill(Green());
	sw.End();
	sw.Text(0, 0, txt, fnt).Stroke(0.5, Black());
}

INITBLOCK {
	RegisterExample("Clipping", ClippingExample);
}
