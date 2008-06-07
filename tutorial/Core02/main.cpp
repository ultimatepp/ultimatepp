#include <Core/Core.h>

using namespace Upp;

void CApiFunction(char *c)
{
	strcpy(c, "Hello");
}

CONSOLE_APP_MAIN
{
	StringBuffer b;
	b.SetLength(200);
	CApiFunction(b);
	b.Strlen();
	String x = b;
	DUMP(x);

	b = x;
	b[1] = 'a';
	x = b;
	DUMP(x);

	b = x;
	b.Cat('!');
	x = b;
	DUMP(x);
}
