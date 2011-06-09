#include "CtrlCore.h"

NAMESPACE_UPP

#ifdef PLATFORM_WIN32

SystemDraw&  ScreenInfo();

bool ScreenInPaletteMode()
{
	return ScreenInfo().PaletteMode();
}

Size GetScreenSize()
{
	return ScreenInfo().GetPageSize();
}

HRGN GetFrameRgn(const Rect& rect, int n) {
	HRGN rgn = CreateRectRgnIndirect(rect);
	Rect r = rect;
	r.Deflate(n);
	if(r.Width() > 0 && r.Height() > 0) {
		HRGN rgnin = CreateRectRgnIndirect(r);
		CombineRgn(rgn, rgn, rgnin, RGN_XOR);
		DeleteObject(rgnin);
	}
	return rgn;
}

void DrawDragRect(SystemDraw& w, const Rect& _rect1, const Rect& _rect2, const Rect& _clip, int n, Color color, uint64 pattern)
{
	Point o = w.GetOffset();
	Rect rect1 = _rect1 + o;
	Rect rect2 = _rect2 + o;
	Rect clip = _clip + o;
	HDC hdc = w.BeginGdi();
	word wpat[8] = {
		(byte)(pattern >> 56), (byte)(pattern >> 48), (byte)(pattern >> 40), (byte)(pattern >> 32),
		(byte)(pattern >> 24), (byte)(pattern >> 16), (byte)(pattern >> 8), (byte)(pattern >> 0),
	};
	HBITMAP bitmap = CreateBitmap(8, 8, 1, 1, wpat);
	HBRUSH brush = ::CreatePatternBrush(bitmap);
	DeleteObject(bitmap);
	SetTextColor(hdc, color);
	SetBkColor(hdc, SColorText());
	Point offset;
#ifdef PLATFORM_WINCE
	offset = Point(0, 0);
#else
	::GetViewportOrgEx(hdc, offset);
#endif
	HRGN rgn = GetFrameRgn(rect1 + offset, n);
	HRGN rgn2 = GetFrameRgn(rect2 + offset, n);
	HRGN cliprgn = CreateRectRgnIndirect(clip + offset);
	CombineRgn(rgn, rgn, rgn2, RGN_XOR);
	CombineRgn(rgn, rgn, cliprgn, RGN_AND);
	SelectClipRgn(hdc, rgn);
	Rect r;
	GetClipBox(hdc, r);
	HBRUSH obrush = (HBRUSH) SelectObject(hdc, brush);
	PatBlt(hdc, r.left, r.top, r.Width(), r.Height(), PATINVERT);
	SelectObject(hdc, obrush);
	SelectClipRgn(hdc, NULL);
	DeleteObject(rgn);
	DeleteObject(rgn2);
	DeleteObject(cliprgn);
	ReleaseDC(NULL, hdc);
	DeleteObject(brush);
	w.EndGdi();
}

#endif

#ifdef PLATFORM_X11

Vector<Rect> RectRgn(const Rect& r)
{
	Vector<Rect> q;
	q.Add(r);
	return q;
}

Vector<Rect> Intersect(const Vector<Rect>& r1, const Vector<Rect>& r2)
{
	Vector<Rect> q;
	for(int i = 0; i < r1.GetCount(); i++)
		for(int j = 0; j < r2.GetCount(); j++) {
			Rect r = r1[i] & r2[j];
			if(!r.IsEmpty())
				q.Add(r);
		}
	return q;
}

Vector<Rect> Subtract(const Vector<Rect>& r1, const Vector<Rect>& r2)
{
	Vector<Rect> q;
	bool dummy;
	q <<= r1;
	for(int i = 0; i < r2.GetCount(); i++)
		q = Subtract(q, r2[i], dummy);
	return q;
}


Vector<Rect> Xor(const Vector<Rect>& r1, const Vector<Rect>& r2)
{
	Vector<Rect> is = Intersect(r1, r2);
	Vector<Rect> q = Subtract(r1, is);
	q.Append(Subtract(r2, is));
	return q;
}

Vector<Rect> GetFrameRgn(const Rect& rect, int n) {
	Vector<Rect> q = RectRgn(rect);
	q.Add(rect);
	Rect r = rect;
	r.Deflate(n);
	if(r.Width() > 0 && r.Height() > 0)
		q = Xor(q, RectRgn(r));
	return q;
}

void DrawDragRect(SystemDraw& w, const Rect& rect1, const Rect& rect2, const Rect& clip, int n,
                  Color color, uint64 pattern)
{
	char bd[8];
	for(int i = 0; i < 8; i++)
		bd[i] = ~(byte)(pattern >> (8 * (7 - i)));
	Pixmap stipple = XCreateBitmapFromData(Xdisplay, w.GetDrawable(), bd, 8, 8);
	Point offset = w.GetOffset();
	GC gc = XCreateGC(Xdisplay, w.GetDrawable(), 0, 0);
	SetClip(gc, w.GetXftDraw(),
	        Intersect(Xor(GetFrameRgn(rect1 + offset, n), GetFrameRgn(rect2 + offset, n)),
	                  RectRgn(clip + offset)));

	XGCValues gcv;
	gcv.function = X11_ROP2_XOR;
	gcv.foreground = GetXPixel(color);
	gcv.fill_style = FillStippled;
	gcv.stipple = stipple;
	XChangeGC(Xdisplay, gc, GCForeground|GCFunction|GCStipple|GCFillStyle, &gcv);
	XFillRectangle(Xdisplay, w.GetDrawable(), gc, 0, 0, Xwidth, Xheight);
	XFreePixmap(Xdisplay, stipple);
}

#endif

END_UPP_NAMESPACE
