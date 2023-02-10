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

String tmp = "u:/upp.tmp";
String upptmp = tmp + "/upp";
String ass = upptmp + "/uppsrc";
String upp = "u:/upp.src";
String uppsrc = upp + "/uppsrc";
String bin = "u:/upp.bin";

int NoDigit(int c) { return IsDigit(c) ? 0 : c; }
int FilterVersion(int c) { return c == ':' ? '_' : c; }

void Make(String pkg, String exe, String method = "CLANGx64")
{
	Syx("c:/upp/umk " + ass + " " + pkg + " c:/upp/" + method + ".bm -arv " + upptmp + "/" + exe);
	FileDelete(upptmp + "/" + ForceExt(exe, ".map"));
}

CONSOLE_APP_MAIN
{
	String version = AsString(atoi(LoadFile("u:/upp-revision")) + 2270);
	Log("version: " + version);
	
	DeleteFolderDeep(tmp);
	RealizeDirectory(tmp);
	
	RealizeDirectory("u:/upload");
	
	CopyFolder(bin, upptmp, false);
	CopyFolder(bin + "/win32", upptmp + "/bin");
	SaveFile(upptmp + "/dbghelp.dll", LoadFile(bin + "/win32_dlls/dbghelp.dll"));
	SaveFile(upptmp + "/libclang.dll", LoadFile(bin + "/win32_dlls/libclang.dll"));
	SaveFile(upptmp + "/README",
///////////////////////////////////////////////////////////////////////////////
R"--(U++ Windows Installation
Windows release is a simple archive .7z archive. Unpack to directory of your
preference, then just run theide.exe.

U++ does not write anything to registry or outside its directory.

The archive contains CLANG based Win32 toolchain and also SDL 2, OpenSSL, MySql
and PostgreSQL client libraries. TheIDE will also setup Microsoft C++ compiler
if it finds one.
	
U++ minimum requirement is Windows 7 64 with latest patches for theide to run
and Windows 7 64 or 32 with latest patches for compiled applications.)--");
	
	CopyFolders(uppsrc, ass, uppsrc + "/packages");
	CopyFolders(uppsrc, ass, uppsrc + "/packages1", false);
	CopyFolders(upp, upptmp, uppsrc + "/assemblies");
	SaveFile(upptmp + "/uppsrc/guiplatform.h", "");
	SaveFile(upptmp + "/uppsrc/uppconfig.h", LoadFile(uppsrc + "/uppconfig.h"));
	SaveFile(upptmp + "/uppsrc/.clang-format", LoadFile(upp + "/.clang-format"));
	SaveFile(upptmp + "/uppsrc/ide/version.h", "#define IDE_VERSION \"" + version + "\"\r\n");
	Make("ide", "theide.exe");
	Make("umk", "umk.exe");

	SetCurrentDirectory(upptmp);
	
	SaveFile("license.chk", "1");

	Syx("c:/upp/7-zip/7z a u:/upload/upp-win-" + Filter(version, FilterVersion) + ".7z"
	    " " + upptmp + " -r -mx -m0fb=255 -mf=off");
}
