#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	for(double x : { 1e200, 1e30, 1123123123., 123123.123123123, 0.123, 0.0, 0.000123, 1.234e-9, 1.234234234e-10, exp(10000), -exp(10000), log(-1) }) {
		for(int pass = 0; pass < 2; pass++) {
			DLOG("====================");
			DDUMP(Sprintf("%.17g", x));

			DDUMP(Format("%g", x));


			DDUMP(Format("%m", x));
			DDUMP(Format("%M", x));
			DDUMP(Format("%mE", x));
			DDUMP(Format("%mf", x));

			DDUMP(Format("%+m", x));
			DDUMP(Format("%,m", x));
			DDUMP(Format("%!m", x));
			DDUMP(Format("%?m", x));
			DDUMP(Format("%_m", x));
			DDUMP(Format("%^m", x));
			DDUMP(Format("%&m", x));
			DDUMP(Format("%#m", x));

			DDUMP(Format("%+mf", x));
			DDUMP(Format("%,mf", x));
			DDUMP(Format("%!mf", x));
			DDUMP(Format("%?mf", x));
			DDUMP(Format("%_mf", x));
			DDUMP(Format("%^mf", x));
			DDUMP(Format("%&mf", x));
			DDUMP(Format("%#mf", x));

			DDUMP(Format("%+me", x));
			DDUMP(Format("%,me", x));
			DDUMP(Format("%!me", x));
			DDUMP(Format("%?me", x));
			DDUMP(Format("%_me", x));
			DDUMP(Format("%^me", x));
			DDUMP(Format("%&me", x));
			DDUMP(Format("%#me", x));

			DDUMP(Format("%.8+m", x));
			DDUMP(Format("%.8,m", x));
			DDUMP(Format("%.8!m", x));
			DDUMP(Format("%.8?m", x));
			DDUMP(Format("%.8_m", x));
			DDUMP(Format("%.8^m", x));
			DDUMP(Format("%.8&m", x));
			DDUMP(Format("%.8#m", x));

			DDUMP(Format("%.8+mf", x));
			DDUMP(Format("%.8,mf", x));
			DDUMP(Format("%.8!mf", x));
			DDUMP(Format("%.8?mf", x));
			DDUMP(Format("%.8_mf", x));
			DDUMP(Format("%.8^mf", x));
			DDUMP(Format("%.8&mf", x));
			DDUMP(Format("%.8#mf", x));

			DDUMP(Format("%.8+me", x));
			DDUMP(Format("%.8,me", x));
			DDUMP(Format("%.8!me", x));
			DDUMP(Format("%.8?me", x));
			DDUMP(Format("%.8_me", x));
			DDUMP(Format("%.8^me", x));
			DDUMP(Format("%.8&me", x));
			DDUMP(Format("%.8#me", x));

			DDUMP(Format("%020m", x));
			DDUMP(Format("%020.5m", x));
			DDUMP(Format("%+020.5m", x));
			DDUMP(Format("% 020.5m", x));
			DDUMP(Format("%-020m", x));
			x = -x;
		}
	}

#ifdef PLATFORM_BSD
	CheckLogEtalon(GetDataFile("MacEtalon.log"));
#else
	CheckLogEtalon();
#endif
}
