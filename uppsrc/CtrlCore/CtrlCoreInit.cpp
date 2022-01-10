#include "CtrlCore.h"

namespace Upp {

#define  TFILE <CtrlCore/CtrlCore.t>
#include <Core/t.h>

static Image sRenderGlyph(int cx, int x, Font font, int chr, int py, int pcy, Color fg, Color bg)
{
	ImageDraw iw(cx, pcy);
	iw.DrawRect(0, 0, cx, pcy, bg);
	iw.DrawText(x, -py, WString(chr, 1), font, fg);
	return iw;
}

void SetRenderGlyph(Image (*f)(int cx, int x, Font font, int chr, int py, int pcy, Color fg, Color bg));

INITIALIZER(CtrlCore) {
	SetRenderGlyph(sRenderGlyph);
}

}
