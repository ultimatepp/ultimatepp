#include "www.h"
#define IMAGECLASS WWW
#define IMAGEFILE  <uppweb/www.iml>
#include <Draw/iml.h>
#include <plugin/ftp/ftp.h>

#define TFILE <uppweb/www.t>
#include <Core/t.h>

#define LLOG(x)  // LOG(x)

#ifdef PLATFORM_WIN32 
String rootdir = "u:\\upp.src";
String uppbox =    rootdir + "uppbox";
String uppsrc =    rootdir + "uppsrc";
String reference = rootdir + "reference";
String examples =  rootdir + "examples";
String targetdir = "u:\\uppwww";
String diffdir   = "u:\\wwwupp";
String pdfdir    = "u:\\pdf";
#else
String rootdir;
String uppbox;
String uppsrc;
String reference;
String examples;
String targetdir;
String diffdir;
String pdfdir;
#endif
String bazaar;
bool ftpupload;
bool outPdf;
bool doSvn;

String Replace(const String &str, const String find, const String &replace) {
	int pos = str.Find(find);
	if (pos >= 0) 
		return str.Left(pos) + replace + str.Mid(pos + find.GetCount());
	else
		return str;
}

String GetRcFile(const char *s)
{
	String f = GetDataFile(s);
	if(FileExists(f))
		return f;
	return GetHomeDirFile("upp.src/uppbox/uppweb/" + AsString(s));
}

bool ContainsAt(const String &source, const String &pattern, int pos)
{
	return    pos >= 0
	       && pos + pattern.GetLength() <= source.GetLength()
	       && 0 == memcmp(source.Begin() + pos, pattern.Begin(), pattern.GetLength());
}

bool StartsWith(const String &source, const String &pattern)
{
	return ContainsAt(source, pattern, 0);
}

bool EndsWith(const String &source, const String &pattern)
{
	return ContainsAt(source, pattern, source.GetLength() - pattern.GetLength());
}

String ImgName(int q)
{
	return Sprintf("%di.png", q);
}

typedef Image (*ImageFn)();

namespace Upp {
template<>
unsigned GetHashValue(const ::ImageFn& fn) { return (unsigned)(uintptr_t) fn; }
};

String GetImageSrc(ImageFn img)
{
	static Index<ImageFn> il;
	int q = il.Find(img);
	if(il.Find(img) < 0) {
		q = il.GetCount();
		il.Add(img);
		PNGEncoder png;
		png.SaveFile(AppendFileName(targetdir, ImgName(q)),(*img)());
	}
	return ImgName(q);
}

Htmls Wimg(ImageFn img)
{
	return HtmlImg(GetImageSrc(img)).Border(0);
}

Htmls RoundFrame(Htmls data, String border, Color bg)
{
	return HtmlPackedTable().BgColor(bg).Width(-100)
	          .Attr("style", "border-style: solid; border-width: 1px; border-color: #" + border + ";")
	       / HtmlLine() / data;
}

HtmlTag BoxWidth(int width)
{
	return HtmlPackedTable().Width(width) / HtmlLine();
}

Htmls BarSection(const char *txt)
{
	Htmls bar;
	bar.Br();
	bar << HtmlPackedTable().BgColor(WhiteGray).Width(-100) /
			(HtmlRow() / (
				HtmlCell().Width(10) / "&nbsp;" +
				HtmlCell() / HtmlArial(8) / HtmlBold() / txt +
				HtmlCell().Width(10) / "&nbsp;"
			));
	return bar;
}

Htmls BarCaption(const char *text)
{
	return HtmlLine()
	         .Width(-100)
	         .VCenter()
	         .Style("background-image: url('" + GetImageSrc(WWW::Caption) + "'); "
			        "border: 0px solid black;"
	                "padding-left:12px; padding-right:0px; "
	                "padding-top:1px; padding-bottom:1px;"
	                "color:#FFFFFF;")
	         / HtmlBold() / HtmlArial(8) / text;
}

Htmls BarCaptionLang(const char *text)
{
	return HtmlLine()
	         .Width(20)
	         .VCenter()
	         .Style("background-image: url('" + GetImageSrc(WWW::News2) + "'); "
			        "border: 0px solid black;"
	                "padding-left:12px; padding-right:0px; "
	                "padding-top:1px; padding-bottom:1px;"
	                "color:#FFFFFF;")
	         / HtmlBold() / HtmlArial(8) / text;
}

Htmls BarItem(Htmls content, const char *style)
{
	String bgStyle = "background-image: url('" + GetImageSrc(WWW::Button) + "'); ";
	return HtmlLine().Width(-100).VCenter().Style(bgStyle + style)
	         / content;
}

Htmls BarLink(const char *link, const char *text, bool nxt = true)
{
	String style = "border: 0px solid black; "
	               "padding-left:12px; padding-right:0px; "
	               "padding-top:6px; padding-bottom:6px;";
	if(nxt)
	  style += " border-top: 1px solid #6E89AE;";

	return BarItem( HtmlLink(link).Class("l1") / text, style );
}

