#include "Examples.h"

void RadialMaskBlending(Painter& sw)
{
	const char *txt = "This is just a test of radial mask blending";
	Size tsz = GetTextSize(txt, Roman(40).Italic());
	sw.Begin();
		sw.BeginMask();
			sw.Move(0, 0).Line(tsz.cx + 20, 0)
			  .Line(tsz.cx + 20, tsz.cx + tsz.cy + 20).Line(0, tsz.cx + tsz.cy + 20)
			  .Fill(tsz.cx / 2, tsz.cy + tsz.cx / 2, White(), tsz.cx / 2, Black());
		sw.End();
		for(int y = 0; y < tsz.cx; y += tsz.cy)
			sw.Text(10, y, txt, Roman(40))
			  .Fill(Red());
	sw.End();
	sw.Begin();
		sw.BeginMask();
			sw.Move(0, 0).Line(tsz.cx + 20, 0)
			  .Line(tsz.cx + 20, tsz.cx + tsz.cy + 20).Line(0, tsz.cx + tsz.cy + 20)
			  .Fill(tsz.cx / 2, tsz.cy + tsz.cx / 2, Black(), tsz.cx / 2, White());
		sw.End();
		for(int y = 0; y < tsz.cx; y += tsz.cy)
			sw.Text(10, y, txt, Roman(40).Italic())
			  .Fill(Blue());
	sw.End();
}

INITBLOCK {
	RegisterExample("Blending using radial alpha mask", RadialMaskBlending);
}
