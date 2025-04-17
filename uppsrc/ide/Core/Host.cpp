#include "Core.h"

#include <plugin/bz2/bz2.h>

#define LLOG(x)
#define METHOD_NAME "Host::" << UPP_FUNCTION_NAME << "(): "

#ifdef FLATPAK
const String Host::CMDLINE_PREFIX = "host-spawn --no-pty ";
#else
const String Host::CMDLINE_PREFIX = "";
#endif

String Host::GetEnvironment()
{
	return environment;
}

void Host::AddEnvironment(const char *id, const char *value)
{
	bool b = *environment;
	if(b)
		environment.Trim(environment.GetCount() - 1);
	environment << id << '=' << value << '\0';
	if(b)
		environment << '\0';
}

Vector<Host::FileInfo> Host::GetFileInfo(const Vector<String>& path)
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

void Host::ChDir(const String& path)
{
#ifdef PLATFORM_WIN32
	SetCurrentDirectory(path);
#endif
#ifdef PLATFORM_POSIX
	IGNORE_RESULT(chdir(path));
#endif
	if(cmdout)
		*cmdout << "cd \"" << path << "\"\n";
}

void Host::DoDir(const String& dir)
{
	if(dir.GetLength() > 3) {
		DoDir(GetFileFolder(dir));
		*cmdout << "mkdir \"" << dir << "\"\n";
	}
}

bool Host::RealizeDir(const String& path)
{
	bool realized = RealizeDirectory(path);
	if(cmdout)
		DoDir(path);
	return realized;
}

int Host::Execute(const char *cmdline)
{
	if(cmdout)
		*cmdout << cmdline << '\n';
	Log(cmdline);
	int q = IdeConsoleExecute(FindCommand(exedirs, cmdline), NULL, environment, false);
	Log(Format("Exitcode: %d", q));
	return q;
}

int Host::ExecuteWithInput(const char *cmdline, bool noconvert)
{
	if(cmdout)
		*cmdout << cmdline << '\n';
	Log(cmdline);
	int q = IdeConsoleExecuteWithInput(FindCommand(exedirs, cmdline), NULL, environment, false, noconvert);
	Log(Format("Exitcode: %d", q));
	return q;
}

int Host::Execute(const char *cmdline, Stream& out, bool noconvert)
{
	Log(cmdline);
	int q = IdeConsoleExecute(FindCommand(exedirs, cmdline), &out, environment, true, noconvert);
	Log(Format("Exitcode: %d", q));
	return q;
}

int Host::AllocSlot()
{
	return IdeConsoleAllocSlot();
}

bool Host::Run(const char *cmdline, int slot, String key, int blitz_count)
{
	return IdeConsoleRun(FindCommand(exedirs, cmdline), NULL, environment, false, slot, key, blitz_count);
}

bool Host::Run(const char *cmdline, Stream& out, int slot, String key, int blitz_count)
{
	return IdeConsoleRun(FindCommand(exedirs, cmdline), &out, environment, true, slot, key, blitz_count);
}

bool Host::Wait()
{
	return IdeConsoleWait();
}

bool Host::Wait(int slot)
{
	return IdeConsoleWait(slot);
}

void Host::OnFinish(Event<>  cb)
{
	IdeConsoleOnFinish(cb);
}

