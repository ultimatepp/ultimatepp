#include "SvoValue.h"

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	Value ist = Opt0();
	if(ist.Is<int>())
		RLOG("int");
	if(IsNumber(ist))
		RLOG("number");

	Value v;
	ASSERT(v.IsVoid());

	CheckType(Value(), true);
	CheckType(String("ahoj"), false, true);
	CheckType(String('a', 20), false, true);
	CheckType(String('b', 200), false, true);
	CheckType(WString("ahoj"));
	CheckType(123);
	CheckType(123.0, false, true);
	CheckType(true);
	CheckType((int64)123);
	CheckType(Date(2012, 1, 1));
	CheckType(Time(2012, 1, 1, 15, 30), false, true);
	ValueArray va;
	va.Add(123);
	va.Add("ahoj");
	ASSERT(va.GetCount() == 2);
	ASSERT(va[0] == 123);
	ASSERT(va[1] == "ahoj");
	CheckType(va, false, true);
	ValueMap map;
	map.Add("1", 1);
	map.Add("2", 2);
	CheckType(map, false, true);
	CheckType(Point(20, 20), false, true);
	CheckType(Size(20, 20), false, true);
	CheckType(Rect(0, 0, 123, 123), false, true);
	CheckType(Pointf(20, 20), false, true);
	CheckType(Sizef(20, 20), false, true);
	CheckType(Rectf(0, 0, 123, 123), false, true);
	CheckType(Blue(), false, true);
	Uuid uuid;
	uuid.a = 11234;
	uuid.b = 321;
	uuid.c = 1111;
	uuid.d = 19999;
	CheckType(uuid, false, true);

	v = ErrorValue("error");
	CheckType(v, true);
	ASSERT(v.IsVoid());
	ASSERT(v.IsError());
	RDUMP(GetErrorText(v));
	ASSERT(GetErrorText(v) == "error");

	CheckValueMap();

	CheckNumber<int>();
	CheckNumber<double>();
	CheckNumber<int64>();
	CheckNumber<bool>();

	RLOG("------------------------------");
	RLOG("CheckString");
	CheckString();

	RLOG("------------------------------");
	RLOG("CheckDateTime");
	CheckDateTime();

	RLOG("------------------------------");
	RLOG("OtherChecks");
	OtherChecks();	
	
	RLOG("------------------------------");
	RLOG("######## Everything is OK");
}
