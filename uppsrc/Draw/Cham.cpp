#include "Draw.h"

#if 0

_DBG_
#include <plugin/png/png.h>

namespace Upp {
inline void LOGPNG(const char *name, const Image& m)
{
	PNGEncoder().SaveFile(GetHomeDirFile(name) + ".png", m);
}
};

#else

#define LOGPNG(a, b)

#endif

namespace Upp {

#define LTIMING(x) //  RTIMING(x)

struct sChLookWith {
	Value look;
	Image img;
	Color (*colorfn)(int i);
	int   ii;
	Color color;
	Point offset;
};

Value ChLookWith(const Value& look, const Image& img, Point offset)
{
	sChLookWith x;
	x.look = look;
	x.img = img;
	x.colorfn = NULL;
	x.color = Null;
	x.offset = offset;
	return RawToValue(x);
}

Value ChLookWith(const Value& look, const Image& img, Color color, Point offset)
{
	sChLookWith x;
	x.look = look;
	x.img = img;
	x.colorfn = NULL;
	x.color = color;
	x.offset = offset;
	return RawToValue(x);
}

Value ChLookWith(const Value& look, const Image& img, Color (*color)(int i), int i, Point offset)
{
	sChLookWith x;
	x.look = look;
	x.img = img;
	x.colorfn = color;
	x.ii = i;
	x.offset = offset;
	return RawToValue(x);
}

void ChLookWith(Value *look, const Image& image, const Color *color, int n)
{
	for(int i = 0; i < n; i++)
		look[i] = ChLookWith(look[i], image, color[i]);
}

struct sChBorder {
	const ColorF *border;
	Value face;
};

Value ChBorder(const ColorF *colors, const Value& face)
{ // adds border to face value
	sChBorder b;
	b.border = colors;
	b.face = face;
	return RawToValue(b);
}

static void sDrawScrollbarThumb(Draw& w, int x, int y, int cx, int cy, const Image& m, bool vert)
{
	if(cx <= 0)
		return;
	Size isz = m.GetSize();
	if(vert) {
		Swap(isz.cx, isz.cy);
		Swap(cx, cy);
	}
	int ecx = isz.cy / 2; // size of 'ending' (can be circular)
	int ccx = isz.cy / 2; // size of central part, there is sometimes some sort of 'handle'
	int sl = (isz.cx - 2 * ecx - ccx) / 2; // size of left stretched part source
	int sr = isz.cx - 2 * ecx - ccx - sl; // size of right stretched part source

	int tel = ecx; // left ending target size
	int l = (cx - 2 * ecx - ccx) / 2; // size of left stretched part
	int tccx = ccx; // target center size
	int r = cx - 2 * ecx - ccx - l; // size of right stretched part
	int ter = ecx; // size of right stretched part

	if(l < 0 || r < 0) { // if not enough space, remove left stretch and center
		l = 0;
		tccx = 0;
		r = cx - 2 * ecx - tccx - l;
	}
	
	if(r < 0) { // still not enough space, need to resize endings too
		r = 0;
		tel = cx / 2;
		ter = cx = tel;
	}

	int sx = 0;
	auto DrawPart = [&](int tcx, int scx) {
		if(scx) {
			if(vert) {
				w.DrawImage(x, y, CachedRescale(m, Size(cy, tcx), RectC(0, sx, isz.cy, scx)));
				y += tcx;
			}
			else {
				w.DrawImage(x, y, CachedRescale(m, Size(tcx, cy), RectC(sx, 0, scx, isz.cy)));
				x += tcx;
			}
		}
		sx += scx;
	};
	DrawPart(tel, ecx); // left ending
	DrawPart(l, sl); // left stretch
	DrawPart(tccx, ccx); // central part
	DrawPart(r, sr); // right stretch
	DrawPart(ter, ecx); // right ending
}

Value ChLookFnImage(Draw& w, const Rect& r, const Image& img, int op, Color ink, Point p, Point p2)
{
	bool nocache = op & LOOK_NOCACHE;
	op = op & ~LOOK_NOCACHE;

	auto ChDraw = [&](Draw& w, int x, int y, int cx, int cy, const Image& img, const Rect& src, Color ink)
	{
		LTIMING("ChDraw");
		if(cx > 0 && cy > 0) {
			Image m = nocache ? Rescale(img, Size(cx, cy), src) : CachedRescale(img, Size(cx, cy), src);
			if(IsNull(ink))
				w.DrawImage(x, y, m);
			else
				w.DrawImage(x, y, m, ink);
		}
	};
	
	auto ChDrawA = [&](Draw& w, int x, int y, const Image& img, const Rect& src, Color ink)
	{
		ChDraw(w, x, y, src.GetWidth(), src.GetHeight(), img, src, ink);
	};
	
	auto ChDrawR = [&](Draw& w, const Rect& r, const Image& img, const Rect& src, Color ink)
	{
		ChDraw(w, r.left, r.top, r.GetWidth(), r.GetHeight(), img, src, ink);
	};

	Size isz = img.GetSize();
	Size sz = r.GetSize();
	if(p.x == CH_SCROLLBAR_IMAGE) {
		if(op == LOOK_MARGINS)
			return Rect(0, 0, 0, 0);
		if(op == LOOK_PAINT)
			sDrawScrollbarThumb(w, r.left, r.top, r.GetWidth(), r.GetHeight(), img, isz.cx < isz.cy);
		return true;
	}
	int tile = 0;
	if(p2.x == CH_EDITFIELD_IMAGE) {
		if(op != LOOK_MARGINS)
			p.x = p.y = p2.y;
		p2.x = p2.y = 0;
	}
	if(p2.x || p2.y) {
		if(p2.x < p.x) {
			Swap(p2.x, p.x);
			tile = 1;
		}
		if(p2.y < p.y) {
			Swap(p2.y, p.y);
			tile += 2;
		}
		p2.x++;
		p2.y++;
	}
	else {
		p2.x = isz.cx - p.x;
		p2.y = isz.cy - p.y;
		if(p.x > isz.cx / 2) {
			tile = 1;
			p2.x = p.x;
			p.x = isz.cx - p.x - 1;
		}
		if(p.y > isz.cy / 2) {
			tile += 2;
			p2.y = p.y;
			p.y = isz.cy - p.y - 1;
		}
	}
	if(op == LOOK_MARGINS)
		return Rect(p.x, p.y, isz.cx - p2.x, isz.cy - p2.y);
	if(IsNull(img))
		return 1;
	LTIMING("ChPaint Image");
	w.Clipoff(r);
	Rect sr(p, p2);
	Size sz2(isz.cx - sr.right, isz.cy - sr.bottom);
	Rect rr = RectC(p.x, p.y, sz.cx - sr.left - sz2.cx, sz.cy - sr.top - sz2.cy);
	int top = minmax(sz.cy / 2, 0, isz.cy);
	int bottom = minmax(sz.cy - top, 0, isz.cy);
	int yy = isz.cy - bottom;
	int left = minmax(sz.cx / 2, 0, isz.cx);
	int right = minmax(sz.cx - left, 0, isz.cx);
	int xx = isz.cx - right;
	if(!rr.IsEmpty()) {
		ChDrawA(w, 0, 0, img, RectC(0, 0, p.x, p.y), ink);
		ChDrawA(w, 0, rr.bottom, img, RectC(0, sr.bottom, p.x, sz2.cy), ink);
		ChDrawA(w, rr.right, 0, img, RectC(sr.right, 0, sz2.cx, p.y), ink);
		ChDrawA(w, rr.right, rr.bottom, img, RectC(sr.right, sr.bottom, sz2.cx, sz2.cy), ink);
		ChDraw(w, p.x, 0, rr.Width(), p.y, img, RectC(p.x, 0, sr.Width(), p.y), ink);
		ChDraw(w, p.x, rr.bottom, rr.Width(), sz2.cy, img, RectC(p.x, sr.bottom, sr.Width(), sz2.cy), ink);
		ChDraw(w, 0, p.y, p.x, rr.Height(), img, RectC(0, p.y, p.x, sr.Height()), ink);
		ChDraw(w, rr.right, p.y, sz2.cx, rr.Height(), img, RectC(sr.right, p.y, sz2.cx, sr.Height()), ink);
		if(op == LOOK_PAINT) {
			if(IsNull(rr) || IsNull(sr)) {
				w.End();
				return 1;
			}
			if(tile) {
				LTIMING("Ch-Tiles");
				LTIMING("Ch-Tiles");
				Size sz;
				sz.cx = (tile & 1 ? sr : rr).GetWidth();
				sz.cy = (tile & 2 ? sr : rr).GetHeight();
				DrawTiles(w, rr, Rescale(img, sz, sr));
			}
			else {
				static VectorMap<int64, bool> single_color_body_cache;
				int64 key = img.GetSerialId();
				bool single_color_body;
				int q = single_color_body_cache.Find(key);
				if(q < 0) {
					LTIMING("ClassifyContent");
					single_color_body = IsSingleColor(img, sr);
					if(single_color_body_cache.GetCount() > 1000)
						single_color_body_cache.Clear();
					single_color_body_cache.Add(key, single_color_body);
				}
				else
					single_color_body = single_color_body_cache[q];
				RGBA c = img[sr.top][sr.left];
				if(single_color_body && c.a == 255) {
					LTIMING("Ch-singlecolor");
					w.DrawRect(rr, c);
				}
				else
					ChDrawR(w, rr, img, sr, ink);
			}
		}
	}
	else
	if(rr.left < rr.right) {
		ChDrawA(w, 0, 0, img, RectC(0, 0, p.x, top), ink);
		ChDrawA(w, 0, sz.cy - bottom, img, RectC(0, yy, p.x, bottom), ink);
		ChDrawA(w, rr.right, 0, img, RectC(sr.right, 0, sz2.cx, top), ink);
		ChDrawA(w, rr.right, sz.cy - bottom, img, RectC(sr.right, yy, sz2.cx, bottom), ink);
		ChDraw(w, p.x, 0, rr.Width(), top, img, RectC(p.x, 0, sr.Width(), top), ink);
		ChDraw(w, p.x, sz.cy - bottom, rr.Width(), bottom, img, RectC(p.x, yy, sr.Width(), bottom), ink);
	}
	else
	if(rr.top < rr.bottom) {
		ChDrawA(w, 0, 0, img, RectC(0, 0, left, p.y), ink);
		ChDrawA(w, 0, rr.bottom, img, RectC(0, sr.bottom, left, sz2.cy), ink);
		ChDrawA(w, sz.cx - right, 0, img, RectC(xx, 0, right, p.y), ink);
		ChDrawA(w, sz.cx - right, rr.bottom, img, RectC(xx, sr.bottom, right, sz2.cy), ink);
		ChDraw(w, 0, p.y, left, rr.Height(), img, RectC(0, p.y, left, sr.Height()), ink);
		ChDraw(w, sz.cx - right, p.y, right, rr.Height(), img, RectC(xx, p.y, right, sr.Height()), ink);
	}
	else {
		ChDrawA(w, 0, 0, img, RectC(0, 0, left, top), ink);
		ChDrawA(w, 0, sz.cy - bottom, img, RectC(0, yy, left, top), ink);
		ChDrawA(w, sz.cx - right, 0, img, RectC(xx, 0, right, top), ink);
		ChDrawA(w, sz.cx - right, sz.cy - bottom, img, RectC(xx, yy, right, bottom), ink);
	}
	w.End();
	return 1;
}

Value ChLookFnImage(Draw& w, const Rect& r, const Image& img, int op, Color ink)
{
	return ChLookFnImage(w, r, img, op, ink, img.GetHotSpot(), img.Get2ndSpot());
}

struct ChPainterFn__ {
	Value (*op)(Draw& w, const Rect& r, const Value& v, int op, Color ink) = nullptr;
	Value (*op2)(Ctrl *ctrl, Draw& w, const Rect& r, const Value& v, int op, Color ink) = nullptr;
};

static Vector<ChPainterFn__>  sChPainterFns;

static Value sChOp(Ctrl *ctrl, Draw& w, const Rect& r, const Value& v, int op, Color ink = Null)
{
	if(r.right < r.left || r.bottom < r.top)
		return Rect(0, 0, 0, 0);
	Value q;
	if(!IsNull(v))
		for(auto& a : ReverseRange(sChPainterFns)) {
			q = a.op ? (*a.op)(w, r, v, op, ink) : (*a.op2)(ctrl, w, r, v, op, ink);
			if(!IsNull(q))
				break;
		}
	return q;
}

void ChLookFn(Value (*fn)(Draw& w, const Rect& r, const Value& v, int op, Color ink))
{
	if(FindMatch(sChPainterFns, [&](const ChPainterFn__& m) { return m.op == fn; }) < 0)
		sChPainterFns.Add().op = fn;
}

void ChLookFn(Value (*fn)(Ctrl *ctrl, Draw& w, const Rect& r, const Value& v, int op, Color ink))
{
	if(FindMatch(sChPainterFns, [&](const ChPainterFn__& m) { return m.op2 == fn; }) < 0)
		sChPainterFns.Add().op2 = fn;
}


Value StdChLookFn(Ctrl *ctrl, Draw& w, const Rect& r, const Value& v, int op, Color ink)
{
	if(IsType<sChLookWith>(v)) {
		const sChLookWith& x = ValueTo<sChLookWith>(v);
		if(op == LOOK_PAINT) {
			LOGPNG(AsString(x.img.GetSerialId()), x.img);
			ChPaint(ctrl, w, r, x.look);
			Point p = r.CenterPos(x.img.GetSize()) + x.offset;
			if(x.colorfn)
				w.DrawImage(p.x, p.y, x.img, (*x.colorfn)(x.ii));
			else
			if(!IsNull(x.color))
				w.DrawImage(p.x, p.y, x.img, x.color);
			else
				w.DrawImage(p.x, p.y, x.img);
			return 1;
		}
		return sChOp(ctrl, w, r, x.look, op);
	}

	if(IsType<sChBorder>(v)) {
		sChBorder b = ValueTo<sChBorder>(v);
		int n = (int)(intptr_t)*b.border;
		switch(op) {
		case LOOK_PAINT:
			ChPaint(ctrl, w, r.Deflated(n), b.face);
			// fall through - need to paint border now
		case LOOK_PAINTEDGE:
			DrawBorder(w, r, b.border);
			return 0;
		case LOOK_MARGINS:
			return Rect(n, n, n, n);
		}
	}

	if(IsType<Color>(v)) {
		Color c = v;
		switch(op) {
		case LOOK_PAINT:
			w.DrawRect(r, c);
			return 0;
		case LOOK_PAINTEDGE:
			DrawFrame(w, r, c);
			return 0;
		case LOOK_MARGINS:
			return Rect(1, 1, 1, 1);
		}
	}

	if(IsType<Image>(v))
		return ChLookFnImage(w, r, v, op, ink);

	return Null;
}

struct sStyleCh : Moveable<sStyleCh> {
	byte *status;
	void (*init)();
};

Vector<sStyleCh>& sChStyle()
{
	static Vector<sStyleCh> x;
	return x;
}

void ChRegisterStyle__(byte& state, byte& registered, void (*init)())
{
	if(!registered) {
		sStyleCh& s = sChStyle().Add();
		s.status = &state;
		s.init = init;
		registered = true;
	}
}

static bool sChInvalid = true;

void ChInvalidate()
{
	sChInvalid = true;
}

bool ChIsInvalidated()
{
	return sChInvalid;
}

static bool sLabelTextColorMismatch;

bool IsLabelTextColorMismatch()
{
	return sLabelTextColorMismatch;
}

static bool sIsDarkColorFace;

bool IsDarkColorFace()
{
	return sIsDarkColorFace;
}

extern bool AColor_dark_mode__;

void ChReset()
{
	AColor_dark_mode__ = dark_theme__ = false;
	for(int i = 0; i < sChStyle().GetCount(); i++)
		*sChStyle()[i].status = 0;
	ChLookFn(StdChLookFn);
}

void ChFinish()
{
	sChInvalid = false;
	for(int i = 0; i < sChStyle().GetCount(); i++)
		sChStyle()[i].init();
	sIsDarkColorFace = IsDark(SColorFace());
	sLabelTextColorMismatch = IsDark(SColorText()) != IsDark(SColorLabel());
	for(int i = 0; i < 5; i++) // repeat to propagate changes
		SColor::Refresh();
}

void ChPaint(Ctrl *ctrl, Draw& w,const Rect& r, const Value& look, Color ink)
{
	sChOp(ctrl, w, r, look, LOOK_PAINT, ink);
}

void ChPaint(Ctrl *ctrl, Draw& w,int x, int y, int cx, int cy, const Value& look, Color ink)
{
	sChOp(ctrl, w, RectC(x, y, cx, cy), look, LOOK_PAINT, ink);
}

void ChPaintNoCache(Ctrl *ctrl, Draw& w,int x, int y, int cx, int cy, const Value& look, Color ink)
{
	sChOp(ctrl, w, RectC(x, y, cx, cy), look, LOOK_PAINT|LOOK_NOCACHE, ink);
}

void ChPaintEdge(Ctrl *ctrl, Draw& w,const Rect& r, const Value& look, Color ink)
{
	sChOp(ctrl, w, r, look, LOOK_PAINTEDGE, ink);
}

void ChPaintEdge(Ctrl *ctrl, Draw& w,int x, int y, int cx, int cy, const Value& look, Color ink)
{
	sChOp(ctrl, w, RectC(x, y, cx, cy), look, LOOK_PAINTEDGE, ink);
}

void ChPaintBody(Ctrl *ctrl, Draw& w, const Rect& r, const Value& look, Color ink)
{
	Rect m = ChMargins(ctrl, look);
	w.Clip(r);
	ChPaint(ctrl, w, Rect(r.left - m.left, r.top - m.top, r.right + m.right, r.bottom + m.bottom),
	        look, ink);
	w.End();
}

void ChPaintBody(Ctrl *ctrl, Draw& w, int x, int y, int cx, int cy, const Value& look, Color ink)
{
	ChPaintBody(ctrl, w, RectC(x, y, cx, cy), look, ink);
}

Rect ChMargins(Ctrl *ctrl, const Value& look)
{
	NilDraw w;
	return sChOp(ctrl, w, Null, look, LOOK_MARGINS);
}

void ChPaint(Draw& w, const Rect& r, const Value& look, Color ink)
{
	sChOp(nullptr, w, r, look, LOOK_PAINT, ink);
}

void ChPaint(Draw& w, int x, int y, int cx, int cy, const Value& look, Color ink)
{
	sChOp(nullptr, w, RectC(x, y, cx, cy), look, LOOK_PAINT, ink);
}

void ChPaintNoCache(Draw& w, int x, int y, int cx, int cy, const Value& look, Color ink)
{
	sChOp(nullptr, w, RectC(x, y, cx, cy), look, LOOK_PAINT|LOOK_NOCACHE, ink);
}

void ChPaintEdge(Draw& w, const Rect& r, const Value& look, Color ink)
{
	sChOp(nullptr, w, r, look, LOOK_PAINTEDGE, ink);
}

void ChPaintEdge(Draw& w, int x, int y, int cx, int cy, const Value& look, Color ink)
{
	sChOp(nullptr, w, RectC(x, y, cx, cy), look, LOOK_PAINTEDGE, ink);
}

void ChPaintBody(Draw& w, const Rect& r, const Value& look, Color ink)
{
	Rect m = ChMargins(look);
	w.Clip(r);
	ChPaint(w, Rect(r.left - m.left, r.top - m.top, r.right + m.right, r.bottom + m.bottom),
	        look, ink);
	w.End();
}

void ChPaintBody(Draw& w, int x, int y, int cx, int cy, const Value& look, Color ink)
{
	ChPaintBody(w, RectC(x, y, cx, cy), look, ink);
}

Rect ChMargins(const Value& look)
{
	NilDraw w;
	return sChOp(nullptr, w, Null, look, LOOK_MARGINS);
}

void   DeflateMargins(Rect& r, const Rect& m)
{
	r = Rect(r.left + m.left, r.top + m.top, r.right - m.right, r.bottom - m.bottom);
}

void   ChDeflateMargins(Rect& r, const Value& look)
{
	return DeflateMargins(r, ChMargins(look));
}

void DeflateMargins(Size& sz, const Rect& m)
{
	sz = Size(sz.cx + m.left + m.right, sz.cy + m.top + m.bottom);
}

void ChDeflateMargins(Size& sz, const Value& look)
{
	DeflateMargins(sz, ChMargins(look));
}

void   InflateMargins(Rect& r, const Rect& m)
{
	r = Rect(r.left - m.left, r.top - m.top, r.right + m.right, r.bottom + m.bottom);
}

void   ChInflateMargins(Rect& r, const Value& look)
{
	return InflateMargins(r, ChMargins(look));
}

void InflateMargins(Size& sz, const Rect& m)
{
	sz = Size(sz.cx + m.left + m.right, sz.cy + m.top + m.bottom);
}

void ChInflateMargins(Size& sz, const Value& look)
{
	InflateMargins(sz, ChMargins(look));
}

Image AdjustColors(const Image& simg) {
	Image h = simg;
	ImageBuffer img(h);
	Unmultiply(img);
	const RGBA *s = ~img;
	const RGBA *e = s + img.GetLength();
	ImageBuffer w(img.GetSize());
	RGBA *t = w;
	Color black = SColorText();
	Color shadow = SColorShadow();
	Color face = SColorFace();
	Color light = SColorLight();
	Color highlight = SColorHighlight();
	Color hot = highlight;
	Color paper = SColorPaper();
	while(s < e) {
		*t = *s;
		if(s->r == s->g && s->g == s->b)
			if(s->r < 128)
				*t = Blend(black, shadow, 2 * s->r);
			else
			if(s->r >= 128 && s->r < 192)
				*t = Blend(shadow, face, 4 * (s->r - 128));
			else
				*t = Blend(face, light, 4 * (s->r - 192));
		else
		if(s->r == 0 && s->g == 0)
			*t = Blend(black, highlight, 2 * (s->b - 128));
		else
		if(s->b == 0 && s->g == 0)
			*t = Blend(black, hot, 2 * (s->r - 128));
		else
		if(s->r == s->g && s->b == 0)
			*t = Blend(black, paper, s->r);
		t->a = s->a;
		t++;
		s++;
	}
	w.SetHotSpot(img.GetHotSpot());
	w.Set2ndSpot(img.Get2ndSpot());
	Premultiply(w);
	return Image(w);
}

void  Override(Iml& target, Iml& source, bool colored)
{
	for(int i = 0; i < target.GetCount(); i++) {
		int q = source.Find(target.GetId(i));
		if(q >= 0) {
			Image m = source.Get(q);
			if(colored)
				m = AdjustColors(m);
			target.Set(i, m);
		}
	}
}

void ColoredOverride(Iml& target, Iml& source)
{
	Override(target, source, true);
}

}
