#include <CtrlCore/CtrlCore.h>

#ifdef GUI_GTK

NAMESPACE_UPP

void SetSurface(SystemDraw& w, const Rect& dest, const RGBA *pixels, Size psz, Point poff)
{
}

void DrawDragRect(SystemDraw& w, const Rect& rect1, const Rect& rect2, const Rect& clip, int n, Color color, uint64 pattern)
{
}

/*
Size GetScreenSize()
{
	return ScreenInfo().GetPageSize();
}
*/

void DrawDragRect(Ctrl& q, const Rect& rect1, const Rect& rect2, const Rect& clip, int n,
                  Color color, int type, int animation)
{
	ViewDraw w(&q);
	w.Clip(clip);
	cairo_t *cr = w;
	double dashes[] = { 1, 1, 1, 1, 1, 1, 1, 1 };
	w.SetColor(color);
	cairo_set_operator(cr, CAIRO_OPERATOR_DIFFERENCE);
	cairo_set_dash(cr, dashes, 8, 0);
	cairo_set_line_width(cr, 1);
	cairo_rectangle(cr, rect1.left + 0.5, rect1.top + 0.5, rect1.GetWidth() + 0.5, rect1.GetHeight() + 0.5);
	cairo_stroke(cr);
	cairo_rectangle(cr, rect2.left + 0.5, rect2.top + 0.5, rect2.GetWidth() + 0.5, rect2.GetHeight() + 0.5);
	cairo_stroke(cr);
	w.End();
/*	uint64 pattern = type == DRAWDRAGRECT_DASHED ? I64(0xf0783c1e0f87c3e1) :
	                 type == DRAWDRAGRECT_NORMAL ? I64(0x55aa55aa55aa55aa) : 0;	                                             
	DrawDragRect(w, rect1, rect2, clip, n, color, sGetAniPat(pattern, animation));*/
}

GdkRect::GdkRect(const Rect& r)
{
	x = r.left;
	y = r.top;
	width = r.GetWidth();
	height = r.GetHeight();
}

int rmsecs()
{
	static int msecs0 = msecs();
	return msecs(msecs0);
}
 
END_UPP_NAMESPACE

#endif
