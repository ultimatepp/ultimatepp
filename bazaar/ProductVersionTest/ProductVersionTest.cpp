#include <Core/Core.h>

#include <ProductVersion/ProductVersion.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	ProductVersion v(1, 2, 3);
	ProductVersion v2 = v;
	v2.IncDevel();
	DUMP(v < v2);
}
