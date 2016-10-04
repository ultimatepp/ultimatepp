#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	Index<int> x = GetLngSet();
	for(int i = 0; i < x.GetCount(); i++)
		DDUMP(LNGAsText(x[i]));
}

