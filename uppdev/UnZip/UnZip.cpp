#include <Core/Core.h>
#include <plugin/zip/zip.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);
	FileIn in("/home/cxl/test.zip");

	UnZip unzip(in);
	
	for(int i = 0; i < unzip.GetCount(); i++)
		DDUMP(unzip.GetPath(i));
	
	String outdir = GetHomeDirFile("zipout.tst");
	
	DeleteFolderDeep(outdir);
	
	while(unzip) {
		String path = AppendFileName(outdir, unzip.GetPath());
		DDUMP(path);
		if(unzip.IsFolder()) {
			RealizeDirectory(path);
			unzip.Skip();
		}
		else {
			RealizePath(path);
			SaveFile(path, unzip.ReadFile());
		}
	}
}
