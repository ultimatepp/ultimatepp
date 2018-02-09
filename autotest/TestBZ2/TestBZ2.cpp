#include <Core/Core.h>
#include <plugin/bz2/bz2.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_FILE|LOG_COUT);

	String data = LoadDataFile("TestBZ2.cpp");
	for(int i = 0; i < 6; i++)
		data << data;
	
	DUMP(data.GetCount());
	String s = BZ2Compress(data);
	DUMP(s.GetCount());
	ASSERT(BZ2Decompress(s) == data);
	
	StringStream ss(s);
	BZ2DecompressStream in(ss);
	String s2;
	while(!in.IsEof())
		s2.Cat(in.Get());
	ASSERT(s2 == data);
	
	LOG("============= OK");
}
