#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);
	DDUMP(FileGetTime("asfasdfasdfasdf"));
	DDUMP(IsNull(FileGetTime("asfasdfasdfasdf")));
	DLOG("================");
	for(int pass = 0; pass < 2; pass++) {
		FileIn in;
		if(pass) {
			DLOG("==== Opening nonexistent file");
			in.Open("aklsdflkasljkdf");
		}
	
		bool second = false;
		for(;;) {
			DDUMP((bool)in);
			DDUMP(in.IsEof());
			DDUMP(in.IsError());
			DDUMP(in.GetPos());
			DDUMP(in.GetSize());
			DDUMP(in.Get());
			if(second)
				break;
			second = true;
			DLOG("--- Seek 0");
			in.Seek(0);
		}
	}
	
	CheckLogEtalon();
}
