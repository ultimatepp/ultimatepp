#include "ide.h"

struct UppHubNest : Moveable<UppHubNest> {
	int              tier = -1;
	String           name;
	Vector<String>   packages;
	Vector<String>   uses;
	String           description;
	String           repo;
	String           status = "unknown";
	String           category;
	String           list_name;
	String           readme;
};

struct UppHubDlg : WithUppHubLayout<TopWindow> {
	VectorMap<String, UppHubNest> upv;
	Index<String> loaded;
	Progress pi;
	bool loading_stopped;
	String last_package;

	void  Readme();
	Value LoadJson(const String& url);
	void  Load(int tier, const String& url);
	void  Load();
	void  Install(bool noprompt = false);
	void  Uninstall(bool noprompt = false);
	void  Reinstall();
	void  Install(const Index<String>& ii);
	void  SyncList();
	
	UppHubNest *Get(const String& name) { return upv.FindPtr(name); }
	UppHubNest *Current()               { return Get(list.GetKey()); }

	UppHubDlg();
};

UppHubDlg::UppHubDlg()
{
	CtrlLayoutCancel(*this, "UppHub");
	
	list.EvenRowColor();

	list.AddColumn("Name");
	list.AddColumn("Category");
	list.AddColumn("Description");
	list.AddColumn("Packages");
	list.AddColumn("Status");
	list.AddColumn("INSTALLED", "Installed");
	
	list.AddIndex("REPO");
	list.AddIndex("README");
	list.AddIndex("USES");
	
	list.ColumnWidths("94 72 373 251 119 53");
	list.WhenSel = [=] {
		action.Disable();
		readme.Disable();
		reinstall.Disable();
		if(list.IsCursor()) {
			readme.Enable();
			action.Enable();
			if(IsNull(list.Get("INSTALLED"))) {
				action.SetLabel("Install");
				action ^= [=] { Install(); };
			}
			else {
				action.SetLabel("Uninstall");
				reinstall.Enable();
				action ^= [=] { Uninstall(); };
			}
		}
		readme.Enable(list.IsCursor() && !IsNull(list.Get("README")));
		UppHubNest *n = Current();
		last_package = n && n->packages.GetCount() ? n->packages[0] : String();
	};
	list.WhenLeftDouble = [=] { Readme(); };
	readme << [=] { Readme(); };
	reinstall << [=] { Reinstall(); };
	
	settings.Show(IsVerbose());
	settings << [=] {
		String s = LoadFile(ConfigFile("upphub_root"));
		EditText(s, "UppHub root", "Root url");
		SaveFile(ConfigFile("upphub_root"), s);
		Load();
	};
	
	help << [=] { LaunchWebBrowser("https://www.ultimatepp.org/app$ide$UppHub_en-us.html"); };
}

void UppHubDlg::Readme()
{
	UppHubNest *n = Current();
	if(!n) return;
	String s = HttpRequest(n->readme).RequestTimeout(3000).Execute();
	if(s.GetCount()) {
		if(n->readme.EndsWith(".qtf"))
			PromptOK(s);
		else
			PromptOK("\1" + s);
	}
}

Value UppHubDlg::LoadJson(const String& url)
{
	String s = LoadFile(url);
	
	if(IsNull(s)) {
		pi.SetText(url);

		HttpRequest r(url);
		
		r.WhenWait = r.WhenDo = [&] {
			if(pi.StepCanceled()) {
				r.Abort();
				loading_stopped = true;
			}
		};
		
		r.Execute();
		
		if(loading_stopped)
			return ErrorValue();
	
		s = r.GetContent();
	}
	
	int begin = s.FindAfter("UPPHUB_BEGIN");
	int end = s.Find("UPPHUB_END");
	
	if(begin >= 0 && end >= 0)
		s = s.Mid(begin, end - begin);
	
	DDUMP(s);

	Value v = ParseJSON(s);
	if(v.IsError()) {
		s.Replace("&quot;", "\"");
		s.Replace("&amp;", "&");
		DDUMP(s);
		v = ParseJSON(s);
		DDUMP(v);
	}
	return v;
}

