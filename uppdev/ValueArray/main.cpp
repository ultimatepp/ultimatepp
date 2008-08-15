#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	Value v = 1;
	String s = v;
	DUMP(v.Is<ValueArray>());
	DUMP(v.Is<int>());

	DUMP(v == 1);
	DUMP(v == 1.1);
	DUMP(v == White());

	v = White();
	DUMP(v == White());
	DUMP(v == Blue());
//	DUMP(v == 1);

	Color c = Black();
//	Ref x = c;
//	x = Value(Blue());
	DUMP(c);
}
