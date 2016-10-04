#include "Core.h"

#define LLOG(x)

#include <plugin/bz2/bz2.h>

String LocalHost::GetEnvironment()
{
	return environment;
}

String LocalHost::GetHostPath(const String& path)
{
	return path;
}

String LocalHost::GetLocalPath(const String& path)
{
	return path;
}

String LocalHost::NormalizePath(const String& path)
{
	return ::NormalizePath(path);
}

Vector<Host::FileInfo> LocalHost::GetFileInfo(const Vector<String>& path)
{
	Vector<FileInfo> fi;
	for(int i = 0; i < path.GetCount(); i++) {
		FindFile ff(path[i]);
		FileInfo& f = fi.Add();
		if(ff) {
			(Time&)f = ff.GetLastWriteTime();
			f.length = ff.IsFile() ? (int)ff.GetLength() : -1;
		}
		else {
			(Time&)f = Time::Low();
			f.length = Null;
		}
	}
	return fi;
}

void LocalHost::DeleteFile(const Vector<String>& path)
{
	for(int i = 0; i < path.GetCount(); i++)
		::DeleteFile(path[i]);
}

void LocalHost::DeleteFolderDeep(const String& folder)
{
	::DeleteFolderDeep(folder);
}

void LocalHost::ChDir(const String& path)
{
#ifdef PLATFORM_WIN32
	SetCurrentDirectory(path);
#endif
#ifdef PLATFORM_POSIX
	IGNORE_RESULT( chdir(path) );
#endif
	if(cmdout)
		*cmdout << "cd \"" << GetHostPath(path) << "\"\n";
}

void LocalHost::DoDir(const String& dir)
{
	if(dir.GetLength() > 3) {
		DoDir(GetFileFolder(dir));
		*cmdout << "mkdir \"" << dir << "\"\n";
	}
}

void LocalHost::RealizeDir(const String& path)
{
	RealizeDirectory(path);
	if(cmdout)
		DoDir(path);
}

void    LocalHost::SaveFile(const String& path, const String& data)
{
	::SaveFile(path, data);
}

String  LocalHost::LoadFile(const String& path)
{
	return ::LoadFile(path);
}

int LocalHost::Execute(const char *cmdline)
{
	if(cmdout)
		*cmdout << cmdline << '\n';
	PutVerbose(cmdline);
	int q = IdeConsoleExecute(FindCommand(exedirs, cmdline), NULL, environment, false);
	PutVerbose(Format("Exitcode: %d", q));
	return q;
}

int LocalHost::ExecuteWithInput(const char *cmdline, bool noconvert)
{
	if(cmdout)
		*cmdout << cmdline << '\n';
	PutVerbose(cmdline);
	int q = IdeConsoleExecuteWithInput(FindCommand(exedirs, cmdline), NULL, environment, false, noconvert);
	PutVerbose(Format("Exitcode: %d", q));
	return q;
}

int LocalHost::Execute(const char *cmdline, Stream& out, bool noconvert)
{
	PutVerbose(cmdline);
	int q = IdeConsoleExecute(FindCommand(exedirs, cmdline), &out, environment, true, noconvert);
	PutVerbose(Format("Exitcode: %d", q));
	return q;
}

int LocalHost::AllocSlot()
{
	return IdeConsoleAllocSlot();
}

bool LocalHost::Run(const char *cmdline, int slot, String key, int blitz_count)
{
	return IdeConsoleRun(FindCommand(exedirs, cmdline), NULL, environment, false, slot, key, blitz_count);
}

bool LocalHost::Run(const char *cmdline, Stream& out, int slot, String key, int blitz_count)
{
	return IdeConsoleRun(FindCommand(exedirs, cmdline), &out, environment, true, slot, key, blitz_count);
}

bool LocalHost::Wait()
{
	return IdeConsoleWait();
}

bool LocalHost::Wait(int slot)
{
	return IdeConsoleWait(slot);
}

void LocalHost::OnFinish(Event<>  cb)
{
	IdeConsoleOnFinish(cb);
}

One<AProcess> LocalHost::StartProcess(const char *cmdline)
{
	try {
		PutVerbose(cmdline);
		One<AProcess> p;
		if(p.Create<LocalProcess>().Start(FindCommand(exedirs, cmdline), environment))
			return p;
	}
	catch(...) {
	}
	return NULL;
}

