#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);
	
	VectorMap<String, String> data;
	
	data.Add("0", "zero");
	data.Add("1", "one");
	DUMP(data);
	DUMP(data.Get("0", "ZERO"));
	DUMP(data.Get("1", "ONE"));
	data.UnlinkKey("1");
	DUMP(data);
	DUMP(data.Get("0", "ZERO"));
	DUMP(data.Get("1", "ONE"));
	data.GetPut("1", "I");
	DUMP(data);
	DUMP(data.Get("0", "ZERO"));
	DUMP(data.Get("1", "ONE"));

	VectorMap<String, int> vm;
	vm.GetAdd("getadd", 1);
	vm.GetPut("getput", 2);
	DDUMP(vm);
	
	vm.Unlink(0);
	vm.GetPut("getput2", 3);
	DDUMP(vm);
	
	CheckLogEtalon();
}
