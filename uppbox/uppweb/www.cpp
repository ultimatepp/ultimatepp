#include "www.h"

#include <build_info.h>
#include <plugin/ftp/ftp.h>

#define IMAGECLASS WWW
#define IMAGEFILE  <uppweb/www.iml>
#include <Draw/iml.h>

#define TFILE <uppweb/www.t>
#include <Core/t.h>

#define LLOG(x)  // LOG(x)

String devdir   = GetHomeDirFile("upp.src");
String rootdir   = GetHomeDirFile("upp.stable");
String uppbox    = rootdir + "/uppbox";
String uppsrc    = rootdir + "/uppsrc";
String devsrc    = devdir + "/uppsrc";
String reference = rootdir + "/reference";
String examples  = rootdir + "/examples";
#ifdef _DEBUG
String targetdir = GetHomeDirFile("www");
#else
String targetdir = "/var/www";
#endif

VectorMap<String, String> escape;

ArrayMap<String, Topic> tt;
Vector<int> ttId;
Vector<String> ttFullIds;

VectorMap<String, String> links;
VectorMap<String, String> labels;
Htmls header, lastUpdate;

Array <Htmls> bar;
Array <int> languages;

typedef Image (*ImageFn)();

Index<ImageFn> il;

namespace Upp {

struct TopicData__ : Moveable<TopicData__> {
	String      title;
	const byte *data;
	int         len;
};

VectorMap<String, VectorMap<String, VectorMap<String, TopicData__ > > >& Topics__();

};

void ClearGlobals()
{
	reflink.Clear();

	escape.Clear();
	
	tt.Clear();
	ttId.Clear();
	ttFullIds.Clear();
	
	links.Clear();
	labels.Clear();
	
	bar.Clear();
	languages.Clear();
	
	il.Clear();
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
hash_t GetHashValue(const ::ImageFn& fn) { return (unsigned)(uintptr_t) fn; }
};

String GetImageSrc(ImageFn img)
{
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
	  HtmlForm("https://www.google.com/search", false, "GET")
	    .Style("margin:0px;padding:0px;") /
	    ( HtmlHidden("ie", "UTF-8") +
	      HtmlHidden("oe", "UTF-8") +
	      ~HtmlEdit("q", 15).Attr("placeholder", t_("Site search")) +
	      ~HtmlHidden("sitesearch", domain)
	    );

	Htmls content;
	content << HtmlPackedTable().Width(-100)
	           / HtmlRow() / (
	               HtmlCell() / Wimg(WWW::google) +
	               HtmlCell() / form
	            );

	String style = "border: 0px solid black; "
	               "padding-left:6px; padding-right:0px; "
	               "padding-top:4px; padding-bottom:4px;"
		           "border-top: 1px solid #6E89AE;";
	return BarItem(content, style);
}

HtmlTag HtmlPadding(int p)
{
	return HtmlPackedTable().Width(-100) /
		   HtmlLine();
}

String QtfAsHtml(const char *qtf, Index<String>& css,
                 const VectorMap<String, String>& links,
                 const VectorMap<String, String>& labels,
                 const String& outdir, const String& fn = Null)
{
	return EncodeHtml(ParseQTF(qtf), css, links, labels, outdir, fn, Zoom(8, 40), escape, 40);
}

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

String Www(const char *topic, int lang, String topicLocation = "topic://uppweb/www/")
{
	String strLang = ToLower(LNGAsText(lang));
	String www = GatherTopics(tt, ttFullIds, String().Cat() << topicLocation << topic << "$" << strLang, "", false);
	if (www != "index.html")
		return www;
	return GatherTopics(tt, ttFullIds, String().Cat() << topicLocation << topic << "$" << "en-us", "", false);
}

struct Title {
	String title, link;
	Title(String title, String link) : title(TrimBoth(title)), link(link) {};
	bool operator < (const Title& other) const {
		const char *val = title.Begin();
		const char *valOther = other.title.Begin();
		for(;; val++, valOther++) {
			String str, strOther;
			str.Clear();
			strOther.Clear();
			for (; *val != '\0' && *val != '.' && *val != ' '; ++val)
				str.Cat(*val);
			for (; *valOther != '\0' && *valOther != '.' && *valOther != ' '; ++valOther)
				strOther.Cat(*valOther);

			int num = ScanInt(str);
			int numOther = ScanInt(strOther);
			if (IsNull(num) && IsNull(numOther))
				return str < strOther;
			if (IsNull(num))
				return true;
			if (IsNull(numOther))
				return false;
			if (num != numOther)
				return num < numOther;
		}
		return title < other.title;
	}
};

