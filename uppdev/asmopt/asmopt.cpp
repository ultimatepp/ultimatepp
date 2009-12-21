#include <Core/Core.h>

using namespace Upp;

int Count(const Vector<String>& v, const String& x)
{
	int count = 0;
	for(int i = 0; i < v.GetCount(); i++)
		if(v[i] == x)
			count++;
	return count;
}


CONSOLE_APP_MAIN
{
	Vector<String> x;
	for(int i = 0; i < 1000; i++)
		x.Add(AsString(Random(100)));
	Cout() << Count(x, "50");
}
