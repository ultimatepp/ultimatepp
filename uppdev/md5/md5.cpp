#include <Core/Core.h>
#include <Web/Web.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	String x = MD5Digest("alajos");
	LOGHEXDUMP(x, x.GetCount());
	String r;
	for(int i = 0; i < x.GetCount(); i++) {
		r << FormatIntHex((byte)x[i], 2);
	}
	DUMP(r);
}