String FolderLinks(String package, String group, int lang, int level, String parentIds)
{
	String qtf;
	Array<Title> titles;
	FindFile ff(AppendFileName(AppendFileName(AppendFileName(uppsrc, package), group + ".tpp"), "*.tpp"));
	while(ff) {
		if(ff.IsFile()) {
			if (ff.GetName().Find("en-us") >= 0) {
				String title;
				String tl = "topic://" + package + '/' + group + '/' + GetFileTitle(ff.GetName());
				GatherTopics(tt, ttFullIds, tl, title, parentIds, false);
				titles.Add(Title(DeQtf(Nvl(title, tl)), tl));
				//qtf << "________[^" << tl << "^ " << DeQtf(Nvl(title, tl)) << "]&";
			}
		}
		ff.Next();
	}
	Sort(titles);
	for (int i = 0; i < titles.GetCount(); ++i)
		qtf << "________[^" << titles[i].link << "^ " << titles[i].title << "]&";
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

String MakeExamples(const char *dir, const char *www, int language, String parentIds)
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
		int q = tt.GetCount() - 1;
		String fullIds = parentIds + "/" + FormatInt(q);
		ttFullIds.Add(fullIds);

		String fn = AppendFileName(
						AppendFileName(
							AppendFileName(uppbox, "uppweb"),
							String(www) + ".tpp"
						),
						topic.title + "_" + ToLower(LNGAsText(language))  + ".tpp"
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
		AddFiles(topic.text, d, "icpp", b);
		AddFiles(topic.text, d, "java", b);
		AddFiles(topic.text, d, "witz", b);
		AddFiles(topic.text, d, "usc", b);
		AddFiles(topic.text, d, "lay", b);
		AddFiles(topic.text, d, "key", b);
		AddFiles(topic.text, d, "brc", b);
		AddFiles(topic.text, d, "sch", b);
		AddFiles(topic.text, d, "xml", b);
		if(b) {
			if(next)
				ttxt << "\n::^ ";
			ttxt << "[^" << link << "^* " << DeQtf(topic.title) << "]::^ "
			     << DeQtf(p.description);
			next = true;
		}
	}
	ttxt << "}}&&";
	return ttxt;
}

class HeaderNumber {
public:
	HeaderNumber(const String& prefix = {})
		: m_prefix(prefix)
	{
	}

	bool IsFirst() const  { return m_number == 1; }
	int GetNumber() const { return m_number; }
	String GenNext()      { return m_prefix + IntStr(m_number++); }

private:
	String m_prefix;
	int m_number = 1;
};

class PackageDocsGenerator final {
public:
	PackageDocsGenerator()
		: m_hnumber("4.")
	{}

	const Vector<String>& GetHeaders() { return m_headers; }

	void Generate(Index<String>& x, String& qtf, const char* folder, int lang, int level,
	              String parent)
	{
		if(x.Find(folder) >= 0)
			return;
		x.Add(folder);
		String srcdoc = FolderLinks(folder, "srcdoc", lang, level, parent);
		String src = FolderLinks(folder, "src", lang, level, parent);
		Package p;
		p.Load(AppendFileName(uppsrc, AppendFileName(folder, GetFileName(folder) + ".upp")));
		if(srcdoc.GetLength() || src.GetLength()) {
			bool first = m_hnumber.IsFirst();
			String ref = ":4`_" + IntStr(m_hnumber.GetNumber()) + ":";
			m_headers.Add(m_hnumber.GenNext() + " " + folder);

			qtf << (first ? "" : "[s0; &&]\n") << "[s19;" << ref << " " << m_headers.Top() << " 📦&]";
			if(!IsNull(p.description))
				qtf << "[2 " << p.description << "&]";
			if(srcdoc.GetCount()) {
				qtf << "&[3/* " + Format(t_("Using %s"), folder) << "&]";
				qtf << srcdoc;
			}
			if(src.GetCount()) {
				qtf << "&[3/* " << Format(t_("%s reference"), folder) << "&]";
				qtf << src;
			}
			qtf << "\n";
		}
	}

private:
	HeaderNumber m_hnumber;
	Vector<String> m_headers;
};

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

