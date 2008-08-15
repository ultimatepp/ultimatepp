#include "Esc/Esc.h"

void TestEscValue()
	{
		EscValue x("0123456789");
		DUMP(x);
		x.Replace(3, 3, x);
		DUMP(x);
		DUMP(x.ArrayGet(5, 3));
		x.Replace(1, 0, x.ArrayGet(5, 3));
		DUMP(x);
		DUMP(x.GetCount());

		x.ArraySet(1231231231, 12);

		DUMP(EscValue::GetTotalCount());

		x.SetEmptyMap();
		DUMP(x.GetCount());
		x.MapSet("ahoj", 123);
		DUMP(x.GetCount());
		x.MapSet("kamo", 123);
		DUMP(x);
		DUMP(x.GetCount());
		x.MapSet("kamo", 124);
		DUMP(x);
		DUMP(x.GetCount());
		x.MapSet("ahoj", EscValue());
		DUMP(x);
		DUMP(x.GetCount());

		DUMP(EscValue::GetTotalCount());


		x = "";
		for(int i = 0; i < 100; i++)
			x.ArraySet(x.GetCount(), i);
		DUMP(x);

		DUMP(EscValue::GetTotalCount());

		Vector<String> k;
		for(int i = 0; i < 10000; i++)
			k.Add(AsString(i));

		{
			EscValue x;
			RTIMING("map");
			for(int i = 0; i < 1000; i++)
				x.MapSet(k[i], i);
			DUMP(EscValue::GetTotalCount());
		}
		DUMP(EscValue::GetTotalCount());

		{
			EscValue x = "";
			RTIMING("array add");
			for(int i = 0; i < 100000; i++)
				x.ArraySet(x.GetCount(), i);
			DUMP(EscValue::GetTotalCount());
		}
		DUMP(EscValue::GetTotalCount());
	}


void SIC_Print(EscEscape& e)
{
	RTIMING("print");
	if(e[0].IsArray())
		VppLog() << (String) e[0];
	else
	if(e[0].IsNumber())
		VppLog() << e[0].GetNumber();
	else
	if(!e[0].IsVoid())
		e.ThrowError("invalid argument to 'print'");
}

void SIC_LOG(EscEscape& e)
{
	VppLog() << e[0].ToString() << '\n';
}

struct Test : public EscHandle {
	void Print(EscEscape&)  { LOG("Test::Print"); }
	void Print2(EscEscape&) { LOG("Test::Print2"); }

	typedef Test CLASSNAME;

	Test(EscValue& v) {
		LOG("Test created");
		v.Escape("Print()", this, THISBACK(Print));
		v.Escape("Print2()", this, THISBACK(Print2));
	}
	~Test()  { LOG("Test destroyed"); }
};


void SIC_OpenTest(EscEscape& e)
{
	new Test(e.ret_val);
}

void Test::Print2(EscEscape&) {}

void Test::Print(EscEscape&)
{
}

Test::Test(EscValue& v)
{
}

~ Test::Test()
{
}


~Test::Test()
{
}

~Test::Test()
{
}

CONSOLE_APP_MAIN
{
	String code = LoadFile(GetDataFile("test.esc"));
	ArrayMap<String, EscValue> global;

	Escape(global, "print(x)", SIC_Print);
	Escape(global, "LOG(x)", SIC_LOG);
//	Escape(global, "dump_locals()", SIC_DumpLocals);
//	Escape(global, "OpenTest()", SIC_OpenTest);
	StdLib(global);

	try {
		RTIMING("run");
		Scan(global, code);
		Execute(global, "main", INT_MAX);
	}
	catch(CParser::Error e) {
		RLOG(e);
	}

	RLOG("");
	RLOG("-----------------------");

	DUMP(global.GetAdd("result"));

//	EscValue v;
//	v.SetArray().Add(123);
//	EscValue v2 = v;
//	v.SetArray().Add(v);
//	DUMP(v);
}
