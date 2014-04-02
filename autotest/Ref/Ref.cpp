#include <Draw/Draw.h>
#include <Painter/Painter.h>

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

		ref.Get<T>() = x;
		ASSERT(!IsNull(a));
		ASSERT(!ref.IsNull());

		ref.SetValue(Null);
		ASSERT(IsNull(a));
		ASSERT(ref.IsNull());

		ref.Get<T>() = x;
		ASSERT(!IsNull(a));
		
		ref = Null;
		ASSERT(IsNull(a));
		ASSERT(ref.IsNull());

		ref.Get<T>() = x;
		ASSERT(!IsNull(a));

		ref = (Value)Null;
		ASSERT(IsNull(a));
		ASSERT(ref.IsNull());
	}

	DUMP(ref.GetType());
	DUMP(GetValueTypeNo<T>());
	ASSERT(ref.GetType() == GetValueTypeNo<T>());
	
	ASSERT(ref.Is<T>());
	
	T b;
	Ref r2(b);
	T& f = r2.Get<T>();
	f = x;
	ASSERT(x == b);

	r2.Get<T>() = x;
	ASSERT(x == b);
	
	LOG("-------------------");
}

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	TestRef(Date(2012, 3, 4));
	TestRef(Time(2012, 3, 4, 23, 1));
	TestRef(String("hello!"));
	TestRef(WString("hello!"));
	TestRef(true);
	TestRef(123);
	TestRef(123.456);
	TestRef((int64)123);
	
	TestRef(Value("hello Value!"));

	TestRef(ValueMap()("key1", 1)("key2", 2));

	TestRef(ValueArray() << "v1" << "v2");
	
	TestRef(Blue());
	
	TestRef(Rect(1, 2, 3, 4));
	TestRef(Rectf(1, 2, 3, 4));

	TestRef(Point(1, 4));
	TestRef(Pointf(1, 4));

	TestRef(Size(1, 4));
	TestRef(Sizef(1, 4));
	
	TestRef(Complex(1, 2));

	TestRef(CreateImage(Size(1, 2), Blue()));

	TestRef(Arial(20).Bold());
	
	DrawingDraw w(20, 20);
	w.DrawRect(1, 1, 1, 1, Black());
	TestRef(w.GetResult());
	
	PaintingPainter p(20, 20);
	p.DrawRect(1, 1, 1, 1, Black());
	TestRef(p.GetResult());

	String text;
	Ref textref(text);
	textref = "TEST";
	DUMP(text);
	ASSERT(text == "TEST");

	int x;
	Ref ref(x);
	ref.SetValue(1.0);
	DUMP(x);
	ASSERT(x == 1);
	
	Ref empty;
	DUMP(empty.GetValue());
	DUMP(empty.IsNull());
	DUMP(IsNull(empty.GetValue()));
	DUMP(~empty);
	
	LOG("--- OK: All tests finished successfully");
}
