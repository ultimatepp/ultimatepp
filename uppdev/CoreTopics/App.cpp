#include "Core.h"

#ifdef PLATFORM_WIN32
#include <shellapi.h>
#endif

NAMESPACE_UPP

#ifdef PLATFORM_WIN32

String GetExeFilePath()
{
	return GetModuleFileName();
}

String  GetHomeDirectory() {
	return FromSystemCharset(getenv("HOMEDRIVE")) +
	       FromSystemCharset(getenv("HOMEPATH"));
}

#endif

#ifdef PLATFORM_POSIX

char Argv0__[_MAX_PATH + 1];

static void sSetArgv0__(const char *title)
{
	strcpy(Argv0__, title);
}

extern char Argv0__[];

const char *procexepath_() {
	static char h[_MAX_PATH + 1];
	ONCELOCK {
		char link[100];
#ifdef PLATFORM_FREEBSD
		sprintf(link, "/proc/%d/file", getpid());
#else
		sprintf(link, "/proc/%d/exe", getpid());
#endif
		int ret = readlink(link, h, _MAX_PATH);
		if(ret > 0 && ret < _MAX_PATH)
			h[ret] = '\0';
		else
			*h = '\0';
	}
	return h;
}

String GetExeFilePath()
{
	static String exepath;
	ONCELOCK {
		const char *exe = procexepath_();
		if(*exe)
			exepath = exe;
		else {
			String x = Argv0__;
			if(IsFullPath(x) && FileExists(x))
				exepath = x;
			else {
				exepath = GetHomeDirFile("upp");
				Vector<String> p = Split(FromSystemCharset(Environment().Get("PATH")), ':');
				if(x.Find('/') >= 0)
					p.Add(GetCurrentDirectory());
				for(int i = 0; i < p.GetCount(); i++) {
					String ep = NormalizePath(AppendFileName(p[i], x));
					if(FileExists(ep))
						exepath = ep;
				}
			}
		}
	}
	return exepath;
}

#endif

String GetExeDirFile(const char *filename)
{
	return AppendFileName(GetFileFolder(GetExeFilePath()), filename);
}

String GetExeTitle()
{
	return GetFileTitle(GetExeFilePath());
}

#ifdef PLATFORM_POSIX

static StaticCriticalSection sHlock;

String& sHomeDir() {
	static String s;
	return s;
}

String  GetHomeDirectory() {
	String r;
	INTERLOCKED_(sHlock) {
		String& s = sHomeDir();
		if(s.IsEmpty()) {
			const char *home = getenv("HOME");
			s = FromSystemCharset(home ? home : "/root");
		}
		r = s;
	}
	return r;
}

void    SetHomeDirectory(const char *dir)
{
	INTERLOCKED_(sHlock) {
		sHomeDir() = dir;
	}
}

#endif//PLATFORM_POSIX

String  GetHomeDirFile(const char *fp) {
	return AppendFileName(GetHomeDirectory(), fp);
}

static bool sHomecfg;

void    UseHomeDirectoryConfig(bool b)
{
	sHomecfg = b;
}

String  ConfigFile(const char *file) {
#if defined(PLATFORM_WIN32)
	if(sHomecfg) {
		String p = GetHomeDirFile(GetExeTitle());
		ONCELOCK
			RealizeDirectory(p);
		return AppendFileName(p, file);
	}
	return GetExeDirFile(file);
#elif defined(PLATFORM_POSIX)
	String p = GetHomeDirFile(".upp/" + GetExeTitle());
	ONCELOCK
		RealizeDirectory(p);
	return AppendFileName(p, file);
#else
#error ConfigFile not implemented for this platform, comment this line to get input string back
	return file;
#endif//PLATFORM
}

String  ConfigFile() {
	return ConfigFile(GetExeTitle() + ".cfg");
}

GLOBAL_VAR(Vector<String>, coreCmdLine__)

const Vector<String>& CommandLine()
{
	return coreCmdLine__();
}

typedef VectorMap<String, String> StringMap;
GLOBAL_VAR(StringMap, coreEnvPtr__)
const VectorMap<String, String>& Environment()
{
	return coreEnvPtr__();
}

static int exitcode;
static bool sMainRunning;

void  SetExitCode(int code) { exitcode = code; }
int   GetExitCode()         { return exitcode; }

bool  IsMainRunning()
{
	return sMainRunning;
}

void LoadLangFiles(const char *dir)
{
	FindFile ff(AppendFileName(dir, "*.tr"));
	while(ff) {
		LoadLngFile(AppendFileName(dir, ff.GetName()));
		ff.Next();
	}
}

