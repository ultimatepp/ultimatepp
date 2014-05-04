#ifdef flagGUI
#include <CtrlLib/CtrlLib.h>
#include <ide/Browser/Browser.h>

#include <Functions4U/Functions4U.h>
#include "GatherTpp.h"
#include <Functions4U/Html/htmld.h>

NAMESPACE_UPP

struct ScanTopicIterator : RichText::Iterator {
	VectorMap<String, String> *reflink;
	String link;
	StaticCriticalSection reflink_lock;
	
	ScanTopicIterator(VectorMap<String, String> *reflink) : reflink(reflink) {};
	virtual bool operator()(int pos, const RichPara& para)
	{
		if(!IsNull(para.format.label)) 
			reflink->Add(para.format.label, link);
		return false;
	}
};

void GatherTpp::GatherRefLinks(const char *upp)
{
	for(FindFile pff(AppendFileName(upp, "*.*")); pff; pff.Next()) {
		if(pff.IsFolder()) {
			String package = pff.GetName();
			String pdir = AppendFileName(upp, package);
			TopicLink tl;
			tl.package = package;
			for(FindFile ff(AppendFileName(pdir, "*.tpp")); ff; ff.Next()) {
				if(ff.IsFolder()) {
					String group = GetFileTitle(ff.GetName()	);
					tl.group = group;
					String dir = AppendFileName(pdir, ff.GetName());
					for(FindFile ft(AppendFileName(dir, "*.tpp")); ft; ft.Next()) {
						if(ft.IsFile()) {
							String path = AppendFileName(dir, ft.GetName());
							tl.topic = GetFileTitle(ft.GetName());
							String link = TopicLinkString(tl);
							ScanTopicIterator sti(&reflink);
							sti.link = link;
							ParseQTF(ReadTopic(LoadFile(path))).Iterate(sti);
						}
					}
				}
			}
		}
	}
}

struct GatherLinkIterator : RichText::Iterator {
	VectorMap<String, String> *reflink;
	Index<String> link;

	GatherLinkIterator(VectorMap<String, String> *reflink) : reflink(reflink) {};
	virtual bool operator()(int pos, const RichPara& para)
	{
		for(int i = 0; i < para.GetCount(); i++) {
			String l = para[i].format.link;
			if(!IsNull(l)) {
				if(l[0] == ':') {
					int q = reflink->Find(l);
					int w = q;
					if(q < 0)
						q = reflink->Find(l + "::class");
					if(q < 0)
						q = reflink->Find(l + "::struct");
					if(q < 0)
						q = reflink->Find(l + "::union");
					if(q >= 0)
						l = (*reflink)[q];
				}
				link.FindAdd(Nvl(reflink->Get(l, Null), l));
			}
		}
		return false;
	}
};

