#include "ide.h"

struct UppHubNest : Moveable<UppHubNest> {
	String         name;
	Vector<String> packages;
	String         description;
	String         repo;
};

struct UppHubDlg : WithUpvLayout<TopWindow> {
	Vector<UppHubNest> upv;
	Index<String> loaded;
	Progress pi;
	bool loading_stopped;

	void Load(const String& url);
	void Load();
	void Install();

	UppHubDlg();
};

UppHubDlg::UppHubDlg()
{
	CtrlLayoutOKCancel(*this, "Uppiverse");
	
	list.EvenRowColor();

	void (*factory)(int, One<Ctrl>&) = [](int, One<Ctrl>& o) { o.Create<Option>().NoWantFocus(); };

	list.AddColumn("Install").Ctrls(factory);
	list.AddColumn("Name");
	list.AddColumn("Description");
	list.AddColumn("Packages");
	list.AddIndex("REPO");
	list.ColumnWidths("49 106 136 671");
}

void UppHubDlg::Load(const String& url)
{
	if(loaded.Find(url) >= 0)
		return;
	loaded.Add(url);
	
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

	String s = r.GetContent();
	
	int begin = s.FindAfter("UPPHUB_BEGIN");
	int end = s.Find("UPPHUB_END");
	
	if(begin >= 0 && end >= 0) {
		s = s.Mid(begin, end - begin);
		Value v = ParseJSON(s);
		if(v.IsError()) {
			s.Replace("&quot;", "\"");
			s.Replace("&amp;", "&");
			v = ParseJSON(s);
		}
		try {
			for(Value ns : v["nests"]) {
				UppHubNest& n = upv.Add();
				n.name = ns["name"];
				for(Value p : ns["packages"])
					n.packages.Add(p);
				n.description = ns["description"];
				n.repo = ns["repository"];
			}
			for(Value l : v["links"]) {
				if(loading_stopped)
					break;
				Load(l);
			}
		}
		catch(ValueTypeError) {}
	}
}

void UppHubDlg::Load()
{
	loading_stopped = false;
	loaded.Clear();
	upv.Clear();

	Load("https://raw.githubusercontent.com/ultimatepp/ultimatepp/master/upphub.root");
	
	list.Clear();
	for(const UppHubNest& n : upv)
		list.Add(false, n.name, n.description, Join(n.packages, " "), n.repo);
}

void UppHubDlg::Install()
{
	UrepoConsole console;
	for(int i = 0; i < list.GetCount(); i++) {
		if((bool)list.Get(i, 0)) {
			String cmd = "git clone ";
			String repo = list.Get(i, "REPO");
			String repo2, branch;
			if(SplitTo(repo, ' ', repo2, branch))
				cmd << "-b " + branch + " " + repo2;
			else
				cmd << repo;
			cmd << ' ' << GetHubDir() << '/' <<  list.Get(i, 1);
			if(console.System(cmd))
				break;
		}
	}
	console.Perform();
	InvalidatePackageCache();
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
	int pmissing = -1;
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
		int found = 0;
		for(const UppHubNest& n : dlg.upv)
			for(const String& p : n.packages)
				if(missing.Find(p) >= 0) {
					found++;
					int i = dlg.list.Find(n.name, 1);
					if(i >= 0)
						dlg.list.Set(i, 0, true);
				}

		if(found == missing.GetCount() && missing.GetCount() != pmissing &&
		   (noprompt || PromptYesNo("Missing packages were found in UppHub. Install?") && dlg.Run() == IDOK)) {
			dlg.Install();
			noprompt = true;
			pmissing = missing.GetCount();
			continue;
		}

		PromptOK("Some packages are missing:&&[* \1" + Join(missing.GetKeys(), "\n"));
		break;
	}
}
