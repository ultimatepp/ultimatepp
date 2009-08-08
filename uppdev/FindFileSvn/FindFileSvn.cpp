#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	FindFile ff("/bin/*.*");
	while(ff) {
		DUMP(ff.GetName());
		DUMP(ff.IsSymLink());
		DUMP(ff.IsFolder());
		DUMP(ff.GetLength());
		LOG("----------------------");
		ff.Next();
	}
}

