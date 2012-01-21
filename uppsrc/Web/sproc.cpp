#include "Web.h"

#ifdef PLATFORM_POSIX
//#BLITZ_APPROVE
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#endif

NAMESPACE_UPP

/* Mirek:unused
static void WriteLog(const String& s)
{
#ifdef PLATFORM_POSIX
	if(s.IsEmpty())
		return;
	int fd = open("con", O_WRONLY | O_APPEND);
	if(fd >= 0)
		write(fd, s, s.GetLength());
	close(fd);
#endif
}
*/

#define DO_SVRLOG 0

#if DO_SVRLOG
#define SVRLOG(x) { StringStream ss; ss << x << '\n'; RLOG(ss.GetResult()); WriteLog(ss.GetResult()); }
#else
#define SVRLOG(x)
#endif

class LocalSlaveProcess : public SlaveProcess
{
public:
	typedef LocalSlaveProcess CLASSNAME;
	LocalSlaveProcess()                                            { Init(); }
	LocalSlaveProcess(const char *cmdline, const char *envptr = 0) { Init(); Open(cmdline, envptr); }
	virtual ~LocalSlaveProcess()                                   { Kill(); }

	void         Open(const char *cmdline, const char *envptr = 0);

	virtual void Kill();
	virtual bool IsRunning();
	virtual void Write(String s);
	virtual bool Read(String& s);
	virtual int  GetExitCode();
	virtual void Detach();

private:
	void         Init();
	void         Free();
#ifdef PLATFORM_POSIX
	bool         DecodeExitCode(int code);
#endif

private:
#ifdef PLATFORM_WIN32
	HANDLE       hProcess;
	HANDLE       hOutputRead;
	HANDLE       hInputWrite;
#endif
#ifdef PLATFORM_POSIX
	Buffer<char> cmd_buf;
	Vector<char *> args;
	pid_t        pid;
	int          rpipe[2], wpipe[2];
	String       exit_string;
	bool         output_read;
#endif
	int          exit_code;
};

One<SlaveProcess> StartLocalProcess(const char *cmdline, const char *envptr)
{
	return new LocalSlaveProcess(cmdline, envptr);
}

void LocalSlaveProcess::Init() {
#ifdef PLATFORM_WIN32
	hProcess = hOutputRead = hInputWrite = NULL;
#endif
#ifdef PLATFORM_POSIX
	pid = 0;
	rpipe[0] = rpipe[1] = wpipe[0] = wpipe[1] = -1;
	output_read = false;
#endif
	exit_code = Null;
}

void LocalSlaveProcess::Free() {
#ifdef PLATFORM_WIN32
	if(hProcess) {
		CloseHandle(hProcess);
		hProcess = NULL;
	}
	if(hOutputRead) {
		CloseHandle(hOutputRead);
		hOutputRead = NULL;
	}
	if(hInputWrite) {
		CloseHandle(hInputWrite);
		hInputWrite = NULL;
	}
#endif
#ifdef PLATFORM_POSIX
	SVRLOG("\nLocalSlaveProcess::Free, pid = " << (int)getpid());
	SVRLOG("rpipe[" << rpipe[0] << ", " << rpipe[1] << "]");
	SVRLOG("wpipe[" << wpipe[0] << ", " << wpipe[1] << "]");
	if(rpipe[0] >= 0) { close(rpipe[0]); rpipe[0] = -1; }
	if(rpipe[1] >= 0) { close(rpipe[1]); rpipe[1] = -1; }
	if(wpipe[0] >= 0) { close(wpipe[0]); wpipe[0] = -1; }
	if(wpipe[1] >= 0) { close(wpipe[1]); wpipe[1] = -1; }
	if(pid) waitpid(pid, 0, WNOHANG | WUNTRACED);
	pid = 0;
	output_read = false;
#endif
}

