#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	FileIn fin(GetDataFile("InFilterStream2.cpp"));
	Zlib zlib;
	InFilterStream in(fin, zlib);
	zlib.GZip().Header().Compress();
	
	String out_path = GetHomeDirFile("infiltertest-output.gz");
	{
		FileOut out(out_path);
		CopyStream(out, in);
	}

	ASSERT(LoadDataFile("InFilterStream2.cpp") == GZDecompress(LoadFile(out_path)));

	FileDelete(out_path);

	LOG("============= OK");
}
