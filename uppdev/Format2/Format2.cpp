#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	DDUMP(Format("|%*s|", 20, "test")); 
	DDUMP(Format("|%*s|", 400, "test")); 
	DDUMP(Format("|%*=s|", 20, "test")); 
	DDUMP(Format("|%*=s|", 400, "test")); 
	DDUMP(Format("%.*f", 2, 1.22));
}
