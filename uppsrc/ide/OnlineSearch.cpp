#include "ide.h"

String SearchEnginesFile()
{
	return ConfigFile("web_search_engines.json");
}

Value  LoadSearchEngines()
{
	return ParseJSON(LoadFile(SearchEnginesFile()));
}

String GetWebsiteIconAsPNG(const String& uri, Progress& pi, bool uhd)
{
	String ico;
	int q = uri.Find('/', max(0, uri.FindAfter("//")));
	if(q < 0)
		q = uri.GetCount();

	Image r;
	Size wanted = uhd ? Size(32, 32) : Size(16, 16);
	pi.SetText("Setting up " + uri.Mid(0, q));
	HttpRequest http(uri.Mid(0, q) + "/favicon.ico");
	http.WhenWait = [&] {
		if(pi.StepCanceled())
			http.Abort();
	};
	String data = http.RequestTimeout(3000).Execute();
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
	return IsNull(r) ? String() :
	       PNGEncoder().SaveString(2 * r.GetSize() == wanted ? Upscale2x(r)
	                               : r.GetSize() == 2 * wanted ? Downscale2x(r)
	                               : Rescale(r, wanted));
}

void SearchEnginesDefaultSetup()
{
	static const Tuple<const char *, const char *> defs[] = {
		{ "Google", "https://www.google.com/search?q=%s" },
		{ "C++ reference", "https://en.cppreference.com/mwiki/index.php?search=%s" },
		{ "Wikipedia", "https://en.wikipedia.org/w/index.php?search=%s" },
		{ "GitHub", "https://github.com/search?q=%s" },
		{ "CodeProject", "https://www.codeproject.com/search.aspx?q=%s" },
		{ "WolframAlpha", "https://www.wolframalpha.com/input/?i=%s" },
	};
	
	Progress pi("Search engines setup");
	JsonArray ja;
	for(int i = 0; i < __countof(defs); i++)
		ja << Upp::Json("Name", defs[i].a)
		               ("URI", defs[i].b)
		               ("Icon16", Encode64(i == 0 ? PNGEncoder().SaveString(IdeImg::Google16())
		                                          : GetWebsiteIconAsPNG(defs[i].b, pi, false)))
		               ("Icon32", Encode64(i == 0 ? PNGEncoder().SaveString(IdeImg::Google32())
		                                          : GetWebsiteIconAsPNG(defs[i].b, pi, true)));

	SaveChangedFile(SearchEnginesFile(), ja);
}

struct IconDisplay : Display {
	virtual Size GetStdSize(const Value&) const { return DPI(16, 16); }
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
	if(IsUHDMode())
		list.AddIndex();
	list.AddColumn("Icon").SetDisplay(Single<IconDisplay>());
	if(!IsUHDMode())
		list.AddIndex();
	list.Moving().RowName("search engine").Removing();
	list.WhenLeftDouble = [=] { Edit(); };
	list.SetLineCy(max(GetStdFontSize().cy, DPI(18)));
	list.WhenSel << [=] { Sync(); };
	list.WhenBar = [=](Bar& bar) {
		bool b = list.IsCursor();
		bar.Add("Add search engine", IdeImg::add(), [=] { Add(); }).Key(K_INSERT);
		bar.Add(b, "Edit search engine", IdeImg::pencil(), [=] { Edit(); }).Key(K_ENTER);
		bar.Add(b, "Remove search engine", IdeImg::remove(), [=] { list.DoRemove(); Sync(); }).Key(K_DELETE);
		bar.Add(b, "Set as default engine", IdeImg::star(), [=] { Default(); Sync(); });
		bar.Add(b, "Move up", IdeImg::arrow_up(), [=] { list.SwapUp(); Sync(); }).Key(K_CTRL_UP);
		bar.Add(b, "Move down", IdeImg::arrow_down(), [=] { list.SwapDown(); Sync(); }).Key(K_CTRL_DOWN);
		bar.Separator();
		bar.Add("Restore the default list", [=] {
			if(PromptYesNo("Delete the current list and restore defaults?")) {
				SearchEnginesDefaultSetup();
				Load();
			}
		});
	};
	list.EvenRowColor();
	list.ColumnWidths("104 382 30");
	list.AutoHideSb();
	
