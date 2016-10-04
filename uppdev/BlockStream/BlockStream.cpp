#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	Cout() << "Hello wonderful U++ world\n";
	FileOut out(ConfigFile("test"));
	out.Put("hello", 5);
	out.Put("hello", 5);
	out.Put("hello", 5);
	FileIn f;
	f.Open(GetDataFile("BlockStream.cpp"));
	for (;;) {
		byte b;
		if (f.Get(&b, 1) != 1) break;
		Cout() << b;
	}

}
//add many comment lines to get big filesize
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
