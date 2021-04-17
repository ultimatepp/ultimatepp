#include <Core/Core.h>

using namespace Upp;

struct StreamCopyFilter { // Simple filter example just prints amount of data processed
	Event<const void *, int> WhenOut;
	void Put(const void *ptr, int size) {
		WhenOut(ptr, size);
	}
	void End() {}
};

CONSOLE_APP_MAIN
{
	String data;
	for(int i = 0; i < 20000000; i++)
		data << i << "\n";

	for(int pass = 0; pass < 2; pass++) {
		StringStream fin(data);
		StreamCopyFilter pf;
		InFilterStream in(fin, pf);
		String data2;
		SeedRandom();
		while(!in.IsEof()) {
			ASSERT(data2.GetCount() == in.GetPos());
			data2.Cat(in.Get(Random(pass ? 20 : 100000)));
		}
		ASSERT(data == data2);
	}
	
	LOG("============= OK");
}
