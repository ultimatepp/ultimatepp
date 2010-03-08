#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN {
	FindFile ff("c:\\*.*");
	if(ff)
		DDUMP(ff.GetName());
	
	FindFile ff2("d:\\*.*");
	if(ff)
		DDUMP(ff.GetName());
}