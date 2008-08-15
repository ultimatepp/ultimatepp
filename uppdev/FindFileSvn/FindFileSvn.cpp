#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	FindFile ff("u:\\upp.src\\uppdev\\aggdraw\\*.*");
	while(ff) {
		DUMP(ff.GetName());
		ff.Next();
	}
}

