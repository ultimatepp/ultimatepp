#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	String p = GetExeDirFile("test.txt");
	FileOut out(p);
	DDUMP((bool)out);
	DDUMP((bool)out.IsError());
	FileOut out2(p);
	DDUMP((bool)out2);
	DDUMP((bool)out2.IsError());
	
	FileIn in(GetExeDirFile("does_not_exist.file123"));
	DDUMP((bool)in);
	DDUMP(in.IsError());
}
