#include <IPNServer/IPNServer.h>

using namespace Upp;

// comment following one to have the server running as a daemon (UNIX ONLY BY NOW...
#define DEBUGME

#ifdef PLATFORM_POSIX
#ifndef DEBUGME
static bool Daemonize(void)
{
	pid_t pid, sid;

	// already a daemon
	if ( getppid() == 1 )
		return true;

	// Fork off the parent process
	pid = fork();
	if (pid < 0)
		return false;

	// If we got a good PID, then we can exit the parent process.
	if (pid > 0)
		_exit(0);

	// At this point we are executing as the child process

	// Change the file mode mask
	umask(0);

	// Create a new SID for the child process
	sid = setsid();
	if (sid < 0)
		return false;

	// Change the current working directory.  This prevents the current
	// directory from being locked; hence not being able to remove it.
	if ((chdir("/")) < 0)
		return false;

	// Redirect standard files to a log file (NULL for input)
	String logPath = "/var/log/IPNServer/";
	RealizePath(logPath);
	freopen("/dev/null", "r", stdin);
	freopen(AppendFileName(logPath, "out.log"), "w", stdout);
	freopen(AppendFileName(logPath, "err.log"), "w", stderr);
	return true;
}
#endif
#endif

void OnVerified(IPNServer *s)
{
	VectorMap<String, String> data = s->GetVerified();
	Cout() << "DUMP OF TRANSACTION DATA:\n\n";
	for(int i = 0; i < data.GetCount(); i++)
		Cout() << data.GetKey(i) << "=" << data[i] << "\n";
}

void OnInvalid(void)
{
	Cout() << "'INVALID' ANSWER FROM PayPal SERVER\n\n";
}

CONSOLE_APP_MAIN
{
#ifdef PLATFORM_POSIX
#ifndef DEBUGME
	// daemonize me
	if(!Daemonize())
	{
		Cerr() << "Error making me a daemon!\n";
		exit(1);
	}
#endif
#endif

	// create server object -- beware of port, it must match
	// SCGI port setup on your server
	IPNServer server(8789);
	
	// setup paypal confirmation server and port
	// (here the sandbox testing one, change for real usage !)
	server
		.SetPayServer("www.sandbox.paypal.com")
		.SetPayPort(443)
	;
	
	// setup handlers for VERIFIED and INVALID paypal messages
	server
		.SetVerifiedHandler(STDBACK1(OnVerified, &server))
		.SetInvalidHandler(STDBACK(OnInvalid))
	;
	
	// runs server -- TO DO : make it multithreaded
	server.Run();
}
