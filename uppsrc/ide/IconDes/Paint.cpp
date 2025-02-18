#include "IconDes.h"

#define IMAGECLASS IconDesImg
#define IMAGEFILE <ide/IconDes/IconDes.iml>
#include <Draw/iml_source.h>

void IconShow::Paint(Draw& w)
{
	Size sz = GetSize();
	static Color color[] = { White(), Black(), WhiteGray(), LtGray(), Gray(),
	                         Yellow(), Brown(), Red(), Green(), Blue(), Cyan(), Magenta() };
	Image image = this->image;
	if(show_downscaled || show_synthetics)
		if(flags & IML_IMAGE_FLAG_S3)
			image = DownSample3x(image, true);
	
	Size msz = image.GetSize();
	Size isz = msz;
	int  gap = DPI(8);
	double fits = msz.cx * msz.cy < 20000;
	if(fits) {
		if(show_downscaled) {
			isz.cx += (isz.cx + 1) / 2 + gap;
			isz.cy += max(isz.cy, (isz.cy + 1) / 2 + (isz.cy + 2) / 3 + gap);
		}
		if(show_synthetics) {
			if(flags & IML_IMAGE_FLAG_UHD) {
				isz.cx += isz.cx + gap;
				isz.cy += isz.cy + (isz.cy + 1 / 2) + 2 * gap;
			}
			else {
				isz.cx += 3 * isz.cx + gap;
				isz.cy += 2 * isz.cy + gap;
			}
		}
	}
	int n = isz.cx ? clamp(sz.cx / isz.cx, 1, __countof(color)) : 1;
	int ncx = sz.cx / n;
	Image m2, m3;
	Image dk, s2, s2dk, s22, s2dk2;
	if(fits) {
		if(msz.cx && show_downscaled) {
			m2 = DownSample2x(image);
			m3 = DownSample3x(image);
		}
		if(show_synthetics) {
			dk = DarkTheme(image);
			if(flags & IML_IMAGE_FLAG_UHD) {
				s2 = Downscale2x(image);
				s2dk = Downscale2x(DarkTheme(image));
				if(IsUHDMode()) {
					s22 = Magnify(s2, 2, 2, true);
					s2dk2 = Magnify(s2dk, 2, 2, true);
				}
			}
			else {
				s2 = Upscale2x(image);
				s2dk = Upscale2x(DarkTheme(image));
			}
		}
	}
	else
		isz = msz;

	for(int i = 0; i < n; i++) {
		int x = i * ncx;
		int cx = (i + 1) * ncx - x;
		w.DrawRect(x, 0, cx, sz.cy, color[i]);
		if(msz.cx) {
			Point pos(x + (cx - isz.cx) / 2, (sz.cy - isz.cy) / 2);
			if(fits) {
				int x2 = pos.x + isz.cx / 2;
				if(show_synthetics) {
					w.DrawImage(pos.x, pos.y, image);
					w.DrawImage(x2, pos.y, dk);
					pos.y += msz.cy + gap;
					w.DrawImage(pos.x, pos.y, s2);
					w.DrawImage(x2, pos.y, s2dk);
					if((flags & IML_IMAGE_FLAG_UHD) && IsUHDMode()) {
						pos.y += (msz.cy + 1) / 2 + gap;
						w.DrawImage(pos.x, pos.y, s22);
						w.DrawImage(x2, pos.y, s2dk2);
					}
				}
				else {
					int y = pos.y + (isz.cy - msz.cy) / 2;
					w.DrawImage(pos.x, y, image);
					if(show_downscaled) {
						w.DrawImage(x2, y, m2);
						w.DrawImage(x2, y + (msz.cx + 1) / 2 + gap, m3);
					}
				}
			}
			else {
				w.Clip(x, 0, isz.cx, sz.cy);
				w.DrawImage(x + DPI(2), DPI(2), image);
				w.End();
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
	Size isz = image.GetSize();
	bool pastepaint = HasCapture() && IsPasting();
	magnify = max(magnify, 1);
	if(magnify == 1) {
		w.DrawRect(sz, SColorPaper());
		ImageBuffer pb(isz);
		RGBA c1 = GrayColor(102);
		RGBA c2 = GrayColor(153);
		int xn = isz.cx / DPI(4);
		for(int y = 0; y < isz.cy; y++) {
			RGBA *b = pb[y];
			RGBA cc1 = c1;
			RGBA cc2 = c2;
			if(y & DPI(4))
				Swap(cc1, cc2);
			for(int q = xn; q--;) {
				Fill(b, cc1, DPI(4));
				Swap(cc1, cc2);
				b += DPI(4);
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
		if(show_grid2) {
			w.DrawRect(r.right, 0, 1, r.bottom + 1, LtRed());
			w.DrawRect(0, r.bottom, r.right, 1, LtRed());
		}
		m1refresh = Null;
		if(IsPasting())
			DrawFrame(w, Rect(Current().pastepos - spos, Current().paste_image.GetSize()), LtRed);
		return;
	}

	Rect src = Rect((sz + magnify) / magnify).Offseted(spos) & isz;
	Size ssz = src.GetSize();
	
	ImageBuffer b(src.GetSize() * magnify);
	int m2 = magnify / 2;
	CoFor(src.GetHeight(), [&](int y) { // create chequered background
		RGBA *q = b[y * magnify];
		int i = 0;
		Color c1 = GrayColor(102);
		Color c2 = GrayColor(153);
		int m = magnify - m2;
		for(int pass = 0; pass < 2; pass++) {
			RGBA *t = q;
			for(int i = 0; i < src.GetWidth(); i++) {
				Fill(q, c1, magnify);
				Fill(q + 1, c2, m2);
				q += magnify;
			}
			i++;
			while(i < m) {
				memcpy_t(q, t, b.GetWidth());
				q += b.GetWidth();
				i++;
			}
			m = magnify;
			Swap(c1, c2);
		}
	});
	
	w.DrawImage(0, 0, b); // paint chequered background
	w.DrawImage(0, 0, Magnify(image, src, magnify, magnify, true)); // paint actual image

	if(!pastepaint && !IsNull(selection)) { // paint selection
		RGBA c1 = 170 * Black();
		RGBA c2 = 80 * LtGray();
		int ncx = ssz.cx * magnify;
		ImageBuffer b(ncx, ssz.cy * magnify);
		CoFor(ssz.cy, [&](int y) {
			const RGBA *s = selection[src.top + y] + src.left;
			const RGBA *e = s + ssz.cx;
			RGBA *t = ~b + y * ncx * magnify;
			RGBA *q = t;
			while(s < e) {
				RGBA c = s->r ? RGBAZero() : c1;
				Fill(q, c, magnify);
				Fill(q + ncx, c, magnify);
				if(!s->r)
					for(int i = 0; i < magnify - 1; i += 2)
						q[i] = c2;
				q += magnify;
				s++;
			}
		
			for(int i = 2; i < magnify; i++) {
				q += ncx;
				memcpy_t(q, t, ncx);
				t += ncx;
			}
		});
		w.DrawImage(0, 0, b);
	}

	if(magnify > 6) {
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
		int m = ssz.cx * magnify + 1;
		for(int y = 0; y <= src.GetHeight(); y++)
			w.DrawRect(0, magnify * y, m, 1, GridColor(y + src.top, isz.cy));
		m = ssz.cy * magnify + 1;
		for(int x = 0; x <= src.GetWidth(); x++)
			w.DrawRect(magnify * x, 0, 1, m, GridColor(x + src.left, isz.cx));
	}

	if(IsPasting() && !pastepaint)
		DrawFatFrame(w, Rect(magnify * (Current().pastepos - spos),
		                     magnify * Current().paste_image.GetSize() + Size(1, 1)),
		             Color(200, 200, 255), 3);

	if(magnify > 8 || IsHotSpot()) {
		auto PaintHotSpot = [&](Point hotspot, Color c) {
			if(hotspot.x > 0 || hotspot.y > 0 || tool == &IconDes::HotSpotTool) {
				hotspot = (hotspot - spos) * magnify;
				DrawFatFrame(w, hotspot.x, hotspot.y, magnify + 1, magnify + 1, c, max(DPI(1), 1 + magnify / 8));
			}
		};
		PaintHotSpot(image.GetHotSpot(), LtRed());
		PaintHotSpot(image.Get2ndSpot(), LtBlue());
	}
}
