#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	DUMP(sizeof(int *));
	DUMP(sizeof(int));
	DUMP(sizeof(dword));
	DUMP(sizeof(int64));

	int64 a = 1;

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
}