	add.SetImage(IdeImg::add()) << [=] { Add(); };
	edit.SetImage(IdeImg::pencil()) ^= [=] { Edit(); };
	remove.SetImage(IdeImg::remove()) << [=] { list.DoRemove(); Sync(); };
	setdef.SetImage(IdeImg::star()) << [=] { Default(); };
	up.SetImage(IdeImg::arrow_up()) << [=] { list.SwapUp(); Sync(); };
	down.SetImage(IdeImg::arrow_down()) << [=] { list.SwapDown(); Sync(); };
}

bool WebSearchTab::EditDlg(String& name, String& uri, String& ico16, String& ico32)
{
	WithSetupWebSearchEngineLayout<TopWindow> dlg;
	CtrlLayoutOKCancel(dlg, "Web search engine setup");
	CtrlRetriever v;
	v(dlg.name, name)(dlg.uri, uri);
	if(dlg.Execute() == IDOK) {
		v.Retrieve();
		Progress pi;
		ico16 = GetWebsiteIconAsPNG(uri, pi, false);
		ico32 = GetWebsiteIconAsPNG(uri, pi, true);
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
	
	bool b = list.IsCursor();
	edit.Enable(b);
	remove.Enable(b);
	setdef.Enable(b);
	up.Enable(b);
	down.Enable(b);
}

void WebSearchTab::Add()
{
	String name, uri, zico16, zico32;
	if(EditDlg(name, uri, zico16, zico32))
		list.Add(name, uri, zico16, zico32);
	Sync();
}

void WebSearchTab::Edit()
{
	if(!list.IsCursor())
		return;
	String name = list.Get(0);
	String uri = list.Get(1);
	String zico16 = list.Get(2);
	String zico32 = list.Get(3);
	if(EditDlg(name, uri, zico16, zico32)) {
		list.Set(0, name);
		list.Set(1, uri);
		list.Set(2, zico16);
		list.Set(3, zico32);
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
	list.Clear();
	for(Value se : LoadSearchEngines())
		list.Add(se["Name"], se["URI"], Decode64(se["Icon16"]), Decode64(se["Icon32"]));
	Sync();
}

void WebSearchTab::Save()
{
	JsonArray ja;
	for(int i = 0; i < list.GetCount(); i++)
		ja << Upp::Json("Name", list.Get(i, 0))
		               ("URI", list.Get(i, 1))
		               ("Icon16", Encode64(list.Get(i, 2)))
		               ("Icon32", Encode64(list.Get(i, 3)));
	
	SaveChangedFile(SearchEnginesFile(), ja);
}

void Ide::OnlineSearchMenu(Bar& menu)
{
	static Time search_engines_tm = Null;
	static Value search_engines;
	static Vector<Image> search_icon;
	static Vector<bool> search_icon_loaded;
	static Time next_check = Time::Low();

	Time ct = GetSysTime();
	if(ct > next_check) {
		next_check = ct + 4;
		Time h = FileGetTime(SearchEnginesFile());
		if(h != search_engines_tm) {
			search_engines = LoadSearchEngines();
			search_engines_tm = h;
			search_icon_loaded.Clear();
		}
	}
	
	bool b = editor.IsSelection() || IsAlNum(editor.GetChar()) || editor.GetChar() == '_';

	auto OnlineSearch = [=](const String& url) {
		String h = url;
		h.Replace("%s", UrlEncode(Nvl(editor.GetSelection(), editor.GetWord())));
		LaunchWebBrowser(h);
	};
	
	auto Icon = [&](int i) {
		bool& b = search_icon_loaded.At(i, false);
		Image& m = search_icon.At(i);
		if(!b) {
			b = true;
			m = StreamRaster::LoadStringAny(Decode64(search_engines[i][IsUHDMode() ? "Icon32" : "Icon16"]));
		}
		return m;
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
		OnlineSearch("https://www.google.com/search?q=%s&sitesearch=ultimatepp.org");
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
