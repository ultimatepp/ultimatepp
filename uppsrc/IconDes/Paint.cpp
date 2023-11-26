#include "IconDes.h"

namespace Upp {

#define IMAGECLASS IconDesImg
#define IMAGEFILE <IconDes/IconDes.iml>
#include <Draw/iml_source.h>

void IconShow::Paint(Draw& w)
{
	Size sz = GetSize();
	static Color color[] = { White(), WhiteGray(), LtGray(), Gray(), Black(),
	                         Yellow(), Brown(), Red(), Green(), Blue(), Cyan(), Magenta() };
	Size msz = image.GetSize();
	Size isz = msz;
	if(show_small)
		isz.cx += isz.cx / 2 + DPI(4);
	int n = msz.cx ? minmax(sz.cx / isz.cx, 1, __countof(color)) : 1;
	Image m2, m3;
	if(msz.cx && show_small) {
		m2 = DownSample2x(image);
		m3 = DownSample3x(image);
	}
	for(int i = 0; i < n; i++) {
		int x = i * sz.cx / n;
		int cx = (i + 1) * sz.cx / n - x;
		w.DrawRect(x, 0, cx, sz.cy, color[i]);
		if(msz.cx) {
			if(show_other) {
				Point c(x + cx / 2, sz.cy / 2);
				w.DrawImage(c.x - DPI(8) - isz.cx, c.y - DPI(8) - isz.cy, image);
				w.DrawImage(c.x + DPI(8), c.y - DPI(8) - isz.cy, DarkTheme(image));
				w.DrawImage(c.x - DPI(8) - isz.cx, c.y + DPI(8), Upscale2x(image));
				w.DrawImage(c.x + DPI(8), c.y + DPI(8), Upscale2x(DarkTheme(image)));
			}
			else {
				w.DrawImage(x + (cx - isz.cx) / 2, (sz.cy - isz.cy) / 2, image);
				if(show_small) {
					w.DrawImage(x + (cx - isz.cx) / 2 + msz.cx + DPI(4), (sz.cy - isz.cy) / 2, m2);
					w.DrawImage(x + (cx - isz.cx) / 2 + msz.cx + DPI(4), (sz.cy - isz.cy) / 2 + 2 * msz.cy / 3, m3);
				}
			}
		}
	}
}

void sDrawChRect(Draw& w, int x0, int y, int cx, int cy, bool start = false)
{
	w.DrawRect(x0, y, cx, cy, Color(255, 150, 200));
}

void IconDes::Paint(Draw& w)
{
	Size sz = GetSize();
	if(!IsCurrent()) {
		w.DrawRect(sz, SColorPaper());
		return;
	}
	const Image& image = Current().image;
	const Image& selection = Current().selection;
	Point spos = sb;
	scroller.Set(spos);
	Size isz = image.GetSize();
	bool pastepaint = HasCapture() && IsPasting();
	magnify = max(magnify, 1);
	if(magnify == 1) {
		w.DrawRect(sz, SColorPaper());
		ImageBuffer pb(isz);
		RGBA c1 = GrayColor(102);
		RGBA c2 = GrayColor(153);
		int xn = isz.cx / 4;
		for(int y = 0; y < isz.cy; y++) {
			RGBA *b = pb[y];
			RGBA cc1 = c1;
			RGBA cc2 = c2;
			if(y & 4)
				Swap(cc1, cc2);
			for(int q = xn; q--;) {
				b[0] = b[1] = b[2] = b[3] = cc1;
				Swap(cc1, cc2);
				b += 4;
			}
			while(b < pb[y] + isz.cx)
				*b++ = cc1;
		}
		AlphaBlend(pb, image, (int)image.GetLength());
		if(!pastepaint)
			for(int y = 0; y < isz.cy; y++) {
				const RGBA *m = selection[y];
				RGBA *t = pb[y];
				const RGBA *e = m + isz.cx;
				while(m < e) {
					if(!m->r)
						t->r = t->g = t->b = ((t->r + t->g + t->b) >> 2);
					m++;
					t++;
				}
			}
		w.DrawImage(-spos.x, -spos.y, pb);
		Rect r = pb.GetSize();
		r.Offset(-spos);
		w.DrawRect(r.right, 0, 1, r.bottom + 1, LtRed());
		w.DrawRect(0, r.bottom, r.right, 1, LtRed());
		m1refresh = Null;
		if(IsPasting())
			DrawFrame(w, Rect(Current().pastepos - spos, Current().paste_image.GetSize()), LtRed);
		return;
	}
	Point hotspot = image.GetHotSpot();
	Point spot2 = image.Get2ndSpot();
	int m = isz.cx * magnify + 1;
	Point mpos = magnify * spos;
	int grid2 = 0;
	if(max(isz.cx, isz.cy) > 16 && show_grid2) {
		if(isz.cx % 3 == 0 && isz.cy % 3 == 0)
			grid2 = 3;
		else
		if(isz.cx % 4 == 0 && isz.cy % 4 == 0)
			grid2 = 4;
	}
	auto GridColor = [&](int x, int sz) -> Color {
		if(grid2) {
			if(x == sz / 2)
				return LtMagenta();
			if(x % grid2 == 0)
				return LtRed();
		}
		return Black();
	};
	for(int y = isz.cy; y >= 0; y--)
		w.DrawRect(-mpos.x, magnify * y - mpos.y, m, 1, GridColor(y, isz.cy));
	m = isz.cy * magnify + 1;
	for(int x = isz.cx; x >= 0; x--)
		w.DrawRect(magnify * x - mpos.x, -mpos.y, 1, m, GridColor(x, isz.cx));
	if(!IsHotSpot()) {
		hotspot = Null;
		spot2 = Null;
	}
	w.DrawRect(isz.cx * magnify - mpos.x + 1, 0, 9999, 9999, SColorPaper());
	w.DrawRect(0, isz.cy * magnify - mpos.y + 1, 9999, 9999, SColorPaper());
	int my0 = -magnify * spos.y;
	int magnify4 = 4 * magnify;
	int md = (magnify + 1) / 2;
	int mc = (magnify - 1) / 2;
	Image masked = Crop(IconDesImg::Masked(), Size(magnify - 1, magnify - 1));
	for(int y0 = 0; y0 < isz.cy; y0 += 4, my0 += magnify4) {
		int mx0 = -magnify * spos.x;
		for(int x0 = 0; x0 < isz.cx; x0 += 4, mx0 += magnify4) {
			if(w.IsPainting(mx0, my0, magnify4, magnify4)) {
				int ex = min(x0 + 4, isz.cx);
				int my = my0;
				for(int y = y0; y < min(y0 + 4, isz.cy); y++, my += magnify) {
					const RGBA *s = image[y] + x0;
					const RGBA *k = selection[y] + x0;
					const RGBA *pk = y ? selection[y - 1] + x0 : NULL;
					const RGBA *nk = y + 1 < isz.cy ? selection[y + 1] + x0 : NULL;
					int mx = mx0;
					for(int x = x0; x < ex; x++) {
						if(s->a == 255)
							w.DrawRect(mx + 1, my + 1, magnify - 1, magnify - 1, *s);
						else { // paint chequered background
							Color c = StraightColor(*s);
							Color c1 = Blend(GrayColor(102), c, s->a);
							w.DrawRect(mx + 1, my + 1, magnify - 1, magnify - 1, c1);
							Color c2 = Blend(GrayColor(153), c, s->a);
							w.DrawRect(mx + md, my + 1, mc, mc, c2);
							w.DrawRect(mx + 1, my + md, mc, mc, c2);
						}
						if(!pastepaint)
							if(!k->r) {
								w.DrawImage(mx + 1, my + 1, masked);
								if(x > 0) {
									if(pk && pk[-1].r)
										sDrawChRect(w, mx + 1, my + 1, 4, 4);
									if(k[-1].r)
										sDrawChRect(w, mx + 1, my + 1, 4, magnify - 1);
									if(nk && nk[-1].r)
										sDrawChRect(w, mx + 1, my + magnify - 4, 4, 4, true);
								}
								if(pk && pk->r)
									sDrawChRect(w, mx + 1, my + 1, magnify - 1, 4);
								if(nk && nk->r)
									sDrawChRect(w, mx + 1, my + magnify - 4, magnify - 1, 4, true);
								if(x + 1 < isz.cx) {
									if(pk && pk[1].r)
										sDrawChRect(w, mx + magnify - 4, my + 1, 4, 4, true);
									if(k[1].r)
										sDrawChRect(w, mx + magnify - 4, my + 1, 4, magnify - 1, true);
									if(nk && nk[1].r)
										sDrawChRect(w, mx + magnify - 4, my + magnify - 4, 4, 4);
								}
							}
						if(spot2.x == x && spot2.y == y)
							w.DrawImage(mx + 1, my + 1, Rescale(IconDesImg::HotSpotB(), magnify - 1, magnify - 1));
						if(hotspot.x == x && hotspot.y == y)
							w.DrawImage(mx + 1, my + 1, Rescale(IconDesImg::HotSpotA(), magnify - 1, magnify - 1));
						s++;
						k++;
						mx += magnify;
						if(pk)
							pk++;
						if(nk)
							nk++;
					}
				}
			}
		}
	}
	if(IsPasting() && !pastepaint)
		DrawFatFrame(w, Rect(magnify * (Current().pastepos - spos),
		                     magnify * Current().paste_image.GetSize() + Size(1, 1)),
		             Color(200, 200, 255), 3);

	if(magnify < 9)
		return;
	auto PaintHotSpot = [&](Point hotspot, Color c) { // Show hotspots even if not designing them
		if(hotspot.x > 0 || hotspot.y > 0)
			DrawFatFrame(w, hotspot.x * magnify, hotspot.y * magnify, magnify + 1, magnify + 1, c, 1 + magnify / 8);
	};
	PaintHotSpot(image.GetHotSpot(), LtRed());
	PaintHotSpot(image.Get2ndSpot(), LtBlue());
}

}
