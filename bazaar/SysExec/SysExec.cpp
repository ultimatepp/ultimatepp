#include "SysExec.h"
#include "ArgEnv.h"

#ifdef PLATFORM_POSIX

#include <unistd.h>
#include <sys/wait.h>
#include "SudoLib.h"

#else

#include <process.h>
#include "ShellLib.h"

#endif

NAMESPACE_UPP

// replacement of tmpfile() -- has problems in windows
static FILE *TempFile(void)
{
	String fName = GetTempFileName();
	FILE *f = fopen(fName, "w+");
	return f;
}

///////////////////////////////////////////////////////////////////////////////////////////////
// executes an external command, passing a command line to it and gathering the output
// from both stdout and stderr
bool SysExec(String const &command, String const &args, const VectorMap<String, String> &Environ, String &OutStr, String &ErrStr)
{
	// flushes both stdout and stderr files
	fflush(stdout);
	fflush(stderr);

	// saves stdout stream state
	int saveStdout = dup(fileno(stdout));

	// creates and opens a temporary file and assigns stdout to it
	int OutFile = fileno(TempFile());
	dup2(OutFile, 1);

	// saves stderr stream state
	int saveStderr = dup(fileno(stderr));

	// creates and opens a temporary file and assigns stdout to it
	int ErrFile = fileno(TempFile());
	dup2(ErrFile, 2);

	// builds the arguments and the environment
	char **argv = BuildArgs(command, args);
	char **envv = BuildEnv(Environ);
	
	// executes the command
	int result = 0;
#ifdef PLATFORM_POSIX
	pid_t pID = fork();
	if (pID == 0)
	{
		// Code only executed by child process

		// exec svn, function shall not return if all ok
		result = execvpe(command, argv, envv);

		// terminates child process
		_exit(result);
	}
	else if (pID < 0)
	{
		// code executed only if couldn't fork
		Cerr() << "Couldn't fork...\n";
		result = -1;
	}
	else
	{
		// Code only executed by parent process
		
		// waits for parent to terminate
		wait(&result);
	}


#else
	result = _spawnvpe(_P_WAIT, command, argv, envv);
#endif

	// flushes both stdout and stderr files
	fflush(stdout);
	fflush(stderr);

	// restores stdout and stderr handles
	if(saveStdout != -1)
		dup2(saveStdout, 1);
	if(saveStderr != -1)
		dup2(saveStderr, 2);

	if (result == -1)
		Cerr() << "Error spawning process\n";

	// allocates char buffers and reads out end err data into them
	OutStr.Clear();
	int OutSize = lseek(OutFile, 0L, SEEK_END);
	if (OutSize > 0)
	{
		char *buf = (char *)malloc(OutSize + 1);
		lseek(OutFile, 0L, SEEK_SET);
		/*int dummy = */ (void)read(OutFile, buf, OutSize);
		buf[OutSize] = 0;
		OutStr.Cat(buf);
		free(buf);
	}

	ErrStr.Clear();
	int ErrSize = lseek(ErrFile, 0L, SEEK_END);
	if (ErrSize > 0)
	{
		char *buf = (char *)malloc(ErrSize + 1);
		lseek(ErrFile, 0L, SEEK_SET);
		/* int dummy = */(void)read(ErrFile, buf, ErrSize);
		buf[ErrSize] = 0;
		ErrStr.Cat(buf);
		free(buf);
	}

	// closes files
	close(OutFile);
	close(ErrFile);

	return (!result);

} // END SysExec()

bool SysExec(String const &command, String const &args, String &OutStr, String &ErrStr)
{
	return SysExec(command, args, Environment(), OutStr, ErrStr);

} // END SysExec()

bool SysExec(String const &command, String const &args, const VectorMap<String, String> &Environ, String &OutStr)
{
	String ErrStr;
	return SysExec(command, args, Environ, OutStr, ErrStr);

} // END SysExec()

bool SysExec(String const &command, String const &args, String &OutStr)
{
	String ErrStr;
	return SysExec(command, args, Environment(), OutStr, ErrStr);

} // END SysExec()

bool SysExec(String const &command, String const &args, const VectorMap<String, String> &Environ)
{
	String OutStr, ErrStr;
	return SysExec(command, args, Environ, OutStr, ErrStr);
}

bool SysExec(String const &command, String const &args)
{
	String OutStr, ErrStr;
	return SysExec(command, args, Environment(), OutStr, ErrStr);

} // END SysExec()