Htmls SearchBar(const char *domain)
{
	Htmls form =
	  HtmlForm("http://www.google.com/search", false, "GET")
	    .Style("margin:0px;padding:0px;") /
	    ( HtmlHidden("ie", "UTF-8") +
	      HtmlHidden("oe", "UTF-8") +
	      HtmlEdit("q", 15) +
	      HtmlHidden("domains", domain) +
	      HtmlHidden("sitesearch", domain)
	    );

	Htmls content;
	content << HtmlPackedTable().Width(-100)
	           / HtmlRow() / (
	               HtmlCell() / Wimg(WWW::google) +
	               HtmlCell() / form
	            );

	String style = "border: 0px solid black; "
	               "padding-left:6px; padding-right:0px; "
	               "padding-top:4px; padding-bottom:4px;";
		               "border-top: 1px solid #6E89AE;";
	return BarItem(content, style);
}

HtmlTag HtmlPadding(int p)
{
	return HtmlPackedTable().Width(-100) /
		   HtmlLine();
}

VectorMap<String, String> escape;

String QtfAsHtml(const char *qtf, Index<String>& css,
                 const VectorMap<String, String>& links,
                 const VectorMap<String, String>& labels,
                 const String& outdir, const String& fn = Null)
{
	return EncodeHtml(ParseQTF(qtf), css, links, labels, outdir, fn, Zoom(8, 40), escape, 40);
}

#define TOPICFILE <uppweb/www.tpp/all.i>
#include <Core/topic_group.h>

#define TOPICFILE <ide/app.tpp/all.i>
#include <Core/topic_group.h>

String GetText(const char *s)
{
	return GetTopic(s).text;
}

String ChangeTopicLanguage(const String &topic, int lang) {
	int pos = topic.ReverseFind('$');
	if (pos < 0)
		return "";			
	String langtxt = ToLower(LNGAsText(lang));		
	return topic.Left(pos+1) + langtxt + topic.Mid(pos+1+langtxt.GetCount()); 
}

String GetTopicLanguage(const String &topic) {
	int pos = topic.ReverseFind('$');
	if (pos < 0)
		return "";			
	return topic.Mid(pos+1, 5); 
}

String FormatDateRFC822(const Time& t) {
	int tz = int((GetSysTime()-GetUtcTime())/60);
	return Format("%s, %d %Mon %d %02d:%02d:%02d %s%04d",
	              DayName(DayOfWeek(t)).Left(3),
	              t.day,t.month,t.year,
	              t.hour,t.minute,t.second,
	              tz>0?"+":"",tz/60*100+(tz+1440)%60);
}

void CreateRssFeed() {
	String header="<rss version=\"2.0\" xmlns:atom=\"http://www.w3.org/2005/Atom\">\n" 
	"<channel>\n"
	"<title>Ultimate++ svn changes</title>\n" 
	"<link>http://ultimatepp.org/</link>\n"
	"<description>This feed offers list of commits to Ultimate++ framework svn repository</description>\n"
	"<lastBuildDate>"+ FormatDateRFC822(GetSysTime()) + "</lastBuildDate>\n"
	"<language>en-us</language>\n"
	"<atom:link href=\"http://ultimatepp.org/svnchanges.xml\" rel=\"self\" type=\"application/rss+xml\" />\n\n";
	
	String items;
	for(int i = 0; i < min(30,svnlog.GetCount()); i++){
		items+="<item>\n"
		"<title>Revision " + svnlog[i].revision + "</title>\n" 
		"<link>http://code.google.com/p/upp-mirror/source/detail?r=" + svnlog[i].revision + "</link>\n"
		"<guid>http://code.google.com/p/upp-mirror/source/detail?r=" + svnlog[i].revision + "</guid>\n"
		"<pubDate>" + FormatDateRFC822(svnlog[i].time) + "</pubDate>\n"
		"<description><![CDATA[\n"
		"	<table style=\"font-size:small;\">\n" 
		"		<tr><td>Revision:</td><td><b>" + svnlog[i].revision + "</b></td></tr>\n"
		"		<tr><td>Description:</td><td><code>" + svnlog[i].msg + "</code></td></tr>\n"
		"		<tr><td>Submitted:</td><td><i>" + FormatDateRFC822(svnlog[i].time) + "</i> by <i>" + svnlog[i].author + "</i></td></tr>\n"
		"		<tr><td>Affected files:</td><td>&nbsp;</td></tr>\n"
		"		<tr><td colspan=\"2\">\n"
		"			<div style=\"margin-left:20px;\">\n";
		for(int j = 0; j < svnlog[i].changes.GetCount(); j++)
			items+="<a href=\"http://code.google.com/p/upp-mirror/source/diff?spec=svn" + svnlog[i].revision + "&amp;r=" + svnlog[i].revision + "&amp;format=side&amp;path=" + svnlog[i].changes[j].path + "\" target=\"gcode\">" + svnlog[i].changes[j].action + "</a>&nbsp;"
			       "<a href=\"http://code.google.com/p/upp-mirror/source/browse" + svnlog[i].changes[j].path + "\" target=\"gcode\">" + svnlog[i].changes[j].path + "</a><br>\n";
		items+=
		"			</div>\n"
		"		</td></tr>\n"
		"	</table>\n"
		"]]></description>\n" 
		"</item>\n\n";
	}
	SaveFile(AppendFileName(targetdir, "svnchanges.xml"), header + items + "</channel>\n</rss>\n");
}

