#include "RichText.h"

NAMESPACE_UPP

#define IMAGECLASS RichTextImg
#define IMAGEFILE <RichText/RichText.iml>
#include <Draw/iml.h>

RichPara::Lines RichPara::Begin(const Rect& page, PageY& py, int nbefore, int nline) const
{
	Lines pl = FormatLines(page.Width());
	int cy = format.ruler + format.before;
	if(format.keep || format.keepnext)
		cy += pl.BodyHeight();
	else
		cy += pl[0].Sum();
	int after = format.after;
	if(!format.keepnext)
		nbefore = nline = after = 0;
	if(page.Height() < 32000 &&
	   (format.newpage || py.y + cy + after + nbefore + nline > page.bottom &&
	    cy < page.Height())) {
		py.page++;
		py.y = page.top;
	}
	py.y += format.before + format.ruler;
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
				              pi.coloroverride ? SColorPaper() : pi.indexentry);
			else
				draw.DrawRect(zx0, zy0, width, 2, pi.indexentry);
		}
		if(!IsNull(f.paper) && !highlight)
			draw.DrawRect(zx0, z * y, width, z * (y + linecy) - z * y,
			              pi.coloroverride ? SColorPaper() : f.paper);
		Font fnt = f;
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
		if(f.dashed) {
			int dx = max(fi.GetAscent() / 5, 2);
			for(int i = 0; dx * i < width; i++)
				draw.DrawRect(zx0 + i * dx, zy0 + fi.GetDescent() / 2,
				              dx / 2, max(fi.GetDescent() / 3, 1),
				              pi.coloroverride ? SColorText() : f.ink);
		}
		Color ink = pi.coloroverride ? SColorText() : f.ink;
		if(!IsNull(f.link) && !IsNull(pi.hyperlink) && !(fnt.IsUnderline() || f.dashed)) {
			fnt.Underline();
			if(!pi.coloroverride)
				ink = pi.hyperlink;
		}
		x = zx0;
		Vector< Tuple2<int, int> > bak;
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
	}
}

bool RichPara::BreaksPage(PageY py, const Lines& pl, int i, const Rect& page) const
{
	int linecy = pl[i].Sum();
	if(linecy >= page.Height()) return false;
	if(linecy + py.y > page.bottom)
		return true;
	if(format.orphan || pl.GetCount() < 2) return false;
	if((i == 0 || i == pl.GetCount() - 2) && py.y + linecy + pl[i + 1].Sum() > page.bottom)
		return true;
	return false;
}

struct RichObjectImageMaker : ImageMaker {
	RichObject object;
	Size       sz;
	void       *context;

	virtual String Key() const;
	virtual Image  Make() const;
};

String RichObjectImageMaker::Key() const
{
	StringBuffer b;
	int64 id = object.GetSerialId();
	b.Cat((const char *)&id, sizeof(id));
	b.Cat((const char *)&sz.cx, sizeof(sz.cx));
	b.Cat((const char *)&sz.cy, sizeof(sz.cy));
	b.Cat((const char *)&context, sizeof(context));
	return b;
}

