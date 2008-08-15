#include <Core/Core.h>

CONSOLE_APP_MAIN
{
	VectorMap<int, Vector<String> > w;
	w.GetAdd(1).Add("Ahoj");
	String x = StoreAsString(w);
	LOGHEXDUMP(x, x.GetCount());
}
