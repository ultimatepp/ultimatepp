#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	Vector<String> v;

v.Add("Str1");
v.Add("Str2");
v.Add("Str3");


for (Vector<String>::Iterator b = v.Begin(), e = v.End(); b != e; b++)
{
Cout() << "member " << " : " << *b << std::endl;
}
}

