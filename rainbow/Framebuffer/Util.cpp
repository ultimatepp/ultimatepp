#include <CtrlCore/CtrlCore.h>

#ifdef GUI_FB

NAMESPACE_UPP

/*
static void sRenderLine(SystemDraw& w, int x, int y, int dx, int dy, int cx, int cy, int len, byte pattern)
{
	DLOG(len);
	for(int i = 0; i < len; i++)
		if((128 >> (i & 7)) & pattern)
			w.DrawRect(x + dx * i, y + dy * i, cx, cy, Black);
	DDUMP("~sRenderLine");
}

static void sRenderRect(SystemDraw& w, const Rect& r, int n, byte pattern)
{
	sRenderLine(w, r.left, r.top, 1, 0, 1, n, r.GetWidth(), pattern);
	sRenderLine(w, r.left, r.bottom - 1, 1, 0, 1, n, r.GetWidth(), pattern);
	sRenderLine(w, r.left, r.top, 0, 1, n, 1, r.GetHeight(), pattern);
	sRenderLine(w, r.right - 1, r.top, 0, 1, n, 1, r.GetHeight(), pattern);
}

void DrawDragRect(SystemDraw& w, const Rect& rect1, const Rect& rect2,
                  const Rect& clip, int n, Color color, uint64 pattern)
{
	DLOG("@ DrawDragRect " << rect1 << " " << rect2 << ", clip: " << clip << ", pattern: " << pattern);
	TIMING("DrawDrawRect");
	w.Clip(clip);
	w.Invert();
	sRenderRect(w, rect1, n, (byte)pattern);
	sRenderRect(w, rect2, n, (byte)pattern);
	Ctrl::AddUpdate((rect1 | rect2).Offseted(w.GetOffset()));
//	MemoryProfile mem;
//	DDUMP(mem);
	w.End();
}




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
*/


/*
Size GetScreenSize()
{
	return ScreenInfo().GetPageSize();
}
*/

void DrawDragRect(Ctrl& q, const Rect& rect1, const Rect& rect2, const Rect& clip, int n,
                  Color color, int type, int animation)
{
	q.DragRectDraw(rect1, rect2, clip, n, color, type, animation);
}

void FinishDragRect(Ctrl& q)
{
}

END_UPP_NAMESPACE

#endif
