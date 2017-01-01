#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	Complex a(3, 4);
	
	DUMP(a);
	DUMP(a.imag());
	DUMP(a.real());
	DUMP(abs(a));
	DUMP(a + a);
	DUMP(2.0 + a);
	DUMP(2.0 * a);
	DUMP(a * a);
}
