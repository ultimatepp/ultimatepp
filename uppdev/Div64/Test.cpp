#include <Core/Core.h>

using namespace Upp;

int IScale(int x, int y, int z)
{
	int64 res = x;
	res *= y;
	res /= z;
	return (int)res;
}
