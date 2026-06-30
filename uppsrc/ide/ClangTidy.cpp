#include "ide.h"

struct ClangTidyDlg : WithClangTidy<TopWindow> {
	static String         path;
	static Index<String>  options;
	static Index<String>  groups;
	
	static bool HasClangTidy();

	struct OptionWithLink : Option {
		RichTextCtrl text;
	};
	
	ArrayMap<String, OptionWithLink> checks;
	
	void Group();

	ClangTidyDlg();
};

String ClangTidyDlg::path;
Index<String> ClangTidyDlg::options;
Index<String> ClangTidyDlg::groups;

ClangTidyDlg::ClangTidyDlg()
{
	CtrlLayoutOKCancel(*this, "Clang Tidy");
	
	group.AddColumn("Group");
	group.NoHeader();
	group.Add(AttrText("All").Italic().NormalInk(SLtBlue()));
	for(String s : groups)
		group.Add(s);

	group.WhenSel = [=] {
		Group();
	};

	option.NoHeader();
	option.AddKey();
	option.AddColumn();
	option.NoCursor();

	for(String s : options) {
		OptionWithLink& opt = checks.Add(s);
		opt.NoWantFocus();
		opt << opt.text.NoSb().VCenter().HSizePos(DPI(18), 0).VSizePos();
		String txt = "[g";
		if(s.TrimStart("clang-analyzer-")) // TODO: Improve
			txt << "^https://clang.llvm.org/extra/clang-tidy/checks/clang-analyzer/" + s + "^";
		else {
			int q = s.Find('-');
			if(q >= 0)
				txt << "^https://clang.llvm.org/extra/clang-tidy/checks/" << s.Mid(0, q) << "/" << s.Mid(q + 1) << ".html^";
		}
		txt << " \1" << s;
		opt.text <<= txt;
	}
}

void ClangTidyDlg::Group()
{
	String st;
	if(group.GetCursor() > 0)
		st = group.GetKey();

	option.Clear();
	for(auto m : ~checks)
		if(m.key.StartsWith(st)) {
			option.Add(m.key);
			option.SetCtrl(option.GetCount() - 1, 0, m.value, false);
		}
}

bool ClangTidyDlg::HasClangTidy()
{
	ONCELOCK {
#ifdef PLATFORM_WIN32
		for(String p : Split(GetMethodVars("CLANGx64").Get("PATH", ""), ';')) {
			p << "/clang-tidy.exe";
#else
			String p = "clang-tidy";
#endif
			String s = Sys(p + " -checks=* --list-checks");
			if(s.GetCount()) {
				path = p;
				for(String l : Split(s, '\n')) {
					l = TrimBoth(l);
					if(*l.Last() != ':') // Ignore "Enabled checks:"
						options.FindAdd(l);
				}
				
				for(String s : options) {
					int q = s.Find('.');
					if(q < 0)
						q = s.Find('-');
					if(q >= 0)
						s.Trim(q);
					groups.FindAdd(s);
				}
			}
#ifdef PLATFORM_WIN32
		}
#endif
	}
	return path.GetCount() && options.GetCount();
}


bool Ide::HasClangTidy()
{
	return ClangTidyDlg::HasClangTidy();
}

void Ide::ClangTidy()
{
	if(!HasClangTidy()) {
		Exclamation("No clang-tidy...");
		return;
	}
	
	ClangTidyDlg dlg;
	
	dlg.Execute();
}