void LocalSlaveProcess::Open(const char *command, const char *envptr) {
	SVRLOG("LocalSlaveProcess::Open(" << command << ")");

	Kill();

	while(*command && (byte)*command <= ' ')
		command++;

#ifdef PLATFORM_WIN32
	HANDLE hOutputReadTmp, hInputRead;
	HANDLE hInputWriteTmp, hOutputWrite;
	HANDLE hErrorWrite;
	SECURITY_ATTRIBUTES sa;

	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;

	HANDLE hp = GetCurrentProcess();

	CreatePipe(&hOutputReadTmp, &hOutputWrite, &sa, 0);
	DuplicateHandle(hp, hOutputWrite, hp, &hErrorWrite, 0, TRUE, DUPLICATE_SAME_ACCESS);
	CreatePipe(&hInputRead, &hInputWriteTmp, &sa, 0);
	DuplicateHandle(hp, hOutputReadTmp, hp, &hOutputRead, 0, FALSE, DUPLICATE_SAME_ACCESS);
	DuplicateHandle(hp, hInputWriteTmp, hp, &hInputWrite, 0, FALSE, DUPLICATE_SAME_ACCESS);
	CloseHandle(hOutputReadTmp);
	CloseHandle(hInputWriteTmp);

	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_HIDE;
	si.hStdInput  = hInputRead;
	si.hStdOutput = hOutputWrite;
	si.hStdError  = hErrorWrite;
	int n = (int)strlen(command) + 1;
	Buffer<char> cmd(n);
	memcpy(cmd, command, n);
	bool h = CreateProcess(NULL, cmd, &sa, &sa, TRUE,
	                       NORMAL_PRIORITY_CLASS, (void *)envptr, NULL, &si, &pi);
	SVRLOG("CreateProcess " << (h ? "succeeded" : "failed"));
	CloseHandle(hErrorWrite);
	CloseHandle(hInputRead);
	CloseHandle(hOutputWrite);
	if(h) {
		hProcess = pi.hProcess;
		CloseHandle(pi.hThread);
	}
	else {
		Free();
		throw Exc(NFormat("Error running process: %s\nCommand: %s", GetErrorMessage(GetLastError()), command));
	}
#endif
#ifdef PLATFORM_POSIX
	// parse command line for execve
	cmd_buf.Alloc(strlen(command) + 1);
	char *cmd_out = cmd_buf;
	const char *p = command;
	const char *b = p;
	while(*p && (byte)*p > ' ')
		if(*p++ == '\"')
			while(*p && *p++ != '\"')
				;
	const char *app = cmd_out;
	args.Add(cmd_out);
	memcpy(cmd_out, b, p - b);
	cmd_out += p - b;
	*cmd_out++ = '\0';

	while(*p)
		if((byte)*p <= ' ')
			p++;
		else {
			args.Add(cmd_out);
			while(*p && (byte)*p > ' ')
				if(*p == '\\') {
					if(*++p)
						*cmd_out++ = *p++;
				}
				else if(*p == '\"') {
					p++;
					while(*p && *p != '\"')
						if(*p == '\\') {
							if(*++p)
								*cmd_out++ = *p++;
						}
						else
							*cmd_out++ = *p++;
					if(*p == '\"')
						p++;
				}
				else
					*cmd_out++ = *p++;
			*cmd_out++ = '\0';
		}

	args.Add(NULL);

	String app_full = GetFileOnPath(app, getenv("PATH"), true);
	if(IsNull(app_full))
		throw Exc(Format("Cannot find executable '%s'\n", app));

	if(pipe(rpipe) || pipe(wpipe))
		throw Exc(NFormat(t_("pipe() error; error code = %d"), errno));
	SVRLOG("\nLocalSlaveProcess::Open");
	SVRLOG("rpipe[" << rpipe[0] << ", " << rpipe[1] << "]");
	SVRLOG("wpipe[" << wpipe[0] << ", " << wpipe[1] << "]");
	pid = fork();
	SVRLOG("\tfork, pid = " << (int)pid << ", getpid = " << (int)getpid());
	if(pid < 0)
		throw Exc(NFormat(t_("fork() error; error code = %d"), errno));
	if(pid)
	{ // parent process; clear child pipe endpoints
		SVRLOG("parent process - continue");
//		rpipe[0] = wpipe[1] = -1;
		return;
	}
	SVRLOG("child process - execute application");
//	rpipe[1] = wpipe[0] = -1;
	if(dup2(rpipe[0], 0) < 0)
	{ // stdin
		SVRLOG("dup2(stdin) error: " << errno << ", " << strerror(errno));
	}
	if(dup2(wpipe[1], 1) < 0)
	{ // stdout
		SVRLOG("dup2(stdout) error: " << errno << ", " << strerror(errno));
	}
	if(dup2(wpipe[1], 2) < 0)
	{ // stderr
		SVRLOG("dup2(stderr) error: " << errno << ", " << strerror(errno));
	}

#if DO_SVRLOG
	SVRLOG(args.GetCount() << "arguments:");
	for(int a = 0; a < args.GetCount(); a++)
		SVRLOG("[" << a << "]: <" << (args[a] ? args[a] : "NULL") << ">");
#endif//DO_SVRLOG

	SVRLOG("running execve, app = " << app << ", #args = " << args.GetCount());
	const char *from = envptr;
	Vector<const char *> env;
	while(*from) {
		env.Add(from);
		from += strlen(from) + 1;
	}
	env.Add(NULL);
	execve(app_full, args.Begin(), (char *const *)env.Begin());
	SVRLOG("execve failed, errno = " << errno);
	printf("Error running '%s', error code %d\n", command, errno);
	exit(-errno);
#endif
}

