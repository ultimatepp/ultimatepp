#include <CtrlCore/CtrlCore.h>

#ifdef GUI_SKELETON

NAMESPACE_UPP

void DrawDragRect(SystemDraw& w, const Rect& rect1, const Rect& rect2, const Rect& clip, int n, Color color, uint64 pattern)
{
}

/*
Size GetScreenSize()
{
	return ScreenInfo().GetPageSize();
}
*/

static uint64 sGetAniPat(uint64 src, int pos)
{
	uint64 out = 0;
	pos &= 7;
	for(int i = 8; --i >= 0;) {
		byte sr = (byte)(src >> (8 * ((7 - i - pos) & 7)));
		out = (out << 8) | (byte)((sr | (sr << 8)) >> pos);
	}
	return out;
}

void DrawDragRect(Ctrl& q, const Rect& rect1, const Rect& rect2, const Rect& clip, int n,
                  Color color, int type, int animation)
{
	ViewDraw w(&q);
	uint64 pattern = type == DRAWDRAGRECT_DASHED ? I64(0xf0783c1e0f87c3e1) :
	                 type == DRAWDRAGRECT_NORMAL ? I64(0x55aa55aa55aa55aa) : 0;	                                             
	DrawDragRect(w, rect1, rect2, clip, n, color, sGetAniPat(pattern, animation));
}

END_UPP_NAMESPACE

#endif
