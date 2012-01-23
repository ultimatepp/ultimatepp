#include <Core/Core.h>

using namespace Upp;

template <class T>
void TestRef(const T& x)
{
	LOG(typeid(T).name());
	T a;
	Ref ref(a);
	ref.SetValue(x);
	ASSERT(a == x);
	LOG(a);
	Value xx = x;
	if(!xx.Is<bool>()) {
		ref.SetNull();
		ASSERT(IsNull(a));
		ASSERT(ref.IsNull());
		ref.SetValue(Null);
		ASSERT(IsNull(a));
		ASSERT(ref.IsNull());
	}
	LOG("-------------------");
}

CONSOLE_APP_MAIN
{
	TestRef(Date(2012, 3, 4));
	TestRef(Time(2012, 3, 4, 23, 1));
	TestRef(String("hello!"));
	TestRef(WString("hello!"));
	TestRef(true);
	TestRef(123);
	TestRef(123.456);
	TestRef((int64)123);

	int x;
	Ref ref(x);
	ref.SetValue(1.0);
	DLOG(x);
	ASSERT(x == 1);
}
