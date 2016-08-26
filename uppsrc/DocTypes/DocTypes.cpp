#include "DocTypes.h"

namespace Upp {

#define LLOG(x)  // DLOG(x)
#define LDUMP(x) // DDUMP(x)

class BulletDisplay : public Display {
	void  Paint(Draw& w, const Rect& r, const Value& q, Color i, Color p, dword s) const {
		Size sz = r.Size();
		w.DrawEllipse(r.left, r.top + sz.cy / 4, sz.cx / 2, sz.cy / 2, i);
	}
};

class WhiteBulletDisplay : public Display {
	void  Paint(Draw& w, const Rect& r, const Value& q, Color i, Color p, dword s) const {
		Size sz = r.Size();
		w.DrawEllipse(r.left, r.top + sz.cy / 4, sz.cx / 2, sz.cy / 2, p, sz.cx / 8, i);
	}
};

class BoxBulletDisplay : public Display {
	void  Paint(Draw& w, const Rect& r, const Value& q, Color i, Color p, dword s) const {
		Size sz = r.Size();
		w.DrawRect(r.left, r.top + sz.cy / 4, sz.cx / 2, sz.cy / 2, i);
	}
};

class WhiteBoxBulletDisplay : public Display {
	void  Paint(Draw& w, const Rect& r, const Value& q, Color i, Color p, dword s) const {
		Size sz = r.Size();
		w.DrawRect(r.left, r.top + sz.cy / 4, sz.cx / 2, sz.cy / 2, i);
		w.DrawRect(r.left + sz.cx / 8, r.top + sz.cy / 4 + sz.cx / 8,
			       sz.cx / 2 - sz.cx / 4, sz.cy / 2 - sz.cx / 4, p);
	}
};

static Display& sSb() { static BulletDisplay d; return d; }
static Display& sWb() { static WhiteBulletDisplay d; return d; }
static Display& sBb() { static BoxBulletDisplay d; return d; }
static Display& sWd() { static WhiteBoxBulletDisplay d; return d; }

GLOBAL_VARP(PaintRect, Paragraph::StdBullet, (sSb()))
GLOBAL_VARP(PaintRect, Paragraph::WhiteBullet, (sWb()))
GLOBAL_VARP(PaintRect, Paragraph::BoxBullet, (sBb()))
GLOBAL_VARP(PaintRect, Paragraph::WhiteBoxBullet, (sWd()))

Paragraph& Paragraph::Cat(const String& s, Font font, Color color, const Value& val, int delta) {
	length += s.GetLength();
	Part& n = part.Add();
	n.text = s;
	n.font = font;
	n.color = color;
	n.value = val;
	n.delta = delta;
	return *this;
}

Paragraph& Paragraph::Cat(const String& s, Font font, Color color, int delta) {
	return Cat(s, font, color, Null, delta);
}

Paragraph& Paragraph::Cat(const char *s, Font font, Color color, int delta) {
	return Cat(String(s), font, color, delta);
}

Paragraph& Paragraph::Cat(const PaintRect& pr, int cx, int cy, const Value& val, Color color) {
	if(pr) {
		length++;
		Part& n = part.Add();
		n.pr = pr;
		n.sz = pr.RatioSize(cx, cy);
		n.value = val;
		n.color = color;
	}
	return *this;
}

Paragraph& Paragraph::Cat(const Paragraph& p) {
	part.Append(p.part);
	length += p.GetLength();
	style.before = max(style.before, p.style.before);
	style.after = max(style.after, p.style.after);
	return *this;
}

Paragraph& Paragraph::Bullet(const PaintRect& bullet, int indent, Size size, Color color) {
	style.bullet = bullet;
	style.bulletindent = indent;
	style.bulletsize = size;
	style.bulletcolor = color;
	return *this;
}

int  Paragraph::GetWidth(int zoom) const {
	const Part *pptr = part.Begin();
	const Part *plim = part.End();
	int cx = DocZoom(zoom, style.indent);
	while(pptr != plim) {
		if(pptr->pr)
			cx += pptr->pr && !pptr->sz.cx ? pptr->pr.GetStdSize().cx
			                               : DocZoom(zoom, pptr->sz.cx);
		else {
			FontInfo pf = Font(pptr->font).Height(DocZoom(zoom, pptr->font.GetHeight())).Info();
			const char *s = pptr->text;
			int n = pptr->text.GetLength();
			while(n--) {
				cx += pf[*s == 31 ? 32 : ToUnicode((byte) *s, CHARSET_DEFAULT)];
				s++;
			}
		}
		pptr++;
	}
	return cx + style.lm + style.rm + (style.bullet ? style.bulletindent : 0);
}

bool Paragraph::Format(ParaTypo& pfmt, int cx, int zoom) const {
	int len = length + !!style.indent;
	Buffer<char> chr(len);
	Buffer<int>  width(len);
	Buffer<ParaTypo::Part *> info(len);
	Buffer<ParaTypo::Part>   pinfo(part.GetCount() + !!style.indent);
	const Part *pptr = part.Begin();
	const Part *plim = part.End();
	ParaTypo::Part *pp = pinfo;
	char *cp = chr;
	int  *wp = width;
	ParaTypo::Part **ip = info;
	if(!IsNull(parafont)) {
		Font pf = parafont;
//		pf.Height(max(1, DocZoom(zoom, parafont.GetHeight())));
		int n = DocZoom(zoom, parafont.GetHeight());
		pf.Height(n ? n : 1);
		FontInfo f = pf.Info();
		pfmt.SetMin(f.GetAscent(), f.GetDescent(), f.GetExternal());
	}
/*	if(!IsNull(parafont)) {
		FontInfo f = w.GetFontInfo(parafont);
		pfmt.SetMin(DocZoom(zoom, f.GetAscent()),
			        DocZoom(zoom, f.GetDescent()),
					DocZoom(zoom, f.GetExternal()));
	}
*/	if(style.indent) {
		static Part dummy;
		*cp++ = ' ';
		*wp++ = DocZoom(zoom, style.indent);
		pp->Set(Arial(0), Black);
		pp->voidptr = &dummy;
		*ip++ = pp;
		pp++;
	}
	while(pptr != plim) {
		if(pptr->pr) {
			*cp++ = '@';
			Size sz = pptr->pr.GetStdSize();
			*wp++ = pp->width = pptr->pr && !pptr->sz.cx ? sz.cx : DocZoom(zoom, pptr->sz.cx);
			*ip++ = pp;
			pp->ascent = minmax(pptr->pr && !pptr->sz.cy ? sz.cy : DocZoom(zoom, pptr->sz.cy), 0, 1000);
			pp->descent = 0;
			pp->external = pp->overhang = 0;
			pp->color = pptr->color;
		}
		else {
			Font font = pptr->font;
			font.Height(DocZoom(zoom, pptr->font.GetHeight()));
			FontInfo pf = pp->Set(font, pptr->color);
			const char *s = pptr->text;
			int n = pptr->text.GetLength();
			while(n--) {
				*cp++ = *s;
				*wp++ = pf[*s == 31 ? 32 : ToUnicode((byte) *s, CHARSET_DEFAULT)];
				*ip++ = pp;
				s++;
			}
		}
		pp->voidptr = (void *)pptr;
		pp++;
		pptr++;
	}
	return pfmt.Format(style.align, len, chr, width, info, cx);
}

class Paragraph::ParaHeight : public ParaTypo {
protected:
	virtual bool Put();

public:
	int    cy;
};

bool  Paragraph::ParaHeight::Put() {
	LDUMP(ascent);
	LDUMP(descent);
	LDUMP(external);
	cy += ascent + descent + external;
	return false;
}

int  Paragraph::GetHeight(int zoom, int cx) const {
	ParaHeight pp;
	pp.cy = 0;
	cx -= DocZoom(zoom, style.lm + style.rm + style.bulletindent);
	if(cx <= 0) return 0;
	Format(pp, cx, zoom);
	LLOG("ParaHeight " << pp.cy);
	return pp.cy;
}

class Paragraph::ParaPaint : public ParaTypo {
public:
	Draw&  draw;
	int    zoom;
	int    posx;
	int    liney;
	int    lm;
	int    ymax;
	int    yl, yp;
	int    bh;
	Color  paper;

