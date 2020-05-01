#include <Core/Core.h>

using namespace Upp;

struct Foo {
	byte   b = Random();
	int16  i16 = Random();
	int32  i32 = Random();
	int64  i64 = Random64();
	float  f = Randomf();
	double d = Randomf();
	String s = AsString(Randomf());
	int    v = Random(500);
	
	bool operator==(const Foo& x) {
		return b = x.b && i16 == x.i16 && i32 == x.i32 && i64 == x.i64 && f == x.f && d == x.d
		&& s == x.s && v == x.v;
	}
	
	void Serialize(Stream& st) {
		st % b % i16 % i32 % i64 % f % d % s / v;
	}
};

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	for(int i = 0; i < 10; i++) {
		Foo a;
		String h = StoreAsString(a);
		Foo b;
		LoadFromString(b, h);
		ASSERT(a == b);
		ASSERT(StoreAsString(b) == h);
	}
	LOG("=========== OK");
}
