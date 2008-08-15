#include "www.h"

HtmlTag HtmlLine()
{
	return HtmlRow() / HtmlCell();
}

HtmlTag HtmlPara()
{
	return HtmlTag("p");
}

HtmlTag HtmlCourier(int size)
{
	return HtmlTag("span")
	       .Attr("style", Format("font-size: %dpt; font-family: monospace;", size));
}

HtmlTag HtmlArial(int size)
{
	return HtmlTag("span")
	       .Attr("style", Format("font-size: %dpt; font-family: sans-serif;", size));
}

HtmlTag HtmlRoman(int size)
{
	return HtmlTag("span")
	       .Attr("style", Format("font-size: %dpt; font-family: serif;", size));
}

HtmlTag HtmlCourier()
{
	return HtmlTag("span").Attr("style", "font-family: monospace;");
}

HtmlTag HtmlArial()
{
	return HtmlTag("span").Attr("style", "font-family: sans-serif;");
}

HtmlTag HtmlRoman()
{
	return HtmlTag("span").Attr("style", "font-family: serif;");
}

HtmlTag HtmlFntSize(int size)
{
	return HtmlTag("span").Attr("style", Format("font-size: %dpt", size));
}

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
		style << Format("font-size:%dpt;", f.GetHeight() * 72 / 600);
	if(f.IsBold() != base.IsBold())
		style << (f.IsBold() ? "font-weight:bold;" : "font-weight:normal;");
	if(f.IsItalic() != base.IsItalic())
		style << (f.IsItalic() ? "font-style:italic;" : "font-style:normal;");
	if(f.IsUnderline() != base.IsUnderline())
		style << (f.IsUnderline() ? "text-decoration:underline;" : "text-decoration:none;");
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
	style << Format("font-size:%dpt;", f.GetHeight() * 72 / 600);
	style << (f.IsBold() ? "font-weight:bold;" : "font-weight:normal;");
	style << (f.IsItalic() ? "font-style:italic;" : "font-style:normal;");
	style << (f.IsUnderline() ? "text-decoration:underline;" : "text-decoration:none;");
	return style;
}

Htmls Header()
{
	return HtmlPackedTable().Width(-100) /
			(HtmlRow() / (
			    HtmlCell() / (HtmlLink("index.html") / HtmlSingleTag("IMG").Src("logo.gif").Border(0)) +
			    HtmlCell().Bottom().Right() / HtmlArial(12).Color(LtBlue) / HtmlBold() / HtmlItalic() /
			    	""
			));
}

Htmls BarSection(const char *txt)
{
	Htmls bar;
	bar.Br();
	bar << HtmlPackedTable().BgColor(LtGray).Width(-100) /
			(HtmlRow() / (
				HtmlCell().Width(10) / "&nbsp;" +
				HtmlCell() / HtmlArial(8) / HtmlBold() / txt +
				HtmlCell().Width(10) / "&nbsp;"
			));
	return bar;
}

Htmls BarLink(const char *link, const char *text)
{
	Htmls bar;
	bar << HtmlPackedTable().Width(-100) /
			(HtmlRow() / (
				HtmlCell().Width(12) / "&nbsp;" +
				HtmlCell() / HtmlArial(10) / HtmlLink(link) / text +
				HtmlCell().Width(10) / "&nbsp;"
			));
	return bar;
}

HtmlTag HtmlPadding(int p)
{
	return HtmlTable().Width(-100).Border(0).CellPadding(p).CellSpacing(0) /
		   HtmlLine();
}