///////////////////////////////////////////////////////////////////////////////////////////////
// executes an external command, passing a command line to it without waiting for its termination
// optionally returns pid of started process
bool SysStart(String const &command, String const &args, const VectorMap<String, String> &Environ, intptr_t *pid)
{
	// builds the arguments and the environment
	char **argv = BuildArgs(GetFileName(command), args);
	char **envv = BuildEnv(Environ);
	
	// executes the command
	int result = 0;
#ifdef PLATFORM_POSIX

	// initializes result in case of successful fork but
	// failed execvpe.... so the return value is correct
	result = -1;
	
	// forks the process
	pid_t pID = fork();
	if (pID == 0)
	{
		// Code only executed by child process

		// exec process, function shall not return if all ok
		result = execvpe(command, argv, envv);

		// terminates child process
		_exit(result);
	}
	else if (pID < 0)
	{
		// code executed only if couldn't fork
		Cerr() << "Couldn't fork...\n";
		result = -1;
	}
	else
	{
		// Code only executed by parent process
		
		// just return spawned process id
		result = getpid();
	}


#else
	result = _spawnvpe(_P_NOWAIT, command, argv, envv);
#endif

	if(pid)
		*pid = result;

	if (result == -1)
		Cerr() << "Error spawning process\n";

	return (result != -1 ? true : false);

}

bool SysStart(String const &command, String const &args, intptr_t *pid)
{
	return SysStart(command, args, Environment(), pid);
}

///////////////////////////////////////////////////////////////////////////////////////////////
// executes an external command as Admin user, passing a command line to it without waiting for its termination
// it WILL prompt for user intervention on secure systems (linux - Vista+ OSes)
// return true on success, false otherwise
bool SysStartAdmin(String const &password, String const &command, String const &args, const VectorMap<String, String> &Environ)
{
#ifdef PLATFORM_POSIX
	// on linux, we must provide the password
	return SudoExec("root", password, command + " " + args, Environ, false);
#else
	// on windows, no pass should be needed, it'll display the dialog automatically
	if(IsUserAdministrator())
		return SysStart(command, args, Environ);
	else
		return ShellExec(command + " " + args, Environ, false);
#endif
}

bool SysStartAdmin(String const &password, String const &command, String const &args)
{
	return SysStartAdmin(password, command, args, Environment());
	
}

///////////////////////////////////////////////////////////////////////////////////////////////
// executes an external command as Admin user, passing a command line to it waiting for its termination
// it WILL prompt for user intervention on secure systems (linux - Vista+ OSes)
// return true on success, false otherwise
bool SysExecAdmin(String const &password, String const &command, String const &args, const VectorMap<String, String> &Environ)
{
#ifdef PLATFORM_POSIX
	// on linux, we must provide the password
	return SudoExec("root", password, command + " " + args, Environ, true);
#else
	// on windows, no pass should be needed, it'll display the dialog automatically
	if(IsUserAdministrator())
		return SysExec(command, args, Environ);
	else
		return ShellExec(command + " " + args, Environ, true);
#endif
}

bool SysExecAdmin(String const &password, String const &command, String const &args)
{
	return SysExecAdmin(password, command, args, Environment());
	
}

///////////////////////////////////////////////////////////////////////////////////////////////
// executes an external command as required user, passing a command line to it without waiting for its termination
// on linux, will return an error if password is required AND wrong
// on windows, by now... it just spawn the process without changing security level
// I still shall find a way to go back to user mode on windows
// return true on success, false otherwise
bool SysStartUser(String const &user, String const &password, String const &command, String const &args, const VectorMap<String, String> &Environ)
{
#ifdef PLATFORM_POSIX
	// on linux, we must provide the password
	return SudoExec(user, password, command + " " + args, Environ, false);
#else
	// on windows, don't know a way for re-lowering app level to user one, so I simply spawn a new process
	return SysStart(command, args, Environ, NULL);
#endif
}

bool SysStartUser(String const &user, String const &password, String const &command, String const &args)
{
	return SysStartUser(user, password, command, args, Environment());
}

///////////////////////////////////////////////////////////////////////////////////////////////
// executes an external command as required user, passing a command line to it waiting for its termination
// on linux, will return an error if password is required AND wrong
// on windows, by now... it just spawn the process without changing security level
// I still shall find a way to go back to user mode on windows
// return true on success, false otherwise
bool SysExecUser(String const &user, String const &password, String const &command, String const &args, const VectorMap<String, String> &Environ)
{
#ifdef PLATFORM_POSIX
	// on linux, we must provide the password
	return SudoExec(user, password, command + " " + args, Environ, true);
#else
	// on windows, don't know a way for re-lowering app level to user one, so I simply spawn a new process
	return SysExec(command, args, Environ);
#endif
}

bool SysExecUser(String const &user, String const &password, String const &command, String const &args)
{
	return SysExecUser(user, password, command, args, Environment());
}

END_UPP_NAMESPACE
