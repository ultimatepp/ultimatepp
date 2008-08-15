#include <Core/Core.h>

#define Union NewUnion

template <class T>
int StaticTypeNo() {
	static int typeno = -1;
	if(typeno < 0)
		typeno = RegisterTypeNo(typeid(T).name());
	return typeno;
}

CONSOLE_APP_MAIN
{
	Union test;
	test.Create<String>();
	test.Get<String>() = "ahoj";
	test.Is<String>();
	__BREAK__;
	test.Is<String>();
	__BREAK__;
	DUMP(test.Is<String>());
	DUMP(test.Is<int>());
	DUMP(test.Get<String>());
	DUMP((String)test);
	DUMP(StaticTypeNo<int>());
	DUMP(StaticTypeNo<Value>());
	DUMP(StaticTypeNo<Stream>());
	DUMP(StaticTypeNo<FileIn>());
	DUMP(StaticTypeNo<int>());
	DUMP(StaticTypeNo<Value>());
	DUMP(StaticTypeNo<Stream>());
	DUMP(StaticTypeNo<FileIn>());
}