#ifdef PLATFORM_POSIX
bool LocalSlaveProcess::DecodeExitCode(int status)
{
	if(WIFEXITED(status)) {
		exit_code = (byte)WEXITSTATUS(status);
		return true;
	}
	else if(WIFSIGNALED(status) || WIFSTOPPED(status)) {
		static const struct {
			const char *name;
			int         code;
		}
		signal_map[] = {
#define SIGDEF(s) { #s, s },
SIGDEF(SIGHUP) SIGDEF(SIGINT) SIGDEF(SIGQUIT) SIGDEF(SIGILL) SIGDEF(SIGABRT)
SIGDEF(SIGFPE) SIGDEF(SIGKILL) SIGDEF(SIGSEGV) SIGDEF(SIGPIPE) SIGDEF(SIGALRM)
SIGDEF(SIGPIPE) SIGDEF(SIGTERM) SIGDEF(SIGUSR1) SIGDEF(SIGUSR2) SIGDEF(SIGTRAP)
SIGDEF(SIGURG) SIGDEF(SIGVTALRM) SIGDEF(SIGXCPU) SIGDEF(SIGXFSZ) SIGDEF(SIGIOT)
SIGDEF(SIGIO) SIGDEF(SIGWINCH)
#ifndef PLATFORM_BSD
//SIGDEF(SIGCLD) SIGDEF(SIGPWR)
#endif
//SIGDEF(SIGSTKFLT) SIGDEF(SIGUNUSED) // not in Solaris, make conditional if needed
#undef SIGDEF
		};

		int sig = (WIFSIGNALED(status) ? WTERMSIG(status) : WSTOPSIG(status));
		exit_code = (WIFSIGNALED(status) ? 1000 : 2000) + sig;
		exit_string << "\nProcess " << (WIFSIGNALED(status) ? "terminated" : "stopped") << " on signal " << sig;
		for(int i = 0; i < __countof(signal_map); i++)
			if(signal_map[i].code == sig)
			{
				exit_string << " (" << signal_map[i].name << ")";
				break;
			}
		exit_string << "\n";
		return true;
	}
	return false;
}
#endif//PLATFORM_POSIX

void LocalSlaveProcess::Kill() {
#ifdef PLATFORM_WIN32
	if(hProcess && IsRunning()) {
		TerminateProcess(hProcess, (DWORD)-1);
		exit_code = 255;
	}
#endif
#ifdef PLATFORM_POSIX
	if(IsRunning()) {
		SVRLOG("\nLocalSlaveProcess::Kill, pid = " << (int)pid);
		exit_code = 255;
		kill(pid, SIGTERM);
		GetExitCode();
		int status;
		if(pid && waitpid(pid, &status, 0) == pid)
			DecodeExitCode(status);
		exit_string = "Child process has been killed.\n";
	}
#endif
	Free();
}

void LocalSlaveProcess::Detach()
{
	Free();
}

bool LocalSlaveProcess::IsRunning() {
#ifdef PLATFORM_WIN32
	dword exitcode;
	if(!hProcess)
		return false;
	if(GetExitCodeProcess(hProcess, &exitcode) && exitcode == STILL_ACTIVE)
		return true;
	dword n;
	if(PeekNamedPipe(hOutputRead, NULL, 0, NULL, &n, NULL) && n)
		return true;
	exit_code = exitcode;
	return false;
#endif
#ifdef PLATFORM_POSIX
	if(!pid || !IsNull(exit_code)) {
		SVRLOG("IsRunning() -> no");
		return false;
	}
	int status = 0, wp;
	if((wp = waitpid(pid, &status, WNOHANG | WUNTRACED)) != pid || !DecodeExitCode(status))
		return true;
	SVRLOG("IsRunning() -> no, just exited, exit code = " << exit_code);
	return false;
#endif
}

