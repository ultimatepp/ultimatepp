#include <ide/Builders/Builders.h>

CONSOLE_APP_MAIN {
	String uppsrc = GetHomeDirFile("upp.src/uppsrc");

	Workspace wspc;
	SetVar("UPP", uppsrc, false);
	wspc.Scan("umk");

	String ifile;
	String blitz;
	int index = 0;
	for(int i = 0; i < wspc.GetCount(); i++) {
		DDUMP(wspc[i]);
		String pname = wspc[i];
		const Package& pkg = wspc.GetPackage(i);
		for(int j = 0; j < pkg.GetCount(); j++) {
			if(!pkg[j].separator) {
				String fn = pkg[j];
				fn.Replace("\\", "/");
				DDUMP(fn);
				String ext = GetFileExt(fn);
				String sourceFile = uppsrc + "/" + pname + "/" + fn;
				DDUMP(sourceFile);
				String sfn = "uppsrc/" + pname + "/" + fn;
				if(ext == ".cpp" && HdependBlitzApproved(sourceFile))
					BlitzFile(blitz, sfn, HdependGetDefines(sourceFile), ++index);
				else
				if(findarg(ext, ".c", ".cpp") >= 0)
					MergeWith(ifile, " ", sfn);
			}
		}
	}
	DDUMP(blitz);
	DDUMP(ifile);
	
	SaveFile(GetFileFolder(uppsrc) + "/mkumk_blitz.cpp", blitz);
	
	FileOut info(GetFileFolder(uppsrc) + "/build_info.h");
	MkBuildInfo(info);
	
	String cmdline = "clang -pthread -Iuppsrc -I. -DflagMAIN -DNO_FONTSYS -DCUSTOM_FONTSYS mkumk_blitz.cpp " + ifile + " -lstdc++ -lm -lcrypto -lssl -lz -lbz2 "
	                 "`pkg-config --libs libpng` -o ~/test";
	DDUMP(cmdline);
}
