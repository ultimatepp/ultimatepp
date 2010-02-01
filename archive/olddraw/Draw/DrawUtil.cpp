#include "Draw.h"

NAMESPACE_UPP

#define LTIMING(x) // TIMING(x)

void AddNotEmpty(Vector<Rect>& result, int left, int right, int top, int bottom)
{
	if(left < right && top < bottom)
		result.Add(Rect(left, top, right, bottom));
}

bool Subtract(const Rect& r, const Rect& sub, Vector<Rect>& result)
{
	LTIMING("SubtractRect0");
	Rect is = r & sub;
	if(!is.IsEmpty()) {
		AddNotEmpty(result, r.left, is.left, r.top, is.top);
		AddNotEmpty(result, is.left, is.right, r.top, is.top);
		AddNotEmpty(result, is.right, r.right, r.top, is.top);
		AddNotEmpty(result, r.left, is.left, is.top, is.bottom);
		AddNotEmpty(result, is.right, r.right, is.top, is.bottom);
		AddNotEmpty(result, r.left, is.left, is.bottom, r.bottom);
		AddNotEmpty(result, is.left, is.right, is.bottom, r.bottom);
		AddNotEmpty(result, is.right, r.right, is.bottom, r.bottom);
		return true;
	}
	else {
		result.Add(r);
		return false;
	}
}

bool Subtract(const Vector<Rect>& rr, const Rect& sub, Vector<Rect>& result)
{
	LTIMING("SubtractRect");
	bool changed = false;
	for(int i = 0; i < rr.GetCount(); i++) {
		const Rect& r = rr[i];
		if(Subtract(r, sub, result))
			changed = true;
	}
	return changed;
}

Vector<Rect> Subtract(const Vector<Rect>& rr, const Rect& sub, bool& changed)
{
	Vector<Rect> result;
	if(Subtract(rr, sub, result))
		changed = true;
	return result;
}

void Subtract(Vector<Rect>& rr, const Rect& sub)
{
	LTIMING("Subtract");
	if(sub.IsEmpty())
		return;
	bool dummy;
	rr = Subtract(rr, sub, dummy);
}

void Union(Vector<Rect>& rr, const Rect& add)
{
	LTIMING("Union");
	if(add.IsEmpty())
		return;
	Vector<Rect> r;
	r.Add(add);
	for(int i = 0; i < rr.GetCount() && r.GetCount(); i++)
		Subtract(r, rr[i]);
	for(int i = 0; i < r.GetCount(); i++)
		rr.Add(r[i]);
}

Vector<Rect> Intersect(const Vector<Rect>& b, const Rect& a, bool& changed)
{
	Vector<Rect> result;
	for(int i = 0; i < b.GetCount(); i++) {
		Rect r = b[i] & a;
		if(r.IsEmpty())
			changed = true;
		else {
			if(r != b[i]) changed = true;
			result.Add(r);
		}
	}
	return result;
}

void DrawFatFrame(Draw& w, int x, int y, int cx, int cy, Color color, int n) {
	if(n < 0) {
		x += n;
		y += n;
		n = -n;
		cx += 2 * n;
		cy += 2 * n;
	}
	w.DrawRect(x, y, cx, n, color);
	w.DrawRect(x, y + n, n, cy - n, color);
	w.DrawRect(x + cx - n, y + n, n, cy - n, color);
	w.DrawRect(x + n, y + cy - n, cx - 2 * n, n, color);
}

void DrawFatFrame(Draw& w, const Rect& r, Color color, int n) {
	DrawFatFrame(w, r.left, r.top, r.Width(), r.Height(), color, n);
}

void DrawFrame(Draw& w, int x, int y, int cx, int cy,
			   Color leftcolor, Color topcolor, Color rightcolor, Color bottomcolor)
{
	w.DrawRect(x, y, cx - 1, 1, topcolor);
	w.DrawRect(x, y, 1, cy - 1, leftcolor);
	w.DrawRect(x + cx - 1, y, 1, cy, rightcolor);
	w.DrawRect(x, y + cy - 1, cx, 1, bottomcolor);
}