	void Put(int px, int py, Part *ip, Part **ibeg, Part **i, const char *text);

	virtual bool PutAligned(int lm);

	ParaPaint(Draw& w) : draw(w) {}
};

inline
void Paragraph::ParaPaint::Put(int px, int py, Part *ip, Part **ibeg, Part **i, const char *text) {
	if(!ip) return;
	const Paragraph::Part *p = (const Paragraph::Part *) ip->voidptr;
	if(p->pr)
		p->pr.Paint(draw, px, py - ip->ascent, ip->width, ip->ascent, ip->color, paper);
	else
		draw.DrawText(px, py - ip->ascent + DocZoom(zoom, p->delta),
		              text + (ibeg - info), ip->font, ip->color, i - ibeg);
}

bool Paragraph::ParaPaint::PutAligned(int alm) {
	if(yp == 0) bh = ascent + descent;
	if(liney + ascent + descent > ymax) return true;
	if(yp >= yl) {
		if(len) {
			Part **i = info;
			Part **ilim = info + len;
			Part **ibeg = i;
			Part  *ip = *ibeg;
			int   *w = width;
			int x = posx + alm + lm;
			int px = x;
			int py = liney + ascent;
			while(i < ilim) {
				if(*i != ip) {
					Put(px, py, ip, ibeg, i, text);
					px = x;
					ibeg = i;
					ip = *i;
				}
				x += *w++;
				i++;
			}
			Put(px, py, ip, ibeg, i, text);
		}
		liney += ascent + descent + external;
	}
	yp += ascent + descent + external;
	return false;
}

bool Paragraph::Paint(int zoom, Draw& w, int x, int y, int cx, int ymax, PaintInfo& pi,
                      Color paper) const {
	ParaPaint pp(w);
	int bi = style.bullet ? style.bulletindent : 0;
	pp.zoom = zoom;
	pp.posx = x;
	pp.liney = y;
	pp.ymax = ymax;
	pp.lm = DocZoom(zoom, style.lm + bi);
	pp.yl = pi.yl;
	pp.yp = 0;
	pp.paper = paper;
	cx -= DocZoom(zoom, style.lm + style.rm + bi);
	bool r = Format(pp, max(1, cx), zoom);
	if(pi.yl == 0 && pp.yp > 0 && style.bullet)
		style.bullet.Paint(w, x + DocZoom(zoom, style.lm), y,
		                   style.bulletsize.cx ? DocZoom(zoom, style.bulletsize.cx) : pp.bh,
		                   style.bulletsize.cy ? DocZoom(zoom, style.bulletsize.cy) : pp.bh,
						   style.bulletcolor, paper, 0);
	pi.ypos = pp.liney;
	pi.yl = pp.yp;
	return r;
}

int Paragraph::Paint(int zoom, Draw& w, int x, int y, int cx, Color paper) const {
	PaintInfo pi;
	Paint(zoom, w, x, y, cx, INT_MAX, pi, paper);
	return pi.ypos;
}

class Paragraph::ParaAngle : public ParaTypo {
public:
	Draw&  draw;
	int    posx;
	int    posy;
	int    y;
	int    lm;
	int    bh;
	double cosf, sinf;
	int    angle;

