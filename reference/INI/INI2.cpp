#include <Core/Core.h>

using namespace Upp;

// referencing INI_* variables from another file (or header)

namespace Config {

extern IniBool flag1;
extern IniBool flag2;

extern IniString text;

extern IniInt number;

};

void ExternalFn()
{
	LOG("text accessed from another file: " << Config::text);
}
