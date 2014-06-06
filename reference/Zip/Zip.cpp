#include <plugin/zip/zip.h>

using namespace Upp;

CONSOLE_APP_MAIN {
	StdLogSetup(LOG_COUT|LOG_FILE);

	String zipfile = GetHomeDirFile("test.zip");

	// Compression
	FileZip zip(zipfile);

	zip.WriteFolder("test", GetSysTime());

	const char *somedata = "some data";
	zip.WriteFile(somedata, strlen(somedata), "somedata.txt");

	zip.WriteFile(String("other data"), "test/otherdata.txt");
	
	zip.BeginFile("file1.txt");
	zip.Put(somedata, strlen(somedata));
	zip.EndFile();
	
	{
		OutFilterStream oz;
		zip.BeginFile(oz, "file2.txt");
		oz << "some content";
	} //OutFilterStream destructor calls EndFile

	if(zip.Finish())
		LOG(zipfile << " created succesfully");
	
	// Decompression
	FileUnZip unzip(zipfile);
	while(!(unzip.IsEof() || unzip.IsError())) {
		LOG("Path:" << unzip.GetPath());
		LOG("  Time:" << unzip.GetTime());
		if(!unzip.IsFolder()) {
			LOG("  Size:" << unzip.GetLength());
			LOG("  Content:" << unzip.ReadFile());
		}
		else
			unzip.SkipFile();
	}
}
