#include "SysExec.h"

#ifdef PLATFORM_POSIX
#include <unistd.h>
#include <sys/wait.h>

#define DEFAULT_PATH 	"/bin:/usr/bin:."
#if 0
static int execvpe(const char *file, char * const *argv, char * const *envp)
{
	char path[PATH_MAX];
	const char *searchpath, *esp;
	size_t prefixlen, filelen, totallen;

	if (strchr(file, '/'))	/* Specific path */
		return execve(file, argv, envp);

	filelen = strlen(file);

	searchpath = getenv("PATH");
	if (!searchpath)
		searchpath = DEFAULT_PATH;

	errno = ENOENT; /* Default errno, if execve() doesn't change it */

	do
	{
		esp = strchr(searchpath, ':');
		if (esp)
			prefixlen = esp - searchpath;
		else
			prefixlen = strlen(searchpath);

		if (prefixlen == 0 || searchpath[prefixlen-1] == '/')
		{
			totallen = prefixlen + filelen;
			if (totallen >= PATH_MAX)
				continue;
			memcpy(path, searchpath, prefixlen);
			memcpy(path + prefixlen, file, filelen);
		}
		else
		{
			totallen = prefixlen + filelen + 1;
			if (totallen >= PATH_MAX)
				continue;
			memcpy(path, searchpath, prefixlen);
			path[prefixlen] = '/';
			memcpy(path + prefixlen + 1, file, filelen);
		}
		path[totallen] = '\0';

		execve(path, argv, envp);
		if (errno == E2BIG || errno == ENOEXEC ||
			errno == ENOMEM || errno == ETXTBSY)
			break;			/* Report this as an error, no more search */

		searchpath = esp + 1;
	}
	while (esp);

	return -1;
}
#endif
#else
#include <process.h>
#endif

NAMESPACE_UPP

///////////////////////////////////////////////////////////////////////////////////////////////
// parses an args line to be useable by spawnxx functions
static Buffer<char *>_BuildArgs(String const &argline)
{
	Array<String> args;
	int pos = 0;
	char c;
	int buflen = 0;
	
	// first arg should be command name... let's make it null
	// it MUST be set, as argv[0] has a special meaning
	args.Add("dummy");

	// skips leading spaces
	while ((c = argline[pos]) != 0 && isspace(c))
		pos++;

	// loop reading args and putting to array
	while (c)
	{
		String &s = args.Add();
		buflen++;
		while (c && !isspace(c))
		{
			// reads enquoted strings
			if (c == '"')
			{
				c = argline[++pos];
				while (c && c != '"')
				{
					s << c;
					buflen++;
					c = argline[++pos];
				}
				if (c)
					c = argline[++pos];
			}
			else
			{
				s << c;
				buflen++;
				c = argline[++pos];
			}
		}

		// skips trailing spaces
		while (c && isspace(c))
			c = argline[++pos];
	}
	buflen += (args.GetCount() + 1) * sizeof(char *);

	// here we've got an array of args and the total size (in bytes) of them
	// we allocates a  buffer for arg array
	Buffer<char *>buf(buflen);

	// we fill the buffer with arg strings
	char **bufindex = buf;
	char *bufpos = (char *)(buf + args.GetCount() + 1);
	int i = 0;
	while (i < args.GetCount())
	{
		String &s = args[i];
		strcpy(bufpos, (const char *)s);
		*bufindex++ = bufpos;
		bufpos += s.GetCount() + 1 ;
		i++;
	}
	*bufindex = 0;

	// returns array of args
	return buf;

} // END _BuildArgs()


///////////////////////////////////////////////////////////////////////////////////////////////
// parses environment map and builds env array
static Buffer<char *>_BuildEnv(const VectorMap<String, String> &env = Environment())
{
	// calculates total environment size
	int envSize = 0;
	for (int i = 0; i < env.GetCount(); i++)
		envSize += env.GetKey(i).GetCount() + env[i].GetCount() + 2 + sizeof(char *);

	// we allocates a  buffer for env array
	Buffer<char *>buf(envSize);

	// we fill the buffer with env strings
	char **bufindex = buf;
	char *bufpos = (char *)(buf + env.GetCount() + 1);
	int i = 0;
	while (i < env.GetCount())
	{
		const String &s = env.GetKey(i) + "=" + env[i];
		strcpy(bufpos, (const char *)s);
		*bufindex++ = bufpos;
		bufpos += s.GetCount() + 1 ;
		i++;
	}
	*bufindex = 0;

	// returns array of args
	return buf;

} // END _BuildEnv()


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
	int OutFile = fileno(tmpfile());
	dup2(OutFile, 1);

	// saves stderr stream state
	int saveStderr = dup(fileno(stderr));

	// creates and opens a temporary file and assigns stdout to it
	int ErrFile = fileno(tmpfile());
	dup2(ErrFile, 2);

	// builds the arguments and the environment
	Buffer<char *>argv = _BuildArgs(args);
	Buffer<char *>envv = _BuildEnv(Environ);
	
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
	dup2(saveStdout, 1);
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
		read(OutFile, buf, OutSize);
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
		read(ErrFile, buf, ErrSize);
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

bool SysExec(String const &command, String const &args)
{
	String OutStr, ErrStr;
	return SysExec(command, args, Environment(), OutStr, ErrStr);

} // END SysExec()



///////////////////////////////////////////////////////////////////////////////////////////////
// executes an external command, passing a command line to it without waiting for its termination
// returns spawned process id (pid) on success, -1 on error
intptr_t SysStart(String const &command, String const &args, const VectorMap<String, String> &Environ)
{
	// builds the arguments and the environment
	Buffer<char *>argv = _BuildArgs(args);
	Buffer<char *>envv = _BuildEnv(Environ);
	
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

	if (result == -1)
		Cerr() << "Error spawning process\n";

	return (result);

}

intptr_t SysStart(String const &command, String const &args)
{
	return SysStart(command, args, Environment());
}

END_UPP_NAMESPACE