void CommonInit()
{
#ifdef PLATFORM_WIN32
	LoadLangFiles(GetFileFolder(GetExeFilePath()));
#else
	LoadLangFiles(GetHomeDirectory());
#endif

	Vector<String>& cmd = coreCmdLine__();
	for(int i = 0; i < cmd.GetCount();) {
		if(cmd[i] == "--export-tr") {
			{
				i++;
				int lang = 0;
				byte charset = CHARSET_UTF8;
				String fn = "all";
				if(i < cmd.GetCount())
					if(cmd[i].GetLength() != 4 && cmd[i].GetLength() != 5)
						lang = 0;
					else {
						lang = LNGFromText(cmd[i]);
						fn = cmd[i];
						int c = cmd[i][4];
						if(c >= '0' && c <= '8')
							charset = c - '0' + CHARSET_WIN1250;
						if(c >= 'A' && c <= 'J')
							charset = c - 'A' + CHARSET_ISO8859_1;
					}
				fn << ".tr";
			#ifdef PLATFORM_WIN32
				FileOut out(GetExeDirFile(fn));
			#else
				FileOut out(GetHomeDirFile(fn));
			#endif
				if(lang)
					SaveLngFile(out, SetLNGCharset(lang, charset));
				else {
					Index<int> l = GetLngSet();
					for(int i = 0; i < l.GetCount(); i++)
						SaveLngFile(out, SetLNGCharset(l[i], charset));
				}
			}
			exit(0);
		}
	#if defined(_DEBUG) && defined(UPP_HEAP)
		if(cmd[i] == "--memory-breakpoint__" && i + 1 < cmd.GetCount()) {
			MemoryBreakpoint(atoi(cmd[i + 1]));
			cmd.Remove(i, 2);
		}
		else
			i++;
	#else
		i++;
	#endif
	}
	sMainRunning = true;
}

#ifdef PLATFORM_POSIX

void s_ill_handler(int)
{
	Panic("Illegal instruction!");
}

void s_segv_handler(int)
{
	Panic("Invalid memory access!");
}

void s_fpe_handler(int)
{
	Panic("Invalid arithmetic operation!");
}

void AppInit__(int argc, const char **argv, const char **envptr)
{
	SetLanguage(LNG_ENGLISH);
	sSetArgv0__(argv[0]);
	for(const char *var; (var = *envptr) != 0; envptr++)
	{
		const char *b = var;
		while(*var && *var != '=')
			var++;
		String varname(b, var);
		if(*var == '=')
			var++;
		coreEnvPtr__().Add(varname, var);
	}
	Vector<String>& cmd = coreCmdLine__();
	for(int i = 1; i < argc; i++)
		cmd.Add(argv[i]);
	CommonInit();
	signal(SIGILL, s_ill_handler);
	signal(SIGSEGV, s_segv_handler);
	signal(SIGBUS, s_segv_handler);
	signal(SIGFPE, s_fpe_handler);
}
#endif

#if defined(PLATFORM_WIN32)

void AppInitEnvironment__()
{
#ifndef PLATFORM_WINCE
	char *env = GetEnvironmentStrings();
	for(char *ptr = env; *ptr; ptr++)
	{
		const char *b = ptr;
		if(*ptr)
			ptr++;
		while(*ptr && *ptr != '=')
			ptr++;
		String varname(b, ptr);
		if(*ptr)
			ptr++;
		b = ptr;
		while(*ptr)
			ptr++;
		coreEnvPtr__().GetAdd(ToUpper(varname)) = String(b, ptr);
	}
	FreeEnvironmentStrings(env);
#endif
	CommonInit();
}

void AppInit__(int argc, const char **argv)
{
	SetLanguage(LNG_ENGLISH);
	Vector<String>& cmd = coreCmdLine__();
	for(int i = 1; i < argc; i++)
		cmd.Add(argv[i]);
	AppInitEnvironment__();
}
#endif

void AppExit__()
{
	sMainRunning = false;
#ifdef PLATFORM_POSIX
	MemoryIgnoreLeaksBegin(); // Qt leaks on app exit...
#endif
}

void    LaunchWebBrowser(const String& url)
{
#if defined(PLATFORM_WIN32) && !defined(PLATFORM_WINCE)
	ShellExecute(NULL, "open", url, NULL, ".", SW_SHOWDEFAULT);
#endif
#ifdef PLATFORM_POSIX
	const char * browser[] = {
		"htmlview", "xdg-open", "x-www-browser", "firefox", "konqueror", "opera", "epiphany", "galeon", "netscape"
	};
	for(int i = 0; i < __countof(browser); i++)
		if(system("which " + String(browser[i])) == 0) {
			system(String(browser[i]) + " " + url + " &");
			break;
		}
#endif
}

String GetDataFile(const char *filename)
{
	String s = Environment().Get("UPP_MAIN__",
#ifdef PLATFORM_WIN32
	                             GetFileFolder(GetExeFilePath())
#endif
#ifdef PLATFORM_POSIX
	                             GetHomeDirectory()
#endif
	);

	return AppendFileName(s, filename);
}

END_UPP_NAMESPACE
