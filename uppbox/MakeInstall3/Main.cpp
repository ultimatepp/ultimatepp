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
	String r = Sys(s);
	if(r.IsVoid())
		Error("Failed: " + String(s));
	return r;
}

void CopyFolder(const char *src, const char *dst, bool deep = true)
{
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
/*
void MakeInstall(const String& ifn)
{
	SetCurrentDirectory(AppendFileName(outdir, "u"));
	DeleteFile("c:\\upp.7z");
	Syx("7z a c:\\upp.7z * -r -mx -m0fb=255 -mf=off");
	SetCurrentDirectory("C:\\");
	Syx("umk uppbox WinInstaller2 MSC8 -ar " + ifn);
	DeleteFile("c:\\upp.7z");
}
*/

String tmp = "u:/upp.tmp";
String upptmp = tmp + "/u";
String upp = "u:/upp.src";
String uppsrc = upp + "/uppsrc";
String win32 = "u:/theide";

void CopyIdeFile(const String& fn)
{
	SaveFile(upptmp + '/' + fn, LoadFile(win32 + '/' + fn));
}

int NoCrLf(int c) { return c == '\r' || c == '\n' ? 0 : c; }
int FilterVersion(int c) { return c == ':' ? '_' : c; }

CONSOLE_APP_MAIN
{
	String version = Filter(Syx("svnversion " + upp), NoCrLf);
	Log("version: " + version);
	
	DeleteFolderDeep(tmp);
	RealizeDirectory(tmp);
	
	CopyFolders(uppsrc, upptmp + "/uppsrc", uppsrc + "/packages");
	CopyFolders(uppsrc, upptmp + "/uppsrc", uppsrc + "/packages1", false);
	CopyFolders(upp, upptmp, uppsrc + "/assemblies");
	SaveFile(upptmp + "/uppsrc/ide/version.h", "#define IDE_VERSION \"" + version + "\"");
	Syx("umk upptmp ide MSC9 -ar " + upptmp + "/theide.exe");
	Syx("umk upptmp umk MSC9 -ar " + upptmp + "/umk.exe");

	CopyIdeFile("dbghelp.dll");
	CopyIdeFile("en-us.scd");
	CopyIdeFile("en-gb.scd");

	SetCurrentDirectory(upptmp);
	
	SaveFile("install.upp", LoadFile(uppsrc + "/install.upp"));
	SaveFile("license.chk", "1");

	Syx(win32 + "/7za/7za.exe a " + tmp + "/upp.7z * -r -mx -m0fb=255 -mf=off");
	SetCurrentDirectory(tmp);
	Syx("umk uppbox WinInstaller2 MSC9 -ar u:/upp-win32-" + Filter(version, FilterVersion) + ".exe");

#if 0
	Cout() << "Ssvnversion
	
	version = ~dlg.version;
	outdir = ~dlg.outdir;
	uppdir = ~dlg.uppdir;
	idedir = ~dlg.idedir;
	mingw = ~dlg.mingw;

	String version_h = AppendFileName(uppdir, "uppsrc/ide/version.h");
	String version_bak = AppendFileName(uppdir, "uppsrc/ide/version.bak");
	MoveFile(version_h, version_bak);
	SaveFile(version_h, "#define IDE_VERSION \"" + version + "\"");
	try {
		AllocConsole();

		vdir = AppendFileName(outdir, version);
//		if(DirectoryExists(vdir))
//			Error("version already exists");
		RealizeDirectory(vdir);

		DeleteFolderDeep(AppendFileName(outdir, "u"));
		CopyUppFolders("uppsrc", "u/uppsrc",
			"BuildAll;CbGen;CodeEditor;"
			"coff;Core;CppBase;Crypto;CtrlCore;"
			"CtrlLib;Draw;DropGrid;Esc;Geom;"
			"GLCtrl;GridCtrl;HexView;IconDes;ide;"
			"MySql;Ole;OleDB;Oracle;PdfDraw;"
			"plugin;PostgreSQL;Report;RichEdit;RichText;"
			"Sql;SqlCommander;SqlCtrl;umk;Updater"
		);


		CopyUppFolders("uppsrc", "u/uppsrc", "Web;Web/TServ;Web/SSL", false);

		CopyUppFolder("examples", "u/examples");
		CopyUppFolder("reference", "u/reference");
		CopyUppFolder("tutorial", "u/tutorial");
		CopyUppFolder("uppsrc/Common", "u/Common");

		CopyFolder(AppendFileName(idedir, "sdl"), AppendFileName(outdir, "u/sdl"));

		if(dlg.make_mingw || dlg.make_win) {
			Sys("umk uppsrc ide MSC71cdb -arm " + AppendFileName(outdir, "u/theide.exe"));
			Sys("umk uppsrc umk MSC71cdb -arm " + AppendFileName(outdir, "u/umk.exe"));
		}

		MoveFile(AppendFileName(outdir, "u/theide.map"), AppendFileName(vdir, "theide.map"));
		MoveFile(AppendFileName(outdir, "u/umk.map"), AppendFileName(vdir, "umk.map"));

		CopyIdeFile("dbghelp.dll", "u/dbghelp.dll");
		SaveFile(AppendFileName(outdir, "u/install.upp"),
		         LoadFile(AppendFileName(uppdir, "uppsrc/install.upp")));
		CopyIdeFile("en-us.scd", "u/en-us.scd");
		CopyIdeFile("en-gb.scd", "u/en-gb.scd");
		SaveFile(AppendFileName(outdir, "u/license.chk"), "1");

		FindFile ff(AppendFileName(idedir, "*.upt"));
		while(ff) {
			SaveFile(AppendFileName(outdir, String("u/") + ff.GetName()),
			         LoadFile(AppendFileName(idedir, ff.GetName())));
			ff.Next();
		}

		String mw = AppendFileName(outdir, "u/mingw");
		CopyFolder(mingw, mw);
		
		DeleteFolderDeep(vdir);
		RealizeDirectory(vdir);

		String uppmingw = AppendFileName(vdir, "upp-mingw-" + version + ".exe");
		if(dlg.make_mingw)
			MakeInstall(uppmingw);

		String uppwin = AppendFileName(vdir, "upp-win-" + version + ".exe");
		DeleteFolderDeep(mw);
		if(dlg.make_win)
			MakeInstall(uppwin);

		String uppsrc = AppendFileName(vdir, "upp-src-" + version + ".zip");
		if(dlg.make_src)
		{
			FindFile ff(AppendFileName(outdir, "u/*.*"));
			while(ff) {
				String name = ff.GetName();
				String p = AppendFileName(outdir, "u/" + name);
				if(ff.IsFile())
					DeleteFile(p);
				ff.Next();
			}
			SetCurrentDirectory(AppendFileName(outdir, "u"));
			Sys("zip -r " + uppsrc + " *");
		}
		
		if(dlg.upload) {
			if(!IsNull(dlg.hour)) {
				Time tm = GetSysTime();
				Time tmu = tm;
				tmu.hour = (int)~dlg.hour;
				if(tmu < tm)
					tmu += 86400;
				int ws = int(tmu - tm);
				Cout() << "Waiting " << ws << " second until " << tmu << '\n';
				Sleep(ws * 1000);
			}
			Cout() << "*** Uploading started at " << GetSysTime() << '\n';
			if(dlg.make_mingw)
				SfSave(uppmingw);
			if(dlg.make_win)
				SfSave(uppwin);
			if(dlg.make_src)
				SfSave(uppsrc);
			Cout() << "*** Uploading finished at " << GetSysTime() << '\n';
		}
		Cout() << Sprintf("*** OK\n");
		PromptOK("Release successfull");
	}
	catch(...) {
		Exclamation("Error!");
	}
	DeleteFile(version_h);
	MoveFile(version_bak, version_h);
#endif
}