void DrawFrame(Draw& w, const Rect& r,
			   Color leftcolor, Color topcolor, Color rightcolor, Color bottomcolor)
{
	DrawFrame(w, r.left, r.top, r.Width(), r.Height(),
		      leftcolor, topcolor, rightcolor, bottomcolor);
}

void DrawFrame(Draw& w, int x, int y, int cx, int cy,
			   Color topleftcolor, Color bottomrightcolor)
{
	DrawFrame(w, x, y, cx, cy, topleftcolor, topleftcolor, bottomrightcolor, bottomrightcolor);
}

void DrawFrame(Draw& w, const Rect& r,
			   Color topleftcolor, Color bottomrightcolor) {
	DrawFrame(w, r, topleftcolor, topleftcolor, bottomrightcolor, bottomrightcolor);
}

void DrawFrame(Draw& w, int x, int y, int cx, int cy, Color color) {
	DrawFrame(w, x, y, cx, cy, color, color);
}

void DrawFrame(Draw& w, const Rect& r, Color color) {
	DrawFrame(w, r, color, color);
}

void DrawBorder(Draw& w, int x, int y, int cx, int cy, const ColorF *c) {
	if(!c) return;
	int n = (int)(uintptr_t)*c;
	c++;
	while(n--) {
		if(cx <= 0 || cy <= 0)
			break;
		DrawFrame(w, x, y, cx, cy, c[0], c[1], c[2], c[3]);
		x += 1;
		y += 1;
		cx -= 2;
		cy -= 2;
		c += 4;
	}
}

void DrawBorder(Draw& w, const Rect& r, const ColorF *c) {
	DrawBorder(w, r.left, r.top, r.Width(), r.Height(), c);
}

const ColorF *BlackBorder()
{
	static ColorF data[] = {
		(ColorF)1,
		&SColorText, &SColorText, &SColorText, &SColorText,
	};
	return data;
}

const ColorF *DefButtonBorder()
{
	static ColorF data[] = {
		(ColorF)3,
		&SColorText, &SColorText, &SColorText, &SColorText,
		&SColorLight, &SColorLight, &SColorText, &SColorText,
		&SColorLtFace, &SColorLtFace, &SColorShadow, &SColorShadow,
	};
	return data;
}

const ColorF *ButtonBorder()
{
	static ColorF data[] = {
		(ColorF)2,
		&SColorLight, &SColorLight, &SColorText, &SColorText,
		&SColorLtFace, &SColorLtFace, &SColorShadow, &SColorShadow,
	};
	return data;
}

const ColorF *EdgeButtonBorder()
{
	static ColorF data[] = {
		(ColorF)2,
		&SColorLtFace, &SColorLtFace, &SColorText, &SColorText,
		&SColorLight, &SColorLight, &SColorShadow, &SColorShadow,
	};
	return data;
}

const ColorF *ButtonPushBorder()
{
	static ColorF data[] = {
		(ColorF)2,
		&SColorText, &SColorText, &SColorText, &SColorText,
		&SColorShadow, &SColorShadow, &SColorText, &SColorText
	};
	return data;
}

const ColorF *InsetBorder()
{
	static ColorF data[] = {
		(ColorF)2,
		&SColorShadow, &SColorShadow, &SColorLight, &SColorLight,
		&SColorText, &SColorText, &SColorFace, &SColorFace
	};
	return data;
}

const ColorF *OutsetBorder()
{
	static ColorF data[] = {
		(ColorF)2,
		&SColorFace, &SColorFace, &SColorText, &SColorText,
		&SColorLight, &SColorLight, &SColorShadow, &SColorShadow,
	};
	return data;
}

const ColorF *ThinOutsetBorder()
{
	static ColorF data[] = {
		(ColorF)1,
		&SColorLight, &SColorLight, &SColorShadow, &SColorShadow,
	};
	return data;
}

