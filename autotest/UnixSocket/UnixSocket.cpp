#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
#ifdef PLATFORM_POSIX

	StdLogSetup(LOG_COUT|LOG_FILE);

	String path = Format("/tmp/upp-unixsocket-test-%d", getpid());
	
	Socket server, client;
	
	// Test server listen
	if(!server.ListenFileSystem(path)) {
		LOG("Server listen failed: " << server.GetErrorDesc());
		Exit(1);
	}
	
	// Test client connect
	if(!client.ConnectFileSystem(path)) {
		LOG("Client connect failed: " << client.GetErrorDesc());
		Exit(1);
	}
	
	// Test data exchange
	String test_data = "Hello, world!";
	client.Put(test_data + "\n");
	
	Socket accepted;
	if(!accepted.Accept(server)) {
		LOG("Accept failed: " << accepted.GetErrorDesc());
		Exit(1);
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

#endif
}