#include "GCCBug.h"

void Bar::DoTest(int i, int count)
{
	li.InsertN(minmax(i, 0, li.GetCount()), my_max(count, 0));
}
