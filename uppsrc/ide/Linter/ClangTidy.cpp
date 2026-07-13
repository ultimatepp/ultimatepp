#include "ClangTidy.h"

static String sExeFilePath;

String ClangTidy::GetConfigFilePath() const
{
	return ConfigFile(AppendFileName("clangtidy", IdeGetCurrentMainPackage() + "-clangtidy.json"));
}

Value ClangTidy::LoadConfig()
{
	String path = GetConfigFilePath();
	if(!FileExists(path))
		return Null;
	return ParseJSON(LoadFile(path));
}

void ClangTidy::SaveConfig(const Value& cfg)
{
	String path = GetConfigFilePath();
	RealizePath(path);
	SaveChangedFile(path, cfg);
}

bool ClangTidy::Exists() const
{
	static bool b = false;
	ONCELOCK
	{
#ifdef flagWIN32
		// FIXME: Check path.
		constexpr const char *exe = "C:\\Program Files\\LLVM\\bin\\clang-tidy.exe";
		b = FileExists(exe);
		if(b) sExeFilePath << "\"" << exe << "\"";
#else
		b = Sys("which clang-tidy", sExeFilePath) == 0;
		if(b) sExeFilePath = TrimRight(sExeFilePath);
		else  sExeFilePath.Clear();
#endif
	}
	return b && TheIde();
}

void ClangTidy::Settings()
{
	ClangTidyConfigDlg dlg(*this);
	dlg.Load();
	if(dlg.ExecuteOK())
		dlg.Save();
}

Vector<String> ClangTidy::ResolveProject(const String& ccjpath)
{
	Vector<String> files;
	Value ccj = ParseJSON(LoadFile(ccjpath));
	for(int i = 0; i < ccj.GetCount(); i++)
		files.Add(ccj[i]["file"]);
	return files;
}

Vector<String> ClangTidy::ResolvePackage(const String& ccjpath, const Vector<String>& paths)
{
	Vector<String> files;
	Value ccj = ParseJSON(LoadFile(ccjpath));
	for(int i = 0; i < ccj.GetCount(); i++) {
		String file = ccj[i]["file"];
		// Length-check prevents prefix collisions (e.g. /upp/Core vs /upp/Core2)
		for(const String& p : paths) {
			int pl = p.GetLength();
			if(file.StartsWith(p) && (file.GetLength() == pl || file[pl] == '/' || file[pl] == '\\')) {
				files.Add(file);
				break;
			}
		}
	}
	return files;
}

Vector<String> ClangTidy::ResolveFiles(Scope sc, const String& ccjpath, const Vector<String>& paths)
{
	bool hasccj = FileExists(ccjpath);
	switch(sc) {
	case Scope::File:
		return clone(paths);
	case Scope::Project:
		return hasccj ? ResolveProject(ccjpath)        : clone(paths);
	case Scope::Package:
		return hasccj ? ResolvePackage(ccjpath, paths) : clone(paths);
	}
	return clone(paths);
}

String ClangTidy::MakeCmdLine(Scope sc, Vector<String>& paths)
{
	Value v = LoadConfig()["ClangTidy"];

	// Is there a better way to obtain compile_commands.json file?
	String ccjdir  = GetTempPath();
	String ccjpath = AppendFileName(ccjdir, "compile_commands.json");

	if(MakeBuild *mb = dynamic_cast<MakeBuild *>(TheIdeContext()))
		mb->SaveCCJ(ccjpath, false);

	String path;
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
}

void ClangTidy::OnResults(const String& results)
{
	// FIXME: This is fragile, but not all versions of clang have JSON output...
	RegExp r("([A-Za-z]?:?[/\\\\][^:]+\\.[ch]pp):(\\d+):(\\d+): (\\w+): (.+?)(\\s+\\[[\\w,.-]+\\])?$");

	for(const String& line : Split(results, '\n', false)) {
		if(!r.Match(line))
			continue;

		Vector<String> v = r.GetStrings();
		if(v.GetCount() < 5)
			continue;

		String severity = v.At(3);
		if(severity == "note")
			continue;

		Ide::ListLineInfo e;
		e.file    = v.At(0);
		e.lineno  = StrInt(v.At(1));
		e.linepos = StrInt(v.At(2));
		e.kind    = severity == "error" ? 0 : 1;
		e.message = v.At(4) + TrimLeft(v.At(5));

		Image img = decode(
			severity,
			"error",   LinterImg::error(),
			"warning", LinterImg::warning(),
			LinterImg::warning()
		);
		Color paper = HighlightSetup::GetHlStyle(e.kind == 0 ? HighlightSetup::PAPER_ERROR
		                                                      : HighlightSetup::PAPER_WARNING).color;
		int linecy;
		AttrText txt(TheIde()->FormatErrorLine(e.message, linecy));
		txt.NormalPaper(paper);
		txt.SetImage(img);

		ArrayCtrl& error = TheIde()->error;
		error.Add(e.file, e.lineno, txt, RawToValue(e));
		error.SetLineCy(error.GetCount() - 1, linecy);
	}
}

INITIALIZER(ClangTidy)
{
	RegisterLinterModule(Single<ClangTidy>("ClangTidy"));
}