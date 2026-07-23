#include "Builders.h"

#define LDUMP(x) // DDUMP(x)

String        ClangTidy::path;
Index<String> ClangTidy::options;
Index<String> ClangTidy::groups;
Index<String> ClangTidy::active_checks;

bool ClangTidy::HasClangTidy()
{
	if(!TheIdeContext())
		return false; // so that GetMethodVars returns something real...

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
				Load(ClangTidyConfigPath());
#ifdef PLATFORM_WIN32
				break;
#endif
			}
#ifdef PLATFORM_WIN32
		}
#endif
	}
exit:
	return path.GetCount() && options.GetCount();
}

String ClangTidy::ClangTidyConfigPath() {
	return ConfigFile("ide-clang-tidy.json");
}

void ClangTidy::Load(const char *path)
{
	if(FileExists(path)) {
		Value json = ParseJSON(LoadFile(path));
		active_checks.Clear();
		for(Value v : json["active_checks"])
			active_checks << ~v;
	}
	else
	for(const String& s : options) {
		if(s.StartsWith("clang-analyzer-core."))
			active_checks << s;
	}
}

void ClangTidy::Save(const char *path)
{
	Value json;
	ValueArray va;
	for(String s : active_checks)
		va << s;
	json("active_checks") = va;
	SaveChangedFile(path, AsJSON(json, true));
}

void ClangTidy::RunClangTidy(const Array<CompileCommand>& commands)
{
	String cmdline;

	String win32_cmdline; // fix mingw-clang issue
#ifdef PLATFORM_WIN32
// "C:/upp/bin/clang/bin/clang-tidy.exe"
// "C:/upp/bin/clang/include/c++/v1/typeinfo"
	win32_cmdline = " -I" + GetPathQ(GetFileFolder(GetFileFolder(path)) + "/include/c++/v1");
#endif

	JsonArray ccj;
	String outdir;
	for(const auto& m : commands) {
		if(IsNull(outdir))
			outdir = GetFileFolder(m.ofile);
		ccj << Upp::Json("directory", GetFileFolder(m.file))
		                ("command", m.command + win32_cmdline)
		                ("file", GetFileName(m.file));
		cmdline << ' ' << GetPathQ(m.file);
		LDUMP(cmdline);
	}

	String cc_path = outdir + "/compile_commands.json";
	RealizePath(cc_path);
	Upp::SaveFile(cc_path, ccj.ToString());

	cmdline << " -checks=" << Join(active_checks.GetKeys(), ",")
	        << " -p " << cc_path;


	String rf_path = outdir + "/clang_tidy_parameters_file";
	Upp::SaveFile(rf_path, cmdline);

	Host host;
	MakeBuild *mb = dynamic_cast<MakeBuild *>(TheIdeContext());
	if(mb) {
		mb->CreateHost(host, false, false);
		host.Execute(path + " @" + rf_path);
	}
}
