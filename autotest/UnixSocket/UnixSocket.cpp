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
	StdLogSetup(LOG_COUT|LOG_FILE);

	String path = GetSocketPath();

	try {
		Socket server, client;

		DeleteFile(path); // "unlink" existing FS socket if possible
		
		// Test server listen
		if(!server.ListenFileSystem(path, 5, false)) {
			throw Exc("Server listen failed: " << server.GetErrorDesc());
		}
		
		// Test client connect
		if(!client.ConnectFileSystem(path)) {
			throw Exc("Client connect failed: " << client.GetErrorDesc());
		}
		
		// Test data exchange
		String test_data = "Hello, world!";
		client.Put(test_data + "\n");
		
		Socket accepted;
		if(!accepted.Accept(server)) {
			throw Exc("Accept failed: " << accepted.GetErrorDesc());
		}
		
		String received = accepted.GetLine();
		DUMP(received);
		
		ASSERT(received == test_data);
		
		// Test peer PID (on supported platforms)
		int pid = accepted.GetPeerPid();
		DUMP(pid);
		if(pid != -1)
			ASSERT(pid == getpid()); // Should be our own process in this test

		LOG("=========== OK");
	}
	catch(const Exc& e)
	{
		LOG(e);
		SetExitCode(1);
	}
}
