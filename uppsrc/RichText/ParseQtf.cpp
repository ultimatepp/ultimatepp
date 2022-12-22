#include "RichText.h"

namespace Upp {

Color (*QTFColor[])() = {
	Black, LtGray, White, Red, Green, Blue, LtRed, WhiteGray, LtCyan, Yellow
};

Color NullColorF()
{
	return Null;
}

static Color (*QTFColorl[])() = {
	/*a*/White, /*b*/Blue, /*c*/Cyan, /*d*/White, /*e*/White, /*f*/White, /*g*/ Green, /*h*/White,
	/*i*/White, /*j*/White, /*k*/Black, /*l*/LtGray, /*m*/Magenta, /*n*/NullColorF, /*o*/Brown, /*p*/White,
	/*q*/White, /*r*/Red, /*s*/White, /*t*/White, /*u*/White, /*v*/White, /*w*/WhiteGray, /*x*/White,
	/*y*/Yellow, /*z*/ White
};

static Color (*QTFColorL[])() = {
	/*A*/White, /*B*/LtBlue, /*C*/LtCyan, /*D*/White, /*E*/White, /*F*/White, /*G*/LtGreen, /*H*/White,
	/*I*/White, /*J*/White, /*K*/Gray, /*L*/WhiteGray, /*M*/LtMagenta, /*N*/NullColorF, /*O*/Brown, /*P*/White,
	/*Q*/White, /*R*/LtRed, /*S*/White, /*T*/White, /*U*/White, /*V*/White, /*W*/White, /*X*/White,
	/*Y*/LtYellow, /*Z*/White
};

int QTFFontHeight[] = {
	50, 67, 84, 100, 134, 167, 200, 234, 300, 400
};

class RichQtfParser {
	void       *context;
	const char *term;
	WString     text;
	RichPara    paragraph;
	RichTable   tablepart;
	bool        istable;
	bool        breakpage;
	int         accesskey;

	struct PFormat : public RichPara::Format {
		byte                  charset;
	};

	struct Tab {
		RichCell::Format format;
		int              vspan, hspan;
		RichTxt          text;
		RichTable        table;
		int              cell;
		Vector<int>      rown;

		void Old()                    { RichTable::Format fmt; fmt.grid = 10; table.SetFormat(fmt); }

		Tab()                         { cell = 0; vspan = hspan = 0; }
	};

	PFormat          format;
	Array<PFormat>   fstack;
	Vector<Uuid>     styleid;
	Vector<int>      stylenext;
	Array<Tab>       table;
	bool             oldtab;

	bool   Key(int c)                 { if(*term == c) { term++; return true; } return false; }
	bool   Key2(int c, int d);
	bool   Key2(int c)                { return Key2(c, c); }
	int    GetNumber();
	int    ReadNumber();
	String GetText(int delim);
	String GetText2(int delim1, int delim2);
	Color  GetColor();
	void   Number2(int& a, int& b);

	void   Flush();
	void   SetFormat();
	void   FlushStyles();
	void   Error(const char *s);

	void   ReadObject();

	RichTable& Table();
	void       TableFormat(bool bw = false);
	void       FinishCell();
	void       FinishTable();
	void       FinishOldTable();
	void       S(int& x, int a);

	void       EndPart();

	void       Cat(int chr);

public:
	struct Exc {};

	RichText target;

	void     Parse(const char *qtf, int accesskey);

