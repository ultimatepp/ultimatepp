#include "www.h"

int DotToPx(int q)
{
	return q * 8 / 40;
}

Size DotToPx(Size sz)
{
	return Size(DotToPx(sz.cx), DotToPx(sz.cy));
}

String HtmlStyleColor(Color c, const char *cl = "color")
{
	return Format(String(cl) + ":#%02x%02x%02x;", c.GetR(), c.GetG(), c.GetB());
}

String HtmlCharStyle(const RichPara::CharFormat& cf, const RichPara::CharFormat& sf)
{
	String style;
	if(cf.ink != sf.ink)
		style = HtmlStyleColor(cf.ink);
	return style + HtmlFontStyle(cf, sf);
}

String HtmlParaStyle(const RichPara::Format& f)
{
	String style;
	int lm = DotToPx(f.lm);
	if(f.bullet && f.bullet != RichPara::BULLET_TEXT) {
		style << "display:list-item;line-style-type:";
		switch(f.bullet) {
		case RichPara::BULLET_ROUND: style << "disc"; break;
		case RichPara::BULLET_ROUNDWHITE: style << "circle"; break;
		case RichPara::BULLET_BOX:
		case RichPara::BULLET_BOXWHITE: style << "square"; break;
		}
		style << ";line-style-position:inside;";
		lm += 20;
	}
	style << Format("margin:%d %d %d %d;text-indent:%d;",
	                DotToPx(f.before),
	                DotToPx(f.rm),
	                DotToPx(f.after),
	                lm,
	                DotToPx(f.indent)
	         );
	style << "text-align:";
	switch(f.align) {
	case ALIGN_LEFT: style << "left;"; break;
	case ALIGN_RIGHT: style << "right;"; break;
	case ALIGN_CENTER: style << "center;"; break;
	case ALIGN_JUSTIFY: style << "justify;"; break;
	}
	style << HtmlStyleColor(f.ink) + HtmlFontStyle(f);
	if(!IsNull(f.paper))
		style << HtmlStyleColor(f.paper, "background-color");
	return style;
}

String FormatClass(Index<String>& css, const String& fmt)
{
	return " CLASS=" + FormatIntAlpha(css.FindAdd(fmt) + 1);
}

String AsHtml(const RichText& text, Index<String>& css, const VectorMap<String, String>& links,
              const String& outdir, const String& namebase)
{
	String html;
	int im = 0;
	for(int i = 0; i < text.GetParaCount(); i++) {
		RichPara p = text.Get(i);
		if(p.format.bullet == RichPara::BULLET_TEXT) {
			html << "<TABLE WIDTH=100% BORDER=0 "
			        "CELLPADDING=2 CELLSPACING=2>"
			        "<TR>";
			int q = DotToPx(p.format.lm) - 8;
			if(q > 0)
				html << Format("<TD WIDTH=%d></TD>", q);
			html << Format("<TD VALIGN=top WIDTH=%d BGCOLOR=#F0F0F0>\r\n",
			               max(DotToPx(p.format.indent), 0));
			p.format.after = p.format.before = p.format.indent = p.format.lm = 0;
		}
		String par = "<P" + FormatClass(css, HtmlParaStyle(p.format)) + ">";
		html << par;
		for(int i = 0; i < p.part.GetCount(); i++) {
			const RichPara::Part& part = p.part[i];
			if(part.object) {
				String name;
				name << namebase << "_" << im++ << ".png";
				Size sz = DotToPx(part.object.GetSize());
				Image img(sz);
				ImageDraw w(img);
				part.object.Paint(w, sz);
				PngEncoder::New()->SaveImageFile(AppendFileName(outdir, name), img);
				html << HtmlSingleTag("IMG").Src(name);
			}
			else {
				String lnk = links.Get(part.format.link, part.format.link);
				if(!lnk.IsEmpty())
					html << "<A HREF=\"" << lnk << "\">";
				String cs;
				if(part.text[0] != 9)
					cs = HtmlCharStyle(part.format, p.format);
				if(!cs.IsEmpty())
					html << "<SPAN" << FormatClass(css, cs) << ">";
				bool spc = false;
				for(const wchar *s = part.text.Begin(); s != part.text.End(); s++) {
					if(*s == ' ') {
						html.Cat(spc ? "&nbsp;" : " ");
						spc = true;
					}
					else {
						spc = false;
						if(*s == 160)
							html.Cat("&nbsp;");
						else
						if(*s == '<')
							html.Cat("&lt;");
						else
						if(*s == '>')
							html.Cat("&gt;");
						else
						if(*s == '&')
							html.Cat("&amp;");
						else
						if(*s == '\"')
							html.Cat("&quot;");
						else
						if(*s == 9) {
							if(p.format.bullet == RichPara::BULLET_TEXT) {
								if(!cs.IsEmpty() && part.text[0] != 9)
									html << "</SPAN>";
								html << "</P>";
								html << "</TD>\r\n<TD VALIGN=top BGCOLOR=#F0F0F0>\r\n";
								html << par;
								if(s[1]) {
									cs = HtmlCharStyle(part.format, p.format);
									if(!cs.IsEmpty())
										html << "<SPAN" << FormatClass(css, cs) << ">";
								}
							}
							else
								html.Cat("&nbsp;&nbsp;&nbsp;&nbsp;");
						}
						else
							html.Cat(*s);
					}
				}
				if(!cs.IsEmpty())
					html << "</SPAN>";
				if(!lnk.IsEmpty())
					html << "</A>";
			}
		}
		if(p.part.GetCount() == 0)
			html << "&nbsp;";
		html << "</P>";
		if(p.format.bullet == RichPara::BULLET_TEXT)
			html << "</TD></TR></TABLE>";
		html << "\r\n";
	}
	return html;
}

String AsCss(Index<String>& ss)
{
	String css;
	for(int i = 0; i < ss.GetCount(); i++) {
		css << "." + FormatIntAlpha(i + 1);
		css << "{" << ss[i] << "}\r\n";
	}
	return css;
}
