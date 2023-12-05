#include "Painter.h"

namespace Upp {

template <class T>
void sGet(T& r, StringStream& ss)
{
	ss.Get(&r, sizeof(T));
}


template <class T>
T sGet(StringStream& ss)
{
	T r;
	ss.Get(&r, sizeof(T));
	return r;
}

void Painter::Paint(const Painting& pic)
{
	StringStream ss(pic.cmd);
	Pointf p, p1, p2;
	RGBA  c, c1;
	Value  v;
	int    f, ch, n, hasdx;
	Xform2D m;
	double r, w;
	Font   fnt;
	int ii = 0;
	bool   large, sweep;
	for(;;) {
		int cmd = ss.Get();
		if(cmd < 0)
			return;
		bool rel = cmd & 1;
		switch(cmd) {
		case PAINTING_CLEAR:
			ClearOp(sGet<RGBA>(ss));
			break;
		case PAINTING_MOVE:
		case PAINTING_MOVE_REL:
			sGet(p, ss);
			MoveOp(p, rel);
			break;
		case PAINTING_LINE:
		case PAINTING_LINE_REL:
			sGet(p, ss);
			LineOp(p, rel);
			break;
		case PAINTING_QUADRATIC:
		case PAINTING_QUADRATIC_REL:
			sGet(p1, ss);
			sGet(p, ss);
			QuadraticOp(p1, p, rel);
			break;
		case PAINTING_QUADRATIC_S:
		case PAINTING_QUADRATIC_S_REL:
			sGet(p, ss);
			QuadraticOp(p, rel);
			break;
		case PAINTING_CUBIC:
		case PAINTING_CUBIC_REL:
			sGet(p1, ss);
			sGet(p2, ss);
			sGet(p, ss);
			CubicOp(p1, p2, p, rel);
			break;
		case PAINTING_CUBIC_S:
		case PAINTING_CUBIC_S_REL:
			sGet(p2, ss);
			sGet(p, ss);
			CubicOp(p2, p, rel);
			break;
		case PAINTING_ARC:
		case PAINTING_ARC_REL:
			sGet(p, ss);
			sGet(p1, ss);
			sGet(r, ss);
			sGet(w, ss);
			ArcOp(p, p1, r, w, rel);
			break;
		case PAINTING_SVGARC:
		case PAINTING_SVGARC_REL:			
			sGet(p1, ss);
			sGet(r, ss);
			sGet(large, ss);
			sGet(sweep, ss);
			sGet(p, ss);
			SvgArcOp(p1, r, large, sweep, p, rel);
			break;
		case PAINTING_CLOSE:
			CloseOp();
			break;
		case PAINTING_DIV:
			DivOp();
			break;
		case PAINTING_FILL_SOLID:
			FillOp(sGet<RGBA>(ss));
			break;
		case PAINTING_FILL_IMAGE:
			sGet(m, ss);
			f = ss.Get();
			if(ii >= pic.data.GetCount())
				return;
			v = pic.data[ii++];
			if(!v.Is<Image>())
				return;
			FillOp((Image)v, m, f);
			break;
		case PAINTING_FILL_GRADIENT:
			sGet(p, ss);
			sGet(c, ss);
			sGet(p1, ss);
			sGet(c1, ss);
			f = ss.Get();
			FillOp(p, c, p1, c1, f);
			break;
		case PAINTING_FILL_GRADIENT_X:
			sGet(c, ss);
			sGet(c1, ss);
			sGet(m, ss);
			f = ss.Get();
			FillOp(c, c1, m, f);
			break;
		case PAINTING_FILL_RADIAL_X:
			sGet(p1, ss);
			sGet(c, ss);
			sGet(c1, ss);
			sGet(m, ss);
			f = ss.Get();
			FillOp(p1, c, c1, m, f);
			break;
		case PAINTING_FILL_RADIAL:
			sGet(p, ss);
			sGet(c, ss);
			sGet(p1, ss);
			sGet(r, ss);
			sGet(c1, ss);
			f = ss.Get();
			FillOp(p, c, p1, r, c1, f);
			break;
		case PAINTING_STROKE_SOLID:
			sGet(w, ss);
			sGet(c, ss);
			StrokeOp(w, c);
			break;
		case PAINTING_STROKE_IMAGE:
			sGet(w, ss);
			sGet(m, ss);
			f = ss.Get();
			if(ii >= pic.data.GetCount())
				return;
			v = pic.data[ii++];
			if(!v.Is<Image>())
				return;
			StrokeOp(w, (Image)v, m, f);
			break;
		case PAINTING_STROKE_GRADIENT:
			sGet(w, ss);
			sGet(p, ss);
			sGet(c, ss);
			sGet(p1, ss);
			sGet(c1, ss);
			f = ss.Get();
			StrokeOp(w, p, c, p1, c1, f);
			break;
		case PAINTING_STROKE_GRADIENT_X:
			sGet(w, ss);
			sGet(c, ss);
			sGet(c1, ss);
			sGet(m, ss);
			f = ss.Get();
			FillOp(c, c1, m, f);
			StrokeOp(w, c, c1, m, f);
			break;
		case PAINTING_STROKE_RADIAL_X:
			sGet(w, ss);
			sGet(p1, ss);
			sGet(c, ss);
			sGet(c1, ss);
			sGet(m, ss);
			f = ss.Get();
			FillOp(c, c1, m, f);
			StrokeOp(w, p1, c, c1, m, f);
			break;
		case PAINTING_STROKE_RADIAL:
			sGet(w, ss);
			sGet(p, ss);
			sGet(c, ss);
			sGet(p1, ss);
			sGet(r, ss);
			sGet(c1, ss);
			f = ss.Get();
			StrokeOp(w, p, c, p1, r, c1, f);
			break;
		case PAINTING_CLIP:
			ClipOp();
			break;
		case PAINTING_CHARACTER:
			sGet(p, ss);
			ch = ss.Get32();
			sGet(fnt, ss);
			CharacterOp(p, ch, fnt);
			break;
		case PAINTING_TEXT:
			{
				sGet(p, ss);
				n = ss.Get32();
				hasdx = ss.Get();
				sGet(fnt, ss);
				Buffer<wchar> txt(n);
				Buffer<double> dx(hasdx * n);
				for(int i = 0; i < n; i++) {
					txt[i] = ss.Get32();
					if(hasdx)
						sGet(dx[i], ss);
				}
				TextOp(p, txt, fnt, n, hasdx ? ~dx : NULL);
			}
			break;
		case PAINTING_COLORSTOP:
			sGet(r, ss);
			sGet(c, ss);
			ColorStopOp(r, c);
			break;
		case PAINTING_CLEARSTOPS:
			ClearStopsOp();
			break;
		case PAINTING_OPACITY:
			OpacityOp(sGet<double>(ss));
			break;
		case PAINTING_LINECAP:
			LineCapOp(ss.Get());
			break;
		case PAINTING_LINEJOIN:
			LineJoinOp(ss.Get());
			break;
		case PAINTING_MITERLIMIT:
			MiterLimitOp(ss.Get());
			break;
		case PAINTING_EVENODD:
			EvenOddOp(ss.Get());
			break;
		case PAINTING_INVERT:
			InvertOp(ss.Get());
			break;
		case PAINTING_IMAGE_FILTER:
			ImageFilterOp(ss.Get());
			break;
		case PAINTING_DASH:
			{
				n = ss.Get32();
				Vector<double> dash;
				for(int i = 0; i < n; i++)
					dash.Add(sGet<double>(ss));
				r = sGet<double>(ss);
				DashOp(dash, r);
			}
			break;
		case PAINTING_TRANSFORM:
			sGet(m, ss);
			TransformOp(m);
			break;
		case PAINTING_BEGIN:
			BeginOp();
			break;
		case PAINTING_END:
			EndOp();
			break;
		case PAINTING_BEGINMASK:
			BeginMaskOp();
			break;
		case PAINTING_BEGINONPATH:
			sGet(r, ss);
			BeginOnPathOp(r, ss.Get());
			break;
		}
	}
}

void PaintImageBufferPaintingFn(ImageBuffer& ib, const Painting& p, Size sz, Point pos, int mode)
{
	BufferPainter sw(ib, mode);
	Sizef psz = p.GetSize();
	sw.Translate(-pos.x, -pos.y);
	sw.Scale(sz.cx / psz.cx, sz.cy / psz.cy);
	sw.Paint(p);
}

void PaintImageBufferDrawingFn(ImageBuffer& ib, const Drawing& iw, int mode)
{
	BufferPainter sw(ib, mode);
	Sizef sz = ib.GetSize();
	Size isz = iw.GetSize();
	sw.Scale(sz.cx / isz.cx, sz.cy / isz.cy);
	sw.DrawDrawing(0, 0, isz.cx, isz.cy, iw);
}

void RegisterPaintingFns__(void (*ig)(ImageBuffer& ib, const Painting& pw, Size sz, Point pos, int mode),
                           void (*iw)(ImageBuffer& ib, const Drawing& p, int mode),
                           Image (*rg)(Point at, int angle, int chr, Font fnt, Color color, Size sz));

void ImageAnyDrawPainter(Draw *(*f)(Size sz), Image (*e)(Draw *w));

static Draw *sCP(Size sz)
{
	return new ImagePainter(sz);
}

static Image sEP(Draw *w)
{
	ImagePainter *ip = dynamic_cast<ImagePainter *>(w);
	return ip ? (Image)(*ip) : Image();
}

static Image sRenderGlyph(Point at, int angle, int chr, Font fnt, Color color, Size sz)
{
	ImageBuffer ib(sz);
	BufferPainter sw(ib);
	sw.Clear(RGBAZero());
	sw.DrawText(at.x, at.y, angle, WString(chr, 1), fnt, color);
	Image h = ib;
	return Premultiply(h);
}

INITIALIZER(PaintPainting)
{
	RegisterPaintingFns__(PaintImageBufferPaintingFn, PaintImageBufferDrawingFn, sRenderGlyph);
	ImageAnyDrawPainter(sCP, sEP);
}

}
