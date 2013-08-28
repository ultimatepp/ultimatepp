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

NAMESPACE_UPP

#define LTIMING(x) //  RTIMING(x)

struct ChImageMaker : ImageMaker {
	Size       sz;
	Rect       sr;
	Image      img;

	virtual String Key() const;
	virtual Image  Make() const;
};

String ChImageMaker::Key() const
{
	LTIMING("ChImageMaker::Key");
	StringBuffer b;
	int64 id = img.GetSerialId();
	b.Cat((const char *)&id, sizeof(id));
	b.Cat((const char *)&sz.cx, sizeof(sz.cx));
	b.Cat((const char *)&sz.cy, sizeof(sz.cy));
	b.Cat((const char *)&sr.left, sizeof(sr.left));
	b.Cat((const char *)&sr.right, sizeof(sr.right));
	b.Cat((const char *)&sr.top, sizeof(sr.top));
	b.Cat((const char *)&sr.bottom, sizeof(sr.bottom));
	return b;
}

Image ChImageMaker::Make() const
{
	LTIMING("ChImageMaker::Make");
	return Rescale(img, sz, sr);
}

void  ChDraw(Draw& w, int x, int y, int cx, int cy, const Image& img, const Rect& src)
{
	LTIMING("ChDraw");
	if(cx > 0 && cy > 0) {
		#if defined(flagWINGL) || defined(flagLINUXGL)
		w.DrawImage(x, y, cx, cy, img, src);
		#else
		ChImageMaker m;
		m.sz = Size(cx, cy);
		m.sr = src;
		m.img = img;
		w.DrawImage(x, y, MakeImage(m));
		#endif
	}
}

void  ChDraw(Draw& w, const Rect& r, const Image& img, const Rect& src)
{
	ChDraw(w, r.left, r.top, r.GetWidth(), r.GetHeight(), img, src);
}

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

Value sChOp(Draw& w, const Rect& r, const Value& v, int op);

struct sChBorder {
	const ColorF *border;
	Value face;
};

Value ChBorder(const ColorF *colors, const Value& face)
{
	sChBorder b;
	b.border = colors;
	b.face = face;
	return RawToValue(b);
}

