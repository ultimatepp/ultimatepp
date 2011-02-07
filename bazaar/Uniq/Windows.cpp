#include "Uniq.h"

NAMESPACE_UPP

#ifdef PLATFORM_WIN32

#include <fcntl.h>

String ErrorMsg(int err)
{
	char msg[512];
	FormatMessage(
	  FORMAT_MESSAGE_FROM_SYSTEM,
	  NULL,
	  err,
	  0,
	  msg,
	  512,
	  NULL
	);
	return msg;
}

bool Uniq::SendCmdLine(void)
{
	char buf[256];
	Vector<String> v;
	DWORD pipeState;
	String resMsg;
	
	// we're in non-blocking mode; try to connect pipe
	// if no client ready, we just wait and repeat
	int res = ConnectNamedPipe(pipe, NULL);
	if(res == 0 && GetLastError() != ERROR_PIPE_CONNECTED)
	{
		resMsg = ErrorMsg(GetLastError());
		return false;
	}

Cerr() << "PIPE CONNECTED\n";

	// pipe should be connected here... we shall go in blocking
	// mode and read data from client
	pipeState = PIPE_READMODE_BYTE | PIPE_WAIT;
	SetNamedPipeHandleState(
		pipe, 								// hNamedPipe,
		&pipeState,							// LPDWORD lpMode,
		NULL,								// lpMaxCollectionCount,
		NULL								// lpCollectDataTimeout
	);
LOG("PIPE BLOCKING");
	int p = _open_osfhandle((LONG)pipe, _O_RDONLY);
	FILE *f = fdopen(p, "r");

	// gets number of elements in vector
	if(fgets(buf, 256, f))
	{
		int count = ScanInt(buf);
		
		// get the strings
		for(int i = 0; i < count; i++)
		{
			if(!fgets(buf, 256, f))
				break;
			String s = buf;
			if(s.GetCount() && s[s.GetCount() - 1] == '\n')
				s = s.Left(s.GetCount() - 1);
			v.Add(s);
		}
	}
LOG("DATA READ FROM PIPE");
LOG("Count : " << v.GetCount());
for(int i = 0; i < v.GetCount(); i++)
	LOG("Item #" << i << " = " << v[i]);
	fclose(f);
//	_close(p);
LOG("FILE CLOSED");
	
	// go in non-blocking mode again
	pipeState = PIPE_READMODE_BYTE | PIPE_NOWAIT;
	SetNamedPipeHandleState(
		pipe, 								// hNamedPipe,
		&pipeState,							// LPDWORD lpMode,
		NULL,								// lpMaxCollectionCount,
		NULL								// lpCollectDataTimeout
	);
LOG("PIPE UNBLOCKED");
	// disconnects from client and wait for new connections
	DisconnectNamedPipe(pipe);
	ConnectNamedPipe(pipe, NULL);
	
LOG("PIPE DISCONNECTED");
	// posts the callback to handle new instance's command line
	PostCallback(callback1(WhenInstance, v));
	return true;
}

// polling callback
void Uniq::pollCb()
{
#ifdef flagMT

	while(!Thread::IsShutdownThreads())
	{
		if(!SendCmdLine())
		{
			Sleep(200);
			continue;
		}
	}
		
#else
	// non multithreaded -- we shall check for data from pipe
	// in non-blocking mode and read it if available
	SendCmdLine();
	
	// respawn itself by aid of another timed callback
	SetTimeCallback(200, THISBACK(pollCb));
#endif

}

Uniq::Uniq()
{
	LOG("START");
	// we still don't know if we're inside first app instance
	// so we default for NOT
	isFirstInstance = false;

	pipePath = Format("\\\\.\\PIPE\\%s", GetExeTitle());

	// try to create the pipe, one instance allowed
	pipe = CreateNamedPipe(
	  pipePath,													// lpName
	  PIPE_ACCESS_INBOUND ,										// dwOpenMode,
	  PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_NOWAIT,		// dwPipeMode,
	  1,														// nMaxInstances,
	  4096,														// nOutBufferSize,
	  4096,														// nInBufferSize,
	  50,														// nDefaultTimeOut,
	  NULL														// lpSecurityAttributes
	);
	if(pipe == NULL || pipe == INVALID_HANDLE_VALUE)
	{
		// error creating the pipe... it should be there already
		LOG("ERROR CREATING PIPE : " << ErrorMsg(GetLastError()));
		LOG("THERE SHOULD BE A SERVER WAITING CONNECTIONS...");
		
/*
		LOG("WAITING FOR PIPE, 5 SECONDS");
		if(!WaitNamedPipe(pipePath, 5000))
		{
			LOG("TIMEOUT WAITING FOR CLIENT... BAILING OUT");
			return;
		}
*/

		// pipe should be ready for connection, try it
		LOG("OK, PIPE SHOULD BE THERE.... TRYING TO CONNECT");
		pipe = CreateFile(
			pipePath,
			GENERIC_WRITE,
			0, //i.e. No Share
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
			
		if(pipe == NULL || pipe == INVALID_HANDLE_VALUE)
		{
			LOG("ERROR CONNECTING TO PIPE : " << ErrorMsg(GetLastError()));
			LOG("BAILING OUT....");
			return;
		}
		
		// we go blocking here
		DWORD pipeState = PIPE_READMODE_BYTE | PIPE_WAIT;
		SetNamedPipeHandleState(
			pipe, 								// hNamedPipe,
			&pipeState,							// LPDWORD lpMode,
			NULL,								// lpMaxCollectionCount,
			NULL								// lpCollectDataTimeout
		);

		// succesfully connected to server, send command line to him
		LOG("CONNECTED! SENDING COMMAND LINE....");
		
		int p = _open_osfhandle((LONG)pipe, _O_WRONLY);
		FILE *f = fdopen(p, "w");
		fprintf(f, "%d\n", CommandLine().GetCount());
		for(int i = 0; i < CommandLine().GetCount(); i++)
			fprintf(f, "%s\n", ~CommandLine()[i]);
		fclose(f);
		FlushFileBuffers(pipe);
		_close(p);
		CloseHandle(pipe);
		LOG("COMMAND LINE SENT AND PIPE FREED");
	}
	else
	{
		// pipe successfully created, connect to it and wait for clients
		LOG("PIPE CREATED");
		int res = ConnectNamedPipe(pipe, NULL);
		LOG("CONNECTNAMEDPIPE RETURNED " << res << " ERROR IS " << ErrorMsg(GetLastError()));

		// remember we're inside first app instance
		isFirstInstance = true;
		
#ifdef flagMT
		pollThread.Run(THISBACK(pollCb));
#else
		// use a 200 ms delayed callback to check if another instance
		// gave its command line
		SetTimeCallback(200, THISBACK1(pollCb));
#endif
	}

}

Uniq::~Uniq()
{
	if(isFirstInstance)
	{
#ifdef flagMT
		Thread::ShutdownThreads();
		pollThread.Wait();
#endif
		CloseHandle(pipe);
	}
}

#endif

END_UPP_NAMESPACE
