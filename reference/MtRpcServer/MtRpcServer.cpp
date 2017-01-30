#include <Core/Core.h>
#include <Core/Rpc/Rpc.h>
#ifdef PLATFORM_WIN32
#include <wincon.h>
#endif

using namespace Upp;

RPC_METHOD(GetClientList)
{
	INTERLOCKED { LOG("GetClientList()"); }
	
	ValueArray clients;
	for(int i = 0; i < 1000; i++)
	{
		ValueMap client;
		
		client.Add("firstName", Format("First Name %d", i));
		client.Add("lastName", Format("Last Name %d", i));
		client.Add("birthday", GetSysDate());
		
		clients.Add(client);
	}

	Sleep(1000);

	rpc << clients;
	INTERLOCKED { LOG("Done."); }
}

class MtRpcServer
{
private:

	TcpSocket rpc;
	int threadPoolSize;
	int port;
	bool quit;
	
	static MtRpcServer *server;
	
	void Process(int n)
	{
		INTERLOCKED { LOG("Process " << n << " started"); }
		static Mutex accept_mutex;
		
		for(;;)
		{
			TcpSocket http;
			accept_mutex.Enter();
			if(quit)
			{
				accept_mutex.Leave();
				break;
			}
				
			bool b = http.Accept(rpc);
			accept_mutex.Leave();
			if(quit)
				break;
			
			if(b)
			{
				INTERLOCKED { LOG("Request accepted " << n); }
				RpcPerform(http, NULL);
			}
			else
			{
				INTERLOCKED { LOG("Request rejected " << n); }
				break;
			}
		}
	}

	#ifdef PLATFORM_WIN32
	static BOOL WINAPI CtrlCHandlerRoutine(__in  DWORD dwCtrlType)
	{
		LOG("Ctrl+C handler");
		server->quit = true;
		for(int i = 0; i < server->threadPoolSize; i++)
		{
			TcpSocket h;
			h.Connect("127.0.0.1", server->port);
			h.Put("quit");
		}
		return TRUE;
	}
	#endif
	
public:

	MtRpcServer(int port = 1234, int poolSize = 8)
	{
		server = this;
		#ifdef PLATFORM_WIN32
		SetConsoleCtrlHandler(CtrlCHandlerRoutine, true);
		#endif
		this->threadPoolSize = poolSize;
		this->port = port;
		this->quit = false;
	}
	
	void Run()
	{
		rpc.Timeout(Null);
		if(!rpc.Listen(port, 5))
			Exit(1);
		
		for(int i = 0; i < threadPoolSize; i++)
			Thread::Start(callback1(this, &MtRpcServer::Process, i));
		
		while(Thread::GetCount())
			Sleep(100);
	}
};

MtRpcServer *MtRpcServer::server = NULL;


CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);
	LOG("MtRpcServer..");
	LogRpcRequests();
	MtRpcServer server;
	server.Run();
}
