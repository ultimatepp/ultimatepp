#include "Core.h"

NAMESPACE_UPP

#ifdef PLATFORM_POSIX
//#BLITZ_APPROVE
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#endif

#define LLOG(x) // DLOG(x)

void LocalProcess::Init() {
#ifdef PLATFORM_WIN32
	hProcess = hOutputRead = hInputWrite = NULL;
#endif
#ifdef PLATFORM_POSIX
	pid = 0;
	rpipe[0] = rpipe[1] = wpipe[0] = wpipe[1] = epipe[0] = epipe[1] = -1;
#endif
	exit_code = Null;
	convertcharset = true;
}

void LocalProcess::Free() {
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
	LLOG("\nLocalProcess::Free, pid = " << (int)getpid());
	LLOG("rpipe[" << rpipe[0] << ", " << rpipe[1] << "]");
	LLOG("wpipe[" << wpipe[0] << ", " << wpipe[1] << "]");
	if(rpipe[0] >= 0) { close(rpipe[0]); rpipe[0] = -1; }
	if(rpipe[1] >= 0) { close(rpipe[1]); rpipe[1] = -1; }
	if(wpipe[0] >= 0) { close(wpipe[0]); wpipe[0] = -1; }
	if(wpipe[1] >= 0) { close(wpipe[1]); wpipe[1] = -1; }
	if(epipe[0] >= 0) { close(epipe[0]); epipe[0] = -1; }
	if(epipe[1] >= 0) { close(epipe[1]); epipe[1] = -1; }
	if(pid) waitpid(pid, 0, WNOHANG | WUNTRACED);
	pid = 0;
#endif
}

bool LocalProcess::Start(const char *command, bool spliterr, const char *envptr)
{
	LLOG("LocalProcess::Start(\"" << command << "\")");

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

	ASSERT(!spliterr); //spliterr NOT IMPLEMENTED (yet)

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
	LLOG("CreateProcess " << (h ? "succeeded" : "failed"));
	CloseHandle(hErrorWrite);
	CloseHandle(hInputRead);
	CloseHandle(hOutputWrite);
	if(h) {
		hProcess = pi.hProcess;
		CloseHandle(pi.hThread);
	}
	else {
		Free();
		return false;
//		throw Exc(NFormat("Error running process: %s\nCommand: %s", GetErrorMessage(GetLastError()), command));
	}
	return true;
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
		return false;
//		throw Exc(Format("Cannot find executable '%s'\n", app));

	if(pipe(rpipe) || pipe(wpipe))
		return false;
//		throw Exc(NFormat(t_("pipe() error; error code = %d"), errno));

	if(spliterr && pipe(epipe))
		return false;
//		throw Exc(NFormat(t_("pipe() error; error code = %d"), errno));

	LLOG("\nLocalProcess::Start");
	LLOG("rpipe[" << rpipe[0] << ", " << rpipe[1] << "]");
 
 	LLOG("wpipe[" << wpipe[0] << ", " << wpipe[1] << "]");

 	LLOG("epipe[" << epipe[0] << ", " << epipe[1] << "]");
#ifdef CPU_BLACKFIN
	pid = vfork(); //we *can* use vfork here, since exec is done later or the parent will exit
#else
	pid = fork();
#endif
	LLOG("\tfork, pid = " << (int)pid << ", getpid = " << (int)getpid());
	if(pid < 0)
		return false;
//		throw Exc(NFormat(t_("fork() error; error code = %d"), errno));

	if(pid) {
		LLOG("parent process - continue");
		close(rpipe[0]); rpipe[0]=-1;
		close(wpipe[1]); wpipe[1]=-1;
		if (spliterr) {
			close(epipe[1]); epipe[1]=-1;
		}
		return true;
	}
	LLOG("child process - execute application");
//	rpipe[1] = wpipe[0] = -1;
	dup2(rpipe[0], 0);
	dup2(wpipe[1], 1);
	dup2(spliterr ? epipe[1] : wpipe[1], 2);
	close(rpipe[0]);
	close(rpipe[1]);
	close(wpipe[0]);
	close(wpipe[1]);
	if (spliterr) {
		close(epipe[0]);
		close(epipe[1]);
	}
	rpipe[0] = rpipe[1] = wpipe[0] = wpipe[1] = epipe[0] = epipe[1] = -1;

#if DO_LLOG
	LLOG(args.GetCount() << "arguments:");
	for(int a = 0; a < args.GetCount(); a++)
		LLOG("[" << a << "]: <" << (args[a] ? args[a] : "NULL") << ">");
#endif//DO_LLOG

	LLOG("running execve, app = " << app << ", #args = " << args.GetCount());
	if(envptr) {
		const char *from = envptr;
		Vector<const char *> env;
		while(*from) {
			env.Add(from);
			from += strlen(from) + 1;
		}
		env.Add(NULL);
		execve(app_full, args.Begin(), (char *const *)env.Begin());
	}
	else
		execv(app_full, args.Begin());
	LLOG("execve failed, errno = " << errno);
//	printf("Error running '%s', error code %d\n", command, errno);
	exit(-errno);
	return true;
#endif
}

#ifdef PLATFORM_POSIX
bool LocalProcess::DecodeExitCode(int status)
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