String TrimDescription(String description)
{
	description.Replace("\"", "");
	description.Replace(":", ".");

	constexpr int DESCRIPTION_SIZE_LIMIT = 158;
	if (description.GetLength() <= DESCRIPTION_SIZE_LIMIT) {
		return description;
	}

	description.Trim(DESCRIPTION_SIZE_LIMIT);

	int trim_idx = -1;
	for (int i = description.GetCount() - 1; i >= 0; i--)
	{
		if (description[i] != '.' && description[i] != '?') {
			continue;
		}

		trim_idx = i + 1;
		break;
	}

	if (trim_idx == -1 || trim_idx == 0) {
		return description;
	}

	description.Trim(trim_idx);
	return description;
}

String FindPageDescription(const RichText& qtf)
{
	String description;
	Uuid style_id = qtf.GetStyleId("text");
	if (!style_id.IsNullInstance()) {
		RichStyle style = qtf.GetStyle(style_id);
		for (int i = 0; i < qtf.GetPartCount(); i++) {
			if (!qtf.IsPara(i)) {
				continue;
			}

			RichPara para = qtf.Get(i);
			if (style_id == qtf.GetParaStyle(i)) {
				description = para.GetText().ToString();
				break;
			}
		}
	}

	return TrimDescription(description);
}

String AdSense3()
{
#ifdef _DEBUG
	return Null;
#else
	return LoadFile(GetRcFile("adsense3.txt"));
#endif
}

String AdSense2()
{
#ifdef _DEBUG
	return Null;
#else
	return LoadFile(GetRcFile("adsense2.txt"));
#endif
}

String AdSense()
{
#ifdef _DEBUG
	return Null;
#else
	return LoadFile(GetRcFile("adsense.txt"));
#endif
}

String AdLinks()
{
#ifdef _DEBUG
	return Null;
#else
	return LoadFile(GetRcFile("adlinks.txt"));
#endif
}

