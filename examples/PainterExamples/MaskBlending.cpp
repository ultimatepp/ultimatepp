#include "Examples.h"

void MaskBlending(Painter& sw)
{
	const char *txt = "This is just a test of alpha mask blending";
	Size tsz = GetTextSize(txt, Roman(50).Italic());
	sw.Begin();
		sw.BeginMask();
			sw.Move(0, 0).Line(tsz.cx + 20, 0).Line(tsz.cx + 20, tsz.cy + 20).Line(0, tsz.cy + 20)
			  .Fill(0, 0, White(), tsz.cx, 0, Black());
		sw.End();
		sw.Text(10, 10, txt, Roman(50)).Fill(Red());
	sw.End();
	sw.Begin();
		sw.BeginMask();
			sw.Move(0, 0).Line(tsz.cx + 20, 0).Line(tsz.cx + 20, tsz.cy + 20).Line(0, tsz.cy + 20)
			  .Fill(0, 0, Black(), tsz.cx, 0, White());
		sw.End();
		sw.Text(10, 10, txt, Roman(50).Italic()).Fill(Blue());
	sw.End();
}

INITBLOCK {
	RegisterExample("Blending using alpha mask", MaskBlending);
}
