#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	uint64 a = Random(100), b = Random(100);
	uint64 c = a * b;
	uint64 d = b / a;
	DUMP(sizeof(int *));
	DUMP(sizeof(int));
	DUMP(sizeof(dword));
	DUMP(sizeof(int64));

/*
	for(int i = 0; i < 20; i++)
		Cout() << (a = a * 13) << '\n';

	Value x = (int64)500;
	Value y = (double)500;
	Value z = (int)500;
	DUMP((x == y));
	DUMP((y == z));
	DUMP((z == x));
	DUMP((y == x));
	DUMP((y == y));
	DUMP((x == z));
	z = (int)501;
	DUMP((x == y));
	DUMP((y == z));
	DUMP((z == x));
	DUMP((y == x));
	DUMP((y == y));
	DUMP((x == z));
	DUMP(((int)x));
	DUMP(((int)y));
	DUMP(((int)z));
	DUMP(((double)x));
	DUMP(((double)y));
	DUMP(((double)z));
	DUMP(((int64)x));
	DUMP(((int64)y));
	DUMP(((int64)z));
*/
}
