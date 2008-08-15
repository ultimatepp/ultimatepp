#include "PdfDraw.h"

NAMESPACE_UPP

#define LDUMP(x) // DUMP(x)
#define LLOG(x) // LOG(x)

#define PDF_COMPRESS
#define USE_TTF

void PdfDraw::Init(int pagecx, int pagecy, int _margin)
{
	Clear();
	margin = _margin;
	pgsz.cx = pagecx;
	pgsz.cy = pagecy;
	pageMMs = iscale(pgsz, 254, 6000);
	inchPixels = Size(600, 600);
	pagePixels = pgsz;
	pgsz += margin;
	sheetPixels = pgsz;
	pageOffset = Point(0, 0);
	pixels = false;
	actual_offset = Point(0, 0);
	actual_clip = Rect(pgsz);
}

void  PdfDraw::Clear()
{
	out.Clear();
	page.Clear();
	offset.Clear();
	out << "%PDF-1.3\n";
	out << "%\xf1\xf2\xf3\xf4\n\n";
}

int PdfDraw::BeginObj()
{
	offset.Add(out.GetLength());
	out << offset.GetCount() << " 0 obj\n";
	return offset.GetCount();
}

void PdfDraw::EndObj()
{
	out << "endobj\n\n";
}

void PdfDraw::PutRect(const Rect& rc)
{
	page << Pt(rc.left) << ' ' << Pt(pgsz.cy - rc.bottom) << ' '
		<< Pt(rc.Width()) << ' ' << Pt(rc.Height()) << " re\n";
}

int PdfDraw::PutStream(const String& data, const String& keys)
{
#ifdef PDF_COMPRESS
	String c = ZCompress(data);
	if(c.GetLength() < data.GetLength()) {
		BeginObj();
		out << "<< " << keys
		    << "/Length " << c.GetLength() << " "
		    << "/Length1 " << data.GetLength() << " "
		    << "/Filter /FlateDecode "
		    << " >>\n"
		    << "stream\r\n" << c << "\r\nendstream\n";
		EndObj();
		return offset.GetCount();
	}
#endif
	BeginObj();
	out << "<< " << keys << " /Length " << data.GetLength() <<
	       " /Length1 "<< data.GetLength() << " >>\n"
	    << "stream\r\n" << data << "\r\nendstream\n";
	EndObj();
	return offset.GetCount();
}

void PdfDraw::PutrgColor(Color rg)
{
	if(IsNull(rgcolor) || rg != rgcolor)
		page << PdfColor(rgcolor = rg) << " rg\n";
}

void PdfDraw::PutRGColor(Color RG)
{
	if(IsNull(RGcolor) || RG != RGcolor)
		page << PdfColor(RGcolor = RG) << " RG\n";
}

void PdfDraw::PutLineWidth(int lw)
{
	lw = max(Nvl(lw, 0), 5);
	if(linewidth != lw)
		page << Pt(linewidth = lw) << " w\n";
}

void PdfDraw::StartPage()
{
	offstack.Clear();
	clipstack.Clear();
	actual_offset = Point(0, 0);
	actual_clip = Rect(pgsz);
	rgcolor = RGcolor = Null;
	fontid = -1;
	textht = Null;
	linewidth = -1;
	if(margin)
		OffsetOp(Point(margin, margin));
}

void PdfDraw::EndPage()
{
	if(margin)
		EndOp();
	ASSERT(offstack.IsEmpty());
	PutStream(page);
	page.Clear();
}

void PdfDraw::BeginOp()
{
	offstack.Add(actual_offset);
	clipstack.Add(actual_clip);
	page << "q\n";
}

void PdfDraw::EndOp()
{
	fontid = -1;
	textht = Null;
	rgcolor = RGcolor = Null;
	linewidth = -1;
	page << "Q\n";
	if(offstack.GetCount()) {
		actual_offset = offstack.Pop();
		actual_clip = clipstack.Pop();
	}
	else { NEVER(); }
}

