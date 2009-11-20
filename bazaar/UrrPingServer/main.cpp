#include <Urr/Urr.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	UrrServer urr;
	urr.Create(7593);
	Cout() << "URR Ping server\n";
	for(;;) {
		UrrRequest r;
		if(urr.Accept(r)) {
			int q = atoi(~r);
			String response = AsString(sqrt((double)q));
			Cout() << "request: " << q << ", response: " << response << '\n';
			r.Return(response);
		}
	}
}