VectorMap<String, Topic> tt;

String Www(const char *topic, int lang, String topicLocation = "topic://uppweb/www/")
{
	String strLang = ToLower(LNGAsText(lang));
	String www = GatherTopics(tt, String().Cat() << topicLocation << topic << "$" << strLang);
	if (www != "index.html")
		return www;
	return GatherTopics(tt, String().Cat() << topicLocation << topic << "$" << "en-us");
}

String FolderLinks(String package, String group, int lang)
{
	String qtf;
	FindFile ff(AppendFileName(AppendFileName(AppendFileName(uppsrc, package), group + ".tpp"), "*.tpp"));
	while(ff) {
		if(ff.IsFile()) {
			if (ff.GetName().Find("en-us") >= 0) {
				String title;
				String tl = "topic://" + package + '/' + group + '/' + GetFileTitle(ff.GetName());
				tl =  ChangeTopicLanguage(tl, lang);
				GatherTopics(tt, tl, title);
				qtf << "________[^" << tl << "^ " << DeQtf(Nvl(title, tl)) << "]&";
			}
		}
		ff.Next();
	}
	return qtf;
}

void AddFiles(String& qtf, const String& dir, const char* ext, bool& b)
{
	FindFile ff(AppendFileName(dir, "*." + String(ext)));
	while(ff) {
		qtf << "[A4* " << DeQtf(ff.GetName()) << "]&&"
		    << CppAsQtf(LoadFile(AppendFileName(dir, ff.GetName())))
		    << "&&&";
		ff.Next();
		b = true;
	}
}

struct Isort {
	bool operator()(const String& a, const String& b) const
	{
		return ToUpper(a) < ToUpper(b);
	}
};

String MakeExamples(const char *dir, const char *www, int language)
{
	String ttxt;
	FindFile ff(AppendFileName(dir, "*.*"));
	ttxt << "{{1:3 ";
	bool next = false;
	Vector<String> ls;
	while(ff) {
		if(ff.IsFolder())
			ls.Add(ff.GetName());
		ff.Next();
	}
	Sort(ls, Isort());
	for(int i = 0; i < ls.GetCount(); i++) {
		String name = ls[i];
		String link = String().Cat() << www << '$' << name << "$" << ToLower(LNGAsText(language)) << ".html";
		Topic& topic = tt.Add(link);
		topic.title = name;
		String fn = AppendFileName(
						AppendFileName(
							AppendFileName(uppbox, "uppweb"),
							String(www) + ".tpp"
						),
						topic.title + "$" + ToLower(LNGAsText(language))  + ".tpp"
					);
		String h = ReadTopic(LoadFile(fn)).text;
		Package p;
		p.Load(AppendFileName(AppendFileName(dir, name), name + ".upp"));
		topic.text << "[R6* " << name << "]&&" << DeQtf(p.description) << "&";
		if(h.GetCount())
			topic.text << h;
		topic.text << "[A2<l0r0 &&";
		String d = AppendFileName(dir, name);
		bool b = false;
		AddFiles(topic.text, d, "h", b);
		AddFiles(topic.text, d, "hpp", b);
		AddFiles(topic.text, d, "cpp", b);
		AddFiles(topic.text, d, "usc", b);
		AddFiles(topic.text, d, "lay", b);
		AddFiles(topic.text, d, "key", b);
		AddFiles(topic.text, d, "brc", b);
		AddFiles(topic.text, d, "sch", b);
		AddFiles(topic.text, d, "xml", b);
		if(b) {
			if(next)
				ttxt << "\n::^ ";
			ttxt << "[^" << link << "^ " << DeQtf(topic.title) << "]::^ "
			     << DeQtf(p.description);
			next = true;
		}
	}
	ttxt << "}}&&";
	return ttxt;
}

void SrcDocs(Index<String> &x, String& qtf, const char *folder, int lang)
{
	if(x.Find(folder) >= 0)
		return;
	x.Add(folder);
	String srcdoc = FolderLinks(folder, "srcdoc", lang);
	String src = FolderLinks(folder, "src", lang);
	Package p;
	p.Load(AppendFileName(uppsrc, AppendFileName(folder, GetFileName(folder) + ".upp")));
	if(srcdoc.GetLength() || src.GetLength()) {
		qtf << "&&&[*4@b " << folder << "]&";
		if(!IsNull(p.description))
			qtf << "[2 " << p.description << "]&";
		if(srcdoc.GetCount()) {
			qtf << "&[3/* " + Format(t_("Using %s"), folder) << "]&";
			qtf << srcdoc;
		}
		if(src.GetCount()) {
			qtf << "&[3/* " << Format(t_("%s reference"), folder) << "]&";
			qtf << src;
		}
	}
}

int CharFilterLbl(int c)
{
	return IsAlNum(c) ? c : '.';
}


void QtfAsPdf(PdfDraw &pdf, const char *qtf)
{
	RichText txt = ParseQTF(qtf);
	Size page = Size(3968, 6074); 
	UPP::Print(pdf, txt, page);
}

VectorMap<String, String> links;
VectorMap<String, String> labels;
Htmls header, lastUpdate;

Array <Htmls> bar;
Array <int> languages;

