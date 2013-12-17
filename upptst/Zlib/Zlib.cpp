#include <Core/Core.h>

using namespace Upp;

String RandomString(int len)
{
	StringBuffer r;
	for(int i = 0; i < len; i++)
		if(Random(30) == 0)
			r.Cat("\r\n");
		else
			r.Cat(Random(64) + 32);
	return r;
}

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_FILE|LOG_COUT);

	String data = RandomString(2000000);
	
	ASSERT(ZDecompress(ZCompress(data)) == data);
	ASSERT(GZDecompress(GZCompress(data)) == data);
	
	String path = GetHomeDirFile("test.txt");
	SaveFile(path, data);
	GZCompressFile(path);
	GZDecompressFile(path + ".2", path + ".gz");
	
	ASSERT(LoadFile(path + ".2") == data);

	LOG("============= OK");
}
