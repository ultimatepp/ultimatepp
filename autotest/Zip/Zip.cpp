#include <Core/Core.h>
#include <plugin/zip/zip.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	StringZip z;
	
	z.WriteFile("this is content of file1", "file1");
	z.WriteFile("this is content of file2", "file2");
	
	String data = z.Finish();
	
	StringUnZip unzip(data);
	
	for(int pass = 0; pass < 2; pass++) {
		ASSERT(!unzip.IsEof());
		ASSERT(unzip.GetPath() == "file1");
		ASSERT(unzip.ReadFile() == "this is content of file1");
		ASSERT(!unzip.IsEof());
		ASSERT(unzip.GetPath() == "file2");
		ASSERT(unzip.ReadFile() == "this is content of file2");
		ASSERT(unzip.IsEof());
		unzip.Seek(0);
	}
	
	ASSERT(unzip.ReadFile("file1") == "this is content of file1");
	ASSERT(unzip.ReadFile("file2") == "this is content of file2");
	ASSERT(unzip.ReadFile("file3").IsVoid());
	
	LOG("================= OK");
}
