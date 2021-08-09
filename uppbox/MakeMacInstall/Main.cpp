#include <Core/Core.h>
#include <plugin/bz2/bz2.h>

using namespace Upp;

void Log(const char *txt)
{
	Cout() << txt << "\r\n";
}

void Error(const char *e)
{
	Log(e);
	abort();
}

String Syx(String s)
{
	s.Replace("~", GetHomeDirectory());
	Log(s);
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
String release_app = release + "/theide.app";
String upp_src = GetHomeDirFile("upp.src");

int NoDigit(int c) { return IsDigit(c) ? 0 : c; }
int FilterVersion(int c) { return c == ':' ? '_' : c; }

CONSOLE_APP_MAIN
{
	String version = AsString(atoi(LoadFile(GetHomeDirFile("upp-revision"))) + 2270);
	Log("version: " + version);
	
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

#ifdef _DEBUG
	Syx(GetHomeDirFile("bin/umk") + " ~/upp.src/uppsrc ide CLANG -brvs " + release_app);
	Syx(GetHomeDirFile("bin/umk") + " ~/upp.src/uppsrc umk CLANG -brvs " + release + "/umk");
#else
	Syx(GetHomeDirFile("bin/umk") + " ~/upp.src/uppsrc ide CLANG -abrvs " + release_app);
	Syx(GetHomeDirFile("bin/umk") + " ~/upp.src/uppsrc umk CLANG -abrvs " + release + "/umk");
#endif

	system("cp " + GetHomeDirFile("upp.bin") + "/" + "*.udc " + release);

	chdir(release_dir);
	String upload = GetHomeDirFile("upload");
	RealizeDirectory(upload);
	Syx("tar cfJ " + upload + "/upp-macos-" + version + ".tar.xz upp");
}
