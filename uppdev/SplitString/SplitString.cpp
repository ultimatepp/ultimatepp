#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN {
	Vector<String> s = Split("John Doe Had A Dog", "Had"); 
	DUMPC(s);
}
