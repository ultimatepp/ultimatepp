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

	code = LoadFile(GetDataFile("test2.esc"));

	try {
		Scan(global, code);
		EscValue data_ptr;
		for(int i = 0; i < 100; i++)
		{
			RTIMING("run test 2");
			data_ptr = Execute(global, "test", INT_MAX);
		}
		RDUMP(data_ptr);
	}
	catch(CParser::Error e) {
		RLOG(e);
	}

	RLOG("");
	RLOG("-----------------------");
}
