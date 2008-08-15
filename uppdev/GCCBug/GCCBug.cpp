#include "GCCBug.h"

int main(int argc, char argv[])
{
	Bar b;
	b.DoTest(0, 1);
	return 0;
}
