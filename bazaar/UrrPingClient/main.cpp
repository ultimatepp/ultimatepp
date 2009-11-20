#include <Urr/Urr.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	UrrClient urr;
	urr.SetServer(CommandLine().GetCount() ? CommandLine()[0] : "127.0.0.1", 7593);
	for(;;) {
		int tm = GetTickCount();
		String r = AsString(rand());
		Cout() << "request: " << r;
		r = urr.Call(r);
		int tm2 = GetTickCount();
		if(r.GetCount())
			Cout() << ", response " << r << " in " << tm2 - tm << "ms\n";
		else
			Cout() << ", TIMEOUT!\n";
		Sleep(1000);
	}
}
