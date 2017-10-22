#include <Core/Core.h>
#include <plugin/lzma/lzma.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_FILE|LOG_COUT);

	String data = LoadDataFile("TestLZMA.cpp");
	for(int i = 0; i < 6; i++)
		data << data;
	
	DUMP(data.GetCount());
	String s = LZMACompress(data);
	DUMP(s.GetCount());
	ASSERT(LZMADecompress(s) == data);
	LOG("============= OK");
}
