#include <Esc/Esc.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	RDUMP(sizeof(EscValue));

	String code = LoadFile(GetDataFile("test.esc"));
	ArrayMap<String, EscValue> global;

	StdLib(global);

	try {
		Scan(global, code);
		EscValue primes;
		for(int i = 0; i < 100; i++)
		{
			RTIMING("run");
			primes = Execute(global, "sieve", INT_MAX);
		}
		RDUMP(primes);
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
