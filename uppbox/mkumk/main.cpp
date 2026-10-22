#include <ide/Builders/Builders.h>

void SaveChangedFile2(const char *path, const String& data)
{
	if(LoadFile(path) != data)
		SaveFile(path, data);
}

CONSOLE_APP_MAIN {
	String uppsrc = GetHomeDirFile("upp.src/uppsrc");

	Workspace wspc;
	SetVar("UPP", uppsrc, false);
	wspc.Scan("umk");

	String ifile;
	String blitz;
	int index = 0;
	for(int i = 0; i < wspc.GetCount(); i++) {
		String pname = wspc[i];
		const Package& pkg = wspc.GetPackage(i);
		for(int j = 0; j < pkg.GetCount(); j++) {
			if(!pkg[j].separator) {
				String fn = pkg[j];
				fn.Replace("\\", "/");
				String ext = GetFileExt(fn);
				String sourceFile = uppsrc + "/" + pname + "/" + fn;
				String sfn = "uppsrc/" + pname + "/" + fn;
				if(ext == ".cpp" && HdependBlitzApproved(sourceFile))
					BlitzFile(blitz, sfn, HdependGetDefines(sourceFile), ++index);
				else
				if(findarg(ext, ".c", ".cpp") >= 0)
					MergeWith(ifile, " ", sfn);
			}
		}
	}
	
	SaveChangedFile2(GetFileFolder(uppsrc) + "/mkumk_blitz.cpp", blitz);
	
	String cmdline = "-O2 -pthread -Iuppsrc -I. -DflagMAIN -DflagNO_FONTSYS mkumk_blitz.cpp " + ifile +
	                 " -lstdc++ -llzma -lpcre2-8 -lpcre2-32 -llz4 -lzstd -lxxhash -lm -lz -lbz2 `pkg-config --libs libpng`"
	                 " `pkg-config --libs libpng`"
	                 " -o umk";

	SaveChangedFile2(GetFileFolder(uppsrc) + "/mkumk_gcc.sh", "gcc " + cmdline);
	SaveChangedFile2(GetFileFolder(uppsrc) + "/mkumk_clang.sh", "clang " + cmdline);
}