	void Put(int px, int py, Part *ip, Part **ibeg, Part **i, const char *text);

	virtual bool PutAligned(int lm);

	ParaAngle(Draw& w) : draw(w) {}
};

inline
void Paragraph::ParaAngle::Put(int x, int y, Part *ip, Part **ibeg, Part **i, const char *text) {
	if(!ip) return;
	const Paragraph::Part *p = (const Paragraph::Part *) ip->voidptr;
	if(p->pr) return;
	y -= ip->ascent;
	draw.DrawText(int(x * cosf + y * sinf + posx),
	              int(-x * sinf + y * cosf + posy),
	              angle,
		          text + (ibeg - info), ip->font, ip->color, i - ibeg);
}

bool Paragraph::ParaAngle::PutAligned(int alm) {
	if(y == 0) bh = ascent + descent;
	if(len) {
		Part **i = info;
		Part **ilim = info + len;
		Part **ibeg = i;
		Part  *ip = *ibeg;
		int   *w = width;
		int x = alm + lm;
		int px = x;
		int py = ascent + y;
		while(i < ilim) {
			if(*i != ip) {
				Put(px, py, ip, ibeg, i, text);
				px = x;
				ibeg = i;
				ip = *i;
			}
			x += *w++;
			i++;
		}
		Put(px, py, ip, ibeg, i, text);
	}
	y += ascent + descent + external;
	return false;
}

void Paragraph::Paint(int zoom, int angle, Draw& w, int x, int y, int cx) const {
	ParaAngle pp(w);
	int bi = style.bullet ? style.bulletindent : 0;
	double pan = 2 * M_PI * angle / 3600;
	pp.sinf = sin(pan);
	pp.cosf = cos(pan);
	pp.angle = angle;
	pp.posx = x;
	pp.posy = y;
	pp.y = 0;
	pp.lm = DocZoom(zoom, style.lm + bi);
	cx -= DocZoom(zoom, style.lm + style.rm + bi);
	bool r = Format(pp, max(1, cx), zoom);
/*	if(pp.yp > 0 && style.bullet)
		style.bullet.Paint(w, x + DocZoom(zoom, style.lm), y, DocZoom(zoom, style.bulletindent),
		                   pp.bh, Black, White, 0);*/
}

int    Paragraph::GetHeight(int cx) const {
	return GetHeight(1024, cx);
}

bool Paragraph::Paint(Draw& w, int x, int y, int cx, int ymax, PaintInfo& pi, Color paper) const {
	return Paint(1024, w, x, y, cx, ymax, pi, paper);
}

int  Paragraph::Paint(Draw& w, int x, int y, int cx, Color paper) const {
	return Paint(1024, w, x, y, cx, paper);
}

class Paragraph::ParaRect : public ParaTypo {
public:
	Draw&              draw;
	int                liney;
	int                lm;
	int                px;
	Vector<ValueRect>& vr;

