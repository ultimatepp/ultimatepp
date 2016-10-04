#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	Uuid id;
	for(int i = 1; i < 1000; i++) {
		DUMP(Random());
		DUMP(Random(i));
		id.New();
		DUMP(id);
	}
}
