#include "ide.h"

struct ClangTidyDlg : WithClangTidyLayout<TopWindow>, ClangTidy {
	struct OptionWithLink : Option {
		RichTextCtrl text;
	};
	
	ArrayMap<String, OptionWithLink> checks;

	void SyncGroupCounts();
	void Group();
	void SetOptions();
	void ReadOptions();

	ClangTidyDlg();
};

ClangTidyDlg::ClangTidyDlg()
{
	CtrlLayoutOKCancel(*this, "Clang-Tidy");
	
	group.AddIndex();
	group.AddColumn("Group");
	group.NoHeader();
	group.Add(Null, AttrText("All").Italic().NormalInk(SLtBlue()));
	for(String s : groups)
		group.Add(s, s);

	group.WhenSel = [=] {
		Group();
	};

	option.NoHeader();
	option.AddKey();
	option.AddColumn();
	option.NoCursor();
	
	SetOptions();
	
	auto Set = [=](bool b) {
		for(int i = 0; i < option.GetCount(); i++) {
			auto *o = dynamic_cast<Option *>(option.GetCtrl(i, 0));
			if(o)
				*o <<= b;
		}
		SyncGroupCounts();
	};
	
	set << [=] { Set(true); };
	reset << [=] { Set(false); };
	
	save << [=] {
		ReadOptions();
		String p = SelectFileSaveAs("*.json\n*.*");
		if(!IsNull(p))
			Save(p);
	};

	load << [=] {
		String p = SelectFileOpen("*.json\n*.*");
		Load(p);
		SetOptions();
		Group();
	};
}

void ClangTidyDlg::Group()
{
	String g = ~group.GetKey();
	option.Clear();
	for(auto m : ~checks) {
		if(m.key.StartsWith(g)) {
			option.Add(m.key);
			option.SetCtrl(option.GetCount() - 1, 0, m.value, false);
		}
	}
}

void ClangTidyDlg::SyncGroupCounts()
{
	for(int i = 1; i < group.GetCount(); i++) {
		String g = ~group.Get(i, 0);
		int count = 0;
		for(auto m : ~checks)
			if(m.key.StartsWith(g) && ~m.value)
				count++;
		if(count) {
			g << " (" << count << ")";
			group.Set(i, 1, AttrText(g).Bold());
		}
		else
			group.Set(i, 1, g);
	}
}


void ClangTidyDlg::SetOptions()
{
	checks.Clear();
	for(String s : options) {
		OptionWithLink& opt = checks.Add(s);
		opt.NoWantFocus();
		opt << opt.text.NoSb().VCenter().HSizePos(DPI(18), 0).VSizePos();
		opt << [=] { SyncGroupCounts(); };
		String txt = "[g";
		String cs;
		if(cs.TrimStart("clang-analyzer-")) // TODO: Improve
			txt << "^https://clang.llvm.org/extra/clang-tidy/checks/clang-analyzer/" + cs + "^";
		else {
			int q = s.Find('-');
			if(q >= 0)
				txt << "^https://clang.llvm.org/extra/clang-tidy/checks/" << s.Mid(0, q) << "/" << s.Mid(q + 1) << ".html^";
		}
		txt << " \1" << s;
		opt.text <<= txt;
		opt <<= active_checks.Find(s) >= 0;
	}
	SyncGroupCounts();
}

void ClangTidyDlg::ReadOptions()
{
	active_checks.Clear();
	for(auto m : ~checks)
		if(m.value)
			active_checks.FindAdd(m.key);
}

bool Ide::HasClangTidy()
{
	return ClangTidyDlg::HasClangTidy();
}

void Ide::ClangTidy(Gate<const String&> what)
{
	if(!HasClangTidy()) {
		Exclamation("No clang-tidy!");
		return;
	}

	MakeBuild *mb = dynamic_cast<MakeBuild *>(TheIdeContext()); // TODO: Move to Builders/umk

	if(!mb)
		return;
	
	Array<CompileCommand> cs = mb->GetCompileCommands();
	
	cs.RemoveIf([&](int i) {
		return !what(cs[i].file);
	});
	
	if(cs.GetCount() == 0) {
		Exclamation("No files to check!");
		return;
	}

	ClangTidyDlg dlg;
	
	bool b = dlg.ExecuteOK();
	
	dlg.ReadOptions();
	dlg.Save(dlg.ClangTidyConfigPath());

	if(!b)
		return;

	BeginBuilding(true);
	
	dlg.RunClangTidy(cs);

	EndBuilding(true);
	SetErrorEditor();

/*
	String files;
	for(const String& f : ResolveFiles(sc, ccjpath, paths))
		path << "\"" << f << "\" ";

	if(IsNull(v))
		return sExeFilePath + " --checks=* " + path;

	String checks    = Nvl(v["checks"],     "*");
	String extraargs = Nvl(v["extra_args"], "");
	String standard  = Nvl(v["standard"],   "c++14");

	String s;
	s << sExeFilePath << " "
	<< "--checks=\"" << checks << "\" "
	<< "--quiet ";

	if(FileExists(ccjpath))
		s << "-p=\"" << ccjdir << "\" ";

	return s + extraargs + (extraargs.GetCount() ? " " : "") + path
			+ (FileExists(ccjpath) ? "" : "-- -std=" + standard);
*/
}