void PdfDraw::OffsetOp(Point p)
{
	offstack.Add(actual_offset);
	clipstack.Add(actual_clip);
	page << "q\n";
	if(p.x || p.y) {
		page << "1 0 0 1 " << Pt(p.x) << ' ' << Pt(-p.y) << " cm\n";
		actual_offset += p;
		actual_clip -= p;
	}
}

bool PdfDraw::ClipOp(const Rect& r)
{
	offstack.Add(actual_offset);
	clipstack.Add(actual_clip);
	page << "q ";
	PutRect(r);
	page << "W* n\n";
	actual_clip &= r;
	return !actual_clip.IsEmpty();
}

bool PdfDraw::ClipoffOp(const Rect& r)
{
	offstack.Add(actual_offset);
	clipstack.Add(actual_clip);
	actual_offset += r.TopLeft();
	actual_clip &= r;
	page << "q ";
	PutRect(actual_clip);
	actual_clip -= r.TopLeft();
	page << "W* n\n";
	if(r.left || r.top)
		page << "1 0 0 1 " << Pt(r.left) << ' ' << Pt(-r.top) << " cm\n";
	return !actual_clip.IsEmpty();
}

bool PdfDraw::ExcludeClipOp(const Rect& r)
{
	if(r.left <= actual_clip.left && r.right >= actual_clip.right) {
		if(r.top <= actual_clip.top) actual_clip.top = max(actual_clip.top, r.bottom);
		if(r.bottom >= actual_clip.bottom) actual_clip.bottom = min(actual_clip.bottom, r.top);
	}
	if(r.top <= actual_clip.top && r.bottom >= actual_clip.bottom) {
		if(r.left <= actual_clip.left) actual_clip.left = max(actual_clip.left, r.right);
		if(r.right >= actual_clip.right) actual_clip.right = min(actual_clip.right, r.left);
	}

	PutRect(actual_clip);
	PutRect(r & actual_clip);
	page << "W* n\n";
	return !actual_clip.IsEmpty();
}

bool PdfDraw::IntersectClipOp(const Rect& r)
{
	PutRect(r);
	page << "W* n\n";
	actual_clip &= r;
	return !actual_clip.IsEmpty();
}

Rect PdfDraw::GetClipOp() const
{
	return actual_clip;
}

bool PdfDraw::IsPaintingOp(const Rect&) const
{
	return true;
}

PdfDraw::CharPos PdfDraw::GetCharPos(Font fnt, wchar chr)
{
	fnt.Underline(false);
	VectorMap<wchar, CharPos>& fc = fontchars.GetAdd(fnt);
	int q = fc.Find(chr);
	if(q >= 0)
		return fc[q];
	CharPos& p = fc.Add(chr);
	q = pdffont.FindLast(fnt);
	if(q < 0 || pdffont[q].GetCount() > 250) {
		p.fi = pdffont.GetCount();
		p.ci = 0;
		pdffont.Add(fnt).Add(chr);
	}
	else {
		p.fi = q;
		p.ci = pdffont[q].GetCount();
		pdffont[q].Add(chr);
	}
	return p;
}

void  PdfDraw::FlushText(int dx, int fi, int height, const String& txt)
{
	if(fi < 0)
		return;
	if(dx)
		page << Pt(dx) << " 0 Td ";
	PutFontHeight(fi, height);
	page << "<" << txt << "> Tj\n";
}

String PdfDraw::PdfColor(Color c)
{
	return NFormat("%3nf %3nf %3nf", c.GetR() / 255.0, c.GetG() / 255.0, c.GetB() / 255.0);
}

void PdfDraw::PutFontHeight(int fi, double ht)
{
	if(fi != fontid || IsNull(textht) || ht != textht)
		page << "/F" << ((fontid = fi) + 1) << ' ' << Pt(textht = ht) << " Tf\n";
}