const ColorF *ThinInsetBorder()
{
	static ColorF data[] = {
		(ColorF)1,
		&SColorShadow, &SColorShadow, &SColorLight, &SColorLight,
	};
	return data;
}

void DrawBorder(Draw& w, int x, int y, int cx, int cy, const ColorF *(*colors_ltrd)())
{
	DrawBorder(w, x, y, cx, cy, (*colors_ltrd)());
}

void DrawBorder(Draw& w, const Rect& r, const ColorF *(*colors_ltrd)())
{
	DrawBorder(w, r, (*colors_ltrd)());
}

void DrawRectMinusRect(Draw& w, const Rect& rect, const Rect& inner, Color color) {
	Rect r = rect;
	r.bottom = inner.top;
	w.DrawRect(r, color);
	r = inner;
	r.right = r.left;
	r.left = rect.left;
	w.DrawRect(r, color);
	r.left = inner.right;
	r.right = rect.right;
	w.DrawRect(r, color);
	r = rect;
	r.top = inner.bottom;
	w.DrawRect(r, color);
}

void DrawRect(Draw& w, int x, int y, int cx, int cy, const Image& img, bool ra) {
	w.Clip(x, y, cx, cy);
	Size sz = img.GetSize();
	for(int a = ra ? x : x / sz.cx * sz.cx; a < x + cx; a += sz.cx)
		for(int b = ra ? y : y / sz.cy * sz.cy ; b < y + cy; b += sz.cy)
			w.DrawImage(a, b, img);
	w.End();
}

void DrawRect(Draw& w, const Rect& rect, const Image& img, bool ralgn)
{
	DrawRect(w, rect.left, rect.top, rect.Width(), rect.Height(), img, ralgn);
}

void DrawTiles(Draw& w, int x, int y, int cx, int cy, const Image& img) {
	w.Clip(x, y, cx, cy);
	Size sz = img.GetSize();
	for(int a = x; a < x + cx; a += sz.cx)
		for(int b = y; b < y + cy; b += sz.cy)
			w.DrawImage(a, b, img);
	w.End();
}

void DrawTiles(Draw& w, const Rect& rect, const Image& img)
{
	DrawTiles(w, rect.left, rect.top, rect.GetWidth(), rect.GetHeight(), img);
}

void DrawHighlightImage(Draw& w, int x, int y, const Image& img, bool highlight,
                        bool enabled, Color maskcolor)
{
	if(highlight) {
		w.DrawImage(x + 1, y, img, maskcolor);
		w.DrawImage(x - 1, y, img, maskcolor);
		w.DrawImage(x, y + 1, img, maskcolor);
		w.DrawImage(x, y - 1, img, maskcolor);
	}
	w.DrawImage(x, y, enabled ? img : MakeImage(img, Etched));
}

Color GradientColor(Color fc, Color tc, int i, int n)
{
	return Color(
			fc.GetR() + i * (tc.GetR() - fc.GetR()) / n,
			fc.GetG() + i * (tc.GetG() - fc.GetG()) / n,
			fc.GetB() + i * (tc.GetB() - fc.GetB()) / n
	);
}