Htmls LeftBar()
{
	Htmls bar;
//	bar.Br();

	bar << BarLink("index.html", "Home");

	bar << BarSection("Screenshots");
	bar << BarLink("packsel.html", "Starting TheIDE..");
	bar << BarLink("theide.html", "TheIDE");
	bar << BarLink("laydes.html", "Layout designer");
	bar << BarLink("icondes.html", "Icon designer");
	bar << BarLink("debug.html", "Debug session");
	bar << BarLink("lang.html", "Language support");
	bar << BarLink("docpp.html", "Doc++");
	bar << BarLink("linux.html", "TheIDE / Linux");

	bar << BarSection("Reference examples");
	bar << BarLink("log.html", "Diagnostic log");
	bar << BarLink("value.html", "Value");
	bar << BarLink("callback.html", "Callback");
	bar << BarLink("draw.html", "Draw");
	bar << BarLink("drawing.html", "Drawing");
	bar << BarLink("image.html", "Raster images");
	bar << BarLink("events.html", "Input events");
	bar << BarLink("display.html", "Display");
	bar << BarLink("menu.html", "Menu");
	bar << BarLink("uscctrl.html", "UscCtrl");

	bar << BarSection("Examples");
	bar << BarLink("scribble1.html", "Scribble, step 1");
	bar << BarLink("scribble2.html", "Scribble, step 2");
	bar << BarLink("scribble3.html", "Scribble, step 3");


	bar.Br();
	bar.Br();
	bar << BarLink("http://lists.sourceforge.net/lists/listinfo/upp-main", "Ultimate++ mailing list");
	bar << BarLink("mailto: upp@ntllib.org", "Send us comments");
	bar.Br();
	bar.Br();
	Date d = GetSysDate();
	bar << HtmlItalic() / HtmlArial(8) /
	       ("&nbsp;&nbsp;Last update " + Format("%d/%d/%d", d.year, d.month, d.day));
	bar.Br();
	bar << "<IMG SRC=\"http://pocitadlo.servery.cz/counter.php?id=1463\">";
	return HtmlArial(10) / bar;
}

String GetText(const DocKey& key)
{
	static DocDir dir;
	static bool init;
	static String styles;
	if(!init) {
		init = true;
		SetVar("UPP", "f:/uppdev;f:/uppsrc;");
		DocQuery q;
		q.lang = LNG_ENGLISH;
		q.undocumented = q.normal = q.external = q.obsolete = q.ignored = true;

		::Workspace  wspc;
		wspc.Scan("www");
		for(int i = 0; i < wspc.GetCount(); i++)
			dir.Refresh(wspc[i]);

		dir.Select(q, CppBase());
		styles = LoadFile("F:/common/dppstyles.qtf");
	}

	return styles + dir.GetText(key);
}

String GetText(const String& link)
{
	DocKey k;
	String d;
	if(!ParseLink(link, k, d)) return Null;
	k.lang = LNG_ENGLISH;
	return GetText(k);
}

String QtfAsHtml(const char *qtf, Index<String>& css, const VectorMap<String, String>& links,
                 const String& outdir, const String& fn = Null)
{
	return AsHtml(ParseQTF(qtf), css, links, outdir, fn);
}

void AppMain()
{
	VectorMap<String, String> links;

	static char *theme[] = {
		"index",
		"packsel",
		"package",
		"theide",
		"laydes",
		"icondes",
		"debug",
		"lang",
		"docpp",
		"linux",
		"scribble1",
		"log",
		"value",
		"draw",
		"drawing",
		"image",
		"display",
		"events",
		"menu",
		"uscctrl",
		"callback",
		"scribble2",
		"scribble3"
	};

	for(int i = 0; i < __countof(theme); i++)
		links.Add("dpp://www/upp/" + String(theme[i]), String(theme[i]) + ".html");

	for(int i = 0; i < links.GetCount(); i++) {
		Index<String> css;
		String path = links.GetKey(i);
		if(path.Mid(0, 6) == "dpp://") {
			String text = GetText(links.GetKey(i));
			Htmls html =
				HtmlTable().Width(-100).Border(1).BorderColor(WhiteGray).CellPadding(0).CellSpacing(0) / (
					HtmlLine().ColSpan(2).Background("bluebg.gif") / (HtmlPadding(8) / Header()) +
					HtmlRow() / (
						HtmlCell().Top().Width(160).Background("yellowbg.gif") / LeftBar() +
						HtmlCell().Top().BgColor(White).CellSpacing(8) /
							(HtmlPadding(8) / QtfAsHtml(text, css, links, "E:\\uppwww", FormatIntAlpha(i)))
					)
				);
			Htmls content = HtmlHeader("Ultimate++", AsCss(css)).BgColor(WhiteGray) / html;
			SaveFile(AppendFileName("E:\\uppwww", links[i]), content);
		}
	}
}
