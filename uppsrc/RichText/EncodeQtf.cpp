#include "RichText.h"

namespace Upp {

extern Color (*QTFColor[])();
extern int   QTFFontHeight[];

void SeparateNumber(String& s)
{
	if(*s.Last() >= '0' && *s.Last() <= '9')
		s.Cat(';');
}

String QtfFormat(Color c)
{
	if(IsNull(c)) return "N";
	for(int i = 0; i < 10; i++)
		if((*QTFColor[i])() == c)
			return String(i + '0', 1);
	if(c.GetR() == c.GetG() && c.GetG() == c.GetB())
		return Sprintf("(%d)", c.GetR());
	return Sprintf("(%d.%d.%d)", c.GetR(), c.GetG(), c.GetB());
}

void LngFmt(String& fmt, dword l, dword lang)
{
	if(lang != (dword)l) {
		if(l == 0)
			fmt << "%-";
		else
		if(l == LNG_ENGLISH)
			fmt << "%%";
		else
			fmt << "%" << LNGAsText(l);
	}
}

void CharFmt(String& fmt, const RichPara::CharFormat& a, const RichPara::CharFormat& b)
{
	if(a.IsBold() != b.IsBold()) fmt.Cat('*');
	if(a.IsItalic() != b.IsItalic()) fmt.Cat('/');
	if(a.IsUnderline() != b.IsUnderline()) fmt.Cat('_');
	if(a.IsStrikeout() != b.IsStrikeout()) fmt.Cat('-');
	if(a.IsNonAntiAliased() != b.IsNonAntiAliased()) fmt.Cat('t');
	if(a.capitals != b.capitals) fmt.Cat('c');
	if(a.dashed != b.dashed) fmt.Cat('d');
	if(a.sscript != b.sscript)
		fmt.Cat(b.sscript == 0 ? a.sscript == 1 ? '`' : ',' :
		        b.sscript == 1 ? '`' : ',');
	if(a.GetFace() != b.GetFace())
		switch(b.GetFace()) {
		case Font::ARIAL:     fmt.Cat('A'); break;
		case Font::ROMAN:     fmt.Cat('R'); break;
		case Font::COURIER:   fmt.Cat('C'); break;
		case Font::STDFONT:   fmt.Cat('G'); break;
	#ifdef PLATFORM_WIN32
		case Font::SYMBOL:    fmt.Cat('S'); break;
	#endif
		default:
			fmt << "!" << b.GetFaceName() << "!";
		}
	if(a.link != b.link)
		fmt << '^' << DeQtf(b.link) << '^';
	if(a.indexentry != b.indexentry)
		fmt << 'I' << DeQtf(ToUtf8(b.indexentry)) << ';';
	if(a.ink != b.ink)
		fmt << "@" << QtfFormat(b.ink);
	if(a.paper != b.paper)
		fmt << "$" << QtfFormat(b.paper);
	if(a.GetHeight() != b.GetHeight()) {
		for(int i = 0; i < 10; i++)
			if(b.GetHeight() == QTFFontHeight[i]) {
				SeparateNumber(fmt);
				fmt.Cat('0' + i);
				return;
			}
		fmt.Cat(Format("+%d", b.GetHeight()));
	}
}

void FmtNumber(String& qtf, char c, int former, int current)
{
	qtf << (former != current ? Format("%c%d;", c, current) : String());
}

void QTFEncodeParaFormat(String& qtf, const RichPara::Format& format, const RichPara::Format& style)
{
	if(format.align != style.align)
		switch(format.align) {
		case ALIGN_LEFT:    qtf << '<'; break;
		case ALIGN_RIGHT:   qtf << '>'; break;
		case ALIGN_CENTER:  qtf << '='; break;
		case ALIGN_JUSTIFY: qtf << '#'; break;
		}
	FmtNumber(qtf, 'l', style.lm, format.lm);
	FmtNumber(qtf, 'r', style.rm, format.rm);
	FmtNumber(qtf, 'i', style.indent, format.indent);
	FmtNumber(qtf, 'H', style.ruler, format.ruler);
	if(style.rulerink != format.rulerink)
		qtf << "h" << QtfFormat(format.rulerink);
	FmtNumber(qtf, 'L', style.rulerstyle, format.rulerstyle);
	FmtNumber(qtf, 'b', style.before, format.before);
	FmtNumber(qtf, 'a', style.after, format.after);
	if(style.newpage != format.newpage)
		qtf << 'P';
	if(style.firstonpage != format.firstonpage)
		qtf << 'F';
	if(style.keep != format.keep)
		qtf << 'k';
	if(style.keepnext != format.keepnext)
		qtf << 'K';
	if(style.orphan != format.orphan)
		qtf << 'Q';
	if(style.linespacing != format.linespacing)
		switch(format.linespacing) {
		case RichPara::LSP115: qtf << "pw"; break;
		case RichPara::LSP15: qtf << "ph"; break;
		case RichPara::LSP20: qtf << "pd"; break;
		default:              qtf << "po"; break;
		}
	if(style.bullet != format.bullet) {
		qtf << 'O';
		switch(format.bullet) {
		case RichPara::BULLET_NONE:        qtf << '_'; break;
		case RichPara::BULLET_ROUNDWHITE:  qtf << '1'; break;
		case RichPara::BULLET_BOX:         qtf << '2'; break;
		case RichPara::BULLET_BOXWHITE:    qtf << '3'; break;
		case RichPara::BULLET_TEXT:        qtf << '9'; break;
		default:                           qtf << '0'; break;
		}
		qtf << ';';
	}
	if(!IsEmpty(format.label))
		qtf << ':' << DeQtf(format.label) << ':';
	if(style.newhdrftr != format.newhdrftr)
		qtf << "tP" << format.header_qtf << "^^" << format.footer_qtf << "^^";

	if(NumberingDiffers(style, format)) {
		if(format.before_number != style.before_number) {
			qtf << "n";
			qtf << DeQtf(format.before_number);
			qtf << ';';
		}
		if(format.after_number != style.after_number) {
			qtf << "m";
			qtf << DeQtf(format.after_number);
			qtf << ";";
		}
		int l;
		for(l = 7; l >= 0; --l)
			if(format.number[l])
				break;
		qtf << "N";
		for(int i = 0; i <= l; i++) {
			static char h[] = { '-', '1', '0', 'a', 'A', 'i', 'I' };
			if(format.number[i] <= RichPara::NUMBER_I)
				qtf << h[format.number[i]];
			else
				qtf << '-';
			}
		if(format.reset_number)
			qtf << '!';
		else
			qtf << ';';
	}
	FmtNumber(qtf, 't', style.tabsize, format.tabsize);
	if(style.tab != format.tab) {
		qtf << "~~";
		int i;
		for(i = 0; i < format.tab.GetCount(); i++) {
			RichPara::Tab tab = format.tab[i];
			qtf << '~';
			if(tab.align == ALIGN_RIGHT)
				qtf << '>';
			if(tab.align == ALIGN_CENTER)
				qtf << '=';
			if(tab.fillchar == 1)
				qtf << '.';
			if(tab.fillchar == 2)
				qtf << '-';
			if(tab.fillchar == 3)
				qtf << '_';
			qtf << tab.pos;
		}
		if(i)
			qtf << ';';
	}
}

bool s_nodeqtf[128];

void QTFEncodePara(String& qtf, const RichPara& p, const RichPara::Format& style, byte charset, dword lang, bool crlf)
{
	int d = qtf.GetLength();
	QTFEncodeParaFormat(qtf, p.format, style);
	if(p.part.GetCount()) {
		dword l = p.part.Top().format.language;
		LngFmt(qtf, l, lang);
		lang = l;
	}
	else {
		CharFmt(qtf, style, p.format);
		LngFmt(qtf, p.format.language, lang);
	}
	qtf.Cat(' ');
	d = qtf.GetLength() - d;
	for(int i = 0; i < p.part.GetCount(); i++) {
		const RichPara::Part& part = p.part[i];
		String cf;
		LngFmt(cf, part.format.language, lang);
		CharFmt(cf, style, part.format);
		if(!cf.IsEmpty()) {
			qtf << '[' << cf << ' ';
			d += cf.GetLength();
		}
		if(part.field) {
			if(crlf)
				qtf << "\r\n";
			qtf << "{:" + DeQtf(part.field.ToString()) + ":" + DeQtf(part.fieldparam) + ":}";
			if(crlf)
				qtf << "\r\n";
			d = 0;
		}
		else
		if(part.object) {
			const RichObject& object = part.object;
			Size sz = object.GetSize();
			if(crlf)
				qtf << "\r\n";
			qtf << "@@" << object.GetTypeName() << ':' << sz.cx
			    << (object.IsKeepRatio() ? '&' : '*') << sz.cy;
			if(object.GetYDelta())
				qtf << '/' << object.GetYDelta();
			String data = object.Write();
			const char *q = data.Begin();
			const char *slim = data.End();
			qtf.Reserve(8 * data.GetLength() / 7);
			if(object.IsText()) {
				qtf << "`";
				while(q < slim) {
					ASSERT((byte)*q >= 32);
					if(*q == '`')
						data.Cat('`');
					qtf.Cat(*q++);
				}
				qtf << "`";
			}
			else {
				if(crlf)
					qtf << "\r\n";
#if 1
				qtf << '(' << Base64Encode(q, slim) << ')';
#else
				while(q < slim - 7) {
					byte data[8];
					data[0] = ((q[0] & 0x80) >> 7) |
					          ((q[1] & 0x80) >> 6) |
					          ((q[2] & 0x80) >> 5) |
					          ((q[3] & 0x80) >> 4) |
					          ((q[4] & 0x80) >> 3) |
					          ((q[5] & 0x80) >> 2) |
					          ((q[6] & 0x80) >> 1) |
					          0x80;
					data[1] = q[0] | 0x80;
					data[2] = q[1] | 0x80;
					data[3] = q[2] | 0x80;
					data[4] = q[3] | 0x80;
					data[5] = q[4] | 0x80;
					data[6] = q[5] | 0x80;
					data[7] = q[6] | 0x80;
					qtf.Cat(data, 8);
					if(crlf && ++n % 10 == 0)
						qtf << "\r\n";
					q += 7;
				}
				while(q < slim) {
					byte seven = 0;
					const char *lim = slim;
					const char *s;
					for(s = q; s < lim; s++)
						seven = (seven >> 1) | (*s & 0x80);
					seven >>= 8 - (lim - q);
					qtf.Cat(seven | 0x80);
					for(s = q; s < lim; s++)
						qtf.Cat(*s | 0x80);
					if(crlf && ++n % 10 == 0)
						qtf << "\r\n";
					q += 7;
				}
#endif
			}
			if(crlf)
				qtf << "\r\n";
			d = 0;
		}
		else {
			for(const wchar *s = part.text.Begin(); s != part.text.End(); s++) {
				int c = *s;
				if(c < 128) {
					if(s_nodeqtf[c]) {
						qtf.Cat(c);
						d++;
					}
					else
					if(c == 9) {
						qtf.Cat("-|");
						d++;
					}
					else
					if(c == ':' && s[1] != ':')
						qtf.Cat(':');
					else {
						qtf.Cat('`');
						qtf.Cat(c);
						d += 2;
					}
					if(crlf && d > 60 && c == ' ') {
						qtf.Cat("\r\n");
						d = 0;
					}
				}
				else {
					if(c == 160) {
						qtf.Cat("_");
						d++;
					}
					else
					if(charset == CHARSET_UTF8) {
						String q = ToUtf8(c);
						d += q.GetLength();
						qtf << q;
					}
					else {
						int ch = FromUnicode(c, charset, 0);
						if(ch)
							qtf << (char)ch;
						else
							qtf << "@$" << Format("%04X", ch) << ';';
						d++;
					}
				}
				if(crlf && d > 80) {
					qtf.Cat("\r\n");
					d = 0;
				}
			}
		}
		if(!cf.IsEmpty()) {
			d++;
			qtf << ']';
		}
	}
}

void FmtNumber2(String& qtf, char c, int da, int a, int db, int b)
{
	if(da != a || db != b) {
		qtf << c;
		if(da != a)
			qtf << a;
		qtf << '/';
		qtf << b;
	}
}


void QTFEncodeTxt(String& qtf, const RichTxt& text, const RichStyles& styles, const RichStyle& defstyle,
                  dword options, const Index<Uuid>& sm, byte charset, dword lang)
{
	qtf << '[';
	for(int i = 0; i < text.GetPartCount(); i++) {
		if(i) {
			qtf << "&]";
			if(options & QTF_CRLF)
				qtf << "\r\n";
			qtf << '[';
		}
		if(text.IsTable(i)) {
			qtf << ' ';

			const RichTable& t = text.GetTable(i);
			const RichTable::Format& f = t.GetFormat();
			int nx = f.column.GetCount();
			int ny = t.GetRows();
			qtf << "{{";
			for(int i = 0; i < nx; i++) {
				if(i)
					qtf << ':';
				qtf << f.column[i];
			}
			const RichTable::Format& d = Single<RichTable::Format>();
			FmtNumber(qtf, '<', d.lm, f.lm);
			FmtNumber(qtf, '>', d.rm, f.rm);
			FmtNumber(qtf, 'B', d.before, f.before);
			FmtNumber(qtf, 'A', d.after, f.after);
			FmtNumber(qtf, 'f', d.frame, f.frame);
			if(f.keep)
				qtf << 'K';
			if(f.newpage)
				qtf << 'P';
			if(f.newhdrftr)
				qtf << 'T' << f.header_qtf << "^^" << f.footer_qtf << "^^";
			if(f.framecolor != d.framecolor)
				qtf << 'F' << QtfFormat(f.framecolor);
			FmtNumber(qtf, 'g', d.grid, f.grid);
			if(f.gridcolor != d.gridcolor)
				qtf << 'G' << QtfFormat(f.gridcolor);
			FmtNumber(qtf, 'h', d.header, f.header);
			RichCell::Format cf = Single<RichCell::Format>();
			for(int i = 0; i < ny; i++) {
				const Array<RichCell>& r = t[i];
				for(int j = 0; j < r.GetCount(); j++) {
					const RichCell& c = r[j];
					if(i || j) {
						if(options & QTF_CRLF)
							qtf << "\r\n";
						qtf << "::";
					}
					const RichCell::Format& f = c.format;
					if(f.align != cf.align)
						switch(f.align) {
						case ALIGN_TOP:    qtf << '^'; break;
						case ALIGN_CENTER: qtf << '='; break;
						case ALIGN_BOTTOM: qtf << 'v'; break;
						}
					FmtNumber2(qtf, 'l', cf.border.left, f.border.left, cf.margin.left, f.margin.left);
					FmtNumber2(qtf, 'r', cf.border.right, f.border.right, cf.margin.right, f.margin.right);
					FmtNumber2(qtf, 't', cf.border.top, f.border.top, cf.margin.top, f.margin.top);
					FmtNumber2(qtf, 'b', cf.border.bottom, f.border.bottom, cf.margin.bottom, f.margin.bottom);
					FmtNumber(qtf, 'H', cf.minheight, f.minheight);
					if(f.color != cf.color)
						qtf << '@' << QtfFormat(f.color);
					if(f.bordercolor != cf.bordercolor)
						qtf << 'R' << QtfFormat(f.bordercolor);
					cf = f;
					if(c.hspan)
						qtf << '-' << c.hspan;
					if(c.vspan)
						qtf << '|' << c.vspan;
					if(f.keep)
						qtf << "k";
					if(f.round)
						qtf << "o";
					qtf << ' ';
					QTFEncodeTxt(qtf, c.text, styles, defstyle, options, sm, charset, lang);
				}
			}
			qtf << "}}";
		}
		else {
			RichPara p = text.Get(i, styles);
			int si = sm.Find(text.GetParaStyle(i));
			if(!(options & QTF_NOSTYLES))
				qtf << "s" << si;
			SeparateNumber(qtf);
			const RichStyle& s = si < 0 ? defstyle : GetStyle(styles, sm[si]);
			QTFEncodePara(qtf, p, options & QTF_NOSTYLES ? defstyle.format : s.format, charset, lang,
			              (options & QTF_CRLF));
		}
	}
	qtf << ']';
}

void init_s_nodeqtf()
{
	ONCELOCK {
		for(int c = 1; c < 128; c++)
			s_nodeqtf[c] = IsAlNum(c) || strchr(".,;!?%()/<># ", c);
	}
}

String   AsQTF(const RichText& text, byte charset, dword options)
{
	int i;
	String qtf;

	init_s_nodeqtf();

	RichPara::Format dpf;
	dpf.Face(Font::ARIAL);
	dpf.Height(100);
	dpf.language = LNG_ENGLISH;

	bool crlf = options & QTF_CRLF;

	Index<Uuid> sm;
//	if(options & QTF_ALL_STYLES) //!!!! problem -> GetUsedStyles!!!!
		for(i = 0; i < text.GetStyleCount(); i++)
			sm.FindAdd(text.GetStyleId(i));
//	else
//		for(i = 0; i < text.GetPartCount(); i++)
//			sm.FindAdd(text.GetParaStyle(i));

	String hdr = text.GetHeaderQtf();
	if(hdr.GetCount())
		qtf << "^H" << hdr << "^^\r\n";

	String ftr = text.GetFooterQtf();
	if(ftr.GetCount())
		qtf << "^F" << ftr << "^^\r\n";

	if(!(options & QTF_NOSTYLES))
		for(int j = 1; j < sm.GetCount() + 1; j++) { // we need to store default style last as it immediately sets
			int i = j % sm.GetCount();
			Uuid id = sm[i];
			const RichStyle& s = text.GetStyle(id);
			qtf << '[';
			QTFEncodeParaFormat(qtf, s.format, dpf);
			CharFmt(qtf, dpf, s.format);
			qtf << ' ';
			qtf << "$$" << i << ',' << max(sm.Find(s.next), 0)
			    << '#' << Format(id)
			    << ':' << DeQtf(s.name) << "]";
			if(crlf)
			    qtf << "\r\n";
		}

	if(text.GetPartCount() && (options & QTF_BODY)) {
		VectorMap<dword, int> lngc;
		int m = min(text.GetPartCount(), 30);
		for(int i = 0; i < m; i++)
			if(text.IsPara(i)) {
				RichPara p = text.Get(i);
				lngc.GetAdd(p.format.language, 0)++;
			}
		dword lang = lngc.GetCount() ? lngc.GetKey(FindMax(lngc.GetValues())) : 0;
		qtf << "[";
		if(!(options & QTF_NOCHARSET)) {
			qtf << "{";
			if(charset == CHARSET_UTF8)
				qtf << "_";
			else
			if(charset >= CHARSET_WIN1250 && charset <= CHARSET_WIN1258)
				qtf << (char)('0' + charset - CHARSET_WIN1250);
			else
			if(charset >= CHARSET_ISO8859_1 && charset <= CHARSET_ISO8859_16)
				qtf << (char)('A' + charset - CHARSET_ISO8859_1);
			else
				qtf << CharsetName(charset);
			qtf << "}";
		}
		if(lang && !(options & QTF_NOLANG))
			qtf << "%" << LNGAsText(SetLNGCharset(lang, CHARSET_DEFAULT));
		qtf << " ";
		if(crlf)
			qtf << "\r\n";
		RichStyle defstyle;
		defstyle.format.Height(100);

		QTFEncodeTxt(qtf, text, text.GetStyles(), defstyle, options, sm, charset, lang);

		qtf << "]";
	}

	if(options & QTF_NOSTYLES) // remove redundant []
		while(qtf.StartsWith("[ ") && qtf.EndsWith("]"))
			qtf = qtf.Mid(2, qtf.GetCount() - 3);

	return qtf;
}

String DeQtf(const char *s) {
	StringBuffer r;
	for(; *s; s++) {
		if(*s == '\n')
			r.Cat('&');
		else {
			if((byte)*s > ' ' && !IsDigit(*s) && !IsAlpha(*s) && (byte)*s < 128)
				r.Cat('`');
			r.Cat(*s);
		}
	}
	return String(r);
}

String DeQtfLf(const char *s) {
	StringBuffer r;
	while(*s) {
		if((byte)*s > ' ' && !IsDigit(*s) && !IsAlpha(*s) && (byte)*s < 128)
			r.Cat('`');
		if((byte)*s >= ' ')
			r.Cat(*s);
		else
		if(*s == '\n')
			r.Cat('&');
		else
		if(*s == '\t')
			r.Cat("-|");
		s++;
	}
	return String(r);
}

RichText AsRichText(const RichObject& obj)
{
	RichText x;
	RichPara p;
	RichPara::Format fmt;
	p.Cat(obj, fmt);
	x.Cat(p);
	return x;
}


String AsQTF(const RichObject& obj)
{
	return AsQTF(AsRichText(obj), CHARSET_UTF8, QTF_NOSTYLES|QTF_BODY);
}

}
