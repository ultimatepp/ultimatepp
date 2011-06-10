#include "CtrlCore.h"

#ifdef GUI_WIN

NAMESPACE_UPP

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

END_UPP_NAMESPACE

#endif
