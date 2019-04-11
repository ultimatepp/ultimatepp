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

String Syx(const char *s)
{
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
String release = release_dir + "/theide.app";
String upp_src = GetHomeDirFile("upp.src");

int NoDigit(int c) { return IsDigit(c) ? 0 : c; }
int FilterVersion(int c) { return c == ':' ? '_' : c; }

void Make(String pkg, String exe, String method = "MINGW")
{
//	Syx("c:/upp/umk " + ass + " " + pkg + " c:/upp/" + method + ".bm -arv " + upptmp + "/" + exe);
//	FileDelete(upptmp + "/" + ForceExt(exe, ".map"));
}

CONSOLE_APP_MAIN
{
	Syx("svn update " + upp_src);
	Vector<String> s = Split(Syx("svnversion " + upp_src), NoDigit);
	if(s.GetCount() == 0)
		Error("Invalid version");
	String version = s.Top();
	Log("version: " + version);
	
	String contents = release + "/Contents";
	String dst_src = contents + "/SharedSupport";
	DeleteFolderDeep(dst_src);
	RealizeDirectory(dst_src);

	String uppsrc = upp_src + "/uppsrc";
	String dstsrc = dst_src + "/uppsrc";

	CopyFolders(uppsrc, dstsrc, uppsrc + "/packages");
	CopyFolders(uppsrc, dstsrc, uppsrc + "/packages1", false);
	CopyFolders(upp_src, dst_src, uppsrc + "/assemblies");
	SaveFile(dstsrc + "/guiplatform.h", "");
	SaveFile(dstsrc + "/uppconfig.h", LoadFile(uppsrc + "/uppconfig.h"));
	SaveFile(dstsrc + "/ide/version.h", "#define IDE_VERSION \"" + version + "\"\r\n");
	
	Syx(GetHomeDirFile("bin/umk") + " uppsrc ide CLANG -arvs " + release);
	chdir(release_dir);
	Syx("zip -r9 upp-macos-" + version + ".zip theide.app");

#if 0
	DeleteFolderDeep(tmp);
	RealizeDirectory(tmp);
	
	RealizeDirectory("u:/upload");

	CopyFolder(bin, upptmp, false);
	CopyFolder(bin + "/win32", upptmp + "/bin");
	SaveFile(upptmp + "/README", LoadDataFile("README"));
	
	CopyFolders(uppsrc, ass, uppsrc + "/packages");
	CopyFolders(uppsrc, ass, uppsrc + "/packages1", false);
	CopyFolders(upp, upptmp, uppsrc + "/assemblies");
	SaveFile(upptmp + "/uppsrc/guiplatform.h", "");
	SaveFile(upptmp + "/uppsrc/uppconfig.h", LoadFile(uppsrc + "/uppconfig.h"));
	SaveFile(upptmp + "/uppsrc/ide/version.h", "#define IDE_VERSION \"" + version + "\"\r\n");
	Make("ide", "theide32.exe");
	Make("ide", "theide.exe", "MINGWx64");
	Make("umk", "umk.exe");

	SetCurrentDirectory(upptmp);
	
	SaveFile("license.chk", "1");

	Syx("c:/upp/7-zip/7z a u:/upload/upp-mingw-" + Filter(version, FilterVersion) + ".7z"
	    " " + upptmp + " -r -mx -m0fb=255 -mf=off");
	DeleteFolderDeep(upptmp + "/bin/mingw64");
	Syx("c:/upp/7-zip/7z a u:/upload/upp-win-" + Filter(version, FilterVersion) + ".7z"
	    " " + upptmp + " -r -mx -m0fb=255 -mf=off");
#endif
}
