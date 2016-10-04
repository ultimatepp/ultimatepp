#include <Core/Core.h>

using namespace Upp;

#define TEST

CONSOLE_APP_MAIN
{
	Any alpha;

	alpha.Create<int>();
	
	if(alpha.Is<int>())
		RLOG("int");
	
	Any beta = alpha;
}