PdfDraw::OutlineInfo PdfDraw::GetOutlineInfo(Font fnt)
{
	fnt.Height(0);
	int q = outline_info.Find(fnt);
	if(q >= 0)
		return outline_info[q];
	OutlineInfo of;
	of.sitalic = of.ttf = false;

#ifdef USE_TTF
#ifdef PLATFORM_WIN32
	ScreenDraw info;
	info.SetFont(fnt().Underline(false));
	TEXTMETRIC tm;
	GetTextMetrics(info.GetHandle(), &tm);
	if(tm.tmPitchAndFamily & TMPF_TRUETYPE) {
		of.ttf = true;
		int c = GetOutlineTextMetrics(info.GetHandle(), 0, NULL);
		if(c > 0) {
			Buffer<byte> h(c);
			OUTLINETEXTMETRIC *otm = (OUTLINETEXTMETRIC *)~h;
			GetOutlineTextMetrics(info.GetHandle(), c, otm);
			of.sitalic = otm->otmItalicAngle == 0 && fnt.IsItalic();
		}
	}
#endif
#ifdef PLATFORM_X11
	FontInfo fi = fnt.Info();
	String fn = fi.GetFileName();
	String ext = ToLower(GetFileExt(fn));
	if(ext == ".ttf" || ext == ".otf") {
		String data = LoadFile(fn);
		TTFReader ttf;
		if(ttf.Open(data, false, true)) {
			of.ttf = true;
			of.sitalic = ttf.post.italicAngle == 0 && fnt.IsItalic();
		}
	}
#endif
#endif

	outline_info.Add(fnt, of);

	return of;
}

void PdfDraw::DrawTextOp(int x, int y, int angle, const wchar *s, Font fnt,
		                 Color ink, int n, const int *dx)
{
	if(!n) return;
	if(fnt.GetHeight() == 0)
		fnt.Height(100);
	FontInfo ff = fnt.Info();
	#ifdef PLATFORM_WIN32
	int fh = ff.GetHeight() - ff.GetInternal();//TODO
	#endif
	#ifdef PLATFORM_X11
	int fh = fnt.GetHeight();
	#endif
	OutlineInfo of = GetOutlineInfo(fnt);
	if(of.ttf)
		fnt.Height(0);
	String txt;
	PutrgColor(ink);
	PutRGColor(ink);
	page << "BT ";
	double sina = 0, cosa = 1;
	int posx = 0;
	if(angle || dx || (of.sitalic && !fnt.IsItalic())) {
		M22 m;
		if(of.sitalic)
			m.c = 0.165;
		if(angle) {
			Draw::SinCos(angle, sina, cosa);
			m.Mul(cosa, sina, -sina, cosa);
		}
		int fi = -1;
		bool straight = (fabs(m.a - 1) <= 1e-8 && fabs(m.b) <= 1e-8 && fabs(m.c) <= 1e-8 && fabs(m.d - 1) <= 1e-8);
		Pointf prev(0, 0);
		for(int i = 0; i < n; i++) {
			Pointf next(Pt(x + posx * cosa + fround(ff.GetAscent() * sina)),
				Pt(pgsz.cy - (y - posx * sina) - fround(ff.GetAscent() * cosa)));
			CharPos fp = GetCharPos(fnt, s[i]);
			if(fi != fp.fi) {
				fi = fp.fi;
				PutFontHeight(fi, fh);
			}
			if(straight)
				page << (next.x - prev.x) << ' ' << (next.y - prev.y) << " Td";
			else
				page << m.a << ' ' << m.b << ' ' << m.c << ' ' << m.d << ' ' << next.x << ' ' << next.y << " Tm";
			page << " <" << FormatIntHex(fp.ci, 2);
			while(i + 1 < n) {
				int cw = ff[s[i]];
				if(dx && dx[i] != cw)
					break;
				posx += cw;
				CharPos np = GetCharPos(fnt, s[i + 1]);
				if(np.fi != fp.fi)
					break;
				page << FormatIntHex(np.ci, 2);
				i++;
			}
			page << "> Tj\n";
			posx += dx ? dx[i] : ff[s[i]];
			prev = next;
		}
	}
	else {
		page << Pt(x) << " " << Pt(pgsz.cy - y - ff.GetAscent()) << " Td\n";
		int fi = -1;
		int ppos = 0;
		int np = 0;
		for(int i = 0; i < n; i++) {
			CharPos fp = GetCharPos(fnt, s[i]);
			if(fp.fi != fi) {
				FlushText(np, fi, fnt.GetHeight(), txt);
				txt.Clear();
				np = posx - ppos;
				ppos = posx;
				fi = fp.fi;
			}
			txt.Cat(Sprintf("%02.2X", fp.ci));
			posx += ff[s[i]];
		}
		FlushText(np, fi, fh, txt);
	}
	page << "ET\n";
	if(fnt.IsUnderline()) {
		int w = ff.GetAscent() / 15;
		int dy = ff.GetAscent() + max((ff.GetDescent() - w) / 2, ff.GetAscent() / 10);
		DrawLine(fround(x + sina * dy),
		         fround(y + cosa * dy),
		         fround(x + cosa * posx + sina * dy),
		         fround(y + cosa * dy - sina * posx), w, ink);
	}
}

