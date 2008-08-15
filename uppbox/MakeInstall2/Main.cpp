#include <CtrlLib/CtrlLib.h>
#include <plugin/bz2/bz2.h>
#include <plugin/ftp/ftp.h>

using namespace Upp;

#define  LAYOUTFILE <MakeInstall2/install.lay>
#include <CtrlCore/lay.h>

#include <Wincon.h>

String version = "708dev2a";

String outdir = "d:\\";
String uppdir = "d:\\";
String idedir = "c:\\upp";
String mingw  = "c:\\mingwi";
String vdir;

void Sys(const char *s)
{
	Cout() << s << '\n';
	if(system(s)) {
		Cout() << "Failed!";
		throw Exc();
	}
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
		if(ff.IsFolder()) {
			if(deep || *GetFileExt(s))
				CopyFolder(s, d, deep);
		}
		ff.Next();
	}
}

void CopyUppFolder(const char *src, const char *dst, bool deep = true)
{
	CopyFolder(AppendFileName(uppdir, src), AppendFileName(outdir, dst), deep);
}

void CopyUppFolders(const char *src, const char *dst, const char *folders, bool deep = true)
{
	Vector<String> folder = Split(folders, ';');
	for(int i = 0; i < folder.GetCount(); i++)
		CopyUppFolder(AppendFileName(src, folder[i]), AppendFileName(dst, folder[i]), deep);
}

void CopyIdeFile(const char *src, const char *dst)
{
	SaveFile(AppendFileName(outdir, dst), LoadFile(AppendFileName(idedir, src)));
}

bool ShowFtpProgress(int a, int b)
{
	if((a & 65535) == 0)
		Cout() << Sprintf("%d %%,  %d bytes (%d total)\n", a * 100 / b, a, b);
	return false;
}

void Error(const char *s)
{
	Cout() << "\n## ERROR " << s << '\n';
	throw Exc();
}

void SfSave(const char *fn)
{
	FtpClient ftp;
	Cout() << Sprintf("connecting to sf.net...");
	if(!ftp.Connect("upload.sourceforge.net", "anonymous", "", true))
		Error("Unable to connect to sf.net: " + ftp.GetError());
	Cout() << Sprintf("connected\n");
	if(!ftp.Cd("incoming"))
		Error("FTP error (cd incoming): " + ftp.GetError());
	Cout() << Sprintf("uploading %s\n", fn);
	if(!ftp.Save(GetFileName(fn), LoadFile(fn), callback(ShowFtpProgress)))
		Error("FTP error (file upload): " + ftp.GetError());
	Cout() << Sprintf("OK: %s uploaded\n", fn);
}

void MakeInstall(const String& ifn)
{
	SetCurrentDirectory(AppendFileName(outdir, "u"));
	DeleteFile("c:\\upp.7z");
	Sys("7z a c:\\upp.7z * -r -mx -m0fb=255 -mf=off");
	SetCurrentDirectory("C:\\");
	Sys("umk uppbox WinInstaller2 MSC8 -ar " + ifn);
	DeleteFile("c:\\upp.7z");
}

GUI_APP_MAIN
{
	WithInstallLayout<TopWindow> dlg;
	CtrlLayoutOKCancel(dlg, "MakeInstall");

	dlg.version <<= "603alfa3";
	dlg.outdir <<= "d:\\";
	dlg.uppdir <<= "d:\\";
	dlg.idedir <<= "d:\\theide";
	dlg.mingw  <<= "c:\\mingwi";

	LoadFromFile(dlg);
	int c = dlg.Execute();
	StoreToFile(dlg);
	if(c != IDOK)
		return;

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
}
