#include "ide.h"

struct UppHubNest : Moveable<UppHubNest> {
	int              tier = -1;
	String           name;
	Vector<String>   packages;
	String           description;
	String           repo;
	String           status = "Unknown";
};

struct UppHubDlg : WithUppHubLayout<TopWindow> {
	VectorMap<String, UppHubNest> upv;
	Index<String> loaded;
	Progress pi;
	bool loading_stopped;

	void Load(int tier, const String& url, bool deep);
	void Load();
	void Install();
	void Install(const Index<int>& ii);

	UppHubDlg();
};

UppHubDlg::UppHubDlg()
{
	CtrlLayoutOKCancel(*this, "UppHub");
	
	list.EvenRowColor();

	list.AddColumn("Name");
	list.AddColumn("Description");
	list.AddColumn("Packages");
	list.AddColumn("Status");
	
	list.AddIndex("REPO");
	
	settings.Show(IsVerbose());
	settings << [=] {
		String s = LoadFile(ConfigFile("upphub_root"));
		EditText(s, "UppHub root", "Root url");
		SaveFile(ConfigFile("upphub_root"), s);
		Load();
	};
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
		for(Value ns : v["nests"]) {
			String name = ns["name"];
			UppHubNest& n = upv.GetAdd(ns["name"]);
			n.name = name;
			bool tt = tier > n.tier;
			if(tt || n.packages.GetCount() == 0)
				for(Value p : ns["packages"])
					n.packages.Add(p);
			if(tt || n.description.GetCount() == 0)
				n.description = ns["description"];
			if(tt || n.repo.GetCount() == 0)
				n.repo = ns["repository"];
			String status = ns["status"];
			if(findarg(status, "stable", "testing", "experimental", "unknown", "broken") >= 0 &&
			   (tt || n.status.GetCount() == 0))
				n.status = status;
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

void UppHubDlg::Load()
{
	loading_stopped = false;
	loaded.Clear();
	upv.Clear();

	Load(0, Nvl(LoadFile(ConfigFile("upphub_root")),
	            "https://raw.githubusercontent.com/ultimatepp/ultimatepp/master/upphub.root"),
	     true);
	
	list.Clear();
	for(const UppHubNest& n : upv)
		list.Add(n.name, n.description, Join(n.packages, " "), n.status, n.repo);
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

void UppHubDlg::Install()
{
	if(list.IsCursor() && PromptYesNo("Install " + ~list.GetKey() + "?")) {
		Index<int> h;
		h << list.GetCursor();
		Install(h);
	}
}

void UppHub()
{
	UppHubDlg dlg;
	dlg.Load();
	if(dlg.Run() == IDOK)
		dlg.Install();
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
