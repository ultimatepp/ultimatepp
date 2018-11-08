#include "LionBenchmark.h"

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
