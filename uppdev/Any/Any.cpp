#include <Core/Core.h>

using namespace Upp;

int x;

thread__ int *alfa[10] = { &x, &x };

CONSOLE_APP_MAIN
{
	Any alpha;

	alpha.Create<int>();
	
	One<String> x;
	x.

	if(alpha.Is<int>())
		RLOG("int");
	if(alpha.Is<int>())
		RLOG("int");
}
