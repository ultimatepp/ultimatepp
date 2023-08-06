#include "RichText.h"

#define LLOG(x) // DLOG(x)

namespace Upp {

const byte html_colors_z[] = {
	120,156,93,86,107,186,171,42,12,157,10,67,176,62,113,56,129,132,202,173,130,27,117,247,235,25,253,13,143,218,186,229,143,89,96,12,201,90,1,53,131,126,136,42,61,98,179,243,47,5,161,171,56,196,61,192,75,200,42,14,241,156,236,78,194,164,71,44,16,188,119,60,149,62,10,132,60,145,94,215,35,172,51,165,9,254,198,28,122,218,44,164,73,254,234,30,136,156,136,51,188,114,182,11,137,8,243,187,159,237,47,165,31,177,241,162,121,246,207,244,39,182,28,252,190,82,112,236,78,205,7,165,119,246,181,19,204,201,21,227,240,115,64,114,197,56,204,86,83,90,104,42,35,35,224,118,251,115,80,9,31,72,225,112,253,128,223,121,59,214,145,24,216,198,86,192,191,35,164,207,227,180,34,123,103,163,51,29,106,161,236,246,19,61,27,106,117,203,225,124,37,142,13,167,39,66,152,23,239,98,58,72,105,188,68,28,223,127,173,159,105,23,18,106,69,181,80,193,63,157,128,174,134,26,132,58,194,252,122,122,143,2,73,73,57,8,13,72,123,114,208,153,145,160,18,122,
	130,176,7,58,182,28,105,21,17,175,253,12,188,49,172,251,241,70,66,251,192,105,49,102,48,93,21,13,103,56,149,20,146,151,190,29,59,46,84,68,185,202,143,152,96,201,123,210,193,46,27,215,18,245,173,109,216,124,129,123,167,6,33,60,206,29,72,149,236,50,47,85,177,239,126,70,114,129,195,230,160,251,170,128,145,55,48,198,81,236,92,248,190,229,160,11,112,89,240,152,224,97,133,66,53,244,217,195,2,119,114,59,8,169,206,63,39,146,100,79,93,215,171,58,199,199,27,118,177,64,70,234,226,219,7,61,89,20,227,216,212,90,39,36,242,51,58,42,11,182,84,35,65,227,216,15,144,17,130,236,88,26,165,101,118,188,197,188,166,221,183,178,193,18,67,2,211,238,106,211,242,248,6,233,2,238,71,248,57,188,221,98,242,52,225,45,129,165,252,35,167,1,27,174,51,173,171,117,177,18,183,118,204,246,246,120,149,140,171,84,1,187,164,191,245,99,28,217,164,143,233,241,94,138,123,163,49,114,204,216,64,42,88,166,165,170,227,35,184,254,204,136,83,187,96,88,
	148,62,208,182,231,253,214,181,84,113,213,31,157,130,117,155,242,193,51,41,226,16,247,201,111,123,113,34,147,170,98,217,121,57,14,156,211,15,5,16,152,204,215,182,113,209,124,50,138,190,1,141,169,83,79,56,215,78,222,209,11,233,153,181,23,237,189,228,167,31,85,43,172,67,11,46,22,83,99,167,59,157,128,187,23,109,228,72,45,236,175,15,175,220,160,42,145,9,101,42,234,165,22,51,252,146,67,110,107,212,83,111,224,180,57,115,219,20,191,168,76,199,224,211,229,96,7,109,34,151,102,98,150,48,149,140,97,178,196,220,177,158,103,123,159,178,38,1,81,82,159,129,162,186,188,137,140,68,149,80,86,81,2,206,28,189,187,27,24,192,186,204,197,116,97,19,199,27,136,97,140,21,113,81,79,228,186,164,228,69,245,250,150,129,66,106,14,179,98,82,103,232,205,234,186,82,53,188,193,66,48,57,104,50,111,236,100,245,48,72,57,142,23,148,254,160,59,209,156,60,168,74,183,72,25,253,234,217,116,109,237,241,61,71,193,114,196,38,110,217,177,17,153,200,233,123,
	203,188,16,239,122,172,44,132,246,88,190,250,115,223,107,102,88,193,207,198,196,101,201,72,17,190,130,174,227,52,101,172,156,71,99,51,84,168,10,118,230,165,209,170,25,110,111,244,212,251,160,122,73,244,134,87,254,245,253,205,98,195,29,171,76,124,4,222,74,188,113,167,201,112,150,120,34,233,112,235,100,39,22,139,238,100,205,109,188,141,3,239,204,186,93,7,130,37,30,44,92,50,6,182,253,21,252,150,207,22,226,152,188,214,176,89,151,108,213,197,99,16,254,243,111,29,35,1,94,78,70,102,23,31,70,60,133,166,227,180,94,206,211,100,96,0,37,122,37,169,110,196,217,50,161,139,211,201,202,71,120,155,129,148,70,4,78,89,47,86,152,233,35,112,34,146,92,129,4,102,146,114,199,28,101,2,62,9,1,195,235,40,129,159,116,112,111,175,184,201,173,176,194,11,120,35,107,220,154,193,78,172,4,122,90,143,216,189,12,130,26,25,8,7,75,92,118,141,17,133,231,124,27,81,98,157,143,69,32,66,133,40,86,255,196,210,250,84,69,145,74,215,155,7,183,65,
	226,12,22,176,239,155,134,9,252,117,79,225,92,191,242,241,27,27,62,183,252,224,95,144,121,221,222,250,145,43,176,177,194,103,202,107,164,106,187,91,35,222,26,3,89,13,53,91,14,139,15,211,66,219,243,229,233,20,28,73,213,13,209,222,38,86,70,212,69,199,9,217,44,57,7,2,170,174,174,241,207,85,235,91,151,164,196,135,141,61,116,177,243,124,105,148,247,199,125,225,75,158,5,112,89,129,220,89,196,31,214,242,125,64,124,132,219,246,178,230,118,186,115,139,66,126,225,6,249,125,153,218,39,230,34,231,140,143,60,131,82,236,126,129,221,199,22,220,180,131,248,34,61,103,29,43,81,78,52,166,69,205,27,124,78,4,59,115,154,111,48,205,245,194,152,140,109,241,143,124,151,226,126,123,189,230,101,171,112,10,98,163,248,31,84,48,144,29,
};

struct TrivialHtmlParser {
	XmlParser  p;
	Font       base_font;
	RichPara   para;
	RichText   richtext;
	int        prev_margin = 0; // for margin collapsing
	int        parent_tag_margin = 0; // for margin (de)collapsing
	bool       wastext = false;

