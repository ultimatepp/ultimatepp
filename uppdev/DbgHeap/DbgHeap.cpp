#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	MemoryIgnoreLeaksBegin();
	byte *x = new byte[100];
	MemoryIgnoreLeaksEnd();
	new int[20];
//	x[100] = 0;
//	delete x;
}

/*
CONSOLE_APP_MAIN
{
	byte *x = new byte[100];
	x[101] = 3;
	delete x;
}

CONSOLE_APP_MAIN
{
	byte *x = new byte[100];
	x[-1] = 3;
	delete x;
}
*/