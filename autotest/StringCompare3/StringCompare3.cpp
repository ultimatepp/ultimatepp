#include <Core/Core.h>

using namespace Upp;

int EtalonCompare(const String& sa, const String& sb)
{
	int q = memcmp(~sa, ~sb, min(sa.GetLength(), sb.GetLength()));
	return q ? sgn(q) : SgnCompare(sa.GetLength(), sb.GetLength());
}

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	int less = 0, equal = 0;
	for(int pass = 0; pass < 4; pass++) {
		LOG("======= Pass " << pass);
		for(int q = 0; q < 10000000; q++) {
			String x[2];
			for(int ii = 0; ii < 2; ii++) {
				int n = Random(pass & 1 ? 40 : 14);
				for(int i = 0; i < n; i++)
					x[ii].Cat(Random(pass & 2 ? Random(26) + 'A' : 2));
			}
			int r1 = EtalonCompare(x[0], x[1]);
			int r2 = x[0].Compare(x[1]);
			ASSERT(r1 == r2);
			if(r1 < 0)
				less++;
			if(r1 == 0)
				equal++;
		}
		LOG("Less: " << less << ", Equal: " << equal);
	}
	
	LOG("=================== OK");
}