#ifdef PLATFORM_POSIX
//#BLITZ_APPROVE
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

static Vector<int>& sPid()
{
	static Vector<int> q;
	return q;
}

void sCleanZombies(int signal_number)
{
	Vector<int>& pid = sPid();
	int i = 0;
	while(i < pid.GetCount())
		if(pid[i] && waitpid(pid[i], 0, WNOHANG | WUNTRACED) > 0)
			pid.Remove(i);
		else
			i++;
}
#endif

String LinuxHostConsole = "/usr/bin/xterm -e";

void LocalHost::Launch(const char *_cmdline, bool console)
{
	String cmdline = FindCommand(exedirs, _cmdline);
	PutVerbose(cmdline);
#ifdef PLATFORM_WIN32
	if(console)
		cmdline = GetExeFilePath() + " ! " + cmdline;
	int n = cmdline.GetLength() + 1;
	Buffer<char> cmd(n);
	memcpy(cmd, cmdline, n);
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;
	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	String ev = ToSystemCharset(environment);
	Buffer<char> env(ev.GetCount() + 1);
	memcpy(env, ev, ev.GetCount() + 1);
	if(CreateProcess(NULL, cmd, &sa, &sa, TRUE,
		             NORMAL_PRIORITY_CLASS|CREATE_NEW_CONSOLE,
	                ~env, NULL, &si, &pi)) {
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
	else
		PutConsole("Unable to launch " + String(_cmdline));
#endif
#ifdef PLATFORM_POSIX
	String script = ConfigFile("console-script-" + AsString(getpid()) + ".tmp");
	String lc;
	static const char *term[] = {
		"/usr/bin/mate-terminal -x",
		"/usr/bin/gnome-terminal -x",
		"/usr/bin/konsole -e",
		"/usr/bin/xterm -e",
	};
	int ii = 0;
	for(;;) { // If (pre)defined terminal emulator is not available, try to find one
		int c = LinuxHostConsole.FindFirstOf(" ");
		lc = c < 0 ? LinuxHostConsole : LinuxHostConsole.Left(c);
		if(ii >= __countof(term) || FileExists(lc))
			break;
		LinuxHostConsole = term[ii++];
	}
	if(FileExists(lc)) {
		if(console) {
			FileStream out(script, FileStream::CREATE, 0777);
			out << "#!/bin/sh\n"
			    << cmdline << '\n'
			    << "echo \"<--- Finished, press any key to close the window --->\"\nread dummy\n";
			cmdline = LinuxHostConsole + " sh " + script;
		}
	}
	else
	if(LinuxHostConsole.GetCount())
		PutConsole("Warning: Terminal '" + lc + "' not found, executing in background.");
	Buffer<char> cmd_buf(strlen(cmdline) + 1);
	char *cmd_out = cmd_buf;
	Vector<char *> args;
	const char *p = cmdline;
	const char *b = p;
	while(*p && (byte)*p > ' ')
		if(*p++ == '\"')
			while(*p && *p++ != '\"')
				;
	args.Add(cmd_out);
	memcpy(cmd_out, b, p - b);
	cmd_out += p - b;
	*cmd_out++ = '\0';

	while(*p)
		if((byte)*p <= ' ')
			p++;
		else {
			args.Add(cmd_out);
			b = p;
			while(*p && (byte)*p > ' ')
				if(*p++ == '\"')
				{
					memcpy(cmd_out, b, p - b - 1);
					cmd_out += p - b - 1;
					b = p;
					while(*p && *p != '\"')
						p++;
					memcpy(cmd_out, b, p - b);
					cmd_out += p - b;
					if(*p == '\"')
						p++;
					b = p;
				}
			memcpy(cmd_out, b, p - b);
			cmd_out += p - b;
			*cmd_out++ = '\0';
		}

	args.Add(NULL);

	ONCELOCK {
		struct sigaction sigchld_action;
		memset(&sigchld_action, 0, sizeof(sigchld_action));
		sigchld_action.sa_handler = sCleanZombies;
		sigaction(SIGCHLD, &sigchld_action, NULL);
	}

	pid_t pid = fork();
	if(pid == 0)
	{
		const char *from = environment;
		Vector<const char *> env;
		while(*from) {
			env.Add(from);
			from += strlen(from) + 1;
		}
		env.Add(NULL);
		const char **envp = env.Begin();
		execve(args[0], args, (char *const *)envp);
		abort();
	}
	LLOG("Launch pid: " << pid);
	sPid().Add(pid);
#endif
}

void LocalHost::AddFlags(Index<String>& cfg)
{
#if   defined(PLATFORM_WIN32)
	cfg.Add("WIN32");
#endif

#ifdef PLATFORM_LINUX
	cfg.Add("LINUX");
#endif

#ifdef PLATFORM_POSIX
	cfg.Add("POSIX");
#endif

#ifdef PLATFORM_BSD
	cfg.Add("BSD");
#endif

#ifdef PLATFORM_FREEBSD
	cfg.Add("FREEBSD");
#endif

#ifdef PLATFORM_OPENBSD
	cfg.Add("OPENBSD");
#endif

#ifdef PLATFORM_NETBSD
	cfg.Add("NETBSD");
#endif

#ifdef PLATFORM_DRAGONFLY
	cfg.Add("DRAGONFLY");
#endif

#ifdef PLATFORM_SOLARIS
	cfg.Add("SOLARIS");
#endif

#ifdef PLATFORM_OSX11
	cfg.Add("OSX11");
#endif
}

#if 0
static bool IsSamePath(const char *a, const char *b, int count) {
	for(; --count >= 0; a++, b++)
		if(a != b && ToLower(*a) != ToLower(*b) && !((*a == '\\' || *a == '/') && (*b == '\\' || *b == '/')))
			return false;
	return true;
}

String RemoteHost::GetEnvironment()
{
	return environment;
}

String RemoteHost::GetHostPath(const String& path)
{
	bool slash = (os_type != "WINDOWS");
	for(int i = 0; i < path_map_local.GetCount(); i++) {
		String lc = path_map_local[i];
		if(path.GetLength() >= lc.GetLength() && IsSamePath(path, lc, lc.GetLength()))
		{
			String r = CatAnyPath(path_map_remote[i], path.Mid(lc.GetLength()));
			return slash ? UnixPath(r) : WinPath(r);
		}
	}
	return slash ? UnixPath(path) : WinPath(path);
}

String RemoteHost::GetLocalPath(const String& path)
{
	for(int i = 0; i < path_map_remote.GetCount(); i++) {
		String rc = path_map_remote[i];
		if(path.GetLength() >= rc.GetLength() && IsSamePath(path, rc, rc.GetLength()))
		   return path_map_local[i] + path.Mid(rc.GetLength());
	}
	if(!memcmp(path, "/cygdrive/", 10))
	{
		const char *s = path.Begin() + 10;
		String out;
		if(*s)
			out << *s++ << ':';
		out << s;
		return out;
	}
	return NativePath(path);
}

String RemoteHost::NormalizePath(const String& path)
{
	return path;
}

String RemoteHost::RemoteExec(String cmd)
{
	Socket socket;
	String sockerr;
/*
	String hostname = host;
	int port = 2346;
	int ppos = hostname.Find(':');
	if(ppos >= 0)
	{
		port = atoi(host.GetIter(ppos + 1));
		hostname.Trim(ppos);
	}
*/
	if(!ClientSocket(socket, host, port, true, NULL, 2000)) {
		PutConsole(NFormat("Error connecting to '%s', port %d: %s", host, port, Socket::GetErrorText()));
		return String::GetVoid();
	}
	socket.Write(cmd);
	socket.Write("\0", 1);
	return socket.ReadUntil('\0', Null, 10000000);
}

Vector<Host::FileInfo> RemoteHost::GetFileInfo(const Vector<String>& path)
{
	VectorMap<String, FileInfo> out;
	out.Reserve(path.GetCount());
	String request;
	request << "@" << (int)(GetSysTime() - TimeBase()) << ":";
	for(int i = 0; i < path.GetCount(); i++)
	{
		String hp = GetHostPath(path[i]);
		request << hp << "\n";
		FileInfo& fi = out.Add(hp);
		(Time&)fi = Time::Low();
		fi.length = Null;
	}
	String result = RemoteExec(request);
	const char *p = result;
	while(*p)
	{
		const char *b = p;
		while(*p && *p != '\n' && *p != '\t')
			p++;
		String fn(b, p);
		Time time = Time::Low();
		int size = Null;
		if(*p == '\t')
		{
			int t = ScanInt(p + 1, &p);
			if(!IsNull(t))
				time = TimeBase() + t;
			if(*p == '\t')
				size = ScanInt(p + 1, &p);
		}
		int ifn = out.Find(fn);
		if(ifn >= 0)
		{
			(Time&)out[ifn] = time;
			out[ifn].length = size;
		}
		while(*p && *p++ != '\n')
			;
	}
	return out.PickValues();
}

void RemoteHost::DeleteFile(const Vector<String>& path)
{
	String request = "-";
	for(int i = 0; i < path.GetCount(); i++)
		request << GetHostPath(path[i]) << "\n";
	String out = RemoteExec(request);
	if(!IsNull(out) && out != "OK")
		PutVerbose(out);
}

void RemoteHost::DeleteFolderDeep(const String& folder)
{
	String out = RemoteExec("~" + GetHostPath(folder));
	if(out != "OK")
		PutConsole(out);
}

void RemoteHost::ChDir(const String& path)
{
	chdir_path = GetHostPath(path);
}

void RemoteHost::RealizeDir(const String& path)
{
	RemoteExec("*" + GetHostPath(path));
}

void RemoteHost::SaveFile(const String& path, const String& data)
{
	String request;
	request << ">" << GetHostPath(path)
		<< "\t" << int(GetSysTime() - TimeBase())
		<< "\t" << data.GetLength()
		<< "\t" << ASCII85Encode(BZ2Compress(data))
		<< "\n";
	String out = RemoteExec(request);
	if(out != "OK")
		PutConsole(out);
}

String  RemoteHost::LoadFile(const String& path)
{
	String hpath = GetHostPath(path);
	String request = "^" + hpath;
	String out = RemoteExec(request);
	const char *p = out;
	while(*p && *p != '\n' && *p != '\t')
		p++;
	if(*p++ != '\t')
		return String::GetVoid();
	int len = ScanInt(p, &p);
	if(IsNull(len) || len <= 0 || *p++ != '\t')
		return String::GetVoid();
	String data = BZ2Decompress(ASCII85Decode(p));
	if(data.GetLength() != len)
	{
		PutConsole(NFormat("%s: decompressed length (%d) doesn't match length in header (%d)",
			hpath, data.GetLength(), len));
		return String::GetVoid();
	}
	return data;
}

int RemoteHost::Execute(const char *cmdline)
{
	int q = IdeConsoleExecute(StartProcess(cmdline), cmdline);
	PutVerbose(Format("Exitcode: %d", q));
	return q;
}

int RemoteHost::ExecuteWithInput(const char *cmdline)
{
	int q = IdeConsoleExecute(StartProcess(cmdline), cmdline);
	PutVerbose(Format("Exitcode: %d", q));
	return q;
}

int RemoteHost::Execute(const char *cmdline, Stream& out)
{
	int q = IdeConsoleExecute(StartProcess(cmdline), cmdline, &out, true);
	PutVerbose(Format("Exitcode: %d", q));
	return q;
}

int RemoteHost::AllocSlot()
{
	return IdeConsoleAllocSlot();
}

bool RemoteHost::Run(const char *cmdline, int slot, String key, int blitz_count)
{
	return IdeConsoleRun(StartProcess(cmdline), cmdline, NULL, false, slot, key, blitz_count);
}

bool RemoteHost::Run(const char *cmdline, Stream& out, int slot, String key, int blitz_count)
{
	return IdeConsoleRun(StartProcess(cmdline), cmdline, &out, false, slot, key, blitz_count);
}

bool RemoteHost::Wait()
{
	return IdeConsoleWait();
}

One<AProcess> RemoteHost::StartProcess(const char *cmdline)
{
	try {
		PutVerbose(cmdline);
		return StartRemoteProcess(host, port, cmdline, environment, REMOTE_TIMEOUT);
	}
	catch(...) {
		return NULL;
	}
}

void RemoteHost::Launch(const char *_cmdline, bool)
{
}

void RemoteHost::AddFlags(Index<String>& cfg)
{
	cfg.Add(os_type);
}
#endif