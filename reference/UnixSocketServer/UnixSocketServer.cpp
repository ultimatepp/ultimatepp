#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	const String& path = NativePath(GetTempDirectory() + "/upp-unixsocket.sock");
	
	if(FileExists(path))
		DeleteFile(path); // "unlink"
	
	Socket server;
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
