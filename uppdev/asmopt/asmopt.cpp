#include <Core/Core.h>

using namespace Upp;

int Count(const Vector<String>& v, const String& x);

CONSOLE_APP_MAIN
{
	Vector<String> x;
	for(int i = 0; i < 1000; i++)
		x.Add(AsString(Random(100)));
	Cout() << Count(x, "50");
}
