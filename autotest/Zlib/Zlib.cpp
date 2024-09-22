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
	return String(r);
}

void Check(const String& data)
{
	ASSERT(ZDecompress(ZCompress(data)) == data);
	ASSERT(GZDecompress(GZCompress(data)) == data);
	
	String path = GetHomeDirFile("test.txt");
	SaveFile(path, data);
	GZCompressFile(path);
	GZDecompressFile(path + ".2", path + ".gz");
	
	ASSERT(LoadFile(path + ".2") == data);
}

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_FILE|LOG_COUT);

	Check(String());
	for(int i = 1; i < 2000; i++)
		Check(RandomString(i));
	Check(RandomString(20000));
	Check(RandomString(2000000));
	
	String data = "Hello!";
	StringStream in(data);
	StringStream out;
	ASSERT(ZCompress(out, in) == out.GetResult().GetCount());

	LOG("============= OK");
}
