#include "RichText.h"

namespace Upp {

#define IMAGECLASS RichTextImg
#define IMAGEFILE <RichText/RichText.iml>
#include <Draw/iml_source.h>

Color PaintInfo::ResolveInk(Color ink) const
{
	return Nvl(textcolor, darktheme ? Nvl(DarkThemeCached(ink), SColorText()) : Nvl(ink, SColorText()));
}

Color PaintInfo::ResolvePaper(Color paper) const
{
	return IsNull(paper) ? paper : darktheme ? DarkThemeCached(paper) : paper;
}

RichPara::Lines RichPara::Begin(RichContext& rc) const
{
	Lines pl = FormatLines(rc.page.Width(), rc.number);
	rc.py.y += format.before + format.ruler;
	pl.Justify(format);
	return pl;
}

void RichPara::Flush(Draw& draw, const PaintInfo& pi, wchar *text,
                     const CharFormat **i0,
                     int *wd, int pos, int len, int x0, int x, int y0,
                     int y, int linecy, int lineascent, Zoom z,
                     bool highlight)
{
	if(*i0) {
		const CharFormat& f = **i0;
		int width = z * x - z * x0;
		int zy0 = z * y0;
		int zx0 = z * x0;
		if(!IsNull(f.indexentry) && !IsNull(pi.indexentry)) {
			if(pi.indexentrybg)
				draw.DrawRect(zx0, z * y, width, z * (y + linecy) - z * y,
				              pi.ResolvePaper(pi.indexentry));
			else
				draw.DrawRect(zx0, zy0, width, 2, pi.indexentry);
		}
		if(!IsNull(f.paper) && !highlight && IsNull(pi.textcolor))
			draw.DrawRect(zx0, z * y, width, z * (y + linecy) - z * y, pi.ResolvePaper(f.paper));
		Font fnt = f;
		if(pi.mono_glyphs)
			fnt.NoColor();
		int zht = z * tabs(f.GetHeight());
		int ssa = 0;
		int ssd = 0;
		if(f.sscript) {
			FontInfo fi = fnt(zht).Info();
			ssa = fi.GetAscent();
			ssd = fi.GetDescent();
			zht = 3 * zht / 5;
		}
		fnt.Height(zht ? zht : 1);
		FontInfo fi = fnt.Info();
		Color ink = pi.ResolveInk(f.ink);
		if(f.dashed) {
			int dx = max(fi.GetAscent() / 5, 2);
			for(int i = 0; dx * i < width; i++)
				draw.DrawRect(zx0 + i * dx, zy0 + fi.GetDescent() / 2,
				              dx / 2, max(fi.GetDescent() / 3, 1), ink);
		}
		if(!IsNull(f.link) && !IsNull(pi.hyperlink) && !(fnt.IsUnderline() || f.dashed)) {
			fnt.Underline();
			if(IsNull(pi.textcolor))
				ink = pi.hyperlink;
		}
		x = zx0;
		Vector< Tuple2<int, int> > bak;
		if(f.link.GetCount())
			draw.Escape("url:" + f.link);
		for(int i = 0; i < len; i++) {
			int w = wd[pos + i];
			Image img;
			wchar& c = text[pos + i];
			if(c == ' ')
				img = RichTextImg::SpaceChar();
			if(c == 160)
				img = RichTextImg::HardSpaceChar();
			if(c >= 9 && c < 9 + 4) {
				if(c > 9) {
					static char fct[] = { ' ', '.', '-', '_' };
					int fc = fct[c - 9];
					int pw = fi[fc];
					int px = (x + pw - 1) / pw * pw;
					while(px + pw < x + w) {
						draw.DrawText(px, zy0 - fi.GetAscent(), String(fc, 1), fnt, Black);
						px += pw;
					}
				}
				img = RichTextImg::TabChar();
				bak.Add(MakeTuple(pos + i, c));
				c = ' ';
			}
			if(img && !IsNull(pi.showcodes)) {
				Size sz = img.GetSize();
				if(sz.cy < z * lineascent)
					draw.DrawImage(x + (w - sz.cx) / 2, zy0 - sz.cy, img, pi.showcodes);
			}
			x += w;
		}
		draw.DrawText(zx0,
		              f.sscript == 1 ? zy0 - ssa :
		              f.sscript == 2 ? zy0 + ssd - fi.GetHeight()
		                             : zy0 - fi.GetAscent(),
		              text + pos, fnt, ink, len, wd + pos);
		for(int i = 0; i < bak.GetCount(); i++)
			text[bak[i].a] = bak[i].b;
		if(f.link.GetCount())
			draw.Escape("url:");
	}
}

bool RichPara::BreaksPage(const RichContext& rc, const Lines& pl, int i) const
{
	int linecy = pl[i].Sum();
	if(linecy >= rc.page.Height()) return false;
	if(linecy + rc.py.y > rc.page.bottom)
		return true;
	if(format.orphan || pl.GetCount() < 2) return false;
	if((i == 0 || i == pl.GetCount() - 2) && rc.py.y + linecy + pl[i + 1].Sum() > rc.page.bottom)
		return true;
	return false;
}

struct EllipseImageMaker : ImageMaker {
	Size       sz;