Image RenderGlyph(int cx, int x, Font font, int chr, int py, int pcy)
{
	ImageDraw iw(cx, pcy);
	iw.DrawRect(0, 0, cx, pcy, White);
	iw.DrawText(x, -py, WString(chr, 1), font, Black);
	return iw;
}

PdfDraw::RGlyph PdfDraw::RasterGlyph(Font fnt, int chr)
{
	RGlyph rg;
	FontInfo fi = fnt.Info();
	rg.x = 0;
	rg.sz.cx = fi[chr];
	rg.sz.cy = fi.GetHeight();
	int l = fi.GetLeftSpace(chr);
	if(l < 0) {
		rg.x = -l;
		rg.sz.cx -= l;
	}
	int r = fi.GetRightSpace(chr);
	if(r < 0)
		rg.sz.cx -= r;
	RasterFormat fmt;
	fmt.Set1mf();
	int linebytes = fmt.GetByteCount(rg.sz.cx);
	Buffer<byte> ob(linebytes);
	int y = 0;
	while(y < rg.sz.cy) {
		int ccy = min(16, rg.sz.cy - y);
		Image m = RenderGlyph(rg.sz.cx, rg.x, fnt, chr, y, ccy);
		for(int i = 0; i < m.GetHeight(); i++) {
			fmt.Write(ob, m[i], rg.sz.cx, NULL);
			rg.data.Cat((const char *)~ob, linebytes);
		}
		y += ccy;
	}
	return rg;
}

void PdfDraw::DrawRectOp(int x, int y, int cx, int cy, Color color)
{
	if(IsNull(color) || cx <= 0 || cy <= 0) return;
	PutrgColor(color);
	PutRGColor(color);
	PutRect(RectC(x, y, cx, cy));
	page << "f\n";
}

void PdfDraw::DrawLineOp(int x1, int y1, int x2, int y2, int width, Color color)
{
	if(!IsNull(width)) {
		PutRGColor(color);
		PutLineWidth(width);
		page << " 1 J "
		     << Pt(x1) << ' ' << Pt(pgsz.cy - y1) << " m "
		     << Pt(x2) << ' ' << Pt(pgsz.cy - y2) << " l S\n";
	}
}

void PdfDraw::DrawImageOp(int x, int y, int cx, int cy, const Image& _img, const Rect& src, Color c)
{
	Image img = _img;
	if(!IsNull(c))
		img = SetColorKeepAlpha(img, c);
	image.Add(img);
	imagerect.Add(src);
	page << "q "
	     << Pt(cx) << " 0 0 " << Pt(cy) << ' '
	     << Pt(x) << ' ' << Pt(pgsz.cy - y - cy)
	     << " cm /Image" << image.GetCount() << " Do Q\n";
}

