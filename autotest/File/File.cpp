#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	String p = GetExeDirFile("test.txt");
	FileOut out(p);
	FileOut out2(p);
	DDUMP((bool)out);
	DDUMP((bool)out.IsError());
	DDUMP((bool)out2);
	DDUMP((bool)out2.IsError());
	out2.Close();
	DLOG("--- out2 closed");
	DDUMP((bool)out);
	DDUMP((bool)out.IsError());
	DDUMP((bool)out2);
	DDUMP((bool)out2.IsError());
	
	out.Close();
	DLOG("--- out1 closed");
	DDUMP((bool)out);
	DDUMP((bool)out.IsError());
	DDUMP((bool)out2);
	DDUMP((bool)out2.IsError());
	
	FileIn in(GetExeDirFile("does_not_exist.file123"));
	DDUMP((bool)in);
	DDUMP(in.IsError());
	
	CheckLogEtalon();
}
