#include "ide.h"

struct UpvNest : Moveable<UpvNest> {
	String         name;
	Vector<String> packages;
	String         description;
	String         repo;
};

struct UpvDlg : WithUpvLayout<TopWindow> {
	Vector<UpvNest> upv;
	Index<String> loaded;
	Progress pi;
	bool loading_stopped;

	void Load(const String& url);
	void Load();
	void Install();

	UpvDlg();
};

UpvDlg::UpvDlg()
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

void UpvDlg::Load(const String& url)
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
	
	int begin = s.FindAfter("UPPIVERSE_BEGIN");
	int end = s.Find("UPPIVERSE_END");
	
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
				UpvNest& n = upv.Add();
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

void UpvDlg::Load()
{
	loading_stopped = false;
	loaded.Clear();
	upv.Clear();

	Load("https://www.ultimatepp.org/forums/index.php?t=msg&goto=55566&#msg_55566");
	
	list.Clear();
	for(const UpvNest& n : upv)
		list.Add(false, n.name, n.description, Join(n.packages, " "), n.repo);
}

void UpvDlg::Install()
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
}

void Uppiverse()
{
	UpvDlg dlg;
	dlg.Load();
	if(dlg.Run() == IDOK) {
		dlg.Install();
	}
}