void PdfDraw::DrawPolyPolylineOp(const Point *vertices, int vertex_count,
	                    const int *counts, int count_count,
	                    int width, Color color, Color doxor)
{
	if(IsNull(color) || IsNull(width))
		return;
	PutRGColor(color);
	PutLineWidth(width);
	while(--count_count >= 0) {
		int part = *counts++;
		page << Pt(vertices->x) << ' ' << Pt(pgsz.cy - vertices->y) << " m\n";
		vertices++;
		for(; --part > 0; vertices++)
			page << Pt(vertices->x) << ' ' << Pt(pgsz.cy - vertices->y) << " l\n";
		page << "S\n";
	}
}

void PdfDraw::DrawEllipseOp(const Rect& r, Color color, int pen, Color outline)
{
	bool fill = !IsNull(color), stroke = !IsNull(outline) && !IsNull(pen);
	if(fill) PutrgColor(color);
	if(stroke) { PutRGColor(outline); PutLineWidth(pen); }
	if(!fill && !stroke)
		return;
	char closeop = (stroke && fill ? 'B' : fill ? 'f' : 'S');
	int sizelim = r.Width() | r.Height();
	int bits = 1;
	while(bits < 20 && sizelim > (1 << bits))
		bits++;
	int parts = 3 + max(((bits * (bits - 6)) >> 3) | 1, 1);
	double k = 2 * M_PI / 3 / parts;
	Pointf center(Pt((r.left + r.right) / 2.0), Pt(pgsz.cy - ((r.top + r.bottom) / 2.0)));
	Pointf size(Pt(r.Width() / 2.0), Pt(r.Height() / 2.0));
	for(int partid = 0; partid < parts; partid++) {
		double phi1 = (partid + 0) * (2 * M_PI / parts);
		double phi2 = (partid + 1) * (2 * M_PI / parts);
		double s1 = sin(phi1), c1 = cos(phi1), s2 = sin(phi2), c2 = cos(phi2);
		Pointf A = center + Pointf(c1, s1) * size;
		Pointf D = center + Pointf(c2, s2) * size;
		Pointf dA = Pointf(-s1, c1) * size;
		Pointf dD = Pointf(-s2, c2) * size;
		Pointf B = A + k * dA;
		Pointf C = D - k * dD;
		if(!partid)
			page << FormatDoubleFix(A.x, 2) << ' ' << FormatDoubleFix(A.y, 2) << " m\n";
		page << FormatDoubleFix(B.x, 2) << ' ' << FormatDoubleFix(B.y, 2) << ' '
			<< FormatDoubleFix(C.x, 2) << ' ' << FormatDoubleFix(C.y, 2) << ' '
			<< FormatDoubleFix(D.x, 2) << ' ' << FormatDoubleFix(D.y, 2) << " c\n";
	}
	page << closeop << '\n';
}

void PdfDraw::DrawArcOp(const Rect& rc, Point start, Point end, int width, Color color)
{
	//TODO!!
	NEVER();
}

void PdfDraw::DrawPolyPolyPolygonOp(const Point *vertices, int vertex_count,
	const int *subpolygon_counts, int subpolygon_count_count,
	const int *disjunct_polygon_counts, int disjunct_polygon_count_count,
	Color color, int width, Color outline, uint64 pattern, Color doxor)
{
	bool fill = !IsNull(color), stroke = !IsNull(outline) && !IsNull(width);
	if(fill)   PutrgColor(color);
	if(stroke) { PutRGColor(outline); PutLineWidth(width); }
	if(!fill && !stroke) return;
	const char *closeop = (fill & stroke ? "B*" : fill ? "f*" : "S");
	while(--disjunct_polygon_count_count >= 0) {
		int disj = *disjunct_polygon_counts++;
		while(disj > 0) {
			int sub = *subpolygon_counts++;
			disj -= sub;
			page << Pt(vertices->x) << ' ' << Pt(pgsz.cy - vertices->y) << " m\n";
			vertices++;
			for(; --sub > 0; vertices++)
				page << Pt(vertices->x) << ' ' << Pt(pgsz.cy - vertices->y) << " l\n";
			page << "h\n";
		}
		page << closeop << '\n';
	}
}