	struct Style : RichPara::Format {
		int  border = 0;
		Rect padding = Rect(0, 0, 0, 0);
		Rect margin = Rect(0, 0, 0, 0);
		int  bullet_style = RichPara::BULLET_ROUND;
		bool pre = false;
	};

	static VectorMap<String, Color>  named_color;
	static VectorMap<String, String> div;
	static VectorMap<String, String> span;
	
	void ReadStyle(Style& fmt, const String& style);
	void AddPara(const Style& fmt);
	void Parse(Style fmt);
	
	TrivialHtmlParser(const char *html);
};

VectorMap<String, Color> TrivialHtmlParser::named_color;
VectorMap<String, String> TrivialHtmlParser::div;
VectorMap<String, String> TrivialHtmlParser::span;

TrivialHtmlParser::TrivialHtmlParser(const char *html)
	: p(html) {
	ONCELOCK {
		div
			("div", "")
			("h1", "font-size: 2em; margin-top: 0.67em; margin-bottom: 0.67em; margin-left: 0; margin-right: 0; font-weight: bold;")
			("h2", "font-size: 1.5em; margin-top: 0.83em; margin-bottom: 0.83em; margin-left: 0; margin-right: 0; font-weight: bold;")
			("h3", "font-size: 1.17em; margin-top: 1em; margin-bottom: 1em; margin-left: 0; margin-right: 0; font-weight: bold;")
			("h4", "margin-top: 1.33em; margin-bottom: 1.33em; margin-left: 0; margin-right: 0; font-weight: bold;")
			("h5", "font-size: .83em; margin-top: 1.67em; margin-bottom: 1.67em; margin-left: 0; margin-right: 0; font-weight: bold;")
			("h6", "font-size: .67em; margin-top: 2.33em; margin-bottom: 2.33em; margin-left: 0; margin-right: 0; font-weight: bold;")
			("p", "margin-top: 1em; margin-bottom: 1em;")
			("dt", "margin-top: 1em; margin-bottom: 1em;")
			("dd", "margin-left: 40px")
			("dl", "")
			("hr", "margin-top: 0.5em; margin-bottom: 0.5em; border-width: 1px;")
			("li", "display: list-item; margin-top: 0; margin-bottom: 0;")
			("ol", "list-style-type: decimal; margin-top: 1em; margin-bottom: 1em; margin-left: 0; margin-right: 0; padding-left: 40px;")
			("ul", "list-style-type: disc; margin-top: 1em; margin-bottom: 1em; margin-left: 0; margin-right: 0; padding-left: 40px;")
			("pre", "font-family: monospace; white-space: pre; margin: 1em 0;")
			("figure", "margin-top: 1em; margin-bottom: 1em; margin-left: 40px; margin-right: 40px;")
			("blockquote", "margin-top: 1em; margin-bottom: 1em; margin-left: 40px; margin-right: 40px;")
			("legend", "padding-left: 2px; padding-right: 2px;")
		;
		
		span
			("code", "font-family:monospace")
			("b", "font-weight:bold")
			("strong", "font-weight:bold")
			("i", "font-style:italic")
			("cite", "font-style:italic")
			("em", "font-style:italic")
			("u", "text-decoration:underline")
			("strike", "text-decoration:line-through")
			("s", "text-decoration:line-through")
			("sub", "vertical-align:sub")
			("sup", "vertical-align:super")
		;

		named_color.Add("transparent", Null);
		String html_colors = ZDecompress(html_colors_z, sizeof(html_colors_z));
		CParser p(html_colors);
		while(!p.IsEof()) {
			String id = p.ReadId();
			dword n = p.ReadNumber(16);
			named_color.Add(id, Color((byte)(n >> 16), (byte)(n >> 8), (byte)(n)));
		}
	}
	p.PreserveAllWhiteSpaces();
	p.Relaxed();
	p.RegisterEntity("nbsp", WString(0xa0, 1).ToString());
}

void TrivialHtmlParser::ReadStyle(Style& fmt, const String& style)
{
	Style parent_fmt = fmt; // TODO: BETTER!
	try {
		CParser p(style);
		auto Do = [&](auto x) {
			while(!p.IsChar(';') && !p.IsEof()) {
				if(!x())
					p.Skip();
			}
		};
		auto ReadLength = [&](double& l, double psize = 2000) {
			if(p.IsDouble()) {
				l = p.ReadDoubleNoE();
				if(p.Char('%'))
					l *= l * psize / 100;
				else
				if(p.Id("em"))
					l *= parent_fmt.GetHeight();
				else
				if(p.Id("rem"))
					l *= base_font.GetHeight();
				else
				if(p.Id("rlh"))
					l *= base_font.GetLineHeight();
				else
				if(p.Id("lh"))
					l *= fmt.GetLineHeight();
				else
				if(p.Id("ch"))
					l *= parent_fmt['0'];
				else
				if(p.Id("ex"))
					l *= parent_fmt.GetHeight() * 0.5;
				else
				if(p.Id("ic"))
					l *= parent_fmt[0x6C34];
				else
				if(p.Id("cm"))
					l *= 600 / 2.54;
				else
				if(p.Id("mm"))
					l *= 60 / 2.54;
				else
				if(p.Id("qm"))
					l *= 60 / 2.54 / 4;
				else
				if(p.Id("in"))
					l *= 600;
				else
				if(p.Id("pt"))
					l *= 600 / 72.0;
				else
				if(p.Id("px"))
					l *= 600 / 96.0;
				else
					l *= psize;
				l = clamp(abs(l), 0., 2000.);
				return true;
			}
			return false;
		};
		auto ReadLengthPar = [&](int& tsize, double psize = 2000) {
			double l;
			if(ReadLength(l, psize))
				tsize = (int)l;
		};
		auto ReadLengthParAdd = [&](int& tsize, double psize = 2000) {
			int n = 0;
			ReadLengthPar(n, psize);
			tsize += n;
		};
		auto ReadColor = [&](Color& c) {
			if(p.IsId()) {
				int q = named_color.Find(p.ReadId());
				if(q >= 0) {
					c = named_color[q];
					return true;
				}
			}
			else
			if(p.Char('#')) {
				dword n = p.ReadNumber(16);
				c = Color((byte)(n >> 16), (byte)(n >> 8), (byte)(n));
				return true;
			}
			return false;
		};
		auto TextAlign = [&] {
			if(p.Id("start") || p.Id("left")) // TODO RTL
				fmt.align = ALIGN_LEFT;
			else
			if(p.Id("end") || p.Id("right")) // TODO RTL
				fmt.align = ALIGN_RIGHT;
			else
			if(p.Id("center"))
				fmt.align = ALIGN_CENTER;
			else
			if(p.Id("justify") || p.Id("justify-all"))
				fmt.align = ALIGN_JUSTIFY;
			else
				return false;
			return true;
		};
		auto TextDecoration = [&] {
			if(p.Id("underline"))
				fmt.Underline();
			else
			if(p.Id("line-through"))
				fmt.Strikeout();
			else
			if(p.Id("none")) {
				fmt.Underline(false);
				fmt.Strikeout(false);
			}
			else
				return false;
			return true;
		};
		auto FontSize = [&] {
			double l;
			if(ReadLength(l, parent_fmt.GetHeight()))
				fmt.Height((int)l);
			else {
				l = base_font.GetHeight();
				if(p.Id("xx-small"))
					l *= 0.512;
				else
				if(p.Id("x-small"))
					l *= 0.64;
				else
				if(p.Id("small"))
					l *= 0.8;
				else
				if(p.Id("medium"))
					;
				else
				if(p.Id("large"))
					l *= 1.25;
				else
				if(p.Id("x-large"))
					l *= 1.5;
				else
				if(p.Id("xx-large"))
					l *= 1.8;
				else
				if(p.Id("xxx-large"))
					l *= 2.16;
				else {
					l = parent_fmt.GetHeight();
					if(p.Id("smaller"))
						l *= 0.8;
					else
					if(p.Id("larger"))
						l *= 1.25;
					else
						return false;
				}
				fmt.Height((int)l);
			}
			return true;
		};
		auto FontStyle = [&] {
			if(p.Id("normal"))
				fmt.Italic(false);
			else
			if(p.Id("italic") || p.Id("oblique"))
				fmt.Italic();
			else
				return false;
			return true;
		};
		auto FontWeight = [&] {
			if(p.Id("normal") || p.Id("lighter"))
				fmt.Bold(false);
			else
			if(p.Id("bold") || p.Id("bolder"))
				fmt.Bold();
			else
			if(p.IsDouble())
				fmt.Bold(p.ReadDouble() >= 500);
			else
				return false;
			return true;
		};
		auto FontFamily = [&] {
			if(p.Id("serif"))
				fmt.Face(Font::SERIF);
			else
			if(p.Id("sans-serif"))
				fmt.Face(Font::SANSSERIF);
			else
			if(p.Id("monospace"))
				fmt.Face(Font::MONOSPACE);
			else
				return false;
			return true;
		};
		auto FontVariant = [&] {
			if(!p.Id("small-caps"))
				return false;
			fmt.capitals = true;
			return true;
		};
		auto LineHeight = [&] {
			double l;
			if(p.Id("normal"))
				fmt.linespacing = RichPara::LSP115;
			if(ReadLength(l, fmt.GetHeight())) {
				l /= fmt.GetHeight();
				fmt.linespacing = l < 1.1 ? RichPara::LSP10 :
					              l < 1.4 ? RichPara::LSP115 :
					              l < 1.8 ? RichPara::LSP15 :
					              RichPara::LSP20;
				return true;
			}
			return false;
		};
		auto ReadLengthPar4 = [&](Rect& r) {
			Vector<double> m;
			for(int i = 0; i < 4; i++) {
				double x;
				if(ReadLength(x, 2000))
					m.Add(x);
				else
					break;
			}
			switch(m.GetCount()) {
			case 1: r.left = r.right = r.top = r.bottom = (int)m[0]; break;
			case 2: r.left = r.right = (int)m[1]; r.top = r.bottom = (int)m[0]; break;
			case 3: r.left = r.right = (int)m[1]; r.top = (int)m[0]; r.bottom = (int)m[2]; break;
			case 4: r.left = (int)m[3]; r.right = (int)m[1]; r.top = (int)m[0]; r.bottom = (int)m[2]; break;
			default:;
			}
		};
		while(!p.IsEof()) {
			try {
				auto Attr = [&](const char *s) {
					if(p.Id(s)) {
						p.Char(':');
						return true;
					}
					return false;
				};
				if(Attr("text-decoration") || Attr("text-decoration-line"))
					Do([&] { return TextDecoration(); });
				else
				if(Attr("font-variant")) {
					if(!FontVariant())
						fmt.capitals = false;
				}
				else
				if(Attr("font-family"))
					Do([&] { return FontFamily(); });
				else
				if(Attr("line-height"))
					LineHeight();
				else
				if(Attr("text-align")) {
					p.Char(':');
					TextAlign();
				}
				else
				if(Attr("border-width"))
					ReadLengthPar(fmt.border);
				else
				if(Attr("text-indent"))
					ReadLengthPar(fmt.indent);
				else
				if(Attr("margin-left"))
					ReadLengthParAdd(fmt.margin.left);
				else
				if(Attr("margin-right"))
					ReadLengthParAdd(fmt.margin.right);
				else
				if(Attr("margin-top"))
					ReadLengthPar(fmt.margin.top);
				else
				if(Attr("margin-bottom"))
					ReadLengthPar(fmt.margin.bottom);
				else
				if(Attr("margin"))
					ReadLengthPar4(fmt.margin);
				else
				if(Attr("border-width"))
					ReadLengthPar(fmt.border);
				else
				if(Attr("text-indent"))
					ReadLengthPar(fmt.indent);
				else
				if(Attr("padding-left"))
					ReadLengthParAdd(fmt.padding.left);
				else
				if(Attr("padding-right"))
					ReadLengthParAdd(fmt.padding.right);
				else
				if(Attr("padding-top"))
					ReadLengthPar(fmt.padding.top);
				else
				if(Attr("padding-bottom"))
					ReadLengthPar(fmt.padding.bottom);
				else
				if(Attr("padding"))
					ReadLengthPar4(fmt.padding);
				else
				if(Attr("font-weight"))
					FontWeight();
				else
				if(Attr("font-size"))
					FontSize();
				else
				if(Attr("font-style"))
					FontStyle();
				else
				if(Attr("font"))
					while(!p.IsChar(';') && !p.IsEof()) {
						if(!FontVariant() && !FontSize() && !FontStyle() && !FontFamily() && !FontWeight()) { // order matters!
							if(p.Char('/'))
								LineHeight();
							else
								p.Skip();
						}
					}
				else
				if(Attr("color"))
					ReadColor(fmt.ink);
				else
				if(Attr("background-color"))
					ReadColor(fmt.paper);
				else
				if(Attr("list-style-type") || Attr("list-style")) {
					if(Attr("disc"))
						fmt.bullet_style = RichPara::BULLET_ROUND;
					else
					if(Attr("circle"))
						fmt.bullet_style = RichPara::BULLET_ROUNDWHITE;
					else
					if(Attr("square"))
						fmt.bullet_style = RichPara::BULLET_BOX;
				}
				else
				if(Attr("display")) {
					if(Attr("list-item")) // TODO: Text, decimal...
						fmt.bullet = fmt.bullet_style;
					else
						fmt.bullet = RichPara::BULLET_NONE;
				}
				else
				if(Attr("vertical-align")) {
					if(Attr("sub"))
						fmt.sscript = 2;
					else
					if(Attr("super"))
						fmt.sscript = 1;
					else
						fmt.sscript = 0;
				}
				else
				if(Attr("white-space"))
					fmt.pre = Attr("pre");
			}
			catch(CParser::Error) {}
			while(!p.Char(';') && !p.IsEof())
				p.Skip();
		}
	}
	catch(CParser::Error) {}
}

void TrivialHtmlParser::AddPara(const Style& fmt)
{
//	para.Dump();
	para.format = fmt;
	para.format.before = max(fmt.margin.top - prev_margin, parent_tag_margin) + fmt.padding.top;
	para.format.after = fmt.margin.bottom + fmt.padding.bottom;
	para.format.lm = fmt.margin.left + fmt.padding.left;
	para.format.rm = fmt.margin.right + fmt.padding.right;
	prev_margin = fmt.margin.bottom;
	parent_tag_margin = 0;
	if(fmt.bullet)
		para.format.indent = fmt.GetHeight();
	LLOG("AddPara " << AsCString(para.GetText().ToString()) << ", before: " << para.format.before << ", after: " << para.format.after);
	richtext.Cat(para);
	para.part.Clear();
	wastext = false;
}

void TrivialHtmlParser::Parse(Style tfmt)
{
	auto Br = [&] {
		Style fmt = tfmt;
		fmt.margin.top = fmt.margin.bottom = fmt.padding.top = fmt.padding.bottom = 0;
		AddPara(fmt);
	};

	while(!p.IsEnd() && !p.IsEof()) {
		if(p.IsText()) {
			String text = p.ReadText();
			if(tfmt.pre) {
				String h;
				for(const char *s = text; *s; s++) {
					if((byte)*s >= ' ' || *s == '\t')
						h.Cat(*s);
					if(*s == '\n') {
						para.Cat(h.ToWString(), tfmt);
						Br();
						h.Clear();
						wastext = true;
					}
				}
				para.Cat(h.ToWString(), tfmt);
			}
			else {
				WString t;
				if(text.GetCount()) {
					WString h = text.ToWString();
					for(const wchar *s = h; *s;) // ignore more than 1 space between words
						if(*s == ' ') {
							s++;
							while(*s == ' ') s++;
							t.Cat(' ');
						}
						else
						if(*s >= ' ') { // ignore other whitespaces
							t.Cat(*s++);
							wastext = true;
						}
						else
							s++;
				}
				if(t.GetCount()) {
					LLOG("Text: " << AsCString(t.ToString()));
					para.Cat(t, tfmt);
				}
			}
		}
		else
		if(p.IsTag()) {
			String tag = ToLower(p.ReadTag());
			if(tag == "br") {
				Br();
				p.End();
			}
			else
			if(tag == "hr") {
				if(wastext)
					AddPara(tfmt);
				para.part.Clear();
				Style fmt = tfmt;
				ReadStyle(fmt, div.Get("hr", ""));
				ReadStyle(fmt, p["style"]);
				int after = fmt.after;
				fmt.Face(Font::SERIF);
				fmt.Height(max(fmt.before, 1));
				fmt.before = fmt.after = 0;
				AddPara(fmt);
				fmt.ruler = fmt.border;
				fmt.Height(max(after, 1));
				AddPara(fmt);
				p.End();
			}
			else {
				LLOG(tag << LOG_BEGIN);
				Style fmt = tfmt;
				int divi = div.Find(tag);
				int parai;
				if(divi >= 0) {
					if(wastext)
						AddPara(tfmt);
					else
						para.part.Clear();
					ReadStyle(fmt, div[divi]);
					parai = richtext.GetPartCount();
					parent_tag_margin = max(parent_tag_margin, fmt.margin.bottom); // margin (de)collapsing
				}
				
				if(tag == "a")
					fmt.link = p["href"];

				int spani = span.Find(tag);
				if(spani >= 0)
					ReadStyle(fmt, span[spani]);
				ReadStyle(fmt, p["style"]);
				Parse(fmt);
				p.End();
				if(divi >= 0 && (wastext || parai == richtext.GetPartCount()))
					AddPara(fmt);
				LLOG("/" << tag << ", wastext: " << wastext << ", waspara: " << (parai == richtext.GetPartCount()) << LOG_END);
			}
		}
		else
			p.Skip();
	}
}

RichText ParseTrivialHtml(const char *html, Font base_font)
{
	TrivialHtmlParser p(html);
	TrivialHtmlParser::Style fmt;
	(Font&)fmt = base_font;
	p.base_font = base_font;
	try {
		p.Parse(fmt);
		p.AddPara(fmt);
	}
	catch(XmlError) {}
	return pick(p.richtext);
}

}