#include <Core/Core.h>

using namespace UPP;

#define ITEM_COUNT 1000000

CONSOLE_APP_MAIN {
	Vector<Value> v;
	getchar();
	for(int i=0;i<ITEM_COUNT;i++) v.Add((int)i);
	getchar();
	v.Clear(); v.Shrink();
	getchar();
	for(int i=0;i<ITEM_COUNT;i++) v.Add((int)i);
	getchar();
	v.Clear(); v.Shrink();
	getchar();
}
