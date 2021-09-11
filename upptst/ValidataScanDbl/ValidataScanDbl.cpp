#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);
	int tm0 = msecs();
	uint64 count = 0;
	for(;;) {
		double x;
		uint64 n;
		n = Random64();
		memcpy(&x, &n, 8);
		if(std::isfinite(x)) {
			count++;
			double y = ScanDouble(FormatG(x, 17));
			if(x != y) {
				RLOG("ERROR");
				RDUMP(Sprintf("%.30g", x));
				RDUMP(Sprintf("%.30g", y));
				RDUMP(x);
				RDUMP(y);
				RDUMP(count);
				Exit(1);
			}
			if(msecs() - tm0 > 1000) {
				RLOG(Sprintf("%.6f billions", count * 1e-9));
				tm0 = msecs();
			}
		}
	}
}