void LocalProcess::Kill() {
#ifdef PLATFORM_WIN32
	if(hProcess && IsRunning()) {
		TerminateProcess(hProcess, (DWORD)-1);
		exit_code = 255;
	}
#endif
#ifdef PLATFORM_POSIX
	if(IsRunning()) {
		LLOG("\nLocalProcess::Kill, pid = " << (int)pid);
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

void LocalProcess::Detach()
{
	Free();
}

bool LocalProcess::IsRunning() {
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
		LLOG("IsRunning() -> no");
		return false;
	}
	int status = 0, wp;
	if(!( (wp = waitpid(pid, &status, WNOHANG | WUNTRACED)) == pid && 
	      DecodeExitCode(status) ))
		return true;
	LLOG("IsRunning() -> no, just exited, exit code = " << exit_code);
	return false;
#endif
}

int  LocalProcess::GetExitCode() {
#ifdef PLATFORM_WIN32
	return IsRunning() ? (int)Null : exit_code;
#endif
#ifdef PLATFORM_POSIX
	if(!IsRunning())
		return Nvl(exit_code, -1);
	int status;
	if(!( waitpid(pid, &status, WNOHANG | WUNTRACED) == pid && 
	      DecodeExitCode(status) ))
		return -1;
	LLOG("GetExitCode() -> " << exit_code << " (just exited)");
	return exit_code;
#endif
}

String LocalProcess::GetExitMessage() {
#ifdef PLATFORM_POSIX
	if (!IsRunning() && GetExitCode() == -1)
		return exit_string;
	else
#endif
		return String();
}

bool LocalProcess::Read(String& res) {
#ifdef PLATFORM_POSIX
	return Read2(res, res);
#endif
#ifdef PLATFORM_WIN32
	LLOG("LocalProcess::Read");
	res = Null;
	if(!hOutputRead) return false;
	dword n;
	if(!PeekNamedPipe(hOutputRead, NULL, 0, NULL, &n, NULL) || n == 0)
		return IsRunning();
	char buffer[1024];
	if(!ReadFile(hOutputRead, buffer, sizeof(buffer), &n, NULL))
		return false;
	res = String(buffer, n);
	if(convertcharset)
		res = FromSystemCharset(res);
	return true;
#endif
}

bool LocalProcess::Read2(String& reso, String& rese)
{
	LLOG("LocalProcess::Read2");
	String res[2] = {Null, Null};
#ifdef PLATFORM_WIN32
	NEVER(); //Not implemented
#endif
#ifdef PLATFORM_POSIX
	bool was_running = IsRunning() || wpipe[0] >= 0 || epipe[0] >= 0;
	for (int wp=0; wp<2;wp++) {
		int *pipe = wp ? epipe : wpipe;
		if (pipe[0] < 0) {
			LLOG("Pipe["<<wp<<"] closed");
			continue;
		}
		fd_set set[1];
		FD_ZERO(set);
		FD_SET(pipe[0], set);
		timeval tval = { 0, 0 };
		int sv;
		while((sv = select(pipe[0]+1, set, NULL, NULL, &tval)) > 0) {
			LLOG("Read() -> select");
			char buffer[1024];
			int done = read(pipe[0], buffer, sizeof(buffer));
			LLOG("Read(), read -> " << done << ": " << String(buffer, done));
			if(done > 0)
				res[wp].Cat(buffer, done);
			else if (done == 0) {
				close(pipe[0]);
				pipe[0] = -1;
			}
		}
		LLOG("Pipe["<<wp<<"]=="<<pipe[0]<<" sv:"<<sv);
		if(sv < 0) {
			LLOG("select -> " << sv);
		}
	}
	reso = Null;
	rese = Null;
	if(convertcharset) {
		reso << FromSystemCharset(res[0]);
		rese << FromSystemCharset(res[1]);
	} else {
		reso << res[0];
		rese << res[1];
	}
	return !IsNull(res[0]) || !IsNull(res[1]) || was_running;
#endif
}

void LocalProcess::Write(String s)
{
	if(convertcharset)
		s = ToSystemCharset(s);
#ifdef PLATFORM_WIN32
	dword n;
	if (hInputWrite)
		WriteFile(hInputWrite, s, s.GetLength(), &n, NULL);
#endif
#ifdef PLATFORM_POSIX
	if (rpipe[1] >= 0) {
		int ret=1;
		for (int wn=0; (ret>0 || errno==EINTR) && wn<s.GetLength(); wn+=ret) {
			ret = write(rpipe[1], s + wn, s.GetLength() - wn);
		}
	}
#endif
}

void LocalProcess::CloseRead()
{
#ifdef PLATFORM_WIN32
	if(hOutputRead) {
		CloseHandle(hOutputRead);
		hOutputRead = NULL;
	}
#endif
#ifdef PLATFORM_POSIX
	if (wpipe[0] >= 0) {
		close(wpipe[0]);
		wpipe[0]=-1;
	}
#endif
}

void LocalProcess::CloseWrite()
{
#ifdef PLATFORM_WIN32
	if(hInputWrite) {
		CloseHandle(hInputWrite);
		hInputWrite = NULL;
	}
#endif
#ifdef PLATFORM_POSIX
	if (rpipe[1] >= 0) {
		close(rpipe[1]);
		rpipe[1]=-1;
	}
#endif
}

int Sys(const char *cmd, String& out, bool convertcharset)
{
	out.Clear();
	LocalProcess p;
	p.ConvertCharset(convertcharset);
	if(!p.Start(cmd))
		return -1;
	while(p.IsRunning()) {
		out.Cat(p.Get());
		Sleep(1); // p.Wait would be much better here!
	}
	out.Cat(p.Get());
	return p.GetExitCode();
}

String Sys(const char *cmd, bool convertcharset)
{
	String r;
	return Sys(cmd, r, convertcharset) ? String::GetVoid() : r;
}

END_UPP_NAMESPACE
