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
bool SudoExec(String user, String const &password, String const &args, VectorMap<String, String> const &envir, bool w)
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
	String sudoArgs = "-H -E -p gimmipass ";
	sudoArgs += "-u " + user;
	sudoArgs += " -- " + args;
	
	String clearArgs = "-K";

	// prepare args buffers
	char **argv = BuildArgs("sudo", sudoArgs);
	char **clearArgv = BuildArgs("sudo", clearArgs);

	// prepare environment buffer
	char **envv = BuildEnv(env);
	
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
		
		// becomes the session leader
		if(setsid() == -1)
			_exit(-1);

		// login into pseudoconsole
		if(login_tty(slavePty) == -1)
			_exit(-1);
		
		// ignore tty hangups so we can close
		// the pseudotty later
		signal(SIGHUP, SIG_IGN);
		
		// we shall first clear the password stored, if any
		int cPid = fork();
		if(cPid == -1)
			_exit(-1);
		else if(cPid == 0)
		{
			result = execvp("/usr/bin/sudo", clearArgv);
			_exit(0);
		}
		else
		{
			int status;
			wait(&status);
		}

		// executes sudo
		result = execvpe("/usr/bin/sudo", argv, envv);

		// just in case execvp failed
        _exit(result);
	}
	else
	{
		// parent process
		
		// associates an stream to child process pseudoterminal
		FILE *sudoFile = fdopen(masterPty, "w+");

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
				if(password != "")
					_PutLine(sudoFile, ~password);
				else
					_PutLine(sudoFile, "<NULL>");
				
				// hmmmm... it hangs sometimes here reading the status
				// it seems that sudo sometimes ask for password AND don't put
				// status back to terminal, OR something very wrong happens
				// on forks above. As is better to not have a status than
				// have the app hanging, we set a timeout on status read

				// blocking stream again, to be sure to get the status back
				// (we know that must come 2 lines of status...)
				fcntl(masterPty, F_SETFL, fcntl(masterPty, F_GETFL) & ~O_NONBLOCK);

				// skip the empty line after password
				_GetLine(sudoFile);
				
				// stop blocking again, we must clean sudo output by removing
				// all chars there
				fcntl(masterPty, F_SETFL, fcntl(masterPty, F_GETFL) | O_NONBLOCK);

				// give sudo some time to setup the status line, IF ANY
				fd_set rfds;
				struct timeval tv;
				FD_ZERO(&rfds);
				FD_SET(masterPty, &rfds);
				tv.tv_sec = 0;
				tv.tv_usec = 200000;
				select(masterPty + 1, &rfds, NULL, NULL, &tv);
				
				// get the status
				line = _GetLine(sudoFile);

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
		bool res = true;
		
		// if requested, wait for child process to terminate
		if(w)
		{
			// it shall terminate sometimes....
			waitpid(pid, &status, 0);

			// check sudo exit status
			if(WIFEXITED(status))
				res = (WEXITSTATUS(status) == 0);
			else if(WIFSIGNALED(status))
				res = true;
			else
				res =  false;
		}
		// otherwise, just check once if process exited, by error
		// for example -- maybe it couldn't spawn command
		else
		{
			while(true)
			{
				// get spawned process executable file name
				FileIn f("/proc/" + FormatInt(pid) + "/cmdline");
				String procName = GetFileTitle(f.GetLine());
				f.Close();
				
				// check if process is still running
				// it should, if all is ok
				int waitRes = waitpid(pid, &status, WNOHANG);
				
				// if still running within sudo, continue waiting
				if(!waitRes && procName == "sudo")
				{
					Sleep(100);
					continue;
				}
				// otherwise, if process exited, return its status
				else if(waitRes)
				{
					// check sudo exit status
					if(WIFEXITED(status))
						res = (WEXITSTATUS(status) == 0);
					else if(WIFSIGNALED(status))
						res = true;
					else
						res =  false;
					return res;
				}
				// otherwise, process is still running as spawned app
				// all is ok, just return true
				return true;
			}
		}
				
		// eat last sudo ouptut, if any
		while(_GetLine(sudoFile) != "")
				;
		// close pseudoterminal
		fclose(sudoFile);
	
		return res;
	}
}

#endif

END_UPP_NAMESPACE