void PaintButtonRect(Draw& w, Rect& r,
                     Color left, Color top, Color right, Color bottom,
                     Color& topleft, Color& topright, Color& bottomleft, Color& bottomright)
{
	w.DrawRect(r.left, r.top, 1, 1, topleft);
	w.DrawRect(r.right - 1, r.top, 1, 1, topright);
	w.DrawRect(r.left, r.bottom - 1, 1, 1, bottomleft);
	w.DrawRect(r.right - 1, r.bottom - 1, 1, 1, bottomright);

	Color b1, b2;
	w.DrawRect(r.left + 1, r.top, 1, 1, b1 = Blend(topleft, top, 160));
	w.DrawRect(r.left, r.top + 1, 1, 1, b2 = Blend(topleft, left, 160));
	w.DrawRect(r.left + 2, r.top, 1, 1, Blend(b1, top));
	w.DrawRect(r.left, r.top + 2, 1, 1, Blend(b2, left));
	topleft = Blend(b1, b2);

	w.DrawRect(r.right - 2, r.top, 1, 1, b1 = Blend(topright, top, 160));
	w.DrawRect(r.right - 1, r.top + 1, 1, 1, b2 = Blend(topright, right, 160));
	w.DrawRect(r.right - 3, r.top, 1, 1, Blend(b1, top));
	w.DrawRect(r.right - 1, r.top + 2, 1, 1, Blend(b2, right));
	topright = Blend(b1, b2);

	w.DrawRect(r.left + 1, r.bottom - 1, 1, 1, b1 = Blend(bottomleft, bottom, 160));
	w.DrawRect(r.left, r.bottom - 2, 1, 1, b2 = Blend(bottomleft, left, 160));
	w.DrawRect(r.left + 2, r.bottom - 1, 1, 1, Blend(b1, bottom));
	w.DrawRect(r.left, r.bottom - 3, 1, 1, Blend(b2, left));
	bottomleft = Blend(b1, b2);

	w.DrawRect(r.right - 2, r.bottom - 1, 1, 1, b1 = Blend(bottomright, bottom, 160));
	w.DrawRect(r.right - 1, r.bottom - 2, 1, 1, b2 = Blend(bottomright, right, 160));
	w.DrawRect(r.right - 3, r.bottom - 1, 1, 1, Blend(b1, bottom));
	w.DrawRect(r.right - 1, r.bottom - 3, 1, 1, Blend(b2, right));
	bottomright = Blend(b1, b2);

	w.DrawRect(r.left + 3, r.top, r.Width() - 6, 1, top);
	w.DrawRect(r.left, r.top + 3, 1, r.Height() - 6, left);
	w.DrawRect(r.right - 1, r.top + 3, 1, r.Height() - 6, right);
	w.DrawRect(r.left + 3, r.bottom - 1, r.Width() - 6, 1, bottom);

	r.Deflate(1, 1);
}

