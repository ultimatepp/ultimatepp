#include "SvoValue.h"

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

// temporary
	Value ist = Opt0();
	if(ist.Is<int>())
		RLOG("int");
	if(IsNumber(ist))
		RLOG("number");
//


	Value v;
	ASSERT(v.IsVoid());

	CheckType(Value());
	CheckType(String("ahoj"), true);
	CheckType(String('a', 20), true);
	CheckType(String('b', 200), true);
	CheckType(WString("ahoj"));
	CheckType(123);
	CheckType(123.0);
	CheckType(true);
	CheckType((int64)123, true);
	CheckType(Date(2012, 1, 1));
	CheckType(Time(2012, 1, 1, 15, 30), true);
	CheckType(Complex(1, 2), true);
	ValueArray va;
	va.Add(123);
	va.Add("ahoj");
	ASSERT(va.GetCount() == 2);
	ASSERT(va[0] == 123);
	ASSERT(va[1] == "ahoj");
	CheckType(va, true);
	ValueMap map;
	map.Add("1", 1);
	map.Add("2", 2);
	CheckType(map, true);
	CheckType(Point(20, 20), true);
	CheckType(Size(20, 20), true);
	CheckType(Rect(0, 0, 123, 123), true);
	CheckType(Point64(20, 20), true);
	CheckType(Size64(20, 20), true);
	CheckType(Rect64(0, 0, 123, 123), true);
	CheckType(Pointf(20, 20), true);
	CheckType(Sizef(20, 20), true);
	CheckType(Rectf(0, 0, 123, 123), true);
	CheckType(Blue(), true);
	Uuid uuid;
	uuid.v[0] = MAKEQWORD(11234, 321);
	uuid.v[1] = MAKEQWORD(1111, 19999);
	CheckType(uuid, true);
	CheckType(Arial(50).Bold().Strikeout(), true);

	{
		DrawingDraw g(100, 100);
		g.DrawText(0, 0, "Hello world");
		Drawing x = g.GetResult();
		CheckType(x);
	}

	{
		PaintingPainter g(100, 100);
		g.DrawText(0, 0, "Hello world");
		Painting x = g.GetResult();
		CheckType(x);
	}

	{
		CheckType(CreateImage(Size(20, 20), Blue));
	}
	
	v = ErrorValue("error");
	CheckType(v, true);
	ASSERT(v.IsVoid());
	ASSERT(v.IsError());
	ASSERT(!Value().IsError());
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
	RLOG("CheckValue");
	CheckRawValue();

	RLOG("------------------------------");
	RLOG("CheckRawValue");
	CheckRawValue();

	RLOG("------------------------------");
	RLOG("CheckRawPickValue");
	CheckRawPickValue();

	RLOG("------------------------------");
	RLOG("CheckCreateRawValue");
	CheckCreateRawValue();
		
	RLOG("------------------------------");
	RLOG("OtherChecks");
	OtherChecks();	
	
	RLOG("------------------------------");
	RLOG("######## Everything is OK");
}
