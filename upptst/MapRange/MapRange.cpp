#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	VectorMap<String, String> h;
	for(int i = 0; i < 20; i++)
		h("key" + AsString(i), "value" + AsString(i));
	
	for(auto kv : ~h) {
		DDUMP(kv.key);
		DDUMP(kv.value);
	}
	
	DLOG("=========");
	
	h.Unlink(11);

	for(auto kv : ~h) {
		DDUMP(kv.key);
		DDUMP(kv.value);
		kv.value = "123";
	}

	DLOG("=========");

	const VectorMap<String, String>& ch = h;
	for(auto kv : ~ch) {
		DDUMP(kv.key);
		DDUMP(kv.value);
	}

	DLOG("=========");
	for(int i = 0; i < h.GetCount(); i++)
		if(!h.IsUnlinked(i))
			h.Unlink(i);

	for(auto kv : ~h) {
		DDUMP(kv.key);
		DDUMP(kv.value);
	}
}
