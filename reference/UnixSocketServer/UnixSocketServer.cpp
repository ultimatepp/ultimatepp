#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
#ifdef PLATFORM_POSIX
	const String& path = "/tmp/upp-unixsocket.sock";
	
	UnixSocket server;
	if(!server.Listen(path, 5)) {
		Cout() << "Unable to initialize server socket!\n";
		SetExitCode(1);
		return;
	}
	Cout() << "Waiting for requests..\n";
	for(;;) {
		UnixSocket s;
		if(s.Accept(server)) {
			String w = s.GetLine();
			Cout() << "Request: " << w << " from: " << s.GetPeerPid() << '\n';
			if(w == "time")
				s.Put(AsString(GetSysTime()));
			else
				s.Put(AsString(3 * atoi(~w)));
			s.Put("\n");
		}
	}
#else
	Cout() << "This example requires a POSIX compliant operating system...\r\n"
	SetExitCode(1);
#endif
}