	void Put(int x, int y, Part *ip, Part **ibeg, Part **i, const char *text);

	virtual bool PutAligned(int lm);

	ParaRect(Draw& w, Vector<ValueRect>& vr) : draw(w), vr(vr) {}
};

inline
void Paragraph::ParaRect::Put(int x, int y, Part *ip, Part **ibeg, Part **i, const char *text) {
	if(!ip) return;
	const Paragraph::Part *p = (const Paragraph::Part *) ip->voidptr;
	if(IsNull(p->value)) return;
	ValueRect& r = vr.Add();
	r.value = p->value;
	r.rect = RectC(px + x, y, p->pr ? ip->width
				              : GetTextSize(text + (ibeg - info), ip->font, i - ibeg).cx,
			       ascent + descent + external);
}

bool Paragraph::ParaRect::PutAligned(int alm) {
	if(len) {
		Part **i = info;
		Part **ilim = info + len;
		Part **ibeg = i;
		Part  *ip = *ibeg;
		int   *w = width;
		int x = alm + lm;
		int px = x;
		while(i < ilim) {
			if(*i != ip) {
				Put(px, liney, ip, ibeg, i, text);
				px = x;
				ibeg = i;
				ip = *i;
			}
			x += *w++;
			i++;
		}
		Put(px, liney, ip, ibeg, i, text);
	}
	liney += ascent + descent + external;
	return false;
}

void Paragraph::GetValueRects(int zoom, Draw& w, int x, int& y, int cx, Vector<ValueRect>& vr)
const {
	ParaRect pr(w, vr);
	int bi = style.bullet ? style.bulletindent : 0;
	pr.lm = DocZoom(zoom, style.lm + bi);
	pr.liney = y;
	pr.px = x;
	cx -= DocZoom(zoom, style.lm + style.rm + bi);
	Format(pr, max(1, cx), zoom);
	y = pr.liney;
}

void Paragraph::Style::Init() {
	align = ALIGN_LEFT;
	before = lm = rm = indent = after = 0;
	bulletindent = 0;
	bulletsize = Size(0, 0);
	bulletcolor = Black;
	bullet = PaintRect();
}

void Paragraph::Clear() {
	part.Clear();
	style.Init();
	length = 0;
	parafont = Null;
}

Paragraph::Paragraph(const Paragraph& p, int) : part(p.part, 1) {
	style = p.style;
	length = p.length;
	parafont = p.parafont;
}

Paragraph::Paragraph() {
	length = 0;
	style.Init();
	parafont = Null;
}

Table& Document::AddTable() {
	Item& m = item.Add();
	m.type = TABLE;
	return m.data;
}

Paragraph& Document::AddParagraph() {
	Item& m = item.Add();
	m.type = PARAGRAPH;
	Table& tab = m.data;
	tab.AddRow();
	return tab.AddCell().Par();
}

void Document::Cat(const Paragraph& p) {
	if(item.GetCount() && item.Top().type == PARAGRAPH)
		item.Top().data.LastCell().Par().Cat(p);
	else
		AddParagraph() <<= p;
}

int Document::GetHeight(int zoom, int cx) const {
	int cy = 0;
	for(int i = 0; i < item.GetCount(); i++) {
		const Item& m = item[i];
		if(m.type == PARAGRAPH) {
			const Paragraph& p = m.data.LastCell().Par();
			cy += DocZoom(zoom, p.GetBefore());
			cy += p.GetHeight(zoom, cx);
			cy += DocZoom(zoom, p.GetAfter());
		}
		else {
			cy += DocZoom(zoom, m.data.GetBefore());
			cy += m.data.GetHeight(zoom, cx);
			cy += DocZoom(zoom, m.data.GetAfter());
		}
	}
	return cy;
}

int Document::GetWidth(int zoom) const {
	int cx = 0;
	for(int i = 0; i < item.GetCount(); i++) {
		const Item& m = item[i];
		if(m.type == PARAGRAPH)
			cx = max(cx, m.data.LastCell().Par().GetWidth(zoom));
		else
			cx = max(cx, m.data.GetWidth(zoom));
	}
	return cx;
}

int Document::Paint(int zoom, Draw& w, int x, int y, int cx, Color paper) const {
	for(int i = 0; i < item.GetCount(); i++) {
		const Item& m = item[i];
		if(m.type == PARAGRAPH) {
			const Paragraph& p = m.data.LastCell().Par();
			y += DocZoom(zoom, p.GetBefore());
			y = p.Paint(zoom, w, x, y, cx, paper);
			y += DocZoom(zoom, p.GetAfter());
		}
		else {
			y += DocZoom(zoom, m.data.GetBefore());
			y = m.data.Paint(zoom, w, x, y, cx);
			y += DocZoom(zoom, m.data.GetAfter());
		}
	}
	return y;
}

Vector<ValueRect> Document::GetValueRects(int zoom, Draw& w, int x, int y, int cx) const {
	Vector<ValueRect> vr;
	for(int i = 0; i < item.GetCount(); i++) {
		const Item& m = item[i];
		if(m.type == PARAGRAPH) {
			const Paragraph& p = m.data.LastCell().Par();
			y += DocZoom(zoom, p.GetBefore());
			p.GetValueRects(zoom, w, 0, y, cx, vr);
			y += DocZoom(zoom, p.GetAfter());
		}
		else {
			y += DocZoom(zoom, m.data.GetBefore());
			m.data.GetValueRects(zoom, w, x, y, cx, vr);
			y += DocZoom(zoom, m.data.GetAfter());
		}
	}
	return vr;
}

int  Document::GetHeight(int zoom, int i, int cx, Document::Cache& cache) const {
	if(cache.cx == cx && cache.zoom == zoom && i < cache.cy.GetCount() && !IsNull(cache.cy[i]))
		return cache.cy[i];
	int cy;
	const Item& m = item[i];
	if(m.type == PARAGRAPH) {
		const Paragraph& p = m.data.LastCell().Par();
		cy = DocZoom(zoom, p.GetBefore());
		cy += p.GetHeight(zoom, cx);
		cy += DocZoom(zoom, p.GetAfter());
	}
	else {
		cy = DocZoom(zoom, m.data.GetBefore());
		cy += m.data.GetHeight(zoom, cx);
		cy += DocZoom(zoom, m.data.GetAfter());
	}
	if(cache.cx != cx || cache.zoom != zoom) {
		cache.Clear();
		cache.cx = cx;
		cache.zoom = zoom;
	}
	cache.cy.DoIndex(i, Null) = cy;
	return cy;
}

int  Document::GetHeight(int zoom, int cx, Cache& cache) const {
	int cy = 0;
	for(int i = 0; i < item.GetCount(); i++)
		cy += GetHeight(zoom, i, cx, cache);
	return cy;
}

void Document::Paint(int zoom, Draw& w, int x, int y, int cx, int cy, int y0,
					 Cache& cache, Color paper) const {
	int yp = 0;
	for(int i = 0; i < item.GetCount() && yp < y0 + cy; i++) {
		const Item& m = item[i];
		int h = GetHeight(zoom, i, cx, cache);
		int yo = yp + y - y0;
		if((yp += h) <= y0)
			continue;
		if(m.type == PARAGRAPH) {
			const Paragraph& p = m.data.LastCell().Par();
			p.Paint(zoom, w, x, yo + DocZoom(zoom, p.GetBefore()), cx, paper);
		}
		else
			m.data.Paint(zoom, w, x, yo + DocZoom(zoom, m.data.GetBefore()), cx);
	}
}

void Document::Qtf0(const char *s, bool append, byte charset) {
	QtfParser parser(s);
	for(;;) {
		switch(parser.Parse()) {
		case QtfParser::PARAGRAPH:
			if(append)
				Cat(parser.GetParagraph());
			else
				AddParagraph() = parser.PickParagraph();
			break;
		case QtfParser::TABLE:
			AddTable() = parser.PickTable();
			break;
		case QtfParser::END:
			return;
		}
		append = false;
	}
}

Document& Document::operator=(const char *s) {
	Clear();
	Qtf(s);
	return *this;
}

Document& Document::operator|=(const char *s) {
	AppendQtf(s, true);
	return *this;
}

Document::Document(const Document& p, int) : item(p.item, 1) {}

Document::Document() {}

Document::Document(const char *qtf) {
	Qtf(qtf);
}

void  DocOut::Qtf(const char *s) {
	QtfParser parser(s);
	for(;;) {
		switch(parser.Parse()) {
		case QtfParser::PARAGRAPH:
			Put(parser.GetParagraph());
			break;
		case QtfParser::TABLE:
			Put(parser.GetTable());
			break;
		case QtfParser::PAGE:
			Page();
			break;
		case QtfParser::END:
			return;
		}
	}
}

void DocOut::Put(const Document& doc) {
	for(int i = 0; i < doc.GetCount(); i++)
		if(doc.IsParagraph(i))
			Put(doc.GetParagraph(i));
		else
			Put(doc.GetTable(i));
}

}