int GetLinkLanguage(const String &link) {
	int pos = link.ReverseFind('$');
	if (pos < 0)
		return 0;
	int lang = LNGFromText(ToUpper(link.Mid(pos+1)));
	for (int i = 0; i < languages.GetCount(); ++i) {
		if (languages[i] == lang)
			return i;
	}
	return 0;
}

void ExportPage(int i)
{
	Index<String> css;
	String path = links.GetKey(i);
	RLOG("Exporting " << path);
	
	int ilang = GetLinkLanguage(path); 
	SetLanguage(languages[ilang]);
	String text = GetText(path);
	int h;
	h = ParseQTF(tt[i].text).GetHeight(1000);
	
	int isvn = svndata.Find(tt.GetKey(i));
	String qtflangs;
	String googleFile;	
	if (isvn > -1) {   				
		String txt = String("[2 ") + t_("Last edit by %s on %s") + ".]";	
		qtflangs += Format(txt, svndata[isvn].author, Format(Date(svndata[isvn].time)));
		googleFile = svndata[isvn].fullPath;
		if (googleFile.GetCount() > rootdir.GetCount())
			googleFile = UnixPath(googleFile.Mid(rootdir.GetCount()));
		else
			googleFile = "";
	}
	String strlang;
	Array <String> arrLangs;
	for (int il = 0; il < languages.GetCount(); ++il) {
		if (il != ilang) {
			String topic = ChangeTopicLanguage(path, languages[il]);
			int itopic;
			if ((itopic = tt.Find(topic)) >= 0) {
				if (tt[itopic].title.Find(" (translated)") < 0) {
					if (!strlang.IsEmpty())
						strlang << ", ";
					arrLangs.Add("[^" + links[itopic] + "^ [2 " + ToLower(GetNativeLangName(languages[il])) + "]]");
				}
			}
		}
	}
	if (arrLangs.GetCount() > 0) {
		for (int i = 0; i < arrLangs.GetCount(); ++i) {
			if (i == arrLangs.GetCount()-1 && i != 0)
				strlang << String(" ") + t_("and") + " ";
			else if (i > 0)
				strlang << ", ";
			strlang << arrLangs[i];
		}
	}	
	if (!strlang.IsEmpty())
		qtflangs += Format(String("[2  ") + t_("This page is also in %s") + ".]", strlang);
	if (tt[i].title.Find("How to contribute. Web page") < 0) {
		String help = "topic://uppweb/www/contribweb$" + ToLower(LNGAsText(languages[ilang]));
		qtflangs += " " + String("[^") + help + "^ [<A2 " + t_("Do you want to contribute?") + "]]";
		if (googleFile != "")
			qtflangs += ". [^http`:`/`/upp`-mirror.googlecode.com`/svn`/trunk" + DeQtf(googleFile) + "^/1 " + "T`+`+" + "]";
	}	

	String langs = QtfAsHtml(qtflangs, css, links, labels, targetdir, links[i]);
	String page = tt[i];
		
	Array<String> htmlrep;
	int posB = 0;
	while (true) {
		posB = page.Find("[IHTMLTEXT", posB);
		if (posB < 0)
			break; 
		int pos0 = page.ReverseFind('[', posB);
		int posBB = posB + strlen("[IHTMLTEXT");
		int posE = page.Find("<`/object`>", posBB);
		int posEE = page.Find("]", posE);
		String html0 = page.Mid(posBB, posE + strlen("<`/object`>") - posBB);
		String html1;
		while (true) { 
			html1 = Replace(html0, "`", "")	;
			if (html1 == html0)
				break;
			html0 = html1; 
		}
		htmlrep.Add(html1);
		page = page.Left(pos0) + "QTFHTMLTEXT" + page.Mid(posEE+1);
	}
	
	page = QtfAsHtml(page, css, links, labels, targetdir, links[i]);
	for (int iHtml = 0; iHtml < htmlrep.GetCount(); ++iHtml) 
		page = Replace(page, "QTFHTMLTEXT", htmlrep[iHtml]);
	
	Color paper = SWhite;
	if(path == "topic://uppweb/www/download$en-us")
		page << LoadFile(GetRcFile("adsense3.txt"));
/*		if(path == "topic://uppweb/www/index$en-us") {
		for(int q = 0; q < news.GetCount(); q++) {
			String n = GetText("uppweb/www_news/" + news[q]);
			String h = news[q];
			int i = h.Find('$');
			if(i >= 0)
				h = h.Mid(0, i);
			if(h.GetLength() == 8)
				h = h.Mid(0, 4) + '-' + h.Mid(4, 2) + '-' + h.Mid(6, 2);
			page << "<br>";
			page << "<div style='font-family:sans-serif; font-weight: bold; "
			        "font-height: 12px; color: White; background: #2020d0'>&nbsp;&nbsp;"
			     << h << "</div><br>";
			page << QtfAsHtml(n, css, links, targetdir, FormatIntAlpha(q) + "_n");
			page << "<br>";
		}
	}*/
	Color bg = Color(210, 217, 210);
	Htmls footer;
	footer << HtmlTable().Border(0).Width(-100) / HtmlLine() +
				RoundFrame(HtmlPadding(8) / langs , "6E89AE;padding: 10px;", White);
	Htmls html;
	html <<
		HtmlPackedTable().Width(-100) /
		   	HtmlLine().ColSpan(3) / header +
		HtmlPackedTable().Width(-100) / (
			HtmlLine().ColSpan(3).BgColor(bg).Height(6) / "" +
			HtmlRow() / (
				HtmlTCell().Center() / BoxWidth(160).Center() / (
					bar[GetLinkLanguage(path)] +
					"<br>" +
//						"<p align=\"center\">" +
					LoadFile(GetRcFile("adsense2.txt")) +
					"<br><br>" +
					LoadFile(GetRcFile("adlinks.txt")) +
					(h > 25000 ? "<br><br>" + LoadFile(GetRcFile("adsense2.txt"))
					                                : "") +
			       	"<br><br><br>" +
//						LoadFile(GetRcFile("referral.txt")) +
//						LoadFile(GetRcFile("referral2.txt")) +
//						LoadFile(GetRcFile("donations.txt")) +
//						"<br><br>" +
//						amazon[i % amazon.GetCount()] +
			       	"<br><br><br>" +
					HtmlLink("http://sourceforge.net/projects/upp/") /
					  HtmlImg("http://sourceforge.net/sflogo.php?group_id=93970&type=2",
					          "SourceForge.net Logo").Border(0).Width(125).Height(37) +
			       	"<br><br>" +
			       	HtmlLink("http://www.sdjournal.org/en/") /
						HtmlImg(GetImageSrc(WWW::Sdj)).Border(0) +
			       	"<br><br>" +
			       	(links[i] == "index.html" ? lastUpdate : Htmls()) +
			       	HtmlImg("http://www.vol.cz/cgi-bin/wc/upp").Width(1).Height(1)
				) +
				HtmlTCell().BgColor(bg) / BoxWidth(6) / "" +
				HtmlTCell().Width(-100).BgColor(bg) / (
					RoundFrame(HtmlPadding(8) / page , "6E89AE;padding: 10px;", White) +
					footer
				)
			)
		);

	String topicTitle = tt.GetKey(i);
	String pageTitle = tt[i].title;
	if(IsNull(pageTitle))
		pageTitle = "Ultimate++";
	if(StartsWith(topicTitle, "examples$"))
		pageTitle = "Demos / " + pageTitle;
	else if(StartsWith(topicTitle, "reference$"))
		pageTitle = "Examples / " + pageTitle;

	if(pageTitle != "Ultimate++")
		pageTitle << " :: Ultimate++";

	Htmls content =
	    "<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">\n" +
		HtmlHeader(pageTitle, AsCss(css) +
			"a.l1         { text-decoration:none; font-size: 8pt; font-family: sans-serif; "
			              "font-weight: normal; }\n"
			"a.l1:link    { color:#000000; }\n"
			"a.l1:visited { color:#000080; }\n"
			"a.l1:hover   { color:#9933CC; }\n"
			"a.l1:active  { color:#000000; }\n"
			"a.l2         { text-decoration:none; font-size: 12pt; font-family: sans-serif; "
			              "font-variant: small-caps; }\n"
			"a.l2:link    { color:#0066FF; }\n"
			"a.l2:visited { color:#FF6600; }\n"
			"a.l2:hover   { color:#BC0624; }\n"
			"a.l2:active  { color:#BC0024; }\n",
			"<META NAME=\"keywords\" "
			"CONTENT=\""
			"framework, toolkit, widget, c++, visual, studio, dev-cpp, builder, ide, class, component,"
			"wxwidgets, qt, rapid, application, development, rad, mfc, linux, gui, sdl, directx, desktop"
			"\">"
	        "<META name=\"robots\" content=\"index,follow\">\n"
            "<LINK rel=\"alternate\" type=\"application/rss+xml\" title=\"SVN changes\" href=\"svnchanges.xml\">"
//				"<link rel=\"shortcut icon\" href=\"/favicon.ico\" />"
		)


	    .BgColor(bg)
	    .Alink(Red).Link(Black).Vlink(Blue)
	    / html;
	SaveFile(AppendFileName(targetdir, links[i]), content);
}