Image RichObjectImageMaker::Make() const
{
	return object.ToImage(sz, context);
/*	ImageDraw iw(sz);
	iw.DrawRect(sz, SColorPaper());
	object.Paint(iw, sz, context);
	return iw;
*/
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

void RichPara::Paint(PageDraw& pw, const Rect& page, PageY py, const PaintInfo& pi,
                     const Number& n, const Bits& spellerror,
                     int nbefore, int nline) const
{
	Zoom z = pi.zoom;
	PageY opy = py;
	if(pw.tracer)
		pw.tracer->Paragraph(page, py, *this);
	Lines pl = Begin(page, py, nbefore, nline);
	bool highlight = pi.highlightpara >= 0 && pi.highlightpara < pl.len;
	int hy = py.y - format.before - format.ruler;
	int phy = py.page;
	if(format.ruler && hy >= 0 && hy + format.ruler < page.bottom)
		DrawRuler(pw.Page(phy), z * page.left + z * format.lm, z * hy,
		                        z * page.right - z * page.left - z * format.rm - z * format.lm,
			                    max(1, z * format.ruler), format.rulerink, format.rulerstyle);
	if(pi.sell < 0 && pi.selh > 0)
		for(int p = opy.page; p <= py.page; p++) {
			int top = z * (p == opy.page ? opy.y : page.top);
			int bottom = z * (p == py.page ? py.y : page.bottom);
			pw.Page(p).DrawRect(z * page.left, top, z * page.right - z * page.left,
			                    bottom - top, InvertColor);
		}
	opy = py;
	int oi = 0;
	int x = 0;
	int y0 = 0;
	int lineascent = 0;
	for(int lni = 0; lni < pl.GetCount(); lni++) {
		const Line& li = pl[lni];
		int linecy = li.Sum();
		lineascent = li.ascent;
		if(BreaksPage(py, pl, lni, page)) {
			if(li.ppos > pi.sell && li.ppos < pi.selh) {
				int y = z * py.y;
				pw.Page(py.page).DrawRect(z * page.left, y, z * page.right - z * page.left,
				                          z * page.bottom - y, InvertColor);
			}
			py.y = page.top;
			py.page++;
		}
		if(py > pi.bottom)
			break;

		if(lni == 0 && format.label.GetCount() && !IsNull(pi.showcodes) && pi.showlabels)
			pw.Page(py.page).DrawImage(z * page.left - 12, z * (py.y + (lineascent - 7) / 2),
			                           RichTextImg::Label(), pi.showcodes);
		const CharFormat **cf = pl.format + li.pos;
		const CharFormat **i = cf;
		const CharFormat **ilim = i + li.len;
		const HeightInfo *hg = pl.height + li.pos;
		if(py + linecy >= pi.top) {
			Draw& draw = pw.Page(py.page);
#ifdef _DEBUG
			int cloff = draw.GetCloffLevel();
#endif
			Buffer<int> wd(li.len);
			int x0 = li.xpos + page.left;
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
				draw.DrawRect(z * page.left, z * py.y, z * page.Width(),
					          z * (py.y + linecy) - z * py.y,
					          pi.highlight);

			const CharFormat **i0 = i;
			wchar *text = pl.text + li.pos;
			x = x0;
			w = pl.width + li.pos;
			y0 = py.y + li.ascent;
			int pp = li.pos;
			int l = page.right;
			int h = -1;

			while(i < ilim) {
				if(pl.pos[pp] == pi.sell && x < l)
					l = x;
				if(pl.pos[pp] == pi.selh && h < 0)
					h = x;
				pp++;
				if(*i0 != *i || hg->object) {
					Flush(draw, pi, text, i0, wd, (int)(i0 - cf),(int)( i - i0), x0, x, y0, py.y, linecy,
					      lineascent, z, highlight);
					i0 = i;
					x0 = x;
				}
				if(hg->object) {
					const RichObject& o = *hg->object;
					if(o) {
						Size sz = z * o.GetSize();
						int ix = z * x;
						if(pi.shrink_oversized_objects && sz.cx + ix > page.right)
							sz.cx = page.right - ix;
						draw.DrawRect(ix, z * py.y, sz.cx, z * linecy, (*i)->paper);
						draw.Clipoff(ix, z * (y0 - hg->ascent), sz.cx, sz.cy);
						if(pi.sizetracking)
							draw.DrawRect(sz, SColorFace);
						else
							if(pi.usecache) {
								RichObjectImageMaker im;
								im.object = o;
								im.sz = sz;
								im.context = pi.context;
								draw.DrawImage(0, 0, MakeImagePaintOnly(im));
							}
							else
								o.Paint(draw, sz, pi.context);
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
				Flush(draw, pi, text, i0, wd, (int)(i0 - cf), (int)(i - i0), x0, x, y0, py.y, linecy,
				      lineascent, z, highlight);
			if(lni == 0) {
				Rect r;
				r.left = page.left + format.lm;
				int q = li.ascent / 2;
				r.top = py.y + 4 * (li.ascent - q) / 5;
				r.right = r.left + q;
				r.bottom = r.top + q;
				q = z * (r.Width() / 4);
				r.left = z * r.left;
				r.top = z * r.top;
				r.right = z * r.right;
				r.bottom = z * r.bottom;
				Rect r1 = r;
				r1.Deflate(max(1, q));
				switch(format.bullet) {
				case BULLET_BOX:
					draw.DrawRect(r, pi.coloroverride ? SColorText() : format.ink);
					break;
				case BULLET_BOXWHITE:
					draw.DrawRect(r, pi.coloroverride ? SColorText() : format.ink);
					draw.DrawRect(r1, White);
					break;
				case BULLET_ROUNDWHITE:
					draw.DrawEllipse(r, pi.coloroverride ? SColorText() : format.ink);
					draw.DrawEllipse(r1, pi.coloroverride ? SColorPaper() : White);
					break;
				case BULLET_ROUND:
					draw.DrawEllipse(r, pi.coloroverride ? SColorText() : format.ink);
					break;
				default:
					String s = n.AsText(format);
					if(!IsNull(s)) {
						CharFormat cf = li.len && *pl.format ? **pl.format : format;
						cf.Height(z * cf.GetHeight());
						draw.DrawText(r.left,
						              z * y0 - cf.Info().GetAscent(),
						              s, cf, pi.coloroverride ? SColorText() : cf.ink);
					}
				}
			}
			int zlcy = z * linecy;
			if(pi.spellingchecker && zlcy > 3) {
				int x = z * (li.xpos + page.left);
				w = wd;
				wl = w + li.len;
				int i = li.pos;
				int q = z * (py.y + linecy);
				while(w < wl) {
					if(spellerror[pl.pos[i++]]) {
						if(zlcy > 16)
							draw.DrawRect(x, q - 3, *w, 1, Red);
						draw.DrawRect(x, q - 2, *w, 1, LtRed);
					}
					x += *w++;
				}
			}

			if(pi.selh == li.ppos + li.plen)
				h = x;
			if(pi.sell == li.ppos + li.plen)
				l = x;
			if(pi.sell < li.ppos)
				l = page.left;
			if(pi.selh > li.ppos + li.plen)
				h = page.right;
			if(pi.sell < pi.selh && pi.selh > li.ppos)
				draw.DrawRect(z * l, z * py.y, z * h - z * l,
							  z * (py.y + linecy) - z * py.y, InvertColor);
			ASSERT(draw.GetCloffLevel() == cloff);
		}
		else
			while(i < ilim) {
				if(hg->object)
					oi++;
				i++;
				hg++;
			}
		py.y += linecy;
	}
	Size sz = RichTextImg::EndParaChar().GetSize();
	if(sz.cy < z * lineascent && !IsNull(pi.showcodes))
		pw.Page(py.page).DrawImage(z * x, z * y0 - sz.cy,
		                           RichTextImg::EndParaChar(),
		                           format.indexentry.GetCount() ? pi.indexentry : pi.showcodes);
	if(format.newpage && !IsNull(pi.showcodes)) {
		Draw& w = pw.Page(opy.page);
		int wd = z * page.right - z * page.left;
		int step = w.Pixels() ? 8 : 50;
		int y = z * opy.y;
		for(int x = 0; x < wd; x += step)
			w.DrawRect(z * page.left + x, y, step >> 1, step >> 3, pi.showcodes);
	}
	if(pl.len >= pi.sell && pl.len < pi.selh && py < pi.bottom) {
		int top = z * py.y;
		pw.Page(py.page).DrawRect(z * page.left, top, z * page.right - z * page.left,
		                          z * min(py.y + format.after, page.bottom) - top, InvertColor);
	}
	if(pw.tracer)
		pw.tracer->EndParagraph(py);
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

RichCaret RichPara::GetCaret(int pos, const Rect& page, PageY py, int nbefore, int nline) const
{
	Lines pl = Begin(page, py, nbefore, nline);
	RichCaret pr;
	FontInfo fi = format.Info();
	pr.caretascent = fi.GetAscent();
	pr.caretdescent = fi.GetDescent();
	for(int lni = 0; lni < pl.GetCount(); lni++) {
		Line& li = pl[lni];
		int linecy = li.Sum();
		if(BreaksPage(py, pl, lni, page)) {
			py.y = page.top;
			py.page++;
		}
		pr.page = py.page;
		pr.top = py.y;
		pr.bottom = py.y + linecy;
		pr.lineascent = li.ascent;
		pr.line = lni;
		if(pos < li.ppos + li.plen) {
			int *w = pl.width + li.pos;
			int *p = pl.pos + li.pos;
			const CharFormat **i = pl.format + li.pos;
			const HeightInfo *h = pl.height + li.pos;
			int x = li.xpos + page.left;
			if(li.len && *i) {
				pr.caretascent = h->ascent;
				pr.caretdescent = h->descent;
			}
			while(pos > *p) {
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
		py.y += linecy;
	}
	const Line& li = pl.line.Top();
	pr.left = li.cx + li.xpos + page.left;
	pr.right = page.right;
	pr.caretdescent = min(pr.caretdescent, pr.Height() - pr.lineascent);
	return pr;
}

int RichPara::PosInLine(int x, const Rect& page, const Lines& pl, int lni) const
{
	const Line& li = pl[lni];
	const int *w = pl.width + li.pos;
	const int *wl = w + li.len;
	if(lni < pl.GetCount() - 1 && li.len > 0 && pl.text[li.pos + li.len - 1] == ' ')
		wl--;
	int xp = li.xpos + page.left;
	while(w < wl && xp + *w <= x)
		xp += *w++;
	int pos = (int)(w - pl.width);
	return pos < pl.clen ? pl.pos[pos] : pl.len;
}

int RichPara::GetPos(int x, PageY y, const Rect& page, PageY py, int nbefore, int nline) const
{
	Lines pl = Begin(page, py, nbefore, nline);
	if(pl.len)
		for(int lni = 0; lni < pl.GetCount(); lni++) {
			const Line& li = pl[lni];
			int linecy = li.Sum();
			if(BreaksPage(py, pl, lni, page)) {
				py.y = page.top;
				py.page++;
			}
			py.y += linecy;
			if(y < py || lni == pl.GetCount() - 1)
				return PosInLine(x, page, pl, lni);
		}
	return pl.len;
}

int RichPara::GetVertMove(int pos, int gx, const Rect& page, int dir) const
{
	Lines pl = FormatLines(page.Width());
	int lni;
	if(pos >= 0) {
		for(lni = 0; lni < pl.GetCount() - 1; lni++) {
			const Line& li = pl[lni];
			if(pos < li.ppos + li.plen)
				break;
		}
		lni += sgn(dir);
		if(lni < 0 || lni >= pl.GetCount())
			return -1;
	}
	else
		lni = dir < 0 ? pl.GetCount() - 1 : 0;
	return PosInLine(gx, page, pl, lni);
}

void  RichPara::GatherLabels(Vector<RichValPos>& info, const Rect& page, PageY py,
                             int pos, int nbefore, int nline) const
{
	Lines pl = Begin(page, py, nbefore, nline);
	WString ie;
	if(!pl.GetCount())
		return;
	if(BreaksPage(py, pl, 0, page)) {
		py.y = page.top;
		py.page++;
	}
	if(format.label.IsEmpty())
		return;
	RichValPos& f = info.Add();
	f.py = py;
	f.pos = pos;
	f.data = format.label.ToWString();
}

void  RichPara::GatherIndexes(Vector<RichValPos>& info, const Rect& page, PageY py,
                              int pos, int nbefore, int nline) const
{
	Lines pl = Begin(page, py, nbefore, nline);
	WString ie;
	for(int lni = 0; lni < pl.GetCount(); lni++) {
		Line& li = pl[lni];
		int linecy = li.Sum();
		if(BreaksPage(py, pl, lni, page)) {
			py.y = page.top;
			py.page++;
		}
		const CharFormat **i0 = pl.format + li.pos;
		const CharFormat **i = i0;
		const CharFormat **ilim = i + li.len;
		while(i < ilim) {
			if(*i && (*i)->indexentry != ie) {
				ie = (*i)->indexentry;
				if(!ie.IsEmpty()) {
					RichValPos& f = info.Add();
					f.py = py;
					f.pos = (int)(i - i0) + pos;
					f.data = ie;
				}
			}
			i++;
		}
		py.y += linecy;
	}
}

void FontHeightRound(Font& fnt, Zoom z)
{
	fnt.Height((fnt.GetHeight() * z.m + (z.d >> 1)) / z.d);
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

END_UPP_NAMESPACE
