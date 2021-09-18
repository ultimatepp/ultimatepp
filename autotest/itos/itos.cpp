#include <Core/Core.h>

using namespace Upp;

void Test(int64 x)
{
	for(int pass = 0; pass < 2; pass++) {
		ASSERT(AsString((int16)x) == String(std::to_string((int16)x)));
		ASSERT(AsString((uint16)x) == String(std::to_string((uint16)x)));
		if(IsNull((int)x))
			ASSERT(AsString((int)x).GetCount() == 0);
		else
			ASSERT(AsString((int32)x) == String(std::to_string((int32)x)));
		ASSERT(AsString((uint32)x) == String(std::to_string((uint32)x)));
		if(IsNull((int64)x))
			ASSERT(AsString((int64)x).GetCount() == 0);
		else
			ASSERT(AsString((int64)x) == String(std::to_string((int64)x)));
		ASSERT(AsString((uint64)x) == String(std::to_string((uint64)x)));
		x = -x;
	}
}

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

/*	
	DDUMP(AsString((uint64)-2));
	DDUMP(String(std::to_string((uint64)-2)));
	return;
*/	
	for(int q = 2; q < 10; q++) {
		uint64 qq = q;
		for(int i = 0; i < 64; i++) {
			DUMP(qq);
			Test(qq);
			qq += qq;
		}
	}
	for(int q = 2; q < 10; q++) {
		uint64 qq = q;
		for(int i = 0; i < 64; i++) {
			DUMP(qq);
			Test(qq);
			qq += qq - 1;
		}
	}
	for(int i = 0; i < 1000000; i++)
		Test(Random());

	Test((int)Null);
	Test((int64)Null);
	
	LOG("================ OK");
}
