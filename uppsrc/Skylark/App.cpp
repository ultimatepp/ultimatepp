#include "Skylark.h"

#ifdef PLATFORM_POSIX
#include <sys/wait.h>
#endif

#ifdef PLATFORM_WIN32
#include <wincon.h>
#endif

namespace Upp {

String GetThreadName()
{
	return FormatIntBase((int64)(Thread::GetCurrentId()) % 33, 35);
}
	
namespace Ini {
	INI_BOOL(skylark_log, false, "Trace of Skylark");
};

#ifdef PLATFORM_WIN32
BOOL WINAPI SkylarkApp::CtrlCHandlerRoutine(__in  DWORD dwCtrlType)
{
	LOG("Ctrl+C handler");
	TheApp().quit = true;
	Cout() << "Ctrl + C\n";
	TcpSocket h;
	h.Connect("127.0.0.1", TheApp().port);
	h.Put("quit");
	return TRUE;
}
#endif

void SkylarkApp::WorkThread()
{
	RunThread();
}

void SkylarkApp::ThreadRun()
{
	WorkThread();
}

void SkylarkApp::RunThread()
{
	if(SQL.IsOpen()) {
		SQL.ClearError();
		SQLR.ClearError();
		SQL.GetSession().ThrowOnError();
		SQLR.GetSession().ThrowOnError();
	}
	
	for(;;) {
		TcpSocket request;
		accept_mutex.Enter();
		if(quit) {
			accept_mutex.Leave();
			break;
		}
		SKYLARKLOG("Waiting for accept ");
		bool b = request.Accept(server);
		accept_mutex.Leave();
		if(quit)
			break;
		if(b) {
			SKYLARKLOG("Accepted ");
		#ifdef PLATFORM_POSIX
			if(prefork)
				alarm(timeout);
		#endif
			Http http(*this);
			http.Dispatch(request);
		#ifdef PLATFORM_POSIX
			if(prefork)
				alarm(0);
		#endif
			SKYLARKLOG("Finished ");
		}
		else {
			SKYLARKLOG("Accept failed: " << request.GetErrorDesc());
		#ifdef _DEBUG 
			break;
		#endif
		}
	}
}

void SkylarkApp::Quit()
{
	quit = true;
#ifdef PLATFORM_POSIX
	Broadcast(SIGTERM);
#endif
	TcpSocket s;
	s.Timeout(100);
	s.Connect("127.0.0.1", port);
}

void SkylarkApp::Main()
{
	Buffer<Thread> uwt(threads);
	for(int i = 0; i < threads; i++)
		uwt[i].Run(THISBACK(ThreadRun));
	
	/* Wait for threads to shut down */
	for (int i = 0; i < threads; i++)
		uwt[i].Wait();
}

void SkylarkApp::Broadcast(int signal)
{
#ifdef PLATFORM_POSIX
	if(getpid() == main_pid)
		for(int i = 0; i < child_pid.GetCount(); i++)
			kill(child_pid[i], signal);	
#endif
}

void SkylarkApp::Signal(int signal)
{
#ifdef PLATFORM_POSIX
	switch(signal) {
	case SIGTERM:
	case SIGHUP:
		quit = true;
		Broadcast(signal);
		break;
	case SIGINT:
		Broadcast(signal);
		exit(0);
		break;
	case SIGUSR1:
		Broadcast(signal);
		SigUsr1();
		break;
	case SIGALRM:
		if(getpid() != TheApp().main_pid) {
			// "Timeout - session stoped"
			exit(0);
		}
		break;
	}
#endif
}

void SkylarkApp::SignalHandler(int signal) 
{
	TheApp().Signal(signal);
}

void DisableHUP()
{
#ifdef PLATFORM_POSIX
	sigset_t mask;
	sigemptyset(&mask);
	sigaddset(&mask, SIGHUP);
	sigprocmask(SIG_BLOCK, &mask, NULL);
#endif
}

void EnableHUP()
{
#ifdef PLATFORM_POSIX
	sigset_t mask;
	sigemptyset(&mask);
	sigaddset(&mask, SIGHUP);
	sigprocmask(SIG_UNBLOCK, &mask, NULL);
#endif
}

void SkylarkApp::Run()
{
//	DisableHUP();
	if(static_dir.GetCount() == 0)
		static_dir = root.IsEmpty() ? "static" : (root + "/static");

	SqlSession::PerThread();
	SqlId::UseQuotes();
	FinalizeViews();

#ifdef PLATFORM_WIN32
	SetConsoleCtrlHandler(CtrlCHandlerRoutine, true);
#endif

	main_pid = getpid();
	quit = false;
	
	IpAddrInfo ipinfo;
	ipinfo.Execute(ip, port, IpAddrInfo::FAMILY_IPV4);
	
#ifdef _DEBUG
	int qq = 0;
	for(;;) {
		if(server.Listen(ipinfo, port, 5, false, true))
			break;
		Cout() << "Trying to start listening (other process using the same port?) " << ++qq << "\n";
		Sleep(1000);
	}
#else
	if(!server.Listen(ipinfo, port, 5, false, true)) {
		SKYLARKLOG("Cannot open server socket for listening!");
		Exit(1);
	}
#endif

	SKYLARKLOG("Starting Skylark, current static path: " << path);

#ifdef PLATFORM_POSIX
	struct sigaction sa;
	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = SignalHandler;
	sigaction(SIGUSR1, &sa, NULL);
	if(prefork) {
		sigaction(SIGTERM, &sa, NULL);
		sigaction(SIGINT, &sa, NULL);
		sigaction(SIGHUP, &sa, NULL);
		sigaction(SIGALRM, &sa, NULL);
//		EnableHUP();
		while(!quit) {
			while(child_pid.GetCount() < prefork && !quit) {
				pid_t p = fork();
				if(p == 0) {
					Main();	
					return;
				}
				else
				if(p > 0)
					child_pid.Add(p);
				else {
					// "cant create new process"
					Broadcast(SIGINT);
					abort();
				}
			}
			int status = 0;
			pid_t p = wait(&status);
			if(p > 0) {
				int q = FindIndex(child_pid, p);
				if(q >= 0)
					child_pid.Remove(q);
			}
		}
	
		Broadcast(SIGTERM);
		int status = 0;
		for(int i = 0; i < child_pid.GetCount(); i++)
			waitpid(child_pid[i], &status, 0);	
		// "server stopped";
	}
	else
#endif
		Main();

	SKYLARKLOG("ExitSkylark");
}

void SkylarkApp::SigUsr1()
{
}

void SkylarkApp::TemplateError(Http& http, const TemplateExc& e)
{
	InternalError(http, Exc());
}

SkylarkApp *SkylarkApp::app;

SkylarkApp& SkylarkApp::TheApp()
{
	ASSERT(app);
	return *app;
}

const SkylarkConfig& SkylarkApp::Config()
{
	ASSERT(app);
	return *app;
}

namespace Ini {
INI_INT(port, 8001, "TCP/IP server port to listen on");
INI_STRING(ip, "0.0.0.0", "IP address to listen on");
INI_STRING(path, Nvl(GetEnv("UPP_ASSEMBLY__"), GetFileFolder(GetExeFilePath())), "Path to witz templates and static files");
INI_INT(threads, 3 * CPU_Cores() + 1, "Number of threads in each Skylark subprocess");
#ifdef _DEBUG
INI_INT(prefork, 0, "Number of preforked Skylark subprocesses");
INI_INT(timeout, 0, "Timeout in seconds for Skylark event handler");
INI_BOOL(use_caching, false, "Cache compiled witz templates or other data");
INI_INT(caching, 1, "Agressivity of caching on static/** "
                    "(0=no caching, 1=use ETag header, 2=use versioned path)");
#else
INI_INT(prefork, 1, "Number of preforked Skylark subprocesses");
INI_INT(timeout, 300, "Timeout in seconds for Skylark event handler");
INI_BOOL(use_caching, true, "Cache compiled witz templates or other data");
INI_INT(caching, 1, "Agressivity of caching on static/** "
                    "(0=no caching, 1=use ETag header, 2=use versioned path)");
#endif
};

SkylarkApp::SkylarkApp()
{
	ASSERT(!app);
	path = Ini::path;
	app = this;
	threads = Ini::threads;
	port = Ini::port;
	ip = Ini::ip;
	use_caching = Ini::use_caching;
	prefork = Ini::prefork;
	timeout = Ini::timeout;
	caching = Ini::caching;
}

SkylarkApp::~SkylarkApp()
{
	app = NULL;
}

};