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

// writes a string to a message-mode pipe
// sending pipe -- blocking mode
static bool WriteFileString(HANDLE h, String const &s)
{
	DWORD written;
	if(!WriteFile(h, ~s, s.GetCount(), &written, NULL))
		return false;
	if(written != s.GetCount())
		return false;
	return true;
}

// reads a string from a message-mode pipe
static String ReadFileString(HANDLE h)
{
	String s;
	char buf[512];
	DWORD nread;

	while(true)
	{
		bool res = ReadFile(h, buf, 511, &nread, NULL);
		buf[nread] = 0;
		if(!res && GetLastError() == ERROR_MORE_DATA)
			s += buf;
		else if(res)
		{
			s += buf;
			break;
		}
		else
			break;
	}
	return s;
}

bool Uniq::SendCmdLine(void)
{
	// we're in overlapped mode -- wait for a connection to happen
	// if no client ready, we just wait and repeat
	int res = ConnectNamedPipe(pipe, &overlapped);
	
	// res should be 0 in overlapped mode otherwise an error occurred
	if(res)
		return false;

	// check what happened to pipe connection
	bool pending = false;
	switch(GetLastError())
	{
		// pipe still not connected, waiting for client
		case ERROR_IO_PENDING:
			pending = true;
			break;	
	
		// pipe already connected, sets the event
		case ERROR_PIPE_CONNECTED: 
			if (SetEvent(overlapped.hEvent)) 
				break; 
			
		// if none of the above (or SetEvent failed...) an error occurred
		default:
			return false;
	}
	
	// now wait for the connection to succeed, with a timeout of 50 ms
	// if not connected then, simply return false
	if(WaitForSingleObject(overlapped.hEvent, 50) != WAIT_OBJECT_0)
		return false;
	
	// gets overlapped result... don't know if needed here, but....
	DWORD dummy;
	if(!GetOverlappedResult(pipe, &overlapped, &dummy, true))
		return false;
	
	// ok, we're (finally...) connected to client
	// just get command line from him
	Vector<String> v;
	int count = ScanInt(ReadFileString(pipe));
	for(int i = 0; i < count; i++)
		v.Add(ReadFileString(pipe));
	
	// disconnects from client
	DisconnectNamedPipe(pipe);
	
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
	
	// initialize OVERLAP for asynchronous pipe waiting
	event = CreateEvent( 
		NULL,	// default security attribute 
		TRUE,	// manual-reset event 
		TRUE,	// initial state = signaled 
		NULL	// unnamed event object 
	);
	overlapped.hEvent = event;

	// try to create the pipe, one instance allowed
	pipe = CreateNamedPipe(
	  pipePath,													// lpName
	  PIPE_ACCESS_INBOUND | FILE_FLAG_OVERLAPPED,				// dwOpenMode,
	  PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,	// dwPipeMode,
	  1,														// nMaxInstances,
	  4096,														// nOutBufferSize,
	  4096,														// nInBufferSize,
	  50,														// nDefaultTimeOut,
	  NULL														// lpSecurityAttributes
	);
	if(pipe == NULL || pipe == INVALID_HANDLE_VALUE)
	{
		// couldn't create pipe -- it already exists on server side
			
		// try to establish a connection to server
		// it's not enough to wait for pipe once, another process
		// could grab it between the wait and the opening
		for(int i = 0; i < 10; i++)
		{
			if(WaitNamedPipe(pipePath, 500))
			{
				// try to open the pipe
				pipe = CreateFile(
					pipePath,
					GENERIC_WRITE ,
					0, //i.e. No Share
					NULL,
					OPEN_EXISTING,
					FILE_ATTRIBUTE_NORMAL,
					NULL);
					
				// if opened ok, continue to next step
				if(pipe != NULL && pipe != INVALID_HANDLE_VALUE)
					break;
			}
		}
		// if still not connected after 50 retries, bail out...
		if(pipe == NULL || pipe == INVALID_HANDLE_VALUE)
		{
			LOG("ERROR CONNECTING TO PIPE : " << ErrorMsg(GetLastError()));
			LOG("BAILING OUT....");
			return;
		}
		
		// here we're connected to server
		// we work in message mode, tell client to do so
		DWORD ps = PIPE_READMODE_MESSAGE;
		SetNamedPipeHandleState(
			pipe, 								// hNamedPipe,
			&ps,								// LPDWORD lpMode,
			NULL,								// lpMaxCollectionCount,
			NULL								// lpCollectDataTimeout
		);
		
		// now we send command line to main instance
		WriteFileString(pipe, FormatInt(CommandLine().GetCount()));
		for(int i = 0; i < CommandLine().GetCount(); i++)
			WriteFileString(pipe, CommandLine()[i]);
		FlushFileBuffers(pipe);
		CloseHandle(pipe);
	}
	else
	{
		// pipe created succesfully, we're inside first app instance
		
		// remember we're inside first app instance
		isFirstInstance = true;
		
#ifdef flagMT
		pollThread.Run(THISBACK(pollCb));
#else
		// use a 200 ms delayed callback to check if another instance
		// gave its command line
		SetTimeCallback(200, THISBACK(pollCb));
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
