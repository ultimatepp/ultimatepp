#include "ide.h"

void Ide::SyncExternalDependencies(bool force, bool report_ok)
{
	String triplet = GetTargetTriplet();
	
	Vector<String> m = MissingExternalDependencies(triplet);
	
	if(m.GetCount() == 0) {
		if(report_ok)
			PromptOK("No missing external dependencies.");
		return;
	}
	
	Sort(m);
	String xd = Join(m, "\n");
	
	String xd_path = ConfigFile(triplet + ".missing");
	RealizePath(xd_path);
	
	bool changed = LoadFile(xd_path) != xd;
	if(changed)
		::SaveFile(xd_path, xd);

	if(!force && !changed)
		return;
	
	String txt = "There are missing external dependencies";
	String h = GetTargetTriplet();
	if(h.GetCount())
		txt << " for the target [* \1" << h << "\1]";
	
	String cmd = InstallMissingExternalDependenciesCommand(triplet);
	if(cmd.GetCount()) {
		WriteClipboardText(cmd);
		txt << " installable by command:&&[* \1" << cmd
		    << "\1]&[/ (the command was placed on clipboard)]";
	}
	else
		txt << ":&&[* \1" + Join(m, " ") <<
		       "\1]";
	
	if(!CanInstallMissingExternalDependencies()) {
		txt << "&&Please use the terminal to execute above command."
		    << "&[/ (this can be invoked again later from the Project menu)";
		PromptOK(txt);
		return;
	}
	
	txt << "&&Proceed?&[/ (this can be invoked again later from the Project menu)";
	
	if(PromptYesNo(txt)) {
		SysConsole console;
		console.Title("Installing missing external dependencies");
		if(InstallMissingExternalDependencies([&](const String& cmd, const String& chdir)
		                                       { return console.System(cmd, chdir); },
		                                       TheIde()->GetTargetTriplet()))
			console.Log("OK", SGreen());
		else
			console.Log("There were errors.", SRed());

		console.Perform();
	}
}

struct SelectExtDepDlg : WithSelectExtDepLayout<TopWindow> {
	SelectExtDepDlg();
	
	void Sync();
	
	Vector<String> src, usrc;
};

SelectExtDepDlg::SelectExtDepDlg()
{
	CtrlLayoutOKCancel(*this, "Select external dependency");

	list.AddColumn("Name");
	list.AddColumn("Description");
	list.ColumnWidths("176 551");
	
	search.NullText("Search (Ctrl+K)");
	search.SetFilter([](int c) { return (int)ToUpper(ToAscii(c)); });
	search << [this] { Sync(); };
	
	if(IsBREW()) {
		Progress pi("Updating database");
		for(Value p : GetBrewJson([&](int pos, int total) { return pi.SetCanceled(pos, total); }))
			src << ~p["name"] + " - " + ~p["desc"];
	}
	else
		src = Split(Sys(IsRPM() ? "dnf repoquery --queryformat '%{name} - %{summary}' '*'"
		                        : "apt-cache search ."),
		            '\n');

	for(String l : src) {
		l.Replace(" - ", "\n");
		usrc << ToUpper(l);
	}

	list.WhenLeftDouble = Acceptor(IDOK);
	
	Sync();
}

void SelectExtDepDlg::Sync()
{
	String s = ~search;
	Value key = list.GetKey();
	int sc = list.GetScroll();
	list.Clear();

	int ii = 0;
	for(String l : src) {
		if(usrc[ii++].Find(s) >= 0) {
			String desc;
			int q = l.Find(" - ");
			if(q >= 0) {
				desc = l.Mid(q + 3);
				l.Trim(q);
			}
			list.Add(l, desc);
		}
	}

	list.ScrollTo(sc);
	if(!IsNull(key))
		list.FindSetCursor(key);
}

String SelectExternalDependency()
{
	SelectExtDepDlg dlg;
	if(dlg.ExecuteOK())
		return ~dlg.list.GetKey();
	return Null;
}
