////////////////////////////////////////////////////////////////////////////////////////////////////////
// a working (better ??) implementation of libsudo equivalent
// allows to run sudo either in sync or async mode
////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "SysExec.h"
#include "ArgEnv.h"

NAMESPACE_UPP

#ifdef PLATFORM_POSIX

#include <sys/wait.h> 
#include <pty.h> 
#include <utmp.h>

// a couple of stream helpers... we can't use Upp ones here because we need non-blocking streams
static void _PutLine(FILE *f, const char *line)
{
	fprintf(f, "%s\n", line);
}

static String _GetLine(FILE *f)
{
	static char buf[256];
	
	*buf = 0;
	fgets(buf, 255, f);
	return buf;
}

///////////////////////////////////////////////////////////////////////////////////////////////
// executes a command via sudo; if wait is true, will wait for command end, otherwise executes it in background
bool SudoExec(String user, String const &password, String const &args, VectorMap<String, String> const &envir, bool wait)
{
	// root user as default
	if(user == "")
		user = "root";
	
	// take this damn'd thing from environment
	// it will make some apps crash... don't know why
	VectorMap<String, String>env(envir, 1);
	if(env.Find("DBUS_SESSION_BUS_ADDRESS"))
		env.RemoveKey("DBUS_SESSION_BUS_ADDRESS");
	
	// setup XAUTHORITY for new user, if different
	// from current one -- this will allow using X display
	// for spawned user
	if(user != GetUserName())
	{
		String xFile = env.Get("XAUTHORITY", "/home/" + GetUserName() + "/.Xauthority");
		if(FileExists(xFile))
		{
			// create a temp location for copied xFile
			String tmpFile = GetTempFileName();
			FileCopy(xFile, tmpFile);
			env.RemoveKey("XAUTHORITY");
			env.Add("XAUTHORITY", tmpFile);
		}
	}

	// prepare args for sudo execution
	String sudoArgs = "-S -E -E -p gimmipass ";

//	THAT ONE DON'T WORK -- MAYBE FOR VIRTUAL TERMINAL ?
//	WE CHOOSE ANOTHER WAY
/*
	if(!wait)
		sudoArgs += "-b ";
*/

	sudoArgs += "-u " + user;
	sudoArgs += " -- " + args;

	// prepare args buffer
	Buffer<char *>argv = BuildArgs("sudo", sudoArgs);

	// prepare environment buffer
	Buffer<char *>envv = BuildEnv(env);
	
	// forks the process to launch sudo command inside child
	// providing a pseudoconsole for sudo.. it refuses to run without
	// beware, forkpty() don't work here... probably the setsid()
	// needs to happen before login_tty()
	int result = 0;
	int masterPty, slavePty;
	if(openpty(&masterPty, &slavePty, NULL, NULL, NULL) == -1)
		return false;

	pid_t pid = fork();
	if(pid < 0)
		return false;
	
	if(pid == 0)
	{
		// child process
		
		// becomes the group leader
		if(setsid() == -1)
			_exit(-1);

		// login into pseudoconsole
		if(login_tty(slavePty) == -1)
			_exit(-1);

		// this other fork is needed because we HAVE
		// to leave the pseudoconsole after sudo has done its
		// work, otherwise closing the pseudoconsole would close the
		// sudo forked process -- 3 days to solve this.....
		pid_t xpid = fork();
		if(xpid == -1)
			_exit(-1);
		else if(xpid == 0)
		{
			// executes sudo
			result = execvpe("/usr/bin/sudo", argv, envv);

			// just in case execvp failed
	        _exit(result);
		}
		else
		{
			int xstatus = 0;

			// if we shall NOT wait for sudo completion
			// we must return asap, but AFTER disconnecting the pseudotty
			if(!wait)
			{
				// give sudo some time to get the password and launch the command
				// (only if we shall not wait its completion...)
				Sleep(1000);

				// then disconnect it from pseudotty only if we're NOT
				// waiting for process completion, otherwise this will
				// make the last waitpid() return an error status
				ioctl(pid, TIOCNOTTY);
			}

			// otherwise, if we shall wait for sudo to complete its command
			// we must wait for xpid process to terminate before leaving
			else
			{
				while(!waitpid(xpid, &xstatus, WNOHANG))
					Sleep(1000);
				if(WIFEXITED(xstatus))
					xstatus = WEXITSTATUS(xstatus);
				else
					xstatus = -1;
			}

			_exit(xstatus);
		}
	}
	else
	{
		// parent process
		
		// associates an stream to child process pseudoterminal
		FILE *sudoFile = fdopen(masterPty, "w+");

/*
		// notice that echo is off (it it is....)
		struct termios tio;
		tcgetattr (masterPty, &tio);
		for (int i = 0; (tio.c_lflag & ECHO) && i < 15; i++)
		{
			usleep (1000);
			tcgetattr (masterPty, &tio);
		}
*/
		
		// set input pipe to non-blocking, so we can read as we like
		// without hanging the program -- we need to check for 'password' input
		fcntl(masterPty, F_SETFL, O_NONBLOCK);
		
		// even if we can read from stream, we wait that there's somethink to read
		// or, 1 second if nothing there, at most, to avoid that sudo prompt
		// arrives too late to catch it
		fd_set rfds;
		struct timeval tv;
		FD_ZERO(&rfds);
		FD_SET(masterPty, &rfds);
		tv.tv_sec = 1;
		tv.tv_usec = 0;
		select(masterPty + 1, &rfds, NULL, NULL, &tv);
		
		// now we can try to read the prompt; it could be the 'gimmipass'
		// we choose, or some error message, garbage or even nothing
		// we repeat read until we find OR the passprompt required, OR
		// some error messages, OR... nothing
		String line;
		int answer = 0;
		for(int i = 0; i < 50; i++)
		{
			line = _GetLine(sudoFile);
			if(line.StartsWith("gimmipass"))
			{
				answer = 1;
				break;
			}
			else if(line.StartsWith("sudo:") || line.StartsWith("no passwd entry for"))
			{
				answer = 2;
				break;
			}
		}

		// here we have :
		switch(answer)
		{
			//	NO PASSWORD REQUIRED
			case 0:
				// we don't need any status from sudo
				line = "";
				break;
			
			//	PASSWORD REQUIRED
			case 1:
				// send the password to sudo
				Sleep(100);
				_PutLine(sudoFile, ~password);
				
				// hmmmm... it hangs sometimes here reading the status
				// it seems that sudo sometimes ask for password AND don't put
				// status back to terminal, OR something very wrong happens
				// on forks above. As is better to not have a status than
				// have the app hanging, we set a timeout on status read
/*
				// blocking stream again, to be sure to get the status back
				// (we know that must come 2 lines of status...)
				fcntl(masterPty, F_SETFL, fcntl(masterPty, F_GETFL) & ~O_NONBLOCK);
*/
				fd_set rfds;
				struct timeval tv;
				FD_ZERO(&rfds);
				FD_SET(masterPty, &rfds);
				tv.tv_sec = 1;
				tv.tv_usec = 0;
				select(masterPty + 1, &rfds, NULL, NULL, &tv);

				// skip the empty line after password
				_GetLine(sudoFile);
				
				// get the status
				line = _GetLine(sudoFile);

				// stop blocking again, we must clean sudo output by removing
				// all chars there
				fcntl(masterPty, F_SETFL, fcntl(masterPty, F_GETFL) | O_NONBLOCK);
				break;
				
			//	USER DOESN'T EXISTS IN SUDOERS -- ERROR
			case 2:
				// close streams and leave with error
				fclose(sudoFile);
				return(false);
		} // switch
		
		// here we have in 'line' the status got back from sudo
		// or an empty line, if password wasn't required
		// we check the status for password correctness
		if(
			line.StartsWith("Sorry, try again.") ||
			line.StartsWith("Sorry, user ")
		)
		{
			fclose(sudoFile);
			return false;
		}
		else
		{
			int spc = line.Find(' ');
			if(spc >= 0 && line.Find("is not in", spc+1) >= 0)
			{
				fclose(sudoFile);
				return false;
			}
		}
		
		// ok, now we should have checked all and the sudo cmd
		// should have been succesful
		int status;
		bool res;
		
		// this one of we shall wait for command completion
//		if(wait)
//		{
			while(!waitpid( pid, &status, WNOHANG ))
				Sleep(1000);

			// eat last sudo ouptut, if any
			while(_GetLine(sudoFile) != "")
				;
	
			// check sudo exit status
			if(WIFEXITED(status))
				res = (WEXITSTATUS(status) == 0);
			else
				res =  false;

			// close pseudoterminal
			fclose(sudoFile);
			
//		}
		// and this one if we shall not wait
/*
		else
		{
			res = !line.StartsWith("sudo:");
			int wPid = vfork();
			if(wPid == -1)
			{
				fclose(sudoFile);
				return res;
			}
			else if(wPid == 0)
			{
				setsid();
				while(!waitpid(wPid, &status, WNOHANG ))
					Sleep(1000);
	
				// eat last sudo ouptut, if any
				while(_GetLine(sudoFile) != "")
					;
	
				// close pseudoterminal
				fclose(sudoFile);
				
				_exit(0);
			}
		}
*/
		return res;
	}
}

#endif

END_UPP_NAMESPACE
