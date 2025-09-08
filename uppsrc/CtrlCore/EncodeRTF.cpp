#include "CtrlCore.h"

#define LLOG(x)

#include <plugin/png/png.h>

namespace Upp {

static int GetParaHeight(const Array<RichPara::Part>& parts)
{
	int ht = 0;
	for(int i = 0; i < parts.GetCount(); i++) {
		int pht = 0;
		const RichPara::Part& part = parts[i];
		if(part.object)
			pht = part.object.GetSize().cy;
		else if(part.field)
			pht = GetParaHeight(part.fieldpart);
		else
			pht = tabs(part.format.GetHeight());
		if(pht > ht)
			ht = pht;
	}
	return ht;
}

class RTFEncoder {
public:
	RTFEncoder(Stream& stream, const RichText& richtext, byte charset,
		Size dot_page_size, const Rect& dot_margins, void *context);

	void            Run();

private:
	void            FacesAddFormat(const RichPara::CharFormat& format);
	void            GetFaces();
	void            GetTxtFaces(const RichTxt& txt);

	void            PutHeader();
	void            PutDocument();
	void            PutTxt(const RichTxt& txt, int nesting, int dot_width);
	void            PutTable(const RichTable& table, int nesting, int dot_width);
	void            PutParts(const Array<RichPara::Part>& parts,
		RichPara::CharFormat& base, int bpart, int boff, int epart, int eoff);

	void            Begin()                           { stream.Put('{'); }
	void            Begin(const char *cmd)            { Begin(); Command(cmd); }
	void            Begin(const char *cmd, int param) { Begin(); Command(cmd, param); }
	void            End()                             { stream.Put('}'); }

	void            Command(const char *cmd);
	void            Command(const char *cmd, int param);
	void            Space()                           { stream.Put(' '); }
	void            PutText(const wchar *text);
	void            PutObject(const RichObject& object);
	void            PutTabs(const Vector<RichPara::Tab>& tabs);
	void            PutBinHex(const byte *data, int count);
	void            PutBinHex(const String& s)        { PutBinHex(s, s.GetLength()); }
	void            PutBorder(StringBuffer& rowfmt, int wd, Color c, const char *clpar);

	bool            PutParaFormat(const RichPara::Format& pf, const RichPara::Format& difpf);
	bool            PutCharFormat(const RichPara::CharFormat& cf, const RichPara::CharFormat& difcf, bool pn);

	struct Group;

	friend struct Group;

	struct Group {
		Group(RTFEncoder *owner) : owner(owner) { owner->Begin(); }
		Group(RTFEncoder *owner, const char *cmd) : owner(owner) { owner->Begin(cmd); }
		Group(RTFEncoder *owner, const char *cmd, int param) : owner(owner) { owner->Begin(cmd, param); }
		~Group() { owner->End(); }
		RTFEncoder *owner;
	};

private:
	Stream&               stream;
	const RichText&       richtext;
	byte                  charset;
	Size                  dot_page_size;
	Rect                  dot_margins;

	RichPara::CharFormat  charfmt;
	RichPara::Format      parafmt;
	Uuid                  oldstyle;
	int                   old_ht;
	int                   para_ht;