struct ProgramData {
	String rootdir;
	String targetdir;
	String diffdir;
	String pdfdir;
	bool ftpUpload;
	bool outPdf;
	bool doSvn;
	void Xmlize(XmlIO xml)	{
		xml
			("rootdir", rootdir)
			("targetdir", targetdir)
			("diffdir", diffdir)
			("pdfdir", pdfdir)
			("ftpUpload", ftpUpload)
			("outPdf", outPdf)			
			("doSvn", doSvn)
		;
	}
};

GUI_APP_MAIN
{
#ifdef PLATFORM_POSIX
	StdLogSetup(LOG_COUT);
	rootdir = GetHomeDirFile("upp.src");
	targetdir = GetHomeDirFile("uppwww");
	diffdir   = GetHomeDirFile("wwwupp");
	pdfdir   = GetHomeDirFile("pdf");
#endif
	ftpupload = true;
	outPdf = true;
	doSvn = true;
	
	ProgramData data;
	
	String configFile = GetHomeDirFile("uppweb.xml");
	bool cfgloaded = false;
	if (FileExists(configFile)) {
		if (LoadFromXMLFile(data, configFile)) {
			rootdir   = data.rootdir;
			targetdir = data.targetdir;
			diffdir   = data.diffdir;
			pdfdir    = data.pdfdir;	
			ftpupload = data.ftpUpload;
			outPdf    = data.outPdf;
			doSvn     = data.doSvn;
			cfgloaded = true;
		}
	}
	if (!cfgloaded) {
		data.rootdir   = rootdir;
		data.targetdir = targetdir;
		data.diffdir   = diffdir;
		data.pdfdir    = pdfdir;
		data.ftpUpload = ftpupload;
		data.outPdf    = outPdf;
		data.doSvn	   = doSvn;
		StoreAsXMLFile(data, NULL, configFile);
	}
	if (!DirectoryExists(rootdir)) {
		Exclamation ("Directory " + DeQtf(rootdir) + " does not exist");
		return;
	}

	uppbox =    AppendFileName(rootdir, "uppbox");
	uppsrc =    AppendFileName(rootdir, "uppsrc");
	reference = AppendFileName(rootdir, "reference");
	examples =  AppendFileName(rootdir, "examples");
	bazaar =    AppendFileName(rootdir, "bazaar");

	languages.Add(LNG_('E','N','U','S'));		// en-us has to be the first one
	languages.Add(LNG_('C','A','E','S'));
	languages.Add(LNG_('C','S','C','Z'));
	languages.Add(LNG_('E','S','E','S'));
	languages.Add(LNG_('F','R','F','R'));
	languages.Add(LNG_('R','O','R','O'));
	languages.Add(LNG_('R','U','R','U'));
	languages.Add(LNG_('Z','H','C','N'));
	languages.Add(LNG_('Z','H','T','W'));
	
	RLOG("--- uppweb started at " << GetSysTime());

	DeleteFolderDeep(targetdir);
	DirectoryCreate(targetdir);
	
	if (outPdf) {
		DeleteFolderDeep(pdfdir);
		DirectoryCreate(pdfdir);
	}
	GatherRefLinks(uppsrc);
	GatherRefLinks(AppendFileName(rootdir, "bazaar"));
	
	SaveFile(AppendFileName(targetdir, "sdj.gif"), LoadFile(GetRcFile("sdj.gif")));
	
	String release = "3211"; 
	escape.Add("RELEASE", release);
	escape.Add("RELEASET", release);
	escape.Add("UPDATETIME", Format("%`", GetUtcTime()));
	
	if (doSvn) {
		GetSvnList(svndata, rootdir);
		GetSvnLog(svnlog);
		CreateRssFeed();
		if (svnlog.GetCount() > 0) {
			escape.Add("LATESTSVN", svnlog[0].revision);
			//Index<String> css;
			//escape.Add("SVNTABLE", QtfAsHtml(SvnChanges(svnlog, "", 100), css, links, labels, targetdir));
			//escape.Add("ANCHOR", "<span id=\"svnTableAnchor\"></span>");
		}
	}

	escape.Add("PAYPAL", LoadFile(GetRcFile("donations.txt")));

	header = HtmlPackedTable()
	       .Width(-100)
	       .BgColor(White)
	       .Attr("style", "border: 1px solid #6E89AE;"
	                      "padding-left: 10px;padding-right: 0px;padding-top: 0px;padding-bottom: 0px;")
	       /
			HtmlRow() / (
			    HtmlCell() / HtmlLink("index.html") / Wimg(WWW::Logo6) +
			    HtmlCell().Right().Bottom()
			              .Style("padding-bottom: 5px; "
			                     "background-image: url('" + GetImageSrc(WWW::HB) + "')")
			    / HtmlArial(14) / (LoadFile(GetRcFile("adsense.txt")) + "&nbsp;&nbsp;"/* + "<br>..harnessing the real power of C++&nbsp;&nbsp;"*/)
			);
	
	bar.SetCount(languages.GetCount());
	
	int lang = GetCurrentLanguage();
	for (int i = 0; i < languages.GetCount(); ++i) {
		Htmls bi, bex, bdoc, bcom, bcon, bsearch, blang;

		SetLanguage(languages[i]);
	
		Www("index", languages[i]);
		Www("contribweb", languages[i]);
	//	bi << BarLink("index.html", "Home", false);
		bi << BarLink(Www("overview", languages[i]), t_("Overview"), false);
		bi << BarLink(Www("examples", languages[i]), t_("Examples"));	
		{
			int di = tt.Find("topic://uppweb/www/examples$" + ToLower(LNGAsText(languages[i])));
			tt[di].text << MakeExamples(examples, "examples", languages[i]);
			tt[di].text << GetTopic("topic://uppweb/www/reference$" + ToLower(LNGAsText(languages[i]))).text;
			tt[di].text << MakeExamples(reference, "reference", languages[i]);
		}

		bi << BarLink(Www("ss", languages[i]), t_("Screenshots"));
		bi << BarLink(Www("comparison", languages[i]), t_("Comparisons"));
		bi << BarLink(Www("apps", languages[i]), t_("Applications"));
		bi << BarLink(Www("download", languages[i]), t_("Download"));

		bi << BarLink(Www("documentation", languages[i]), t_("Manual"));			
		{
			int di = tt.Find("topic://uppweb/www/documentation$" + ToLower(LNGAsText(languages[i])));
			if (di >= 0) {
				Index<String> x;
				x.Clear();
				String qtf;
				SrcDocs(x, qtf, "Core", languages[i]);
				SrcDocs(x, qtf, "Draw", languages[i]);
				SrcDocs(x, qtf, "CtrlCore", languages[i]);
				SrcDocs(x, qtf, "CtrlLib", languages[i]);
				SrcDocs(x, qtf, "RichText", languages[i]);
				SrcDocs(x, qtf, "RichEdit", languages[i]);
				FindFile ff(AppendFileName(uppsrc, "*.*"));
				Array <String> folders;
				folders.Clear();
				while(ff) {
					if(ff.IsFolder())
						folders.Add(ff.GetName());
					ff.Next();
				}
				Sort(folders);
				for (int ifold = 0; ifold < folders.GetCount(); ++ifold)
					SrcDocs(x, qtf, folders[ifold], languages[i]);
				tt[di].text << qtf;
			}
		}
		bi << BarLink(Www("bazaar", languages[i]), t_("Bazaar"));
		bi << BarLink(Www("Roadmap", languages[i]), t_("Status & Roadmap"));
		bi << BarLink(Www("FAQ", languages[i]), t_("FAQ"));
		bi << BarLink(Www("About", languages[i], "topic://ide/app/"), t_("Authors & License"));
	
		bi << BarLink("http://www.ultimatepp.org/forum", t_("Forums"));
	//	bcom << BarLink(Www("mailing"), "Mailing lists");
	//	bi << BarLink("http://www.ultimatepp.org/wiki/index.php", "Wiki");
		bi << BarLink(Www("Funding", languages[i]), t_("Funding Ultimate++"));
	//	bcom << BarLink(Www("helpus"), "Getting involved");
	//	bcom << BarLink("mailto: upp@ntllib.org", "Contact developers");
	
		bsearch << BarCaption(t_("Search on this site"));
		bsearch << SearchBar("www.ultimatepp.org");
	
		//blang << BarCaptionLang(t_("Languages"));
		//blang << BarLink(Www("English", languages[i]), t_("Bazaar"));
		//blang << BarLink(Www("Russian", languages[i]), t_("Bazaar"));
		
		HtmlTag bf = HtmlPackedTable()
		       .Width(-100)
		       .BgColor(White)
		       .Attr("style", "border-style: solid; border-width: 1px; border-color: #6E89AE;"
		                      "padding: 0px");
		String div = HtmlTable().Border(0).Width(-100) / HtmlLine();
		bar[i] = bf / bi + div +
	//	      bf / bex + div +
	//	      bf / bdoc + div +
	//	      bf / bcom + div +
	//	      bf / bcon + div +
		      bf / bsearch + div;
		      //bf / blang;
	}
	SetLanguage(lang);
	
	for(int i = 0; i < tt.GetCount(); i++) {
		String topic = tt.GetKey(i);
		links.Add(topic, topic == "topic://uppweb/www/index$en-us" ? "index.html" :
		                 memcmp(topic, "topic://", 8) ? topic : TopicFileNameHtml(topic));
	}
	
	for(int i = 0; i < tt.GetCount(); i++) {
		if (tt[i].title == "Svn releases") 
			tt[i].text = Replace(tt[i].text, "`[svntable`]", SvnChanges(svnlog, 300, ""));
		else if (tt[i].title == "Svn Web releases") 
			tt[i].text = Replace(tt[i].text, "`[svntable`]", SvnChanges(svnlog, 100, "uppweb"));
		else if (tt[i].title == "Svn Bazaar releases") 
			tt[i].text = Replace(tt[i].text, "`[svntable`]", SvnChanges(svnlog, 100, "bazaar"));
		else if (tt[i].title == "Svn Upp releases") 
			tt[i].text = Replace(tt[i].text, "`[svntable`]", SvnChanges(svnlog, 100, "uppsrc"));		
		else if (tt[i].title == "Svn major releases") 
			tt[i].text = Replace(tt[i].text, "`[svntable`]", SvnChanges(svnlog, 300, "", true));
		else if (tt[i].title == "Svn Web major releases") 
			tt[i].text = Replace(tt[i].text, "`[svntable`]", SvnChanges(svnlog, 100, "uppweb", true));
		else if (tt[i].title == "Svn Bazaar major releases") 
			tt[i].text = Replace(tt[i].text, "`[svntable`]", SvnChanges(svnlog, 100, "bazaar", true));
		else if (tt[i].title == "Svn Upp major releases") 
			tt[i].text = Replace(tt[i].text, "`[svntable`]", SvnChanges(svnlog, 100, "uppsrc", true));		
		
	}

	DUMPC(reflink.GetKeys());

	for(int i = 0; i < reflink.GetCount(); i++) {
		String l = reflink.GetKey(i);
		String lbl = Filter(l, CharFilterLbl);
		String f = links.Get(reflink[i], Null) + '#' + lbl;
		links.Add(l, f);
		static const char *x[] = { "::struct", "::class", "::union" };
		for(int ii = 0; ii < 3; ii++) {
			String e = x[ii];
			if(EndsWith(l, e)) {
				links.Add(l.Mid(0, l.GetLength() - e.GetLength()), f);
			}
		}
		labels.Add(l, lbl);
	}

	Date d = GetSysDate();
	lastUpdate = HtmlItalic() / HtmlArial(8) / HtmlFontColor(Gray()) /
	                   (String().Cat() << "Last update " << GetSysDate());

//	Vector<String> amazon = Split(LoadFile(GetRcFile("amazon.txt")), '\n');//440

	Vector <SvnBazaarItems> bazaarItems = SvnBazaarList(bazaar, svnlog);

	// To fill Bazaar page with real release dates and size
	for(int i = 0; i < tt.GetCount(); i++) {
		if (tt[i].title == "Bazaar") {
			String page = tt[i];
			for (int j = 0; j < bazaarItems.GetCount(); ++j) {
				int pos = page.Find("2 " + DeQtf(bazaarItems[j].name) + "]]");
				if (pos < 0)
					pos = page.Find("2 " + DeQtf(bazaarItems[j].name) + "\r\n]]");
				if (pos >= 0) {
					pos = page.Find("::", pos);	
					pos = page.Find("::", pos);
					pos = page.Find(":: [s0;=2", pos);
					if (pos >= 0)
						pos += strlen(":: [s0;=2");
					String strDate;
					if (pos >= 0) {
						Time t = bazaarItems[j].lastChange;
						if (IsNull(t))
							strDate = "Not in Svn log";
						else 
							strDate = Format("%Mon", t.month) + " " + FormatInt(t.year);
						page.Insert(pos, " " + strDate);
					}
					pos = page.Find(":: [s0;=2", pos);
					if (pos >= 0)
						pos += strlen(":: [s0;=2");
					String strSz;
					if (pos >= 0) {
						if (bazaarItems[j].size == 0)
							strSz = "Not in Svn log";
						else 
							strSz = BytesToString(bazaarItems[j].size);
		
						page.Insert(pos, " " + strSz);
					}
				}
			}
			tt[i].text = page;
		}
	}
	
	if (outPdf) {
		PdfDraw pdf;
		for(int i = 0; i < tt.GetCount(); i++)
			QtfAsPdf(pdf, tt[i]);
		SaveFile(AppendFileName(pdfdir, "Upp.pdf"), pdf.Finish());
	}
	
	for(int i = 0; i < tt.GetCount(); i++)
		ExportPage(i);
	SetLanguage(lang);

//	SaveFile(AppendFileName(targetdir, "favicon.ico"), LoadFile(AppendFileName(uppsrc, "ide/ide.ico")));
	SaveFile(AppendFileName(targetdir, "stats.html"),
	         HtmlImg("http://www.vol.cz/cgi-bin/wc/upp") + "<br>" +
	         HtmlLink("http://www.mygooglepagerank.com", "_blank") /
		         "<img src=\"http://www.mygooglepagerank.com/PRimage.php?url=http://upp.sf.net\" "
		         "border=\"0\" width=\"66\" height=\"13\" "
		         "alt=\"Google PageRank&trade; - Post your PR with MyGooglePageRank.com\">" +
	         "<noscript>" +
	         HtmlLink("http://www.mygooglepagerank.com").Title("My Google Page Rank") /
	           "My Google Page Rank" +
	         "</noscript>" +
	         HtmlLink("http://www.mygooglepagerank.com", "_blank") /
		         "<img src=\"http://www.mygooglepagerank.com/PRimage.php?url=http://www.ultimatepp.org\" "
		         "border=\"0\" width=\"66\" height=\"13\" "
		         "alt=\"Google PageRank&trade; - Post your PR with MyGooglePageRank.com\">" +
	         "<noscript>" +
	         HtmlLink("http://www.mygooglepagerank.com").Title("My Google Page Rank") /
	           "My Google Page Rank" +
	         "</noscript>"
	);
	BeepInformation();
	
	if (!ftpupload)
		return;
	
	RLOG("uppweb Finished, now about to upload the content");
	
	Vector<String> upload;
	{
		FindFile ff(AppendFileName(targetdir, "*.*"));
		while(ff) {
			if(ff.IsFile()) {
				String s = LoadFile(AppendFileName(targetdir, ff.GetName()));
				String f = AppendFileName(diffdir, ff.GetName());
				if(LoadFile(f) != s) {
					upload.Add(ff.GetName());
					RLOG("upload: " << ff.GetName());
				}
			}
			ff.Next();
		}
	}
	DirectoryCreate(diffdir);
	
	if(upload.GetCount()) {
		FtpClient ftp;
		RLOG("Connecting ftp...");
		if(!ftp.Connect(getenv("UPPFTP"), getenv("UPPFTPUSR"), getenv("UPPFTPPWD"), true)) {
			RLOG("Unable to connect!" + ftp.GetError());
			SetExitCode(1);
			return;
		}
		if(!ftp.Cd("www")) {
			RLOG("Unable to 'cd www'");
			SetExitCode(1);
			return;
		}
		for(int i = 0; i < upload.GetCount(); i++) {
			RLOG("Uploading " << upload[i]);
			String s = LoadFile(AppendFileName(targetdir, upload[i]));
			if(!ftp.Save(upload[i], s)) {
				RLOG("FTP error (file upload): " + ftp.GetError());
				SetExitCode(1);
				return;
			}
			SaveFile(AppendFileName(diffdir, upload[i]), s);
		}
	}
	BeepInformation();
}
