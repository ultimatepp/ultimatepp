#include "ide.h"

String SearchEnginesFile()
{
	return ConfigFile("search_engines.json");
}

Value  LoadSearchEngines()
{
	return ParseJSON(LoadFile(SearchEnginesFile()));
}

struct IconDisplay : Display {
	virtual Size GetStdSize(const Value&) { return DPI(16, 16); }
	virtual void Paint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const
	{
		w.DrawRect(r, paper);
		Image m = StreamRaster::LoadStringAny(q);
		Point p = r.CenterPos(m.GetSize());
		w.DrawImage(p.x, p.y, m);
	}
};

WebSearchTab::WebSearchTab()
{
	list.AddColumn("Name", 5);
	list.AddColumn("URI", 5);
	list.AddColumn("Icon").SetDisplay(Single<IconDisplay>());
	list.Moving().RowName("search engine").Removing();
	list.WhenLeftDouble = [=] { Edit(); };
	list.SetLineCy(max(GetStdFontSize().cy, DPI(18)));
	list.WhenBar = [&](Bar& bar) {
		bool b = list.IsCursor();
		bar.Add("Add search engine", IdeImg::add(), [&] { Add(); }).Key(K_INSERT);
		bar.Add(b, "Edit search engine", IdeImg::pencil(), [&] { Edit(); }).Key(K_ENTER);
		bar.Add(b, "Set as default engine", IdeImg::star(), [&] { Default(); });
		list.StdBar(bar);
	};
	list.EvenRowColor();
	list.ColumnWidths("104 382 30");
}

bool WebSearchTab::EditDlg(String& name, String& uri, String& ico)
{
	WithWebSearchEngineLayout<TopWindow> dlg;
	CtrlLayoutOKCancel(dlg, "Web search engine setup");
	CtrlRetriever v;
	v(dlg.name, name)(dlg.uri, uri);
	if(dlg.Execute() == IDOK) {
		v.Retrieve();
		ico = Null;
		int q = uri.Find('/', max(0, uri.FindAfter("//")));
		if(q < 0)
			q = uri.GetCount();

		Image r;
		Size wanted = DPI(16, 16);
		String data = HttpRequest(uri.Mid(0, q) + "/favicon.ico").RequestTimeout(3000).Execute();
		for(const Image& m : ReadIcon(data)) {
			Size isz = m.GetSize();
			if(isz == wanted) {
				r = m;
				break;
			}
			if(isz.cx > r.GetSize().cx)
				r = m;
		}
		if(IsNull(r))
			r = StreamRaster::LoadStringAny(data);
		ico = IsNull(r) ? String() :
		      PNGEncoder().SaveString(2 * r.GetSize() == wanted ? Upscale2x(r)
		                              : r.GetSize() == 2 * wanted ? Downscale2x(r)
		                              : Rescale(r, wanted));
		return true;
	}
	return false;
}

void WebSearchTab::Sync()
{
	for(int i = 0; i < list.GetCount(); i++) {
		list.SetDisplay(i, 0, i == 0 ? BoldDisplay() : StdDisplay());
		list.SetDisplay(i, 1, i == 0 ? BoldDisplay() : StdDisplay());
	}
}

void WebSearchTab::Add()
{
	String name, uri, zico;
	if(EditDlg(name, uri, zico))
		list.Add(name, uri, zico);
	Sync();
}

void WebSearchTab::Edit()
{
	if(!list.IsCursor())
		return;
	String name = list.Get(0);
	String uri = list.Get(1);
	String zico = list.Get(2);
	if(EditDlg(name, uri, zico)) {
		list.Set(0, name);
		list.Set(1, uri);
		list.Set(2, zico);
	}
	Sync();
}

void WebSearchTab::Default()
{
	int i = list.GetCursor();
	if(i < 0) return;
	Vector<Value> v = list.GetLine(i);
	list.Remove(i);
	list.Insert(0, v);
	list.SetCursor(0);
	Sync();
}

void WebSearchTab::Load()
{
	for(Value se : LoadSearchEngines())
		list.Add(se["Name"], se["URI"], Decode64(se["Icon"]));
	Sync();
}

void WebSearchTab::Save()
{
	JsonArray ja;
	for(int i = 0; i < list.GetCount(); i++)
		ja << Upp::Json("Name", list.Get(i, 0))("URI", list.Get(i, 1))("Icon", Encode64(list.Get(i, 2)));
	
	SaveChangedFile(SearchEnginesFile(), ja);
}

void Ide::OnlineSearchMenu(Bar& menu)
{
	static Time search_engines_tm = Null;
	static Value search_engines;
	
	Time h = FileGetTime(SearchEnginesFile());
	if(h != search_engines_tm) {
		search_engines = LoadSearchEngines();
		search_engines_tm = h;
	}
	
	bool b = editor.IsSelection() || IsAlNum(editor.GetChar()) || editor.GetChar() == '_';

	auto OnlineSearch = [=](const String& url) {
		String h = url;
		h.Replace("%s", UrlEncode(Nvl(editor.GetSelection(), editor.GetWord())));
		LaunchWebBrowser(h);
	};
	
	auto Icon = [&](int i) {
		return StreamRaster::LoadStringAny(Decode64(search_engines[i]["Icon"]));
	};

	String name, uri;
	Image m;

	if(search_engines.GetCount()) {
		name = search_engines[0]["Name"];
		uri = search_engines[0]["URI"];
		m = Icon(0);
	}
	else {
		name = "Google";
		uri = "https://www.google.com/search?q=%s";
	}

	using namespace IdeKeys;

	menu.Add(b, "Search on " + name, Nvl(m, CtrlImg::Network()), [=] { OnlineSearch(uri); }).Key(AK_GOOGLE);
	menu.Add(b, AK_GOOGLEUPP, IdeImg::GoogleUpp(), [=] {
		OnlineSearch("https://www.google.com/search?q=%s"
		             "&domains=www.ultimatepp.org&sitesearch=www.ultimatepp.org");
	});

	if(!menu.IsMenuBar() || search_engines.GetCount() < 2)
		return;

	menu.Sub(b, "Search on...", [=](Bar& menu) {
		for(int i = 1; i < search_engines.GetCount(); i++) {
			const String& name = search_engines[i]["Name"];
			const String& uri  = search_engines[i]["URI"];
			menu.Add(b, name, Nvl(Icon(i), CtrlImg::Network()), [=] { OnlineSearch(uri); });
		}
	});
}