String PdfDraw::Finish()
{
	if(!IsNull(page))
		PutStream(page);

	int pagecount = offset.GetCount();

	Vector<int> imageobj;
	for(int i = 0; i < image.GetCount(); i++) {
		Size sz = image[i].GetSize();
		Rect sr = sz & imagerect[i];
		String data;
		const Image& m = image[i];
		int mask = -1;
		int smask = -1;
		if(m.GetKind() == IMAGE_MASK) {
			for(int y = sr.top; y < sr.bottom; y++) {
				const RGBA *p = m[y] + sr.left;
				const RGBA *e = m[y] + sr.right;
				while(p < e) {
					int bit = 0x80;
					byte b = 0;
					while(bit && p < e) {
						if(p->a != 255)
							b |= bit;
						bit >>= 1;
						p++;
					}
					data.Cat(b);
				}
			}
			mask = PutStream(data, String().Cat()
			                    << "/Type /XObject /Subtype /Image /Width " << sr.Width()
				                << " /Height " << sr.Height()
				                << " /BitsPerComponent 1 /ImageMask true /Decode [0 1]");
		}
		if(m.GetKind() == IMAGE_ALPHA) {
			for(int y = sr.top; y < sr.bottom; y++) {
				const RGBA *p = m[y] + sr.left;
				const RGBA *e = m[y] + sr.right;
				while(p < e)
					data.Cat((p++)->a);
			}
			smask = PutStream(data, String().Cat()
			                    << "/Type /XObject /Subtype /Image /Width " << sr.Width()
				                << " /Height " << sr.Height()
				                << " /BitsPerComponent 8 /ColorSpace /DeviceGray /Decode [0 1]");
		}
		data.Clear();
		for(int y = sr.top; y < sr.bottom; y++) {
			const RGBA *p = m[y] + sr.left;
			const RGBA *e = m[y] + sr.right;
			while(p < e) {
				data.Cat(p->r);
				data.Cat(p->g);
				data.Cat(p->b);
				p++;
			}
		}
		String imgobj;
		imgobj << "/Type /XObject /Subtype /Image /Width " << sr.Width()
		       << " /Height " << sr.Height() << " /BitsPerComponent 8 /ColorSpace /DeviceRGB";
		if(mask >= 0)
			imgobj << " /Mask " << mask << " 0 R";
		if(smask >= 0)
			imgobj << " /SMask " << smask << " 0 R";
		imageobj << PutStream(data, imgobj);
	}

/*
	Vector<int>  rgobj;
	Vector<Size> rgsz;
	Vector<int>  rgx;
	for(int i = 0; i < pdffont.GetCount(); i++) {
		Font fnt = pdffont.GetKey(i);
		if(fnt.GetHeight()) {
			for(int c = 0; c < pdffont[i].GetCount(); c++) {
				RGlyph rg = RasterGlyph(fnt, pdffont[i][c]);
				int ii = rgobj.GetCount();
				rgobj << PutStream(rg.data, String().Cat()
				                   << "/Type /XObject /Subtype /Image "
				                   << " /Width " << rg.sz.cx << " /Height " << rg.sz.cy
					               << " /BitsPerComponent 1 /ImageMask true /Decode [0 1]");
				rgsz.Add(rg.sz);
				rgx.Add(rg.x);
			}
		}
	}
*/
	Vector<int> fontobj;
	for(int i = 0; i < pdffont.GetCount(); i++) {
		Font fnt = pdffont.GetKey(i);
		const Vector<wchar>& cs = pdffont[i];
		String cmap;
		cmap <<
			"/CIDInit /ProcSet findresource begin\n"
			"12 dict begin\n"
			"begincmap\n"
			"/CIDSystemInfo\n"
			"<< /Registry (Adobe)\n"
			"/Ordering (UCS)\n"
			"/Supplement 0\n"
			">> def\n"
			"/CMapName /UCS" << i << " def\n"
			"/CMapType 2 def\n"
			"1 begincodespacerange\n"
			"<00> <" << FormatIntHex(cs.GetCount() - 1, 2) << ">\n"
			"endcodespacerange\n"
			"1 beginbfrange\n"
			"<00> <" << FormatIntHex(cs.GetCount() - 1, 2) << ">\n"
			"[\n";
		for(int c = 0; c < cs.GetCount(); c++)
			cmap << '<' << FormatIntHex(cs[c], 4) << ">\n";
		cmap <<
			"]\n"
			"endbfrange\n"
			"endcmap\n"
			"CMapName currentdict /CMap defineresource pop\n"
			"end\n"
			"end\n";
		int cmapi = PutStream(cmap);
		if(fnt.GetHeight()) {
			FontInfo fi = fnt.Info();
			int t3ch = offset.GetCount() + 1;
			int fa = fi.GetHeight() - fi.GetInternal();
			String res;
			for(int c = 0; c < cs.GetCount(); c++) {
				RGlyph rg = RasterGlyph(fnt, pdffont[i][c]);
				String proc;
				proc
					<< 1000 * fi[cs[c]] / fa << " 0 0 "
					<< -1000 * fi.GetDescent() / fa << ' '
					<< 1000 * (rg.sz.cx + rg.x) / fa << ' '
					<< 1000 * rg.sz.cy / fa
					<< " d1\nq "
					<< 1000 * rg.sz.cx / fa
					<< " 0 0 "
					<< 1000 * rg.sz.cy / fa
					<< " "
					<< -1000 * rg.x / fa
					<< " "
					<< -1000 * fi.GetDescent() / fa
					<< " cm BI /W " << rg.sz.cx << " /H " << rg.sz.cy
					<< " /BPC 1 /IM true /D [0 1] ID\n"
					<< rg.data
					<< "\nEI Q"
				;
				PutStream(proc);
			}
			int charprocs = BeginObj();
			out << "<<";
			for(int c = 0; c < cs.GetCount(); c++)
				out << " /Rgch" << c << ' ' << t3ch + c<< " 0 R";
			out << " >>\n";
			EndObj();
			int encoding = BeginObj();
			out << "<< /Type /Encoding /Differences [0";
			for(int c = 0; c < cs.GetCount(); c++)
				out << " /Rgch" << c;
			out << "] >>\n";
			EndObj();
			fontobj.Add() = BeginObj();
			out <<
				"<< /Name /F" << i + 1 <<
				" /Type /Font\n"
				"/Subtype /Type3\n"
				"/FontBBox [0 0 0 0]\n"
				"/FontMatrix [0.001 0 0 0.001 0 0]\n"
				"/CharProcs " << charprocs << " 0 R\n"
				"/Encoding " << encoding << " 0 R\n"
				"/FirstChar 0\n"
				"/LastChar " << cs.GetCount() - 1 << "\n"
				"/Widths [";
			for(int i = 0; i < cs.GetCount(); i++)
				out << ' ' << 1000 * fi[cs[i]] / fa;
			out <<
				"]\n";
			out << "/Resources << /ProcSet [ /PDF /Text /ImageB  ] >>\n"
			    << "/FirstChar 0 /LastChar " << cs.GetCount() - 1 <<" /ToUnicode "
				<< cmapi
				<< " 0 R\n>>\n";
			EndObj();
		}
		else {
	#ifdef PLATFORM_POSIX
			FontInfo fi = pdffont.GetKey(i).Info();
			String fontbuffer = LoadFile(fi.GetFileName());
	#endif
	#ifdef PLATFORM_WIN32
			ScreenDraw sd;
			sd.SetFont(pdffont.GetKey(i));

			int size = GetFontData(sd.GetHandle(), 0, 0, NULL, 0);
			if(size == GDI_ERROR) {
				LLOG("PdfDraw::Finish: GDI_ERROR on font " << pdffont.GetKey(i));
				return Null;
			}
			StringBuffer fontbuffer(size);
			GetFontData(sd.GetHandle(), 0, 0, fontbuffer, size);
	#endif

			TTFReader ttf;
			if(!ttf.Open(fontbuffer))
				return Null;


			String name = FormatIntAlpha(i + 1, true);
			name.Cat('A', 6 - name.GetLength());
			name << '+' << ttf.ps_name;

			int fonti = PutStream(ttf.Subset(cs));

			BeginObj();
			int ascent = ttf.hhea.ascent * 1000 / ttf.head.unitsPerEm;
			int descent = ttf.hhea.descent * 1000 / ttf.head.unitsPerEm;
			out <<
				"<< /Type /FontDescriptor\n"
				"/FontName /" << name << "\n"
				"/Flags 4\n"
				"/FontBBox [ -1000 " << descent << " 3000 " << ascent << " ]\n" //?????
				"/ItalicAngle " << ttf.post.italicAngle / 65536.0 << "\n"
				"/Ascent " << ascent << "\n"
				"/Descent " << -descent << "\n"
				"/CapHeight " << ttf.hhea.ascent * 1000 / ttf.head.unitsPerEm << "\n"
				"/StemV 80\n"
				"/FontFile2 " << fonti << " 0 R\n" <<
				">>\n";
			EndObj();

			fontobj.Add() = BeginObj();
			out <<
				"<< /Type /Font\n"
				"/Subtype /TrueType\n"
				"/BaseFont /" << name << "\n"
				"/FirstChar 0\n"
				"/LastChar " << cs.GetCount() - 1 << "\n"
				"/Widths [ ";
			for(int i = 0; i < cs.GetCount(); i++)
				out << ttf.GetAdvanceWidth(cs[i]) * 1000 / ttf.head.unitsPerEm << ' ';
			out <<
				"]\n"
				"/FontDescriptor " << fonti + 1 << " 0 R\n"
				"/ToUnicode " << cmapi << " 0 R\n" <<
				">>\n";
			EndObj();
		}
	}

	int pages = BeginObj();
	out << "<< /Type /Pages\n"
	    << "/Kids [";
	for(int i = 0; i < pagecount; i++)
		out << i + pages + 1 << " 0 R ";
	out << "]\n"
	    << "/Count " << pagecount << "\n"
	    << "/Resources << /ProcSet [ /PDF /Text /ImageB /ImageC ]\n";
	if(pdffont.GetCount()) {
	    out << "/Font << ";
		for(int i = 0; i < pdffont.GetCount(); i++)
			out << "/F" << i + 1 << ' ' << fontobj[i] << " 0 R ";
		out << ">>\n";
	}
	if(!imageobj.IsEmpty()) {
		out << "/XObject << ";
		for(int i = 0; i < imageobj.GetCount(); i++)
			out << "/Image" << i + 1 << ' ' << imageobj[i] << " 0 R ";
		out << ">>\n";
	}
	out << ">> >>\n";
	EndObj();
	for(int i = 0; i < pagecount; i++) {
		BeginObj();
		out << "<< /Type /Page\n"
		    << "/Parent " << pages << " 0 R\n"
		    << "/MediaBox [0 0 " << Pt(pgsz.cx) << ' ' << Pt(pgsz.cy) << "]\n"
		    << "/Contents " << i + 1 << " 0 R\n"
		    << ">>\n";
		EndObj();
	}
	int outlines = BeginObj();
	out << "<< /Type /Outlines\n"
	       "/Count 0\n"
	       ">>\n";
	EndObj();
	int catalog = BeginObj();
	out << "<< /Type /Catalog\n"
	    << "/Outlines " << outlines << " 0 R\n"
	    << "/Pages " << pages << " 0 R\n"
	    << ">>\n";
	EndObj();
	int startxref = out.GetCount();
	out << "xref\n"
	    << "0 " << offset.GetCount() + 1 << "\n";
	out << "0000000000 65535 f\r\n";
	for(int i = 0; i < offset.GetCount(); i++)
		out << Sprintf("%010d 00000 n\r\n", offset[i]);
	out << "\n"
	    << "trailer\n"
	    << "<< /Size " << offset.GetCount() + 1 << "\n"
	    << "/Root " << catalog << " 0 R\n"
	    << ">>\n"
	    << "startxref\r\n"
	    << startxref << "\r\n"
	    << "%%EOF\r\n";
	return out;
}

END_UPP_NAMESPACE
