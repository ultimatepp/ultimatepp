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
	RLOG("Directory " << src);
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

extern const char *install_script;
extern const char *clean_script;
extern const char *readme;

String SaveText(const char *name, const char *text)
{
	RLOG("File " << name);
	String t = release + "/" + name;
	SaveFile(t, Filter(text, [](int c) { return c == '\r' ? 0 : c; }));
	return t;
}

void SaveScript(const char *name, const char *text)
{
	Syx("chmod +x " + SaveText(name, text));
}

void FixMakefile(String tgt, String src)
{
	String umk = LoadFile(src);
	umk.Replace("-Wl,--gc-sections ", ""); // many systems do not support this
	umk.Replace("-Wl,-O,2 ", ""); // many systems do not support this
	SaveFile(tgt, umk);
}

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	String version = AsString(atoi(LoadFile(GetHomeDirFile("upp-revision"))) + 2270);
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
	SaveFile(release + "/uppsrc/.clang-format", LoadFile(upp_src + "/.clang-format"));
	SaveFile(dstsrc + "/ide/version.h", "#define IDE_VERSION \"" + version + "\"\r\n");

	RealizeDirectory(release + "/.config");
	RealizeDirectory(release + "/.cache");

	chdir(release);
	
	SaveScript("install", install_script);
	SaveScript("clean", clean_script);
	SaveScript("configure", LoadFile(upp_src + "/configure"));
	SaveScript("configure_makefile", LoadFile(upp_src + "/configure_makefile"));
	SaveText("README", readme);

#ifdef _DEBUG
	#define OPTS " -rvb "
#else
	#define OPTS " -arvb "
#endif

#ifdef PLATFORM_LINUX
	Syx(GetHomeDirFile("bin/umk") + " ./uppsrc umk GCC32 " OPTS  + release + "/umks32");
#endif

	Syx(GetHomeDirFile("bin/umk") + " ./uppsrc umk GCC +NOMM -rvsM");
	FixMakefile(release + "/umkMakefile.in", release + "/Makefile");
	Syx(GetHomeDirFile("bin/umk") + " ./uppsrc ide GCC -rvsM theide");
	FixMakefile(release + "/Makefile.in", release + "/Makefile");

	SaveFile(release + "/license.chk", "1");
	
	system("cp " + GetHomeDirFile("upp.bin") + "/" + "*.udc " + release);

	chdir(release_dir);

	Syx("tar cfJ " + GetHomeDirFile("upload") + "/upp-posix-" + version + ".tar.xz upp");
}
