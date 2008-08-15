#include <CtrlLib/CtrlLib.h>

#include <plugin/bz2/bz2.h>
#include <plugin/ftp/ftp.h>

using namespace Upp;

class EditDir : public EditString
{
	public:
		typedef EditDir CLASSNAME;
		EditDir()
		{
			AddFrame(btn);
			btn.SetImage(CtrlImg::smallright()).NoWantFocus();
			btn <<= THISBACK(SelectDir);
		}
	private:
		FrameRight<Button> btn;
		void SelectDir()
		{
			FileSel fs;
			fs.ActiveDir(GetData());
			if(fs.ExecuteSelectDir())
				SetData(fs.Get());
		}
};

#define  LAYOUTFILE <MakeInstall3/install.lay>
#include <CtrlCore/lay.h>
#define AFN AppendFileName
#define CHECKPOINT(x) SaveCheckpoint(x); checkpoint_##x

#include <Wincon.h>

String version = "801-dev1";

String outdir      = "c:\\Dev\\upp.install.final";
String uppdir      = "c:\\Dev\\upp.uvs";
String uppsrc      = "c:\\Dev\\upp.uvs\\uppsrc.uc";
String examples    = "c:\\Dev\\upp.uvs\\examples.uc";
String reference   = "c:\\Dev\\upp.uvs\\reference.uc";
String tutorial    = "c:\\Dev\\upp.uvs\\tutorial.uc";
String bazaar      = "c:\\Dev\\upp.svn\\bazaar";
String theide      = "c:\\Dev\\upp";
String mingw       = "c:\\Dev\\upp.install\\mingw";
String sdl         = "c:\\Dev\\upp.install\\sdl";
String wi          = "c:\\Dev\\upp.uvs\\uppbox.uc\\WinInstaller2";
String wipackage   = "myapps";
String builder     = "MSC80";
String builder_gcc = "MINGW430";
String sevenzipdir = "c:\\Program Files\\7-Zip";

int rebuildall = 1;
bool iserror = false;

FtpClient ftp;
String vdir;
Vector<String> upt;

void Sys(const char *s)
{
	Cout() << s << '\n';
	if(system(s))
	{
		Cout() << "Failed!";
		throw Exc(s);
	}
}

void SysPass(const char *s)
{
	Cout() << s << '\n';
	if(system(s))
	{
		RLOG(s);
		iserror = true;
		Cout() << "Failed!";
	}
}

void SaveCheckpoint(int cp)
{
	SaveFile(AFN(outdir, "checkpoint.tmp"), AsString(cp));
}

int GetCheckpoint()
{
	String s = LoadFile(AFN(outdir, "checkpoint.tmp"));
	int cp = StrInt(s);
	return IsNull(cp) ? 0 : cp;
}

void CopyFolder(const char *src, const char *dst, bool deep = true)
{
	Cout() << Sprintf("Copying %s\n", src);
	RealizeDirectory(dst);
	FindFile ff(String(src) + "/*.*");
	while(ff)
	{
		String name = ff.GetName();
		String s = AFN(src, name);
		String d = AFN(dst, name);
		if(ff.IsFile())
		{
			String q = LoadFile(s);
			String ext = ToLower(GetFileExt(s));
			if(ext != ".aux" && ext != ".$old" && ext != ".upt" && ext != ".dat")
				SaveFile(d, q);
		}
		else if(ff.IsFolder())
		{
			if(name != ".svn" && (deep || *GetFileExt(s)))
				CopyFolder(s, d, deep);
		}
		ff.Next();
	}
}

void ScanUpp(const char *src, Vector<String> &upps)
{
	FindFile ff(String(src) + "/*.*");
	while(ff)
	{
		String s = AFN(src, ff.GetName());
		if(ff.IsFile())
		{
			String ext = ToLower(GetFileExt(s));
			if(ext == ".upp")
				if(LoadFile(s).Find("mainconfig") >= 0)
				upps.Add(GetFileTitle(s));
		}
		else if(ff.IsFolder())
			ScanUpp(s, upps);
		ff.Next();
	}
}

void ScanUpt(const char *src, Vector<String> &upts)
{
	FindFile ff(String(src) + "/*.*");
	while(ff)
	{
		String s = AFN(src, ff.GetName());
		if(ff.IsFile())
		{
			String ext = ToLower(GetFileExt(s));
			if(ext == ".upt")
				upts.Add(GetFileName(s));
		}
		else if(ff.IsFolder())
			ScanUpt(s, upts);
		ff.Next();
	}
}

