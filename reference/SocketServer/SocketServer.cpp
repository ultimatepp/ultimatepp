#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	TcpSocket server;
	if(!server.Listen(3214, 5)) {
		Cout() << "Unable to initialize server socket!\n";
		SetExitCode(1);
		return;
	}
	Cout() << "Waiting for requests..\n";
	for(;;) {
		TcpSocket s;
		if(s.Accept(server)) {
			String w = s.GetLine();
			Cout() << "Request: " << w << " from: " << s.GetPeerAddr() << '\n';
			if(w == "time")
				s.Put(AsString(GetSysTime()));
			else
				s.Put(AsString(3 * atoi(~w)));
			s.Put("\n");
		}
	}
}
