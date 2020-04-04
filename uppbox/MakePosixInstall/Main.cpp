#include <Core/Core.h>
#include <plugin/bz2/bz2.h>

using namespace Upp;

void Error(const char *e)
{
	RLOG(e);
	abort();
}

String Syx(const char *s)
{
	RLOG(s);
	String r;
	int q = Sys(s, r);
	if(q)
		Error(String().Cat() << "Failed: " << s << "\r\n" <<
		      "Exit code: " << q << "\r\n" <<
		      "Output: " << r);
	return r;
}

void CopyFolder(const char *src, const char *dst, bool deep = true)
{
	if(GetFileName(src) == ".svn")
		return;
	Cout() << Sprintf("Directory %s\n", src);
	RealizeDirectory(dst);
	FindFile ff(String(src) + "/*.*");
	while(ff) {
		String s = AppendFileName(src, ff.GetName());
		String d = AppendFileName(dst, ff.GetName());
		if(ff.IsFile()) {
			String q = LoadFile(s);
			String ext = ToLower(GetFileExt(s));
			if(ext != ".aux" && ext != ".$old")
				SaveFile(d, q);
		}
		else
		if(ff.IsFolder() && *ff.GetName() != '.') {
			if(deep || *GetFileExt(s))
				CopyFolder(s, d, deep);
		}
		ff.Next();
	}
}

int CrLfSm(int c)
{
	return c == ';' || c == '\r' || c == '\n';
}

void CopyFolders(const char *src, const char *dst, const char *folders, bool deep = true)
{
	Vector<String> folder = Split(LoadFile(folders), CrLfSm);
	for(int i = 0; i < folder.GetCount(); i++)
		CopyFolder(AppendFileName(src, folder[i]), AppendFileName(dst, folder[i]), deep);
}

String release_dir = GetHomeDirFile("release");
String release = release_dir + "/upp";
String upp_src = GetHomeDirFile("upp.src");

int NoDigit(int c) { return IsDigit(c) ? 0 : c; }
int FilterVersion(int c) { return c == ':' ? '_' : c; }

void Make(String pkg, String exe, String method = "MINGW")
{
//	Syx("c:/upp/umk " + ass + " " + pkg + " c:/upp/" + method + ".bm -arv " + upptmp + "/" + exe);
//	FileDelete(upptmp + "/" + ForceExt(exe, ".map"));
}

extern const char *install_script;
extern const char *clean_script;

void SaveScript(const char *name, const char *text)
{
	String t = release + "/" + name;
	SaveFile(t, Filter(text, [](int c) { return c == '\r' ? 0 : c; }));
	Syx("chmod +x " + t);
}

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	Syx("svn update " + upp_src);
	Vector<String> s = Split(Syx("svnversion " + upp_src), NoDigit);
	if(s.GetCount() == 0)
		Error("Invalid version");
	String version = s.Top();
	RLOG("version: " + version);
	
	DeleteFolderDeep(release);
	RealizeDirectory(release);

	String uppsrc = upp_src + "/uppsrc";
	String dstsrc = release + "/uppsrc";

	CopyFolders(uppsrc, dstsrc, uppsrc + "/packages");
	CopyFolders(uppsrc, dstsrc, uppsrc + "/packages1", false);
	CopyFolders(upp_src, release, uppsrc + "/assemblies");

	SaveFile(dstsrc + "/guiplatform.h", "");
	SaveFile(dstsrc + "/uppconfig.h", LoadFile(uppsrc + "/uppconfig.h"));
	SaveFile(dstsrc + "/ide/version.h", "#define IDE_VERSION \"" + version + "\"\r\n");

	RealizeDirectory(release + "/.config");
	RealizeDirectory(release + "/.cache");

	chdir(release);
	
	SaveScript("install", install_script);
	SaveScript("clean", clean_script);

#ifdef _DEBUG
	#define OPTS " -rvb "
#else
	#define OPTS " -arvb "
#endif

	Syx(GetHomeDirFile("bin/umk") + " uppsrc umk GCC32 " OPTS  + release + "/umks32");
	
	Syx(GetHomeDirFile("bin/umk") + " uppsrc umk GCC -rvsM");
	SaveFile(release + "/uMakefile", LoadFile(release + "/Makefile"));
	Syx(GetHomeDirFile("bin/umk") + " uppsrc ide GCC -rvsM theide");
	
	system("cp " + GetHomeDirFile("upp.bin") + "/" + "*.udc " + release);

	chdir(release_dir);

	Syx("tar cfJ " + GetHomeDirFile("upload") + "/upp-posix-" + version + ".tar.xz upp");
}