void ExportPage(int i)
{
	Index<String> css;
	String path = links.GetKey(i);
	RLOG("Exporting " << path);

	int ilang = GetLinkLanguage(path);
	SetLanguage(languages[ilang]);
	String text = GetText(path);
	RichText qtf = ParseQTF(tt[i].text);
	int h = qtf.GetHeight(1000);

	String qtflangs;
	String strlang;
	String jslang;
	Array <String> arrLangs;
	for (int il = 0; il < languages.GetCount(); ++il) {
		String topic = ChangeTopicLanguage(path, languages[il]);
		int itopic;
		if ((itopic = tt.Find(topic)) >= 0) {
			if (tt[itopic].title.Find(" (translated)") < 0) {
				if (il != ilang) {
					if (!strlang.IsEmpty())
						strlang << ", ";
					arrLangs.Add("[^" + links[itopic] + "^ [2 " + ToLower(GetNativeLangName(languages[il])) + "]]");
				}
				jslang+="'"+ links[itopic] +"':'" + GetNativeLangName(languages[il]) + "',";
			}
		}
	}
	if(!jslang.IsEmpty())
		jslang.Trim(jslang.GetCount()-1);
	if (arrLangs.GetCount() > 0) {
		for (int i = 0; i < arrLangs.GetCount(); ++i) {
			if (i == arrLangs.GetCount()-1 && i != 0)
				strlang << String(" ") + t_("and") + " ";
			else if (i > 0)
				strlang << ", ";
			strlang << arrLangs[i];
		}
	}

	if (tt[i].title.Find("How to contribute. Web page") < 0) {
		String help = "topic://uppweb/www/contribweb$" + ToLower(LNGAsText(languages[ilang]));
		qtflangs += " " + String("[^") + help + "^ [<A2 " + t_("Do you want to contribute?") + "]]";
	}

	String langs = QtfAsHtml(qtflangs, css, links, labels, targetdir, links[i]);
	String page = tt[i];

	Array<String> htmlrep;
	int posB = 0;
	while (true) {
		posB = page.Find("[IHTMLTEXT", posB);
		if (posB < 0)
			break;
		int posBB = posB + (int)strlen("[IHTMLTEXT");
		int pos0 = page.ReverseFind("[", posB-1);
		int posE = page.Find(";2", posBB);
		String html0 = page.Mid(posBB, posE - posBB);
		html0.Replace("`", "");
		htmlrep.Add(html0);
		int posEE = page.Find("]&]", posE) + (int)strlen("]&]");

		page = page.Left(pos0) + "QTFHTMLTEXT" + FormatInt(htmlrep.GetCount()-1) + page.Mid(posEE+1);
	}

	page = QtfAsHtml(page, css, links, labels, targetdir, links[i]);
	for (int iHtml = 0; iHtml < htmlrep.GetCount(); ++iHtml)
		page.Replace(String("QTFHTMLTEXT") + FormatInt(iHtml), htmlrep[iHtml]);

	if(path == "topic://uppweb/www/download$en-us")
		page << AdSense3();
	Color bg = Color(210, 217, 210);
	Htmls footer;
	footer << HtmlTable().Border(0).Width(-100) / HtmlLine() +
				~RoundFrame(HtmlPadding(8) / langs , "6E89AE;padding: 10px;", White);
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
					LoadFile(GetRcFile("facebook.txt")) +
					"<br><br>" +
//						"<p align=\"center\">" +
					AdSense2() +
					"<br><br>" +
					AdLinks() +
					(h > 25000 ? "<br><br>" + AdSense2() : "") +
					"<br><br><br>" +
					"<br><br><br>" +
					~(HtmlLink("https://sourceforge.net/projects/upp/") /
					   HtmlImg("https://sourceforge.net/sflogo.php?group_id=93970&type=2",
					           "SourceForge.net Logo").Border(0).Width(125).Height(37)) +
					"<br><br>" +
					~(HtmlLink("https://flathub.org/en/apps/org.ultimatepp.TheIDE") /
					   HtmlImg(GetImageSrc(WWW::Flathub),
					           "SourceForge.net Logo").Border(0).Width(125).Height(37)) +
					"<br><br>" +
					"<div style=\"background-color:#ffffff;width:125;height:35\">" +
						~(HtmlLink("https://github.com/ultimatepp") /
						   HtmlImg(GetImageSrc(WWW::GitHub),
					           "GitHub Logo").Border(0).Width(125).Height(35)) +
					"</div>" +
					"<br>" +
					"<div style=\"background-color:#ffffff;width:125;height:35\">" +
					"<div style=\"height:5\"></div>" +
					   ~(HtmlLink("https://discord.gg/8XzqQzXZzb") /
					      HtmlImg("https://assets-global.website-files.com/6257adef93867e50d84d30e2/636e0b5061df29d55a92d945_full_logo_blurple_RGB.svg",
					           "Discord Logo").Border(0).Width(125).Height(25)) +
					"</div>" +
					"<br>" +
					(links[i] == "index.html" ? lastUpdate : Htmls())
				) +
				HtmlTCell().BgColor(bg) / BoxWidth(6) / "" +
				HtmlTCell().Width(-100).BgColor(bg) / (
					RoundFrame(HtmlPadding(8) / page , "6E89AE;padding: 10px;", White) +
					footer
				)
			)
		) + Htmls("<script type=\"text/javascript\">var a={"+jslang+"};"
				"var p=window.location.pathname.split(\"/\");p=p[p.length-1];"
				"var s='<select id=\"lang\" onchange=\"window.location=document.getElementById(\\'lang\\').value;\">';"
				"for(l in a){if(p==l){d=\" selected=1\"}else{d=\"\"}s+='<option value=\"'+l+'\"'+d+'>'+a[l]+'</option>'}"
				"s+='</select>';"
				"var d=document.createElement('div');d.innerHTML=s;"
				"var c=document.getElementById('langs');c.replaceChild(d,c.firstChild);"
				"document.getElementById('langbox').style.display='block';</script>");

	String topicTitle = tt.GetKey(i);
	String pageTitle = tt[i].title;
	if(IsNull(pageTitle))
		pageTitle = "U++";
	else if(StartsWith(topicTitle, "examples"))
		pageTitle = "Demos / " + pageTitle;
	else if(StartsWith(topicTitle, "reference"))
		pageTitle = "Examples / " + pageTitle;

	bool is_index = path == "topic://uppweb/www/index$en-us";
	if(pageTitle != "U++" && !is_index)
		pageTitle << " :: U++";

	auto htmlHeader = HtmlHeader()
		.Title(pageTitle)
		.Description(FindPageDescription(qtf))
		.Css(AsCss(css) +
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
			"a.l2:active  { color:#BC0024; }\n")
		.Other(
			"<META NAME=\"keywords\" "
			"CONTENT=\""
			"framework, toolkit, widget, c++, visual, studio, dev-cpp, builder, ide, class, component,"
			"wxwidgets, qt, rapid, application, development, rad, mfc, linux, macos, gui, sdl, directx, desktop"
			"\">"
	        "<META name=\"robots\" content=\"index,follow\">\n"
            "<LINK rel=\"alternate\" type=\"application/rss+xml\" title=\"SVN changes\" href=\"svnchanges.xml\">\n"
            "<LINK rel=\"shortcut icon\" type=\"image/png\" href=\"favicon.png\">\n")
		.Create();

	Htmls content =
	    //"<!DOCTYPE html>" +
	    "<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">\n" +
		htmlHeader
	    .BgColor(bg)
	    .Alink(Red).Link(Black).Vlink(Blue)
	    / html;
	if(!SaveFile(AppendFileName(targetdir, links[i]), content)) {
		RLOG("SaveFile failed!");
		abort();
	}
	RLOG("Exported page " << AppendFileName(targetdir, links[i]));
}

