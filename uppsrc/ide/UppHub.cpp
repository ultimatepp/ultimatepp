#include "ide.h"

// Todo: other folders in UppHub

struct UppHubNest : Moveable<UppHubNest> {
	int              tier = -1;
	String           name;
	Vector<String>   packages;
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

	void Readme();
	void Load(int tier, const String& url, bool deep);
	void Load();
	void Install(bool noprompt = false);
	void Uninstall(bool noprompt = false);
	void Reinstall();
	void Install(const Index<int>& ii);
	void SyncList();

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
}

void UppHubDlg::Readme()
{
	String link = list.Get("README");
	String s = HttpRequest(link).RequestTimeout(3000).Execute();
	if(s.GetCount()) {
		if(link.EndsWith(".qtf"))
			PromptOK(s);
		else
			PromptOK("\1" + s);
	}
}

void UppHubDlg::Load(int tier, const String& url, bool deep)
{
	if(loaded.Find(url) >= 0)
		return;
	loaded.Add(url);
	
	
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
			return;
	
		s = r.GetContent();
	}
	
	int begin = s.FindAfter("UPPHUB_BEGIN");
	int end = s.Find("UPPHUB_END");
	
	if(begin >= 0 && end >= 0)
		s = s.Mid(begin, end - begin);

	Value v = ParseJSON(s);
	if(v.IsError()) {
		s.Replace("&quot;", "\"");
		s.Replace("&amp;", "&");
		v = ParseJSON(s);
	}

	try {
		String list_name = v["name"];
		for(Value ns : v["nests"]) {
			String name = ns["name"];
			UppHubNest& n = upv.GetAdd(ns["name"]);
			n.name = name;
			bool tt = tier > n.tier;
			if(tt || n.packages.GetCount() == 0)
				for(Value p : ns["packages"])
					n.packages.Add(p);
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
		if(deep)
			for(Value l : v["links"]) {
				if(loading_stopped)
					break;
				Load(tier + 1, l, true);
			}
		for(Value l : v["refs"]) {
			if(loading_stopped)
				break;
			Load(tier + 1, l, false);
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
	            "https://raw.githubusercontent.com/ultimatepp/ultimatepp/master/upphub.root"),
	     true);

	SyncList();

	pi.Close();
}

void UppHubDlg::Install(const Index<int>& ii)
{
	UrepoConsole console;
	if(ii.GetCount()) {
		for(int i : ii) {
			String n = list.Get(i, 0);
			if(n.GetCount()) {
				String cmd = "git clone ";
				String repo = list.Get(i, "REPO");
				String repo2, branch;
				if(SplitTo(repo, ' ', repo2, branch))
					cmd << "-b " + branch + " " + repo2;
				else
					cmd << repo;
				cmd << ' ' << GetHubDir() << '/' << n;
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
		Index<int> h;
		h << list.GetCursor();
		Install(h);
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

void UppHub()
{
	UppHubDlg dlg;
	dlg.Load();
	dlg.Run();
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
		Index<int> found;
		for(const UppHubNest& n : dlg.upv)
			for(const String& p : n.packages)
				if(missing.Find(p) >= 0) {
					int i = dlg.list.Find(n.name);
					if(i >= 0)
						found.FindAdd(i);
				}

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
