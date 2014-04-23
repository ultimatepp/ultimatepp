#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	ValueArray va;
	va << 12 << 3 << 5 << 1 << 10;
	DUMP(va);
	Vector<Value> v = va.Pick();
	Sort(v);
	va = pick(v);
	DUMP(va);
	
	ValueMap vm;
	vm(1, "world")(2, "hello");
	DUMP(vm);
	VectorMap<Value, Value> m = vm.Pick();
	SortByValue(m);
	vm = pick(m);
	DUMP(vm);
}