String GetIndexTopic(String file)
{
	String topic = GetFileTitle(file);
	String folder = GetFileFolder(file);
	String topicLocation = GetFileTitle(folder);
	folder = GetUpperFolder(folder);
	topicLocation = GetFileTitle(folder) + "/" + topicLocation;

	return "topic://" + topicLocation + "/" + topic;
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

HtmlsD RoundFrame(HtmlsD data, String border, Color bg)
{
	return HtmlPackedTableD()
				.BgColor(bg).Width(-100)
				.Attr("style", "border-style: solid; border-width: 1px; border-color: #" + border + ";") 
			/ HtmlLineD() / data;
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

String GatherTpp::QtfAsHtml(const char *qtf, Index<String>& css,
                 const VectorMap<String, String>& links,
                 const VectorMap<String, String>& labels,
                 const String& outdir, const String& fn)
{
	return EncodeHtml(ParseQTF(qtf), css, links, labels, outdir, fn, Zoom(8, 40), escape, 40);
}

String GetText(const char *s)
{
	return GetTopic(s).text;
}

void GatherTpp::ExportPage(int i, String htmlFolder, String keywords)
{
	Index<String> css;
	String path = links.GetKey(i);
	
	String text = GetText(path);
	int h;
	h = ParseQTF(tt[i].text).GetHeight(1000);
	
	String qtflangs;	
	String strlang;

	String page = tt[i];
	page = QtfAsHtml(page, css, links, labels, htmlFolder, links[i]);
	
	Color paper = SWhite;
	Color bg = Color(210, 217, 210);

	HtmlsD html;
	html << 
		HtmlPackedTableD().Width(-100) /
		   	HtmlLineD().ColSpan(3)  +
		HtmlPackedTableD().Width(-100) / (
			HtmlLineD().ColSpan(3).BgColor(bg).Height(6) / "" +
			HtmlRowD() / (
				HtmlTCellD().Width(-100).BgColor(bg) / (
					RoundFrame(page , "6E89AE;padding: 10px;", White)
				)
			)
		);

	String topicTitle = tt.GetKey(i);
	String pageTitle = tt[i].title;
	if(IsNull(pageTitle))
		pageTitle = title;
/*
	if(StartsWith(topicTitle, "examples$"))
		pageTitle = "Demos / " + pageTitle;
	else if(StartsWith(topicTitle, "reference$"))
		pageTitle = "Examples / " + pageTitle;
*/
	if(pageTitle != title)
		pageTitle << " :: " << title;

	HtmlsD content =
	    "<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">\n" +
		HtmlHeaderD(pageTitle, AsCss(css) +
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
			"CONTENT=\"" + keywords + "\">"
	        "<META name=\"robots\" content=\"index,follow\">"
		)
	    .BgColor(bg)
	    .Alink(Red).Link(Black).Vlink(Blue)
	    / html;
	SaveFile(AppendFileName(htmlFolder, links[i]), content);
}

String GatherTpp::TopicFileName(const char *topic)
{
	TopicLink tl = ParseTopicLink(topic);
	String file = AppendFileName(dir, AppendFileName(tl.group + ".tpp", tl.topic + ".tpp"));
	if (FileExists(file))
		return file;
		
	for (int i = 0; i < rootFolders.GetCount(); ++i) {
		if (rootFolders[i] != dir) {
			file = AppendFileName(rootFolders[i], AppendFileName(tl.package , AppendFileName(tl.group + ".tpp", tl.topic + ".tpp")));
			if (FileExists(file))
				return file;		
		}
	}
	return "";
}

String TopicFileNameHtml(const char *topic)
{
	TopicLink tl = ParseTopicLink(topic);
	return tl.group + "$" + tl.package+ "$" + tl.topic + ".html";
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

String GatherTpp::GatherTopics(const char *topic, String& title)
{
	static StaticCriticalSection mapl;
	int q;
	INTERLOCKED_(mapl)
		q = tt.Find(topic);
	if(q < 0) {
		Topic p = ReadTopic(LoadFile(TopicFileName(topic)));
		title = p.title;
		String t = p;
		if(IsNull(t)) {
			String topicEng = ChangeTopicLanguage(topic, LNG_('E','N','U','S'));		
			p = ReadTopic(LoadFile(TopicFileName(topicEng)));
			String tt = p;
			if(IsNull(tt)) 
				return "index.html";
			title = p.title;
			p.title += " (translated)";			
			String help = "topic://uppweb/www/contribweb$" + GetTopicLanguage(topic);
			p.text = String("{{1f1t0/50b0/50@(240.240.240) [<A2 ") + t_("This page has not been translated yet") + 
					"]. " + "[^" + help + "^ [<A2 " + t_("Do you want to translate it?") + "]]}}&&" + p.text;
		}
		INTERLOCKED_(mapl)
			tt.Add(topic) = p;
		GatherLinkIterator ti(&reflink);
		ParseQTF(t).Iterate(ti);
#ifdef MTC
		CoWork work;
		for(int i = 0; i < ti.link.GetCount(); i++)
			work & callback2(sGatherTopics, &tt, ti.link[i]);
#else
		for(int i = 0; i < ti.link.GetCount(); i++)
			GatherTopics(ti.link[i]);
#endif
	} else {
		INTERLOCKED_(mapl)
			title = tt[q].title;
	}
	return TopicFileNameHtml(topic);
}


String GatherTpp::GatherTopics(const char *topic)
{
	String dummy;
	return GatherTopics(topic, dummy);
}

bool GatherTpp::Load(String indexFile, Gate2<int, int> progress) {
	indexTopic = GetIndexTopic(indexFile);
	for (int i = 0; i < rootFolders.GetCount(); ++i) {
		if (progress(i+1, rootFolders.GetCount()))
			return false;
		dir = rootFolders[i];
		
		if (!DirectoryExists(dir)) 
			return false;

		GatherRefLinks(dir);
		
		if (i == 0)
			GatherTopics(indexTopic);
	}
	return true;
}

bool GatherTpp::MakeHtml(const char *folder, Gate2<int, int> progress) {
	DeleteFolderDeep(folder);
	DirectoryCreate(folder);

	for(int i = 0; i < tt.GetCount(); i++) {
		String topic = tt.GetKey(i);
		links.Add(topic, topic == indexTopic ? "index.html" :
		                 memcmp(topic, "topic://", 8) ? topic : TopicFileNameHtml(topic));
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

	for(int i = 0; i < tt.GetCount(); i++) {
		if (progress(i+1, tt.GetCount()))
			return false;
		ExportPage(i, folder);
	}
	return true;
}

bool GatherTpp::MakePdf(const char *filename, Gate2<int, int> progress) {
	PdfDraw pdf;
	for(int i = 0; i < tt.GetCount(); i++) {
		if (progress(int(0.6*(i+1)), tt.GetCount()))
			return false;
		bool dopdf = true;
		for (int j = 0; j < i; ++j) {
			if (tt[j].text == tt[i].text) {
				dopdf = false;
				break;
			}
		}
		if (dopdf)
			QtfAsPdf(pdf, tt[i]);
	}
	String rawPdf = pdf.Finish();		progress(9, 10);
	SaveFile(filename, rawPdf);			progress(10, 10);
	return true;	
}

int GatherTpp::FindTopic(const String name) {
	return tt.Find(name);
}

Topic &GatherTpp::GetTopic(int id) {
	return tt[id];
}

Topic &GatherTpp::AddTopic(const String name) {
	return tt.Add(name);
}

String GatherTpp::Www(const char *topic, int lang, String topicLocation) {
	String strLang = ToLower(LNGAsText(lang));
	String www = GatherTopics(String().Cat() << topicLocation << topic << "$" << strLang);
	if (www != "index.html")
		return www;
	return GatherTopics(String().Cat() << topicLocation << topic << "$" << "en-us");
}

END_UPP_NAMESPACE

#endif