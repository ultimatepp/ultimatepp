#include <Core/Core.h>

using namespace Upp;

String GetSocketPath()
{
	String temp;
#ifdef PLATFORM_WIN32
	temp = GetEnv("TEMP");
#else
	temp = GetTempPath();
#endif
	return AppendFileName(temp, "upp-unixsocket.socket");
}

CONSOLE_APP_MAIN
{
	Socket server;
	String path = GetSocketPath();
	DeleteFile(path); // "unlink" existing file system socket
	if(!server.ListenFileSystem(path, 5, false)) { // Reuse option is not available on Windows
		Cout() << "Unable to initialize server socket!\n";
		SetExitCode(1);
		return;
	}
	Cout() << "Waiting for requests..\n";
	for(;;) {
		Socket s;
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
}
