#include "RichText.h"

namespace Upp {

String HtmlFontStyle(Font f, Font base)
{
	String style;
	if(f.GetFace() != base.GetFace())
		switch(f.GetFace()) {
		case Font::ARIAL: style = "font-family:sans-serif;"; break;
		case Font::ROMAN: style = "font-family:serif;"; break;
		case Font::COURIER: style = "font-family:monospace;"; break;
		}
	if(f.GetHeight() != base.GetHeight())
		style << Sprintf("font-size:%dpt;", f.GetHeight() * 72 / 600);
	if(f.IsBold() != base.IsBold())
		style << (f.IsBold() ? "font-weight:bold;" : "font-weight:normal;");
	if(f.IsItalic() != base.IsItalic())
		style << (f.IsItalic() ? "font-style:italic;" : "font-style:normal;");
	return style;
}

String HtmlFontStyle(Font f)
{
	String style;
	switch(f.GetFace()) {
		case Font::ARIAL: style = "font-family:sans-serif;"; break;
		case Font::ROMAN: style = "font-family:serif;"; break;
		case Font::COURIER: style = "font-family:monospace;"; break;
	}
	style << Sprintf("font-size:%dpt;", f.GetHeight() * 72 / 600);
	style << (f.IsBold() ? "font-weight:bold;" : "font-weight:normal;");
	style << (f.IsItalic() ? "font-style:italic;" : "font-style:normal;");
	return style;
}

String HtmlDot(int q, Zoom z)
{
	return String().Cat() << ' ' << z * q << "px";
}

String HtmlDotl(int q, Zoom z)
{
	return String().Cat() << ' ' << max((int)!!q, z * q) << "px";
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

String HtmlParaStyle(const RichPara::Format& f, Zoom z)
{
	String style;
	int lm = z * f.lm;
	if(f.bullet && f.bullet != RichPara::BULLET_TEXT) {
		style << "display:list-item;list-style-type:";
		switch(f.bullet) {
		case RichPara::BULLET_ROUND: style << "disc"; break;
		case RichPara::BULLET_ROUNDWHITE: style << "circle"; break;
		case RichPara::BULLET_BOX:
		case RichPara::BULLET_BOXWHITE: style << "square"; break;
		}
		style << ';';
//		style << ";list-style-position:inside;";
		lm += 20;
	}
	style << Format("margin:%d`px %d`px %d`px %d`px;text-indent:%d`px;",
	                z * f.before,
	                z * f.rm,
	                z * f.after,
	                lm,
	                z * (f.bullet ? 0 : f.indent)
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
	style << decode(f.linespacing, RichPara::LSP15, "line-height:150%",
	                               RichPara::LSP20, "line-height:200%",
	                               RichPara::LSP115, "line-height:115%",
	                               "");
	return style;
}

String FormatClass(Index<String>& css, const String& fmt)
{
	return " class=\"" + FormatIntAlpha(css.FindAdd(fmt) + 1) + "\"";
}

void TabBorder(String& style, const char *txt, int border, Color bordercolor, const RichTable::Format& tf, Zoom z)
{
	style << "border-" << txt << ':' << HtmlDotl(border + tf.grid, z) << " solid "
	      << ColorToHtml(border ? bordercolor : tf.gridcolor) << ';';
}

String AsHtml(const RichTxt& text, const RichStyles& styles, Index<String>& css,
              const VectorMap<String, String>& links,
              const VectorMap<String, String>& labels,
              Zoom z, const VectorMap<String, String>& escape,
              HtmlObjectSaver& object_saver,
              RichPara::Number& n)
{
	String html;
	for(int i = 0; i < text.GetPartCount(); i++) {
		if(text.IsTable(i)) {
			const RichTable& t = text.GetTable(i);
			const RichTable::Format& tf = t.GetFormat();
			int nx = tf.column.GetCount();
			int ny = t.GetRows();
			html << "<table width=\"100%\" border=\"0\" cellpadding=\"0\" cellspacing=\"0\">";
			if(tf.before > 0)
				html << "<tr><td height=" << HtmlDot(tf.before, z) << " colspan=\"3\"></td></tr>";
			html << "<tr>";
			if (tf.lm > 0)
				html << "<td><table border=\"0\" width=" << HtmlDot(tf.lm, z) << "><tr><td></td></tr></table></td>\r\n";
			html <<	"<td width=\"100%\">";

			String style;
			style << "border-collapse:collapse;table-layout:auto;"
			      << "border:" << HtmlDotl(tf.frame, z) << " solid " << ColorToHtml(tf.framecolor) << ';';

			html << "<table width=\"100%\"" << FormatClass(css, style) << ">";
			int sum = 0;
			for(int i = 0; i < nx; i++)
				sum += tf.column[i];
			html << "<colgroup>";
			for(int i = 0; i < nx; i++)
				html << "<col width=\"" << 100 * tf.column[i] / sum << "%\">";
			html << "</colgroup>";
			html << "\r\n";
			for(int i = 0; i < ny; i++) {
				const Array<RichCell>& r = t[i];
				html << "<tr>";
				for(int j = 0; j < r.GetCount(); j++) {
					if(t(i, j)) {
						const RichCell& c = r[j];
						const RichCell::Format& cf = c.format;
						String style;
						style << "padding:" << HtmlDot(cf.margin.top, z) << HtmlDot(cf.margin.right, z)
						                    << HtmlDot(cf.margin.bottom, z) << HtmlDot(cf.margin.left, z) << ';';
						TabBorder(style, "left", cf.border.left, cf.bordercolor, tf, z);
						TabBorder(style, "top", cf.border.top, cf.bordercolor, tf, z);
						TabBorder(style, "right", cf.border.right, cf.bordercolor, tf, z);
						TabBorder(style, "bottom", cf.border.bottom, cf.bordercolor, tf, z);
						style << "background-color:" << ColorToHtml(cf.color) << ';';
						style << "vertical-align:";
						switch(cf.align) {
						case ALIGN_TOP:    style << "top"; break;
						case ALIGN_CENTER: style << "middle"; break;
						case ALIGN_BOTTOM: style << "bottom"; break;
						}
						style << ';';
						html << "<td" << FormatClass(css, style);
						if(c.hspan)
							html << " colspan=" << c.hspan + 1;
						if(c.vspan)
							html << " rowspan=" << c.vspan + 1;
						html << '>';
						html << AsHtml(c.text, styles, css, links, labels, z, escape, object_saver, n);
						html << "</td>\r\n";
					}
				}
				html << "</tr>\r\n";
			}
			html << "</table></td>\r\n";
			if (tf.rm > 0)
				html << "<td><table border=\"0\" width=" << HtmlDot(tf.rm, z) << "><tr><td></td></tr></table></td>";
			if(tf.after > 0)
				html << "<tr><td height=" << HtmlDot(tf.after, z) << " colspan=\"3\"></td></tr>";
			html << "</tr></table>\r\n";
		}
		else
		if(text.IsPara(i)) {
			RichPara p = text.Get(i, styles);
			if(p.format.ruler)
				html << "<hr>";
			bool bultext = false;
			String number;
			if(p.format.GetNumberLevel() > 0) {
				n.Next(p.format);
				number = n.AsText(p.format);
			}
			else
			if(p.format.bullet == RichPara::BULLET_TEXT)
				for(int i = 0; i < p.part.GetCount(); i++) {
					const RichPara::Part& part = p.part[i];
					if(part.text.Find(9) >= 0) {
						bultext = true;
						break;
					}
				}
			if(bultext) {
				html << "<table width=\"100%\" border=\"0\" "
				        "cellpadding=\"2\" cellspacing=\"2\">"
				        "<tr>";
				int q = z * p.format.lm - 8;
				if(q > 0)
					html << Format("<td width=\"%d\"></td>", q);
				html << Format("<td valign=\"top\" width=\"%d\" bgcolor=\"#F0F0F0\">\r\n",
				               max(z * p.format.indent, 0));
				p.format.ruler = p.format.after = p.format.before = p.format.indent = p.format.lm = 0;
			}
			String par = "<p";
			String lbl = labels.Get(p.format.label, p.format.label);
			if(lbl.GetCount())
				par << " id=\"" << lbl << "\"";
			par << FormatClass(css, HtmlParaStyle(p.format, z)) << ">";
			html << par;
			for(int i = 0; i < p.part.GetCount(); i++) {
				const RichPara::Part& part = p.part[i];
				int q;
				String lnk = part.format.link;
				if(lnk.GetCount()) {
					int q = links.Find(lnk);
					if(q < 0) {
						int q = lnk.ReverseFind('#');
						if(q >= 0) {
							String l = lnk.Left(q);
							lnk = links.Get(l, l) + '#' + lnk.Mid(q + 1);
						}
					}
					else
						lnk = links[q];
				}
				if(part.object)
					html << object_saver.GetHtml(part.object, lnk);
				else
				if(part.format.indexentry.GetCount() &&
				   (q = escape.Find(part.format.indexentry.ToString())) >= 0)
					html << escape[q];
				else {
					String endtag;
					if(!lnk.IsEmpty() && lnk[0] != ':') {
						html << "<a href=\"" << lnk << "\">";
						endtag = "</a>";
					}
					String cs;
					cs = HtmlCharStyle(part.format, p.format);
					if(!cs.IsEmpty()) {
						html << "<span" << FormatClass(css, cs) << ">";
						endtag = "</span>" + endtag;
					}
					if(part.format.sscript == 1) {
						html << "<sup>";
						endtag = "</sup>" + endtag;
					}
					if(part.format.sscript == 2) {
						html << "<sub>";
						endtag = "</sub>" + endtag;
					}
					if(part.format.IsUnderline()) {
						html << "<u>";
						endtag = "</u>" + endtag;
					}
					if(part.format.IsStrikeout()) {
						html << "<del>";
						endtag = "</del>" + endtag;
					}
					if(part.format.capitals) {
						html << "<span style=\"font-variant: small-caps;\">";
						endtag << "</span>";
					}
					bool spc = false;
					const wchar *end = part.text.End();
					const wchar *s = part.text.Begin();
					WString h;
					if(number.GetCount()) {
						if(*s == '\t')
							s++;
						h << number.ToWString() << ' ' << WString(s, end);
						s = h;
						end = h.end();
					}
					for(;s != end; s++) {
						if(*s == ' ') {
							html.Cat(spc ? "&nbsp;" : " ");
							spc = true;
						}
						else {
							spc = false;
							if(*s == 160)  html.Cat("&nbsp;");
							else
							if(*s == '<')  html.Cat("&lt;");
							else
							if(*s == '>')  html.Cat("&gt;");
							else
							if(*s == '&')  html.Cat("&amp;");
							else
							if(*s == '\"') html.Cat("&quot;");
							else
							if(*s == 9) {
								if(bultext) {
									if(!cs.IsEmpty() && part.text[0] != 9)
										html << "</span>";
									html << "</p>";
									html << "</td>\r\n<td valign=\"top\" bgcolor=\"#F0F0F0\">\r\n";
									html << par;
									if(s[1]) {
										cs = HtmlCharStyle(part.format, p.format);
										if(!cs.IsEmpty())
											html << "<span" << FormatClass(css, cs) << ">";
									}
								}
								else
									html.Cat("&nbsp;&nbsp;&nbsp;&nbsp;");
							}
							else
								html.Cat(ToUtf8(*s));
						}
					}
					html << endtag;
				}
			}
			if(p.part.GetCount() == 0)
				html << "&nbsp;";
			html << "</p>";
			if(bultext)
				html << "</td></tr></table>";
			html << "\r\n";
		}
	}
	return html;
}

class DefaultHtmlObjectSaver : public HtmlObjectSaver
{
public:
	DefaultHtmlObjectSaver(const String& outdir_, const String& namebase_, int imtolerance_, Zoom z_)
	: outdir(outdir_), namebase(namebase_), z(z_), imtolerance(imtolerance_), im(0) {}

	virtual String GetHtml(const RichObject& object, const String& link);

private:
	String outdir;
	String namebase;
	Zoom z;
	int imtolerance;
	int im;
};

String DefaultHtmlObjectSaver::GetHtml(const RichObject& object, const String& link)
{
	StringBuffer html;
	String name;
	name << namebase << "_" << im++ << ".png";
	Size psz = object.GetPixelSize();
	Size sz = z * object.GetSize();
	if(abs(100 * (psz.cx - sz.cx) / sz.cx) < imtolerance)
		sz = psz;
	PNGEncoder png;
	RichObjectPaintInfo pi;
	pi.ink = SBlack();
	png.SaveFile(AppendFileName(outdir, name), object.ToImage(psz, pi));
	String el = "</a>";
	if(IsNull(link)) {
		if(psz.cx * psz.cy != 0)
			html << "<a href=\"" << name << "\">";
		else
			el.Clear();
	}
	else
		html << "<a href=\"" << link << "\">";
	html << Format("<img src=\"%s\" border=\"0\" alt=\"\" style=\"width: %d`px; height: %d`px\">",
	               name, sz.cx, sz.cy);
	html << el;
	return String(html);
}

String EncodeHtml(const RichText& text, Index<String>& css,
                  const VectorMap<String, String>& links,
                  const VectorMap<String, String>& labels,
                  const String& outdir, const String& namebase, Zoom z,
                  const VectorMap<String, String>& escape, int imt)
{
	RichPara::Number n;
	DefaultHtmlObjectSaver default_saver(outdir, namebase, imt, z);
	return AsHtml(text, text.GetStyles(), css, links, labels, z, escape, default_saver, n);
}

String EncodeHtml(const RichText& text, Index<String>& css,
                  const VectorMap<String, String>& links,
                  const VectorMap<String, String>& labels,
                  HtmlObjectSaver& object_saver, Zoom z,
                  const VectorMap<String, String>& escape)
{
	RichPara::Number n;
	return AsHtml(text, text.GetStyles(), css, links, labels, z, escape, object_saver, n);
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

String MakeHtml(const char *title, const String& css, const String& body)
{
	String h =
		"<HTML>\r\n"
	    "<HEAD>\t\n"
	    "<META HTTP-EQUIV=\"Content-Type\" CONTENT=\"text/html; charset=utf-8\">\t\n"
	    "<META NAME=\"Generator\" CONTENT=\"U++ HTML Package\">\t\n"
	    "<TITLE>" + String(title) + "</TITLE>\r\n"
	;
	if(!IsNull(css))
		h << "<STYLE TYPE=\"text/css\"><!--\r\n"
		  << css << "\r\n-->\r\n</STYLE>\r\n";
	h << "</HEAD><BODY>" << body << "</BODY>";
	return h;
}

}