int  LocalSlaveProcess::GetExitCode() {
#ifdef PLATFORM_WIN32
	return IsRunning() ? (int)Null : exit_code;
#endif
#ifdef PLATFORM_POSIX
	if(!IsRunning())
		return Nvl(exit_code, -1);
	int status;
	if(waitpid(pid, &status, WNOHANG | WUNTRACED) != pid || !DecodeExitCode(status)) {
		SVRLOG("GetExitCode() -> -1 (waitpid would hang)");
		return -1;
	}
	exit_code = WEXITSTATUS(status);
	SVRLOG("GetExitCode() -> " << exit_code << " (just exited)");
	return exit_code;
#endif
}

bool LocalSlaveProcess::Read(String& res) {
	res = Null;
#ifdef PLATFORM_WIN32
	if(!hOutputRead) return false;
	dword n;
	if(!PeekNamedPipe(hOutputRead, NULL, 0, NULL, &n, NULL) || n == 0)
		return IsRunning();
	char buffer[1024];
	if(!ReadFile(hOutputRead, buffer, sizeof(buffer), &n, NULL))
		return false;
	res.Cat(buffer, n);
	return true;
#endif
#ifdef PLATFORM_POSIX
//??!
	if(wpipe[0] < 0) return false;
	bool was_running = IsRunning();
//	SVRLOG("output_read = " << (output_read ? "yes" : "no"));
	if(!was_running && output_read) {
		if(exit_string.IsEmpty())
			return false;
		res = exit_string;
		exit_string = Null;
		return true;
	}
	fd_set set[1];
	FD_ZERO(set);
	FD_SET(wpipe[0], set);
	timeval tval = { 0, 0 };
	int sv;
	while((sv = select(wpipe[0] + 1, set, NULL, NULL, &tval)) > 0) {
		SVRLOG("Read() -> select");
		char buffer[1024];
		int done = read(wpipe[0], buffer, sizeof(buffer));
//		printf("pipe = %d, done %d bytes: %.*s\n", wpipe[0], done, done, buffer);
		SVRLOG("Read(), read -> " << done << ": " << String(buffer, done));
		if(done > 0)
			res.Cat(buffer, done);
	}
	if(sv < 0) {
		SVRLOG("select -> " << sv);
	}
	if(!was_running)
		output_read = true;
	return !IsNull(res) || was_running;
#endif
}

void LocalSlaveProcess::Write(String s)
{
#ifdef PLATFORM_WIN32
	dword n;
	WriteFile(hInputWrite, s, s.GetLength(), &n, NULL);
#endif
#ifdef PLATFORM_POSIX
	IGNORE_RESULT(
		write(rpipe[1], s, s.GetLength())
	);
#endif
}

class RemoteSlaveProcess : public SlaveProcess
{
public:
	RemoteSlaveProcess() { Init(); }
	RemoteSlaveProcess(const char *host, int port, const char *cmdline,
		const char *envptr = NULL, int timeout = Null)
	{ Init(); Open(host, port, cmdline, envptr, timeout); }
	virtual ~RemoteSlaveProcess();

	void         Open(const char *host, int port, const char *cmdline,
		const char *envptr = NULL, int timeout = Null);

	virtual void Kill();
	virtual bool IsRunning();
	virtual void Write(String s);
	virtual bool Read(String& s);
	virtual int  GetExitCode();

	enum
	{
		DEFAULT_PORT = 2346,
		REQUEST_TIMEOUT = 1000,
	};

private:
	void         Init();
	void         Recv(int part, int timeout);

private:
	Socket       socket;
	String       output[3];
	bool         terminated;
	int          current_part;
	int          timeout;
};

One<SlaveProcess> StartRemoteProcess(const char *host, int port, const char *cmdline, const char *envptr, int timeout)
{
	return new RemoteSlaveProcess(host, port, cmdline, envptr, timeout);
}

void RemoteSlaveProcess::Init()
{
	terminated = true;
}