struct DownloadItem {
	Time tm;
	String fn;
	String path;
	int64 len;
};

String Downloads()
{
	String r;
	r << "{{1:2:2^@L "
	     "Date:: "
	     "[^app$ide$install_win32_en-us.html^ U`+`+ for Windows (with CLANG)]:: "
	     "[^app$ide$install_posix_en-us.html^ U`+`+ for Linux/MacOS/BSD/Solaris]";
	FindFile ff(AppendFileName(targetdir, "downloads/*.*"));

	SortedArrayMap<Date, Array<DownloadItem>> downs;
	while(ff) {
		if(ff.IsFile()) {
			DownloadItem down;
			down.tm = ff.GetLastWriteTime();
			down.fn = ff.GetName();
			down.path = ff.GetPath();
			down.len = ff.GetLength();
			Date date = down.tm;
			downs.GetAdd(date).Add(down);
		}
		ff.Next();
	}

	auto FindDown = [](Array<DownloadItem>& a, const char* arch) -> String {
		String r;
		for(int i = 0; i < a.GetCount(); i++) {
			if(a[i].fn.Find(arch) >= 0) {
				r << FormatTime(a[i].tm, "hh:mm")
				  << " [^downloads/" << a[i].fn << "^ \1" << a[i].fn << "\1]"
				  << " (" << (a[i].len >> 20) << " MB)";
				break;
			}
		}
		return r;
	};

	Date d = GetSysDate();

	for(int i = 0; i < 14; i++, d--) {
		Array<DownloadItem>& a = downs.GetAdd(d);
		r << "::@W " << FormatDate(d, "YYYY-MM-DD") << ":: "
		  << FindDown(a, "win") << ":: "
		  << FindDown(a, "posix");
	}
	r << "}}";

	for(int i = 14; i < downs.GetCount(); i++) {
		if(downs.GetKey(i) < d)
			for(int j = 0; j < downs[i].GetCount(); j++)
				DeleteFile(downs[i][j].path);
	}

	return r;
}

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	RLOG("--- uppweb started at " << GetSysTime());

	const Vector<String>& cmd = CommandLine();
	
	if(cmd.GetCount() < 3) {
		RLOG("Usage: uppweb rootdir targetdir [0|1]");
		Exit(1);
	}
	
	rootdir = cmd[0];
	targetdir = cmd[1];
	
	if(!DirectoryExists(rootdir)) {
		RLOG(rootdir << " does not exist");
		Exit(1);
	}

	if(!DirectoryExists(targetdir)) {
		RLOG(targetdir << " does not exist");
		Exit(1);
	}
	
	bool isdev = cmd[2] == "1";

	uppsrc = AppendFileName(rootdir, "uppsrc");

	RealizeDirectory(targetdir);

	RLOG("RootDir: " << rootdir);
	RLOG("TargetDir: " << targetdir);

	String downloads = Downloads();

	if (!DirectoryExists(rootdir)) {
		Cout() << ("Directory " + DeQtf(rootdir) + " does not exist\n");
		return;
	}

	uppbox =    AppendFileName(rootdir, "uppbox");
	reference = AppendFileName(rootdir, "reference");
	examples =  AppendFileName(rootdir, "examples");

	InitWwwTpp();

	languages.Add(LNG_('E','N','U','S'));		// en-us has to be the first one

	RealizeDirectory(targetdir);

	RLOG("Gather ref links " << uppsrc);
	GatherRefLinks(uppsrc, false);

	SaveFile(AppendFileName(targetdir, "sdj.gif"), LoadFile(GetRcFile("sdj.gif")));