void CopyUpt(const char *src, const char *dst)
{
	FindFile ff(String(src) + "/*.*");
	while(ff)
	{
		String s = AFN(src, ff.GetName());
		String d = AFN(dst, ff.GetName());
		if(ff.IsFile())
		{
			String ext = ToLower(GetFileExt(s));
			if(ext == ".upt")
			{
				SaveFile(d, LoadFile(s));
				upt.Add(GetFileName(s));
				Cout() << Sprintf("Copying %s\n", s);
			}
		}
		else if(ff.IsFolder())
			CopyUpt(s, dst);
		ff.Next();
	}
}

void UpdateList(String &list, const Vector<String> &files)
{
	for(int i = 0; i < files.GetCount(); i++)
		list += " " + files[i];
}

void CopyFolders(const char *src, const char *dst, const char *folders, bool deep = true)
{
	Vector<String> folder = Split(folders, ';');
	for(int i = 0; i < folder.GetCount(); i++)
		CopyFolder(AFN(src, folder[i]), AFN(outdir, AFN(dst, folder[i])), deep);
}

bool ShowFtpProgress()
{
	int a = ftp.GetSavePos();
	int b = ftp.GetSaveTotal();
	Cout() << Sprintf("%d %%,  %d bytes (%d total)        \r", a * 100 / b, a, b);
	return false;
}

void Error(const char *s)
{
	Cout() << "\n## ERROR " << s << '\n';
	throw Exc();
}

void SfSave(const char *fn)
{
	ftp.WhenProgress = callback(ShowFtpProgress);
	Cout() << Sprintf("connecting to sf.net...");
	if(!ftp.Connect("upload.sourceforge.net", "anonymous", "", true))
		Error("Unable to connect to sf.net: " + ftp.GetError());
	Cout() << Sprintf("connected\n");
	if(!ftp.Cd("incoming"))
		Error("FTP error (cd incoming): " + ftp.GetError());
	Cout() << Sprintf("uploading %s\n", fn);
	if(!ftp.Save(GetFileName(fn), LoadFile(fn)))
		Error("FTP error (file upload): " + ftp.GetError());
	Cout() << Sprintf("OK: %s uploaded\n", fn);
}

String OutDir(const char *fn)
{
	return AFN(outdir, fn);
}

void CopyFile(const char *src, const char *dst)
{
	SaveFile(dst, LoadFile(src));
}

String Quote(const String& s)
{
	String t;
	for(int i = 0; i < s.GetLength(); i++)
		if(s[i] == '\\')
			t += "\\\\";
		else
			t += s[i];
	return t;
}

void SaveVar(const String &path, const String &var = Null)
{
	String upp = IsNull(var) ? "UPP = \"" + Quote(OutDir("u\\uppsrc")) + "\";\n"
							 : "UPP = \"" + Quote(OutDir(var)) + ';' + Quote(OutDir("u\\uppsrc")) + "\";\n";
	SaveFile(path, upp + "COMMON = \"" + Quote(OutDir("u\\common")) + "\";\n" +
						 "OUTPUT = \"" + Quote(OutDir("out")) + "\";\n");
}