RemoteSlaveProcess::~RemoteSlaveProcess()
{
}

void RemoteSlaveProcess::Open(const char *host, int port, const char *cmdline, const char *envptr, int to)
{
	SVRLOG("RemoteSlaveProcess(" << host << ":" << port << ")=" << cmdline);
	Kill();

	timeout = to;
	Socket::Init();
	String localhost;
	if(host == 0 || *host == 0)
	{
		localhost = Socket::GetHostName();
		host = localhost;
	}
	if(port == 0)
		port = DEFAULT_PORT;
	terminated = false;
	current_part = 0;
	output[0] = output[1] = output[2] = Null;
	if(!ClientSocket(socket, host, port, true, NULL, REQUEST_TIMEOUT))
		throw Exc(NFormat(t_("Opening host '%s' / port %d failed, error = %s"), host, port, Socket::GetErrorText()));
	int len = (int)strlen(cmdline);
	if(envptr && *envptr) {
		const char *e = envptr;
		while(*e)
			e = e + strlen(e) + 1;
		socket.Write(":");
		socket.Write(ASCII85Encode(String(envptr, e + 1)));
		socket.Write("\n");
	}
	socket.Write("=");
	socket.Write(cmdline, len + 1); // send terminating 0 as well
	Recv(0, timeout);
	if(output[0][0] == '-')
		throw Exc(NFormat(t_("Error running process: %s\nCommand: %s"), output[0].Begin() + 1, cmdline));

	if(output[0] != "+")
		throw Exc(NFormat(t_("Communication error; process = %s"), cmdline));
}

void RemoteSlaveProcess::Kill()
{
	socket.Close();
	terminated = true;
}

bool RemoteSlaveProcess::IsRunning()
{
	Recv(0, Null);
	return !terminated;
}

void RemoteSlaveProcess::Write(String s)
{
	ASSERT(socket.IsOpen());
	socket.Write(s);
}

bool RemoteSlaveProcess::Read(String& s)
{
	Recv(0, Null);
	s = output[1];
	output[1] = Null;
	return !terminated || !s.IsEmpty();
}

void RemoteSlaveProcess::Recv(int part, int timeout)
{
	if(terminated)
		return;
	if(!socket.IsOpen())
	{
		SVRLOG("-> broken pipe");
		terminated = true;
		return;
	}
//	LOG("RemoteSlaveProcess::Recv(" << part << ")");

	int starttick = GetTickCount();
	while( (IsNull(timeout) || int(GetTickCount())-starttick <= timeout) && (socket.Peek() || current_part <= part))
	{
		String data = socket.Read(0);
		SVRLOG("-> [" << current_part << "] = " << data.GetLength() << " bytes: <" << data << ">");
		if(data.IsVoid())
		{
			SVRLOG("-> broken pipe");
			terminated = true;
			return;
		}
		const char *p = data, *e = data.End();
		ASSERT(current_part >= 0 && current_part < __countof(output));
		for(const char *t; e > p && (t = (const char *)memchr(p, 0, e - p));)
		{
			output[current_part].Cat(p, int(t - p));
			p = t + 1;
			if(++current_part >= __countof(output))
			{
				SVRLOG("-> EOF");
				terminated = true;
				return;
			}
		}
		if(p < e)
			output[current_part].Cat(p, int(e - p));
	}
	SVRLOG("-> finished, current_part = " << current_part);
}

int RemoteSlaveProcess::GetExitCode()
{
	if(!terminated)
		return Null;
	Recv(2, timeout);
	return StrInt(output[2]);
}

One<SlaveProcess> StartProcess(const char *cmdline, const char *envptr, int timeout)
{
	if(*cmdline != '(')
		return StartLocalProcess(cmdline, envptr);
	const char *b = cmdline + 1, *p = b;
	while(*p && *p != ')' && *p != ':')
		p++;
	String host(b, p);
	int port = 0;
	if(*p == ':')
		port = stou(p + 1, &p);
	while(*p && *p++ != ')')
		;
	if(*p == 0)
		throw Exc(NFormat(t_("Missing command line (host = %s)."), host));
	One<RemoteSlaveProcess> rsp = new RemoteSlaveProcess(host, port, p, envptr, timeout);
	return rsp.Detach();
}

END_UPP_NAMESPACE
