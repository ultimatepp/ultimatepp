#include "ide.h"

struct ClangTidyDlg : WithClangTidyLayout<TopWindow> {
	static String         path;
	static Index<String>  options;
	static Index<String>  groups;
	static Index<String>  active_checks;
	
	static bool   HasClangTidy();
	static String ClangTidyConfigPath();
	static void Load(const char *path);
	static void Save(const char *path);

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

String        ClangTidyDlg::path;
Index<String> ClangTidyDlg::options;
Index<String> ClangTidyDlg::groups;
Index<String> ClangTidyDlg::active_checks;

ClangTidyDlg::ClangTidyDlg()
{
	CtrlLayoutOKCancel(*this, "Clang Tidy");
	
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
			DDUMP(s);
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
				Load(ClangTidyConfigPath());
				goto exit; // break not compatible with POSIX
			}

#ifdef PLATFORM_WIN32
		}
#endif
	}
exit:
	return path.GetCount() && options.GetCount();
}

String ClangTidyDlg::ClangTidyConfigPath() {
	return ConfigFile("ide-clang-tidy.json");
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


void ClangTidyDlg::Load(const char *path)
{
	Value json = ParseJSON(LoadFile(path));
	active_checks.Clear();
	for(Value v : json["active_checks"])
		active_checks << ~v;
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

void ClangTidyDlg::Save(const char *path)
{
	Value json;
	ValueArray va;
	for(String s : active_checks)
		va << s;
	json("active_checks") = va;
	SaveFile(path, AsJSON(json, true));
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
	
	dlg.ReadOptions();
	dlg.Save(dlg.ClangTidyConfigPath());
	
	MakeBuild *mb = dynamic_cast<MakeBuild *>(TheIdeContext()); // TODO: Move to Builders/umk
	
	if(!mb)
		return;
	
	Array<CompileCommand> commands = mb->GetCompileCommands();

	String cmdline;
	
	JsonArray ccj;
	String outdir;
	for(const auto& m : commands) {
		if(IsNull(outdir))
			outdir = GetFileFolder(m.ofile);
		ccj << Upp::Json("directory", GetFileFolder(m.file))("command", m.command)("file", GetFileName(m.file));
		cmdline << ' ' << GetPathQ(m.file);
	}
	
	String cc_path = outdir + "/compile_commands.json";
	RealizePath(cc_path);
	Upp::SaveFile(cc_path, ccj.ToString());
	
	cmdline << " -checks=" << Join(ClangTidyDlg::active_checks.GetKeys(), ",")
	        << " -p " << cc_path;

	
	String rf_path = outdir + "/clang_tidy_parameters_file";
	Upp::SaveFile(rf_path, cmdline);

	BeginBuilding(true);
	Host host;
	CreateHost(host, darkmode, disable_uhd);
	host.Execute(ClangTidyDlg::path + " @" + rf_path);
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
