#include "IconDes.h"

NAMESPACE_UPP

#define IMAGECLASS IconDesImg
#define IMAGEFILE <IconDes/IconDes.iml>
#include <Draw/iml_source.h>

void IconShow::Paint(Draw& w)
{
	Size sz = GetSize();
	static Color color[] = { White(), WhiteGray(), LtGray(), Gray(), Black(),
	                         Yellow(), Brown(), Red(), Green(), Blue(), Cyan(), Magenta() };
	Size isz = image.GetSize();
	int n = isz.cx ? minmax(sz.cx / isz.cx, 1, __countof(color)) : 1;
	for(int i = 0; i < n; i++) {
		int x = i * sz.cx / n;
		int cx = (i + 1) * sz.cx / n - x;
		w.DrawRect(x, 0, cx, sz.cy, color[i]);
		if(isz.cx)
			w.DrawImage(x + (cx - isz.cx) / 2, (sz.cy - isz.cy) / 2, image);
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
		AlphaBlend(pb, image, image.GetLength());
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
		m1refresh = Null;
		if(IsPasting() && IsPasting())
			DrawFrame(w, Rect(spos, Current().paste_image.GetSize()), LtRed);
		return;
	}
	Point hotspot = image.GetHotSpot();
	Point spot2 = image.Get2ndSpot();
	if(!IsHotSpot()) {
		hotspot = Null;
		spot2 = Null;
	}
	int m = isz.cx * magnify + 1;
	Point mpos = magnify * spos;
	for(int y = isz.cy * magnify; y >= 0; y -= magnify)
		w.DrawRect(-mpos.x, y - mpos.y, m, 1, Black());
	m = isz.cy * magnify + 1;
	for(int x = isz.cx * magnify; x >= 0; x -= magnify)
		w.DrawRect(x - mpos.x, -mpos.y, 1, m, Black());
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
						else {
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
}

END_UPP_NAMESPACE
