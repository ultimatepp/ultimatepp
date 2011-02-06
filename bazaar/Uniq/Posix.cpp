#include "Uniq.h"

NAMESPACE_UPP

#ifdef PLATFORM_POSIX

// send command line to callback handler
bool Uniq::SendCmdLine(int pipe)
{
	char buf[256];
	Vector<String>v;

	fd_set rfds;
	struct timeval tv;
	FD_ZERO(&rfds);
	FD_SET(pipe, &rfds);
	tv.tv_sec = 0;
	tv.tv_usec = 200000;
	if(!select(pipe+1, &rfds, NULL, NULL, &tv))
		return false;

	// FILE gets handy for fgets() &C....
	FILE *f = fdopen(pipe, "r");

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
	// posts the callback to handle new instance's command line
	PostCallback(callback1(WhenInstance, v));
	return true;

}

// polling callback
void Uniq::pollCb(void)
{
	// dont block on open, that would make thread shutdown impossible
	int pipe = open(pipePath, O_RDONLY | O_NDELAY);
	if(pipe == -1)
		return;

#ifdef flagMT
	// multithread makes it easy... just open receiving pipe
	// and wait for data from it -- it can do blocking reads
	// and no need for wait cycles

	while(!Thread::IsShutdownThreads())
	{
		// if there's no data waiting on pipe
		// just sleep a bit and try again
		if(!SendCmdLine(pipe))
		{
			Sleep(200);
			continue;
		}

		// select doesn't handle well closed fifos on second call
		// the solution is close and reopen it again
		close(pipe);
		pipe = open(pipePath, O_RDONLY | O_NDELAY);
	}
		
#else
	// non multithreaded -- we shall check for data from pipe
	// in non-blocking mode and read it if available
	SendCmdLine(pipe);
	
	// respawn itself by aid of another timed callback
	SetTimeCallback(200, THISBACK(pollCb));
#endif

	close(pipe);
}

Uniq::Uniq()
{
	// we still don't know if we're inside first app instance
	// so we default for NOT
	isFirstInstance = false;

	pipePath = Format("/tmp/%s_%d_UNIQ", GetExeTitle(), (int64)getuid());
	
	// try to open pipe for write; if succeed, pipe is there
	// so we're on a new app instance
	int p = open(pipePath, O_WRONLY /* | O_NDELAY */);
	if(p != -1)
	{
		// pipe already there, should write command line to it and leave
		// the O_NDELAY is to be sure tha we don't block if active instance
		// exits on the way
		FILE *f = fdopen(p, "w");
		fprintf(f, "%d\n", CommandLine().GetCount());
		for(int i = 0; i < CommandLine().GetCount(); i++)
			fprintf(f, "%s\n", ~CommandLine()[i]);
		fclose(f);
		close(p);
	}
	else
	{
		// pipe doesn't exist, create it and allow writing for other processes
		if(mkfifo(pipePath, S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH))
		{
			Cerr() << "Error creating interprocess pipe\n";
			exit(1);
		}

		// remember we're inside first app instance
		isFirstInstance = true;
		
		// now, if we do multithreading, we use a listening thread
		// for other's instances communications
		// otherwise we use a timed callback (which is less efficient)
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
		unlink(pipePath);
	}
}

#endif

END_UPP_NAMESPACE