GUI_APP_MAIN
{
	try
	{
		WithInstallLayout<TopWindow> dlg;
		CtrlLayoutOKCancel(dlg, "MakeInstall");

		for(int i = 0; i <= 5; ++i)
			dlg.checkpoint.Add(i , AsString(i));

		dlg.checkpoint = GetCheckpoint();

		dlg.builder     <<= builder;
		dlg.builder_gcc <<= builder_gcc;
		dlg.version     <<= version;
		dlg.outdir      <<= outdir;
		dlg.uppdir      <<= uppdir;
		dlg.uppsrc      <<= uppsrc;
		dlg.examples    <<= examples;
		dlg.reference   <<= reference;
		dlg.tutorial    <<= tutorial;
		dlg.bazaar      <<= bazaar;
		dlg.outdir      <<= outdir;
		dlg.uppdir      <<= uppdir;
		dlg.theide      <<= theide;
		dlg.mingw       <<= mingw;
		dlg.sdl         <<= sdl;
		dlg.wi          <<= wi;
		dlg.wipackage   <<= wipackage;
		dlg.sevenzipdir <<= sevenzipdir;

		dlg.rebuild_all = rebuildall;

		LoadFromFile(dlg);
		int c = dlg.Execute();
		StoreToFile(dlg);

		if(c != IDOK)
			return;

		AllocConsole();

		builder     = ~dlg.builder;
		builder_gcc = ~dlg.builder_gcc;
		version     = ~dlg.version;
		outdir      = ~dlg.outdir;
		uppdir      = ~dlg.uppdir;
		uppsrc      = ~dlg.uppsrc;
		examples    = ~dlg.examples;
		reference   = ~dlg.reference;
		tutorial    = ~dlg.tutorial;
		bazaar      = ~dlg.bazaar;
		mingw       = ~dlg.mingw;
		sdl         = ~dlg.sdl;
		theide      = ~dlg.theide;
		wi          = ~dlg.wi;
		wipackage   = ~dlg.wipackage;
		rebuildall  = dlg.rebuild_all;

		sevenzipdir = ~dlg.sevenzipdir;

		String sevenzipfile = sevenzipdir;

		if(sevenzipfile != "")
		{
			sevenzipfile = "\"" + sevenzipfile + "\\7z\"";
		} else {
			sevenzipfile = "7z";
		}

		vdir = AFN(outdir, version);

		String packages =
			" common"
			" examples"
			" reference"
			" tutorial"
			" bazaar"
			" uppsrc";

		String libs =
			" sdl";

		String compiler_mingw =
			" mingw";

		String idefiles =
			" theide.exe"
			" umk.exe"
			" dbghelp.dll"
			" install.upp"
			" en-us.scd"
			" en-gb.scd"
			" license.chk";

		String uppmingw 	= AFN(vdir, "upp-mingw-" 	+ version + ".exe");
		String uppwin		= AFN(vdir, "upp-win-" 		+ version + ".exe");
		String uppsource 	= AFN(vdir, "upp-src-" 		+ version + ".zip");

		Vector<String> upp_examples;
		Vector<String> upp_reference;
		Vector<String> upp_tutorial;

		if(dlg.use_checkpoint)
		{
			if(upt.IsEmpty())
			{
				ScanUpt(uppsrc, upt);
				UpdateList(idefiles, upt);
				if(upt.IsEmpty())
				{
					Exclamation("Upt files not found");
					return;
				}
			}

			switch((int)~dlg.checkpoint)
			{
				case 1: goto checkpoint_1;
				case 2: goto checkpoint_2;
				case 3: goto checkpoint_3;
				case 4: goto checkpoint_4;
				case 5: goto checkpoint_5;
			}
		}

		SaveCheckpoint(Null);

		DeleteFolderDeep(vdir);
		RealizeDirectory(vdir);

		Cout() << "Removing files...\n";

		DeleteFolderDeep(OutDir("u"));
		DeleteFolderDeep(OutDir("out"));

		DeleteFile(AFN(outdir, "checkpoint.tmp"));
		DeleteFile(AFN(outdir, "upp-win.7z"));
		DeleteFile(AFN(outdir, "upp-mingw.7z"));

		Cout() << "Copying uppsrc...\n";

		CopyFolders(uppsrc, "u/uppsrc",
			"ide;IconDes;Topic;CtrlLib;HexView;Esc;CodeEditor;Core;RichEdit;RichText;"
			"Draw;coff;CppBase;CtrlCore;Report;"
			"MySql;Ole;OleDB;Oracle;Sql;SqlCommander;SqlCtrl;Updater;"
			"PdfDraw;PostgreSQL;GLCtrl;DropGrid;CbGen;Crypto;"
			"plugin;BuildAll;"
			"Geom;umk;GridCtrl;AllForI18n;art"
		);

		CopyFolders(uppsrc, "u/uppsrc", "Web;Web/TServ;Web/SSL", false);

		Cout() << "Copying upt's...\n";

		CopyUpt(uppsrc, AFN(outdir, "u"));
		UpdateList(idefiles, upt);

		Cout() << idefiles << '\n';

		SaveFile(AFN(outdir, "u/uppsrc/ide/version.h"), "#define IDE_VERSION \"" + version + "\"\n");

		Cout() << "Copying examples...\n";

		CopyFolder(examples, OutDir("u/examples"));
		Cout() << "Copying reference...\n";
		CopyFolder(reference, OutDir("u/reference"));
		Cout() << "Copying tutorial...\n";
		CopyFolder(tutorial, OutDir("u/tutorial"));
		Cout() << "Copying bazaar...\n";
		CopyFolder(bazaar, OutDir("u/bazaar"));
		Cout() << "Copying common...\n";
		CopyFolder(AFN(uppsrc, "Common"), OutDir("u/Common"));

		if(dlg.make_mingw || dlg.make_win)
		{
			Cout() << "Copying sdl...\n";
			CopyFolder(sdl, OutDir("u/sdl"));
		}

		if(dlg.make_mingw)
		{
			Cout() << "Copying mingw...\n";
			CopyFolder(mingw, OutDir("u/mingw"));
		}

	CHECKPOINT(1):

		if(dlg.test_mingw)
		{
			ScanUpp(OutDir("u\\examples"), upp_examples);
			ScanUpp(OutDir("u\\reference"), upp_reference);
			ScanUpp(OutDir("u\\tutorial"), upp_tutorial);

			DUMPC(upp_examples);
			DUMPC(upp_reference);
			DUMPC(upp_tutorial);

			if(upp_examples.IsEmpty() ||
			   upp_reference.IsEmpty() ||
			   upp_tutorial.IsEmpty())
			   throw Exc("Upp not found");

			String opt = dlg.test_mingw_blitz == 1 ? " -b " : "";

			String bgcc = " " + builder_gcc + " ";

			if(dlg.test_mingw_ide)
			{
				SaveVar(AFN(theide, "upptmp.var"));
					Sys(theide + "\\umk upptmp ide" + bgcc + opt);
			}

			if(dlg.test_mingw_examples)
			{
				SaveVar(AFN(theide, "upptmp.var"), "u\\examples");
				for(int i = 0; i < upp_examples.GetCount(); i++)
					SysPass(theide + "\\umk upptmp " + upp_examples[i] + bgcc + opt);
			}

			if(dlg.test_mingw_reference)
			{
				SaveVar(AFN(theide, "upptmp.var"), "u\\reference");
				for(int i = 0; i < upp_reference.GetCount(); i++)
					SysPass(theide + "\\umk upptmp " + upp_reference[i] + bgcc + opt);
			}

			if(dlg.test_mingw_tutorial)
			{
				SaveVar(AFN(theide, "upptmp.var"), "u\\tutorial");
				for(int i = 0; i < upp_tutorial.GetCount(); i++)
					SysPass(theide + "\\umk upptmp " + upp_tutorial[i] + bgcc + opt);
			}

			if(iserror)
				throw Exc("MinGW error");
		}

		if(dlg.make_mingw || dlg.make_win)
		{
			SaveVar(AFN(theide, "upptmp.var"));

			String opt = rebuildall ? "-arm " : "-rm ";
			Sys(theide + "\\umk upptmp ide " + builder + " " + opt + " " + OutDir("u/theide.exe"));
			Sys(theide + "\\umk upptmp umk " + builder + " -arm " + OutDir("u/umk.exe"));

			MoveFile(AFN(outdir, "u/theide.map"), AFN(vdir, "theide.map"));
			MoveFile(AFN(outdir, "u/umk.map"), AFN(vdir, "umk.map"));

			CopyFile(AFN(theide, "dbghelp.dll"), AFN(outdir, "u/dbghelp.dll"));

			SaveFile(AFN(outdir, "u/install.upp"), LoadFile(AFN(uppsrc, "install.upp")));

			CopyFile(AFN(theide, "en-us.scd"), AFN(outdir, "u/en-us.scd"));
			CopyFile(AFN(theide, "en-gb.scd"), AFN(outdir, "u/en-gb.scd"));

			SaveFile(AFN(outdir, "u/license.chk"), "1");
		}

	CHECKPOINT(2):

		DeleteFolderDeep(vdir);
		RealizeDirectory(vdir);

		if(dlg.make_mingw)
		{
			String outfile = AFN(outdir, "upp-mingw.7z");
			if(!FileExists(outfile))
			{
				SetCurrentDirectory(AFN(outdir, "u"));
				Sys(sevenzipfile + " a -r -mx -m0fb=255 -mf=off " + outfile + packages + libs + compiler_mingw + idefiles);
			}
			SaveFile(AFN(wi, "data.rc"), "1112 RCDATA MOVEABLE PURE \"" + Quote(outfile) + "\"");
			SetCurrentDirectory(theide);
			Sys(theide + "\\umk " + wipackage + " WinInstaller2 " + builder + " -ar " + uppmingw);
		}

	CHECKPOINT(3):

		if(dlg.make_win)
		{
			String outfile = AFN(outdir, "upp-win.7z");
			if(!FileExists(outfile))
			{
				SetCurrentDirectory(AFN(outdir, "u"));
				Sys(sevenzipfile + " a -r -mx -m0fb=255 -mf=off " + outfile + packages + libs + idefiles);
			}
			SaveFile(AFN(wi, "data.rc"), "1112 RCDATA MOVEABLE PURE \"" + Quote(outfile) + "\"");
			SetCurrentDirectory(theide);
			Sys(theide + "\\umk " + wipackage + " WinInstaller2 " + builder + " -ar " + uppwin);
		}

	CHECKPOINT(4):

		if(dlg.make_src)
		{
			SetCurrentDirectory(AFN(outdir, "u"));
			Sys(sevenzipfile + " a -tzip -r " + uppsource + packages);
		}

	CHECKPOINT(5):

		if(dlg.upload)
		{
			if(!IsNull(dlg.hour))
			{
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
				SfSave(uppsource);
			Cout() << "*** Uploading finished at " << GetSysTime() << '\n';
		}
		Cout() << Sprintf("*** OK\n");

		PromptOK("Release successfull");
	}
	catch(Exc &e)
	{
		Exclamation(DeQtfLf(e));
	}
	catch(...)
	{
		Exclamation("Error!");
	}
}