	Index<int>            used_faces;
	enum { SYMBOL_INDEX = 1, WINGDINGS_INDEX = 2 };
	VectorMap<Color, int> used_ink, used_paper;
	Index<Color>          phys_colors;
	Index<Uuid>           styleid;
};

void EncodeRTF(Stream& stream, const RichText& richtext, byte charset,
	Size dot_page_size, const Rect& dot_margins, void *context)
{
	RTFEncoder(stream, richtext, charset, dot_page_size, dot_margins, context).Run();
}

String EncodeRTF(const RichText& richtext, byte charset, Size dot_page_size, const Rect& dot_margins, void *context)
{
	StringStream out;
	EncodeRTF(out, richtext, charset, dot_page_size, dot_margins, context);
	String s = out.GetResult();
	LLOG("EncodeRTF <<<<<\n" << s << "\n>>>>> EncodeRTF");
	return s;
}

String EncodeRTF(const RichText& richtext, byte charset, int dot_page_width)
{
	int dot_page_height = iscale(dot_page_width, 1414, 1000);
	Rect margins(472, 472, 472, 472);
	return EncodeRTF(richtext, charset, Size(dot_page_width + margins.left + margins.right,
		dot_page_height + margins.top + margins.bottom), margins);
}

String EncodeRTF(const RichText& richtext)
{
	return EncodeRTF(richtext, GetDefaultCharset() == CHARSET_UTF8 ? GetLNGCharset(GetSystemLNG())
	                                                               : GetDefaultCharset());
}

RTFEncoder::RTFEncoder(Stream& stream_, const RichText& richtext_, byte charset_,
	Size dot_page_size_, const Rect& dot_margins_, void *context_)
: stream(stream_)
, richtext(richtext_)
, charset(charset_)
, dot_page_size(dot_page_size_)
, dot_margins(dot_margins_)
{
	for(int i = 0; i < richtext.GetStyleCount(); i++)
		styleid.Add(richtext.GetStyleId(i));
}

void RTFEncoder::Run()
{
	GetFaces();
	Group docgrp(this, "rtf", 1);
	Command("ansi");
	PutHeader();
	PutDocument();
}

void RTFEncoder::FacesAddFormat(const RichPara::CharFormat& format)
{
	used_faces.FindAdd(format.GetFace());
	if(used_ink.Find(format.ink) < 0) {
		Color i(format.ink.GetR(), format.ink.GetG(), format.ink.GetB());
		int x = used_ink.Get(i, -1);
		if(x < 0)
			x = phys_colors.FindAdd(i);
		used_ink.Add(format.ink, x);
	}
	if(used_paper.Find(format.paper) < 0) {
		Color p(format.paper.GetR(), format.paper.GetG(), format.paper.GetB());
		int x = used_paper.Get(p, -1);
		if(x < 0)
			x = phys_colors.FindAdd(p);
		used_paper.Add(format.paper, x);
	}
}

void RTFEncoder::GetFaces()
{
	used_faces.Add(Font::ARIAL); // default font
#ifdef PLATFORM_WIN32
	used_faces.Add(Font::SYMBOL); // used for bullets
	used_faces.Add(Font::WINGDINGS); // used for bullets
#endif
	phys_colors.Add(Null);
	used_ink.Add(Null, 0);
	used_paper.Add(Null, 0);
	used_ink.Add(Black(), 0);
	used_paper.Add(White(), 0);
	GetTxtFaces(richtext);
	for(int i = 0; i < richtext.GetStyleCount(); i++)
		FacesAddFormat(richtext.GetStyle(i).format);
}

void RTFEncoder::GetTxtFaces(const RichTxt& rt)
{
	for(int i = 0; i < rt.GetPartCount(); i++) {
		if(rt.IsTable(i)) {
			const RichTable& table = rt.GetTable(i);
			const RichTable::Format& tfmt = table.GetFormat();
			phys_colors.FindAdd(tfmt.framecolor);
			phys_colors.FindAdd(tfmt.gridcolor);
			for(int r = 0; r < table.GetRows(); r++)
				for(int c = 0; c < table.GetColumns(); c++) {
					const RichCell& cell = table[r][c];
					phys_colors.FindAdd(cell.format.color);
					phys_colors.FindAdd(cell.format.bordercolor);
					GetTxtFaces(cell.text);
				}
		}
		else {
			const RichPara& para = rt.Get(i, richtext.GetStyles());
			FacesAddFormat(para.format);
			for(int p = 0; p < para.part.GetCount(); p++) {
				const RichPara::Part& part = para.part[p];
				if(part.IsText())
					FacesAddFormat(part.format);
			}
		}
	}
}

void RTFEncoder::Command(const char *cmd)
{
	stream.Put('\\');
	stream.Put(cmd);
}

void RTFEncoder::Command(const char *cmd, int param)
{
	stream.Put('\\');
	stream.Put(cmd);
	stream.Put(IntStr(param));
}

void RTFEncoder::PutText(const wchar *text)
{
	for(; *text; text++)
		if(*text >= 128) {
			if(*text >= 65536) {
				Vector<char16> h = ToUtf16(*text);
				stream << "\\uc0 \\u" << (unsigned)h[0] << "\\uc1 \\u" << (unsigned)h[1];
			}
			else
				stream.Put(Format("\\u%d?", (int16)*text));
		}
		else {
			if(*text == '{' || *text == '}' || *text == '\\')
				stream.Put('\\');
			stream.Put((byte)*text);
		}
}

void RTFEncoder::PutBinHex(const byte *b, int count)
{
	enum { BLOCK = 32 };
	for(int l = count; l > 0; l -= BLOCK) {
		stream.PutCrLf();
		for(const byte *e = b + min<int>(l, BLOCK); b < e; b++) {
			static const char binhex[] = "0123456789abcdef";
			stream.Put(binhex[*b >> 4]);
			stream.Put(binhex[*b & 15]);
		}
	}
}

bool RTFEncoder::PutCharFormat(const RichPara::CharFormat& cf, const RichPara::CharFormat& difcf, bool pn)
{
	int64 pos = stream.GetPos();
	int pn2 = (pn ? 0 : 2);
	bool f;
	int t;
	if(cf.GetFace() != difcf.GetFace())
		Command(("pnf") + pn2, used_faces.Find(cf.GetFace()));
	if((t = DotPoints(2 * tabs(cf.GetHeight()))) != DotPoints(2 * tabs(difcf.GetHeight())))
		Command(("pnfs") + pn2, t);
	if(!pn && dword(t = cf.sscript) != difcf.sscript)
		Command(t == 0 ? "nosupersub" : t == 1 ? "super" : "sub");
	if((f = cf.IsBold())          != difcf.IsBold())          Command((f ? "pnb" : "pnb0") + pn2);
	if((f = cf.IsItalic())        != difcf.IsItalic())        Command((f ? "pni" : "pni0") + pn2);
	if((f = cf.IsUnderline())     != difcf.IsUnderline())     Command((f ? "pnul" : "pnul0") + pn2);
	if((f = cf.IsStrikeout())     != difcf.IsStrikeout())     Command((f ? "pnstrike" : "pnstrike0") + pn2);
	if((f = cf.capitals)          != difcf.capitals)          Command((f ? "pncaps" : "pncaps0") + pn2);
	if((t = used_ink.Get(cf.ink)) != used_ink.Get(difcf.ink)) Command(("pncf") + pn2, t);
	if((t = used_paper.Get(cf.paper)) != used_paper.Get(difcf.paper)) Command(("pncb") + pn2, t);
#ifdef PLATFORM_WIN32
	if(!pn && cf.language         != difcf.language)          Command("lang", GetLanguageLCID(cf.language));
#endif
	// todo: link
	return stream.GetPos() != pos;
}

void RTFEncoder::PutObject(const RichObject& object)
{
	Size log_size = object.GetPhysicalSize(); // / 6;
	Size out_size = object.GetSize();
	Size out_size_01mm = iscale(out_size, 254, 60);
	if(log_size.cx <= 0 || log_size.cy <= 0) log_size = out_size / 6;
	Group pict_grp(this, "pict");
	Command("picw", out_size_01mm.cx);
	Command("pich", out_size_01mm.cy);
	Command("picwgoal", DotTwips(out_size.cx));
	Command("pichgoal", DotTwips(out_size.cy));
	Command("pngblip");
	RichObjectPaintInfo pi;
	pi.ink = charfmt.ink;
	PutBinHex(PNGEncoder().SaveString(object.ToImage(out_size, pi)));
}

bool RTFEncoder::PutParaFormat(const RichPara::Format& pf, const RichPara::Format& difpf)
{
	int64 pos = stream.GetPos();
	if(pf.align != difpf.align)
		switch(pf.align) {
		case ALIGN_NULL:
		case ALIGN_LEFT:    Command("ql"); break;
		case ALIGN_CENTER:  Command("qc"); break;
		case ALIGN_RIGHT:   Command("qr"); break;
		case ALIGN_JUSTIFY: Command("qj"); break;
		default:            NEVER();
		}
	int oind = difpf.indent, olm = difpf.lm;
	if(difpf.bullet != RichPara::BULLET_NONE) {
		olm += oind;
		oind = -oind;
	}
	int nind = pf.indent, nlm = pf.lm;
	if(pf.bullet != RichPara::BULLET_NONE) {
		nlm += nind;
		nind = -nind;
	}
//	if(nind      != oind)         Command("fi", DotTwips(nind));
	if(nlm       != olm)          Command("li", DotTwips(nlm));
	if(pf.rm     != difpf.rm)     Command("ri", DotTwips(pf.rm));
	if(pf.newpage)
		Command("pagebb");
	if(pf.before != difpf.before) Command("sb", DotTwips(pf.before));
	if(pf.after  != difpf.after)  Command("sa", DotTwips(pf.after));
	if(pf.tab    != difpf.tab)    PutTabs(pf.tab);
	Command("fi", DotTwips(nind)); // we always put number last as quick patch to "space at bullet" problem
	return stream.GetPos() != pos;
}

void RTFEncoder::PutTabs(const Vector<RichPara::Tab>& tabs)
{
	for(int i = 0; i < tabs.GetCount(); i++) {
		RichPara::Tab t = tabs[i];
		switch(t.align) {
		case ALIGN_NULL:
		case ALIGN_LEFT:   break;
		case ALIGN_CENTER: Command("tqc"); break;
		case ALIGN_RIGHT:  Command("tqr"); break;
		default: NEVER();
		}
		switch(t.fillchar) {
		case 0: break;
		case 1: Command("tldot"); break;
		case 2: Command("tlhyph"); break;
		case 3: Command("tlul"); break;
		default: NEVER();
		}
		Command("tx", DotTwips(t.pos));
	}
}

void RTFEncoder::PutParts(const Array<RichPara::Part>& parts,
	RichPara::CharFormat& base, int bpart, int boff, int epart, int eoff)
{
	if(epart >= parts.GetCount()) {
		epart = parts.GetCount() - 1;
		eoff = INT_MAX;
	}
	ASSERT(boff >= 0 && bpart >= 0);
	for(int p = bpart; p <= epart; p++) {
		const RichPara::Part& part = parts[p];
		if(part.IsText()) {
			WString px = part.text;
			if(p == epart && px.GetLength() > eoff)
				px.Trim(eoff);
			if(p == bpart)
				px.Remove(0, boff);
			if(!px.IsEmpty()) {
				if(PutCharFormat(part.format, base, false) || p == 0)
					Space();
				base = part.format;
				PutText(px);
			}
		}
		else if(part.object)
			PutObject(part.object);
	}

}

void RTFEncoder::PutHeader()
{
	struct CodePage {
		byte  charset;
		short ansicpg;
		short fontcpg;
	};
	
	static const CodePage ansicpg[] = {
		{ CHARSET_WIN1250, 1250, 238 },
		{ CHARSET_WIN1251, 1251, 204 },
		{ CHARSET_WIN1252, 1252,   0 },
		{ CHARSET_WIN1253, 1253, 161 },
		{ CHARSET_WIN1254, 1254, 162 },
		{ CHARSET_WIN1255, 1255, 177 },
		{ CHARSET_WIN1256, 1256, 178 },
		{ CHARSET_WIN1257, 1257, 186 },
		{ CHARSET_WIN1258, 1258, 163 },
	};
	int fontcpg = Null;
	for(int i = __countof(ansicpg); --i >= 0;)
		if(charset == ansicpg[i].charset) {
			Command("ansicpg", ansicpg[i].ansicpg);
			fontcpg = ansicpg[i].fontcpg;
			break;
		}

	Command("deff", 0);
	{
		Group ftbl(this, "fonttbl");
		for(int i = 0; i < used_faces.GetCount(); i++) {
			Group fnt(this, "f", i);
			int fn = used_faces[i];
			int cs = fontcpg;
			dword info = Font::GetFaceInfo(fn);
#ifdef PLATFORM_WIN32
			if(fn == Font::SYMBOL) {
				Command("ftech");
				cs = 2;
			}
			else
#endif
			if(info & Font::FIXEDPITCH)
				Command("fmodern");
			else if(fn == Font::ROMAN)
				Command("froman");
			else if(fn == Font::ARIAL
#ifdef PLATFORM_WIN32
			|| fn == Font::TAHOMA
#endif
			)
				Command("fswiss");
			if(!IsNull(fontcpg))
				Command("fcharset", cs);
			Space();
			stream.Put(Font::GetFaceName(fn));
			stream.Put(';');
		}
	}

	if(phys_colors.GetCount() > 1) {
		Group ctbl(this, "colortbl");
		stream.Put(';');
		for(int i = 1; i < phys_colors.GetCount(); i++) {
			Color rgb = phys_colors[i];
			Command("red",   rgb.GetR());
			Command("green", rgb.GetG());
			Command("blue",  rgb.GetB());
			stream.Put(';');
		}
	}

	Begin("stylesheet");
		RichPara::CharFormat empcfmt;
		RichPara::Format emppfmt;

		for(int i = 0; i < richtext.GetStyleCount(); i++) {
			const RichStyle& style = richtext.GetStyle(i);
			Begin("s", i);
				PutParaFormat(style.format, emppfmt);
				PutCharFormat(style.format, empcfmt, false);
				Command("sbasedon", 222);
				if(style.next != richtext.GetStyleId(i))
					Command("snext", styleid.Find(style.next));
				Space();
				for(const char *n = style.name; *n; n++)
					stream.Put(*n == ';' ? '_' : *n);
				stream.Put(';');
			End();
		}
	End();
	
	Command("paperw", DotTwips(dot_page_size.cx));
	Command("paperh", DotTwips(dot_page_size.cy));
	Command("margl", DotTwips(dot_margins.left));
	Command("margr", DotTwips(dot_margins.right));
	Command("margt", DotTwips(dot_margins.top));
	Command("margb", DotTwips(dot_margins.bottom));
}

void RTFEncoder::PutTable(const RichTable& table, int nesting, int dot_width)
{
	Vector<int> vspan_counts;
	const RichTable::Format& tfmt = table.GetFormat();
	Vector<int> column_pos;
	column_pos.SetCount(tfmt.column.GetCount() + 1);
	column_pos[0] = tfmt.lm;
	int sum = 0;
	for(int c = 0; c < tfmt.column.GetCount(); c++)
		sum += tfmt.column[c];
	int rem_width = dot_width - tfmt.lm - tfmt.rm;
	for(int c = 0; c < tfmt.column.GetCount(); c++) {
		int part = iscale(tfmt.column[c], rem_width, max(sum, 1));
		sum -= tfmt.column[c];
		rem_width -= part;
		column_pos[c + 1] = column_pos[c] + part;
	}
	for(int r = 0; r < table.GetRows(); r++) {
		StringBuffer rowfmt;
		if(nesting)
			rowfmt << "\\*\\nesttableprops";
		rowfmt << "\\trowd"
			"\\trleft" << DotTwips(tfmt.lm)
			// trbrdr[ltrbv]
			;
		Vector<int> cellindex;
		Rect dflt_margin(600, 600, 600, 600);
		for(int c = 0; c < table.GetColumns(); c++) {
			const RichCell& cell = table[r][c];
			dflt_margin.left = min(dflt_margin.left, cell.format.margin.left);
			dflt_margin.top = min(dflt_margin.top, cell.format.margin.top);
			dflt_margin.right = min(dflt_margin.right, cell.format.margin.right);
			dflt_margin.bottom = min(dflt_margin.bottom, cell.format.margin.bottom);
			c += cell.hspan;
		}
/*
		rowfmt <<
			"\\trpaddl" << DotTwips(dflt_margin.left) << "\\trpaddfl3"
			"\\trpaddt" << DotTwips(dflt_margin.top) << "\\trpaddft3"
			"\\trpaddr" << DotTwips(dflt_margin.right) << "\\trpaddfr3"
			"\\trpaddb" << DotTwips(dflt_margin.bottom) << "\\trpaddfb3";
*/
		bool istop = (r == 0);
		bool isbottom = (r == table.GetRows() - 1);
		for(int c = 0; c < table.GetColumns(); c++) {
			const RichCell& cell = table[r][c];
/*
			if(cell.format.margin.left != dflt_margin.left)
				rowfmt << "\\clpadl" << DotTwips(cell.format.margin.left) << "\\clpadfl3";
			if(cell.format.margin.top != dflt_margin.top)
				rowfmt << "\\clpadt" << DotTwips(cell.format.margin.top) << "\\clpadft3";
			if(cell.format.margin.right != dflt_margin.right)
				rowfmt << "\\clpadr" << DotTwips(cell.format.margin.right) << "\\clpadfr3";
			if(cell.format.margin.bottom != dflt_margin.bottom)
				rowfmt << "\\clpadb" << DotTwips(cell.format.margin.bottom) << "\\clpadfb3";
*/
			switch(cell.format.align) {
				case ALIGN_TOP: rowfmt << "\\clvertalt"; break;
				default:
				case ALIGN_CENTER: rowfmt << "\\clvertalc"; break;
				case ALIGN_BOTTOM: rowfmt << "\\clvertalb"; break;
			}
			bool isleft = (c == 0);
			cellindex.Add(c);
			if(cell.hspan)
				c += cell.hspan;
			bool isright = (c + 1 == table.GetColumns());
			int cell_end = column_pos[c + 1];
			if(cell.vspan) {
				vspan_counts.At(c, 0) = cell.vspan;
				rowfmt << "\\clvmgf";
			}
			else if(c < vspan_counts.GetCount() && vspan_counts[c] > 0)
				rowfmt << "\\clvmrg";
			if(!IsNull(cell.format.color))
				rowfmt << "\\clcbpat" << phys_colors.Find(cell.format.color);
			int lb, tb, rb, bb;
			Color lc, tc, rc, bc;
			const RichTable::Format& tf = table.GetFormat();
			lc = tc = rc = bc = tf.gridcolor;
			lb = tb = rb = bb = (!IsNull(tf.gridcolor) ? tf.grid : 0);
			int fw = (!IsNull(tf.framecolor) ? tf.frame : 0);
			if(isleft) {
				lb = fw;
				lc = tf.framecolor;
			}
			if(isright) {
				rb = fw;
				rc = tf.framecolor;
			}
			if(istop) {
				tb = fw;
				tc = tf.framecolor;
			}
			if(isbottom) {
				bb = fw;
				bc = tf.framecolor;
			}
			if(!IsNull(cell.format.bordercolor)) {
				if(cell.format.border.left >= max(lb, 1)) {
					lb = cell.format.border.left;
					lc = cell.format.bordercolor;
				}
				if(cell.format.border.top >= max(tb, 1)) {
					tb = cell.format.border.top;
					tc = cell.format.bordercolor;
				}
				if(cell.format.border.right >= max(rb, 1)) {
					rb = cell.format.border.right;
					rc = cell.format.bordercolor;
				}
				if(cell.format.border.bottom >= max(bb, 1)) {
					bb = cell.format.border.bottom;
					bc = cell.format.bordercolor;
				}
			}
			PutBorder(rowfmt, lb, lc, "clbrdrl");
			PutBorder(rowfmt, tb, tc, "clbrdrt");
			PutBorder(rowfmt, rb, rc, "clbrdrr");
			PutBorder(rowfmt, bb, bc, "clbrdrb");
			rowfmt << "\\cellx" << DotTwips(cell_end);
		}
		Begin();
		Command("pard");
		Command("intbl");
		parafmt = RichPara::Format();
		charfmt = RichPara::CharFormat();
		if(nesting)
			Command("itap", nesting + 1);
		String fmtstr = rowfmt;
		if(!nesting)
			stream.Put(fmtstr);
		for(int c = 0; c < cellindex.GetCount(); c++) {
			int cx = cellindex[c];
			const RichCell& cell = table[r][cx];
			int cell_wd = column_pos[cx + cell.hspan + 1] - column_pos[cx];
			PutTxt(cell.text, nesting + 1, cell_wd);
			Command(nesting ? "nestcell" : "cell");
		}
		if(nesting)
			stream.Put(fmtstr);
		Command(nesting ? "nestrow" : "row");
		End();
	}
	oldstyle = Null;
}

void RTFEncoder::PutBorder(StringBuffer& rowfmt, int wd, Color c, const char *clpar)
{
	if(wd > 0) { 
		rowfmt << "\\" << clpar << "\\brdrs\\brdrw" << wd << "\\brdrcf"
		<< phys_colors.Find(Nvl(c, Black())) << " ";
	}
}

void RTFEncoder::PutTxt(const RichTxt& txt, int nesting, int dot_width)
{
	for(int i = 0; i < txt.GetPartCount(); i++) {
		if(txt.IsTable(i))
			PutTable(txt.GetTable(i), nesting, dot_width);
		else {
			const RichPara& para = txt.Get(i, richtext.GetStyles());
			Uuid pstyle = txt.GetParaStyle(i);
			int new_para_ht = GetParaHeight(para.part);
			int first_part = 0, first_ind = 0;
			bool hdiff = (para.format.bullet != RichPara::BULLET_NONE
				&& para.format.bullet != RichPara::BULLET_TEXT && new_para_ht != para_ht);
			if(pstyle != oldstyle
			|| para.format.bullet != parafmt.bullet || para.format.bullet == RichPara::BULLET_TEXT
			|| hdiff || para.format.tab != parafmt.tab || para.format.newpage || parafmt.newpage) {
				Command("s", styleid.Find(pstyle));
				oldstyle = pstyle;
				parafmt = richtext.GetStyle(pstyle).format;
				Command("pard");
				if(nesting)
					Command("intbl");
				if(nesting > 1)
					Command("itap", nesting);
//				parafmt = RichPara::Format();
//				bool isul = charfmt.IsUnderline();
//				charfmt = RichPara::CharFormat();
//				charfmt.Underline(isul);
				para_ht = 0;
			}
			if(para.format.bullet == RichPara::BULLET_TEXT) {
				int epart = 0, eoff = 0;
				while(epart < para.part.GetCount() && (eoff = para.part[epart].text.Find('\t')) < 0)
					epart++;

				RichPara::CharFormat rcf = charfmt;
				rcf.Height(0);
				Begin("pntext");
					PutParts(para.part, rcf, 0, 0, epart, eoff);
					stream.Put('\t');
				End();

				Begin("*\\pn");
					Command("pnhang");
					Command("pnql");
					rcf = charfmt;
					if(epart > 0 || eoff > 0)
						PutCharFormat(para.part[0].format, rcf, true);
					Begin("pntxta");
					End();
					Begin("pntxtb");
						Space();
						PutParts(para.part, rcf, 0, 0, epart, eoff);
						stream.Put('\t');
					End();
				End();
			}
			else if(para.format.bullet != RichPara::BULLET_NONE) {
				int sym_face = SYMBOL_INDEX;
				byte sym_char = 0xB7;

				switch(para.format.bullet) {
				default:
					NEVER();

				case RichPara::BULLET_ROUND:      sym_face = SYMBOL_INDEX;    sym_char = 0xB7; break;
				case RichPara::BULLET_ROUNDWHITE: sym_face = WINGDINGS_INDEX; sym_char = 0xA1; break;
				case RichPara::BULLET_BOX:        sym_face = WINGDINGS_INDEX; sym_char = 'n'; break;
				case RichPara::BULLET_BOXWHITE:   sym_face = WINGDINGS_INDEX; sym_char = 'o'; break;
				}

				Begin("pntext");
	//					Command("tqr");
	//					Command("tx", DotTwips(para.format.indent));
	//					Command("qr");
					Command("f", sym_face);
					Command("fs", DotPoints(2 * new_para_ht));
					Space();
					stream.Put(sym_char);
					stream.Put('\t');
				End();

				Begin("*\\pn");
					Command("pnlvlblt");
					Command("pnf", sym_face);
					Command("pnfs", DotPoints(2 * new_para_ht));
					Command("pnql");
					Begin("pntxtb");
						Space();
						stream.Put(sym_char);
	//						stream.Put('\t');
					End();
					Begin("pntxta");
					End();
				End();
			}
			PutParaFormat(para.format, parafmt);
			para_ht = new_para_ht;
			parafmt = para.format;
			PutParts(para.part, charfmt, first_part, first_ind, para.part.GetCount(), 0);
			if(i + 1 < txt.GetPartCount())
				Command("par");
		}
	}
}

void RTFEncoder::PutDocument()
{
	charfmt.Height(PointDots(0));
	charfmt.language = 0;
	old_ht = DotPoints(2 * tabs(charfmt.GetHeight()));
	para_ht = 0;
	oldstyle = Uuid::Create();
	PutTxt(richtext, 0, dot_page_size.cx - dot_margins.left - dot_margins.right);
}

}
