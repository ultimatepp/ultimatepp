#include <plugin/zip/zip.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	String outdir = "e:\\zipout";
	FileUnZip unzip("d:/uppsrc.zip");
	while(/*unzip*/0) {
		String fn = AppendFileName(outdir, unzip.GetPath());
		if(unzip.IsFolder()) {
			RealizeDirectory(fn);
			unzip.SkipFile();
		}
		else {
			RealizePath(fn);
			FileOut out(fn);
			unzip.ReadFile(out);
		}
	}
	if(unzip.IsError())
		LOG("Error!");

	{
		FindFile ff("e:/zipout/uppsrc/CtrlLib/*.*");
		FileZip zip("e:/test.zip");
		while(ff) {
			DUMP(ff.GetName());
			if(ff.IsFile())
				zip.WriteFile(LoadFile(AppendFileName("e:/zipout/uppsrc/CtrlLib", ff.GetName())),
				              ff.GetName());
			ff.Next();
		}
	}
}

	unzip.Create("e:/test.zip");
	while(unzip) {
		String fn = AppendFileName(outdir, unzip.GetPath());
		if(unzip.IsFolder()) {
			RealizeDirectory(fn);
			unzip.SkipFile();
		}
		else {
			RealizePath(fn);
			FileOut out(fn);
			unzip.ReadFile(out);
		}
	}
	if(unzip.IsError())
		LOG("Error!");
	
	FileZip zip("e:/xxx.zip");
	zip.WriteFile(LoadFile("e:/xxx.txt"), "xxx.txt");
}
