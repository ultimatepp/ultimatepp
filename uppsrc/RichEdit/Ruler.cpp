#include "RichEdit.h"

namespace Upp {

#define IMAGECLASS RichEditImg
#define IMAGEFILE <RichEdit/RichEdit.iml>
#include <Draw/iml_source.h>

void HotPaint(Draw& w, int x, int y, const Image& img)
{
	Point p = img.GetHotSpot();
	w.DrawImage(x - p.x, y - p.y, img);
}

RichRuler::RichRuler()   { newtabalign = ALIGN_LEFT; }
RichRuler::~RichRuler()  {}

void RichRuler::FrameLayout(Rect& r)
{
	LayoutFrameTop(r, this, Arial(Zy(10)).GetHeight() + Zy(8));
}

void RichRuler::FrameAddSize(Size& sz)
{
	sz.cy += Arial(Zy(10)).GetHeight() + Zy(8);
}

void RichRuler::Paint(Draw& w)
{
	Size sz = GetSize();
	w.DrawRect(sz, SColorFace);
	w.DrawRect(0, sz.cy - Zx(1), sz.cx, Zy(1), SColorShadow);
	int cx = zoom * pgcx;
	w.DrawRect(x0 - Zx(1), Zy(3), cx + Zx(3), sz.cy - Zy(6), SColorPaper);
	int i = 0;
	if(grid * zoom.AsDouble() > 2)
		for(;;) {
			int x = fround(++i * grid) * zoom;
			if(x >= cx) break;
			int h = (sz.cy - Zy(6)) / 3;
			if(i % marks == 0)
				w.DrawRect(x0 + x, Zy(2) + h, Zx(1), h + Zy(2), SColorHighlight);
			else
				w.DrawRect(x0 + x, Zy(3) + h, Zx(1), h, SColorHighlight);
		}
	i = 0;
	int xs = 0;
	for(;;)
		if(++i % numbers == 0) {
			int x = fround(i * grid) * zoom;
			if(x >= cx) break;
			String n = Format("%d", (int)(i * numbermul + 0.5));
			Size tsz = GetTextSize(n, ArialZ(9));
			int px = x0 + x - tsz.cx / 2;
			if(px >= xs && x + tsz.cx - tsz.cx / 2 < cx) {
				w.DrawRect(px, Zy(4), tsz.cx, sz.cy - Zy(8), SColorPaper);
				
				w.DrawText(px, Zy(4) + (sz.cy - Zy(8) - tsz.cy) / 2,
				           n, ArialZ(9), SColorText);
				xs = px + tsz.cx + Zx(4);
			}
		}
	DrawFrame(w, RectC(x0 - Zx(1), Zy(3), cx + Zx(3), sz.cy - Zy(6)), SColorText());
	w.Clip(0, 0, x0 + cx + RichEditImg::Margin().GetWidth() / 2, sz.cy);
	for(i = marker.GetCount() - 1; i >= 0; --i) {
		const Marker& m = marker[i];
		if(!IsNull(m.pos))
			HotPaint(w, x0 + m.pos * zoom, m.top ? Zy(1) : sz.cy - Zy(4), m.image);
	}
	w.End();
	i = 0;
	if(tabsize)
		for(;;) {
			int xp = ++i * tabsize;
			int x = xp * zoom;
			if(x >= cx) break;
			if(xp > tabpos)
				w.DrawRect(x0 + x, sz.cy - Zy(4), Zx(1), Zy(3), SColorShadow);
		}
	w.DrawImage(Zx(4), Zy(6), newtabalign == ALIGN_RIGHT  ? RichEditImg::RightTab() :
	                          newtabalign == ALIGN_CENTER ? RichEditImg::CenterTab() :
	                                                        RichEditImg::LeftTab());
}

int RichRuler::FindMarker(Point p)
{
	int x = p.x - x0;
	bool top = p.y < GetSize().cy / 2;
	for(int i = 0; i < marker.GetCount(); i++) {
		const Marker& m = marker[i];
		int hx = m.image.GetHotSpot().x;
		int cx = m.image.GetSize().cx;
		int p = zoom * m.pos;
		if(m.top == top && x >= p - hx && x < p + cx - hx)
			return i;
	}
	return -1;
}

void RichRuler::LeftDown(Point p, dword)
{
	track = FindMarker(p);
	if(track >= 0) {
		trackdx = marker[track].pos * zoom + x0 - p.x;
		SetCapture();
		WhenBeginTrack();
	}
	else
	if(p.x < Zx(16)) {
		newtabalign++;
		if(newtabalign > ALIGN_CENTER) newtabalign = ALIGN_LEFT;
		Refresh();
		return;
	}
	else {
		pos = ((p.x - x0) / zoom + snap / 2) / snap * snap;
		WhenLeftDown();
	}
}

void RichRuler::LeftDouble(Point p, dword)
{
	if(p.x < x0 - Zx(3)) {
		newtabalign++;
		if(newtabalign > ALIGN_CENTER) newtabalign = ALIGN_LEFT;
		Refresh();
		return;
	}

	WhenLeftDouble();
}

void RichRuler::RightDown(Point p, dword)
{
	if(p.x < x0 - Zx(3)) {
		newtabalign--;
		if(newtabalign < ALIGN_LEFT) newtabalign = ALIGN_CENTER;
		Refresh();
		return;
	}

	track = FindMarker(p);
	if(track < 0)
		pos = ((p.x - x0) / zoom + snap / 2) / snap * snap;
	WhenRightDown();
}

void RichRuler::LeftUp(Point p, dword)
{
	track = -1;
	WhenEndTrack();
}

void RichRuler::MouseMove(Point p, dword flags)
{

	if(HasCapture() && track >= 0) {
		Marker& m = marker[track];
		if((p.y < Zy(-10) || p.y >= GetSize().cy + Zy(10)) && m.deletable)
			m.pos = Null;
		else {
			int x = ((p.x + trackdx - x0) / zoom);
			if(!(flags & K_ALT))
				x = (x + snap / 2) / snap * snap;
			m.pos = minmax(x, m.minpos, m.maxpos);
		}
		Refresh();
		WhenTrack();
	}
}

void RichRuler::SetLayout(int x, int _pgcx, Zoom _zoom,
                          double _grid, int _numbers, double _numbermul, int _marks, int _snap)
{
	if(x0 != x || pgcx != _pgcx || zoom != _zoom || grid != _grid || numbers != _numbers ||
	   numbermul != _numbermul || marks != _marks || snap != _snap) {
		x0 = x;
		pgcx = _pgcx;
		zoom = _zoom;
		grid = _grid;
		numbers = _numbers;
		numbermul = _numbermul;
		marks = _marks;
		snap = _snap;
		Refresh();
	}
}

void RichRuler::Clear()
{
	if(marker.GetCount()) {
		marker.Clear();
		Refresh();
	}
}

void RichRuler::SetCount(int n)
{
	if(marker.GetCount() != n) {
		marker.SetCount(n);
		Refresh();
	}
}

void RichRuler::Set(int i, const Marker& m)
{
	if(i >= marker.GetCount() || marker[i] != m) {
		marker.At(i) = m;
		Refresh();
	}
}

void RichRuler::SetTabs(int pos, int size)
{
	if(tabpos != pos || tabsize != size) {
		tabpos = pos;
		tabsize = size;
		Refresh();
	}
}

}