	virtual String Key() const;
	virtual Image  Make() const;
};

String EllipseImageMaker::Key() const
{
	return String((const char *)&sz, sizeof(sz));
}

Image EllipseImageMaker::Make() const
{
	ImagePainter p(sz);
	p.Clear(RGBAZero());
	p.DrawEllipse(0, 0, sz.cx, sz.cy, Black());
	return p;
}

static Image sEllipseImage(Size sz)
{
	EllipseImageMaker m;
	m.sz = sz;
	return MakeImage(m);
}

struct RichObjectImageMaker : ImageMaker {
	RichObject object;
	Size       sz;
	Color      ink;
	bool       dark = false;
	void       *context;

	virtual String Key() const;
	virtual Image  Make() const;
};

String RichObjectImageMaker::Key() const
{
	StringBuffer b;
	RawCat(b, object.GetSerialId());
	RawCat(b, sz);
	RawCat(b, context);
	RawCat(b, ink);
	RawCat(b, dark);
	return String(b);
}

Image RichObjectImageMaker::Make() const
{
	RichObjectPaintInfo pi;
	pi.ink = ink;
	pi.context = context;
	pi.dark = dark;
	return object.ToImage(sz, pi);
}

void RichPara::DrawRuler(Draw& w, int x, int y, int cx, int cy, Color ink, int style)
{
	int segment = cy;
	int r = x + cx;
	switch(style) {
	case RULER_DASH:
		segment = 2 * cy;
	case RULER_DOT:
		while(x < r) {
			w.DrawRect(x, y, min(r - x, segment), cy, ink);
			x += 2 * segment;
		}
		break;
	default:
		w.DrawRect(x, y, cx, cy, ink);
		break;
	}
}

void RichPara::Paint(PageDraw& pw, RichContext rc, const PaintInfo& pi,
                     const Number& n, const Bits& spellerror, bool baselevel) const
{
	Zoom z = pi.zoom;
	PageY opy = rc.py;
	Lines pl = Begin(rc);
	if(pw.tracer) {
		PageY h = rc.py;
		h.y -= format.before + format.ruler;
		pw.tracer->Paragraph(rc.page, h, *this);
	}
	
	bool highlight = pi.highlightpara >= 0 && pi.highlightpara < pl.len || pi.WhenHighlight(format.label);
	int hy = rc.py.y - format.before - format.ruler;
	int phy = rc.py.page;
	if(format.ruler && hy >= 0 && hy + format.ruler < rc.page.bottom)
		DrawRuler(pw.Page(phy), z * rc.page.left + z * format.lm, z * hy,
		                        z * rc.page.right - z * rc.page.left - z * format.rm - z * format.lm,
			                    max(1, z * format.ruler), pi.ResolveInk(format.rulerink), format.rulerstyle);
	if(pi.sell < 0 && pi.selh > 0)
		for(int p = opy.page; p <= rc.py.page; p++) {
			int top = z * (p == opy.page ? opy.y : rc.page.top);
			int bottom = z * (p == rc.py.page ? rc.py.y : rc.page.bottom);
			pi.DrawSelection(pw.Page(p), z * rc.page.left, top, z * rc.page.right - z * rc.page.left, bottom - top);
		}
	opy = rc.py;
	int x = 0;
	int y0 = 0;
	int lineascent = 0;
	PageY mpy;
	int   mla = Null;
	for(int lni = 0; lni < pl.GetCount(); lni++) {
		const Line& li = pl[lni];
		int linecy = li.Sum();
		lineascent = li.ascent;
		if(BreaksPage(rc, pl, lni)) {
			if(li.ppos > pi.sell && li.ppos < pi.selh) {
				int y = z * rc.py.y;
				pi.DrawSelection(pw.Page(rc.py.page), z * rc.page.left, y, z * rc.page.right - z * rc.page.left,
				                                      z * rc.page.bottom - y);
			}
			rc.Page();
		}
		if(rc.py > pi.bottom)
			break;

		if(lni == 0) {
			mpy = rc.py;
			mla = lineascent;
		}
		const CharFormat **cf = pl.format + li.pos;
		const CharFormat **i = cf;
		const CharFormat **ilim = i + li.len;
		const HeightInfo *hg = pl.height + li.pos;
		if(rc.py + linecy >= pi.top) {
			Draw& draw = pw.Page(rc.py.page);
#ifdef _DEBUG
			int cloff = draw.GetCloffLevel();
#endif
			Buffer<int> wd(li.len);
			int x0 = li.xpos + rc.page.left;
			x = x0;
			int *w = pl.width + li.pos;
			int *wl = w + li.len;
			int *t = wd;
			while(w < wl) {
				int x1 = x + *w++;
				*t++ = z * x1 - z * x;
				x = x1;
			}

			if(highlight)
				draw.DrawRect(z * rc.page.left, z * rc.py.y, z * rc.page.Width(),
					          z * (rc.py.y + linecy) - z * rc.py.y,
					          pi.highlight);

			const CharFormat **i0 = i;
			wchar *text = pl.text + li.pos;
			x = x0;
			w = pl.width + li.pos;
			y0 = rc.py.y + li.ascent;
			int pp = li.pos;
			int l = rc.page.right;
			int h = -1;

			while(i < ilim) {
				if(pl.pos[pp] == pi.sell && x < l)
					l = x;
				if(pl.pos[pp] == pi.selh && h < 0)
					h = x;
				pp++;
				if(*i0 != *i || hg->object) {
					Flush(draw, pi, text, i0, wd, (int)(i0 - cf), (int)(i - i0), x0, x, y0, rc.py.y, linecy,
					      lineascent, z, highlight);
					i0 = i;
					x0 = x;
				}
				if(hg->object) {
					const RichObject& o = *hg->object;
					if(o) {
						Size sz = z * o.GetSize();
						int ix = z * x;
						if(pi.shrink_oversized_objects && sz.cx + ix > rc.page.right)
							sz.cx = rc.page.right - ix;
						draw.DrawRect(ix, z * rc.py.y, sz.cx, z * linecy, (*i)->paper);
						draw.Clipoff(ix, z * (y0 - hg->ascent), sz.cx, sz.cy);
						if(pi.sizetracking)
							draw.DrawRect(sz, SColorFace);
						else
							if(pi.usecache) {
								RichObjectImageMaker im;
								im.object = o;
								im.sz = sz;
								im.ink = (*i)->ink;
								im.dark = pi.darktheme;
								im.context = pi.context;
								draw.DrawImage(0, 0, MakeImagePaintOnly(im));
							}
							else {
								RichObjectPaintInfo oi;
								oi.ink = (*i)->ink;
								oi.context = pi.context;
								oi.dark = pi.darktheme;
								o.Paint(draw, sz, oi);
							}
						draw.End();
					}
					i++;
					hg++;
					x += *w++;
					i0 = i;
					x0 = x;
				}
				else {
					i++;
					hg++;
					x += *w++;
				}
			}
			if(i > i0)
				Flush(draw, pi, text, i0, wd, (int)(i0 - cf), (int)(i - i0), x0, x, y0, rc.py.y, linecy,
				      lineascent, z, highlight);
			if(lni == 0) {
				Rect r;
				r.left = rc.page.left + format.lm;
				int q = li.ascent / 2;
				r.top = rc.py.y + 4 * (li.ascent - q) / 5;
				r.right = r.left + q;
				r.bottom = r.top + q;
				q = z * (r.Width() / 4);
				r.left = z * r.left;
				r.top = z * r.top;
				r.right = z * r.right;
				r.bottom = z * r.bottom;
				Rect r1 = r;
				r1.Deflate(max(1, q));
				Color bullet_ink = format.ink;
				if(format.bullet) {
					if(part.GetCount())
						bullet_ink = part[0].format.ink;
				}
				switch(format.bullet) {
				case BULLET_BOX:
					draw.DrawRect(r, pi.ResolveInk(bullet_ink));
					break;
				case BULLET_BOXWHITE:
					draw.DrawRect(r, pi.ResolveInk(bullet_ink));
					draw.DrawRect(r1, pi.ResolvePaper(White()));
					break;
				case BULLET_ROUNDWHITE:
					if(draw.IsPrinter()) {
						draw.DrawEllipse(r, pi.ResolveInk(bullet_ink));
						draw.DrawEllipse(r1, pi.ResolvePaper(White()));
					}
					else {
						draw.DrawImage(r.left, r.top, sEllipseImage(r.GetSize()), pi.ResolveInk(bullet_ink));
						draw.DrawImage(r1.left, r1.top, sEllipseImage(r1.GetSize()), pi.ResolvePaper(White()));
					}
					break;
				case BULLET_ROUND:
					if(draw.IsPrinter())
						draw.DrawEllipse(r, pi.ResolveInk(bullet_ink));
					else
						draw.DrawImage(r.left, r.top, sEllipseImage(r.GetSize()), pi.ResolveInk(bullet_ink));
					break;
				}
			}
			int zlcy = z * linecy;
			if(pi.spellingchecker && zlcy > 3) {
				int x = z * (li.xpos + rc.page.left);
				w = wd;
				wl = w + li.len;
				int i = li.pos;
				int q = z * (rc.py.y + linecy);
				while(w < wl) {
					if(spellerror[pl.pos[i]] && (lni || (i >= pl.number_chars))) {
						if(zlcy > 16)
							draw.DrawRect(x, q - 3, *w, 1, Red);
						draw.DrawRect(x, q - 2, *w, 1, LtRed);
					}
					x += *w++;
					i++;
				}
			}

			if(pi.selh == li.ppos + li.plen)
				h = x;
			if(pi.sell == li.ppos + li.plen)
				l = x;
			if(pi.sell < li.ppos)
				l = rc.page.left;
			if(pi.selh > li.ppos + li.plen)
				h = rc.page.right;
			if(pi.sell < pi.selh && pi.selh > li.ppos)
				pi.DrawSelection(draw, z * l, z * rc.py.y, z * h - z * l, z * (rc.py.y + linecy) - z * rc.py.y);
			ASSERT(draw.GetCloffLevel() == cloff);
		}
		else
			while(i < ilim) {
				i++;
				hg++;
			}
		rc.py.y += linecy;
	}
	Size sz = RichTextImg::EndParaChar().GetSize();
	if(sz.cy < z * lineascent && !IsNull(pi.showcodes))
		pw.Page(rc.py.page).DrawImage(z * x, z * y0 - sz.cy,
		                              RichTextImg::EndParaChar(),
		                              format.indexentry.GetCount() ? pi.indexentry : pi.showcodes);
	if(format.newpage && !IsNull(pi.showcodes)) {
		Draw& w = pw.Page(opy.page);
		int wd = z * rc.page.right - z * rc.page.left;
		int step = w.Pixels() ? 8 : 50;
		int y = z * opy.y;
		for(int x = 0; x < wd; x += step)
			w.DrawRect(z * rc.page.left + x, y, step >> 1, step >> 3, pi.showcodes);
	}
	if(pl.len >= pi.sell && pl.len < pi.selh && rc.py < pi.bottom) {
		int top = z * rc.py.y;
		pi.DrawSelection(pw.Page(rc.py.page), z * rc.page.left, top, z * rc.page.right - z * rc.page.left,
		                                      z * min(rc.py.y + format.after, rc.page.bottom) - top);
	}
	if(!IsNull(mla) && !IsNull(pi.showcodes) && pi.showlabels) {
		bool b = format.label.GetCount();
		Image img = RichTextImg::Label();
		for(int pass = 0;;pass++) {
			Size isz = img.GetSize();
			if(b)
				pw.Page(mpy.page).DrawImage(-7 - isz.cx, z * mpy.y + (z * mla - isz.cy) / 2,
				                            img, pi.showcodes);
			if(pass)
				break;
			b = format.newhdrftr && baselevel;
			img = RichTextImg::HdrFtr();
		}
	}
	if(pw.tracer)
		pw.tracer->EndParagraph(rc.py);
}

void RichPara::GetRichPos(RichPos& rp, int pos) const
{
	rp.format = format;
	int i = FindPart(pos);
	if(i < GetCount()) {
		const Part& p = part[i];
		(CharFormat&)rp.format = p.format;
		if(p.object) {
			rp.chr = -1;
			rp.object = p.object;
		}
		else
		if(p.IsText())
			rp.chr = p.text[pos];
		else {
			rp.chr = -2;
			rp.field = p.field;
			rp.fieldparam = p.fieldparam;
			if(p.fieldpart.GetCount() > 0)
				rp.fieldformat = p.fieldpart[0].format;
		}
	}
	else
		rp.chr = '\n';
}

RichCaret RichPara::GetCaret(int pos, RichContext rc) const
{
	Lines pl = Begin(rc);
	RichCaret pr;
	FontInfo fi = format.Info();
	pr.caretascent = fi.GetAscent();
	pr.caretdescent = fi.GetDescent();
	for(int lni = 0; lni < pl.GetCount(); lni++) {
		Line& li = pl[lni];
		int linecy = li.Sum();
		if(BreaksPage(rc, pl, lni))
			rc.Page();
		pr.page = rc.py.page;
		pr.top = rc.py.y;
		pr.bottom = rc.py.y + linecy;
		pr.lineascent = li.ascent;
		pr.line = lni;
		if(pos < li.ppos + li.plen) {
			int npos = li.pos;
			int x = li.xpos + rc.page.left;
			if(lni == 0 && pl.number_chars) { // skip the number
				npos += pl.number_chars;
				for(int i = 0; i < pl.number_chars; i++)
					x += pl.width[li.pos + i];
			}
			int *w = pl.width + npos;
			int *p = pl.pos + npos;
			const CharFormat **i = pl.format + npos;
			const HeightInfo *h = pl.height + npos;
			if(li.len && *i) {
				pr.caretascent = h->ascent;
				pr.caretdescent = h->descent;
			}
			while(pos > *p) { // second condition to skip the number
				x += *w++;
				if(*i) {
					pr.caretascent = h->ascent;
					pr.caretdescent = h->descent;
				}
				h++;
				i++;
				p++;
			}
			pr.objectcy = h->ascent + h->ydelta;
			pr.objectyd = h->ydelta;
			pr.left = x;
			pr.right = x + *w;
			pr.caretdescent = min(pr.caretdescent, pr.Height() - pr.lineascent);
			return pr;
		}
		rc.py.y += linecy;
	}
	const Line& li = pl.line.Top();
	pr.left = li.cx + li.xpos + rc.page.left;
	pr.right = rc.page.right;
	pr.caretdescent = min(pr.caretdescent, pr.Height() - pr.lineascent);
	return pr;
}

int RichPara::PosInLine(int x, const Rect& page, const Lines& pl, int lni) const
{
	const Line& li = pl[lni];
	int xp = li.xpos + page.left;
	int npos = li.pos;
	if(lni == 0 && pl.number_chars) { // skip the number
		npos += pl.number_chars;
		for(int i = 0; i < pl.number_chars; i++)
			xp += pl.width[li.pos + i];
	}
	const int *w = pl.width + npos;
	const int *wl = w + li.len;
	if(lni < pl.GetCount() - 1 && li.len > 0 && pl.text[npos + li.len - 1] == ' ')
		wl--;
	while(w < wl && xp + *w <= x)
		xp += *w++;
	int pos = (int)(w - pl.width);
	return pos < pl.clen ? pl.pos[pos] : pl.len;
}

int RichPara::GetPos(int x, PageY y, RichContext rc) const
{
	Lines pl = Begin(rc);
	if(pl.len)
		for(int lni = 0; lni < pl.GetCount(); lni++) {
			const Line& li = pl[lni];
			int linecy = li.Sum();
			if(BreaksPage(rc, pl, lni))
				rc.Page();
			rc.py.y += linecy;
			if(y < rc.py || lni == pl.GetCount() - 1)
				return PosInLine(x, rc.page, pl, lni);
		}
	return pl.len;
}

int RichPara::GetVertMove(int pos, int gx, const Rect& page, int dir, const RichContext& rc) const
{
	Lines pl = FormatLines(page.Width(), rc.number);
	int lni;
	if(pos >= 0) {
		for(lni = 0; lni < pl.GetCount() - 1; lni++) {
			const Line& li = pl[lni];
			if(pos < li.ppos + li.plen)
				break;
		}
		lni += sgn(dir);
		if(lni < 0 || lni >= pl.GetCount() || lni == 0 && pl.number_chars == pl[lni].len)
			return -1;
	}
	else
		lni = dir < 0 ? pl.GetCount() - 1 : 0;
	return PosInLine(gx, page, pl, lni);
}

void  RichPara::GatherLabels(Vector<RichValPos>& info, RichContext rc, int pos) const
{
	Lines pl = Begin(rc);
	WString ie;
	if(!pl.GetCount())
		return;
	if(BreaksPage(rc, pl, 0))
		rc.Page();
	if(format.label.IsEmpty())
		return;
	RichValPos& f = info.Add();
	f.py = rc.py;
	f.pos = pos;
	f.data = format.label.ToWString();
}

void  RichPara::GatherIndexes(Vector<RichValPos>& info, RichContext rc, int pos) const
{
	Lines pl = Begin(rc);
	WString ie;
	for(int lni = 0; lni < pl.GetCount(); lni++) {
		Line& li = pl[lni];
		int linecy = li.Sum();
		if(BreaksPage(rc, pl, lni))
			rc.Page();
		const CharFormat **i0 = pl.format + li.pos;
		const CharFormat **i = i0;
		const CharFormat **ilim = i + li.len;
		while(i < ilim) {
			if(*i && (*i)->indexentry != ie) {
				ie = (*i)->indexentry;
				if(!ie.IsEmpty()) {
					RichValPos& f = info.Add();
					f.py = rc.py;
					f.pos = (int)(i - i0) + pos;
					f.data = ie;
				}
			}
			i++;
		}
		rc.py.y += linecy;
	}
}

void FontHeightRound(Font& fnt, Zoom z)
{
	fnt.Height(max((fnt.GetHeight() * z.m + (z.d >> 1)) / z.d, 1)); // do not allow to fall to zero (that is default font height)
}

void operator*=(RichPara::Format& format, Zoom z)
{
	FontHeightRound(format, z);
	format.before *= z;
	if(format.ruler) {
		format.ruler *= z;
		if(format.ruler == 0)
			format.ruler = 1;
	}
	int ll = format.lm + format.indent;
	format.lm *= z;
	format.indent = z * ll - format.lm;
	format.rm *= z;
	format.after *= z;
	format.tabsize *= z;
	for(int i = 0; i < format.tab.GetCount(); i++) {
		int& pos = format.tab[i].pos;
		pos = ((pos & ~RichPara::TAB_RIGHTPOS) * z) | (pos & RichPara::TAB_RIGHTPOS);
	}
}

void  RichPara::ApplyZoom(Zoom z)
{
	format *= z;
	for(int i = 0; i < part.GetCount(); i++)
		if(part[i].IsText())
			FontHeightRound(part[i].format, z);
		else
		if(part[i].object)
			part[i].object.SetSize(z * part[i].object.GetSize());
}

}
