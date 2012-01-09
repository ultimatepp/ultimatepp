#include "SvoValue.h"

CONSOLE_APP_MAIN
{
	Value v;
	ASSERT(v.IsVoid());

	CheckType(Value(), true);
	CheckType(String("ahoj"), false, true);
	CheckType(WString("ahoj"));
	CheckType(123);
	CheckType(123.0, false, true);
	CheckType(true);
	CheckType((int64)123);
	CheckType(Date(2012, 1, 1));
	CheckType(Time(2012, 1, 1, 15, 30), false, true);
	ValueArray va;
	va.Add(123);
	CheckType(va, false, true);
	ValueMap map;
	map.Add("1", 1);
	map.Add("2", 2);
	CheckType(map, false, true);
	CheckType(Size(20, 20), false, true);
	CheckType(Rect(0, 0, 123, 123), false, true);
	CheckType(Blue(), false, true);
	CheckType(Uuid::Create(), false, true);

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