	RichQtfParser(void *context);
};

void init_s_nodeqtf();

RichQtfParser::RichQtfParser(void *context_)
: context(context_)
{
	format.Face(Font::ARIAL);
	format.Height(100);
	format.charset = GetDefaultCharset();
	format.language = 0;
	breakpage = false;
	istable = false;
	oldtab = false;
	init_s_nodeqtf();
}

bool RichQtfParser::Key2(int c, int d)
{
	if(term[0] == c && term[1] == d) {
		term += 2;
		return true;
	}
	return false;
}

int  RichQtfParser::GetNumber()
{
	int n = 0;
	int sgn = 1;
	if(*term == '-') {
		sgn = -1;
		term++;
	}
	while(IsDigit(*term))
		n = n * 10 + *term++ - '0';
	return sgn * n;
}

String RichQtfParser::GetText(int delim) {
	String s;
	for(;;) {
		if(*term == '\0') return s;
		if(*term == '`') {
			term++;
			if(*term == '\0') return s;
			s.Cat(*term++);
		}
		else
		if(*term == delim) {
			term++;
			return s;
		}
		else
			s.Cat(*term++);
	}
}

String RichQtfParser::GetText2(int delim1, int delim2) {
	String s;
	for(;;) {
		if(*term == '\0') return s;
		if(*term == '`') {
			term++;
			if(*term == '\0') return s;
			s.Cat(*term++);
		}
		else
		if(term[0] == delim1 && term[1] == delim2) {
			term += 2;
			return s;
		}
		else
			s.Cat(*term++);
	}
}

int RichQtfParser::ReadNumber()
{
	if(!IsDigit(*term))
		Error("Expected number");
	return GetNumber();
}

void RichQtfParser::Number2(int& a, int& b)
{
	a = -1;
	b = -1;
	if(IsDigit(*term))
		a = GetNumber();
	if(*term == '/') {
		term++;
		b = GetNumber();
	}
}

Color RichQtfParser::GetColor()
{
	int c = *term++;
	if(c == '(') {
		byte r = GetNumber();
		if(Key(')')) {
			r &= 255;
			return Color(r, r, r);
		}
		Key('.');
		byte g = GetNumber();
		Key('.');
		byte b = GetNumber();
		Key(')');
		return Color(r & 255, g & 255, b & 255);
	}
	else
	if(c >= '0' && c <= '9')
		return QTFColor[c - '0']();
	else
	if(c >= 'a' && c <= 'z')
		return QTFColorl[c - 'a']();
	else
	if(c >= 'A' && c <= 'Z')
		return QTFColorL[c - 'A']();
	else
		return Red;
}

void RichQtfParser::SetFormat()
{
	paragraph.format = format;
}

void RichQtfParser::Flush() {
	if(text.GetLength()) {
		ASSERT(!istable);
		paragraph.Cat(text, format);
		text.Clear();
	}
}

void RichQtfParser::EndPart()
{
	if(istable) {
		if(paragraph.GetCount() == 0 && text.GetCount() == 0)
			if(table.GetCount())
				table.Top().text.CatPick(pick(tablepart));
			else
				target.CatPick(pick(tablepart));
		else {
			paragraph.part.Clear();
			text.Clear();
		}
	}
	else {
		Flush();
		if(table.GetCount())
			table.Top().text.Cat(paragraph, target.GetStyles());
		else {
			if(breakpage)
				paragraph.format.newpage = true;
			target.Cat(paragraph);
		}
		paragraph.part.Clear();
		SetFormat();
		breakpage = false;
	}
	istable = false;
}

void RichQtfParser::ReadObject()
{
	Flush();
	RichObject obj;
	if(*term == '#') {
		term++;
	#ifdef CPU_64
		obj = *(RichObject *)stou64(term, &term);
	#else
		obj = *(RichObject *)stou(term, &term);
	#endif
		term++;
	}
	else {
		String type;
		while(IsAlNum(*term) || *term == '_')
			type.Cat(*term++);
		Size sz;
		Key(':');
		sz.cx = ReadNumber();
		bool keepratio = false;
		if(Key('&'))
			keepratio = true;
		else
			Key('*');
		sz.cy = ReadNumber();
		int yd = 0;
		if(Key('/'))
			yd = GetNumber();
		while(*term && (byte)*term < 32)
			term++;
		String odata;
		if(Key('`'))
			while(*term) {
				if(*term == '`') {
					term++;
					if(*term == '`')
						odata.Cat('`');
					else
						break;
				}
				else
				if((byte)*term >= 32)
					odata.Cat(*term);
				term++;
			}
		else
		if(Key('(')) {
			const char *b = term;
			while(*term && *term != ')')
				term++;
			odata = Base64Decode(b, term);
			if(*term == ')')
				term++;
		}
		else {
			StringBuffer data;
			for(;;) {
				while(*term < 32 && *term > 0) term++;
				if((byte)*term >= ' ' && (byte)*term <= 127 || *term == '\0') break;
				byte seven = *term++;
				for(int i = 0; i < 7; i++) {
					while((byte)*term < 32 && (byte)*term > 0) term++;
					if((byte)*term >= ' ' && (byte)*term <= 127 || *term == '\0') break;
					data.Cat((*term++ & 0x7f) | ((seven << 7) & 0x80));
					seven >>= 1;
				}
			}
			odata = data;
		}
		obj.Read(type, odata, sz, context);
		obj.KeepRatio(keepratio);
		obj.SetYDelta(yd);
	}
	paragraph.Cat(obj, format);
}

int NoLow(int c) {
	return (byte)c >= 32 ? c : 0;
}

void RichQtfParser::Error(const char *s) {
	RichPara::CharFormat ef;
	(Font&) ef = Arial(84).Bold().Underline();
	ef.ink = Red;
	WString e;
	e << "ERROR: " << s;
	if(*term)
		e << ": " << Filter(String(term, min<int>((int)strlen(term), 20)), NoLow).ToWString();
	else
		e << ".";
	paragraph.Cat(e, ef);
	target.Cat(paragraph);
	FlushStyles();
	throw Exc();
}

void RichQtfParser::FlushStyles()
{
	for(int i = 0; i < styleid.GetCount(); i++)
		if(stylenext[i] >= 0 && stylenext[i] < styleid.GetCount()) {
			RichStyle s = target.GetStyle(styleid[i]);
			s.next = styleid[stylenext[i]];
			target.SetStyle(styleid[i], s);
		}
}

RichTable& RichQtfParser::Table()
{
	if(table.GetCount() == 0)
		Error("Not in table");
	return table.Top().table;
}

void RichQtfParser::S(int& x, int a)
{
	if(a >= 0)
		x = a;
}

void RichQtfParser::TableFormat(bool bw)
{
	RichTable& tab = Table();
	RichTable::Format tabformat = tab.GetFormat();
	Tab& t = table.Top();
	int a, b;
	for(;;) {
		if(bw && IsDigit(*term)) {
			t.hspan = GetNumber();
		}
		else
		if(*term == '\0')
			Error("Unexpected end of text in cell format");
		else
		switch(*term++) {
		case ' ':
			tab.SetFormat(tabformat);
			return;
		case ';': break;
		case '<': tabformat.lm = ReadNumber(); break;
		case '>': tabformat.rm = ReadNumber(); break;
		case 'B': tabformat.before = ReadNumber(); break;
		case 'A': tabformat.after = ReadNumber(); break;
		case 'f': tabformat.frame = ReadNumber(); break;
		case '_':
		case 'F': tabformat.framecolor = GetColor(); break;
		case 'g': tabformat.grid = ReadNumber(); break;
		case 'G': tabformat.gridcolor = GetColor(); break;
		case 'h': tabformat.header = GetNumber(); break;
		case '~': tabformat.frame = tabformat.grid = 0; break;
		case '^': t.format.align = ALIGN_TOP; break;
		case '=': t.format.align = ALIGN_CENTER; break;
		case 'v': t.format.align = ALIGN_BOTTOM; break;
		case 'l': Number2(a, b); S(t.format.border.left, a); S(t.format.margin.left, b); break;
		case 'r': Number2(a, b); S(t.format.border.right, a); S(t.format.margin.right, b); break;
		case 't': Number2(a, b); S(t.format.border.top, a); S(t.format.margin.top, b); break;
		case 'b': Number2(a, b); S(t.format.border.bottom, a); S(t.format.margin.bottom, b); break;
		case 'H': t.format.minheight = ReadNumber(); break;
		case '@': t.format.color = GetColor(); break;
		case 'R': t.format.bordercolor = GetColor(); break;
		case '!': t.format = RichCell::Format(); break;
		case 'o': t.format.round = true; break;
		case 'k': t.format.keep = true; break;
		case 'K': tabformat.keep = true; break;
		case 'P': tabformat.newpage = true; break;
		case 'T':
			tabformat.newhdrftr = true;
			tabformat.header_qtf = GetText2('^', '^');
			tabformat.footer_qtf = GetText2('^', '^');
			break;
		case 'a':
			Number2(a, b);
			if(a >= 0)
				t.format.border.left = t.format.border.right = t.format.border.top = t.format.border.bottom = a;
			if(b >= 0)
				t.format.margin.left = t.format.margin.right = t.format.margin.top = t.format.margin.bottom = b;
			break; //!!cell all lines
		case '*':
			tabformat.frame = tabformat.grid =
			t.format.border.left = t.format.border.right = t.format.border.top = t.format.border.bottom =
			t.format.margin.left = t.format.margin.right = t.format.margin.top = t.format.margin.bottom = 0;
			break;
		case '-': t.hspan = GetNumber(); break;
		case '+':
		case '|': t.vspan = GetNumber(); break;
		default:
			Error("Invalid cell format");
		}
	}
}

void RichQtfParser::FinishCell()
{
	EndPart();
	RichTable& t = Table();
	Tab& b = table.Top();
	int i, j;
	if(oldtab) {
		i = b.rown.GetCount() - 1;
		j = b.rown.Top();
		b.rown.Top()++;
	}
	else {
		i = b.cell / t.GetColumns();
		j = b.cell % t.GetColumns();
	}
	t.SetPick(i, j, pick(b.text));
	b.text.Clear();
	t.SetFormat(i, j, b.format);
	t.SetSpan(i, j, b.vspan, b.hspan);
	if(oldtab && b.rown.GetCount() > 1 && j + 1 < b.rown[0])
		b.format = t.GetFormat(0, j + 1);
	else {
		b.cell++;
		b.vspan = 0;
		b.hspan = oldtab;
	}
	b.format.keep = false;
	b.format.round = false;
}

void RichQtfParser::FinishTable()
{
	FinishCell();
	while(table.Top().cell % Table().GetColumns())
		FinishCell();
	tablepart = pick(Table());
	istable = true;
	table.Drop();
}

void RichQtfParser::FinishOldTable()
{
	FinishCell();
	Index<int>  pos;
	Vector<int> srow;
	RichTable& t = Table();
	Tab& b = table.Top();
	for(int i = 0; i < t.GetRows(); i++) {
		int& s = srow.Add();
		s = 0;
		int nx = b.rown[i];
		for(int j = 0; j < nx; j++)
			s += t.GetSpan(i, j).cx;
		int xn = 0;
		for(int j = 0; j < nx; j++) {
			pos.FindAdd(xn * 10000 / s);
			xn += t.GetSpan(i, j).cx;
		}
	}
	Vector<int> h = pos.PickKeys();
	if(h.GetCount() == 0)
		Error("table");
	Sort(h);
	pos = pick(h);
	pos.Add(10000);
	RichTable tab;
	tab.SetFormat(t.GetFormat());
	for(int i = 0; i < pos.GetCount() - 1; i++) {
		tab.AddColumn(pos[i + 1] - pos[i]);
	}
	for(int i = 0; i < t.GetRows(); i++) {
		int s = srow[i];
		int nx = b.rown[i];
		int xn = 0;
		int xi = 0;
		for(int j = 0; j < nx; j++) {
			Size span = t.GetSpan(i, j);
			xn += span.cx;
			int nxi = pos.Find(xn * 10000 / s);
			tab.SetPick(i, xi, t.GetPick(i, j));
			tab.SetFormat(i, xi, t.GetFormat(i, j));
			tab.SetSpan(i, xi, max(span.cy - 1, 0), nxi - xi - 1);
			xi = nxi;
		}
	}
	table.Drop();
	if(table.GetCount())
		table.Top().text.CatPick(pick(tab));
	else
		target.CatPick(pick(tab));
	oldtab = false;
}

void RichQtfParser::Cat(int chr)
{
	if(accesskey && ToUpper(ToAscii(chr)) == LOBYTE(accesskey)) {
		Flush();
		format.Underline(!format.IsUnderline());
		text.Cat(chr);
		Flush();
		format.Underline(!format.IsUnderline());
		accesskey = 0;
	}
	else if(chr >= ' ') {
		text.Cat(chr);
	}
}

extern bool s_nodeqtf[128];

int GetRichTextScreenStdFontHeight()
{
	static int gh = 67;
	ONCELOCK {
		for(int i = 0; i < 1000; i++) {
			int h = GetRichTextStdScreenZoom() * i;
			if(h > 0 && StdFont(h).GetCy() == StdFont().GetCy()) {
				gh = i;
				break;
			}
		}
	}
	return gh;
}

void RichQtfParser::Parse(const char *qtf, int _accesskey)
{
	accesskey = _accesskey;
	term = qtf;
	while(*term) {
		if(Key('[')) {
			Flush();
			fstack.Add(format);
			for(;;) {
				int c = *term;
				if(!c)
					Error("Unexpected end of text");
				term++;
				if(c == ' ' || c == '\n') break;
				switch(c) {
				case 's': {
					Uuid id;
					c = *term;
					if(Key('\"') || Key('\''))
						id = target.GetStyleId(GetText(c));
					else {
						int i = ReadNumber();
						if(i >= 0 && i < styleid.GetCount())
							id = styleid[i];
						else
							id = RichStyle::GetDefaultId();
					}
					const RichStyle& s = target.GetStyle(id);
					bool p = format.newpage;
					int lng = format.language;
					(RichPara::Format&) format = s.format;
					format.styleid = id;
					format.language = lng;
					format.newpage = p;
					break;
				}
				case '/': format.Italic(!format.IsItalic()); break;
				case '*': format.Bold(!format.IsBold()); break;
				case '_': format.Underline(!format.IsUnderline()); break;
				case 'T': format.NonAntiAliased(!format.IsNonAntiAliased()); break;
				case '-': format.Strikeout(!format.IsStrikeout()); break;
				case 'c': format.capitals = !format.capitals; break;
				case 'd': format.dashed = !format.dashed; break;
				case '`': format.sscript = format.sscript == 1 ? 0 : 1; break;
				case ',': format.sscript = format.sscript == 2 ? 0 : 2; break;
				case '^': format.link = GetText('^'); break;
				case 'I': format.indexentry = ToUtf32(GetText(';')); break;
				case '+': format.Height(GetNumber()); break;
				case '@': format.ink = GetColor(); break;
				case '$': format.paper = GetColor(); break;
				case 'A': format.Face(Font::ARIAL); break;
				case 'R': format.Face(Font::ROMAN); break;
				case 'C': format.Face(Font::COURIER); break;
				case 'G': format.Face(Font::STDFONT); break;
				case 'S':
#ifdef PLATFORM_WIN32
					format.Face(Font::SYMBOL);
#endif
					break;
				case '.': {
					int n = GetNumber();
					if(n >= Font::GetFaceCount())
						Error("Invalid face number");
					format.Face(n); break;
				}
				case '!': {
						String fn = GetText('!');
						int i = Font::FindFaceNameIndex(fn);
						if(i < 0)
							i = Font::ARIAL;
						format.Face(i);
					}
					break;
				case '{': {
						String cs = GetText('}');
						if(cs.GetLength() == 1) {
							int c = *cs;
							if(c == '_')
								format.charset = CHARSET_UTF8;
							if(c >= '0' && c <= '8')
								format.charset = c - '0' + CHARSET_WIN1250;
							if(c >= 'A' && c <= 'Z')
								format.charset = c - '0' + CHARSET_ISO8859_1;
						}
						else {
							for(int i = 0; i < CharsetCount(); i++)
								if(stricmp(CharsetName(i), cs) == 0) {
									format.charset = i;
									break;
								}
						}
						break;
					}
				case '%': {
						String h;
						if(*term == '-') {
							format.language = 0;
							term++;
						}
						else
						if(*term == '%') {
							format.language = LNG_ENGLISH;
							term++;
						}
						else {
							while(*term && h.GetLength() < 5)
								h.Cat(*term++);
							format.language = LNGFromText(h);
						}
						break;
					}
				case 'g':
					format.Face(Font::STDFONT);
					format.Height(GetRichTextScreenStdFontHeight());
					break;
				default:
					if(c >= '0' && c <= '9') {
						format.Height(QTFFontHeight[c - '0']);
						break;
					}
					switch(c) {
					case ':': format.label = GetText(':'); break;
					case '<': format.align = ALIGN_LEFT; break;
					case '>': format.align = ALIGN_RIGHT; break;
					case '=': format.align = ALIGN_CENTER; break;
					case '#': format.align = ALIGN_JUSTIFY; break;
					case 'l': format.lm = GetNumber(); break;
					case 'r': format.rm = GetNumber(); break;
					case 'i': format.indent = GetNumber(); break;
					case 'b': format.before = GetNumber(); break;
					case 'a': format.after = GetNumber(); break;
					case 'P': format.newpage = !format.newpage; break;
					case 'F': format.firstonpage = !format.firstonpage; break;
					case 'k': format.keep = !format.keep; break;
					case 'K': format.keepnext = !format.keepnext; break;
					case 'H': format.ruler = GetNumber(); break;
					case 'h': format.rulerink = GetColor(); break;
					case 'L': format.rulerstyle = GetNumber(); break;
					case 'Q': format.orphan = !format.orphan; break;
					case 'n': format.before_number = GetText(';'); break;
					case 'm': format.after_number = GetText(';'); break;
					case 'N': {
						memset8(format.number, 0, sizeof(format.number));
						format.reset_number = false;
						int i = 0;
						while(i < 8) {
							int c;
							if(Key('-'))
								c = RichPara::NUMBER_NONE;
							else
							if(Key('1'))
								c = RichPara::NUMBER_1;
							else
							if(Key('0'))
								c = RichPara::NUMBER_0;
							else
							if(Key('a'))
								c = RichPara::NUMBER_a;
							else
							if(Key('A'))
								c = RichPara::NUMBER_A;
							else
							if(Key('i'))
								c = RichPara::NUMBER_i;
							else
							if(Key('I'))
								c = RichPara::NUMBER_I;
							else
								break;
							format.number[i++] = c;
						}
						if(Key('!'))
							format.reset_number = true;
						break;
					}
					case 'o': format.bullet = RichPara::BULLET_ROUND;
					          format.indent = 150; break;
					case 'O':
						if(Key('_'))
							format.bullet = RichPara::BULLET_NONE;
						else {
							int c = *term++;
							if(!c)
								Error("Unexpected end of text");
							format.bullet =
							                c == '1' ? RichPara::BULLET_ROUNDWHITE :
							                c == '2' ? RichPara::BULLET_BOX :
							                c == '3' ? RichPara::BULLET_BOXWHITE :
							                c == '9' ? RichPara::BULLET_TEXT :
							                           RichPara::BULLET_ROUND;
						}
						break;
					case 'p':
						switch(*term++) {
						case 0:   Error("Unexpected end of text");
						case 'h': format.linespacing = RichPara::LSP15; break;
						case 'd': format.linespacing = RichPara::LSP20; break;
						case 'w': format.linespacing = RichPara::LSP115; break;
						default:  format.linespacing = RichPara::LSP10;
						}
						break;
					case 't':
						if(*term == 'P') {
							term++;
							format.newhdrftr = true;
							format.header_qtf = GetText2('^', '^');
							format.footer_qtf = GetText2('^', '^');
						}
						else
						if(IsDigit(*term))
							format.tabsize = ReadNumber();
						break;
					case '~': {
							if(Key('~'))
								format.tab.Clear();
							else {
								RichPara::Tab tab;
								Key('<');
								if(Key('>'))
									tab.align = ALIGN_RIGHT;
								if(Key('='))
									tab.align = ALIGN_CENTER;
								if(Key('.'))
									tab.fillchar = 1;
								if(Key('-'))
									tab.fillchar = 2;
								if(Key('_'))
									tab.fillchar = 3;
								int rightpos = Key('>') ? RichPara::TAB_RIGHTPOS : 0;
								tab.pos = rightpos | ReadNumber();
								format.tab.Add(tab);
							}
						}
						break;
					default:
						continue;
					}
				}
			}
			SetFormat();
		}
		else
		if(Key(']')) {
			Flush();
			if(fstack.GetCount()) {
				format = fstack.Top();
				fstack.Drop();
			}
			else
				Error("Unmatched ']'");
		}
		else
		if(Key2('{')) {
			if(oldtab)
				Error("{{ in ++ table");
			if(text.GetLength() || paragraph.GetCount())
				EndPart();
			table.Add();
			int r = IsDigit(*term) ? ReadNumber() : 1;
			Table().AddColumn(r);
			while(Key(':'))
				Table().AddColumn(ReadNumber());
			if(breakpage) {
				RichTable& tab = Table();
				RichTable::Format tabformat = tab.GetFormat();
				tabformat.newpage = true;
				tab.SetFormat(tabformat);
				breakpage = false;
			}
			TableFormat();
			SetFormat();
		}
		else
		if(Key2('}')) {
			if(oldtab)
				Error("}} in ++ table");
			FinishTable();
		}
		else
		if(Key2('+'))
			if(oldtab)
				FinishOldTable();
			else {
				Flush();
				if(text.GetLength() || paragraph.GetCount())
					EndPart();
				Tab& b = table.Add();
				b.rown.Add(0);
				b.hspan = 1;
				b.Old();
				oldtab = true;
			}
		else
		if(Key2('|'))
			FinishCell();
		else
		if(Key2('-')) {
			FinishCell();
			table.Top().rown.Add(0);
		}
		else
		if(Key2(':')) {
			if(!oldtab)
				FinishCell();
			TableFormat(oldtab);
		}
		else
		if(Key2('^')) {
			EndPart();
			breakpage = true;
		}
		else
		if(Key2('@')) {
			ReadObject();
		}
		else
		if(Key2('@', '$')) {
			String xu;
			while(isxdigit(*term))
				xu.Cat(*term++);
			int c = stou(~xu, NULL, 16);
			if(c >= 32)
				Cat(c);
			if(*term == ';')
				term++;
			SetFormat();
		}
		else
		if(Key2('^', 'H'))
			target.SetHeaderQtf(GetText2('^', '^'));
		else
		if(Key2('^', 'F'))
			target.SetFooterQtf(GetText2('^', '^'));
		else
		if(Key2('{', ':')) {
			Flush();
			String field = GetText(':');
			String param = GetText(':');
			Id fid(field);
			if(RichPara::fieldtype().Find(fid) >= 0)
				paragraph.Cat(fid, param, format);
			Key('}');
		}
		else
		if(Key('&')) {
			SetFormat();
			EndPart();
		}
		else
		if(Key2('$')) {
			Flush();
			int i = GetNumber();
			Uuid id;
			RichStyle style;
			style.format = format;
			if(Key(','))
				stylenext.At(i, 0) = GetNumber();
			else
				stylenext.At(i, 0) = i;
			if(Key('#')) {
				String xu;
				while(isxdigit(*term))
					xu.Cat(*term++);
				if(xu.GetLength() != 32)
					Error("Invalid UUID !");
				id = ScanUuid(xu);
			}
			else
				if(i)
					id = Uuid::Create();
				else
					id = RichStyle::GetDefaultId();
			if(Key(':'))
				style.name = GetText(']');
			if(fstack.GetCount()) {
				format = fstack.Top();
				fstack.Drop();
			}
			target.SetStyle(id, style);
			styleid.At(i, RichStyle::GetDefaultId()) = id;
			if(id == RichStyle::GetDefaultId()) {
				bool p = format.newpage;
				int lng = format.language;
				(RichPara::Format&) format = style.format;
				format.styleid = id;
				format.language = lng;
				format.newpage = p;
			}
		}
		else
		if(*term == '_') {
			SetFormat();
			text.Cat(160);
			term++;
		}
		else
		if(Key2('-', '|')) {
			SetFormat();
			text.Cat(9);
		}
		else
		if(*term == '\1') {
			if(istable)
				EndPart();
			SetFormat();
			const char *b = ++term;
			for(; *term && *term != '\1'; term++)
				if((byte)*term < 32) {
					text.Cat(ToUnicode(b, (int)(term - b), format.charset));
					if(*term == '\n')
						EndPart();
					if(*term == '\t')
						text.Cat(9);
					b = term + 1;
				}
			text.Cat(ToUnicode(b, (int)(term - b), format.charset));
			if(*term == '\1')
				term++;
		}
		else {
			if(!Key('`')) Key('\\');
			if((byte)*term >= ' ') {
				SetFormat();
				do {
					if(istable)
						EndPart();
					if(format.charset == CHARSET_UTF8) {
						bool ok = true;
						wchar c = FetchUtf8(term, ok);
						if(ok)
							Cat(c);
						else
							Error("Invalid UTF-8 sequence");
					}
					else
						Cat(ToUnicode((byte)*term++, format.charset));
				}
				while((byte)*term >= 128 || s_nodeqtf[(byte)*term]);
			}
			else
			if(*term)
				term++;
		}
	}
	if(oldtab)
		FinishOldTable();
	else
		while(table.GetCount())
			FinishTable();
	EndPart();
	FlushStyles();
}

bool ParseQTF(RichText& txt, const char *qtf, int accesskey, void *context)
{
	RichQtfParser p(context);
	try {
		p.Parse(qtf, accesskey);
	}
	catch(RichQtfParser::Exc) {
		return false;
	}
	txt = pick(p.target);
	return true;
}

RichText ParseQTF(const char *qtf, int accesskey, void *context)
{
	RichQtfParser p(context);
	try {
		p.Parse(qtf, accesskey);
	}
	catch(RichQtfParser::Exc) {}
	return pick(p.target);
}

String QtfRichObject::ToString() const
{
	return String("@@#").Cat() << uintptr_t(&obj) << ";";
}

QtfRichObject::QtfRichObject(const RichObject& o)
	: obj(o)
{}

}
