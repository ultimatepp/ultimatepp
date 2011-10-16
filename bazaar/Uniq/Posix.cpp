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
	tv.tv_usec = 2000;
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
	lockPath = Format("/tmp/%s_%d_UNIQ_LCK", GetExeTitle(), (int64)getuid());
	
	// we check first if the lock file is there AND locked by main instance
	// if it is, we're on a secondary instance; if not, we're main
	if(FileExists(lockPath))
	{
		// lock file does exist, try to open and lock it
		lockFile = open(lockPath, O_RDWR);
		struct flock fl;
		fl.l_type = F_WRLCK;
		fl.l_whence = SEEK_SET;
		fl.l_start = 0;
		fl.l_len = 0;
		int lock = fcntl(lockFile, F_SETLK, &fl);
		close(lockFile);
		if(lock != -1)
		{
			// we could lock the file, it was a stray pipe
			// so we remove both
			unlink(lockPath);
			unlink(pipePath);
		}
	}
	
	// try to open pipe for write; if succeed, pipe is there
	// so we're on a new app instance
	int p = open(pipePath, O_WRONLY /* | O_NDELAY */);
	if(p != -1)
	{
		// pipe already there, we should write command line to it and leave
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
		// we also create the lock file
		if(mkfifo(pipePath, S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH))
		{
			Cerr() << t_("Error creating interprocess pipe") << "\n";
			exit(1);
		}
		lockFile = open(lockPath, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
		struct flock fl;
		fl.l_type = F_WRLCK;
		fl.l_whence = SEEK_SET;
		fl.l_start = 0;
		fl.l_len = 0;
		int lock = fcntl(lockFile, F_SETLK, &fl);
		if(lock == -1)
		{
			Cerr() << t_("Error locking lock file") << "\n";
			unlink(pipePath);
			unlink(lockPath);
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
		close(lockFile);
		unlink(lockPath);
		unlink(pipePath);
	}
}

#endif

END_UPP_NAMESPACE
