#include <Core/Core.h>
#include <plugin/bz2/bz2.h>

void Put(StringStream& ss, const String& data)
{
	ss.PutL(data.GetLength());
	ss.Put(data);
}

String GetFile(const char *s)
{
	return LoadFile(s);
}

void GatherFile(StringStream& ss, const char *dir, const char *name)
{
	String data = LoadFile(AppendFileName(dir, name));
	if(!data.IsVoid()) {
		ss.Put(1);
		Put(ss, name);
		Put(ss, data);
	}
}

void GatherFiles(StringStream& ss, const char *dir, String path)
{
	String p1 = AppendFileName(dir, path);
	FindFile ff(p1 + "/*.*");
	while(ff) {
		String name = ff.GetName();
		if(ff.IsFile()) {
			printf("gathering %s\n", name);
			GatherFile(ss, p1, name);
		}
		else
		if(ff.IsFolder()) {
			ss.Put(0);
			Put(ss, name);
			GatherFiles(ss, dir, AppendFileName(path, name));
		}
		ff.Next();
	}
	ss.Put(2);
}

void CompressFolder(const char *archive, const char *dir)
{
	StringStream ss;
	GatherFiles(ss, dir, "");
	SaveFile(archive, BZ2Compress(ss));
}

void CopyFolder(const char *src, const char *dst)
{
	RealizeDirectory(dst);
	FindFile ff(String(src) + "/*.*");
	while(ff) {
		String s = AppendFileName(src, ff.GetName());
		String d = AppendFileName(dst, ff.GetName());
		if(ff.IsFile()) {
			printf("copying %s\n", s);
			String q = LoadFile(s);
			String ext = ToLower(GetFileExt(s));
			if(ext != ".aux" && ext != ".$old") {
				if(q != LoadFile(d)) {
					SaveFile(d, q);
					FileSetTime(d, ff.GetLastWriteTime());
				}
			}
		}
		else
		if(ff.IsFolder())
			CopyFolder(s, d);
		ff.Next();
	}
}

void CopyFolders(const char *src, const char *dst, const char *folders)
{
	Vector<String> folder = Split(folders, ';');
	for(int i = 0; i < folder.GetCount(); i++)
		CopyFolder(AppendFileName(src, folder[i]), AppendFileName(dst, folder[i]));
}

CONSOLE_APP_MAIN
{
	for(int q = 0; q < 3; q++) {
		DeleteFolderDeep("d:/u");
		CopyFolders("D:/uppsrc", "d:/u/uppsrc",
		    "ide;Topic;CtrlLib;Web;HexView;Esc;CodeEditor;Core;RichEdit;RichText;Image;"
		    "Draw;coff;CppBase;CtrlCore;Report;"
		    "Geom;MySql;Ole;OleDB;Oracle;Sql;SqlCommander;SqlCtrl;TCore;TCtrlLib;TDraw;TSql;Updater;"
		    "PdfDraw;GLCtrl;VectorDes;CbGen;Crypto;"
		    "plugin"
		);

		CopyFolder("d:/examples", "d:/u/examples");
		CopyFolder("d:/reference", "d:/u/reference");
		CopyFolder("d:/uppsrc/Common", "d:/u/Common");

		if(q < 2) {
			CopyFolder("d:/theide/SDL-1.2.9", "d:/u/SDL-1.2.9");
			SaveFile("d:/u/theide.exe", LoadFile("d:/theide/theide_.exe"));
			SaveFile("d:/u/dbghelp.dll", LoadFile("d:/theide/dbghelp.dll"));
			SaveFile("d:/u/install.upp", LoadFile("d:/uppsrc/install.upp"));
			SaveFile("d:/u/license.chk", "1");
			SaveFile("d:/u/en-us.scd", LoadFile("d:/theide/en-us.scd"));
			SaveFile("d:/u/en-gb.scd", LoadFile("d:/theide/en-gb.scd"));
			FindFile ff("d:/theide/*.upt");
			while(ff) {
				SaveFile(AppendFileName("d:/u", ff.GetName()), LoadFile(AppendFileName("d:/theide", ff.GetName())));
				ff.Next();
			}
		}

		if(q == 1)
			CopyFolder("c:/mingw32", "d:/u/mingw");

		printf("Compressing!\n");

		switch(q) {
		case 0:
			CompressFolder("d:/upp.b2a", "d:/u");
			break;
		case 1:
			CompressFolder("d:/uppmingw.b2a", "d:/u");
			break;
		case 2:
			DeleteFile("d:\\src.zip");
			SetCurrentDirectory("d:\\u");
			system("zip -r d:\\src.zip *");
			break;
		}
	}
}