#ifdef bmGIT_REVCOUNT
	auto version = AsString(atoi(bmGIT_REVCOUNT) + 2270);
	escape.Add("LATESTGIT", version);
#endif

	String release = "17810";
	escape.Add("RELEASE", release);
	escape.Add("RELEASET", release);
	escape.Add("UPDATETIME", Format("%`", GetUtcTime()));

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
			    / HtmlArial(14) / (AdSense() + "&nbsp;&nbsp;" + (isdev ? "<br>development version (master branch)" : ""))
			);

	bar.SetCount(languages.GetCount());

	int currentLang = GetCurrentLanguage();
	for (int i = 0; i < languages.GetCount(); ++i) {
		Cout() << "Language " << LNGAsText(languages[i]);
		Htmls bi, bex, bdoc, bcom, bcon, bsearch, blang;
		int lang = languages[i];

		SetLanguage(lang);

		Www("index", lang);
		Www("contribweb", lang);
	//	bi << BarLink("index.html", "Home", false);
		bi << BarLink(Www("overview", lang), t_("Overview"), false);
		bi << BarLink(Www("examples", lang), t_("Examples"));
		{
			IGNORE_RESULT(Www("reference", lang));

			int ri = tt.Find("topic://uppweb/www/reference$" + ToLower(LNGAsText(lang)));
			int di = tt.Find("topic://uppweb/www/examples$" + ToLower(LNGAsText(lang)));

			tt[di].text << MakeExamples(examples, "examples", lang, String("/") + FormatInt(di));
			tt[di].text << tt[ri].text << '\n';
			tt[di].text << MakeExamples(reference, "reference", lang, String("/") + FormatInt(di));
		}

		bi << BarLink(Www("ss", lang), t_("Screenshots"));
		bi << BarLink(Www("comparison", lang), t_("Comparisons"));
		bi << BarLink(Www("apps", lang), t_("Applications"));
		bi << BarLink(Www("download", lang), t_("Download"));

		bi << BarLink(Www("documentation", lang), t_("Documentation"));
		{
			int di = tt.Find("topic://uppweb/www/documentation$" + ToLower(LNGAsText(lang)));
			if (di >= 0) {
				Index<String> x;
				x.Clear();
				String qtf;
				PackageDocsGenerator dgen;

				dgen.Generate(x, qtf, "Core", lang, 2, String("/") + FormatInt(di) + "/[Core]");
				dgen.Generate(x, qtf, "Core/POP3", lang, 2, String("/") + FormatInt(di) + "/[Core/POP3]");
				dgen.Generate(x, qtf, "Core/SMTP", lang, 2, String("/") + FormatInt(di) + "/[Core/SMTP]");
				dgen.Generate(x, qtf, "Core/SSH", lang, 2, String("/") + FormatInt(di) + "/[Core/SSH]");
				dgen.Generate(x, qtf, "Draw", lang, 2, String("/") + FormatInt(di) + "/[Draw]");
				dgen.Generate(x, qtf, "CtrlCore", lang, 2, String("/") + FormatInt(di) + "/[CtrlCore]");
				dgen.Generate(x, qtf, "CtrlLib", lang, 2, String("/") + FormatInt(di) + "/[CtrlLib]");
				dgen.Generate(x, qtf, "RichText", lang, 2, String("/") + FormatInt(di) + "/[RichText]");
				dgen.Generate(x, qtf, "RichEdit", lang, 2, String("/") + FormatInt(di) + "/[RichEdit]");
				dgen.Generate(x, qtf, "Sql", lang, 2, String("/") + FormatInt(di) + "/[Sql]");

				String d = AppendFileName(uppsrc, "*.*");
				String p;
				for(int pass = 0; pass < 2; pass++) {
					FindFile ff(d);
					Array <String> folders;
					folders.Clear();
					while(ff) {
						if(ff.IsFolder() && ff.GetName() != "Web")
							folders.Add(p + ff.GetName());
						ff.Next();
					}
					Sort(folders);
					for (int ifold = 0; ifold < folders.GetCount(); ++ifold) {
						dgen.Generate(x, qtf, folders[ifold], lang, 2, String("/") + FormatInt(di) + "/[" + folders[ifold] + "]");
					}
					d = AppendFileName(uppsrc, "plugin/*.*");
					p = "plugin/";
				}
				tt[di].text.Replace("[s5; <#packages#>]", qtf);

				String toc_qtf;
				const auto& headers = dgen.GetHeaders();
				for (int i = 0; i < headers.GetCount(); i++) {
					toc_qtf
						<< "[s0;l128; [^topic`:`/`/uppweb`/www`/documentation`$en`-us`#4`_"
						<< IntStr(i + 1) << "^ " << headers[i] << "]&]";
				}

				tt[di].text.Replace("[s0;l128; <#packages`_toc#>&]", toc_qtf);
			}
		}
		bi << BarLink(Www("Tutorials", lang), t_("Tutorials"));
		bi << BarLink(Www("UppHub", lang, "topic://ide/app/"), t_("UppHub"));
		bi << BarLink(Www("Roadmap", lang), t_("Status & Roadmap"));
		bi << BarLink(Www("FAQ", lang), t_("FAQ"));
		bi << BarLink(Www("About", lang, "topic://ide/app/"), t_("Authors & License"));

		bi << BarLink("https://www.ultimatepp.org/forums", t_("Forums"));
	//	bcom << BarLink(Www("mailing"), "Mailing lists");
	//	bi << BarLink("http://www.ultimatepp.org/wiki/index.php", "Wiki");
		bi << BarLink(Www("Funding", lang), t_("Funding U++"));
	//	bcom << BarLink(Www("helpus"), "Getting involved");
	//	bcom << BarLink("mailto: upp@ntllib.org", "Contact developers");

		bsearch << BarCaption(t_("Search on this site"));
		bsearch << SearchBar("ultimatepp.org");

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
	//	      bf / blang + div;
	}
	SetLanguage(currentLang);

	for(int i = 0; i < tt.GetCount(); i++) {
		String topic = tt.GetKey(i);
		links.Add(topic, topic == "topic://uppweb/www/index$en-us" ? "index.html" :
		                 memcmp(topic, "topic://", 8) ? topic : TopicFileNameHtml(topic, false));
	}

	for(int i = 0; i < tt.GetCount(); i++) {
		if(tt[i].title == "Nightly builds" || tt[i].title.Find("Download") >= 0)
			tt[i].text.Replace(String("<#downloads#>"), downloads);
		else if (links[i].Find("index") >= 0) {
			String win32 = "upp-win32-RELEASE.exe";
			String win32release = win32;
			win32release.Replace(String("RELEASE"), release);
			String x11 = "upp-x11-src-RELEASE.tar.gz";
			String x11release = x11;
			x11release.Replace(String("RELEASE"), release);
			tt[i].text.Replace(DeQtf(x11), DeQtf(x11release));
			tt[i].text.Replace(DeQtf(win32), DeQtf(win32release));
		}
	}
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

	lastUpdate = HtmlItalic() / HtmlArial(8) / HtmlFontColor(Gray()) /
	                   (String().Cat() << "Last update " << GetSysDate());

	RLOG("Creating htmls");

	for(int i = 0; i < tt.GetCount(); i++)
		ExportPage(i);
	SetLanguage(currentLang);

	FileCopy(AppendFileName(uppbox, "uppweb/favicon.png"), AppendFileName(targetdir, "favicon.png"));
	
	uppsrc = AppendFileName(devdir, "uppsrc");

	RLOG("Finished OK");
}

