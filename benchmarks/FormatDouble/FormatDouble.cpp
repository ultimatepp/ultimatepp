#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	double x = 1e20;
	for(int i = 0; i < 10000000; i++) {
		x = x * 0.99999;
		{
			RTIMING("FormatDouble2");
			char h[256];
			FormatDouble(h, x);
		}
		{
			RTIMING("FormatGc");
			char h[256];
			FormatG(h, x, 17);
		}
		{
			RTIMING("to_string");
			std::to_string(x);
		}
	}
	RDUMP(x);
}