bool Host::StartProcess(LocalProcess& p, const char *cmdline)
{
	try {
		if(canlog) Log(cmdline);
		p.NoConvertCharset();
		if(p.Start(FindCommand(exedirs, CMDLINE_PREFIX + cmdline), environment))
			return true;
	}
	catch(...) {
	}
	return false;
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

#ifdef PLATFORM_WIN32
String HostConsole = "powershell.exe";
#else
String HostConsole = "/usr/bin/xterm -e";
#endif

#ifdef PLATFORM_POSIX
void RemoveConsoleScripts()
{
	FileTime tm = (GetSysTime() - 24*3600).AsFileTime();
	for(FindFile ff(ConfigFile("console-script-*")); ff; ff++) {
		if(ff.GetLastWriteTime() < tm)
			FileDelete(ff.GetPath());
	}
}

String ResolveHostConsole()
{
	String lc;
	
	#ifdef PLATFORM_BSD
	static const char *term[] = {
		"/usr/local/bin/bobcat --",
		"/usr/local/bin/mate-terminal -x",
		"/usr/local/bin/gnome-terminal --window -x",
		"/usr/local/bin/konsole -e",
		"/usr/local/bin/lxterminal -e",
		"/usr/local/bin/xfce4-terminal -x",
		"/usr/local/bin/io.elementary.terminal -n -x",
		"/usr/local/bin/xterm -e",
	};
	#else
		#ifdef FLATPAK
		static const char *term[] = {
			"/run/host/bin/bobcat --",
			"/run/host/bin/mate-terminal -x",
			"/run/host/bin/gnome-terminal --window -x",
			"/run/host/bin/konsole -e",
			"/run/host/bin/lxterminal -e",
			"/run/host/bin/xfce4-terminal -x",
			"/run/host/bin/io.elementary.terminal -n -x",
			"/run/host/bin/xterm -e",
		};
		#else
		static const char *term[] = {
			"/usr/bin/bobcat --",
			"/usr/bin/mate-terminal -x",
			"/usr/bin/gnome-terminal --window -x",
			"/usr/bin/konsole -e",
			"/usr/bin/lxterminal -e",
			"/usr/bin/xfce4-terminal -x",
			"/usr/bin/io.elementary.terminal -n -x",
			"/usr/bin/xterm -e",
		};
		#endif
	#endif

	int ii = 0;
	for(;;) { // If (pre)defined terminal emulator is not available, try to find one
		int c = HostConsole.Find(' ');
		lc = c < 0 ? HostConsole : HostConsole.Left(c);
		if(ii >= __countof(term) || FileExists(lc))
			break;
		HostConsole = term[ii++];
	}
	return lc;
}

#endif


void Host::Launch(const char *_cmdline, bool console)
{
	String cmdline = FindCommand(exedirs, _cmdline);
	Log(cmdline);
#ifdef PLATFORM_WIN32
	if(console)
		cmdline = GetExeFilePath() + " ! " + cmdline;
	PROCESS_INFORMATION pi;
	STARTUPINFOW si;
	ZeroMemory(&si, sizeof(STARTUPINFOW));
	si.cb = sizeof(STARTUPINFOW);
	if(Win32CreateProcess(cmdline, ~environment, si, pi, NULL)) {
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
	else
		PutConsole("Unable to launch " + String(_cmdline));
#endif
#ifdef PLATFORM_POSIX
	String script = ConfigFile("console-script-" + AsString(getpid()));
	if(console) {
		FileStream out(script, FileStream::CREATE, 0777);
		out << "#!/bin/sh\n"
		    << cmdline << '\n'
		#ifndef PLATFORM_COCOA
		    << "echo \"<--- Finished, press [ENTER] to close the window --->\"\nread dummy\n"
		#endif
		;
	}
#ifdef PLATFORM_COCOA
	if(console)
		cmdline = "/usr/bin/open " + script;
#else
	if(console) {
		String lc = ResolveHostConsole();
		if(FileExists(lc))
		{
			#ifdef FLATPAK
				auto real_hc = HostConsole;
				real_hc.Replace(String("/run/host/"), String("/usr/"));
				cmdline = FindCommand(exedirs, CMDLINE_PREFIX + real_hc + " sh " + script);
			#else
				cmdline = HostConsole + " sh " + script;
			#endif
		}
		else
		if(HostConsole.GetCount())
			PutConsole("Warning: Terminal '" + lc + "' not found, executing in background.");
	}
#ifdef FLATPAK
	else {
		cmdline = FindCommand(exedirs, CMDLINE_PREFIX + cmdline);
	}
#endif
#endif
	
	Buffer<char> cmd_buf(strlen(cmdline) + 1);
	Vector<char *> args;
	
	Log(cmdline);

	char *o = cmd_buf;
	const char *s = cmdline;
	while(*s) {
		char *arg = o;
		while((byte)*s > ' ') {
			if(*s == '\"') {
				s++;
				while(*s) {
					if(*s == '\"') {
						s++;
						break;
					}
					*o++ = *s++;
				}
			}
			else
				*o++ = *s++;
		}
		while(*s && (byte)*s <= ' ')
			s++;
		if(o > arg) {
			*o++ = '\0';
			args.Add(arg);
		}
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

void AddHostFlags(Index<String>& cfg)
{
#if   defined(PLATFORM_WIN32)
	cfg.Add("WIN32");
#endif

#ifdef PLATFORM_POSIX
	cfg.Add("POSIX");
#endif

#ifdef PLATFORM_LINUX
	cfg.Add("LINUX");
#endif

#ifdef PLATFORM_ANDROID
	cfg.Add("ANDROID");
#endif

#ifdef PLATFORM_BSD
	cfg.Add("BSD");
#endif

#ifdef PLATFORM_OSX
	cfg.Add("OSX");
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

void Host::AddFlags(Index<String>& cfg)
{
	if(HasPlatformFlag(cfg))
		return;
	AddHostFlags(cfg);
}

const Vector<String>& Host::GetExecutablesDirs() const
{
	return exedirs;
}

void Host::AddExecutable(const String& dir, const String& exe)
{
	String p = dir + DIR_SEPS + exe;
	if(!FileExists(p)) {
		Loge() << METHOD_NAME << "Following executable file \"" << p << "\" doesn't exists.";
		return;
	}
	
	exedirs.Add(dir);
}

bool Host::HasPlatformFlag(const Index<String>& cfg)
{
	static const Index<String> platformFlags = {
		"WIN32", "POSIX", "LINUX", "ANDROID",
		"BSD", "FREEBSD", "OPENBSD", "NETBSD",
		"DRAGONFLY", "SOLARIS", "OSX11", "OSX"
	};

	for(const String& flag : cfg)
		if(platformFlags.Find(flag) >= 0)
			return true;

	return false;
}