void DrawXPButton(Draw& w, Rect r, int type)
{
	Color outlight = SColorLight;
	Color outshade = Blend(SColorShadow, SColorFace);
	Color light = SColorLight;
	Color shade = SColorShadow;
	Color frame = Blend(SColorHighlight, SColorText);
	Color mark = Null;
	Color topleft = SColorFace;
	int   markblend = 0;

	if(type & BUTTON_EDGE)
		frame = Blend(SColorHighlight, SColorLight);

	if(type & BUTTON_TOOL) {
		frame = SColorDisabled;
		light = Blend(SColorFace, light);
	}

	if(type & BUTTON_SCROLL) {
		outlight = SColorFace;
		shade = outshade = SColorFace;
		frame = Blend(SColorHighlight, SColorShadow);
	}

	switch(type & 15) {
	case BUTTON_OK:
		mark = Blend(Blue, SColorLight);
		markblend = 130;
		break;
	case BUTTON_HIGHLIGHT:
		if(!(type & BUTTON_SCROLL)) {
			mark = Blend(Yellow, LtRed, 100);
			markblend = 130;
		}
		break;
	case BUTTON_PUSH:
		light = shade = Blend(SColorHighlight, SColorFace, 235);
		break;
	case BUTTON_DISABLED:
		frame = SColorDisabled;
		outlight = outshade = light = shade = SColorFace;
		break;
	case BUTTON_CHECKED:
		if(type & BUTTON_TOOL)
			light = shade = SColorLight;
		else
			light = shade = Blend(SColorHighlight, SColorFace);
		break;
	}

	Color topright = topleft;
	Color bottomleft = topleft;
	Color bottomright = topleft;

	if(type & BUTTON_EDGE) {
		DrawFrame(w, r, frame);
		light = Blend(frame, SColorLight);
		shade = Blend(frame, SColorShadow);
		w.DrawRect(r.left, r.top, 1, 1, light);
		w.DrawRect(r.right - 1, r.top, 1, 1, light);
		w.DrawRect(r.left, r.bottom - 1, 1, 1, light);
		w.DrawRect(r.right - 1, r.bottom - 1, 1, 1, light);
		r.Deflate(1, 1);
		switch(type & 15) {
		case BUTTON_HIGHLIGHT:
			light = Blend(light, SColorLight);
			shade = Blend(shade, SColorLight);
			break;
		case BUTTON_PUSH:
			light = shade = Blend(SColorHighlight, SColorFace);
			break;
		}
	}
	else {
		if(!(type & BUTTON_TOOL))
			PaintButtonRect(w, r, outshade, outshade, outlight, outlight, topleft, topright, bottomleft, bottomright);
		PaintButtonRect(w, r, frame, frame, frame, frame, topleft, topright, bottomleft, bottomright);
		Color hc = Blend(light, mark, markblend);
		Color sc = Blend(shade, mark, markblend);
		PaintButtonRect(w, r, hc, hc, sc, sc, topleft, topright, bottomleft, bottomright);
		if(markblend) {
			DrawFrame(w, r, Blend(hc, mark, markblend), Blend(sc, mark, markblend));
			r.Deflate(1, 1);
		}
	}

	if(type & BUTTON_SCROLL)
		switch(type & 15) {
		case BUTTON_PUSH:
			light = shade = Blend(SColorFace, SColorHighlight);
			break;
		case BUTTON_HIGHLIGHT:
			light = shade = Blend(SColorLight, SColorHighlight, 40);
			break;
		default:
			light = Blend(SColorLight, SColorHighlight, 80);
			shade = Blend(SColorFace, SColorHighlight, 80);
		}

	Color b1 = Blend(light, shade, 80);
	Color bs = Blend(shade, SColorFace);
	if(type & BUTTON_VERTICAL) {
		int wd = r.Width();
		int w1 = 4 * wd / 5;
		for(int i = 0; i < wd; i++)
			w.DrawRect(r.left + i, r.top, 1, r.Height(), i < w1 ? Blend(light, b1, 255 * i / w1)
			                                                    : Blend(b1, bs, 255 * (i - w1) / (wd - w1)));
	}
	else {
		int h = r.Height();
		int h1 = 4 * h / 5;
		for(int i = 0; i < h; i++)
			w.DrawRect(r.left, r.top + i, r.Width(), 1, i < h1 ? Blend(light, b1, 255 * i / h1)
			                                                   : Blend(b1, bs, 255 * (i - h1) / (h - h1)));
	}
}

#ifdef PLATFORM_WIN32

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

void DrawDragRect(Draw& w, const Rect& _rect1, const Rect& _rect2, const Rect& _clip, int n, Color color, uint64 pattern)
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

void DrawDragRect(Draw& w, const Rect& rect1, const Rect& rect2, const Rect& clip, int n,
                  Color color, uint64 pattern)
{
	char bd[8];
	for(int i = 0; i < 8; i++)
		bd[i] = ~(byte)(pattern >> (8 * (7 - i)));
	Pixmap stipple = XCreateBitmapFromData(Xdisplay, w.GetDrawable(), bd, 8, 8);
	Point offset = w.GetOffset();
	GC gc = XCreateGC(Xdisplay, w.GetDrawable(), 0, 0);
#ifdef PLATFORM_XFT
	SetClip(gc, w.GetXftDraw(),
	        Intersect(Xor(GetFrameRgn(rect1 + offset, n), GetFrameRgn(rect2 + offset, n)),
	                  RectRgn(clip + offset)));
#else
	SetClip(gc, Intersect(Xor(GetFrameRgn(rect1 + offset, n), GetFrameRgn(rect2 + offset, n)),
	                      RectRgn(clip + offset)));
#endif
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

static DrawingToPdfFnType sPdf;

void SetDrawingToPdfFn(DrawingToPdfFnType Pdf)
{
	sPdf = Pdf;
}

DrawingToPdfFnType GetDrawingToPdfFn()
{
	return sPdf;
}

END_UPP_NAMESPACE
