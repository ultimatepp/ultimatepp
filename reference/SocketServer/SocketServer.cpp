#include <Web/Web.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	Socket server;
	if(!ServerSocket(server, 3214)) {
		Cout() << "Unable to initialize server socket!\n";
		SetExitCode(1);
		return;
	}
	Cout() << "Waiting for requests..\n";
	for(;;) {
		Socket s;
		if(server.Accept(s)) {
			String w = s.ReadUntil('\n');
			Cout() << "Request: " << w << " from: " << s.GetPeerName() << '\n';
			if(w == "time")
				s.Write(AsString(GetSysTime()));
			else
				s.Write(AsString(3 * atoi(~w)));
			s.Write("\n");
		}
	}
}
