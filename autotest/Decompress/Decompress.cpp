#include <plugin/zip/zip.h>
#include <plugin/lzma/lzma.h>
#include <plugin/bz2/bz2.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	String orig = LoadDataFile("X11App.cpp");
	ASSERT(orig.GetCount());
	{
		FileIn in(GetDataFile("X11App.cpp.zip"));
		UnZip unzip(in);
		ASSERT(!unzip.IsEof());
		ASSERT(unzip.ReadFile() == orig);
		ASSERT(unzip.IsEof());
	}
	{
		ASSERT(GZDecompress(LoadDataFile("X11App.cpp.gz")) == orig);
	}
	{
		ASSERT(BZ2Decompress(LoadDataFile("X11App.cpp.bz2")) == orig);
	}
	{
		ASSERT(LZMADecompress(LoadDataFile("X11App.cpp.lzma")) == orig);
	}
	
	LOG("================== OK");
}
