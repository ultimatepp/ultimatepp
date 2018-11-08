#include "Examples.h"

void ClippingExample(Painter& sw)
{
	Font fnt = Roman(150).Bold();
	String txt = "CLIPPED!";
	Size tsz = GetTextSize(txt, fnt);
	sw.Begin();
		sw.Ellipse(tsz.cx / 2, tsz.cy / 2, tsz.cx / 2, tsz.cy / 4).Clip();
		sw.Text(0, 0, txt, fnt).Fill(Green());
	sw.End();
	sw.Text(0, 0, txt, fnt).Stroke(0.5, Black());
}

void NoClippingExample(Painter& sw)
{
	Font fnt = Roman(150).Bold();
	String txt = "CLIPPED!";
	sw.Text(0, 0, txt, fnt).Fill(Green());
	sw.Text(0, 0, txt, fnt).Stroke(0.5, Black());
}

void ClippingExample2(Painter& sw)
{
	sw.Begin();
		sw.Ellipse(200, 200, 200, 200).Stroke(2, Red()).Clip();
		sw.Begin();
			sw.Ellipse(200, 200, 100, 300).Clip().Stroke(2, Red());
			sw.Text(0, 0, "CL", Arial(360).Bold()).Fill(Blue());
		sw.End();
		sw.Text(0, 0, "CL", Arial(360).Bold()).Opacity(0.4).Fill(Black());
	sw.End();
}

INITBLOCK {
	RegisterExample("Clipping", ClippingExample);
	RegisterExample("Clipping removed (to benchmark) ", NoClippingExample);
	RegisterExample("Clipping 2", ClippingExample2);
}