void UppHubDlg::Load(int tier, const String& url)
{
	if(loaded.Find(url) >= 0)
		return;
	loaded.Add(url);
	
	Value v = LoadJson(url);
	
	DDUMP(v);

	try {
		String list_name = v["name"];
		for(Value ns : v["nests"]) {
			String url = ns["url"];
			if(url.GetCount())
				ns = LoadJson(url);
			String name = ns["name"];
			UppHubNest& n = upv.GetAdd(name);
			n.name = name;
			bool tt = tier > n.tier;
			if(tt || n.packages.GetCount() == 0)
				for(Value p : ns["packages"])
					n.packages.Add(p);
			if(tt || n.uses.GetCount() == 0)
				for(Value p : ns["uses"])
					n.uses.Add(p);
			auto Attr = [&](String& a, const char *id) {
				if(tt || IsNull(a))
					a = ns[id];
			};
			Attr(n.description, "description");
			Attr(n.repo, "repository");
			Attr(n.category, "category");
			Attr(n.status, "status");
			Attr(n.readme, "readme");
			n.list_name = list_name;
		}
		for(Value l : v["links"]) {
			if(loading_stopped)
				break;
			Load(tier + 1, l);
		}
	}
	catch(ValueTypeError) {}
}

void UppHubDlg::SyncList()
{
	int sc = list.GetScroll();
	Value k = list.GetKey();
	list.Clear();
	for(const UppHubNest& n : upv)
		list.Add(n.name, n.category, n.description, Join(n.packages, " "), n.status,
		         DirectoryExists(GetHubDir() + "/" + n.name) ? "Yes" : "",
		         n.repo, n.readme);
	list.ScrollTo(sc);
	if(!list.FindSetCursor(k))
		list.GoBegin();
}

void UppHubDlg::Load()
{
	loading_stopped = false;
	loaded.Clear();
	upv.Clear();

	Load(0, Nvl(LoadFile(ConfigFile("upphub_root")),
	            "https://raw.githubusercontent.com/ultimatepp/ultimatepp/master/upphub.root"));

	SyncList();

	pi.Close();
}

void UppHubDlg::Install(const Index<String>& ii_)
{
	Index<String> ii = clone(ii_);
	for(int i = 0; i < ii.GetCount(); i++)
		if(UppHubNest *n = Get(ii[i]))
			for(String s : n->uses)
				ii.FindAdd(s);

	UrepoConsole console;
	if(ii.GetCount()) {
		for(String ns : ii) {
			UppHubNest *n = Get(ns);
			if(n) {
				String cmd = "git clone ";
				String repo2, branch;
				if(SplitTo(n->repo, ' ', repo2, branch))
					cmd << "-b " + branch + " " + repo2;
				else
					cmd << n->repo;
				cmd << ' ' << GetHubDir() << '/' << n->name;
				console.System(cmd);
			}
		}
		console.Log("Done", Gray());
		console.Perform();
		InvalidatePackageCache();
	}
}

void UppHubDlg::Install(bool noprompt)
{
	if(list.IsCursor() && (noprompt || PromptYesNo("Install " + ~list.GetKey() + "?"))) {
		Install(Index<String>{ ~list.GetKey() });
		SyncList();
	}
}

void UppHubDlg::Uninstall(bool noprompt)
{
	if(list.IsCursor() && (noprompt || PromptYesNo("Uninstall " + ~list.GetKey() + "?"))) {
		if(!DeleteFolderDeep(GetHubDir() + "/" + ~list.GetKey(), true))
			Exclamation("Failed to delete " + ~list.GetKey());
		SyncList();
	}
}

void UppHubDlg::Reinstall()
{
	if(list.IsCursor() && PromptYesNo("Reinstall " + ~list.GetKey() + "?")) {
		Uninstall(true);
		Install(true);
	}
}

String UppHub()
{
	UppHubDlg dlg;
	dlg.Load();
	dlg.Run();
	return dlg.last_package;
}

void UppHubAuto(const String& main)
{
	bool noprompt = false;
	Index<String> pmissing;
	for(;;) {
		Workspace wspc;
		wspc.Scan(main);
		Index<String> missing;
		for(int i = 0; i < wspc.GetCount(); i++) {
			String p = wspc[i];
			if(!FileExists(PackagePath(p)))
				missing.FindAdd(p);
		}

		if(missing.GetCount() == 0)
			break;

		UppHubDlg dlg;
		dlg.Load();
		Index<String> found;
		for(const UppHubNest& n : dlg.upv)
			for(const String& p : n.packages)
				if(missing.Find(p) >= 0)
					found.FindAdd(n.name);

		if(found.GetCount() == missing.GetCount() && missing != pmissing &&
		   (noprompt || PromptYesNo("Missing packages were found in UppHub. Install?"))) {
			dlg.Install(found);
			noprompt = true;
			pmissing = clone(missing);
			continue;
		}

		PromptOK("Some packages are missing:&&[* \1" + Join(missing.GetKeys(), "\n"));
		break;
	}
}