Value StdChLookFn(Draw& w, const Rect& r, const Value& v, int op)
{
	if(IsType<sChLookWith>(v)) {
		const sChLookWith& x = ValueTo<sChLookWith>(v);
		if(op == LOOK_PAINT) {
			LOGPNG(AsString(x.img.GetSerialId()), x.img);
			ChPaint(w, r, x.look);
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
		return sChOp(w, r, x.look, op);
	}
	if(IsType<sChBorder>(v)) {
		sChBorder b = ValueTo<sChBorder>(v);
		int n = (int)(intptr_t)*b.border;
		switch(op) {
		case LOOK_PAINT:
			ChPaint(w, r.Deflated(n), b.face);
		case LOOK_PAINTEDGE:
			DrawBorder(w, r, b.border);
			return 0;
		case LOOK_MARGINS:
			return Rect(n, n, n, n);
		case LOOK_ISBODYOPAQUE:
		case LOOK_ISOPAQUE:
			return false;
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
		case LOOK_ISBODYOPAQUE:
		case LOOK_ISOPAQUE:
			return !IsNull(c);
		}
	}
	if(IsType<Image>(v)) {
		Image img = v;
		Size isz = img.GetSize();
		Size sz = r.GetSize();
		Point p = img.GetHotSpot();
		Point p2 = img.Get2ndSpot();
		int tile = 0;
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
		if(op == LOOK_ISOPAQUE)
			return img.GetKind() == IMAGE_OPAQUE;
		if(IsNull(img))
			return 1;
		if(op == LOOK_ISBODYOPAQUE) {
			static VectorMap<int64, bool> btc;
			int64 key = img.GetSerialId();
			int q = btc.Find(key);
			if(q < 0) {
				bool opaque = true;
				Rect m = ChMargins(img);
				Rect r = img.GetSize();
				r.left += max(m.left, 0);
				r.right -= max(m.right, 0);
				r.top += m.top;
				r.bottom -= m.bottom;
				int cx = r.right - r.left;
				if(cx >= 0)
					for(int y = r.top; y < r.bottom && opaque; y++) {
						const RGBA *s = img[y] + r.left;
						const RGBA *e = s + cx;
						while(s < e) {
							if(s->a != 255) {
								opaque = false;
								break;
							}
						}
					}
				if(btc.GetCount() > 100)
					btc.Clear();
				btc.Add(key, q);
				return opaque;
			}
			return btc[q];
		}
		if(op == LOOK_PAINT || op == LOOK_PAINTEDGE) {
			LTIMING("ChPaint Image");
			#if defined(flagWINGL) || defined(flagLINUXGL)
			w.Offset(r.TopLeft());
			#else
			w.Clipoff(r);
			#endif
			Rect sr(p, p2);
			Size sz2(isz.cx - sr.right, isz.cy - sr.bottom);
			Rect r = RectC(p.x, p.y, sz.cx - sr.left - sz2.cx, sz.cy - sr.top - sz2.cy);
			int top = minmax(sz.cy / 2, 0, isz.cy);
			int bottom = minmax(sz.cy - top, 0, isz.cy);
			int yy = isz.cy - bottom;
			int left = minmax(sz.cx / 2, 0, isz.cx);
			int right = minmax(sz.cx - left, 0, isz.cx);
			int xx = isz.cx - right;
			if(!r.IsEmpty()) {
				w.DrawImage(0, 0, img, RectC(0, 0, p.x, p.y));
				w.DrawImage(0, r.bottom, img, RectC(0, sr.bottom, p.x, sz2.cy));
				w.DrawImage(r.right, 0, img, RectC(sr.right, 0, sz2.cx, p.y));
				w.DrawImage(r.right, r.bottom, img, RectC(sr.right, sr.bottom, sz2.cx, sz2.cy));
				ChDraw(w, p.x, 0, r.Width(), p.y, img, RectC(p.x, 0, sr.Width(), p.y));
				ChDraw(w, p.x, r.bottom, r.Width(), sz2.cy, img, RectC(p.x, sr.bottom, sr.Width(), sz2.cy));
				ChDraw(w, 0, p.y, p.x, r.Height(), img, RectC(0, p.y, p.x, sr.Height()));
				ChDraw(w, r.right, p.y, sz2.cx, r.Height(), img,
				       RectC(sr.right, p.y, sz2.cx, sr.Height()));
				if(op == LOOK_PAINT) {
					if(IsNull(r) || IsNull(sr)) {
						w.End();
						return 1;
					}
					if(tile) {
						LTIMING("Ch-Tiles");
						LTIMING("Ch-Tiles");
						Size sz;
						sz.cx = (tile & 1 ? sr : r).GetWidth();
						sz.cy = (tile & 2 ? sr : r).GetHeight();
						#if defined(flagWINGL) || defined(flagLINUXGL)
						DrawTiles(w, r, img, sz, sr);
						#else
						img = Rescale(img, sz, sr);
						DrawTiles(w, r, img);
						#endif
					}
					else {
						static VectorMap<int64, int> btc;
						int64 key = img.GetSerialId();
						int q;
						{
							LTIMING("Find");
							q = btc.Find(key);
						}
						if(q < 0) {
							LTIMING("ClassifyContent");
							q = ClassifyContent(img, sr);
							if(btc.GetCount() > 100)
								btc.Clear();
							btc.Add(key, q);
						}
						else
							q = btc[q];
						switch(q) {
						case IMAGECONTENT_VERTDUP|IMAGECONTENT_HORZDUP:
							{
								LTIMING("Ch-singlecolor");
								RGBA c = img[sr.top][sr.left];
								if(c.a == 255) {
									w.DrawRect(r, c);
									break;
								}
							}
						case 0:
						default:
							ChDraw(w, r, img, sr);
							break;
							LTIMING("Ch-linedup");
							img = CachedRescalePaintOnly(
									img,
									Size((q & IMAGECONTENT_HORZDUP) ? max(min(40, r.GetWidth()), sr.GetWidth())
									                                : r.GetWidth(),
									     (q & IMAGECONTENT_VERTDUP) ? max(min(40, r.GetHeight()), sr.GetHeight())
							                                        : r.GetHeight()),
									sr
							);
							LTIMING("Ch-linedup-drawtiles");
							DrawTiles(w, r, img);
							break;
						}
					}
				}
			}
			else
			if(r.left < r.right) {
				w.DrawImage(0, 0, img, RectC(0, 0, p.x, top));
				w.DrawImage(0, sz.cy - bottom, img, RectC(0, yy, p.x, bottom));
				w.DrawImage(r.right, 0, img, RectC(sr.right, 0, sz2.cx, top));
				w.DrawImage(r.right, sz.cy - bottom, img, RectC(sr.right, yy, sz2.cx, bottom));
				ChDraw(w, p.x, 0, r.Width(), top, img, RectC(p.x, 0, sr.Width(), top));
				ChDraw(w, p.x, sz.cy - bottom, r.Width(), bottom, img, RectC(p.x, yy, sr.Width(), bottom));
			}
			else
			if(r.top < r.bottom) {
				w.DrawImage(0, 0, img, RectC(0, 0, left, p.y));
				w.DrawImage(0, r.bottom, img, RectC(0, sr.bottom, left, sz2.cy));
				w.DrawImage(sz.cx - right, 0, img, RectC(xx, 0, right, p.y));
				w.DrawImage(sz.cx - right, r.bottom, img, RectC(xx, sr.bottom, right, sz2.cy));
				ChDraw(w, 0, p.y, left, r.Height(), img, RectC(0, p.y, left, sr.Height()));
				ChDraw(w, sz.cx - right, p.y, right, r.Height(), img, RectC(xx, p.y, right, sr.Height()));
			}
			else {
				w.DrawImage(0, 0, img, RectC(0, 0, left, top));
				w.DrawImage(0, sz.cy - bottom, img, RectC(0, yy, left, top));
				w.DrawImage(sz.cx - right, 0, img, RectC(xx, 0, right, top));
				w.DrawImage(sz.cx - right, sz.cy - bottom, img, RectC(xx, yy, right, bottom));
			}
			w.End();
			return 1;
		}
	}
	return Null;
}

typedef Value (*ChPainterFn)(Draw& w, const Rect& r, const Value& v, int op);

Vector<ChPainterFn>& sChps()
{
	static Vector<ChPainterFn> x;
	return x;
}

void ChLookFn(Value (*fn)(Draw& w, const Rect& r, const Value& v, int op))
{
	if(FindIndex(sChps(), fn) < 0)
		sChps().Add(fn);
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

void ChReset()
{
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
}

Value sChOp(Draw& w, const Rect& r, const Value& v, int op)
{
	Value q;
	if(!IsNull(v))
		for(int i = sChps().GetCount() - 1; i >= 0; i--) {
			q = (*sChps()[i])(w, r, v, op);
			if(!IsNull(q))
				break;
		}
	return q;
}

void ChPaint(Draw& w, const Rect& r, const Value& look)
{
	sChOp(w, r, look, LOOK_PAINT);
}

void ChPaint(Draw& w, int x, int y, int cx, int cy, const Value& look)
{
	sChOp(w, RectC(x, y, cx, cy), look, LOOK_PAINT);
}

void ChPaintEdge(Draw& w, const Rect& r, const Value& look)
{
	sChOp(w, r, look, LOOK_PAINTEDGE);
}

void ChPaintEdge(Draw& w, int x, int y, int cx, int cy, const Value& look)
{
	sChOp(w, RectC(x, y, cx, cy), look, LOOK_PAINTEDGE);
}

void ChPaintBody(Draw& w, const Rect& r, const Value& look)
{
	Rect m = ChMargins(look);
	w.Clip(r);
	ChPaint(w, Rect(r.left - m.left, r.top - m.top, r.right + m.right, r.bottom + m.bottom),
	        look);
	w.End();
}

void ChPaintBody(Draw& w, int x, int y, int cx, int cy, const Value& look)
{
	ChPaintBody(w, RectC(x, y, cx, cy), look);
}

Rect ChMargins(const Value& look)
{
	NilDraw w;
	return sChOp(w, Null, look, LOOK_MARGINS);
}

bool ChIsOpaque(const Value& look)
{
	NilDraw w;
	return sChOp(w, Null, look, LOOK_ISOPAQUE);
}

bool ChIsBodyOpaque(const Value& look)
{
	NilDraw w;
	return sChOp(w, Null, look, LOOK_ISBODYOPAQUE);
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
	return w;
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

END_UPP_NAMESPACE
