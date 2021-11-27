#include <Core/Core.h>

using namespace Upp;

bool print = 0;
bool nocheck = 0;

void Test(double x)
{
	if(print) {
		RLOG("-----------------");
		RLOG(Sprintf("%.17g", x));
	}
	String s, f;
	auto dump = [&](const char *fmt, String f, bool check) {
		String s = Sprintf(fmt, x);
		if(print) {
			RLOG("---- " << fmt);
			RLOG("s = " << s);
			RLOG("f = " << f);
		}
		if(check && !nocheck) {
			s.TrimEnd("(ind)");
			ASSERT(s == f);
		}
	};
	auto dumpf = [&](int p) {
		dump("%." + AsString(p) + "f", FormatF(x, p), false);
	};
	auto dumpg = [&](int p) {
		dump("%." + AsString(p) + "g", FormatG(x, p), p < 19);
	};
	auto dumpe = [&](int p) {
		dump("%." + AsString(p) + "e", FormatE(x, p), p < 18);
	};
	for(int i = 0; i < 20; i++) {
		dumpf(i);
		dumpg(i);
		dumpe(i);
	}
	dumpf(30);
	dumpg(30);
	dumpe(30);
	
	if(std::isfinite(x)) {
		RDUMP(x);
		RDUMP(FormatG(x, 17));
		RDUMPHEX(FormatG(x, 17));
		RDUMP(Atof(FormatG(x, 17)));
		ASSERT(Atof(FormatG(x, 17)) == x);
	}
}

void SubnormalTest()
{
	double x = 1e-300;
	do {
		x = x * 0.5;
		Test(x);
	}
	while(x);
}

void Pow2Test()
{
	for(int pass = 0; pass < 2; pass++) {
		double x = 1;
		while(x > 1e-320 && x < 1e308) {
			x = x * (pass ? 0.5 : 2);
			Test(x);
		}
	}
}

void Test10()
{
	for(int i = -350; i < 310; i++)
		Test(Atof(String() << "1e" << i));
}

void TestF()
{
	double x = double(1000000000000000000 - 1);
	while(x > 0.000000000000000001) {
		if(print) {
			RLOG("=================");
			RLOG(Sprintf("%.17g", x));
		}
		for(int p = 0; p < 19; p++) {
			String fmt = "%." + AsString(p) + "f";
			String s = Sprintf(fmt, x);
			String f = FormatF(x, p);
			if(print) {
				RLOG("---- " << fmt);
				RLOG("s = " << s);
				RLOG("f = " << f);
			}
			if(!nocheck) {
				ASSERT(atof(s) == atof(f));
				ASSERT(Atof(s) == Atof(f));
			}
		}
		x *= 0.9;
	}
}

void QuickTest()
{
	Test(0);
	Test(-0.0);
	Test(log(-1));
	Test10();
	TestF();
	SubnormalTest();
	Pow2Test();
}

CONSOLE_APP_MAIN
{
	QuickTest();
	DLOG("============== OK");
}
