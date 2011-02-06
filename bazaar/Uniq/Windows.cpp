#include "Uniq.h"

NAMESPACE_UPP

#ifdef PLATFORM_WIN32

#include <fcntl.h>
#define PIPE_NOWAIT                 0x00000001

bool Uniq::SendCmdLine(HANDLE pipe)
{
	char buf[256];
	Vector<String> v;
	DWORD pipeState;
	
	// we're in non-blocking mode; try to connect pipe
	// if no client ready, we just wait and repeat
	int res = ConnectNamedPipe(pipe, NULL);
	if(res != 0 || GetLastError() != ERROR_PIPE_CONNECTED)
		return false;

	// pipe should be connected here... we shall go in blocking
	// mode and read data from client
	pipeState = PIPE_READMODE_BYTE | PIPE_WAIT;
	SetNamedPipeHandleState(
		pipe, 								// hNamedPipe,
		&pipeState,							// LPDWORD lpMode,
		NULL,								// lpMaxCollectionCount,
		NULL								// lpCollectDataTimeout
	);

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
	fclose(f);
	_close(p);
	
	// go in non-blocking mode again
	pipeState = PIPE_READMODE_BYTE | PIPE_NOWAIT;
	SetNamedPipeHandleState(
		pipe, 								// hNamedPipe,
		&pipeState,							// LPDWORD lpMode,
		NULL,								// lpMaxCollectionCount,
		NULL								// lpCollectDataTimeout
	);

	// disconnects from client
	DisconnectNamedPipe(pipe);

	// posts the callback to handle new instance's command line
	PostCallback(callback1(WhenInstance, v));
	return true;
}

// polling callback
void Uniq::pollCb(HANDLE pipe)
{
#ifdef flagMT

	while(!Thread::IsShutdownThreads())
	{
		if(!SendCmdLine(pipe))
		{
			Sleep(200);
			continue;
		}
	}
		
#else
	// non multithreaded -- we shall check for data from pipe
	// in non-blocking mode and read it if available
	SendCmdLine(pipe);
	
	// respawn itself by aid of another timed callback
//	pipe = open(FIFO_NAME, O_RDONLY | O_NDELAY);
	SetTimeCallback(200, THISBACK(pollCb));
#endif

}

Uniq::Uniq()
{
	// we still don't know if we're inside first app instance
	// so we default for NOT
	isFirstInstance = false;

	pipePath = Format("\\\\.\\PIPE\\%s", GetExeTitle());

	// try to open pipe for write; if succeed, pipe is there
	// so we're on a new app instance
	HANDLE pipe = CreateFile(
		pipePath,
		GENERIC_WRITE,
		0, //i.e. No Share
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
		
	if(pipe != NULL && pipe != INVALID_HANDLE_VALUE)
	{
		// pipe already there, should write command line to it and leave
		int p = _open_osfhandle((LONG)pipe, _O_WRONLY);
		FILE *f = fdopen(p, "w");
		fprintf(f, "%d\n", CommandLine().GetCount());
		for(int i = 0; i < CommandLine().GetCount(); i++)
			fprintf(f, "%s\n", ~CommandLine()[i]);
		fclose(f);
		_close(p);
	}
	else
	{
		// pipe doesn't exist, create it and act like a listening server
		HANDLE pipe = CreateNamedPipe(
		  pipePath,													// lpName
		  PIPE_ACCESS_INBOUND | FILE_FLAG_FIRST_PIPE_INSTANCE,		// dwOpenMode,
		  PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_NOWAIT,		// dwPipeMode,
		  255,														// nMaxInstances,
		  4096,														// nOutBufferSize,
		  4096,														// nInBufferSize,
		  50,														// nDefaultTimeOut,
		  NULL														// lpSecurityAttributes
		);
		ConnectNamedPipe(pipe, NULL);

		// remember we're inside first app instance
		isFirstInstance = true;
		
#ifdef flagMT
		pollThread.Run(THISBACK1(pollCb, pipe));
#else
		// use a 200 ms delayed callback to check if another instance
		// gave its command line
		SetTimeCallback(200, THISBACK1(pollCb, pipe));
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
		unlink(pipePath);
	}
}

#endif

END_UPP_NAMESPACE
