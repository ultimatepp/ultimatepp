#include "Core.h"

#ifdef PLATFORM_MACOS
#include <mach-o/dyld.h>
#endif

#ifdef PLATFORM_WIN32
#define Ptr Ptr_
#define byte byte_
#define CY win32_CY_

#include <shellapi.h>
#include <wincon.h>

#ifdef COMPILER_MINGW
#undef CY
#endif

#include <shlobj.h>

#undef Ptr
#undef byte
#undef CY
#endif

namespace Upp {

static StaticMutex sHlock;

static char sHomeDir[_MAX_PATH + 1];
static char Argv0__[_MAX_PATH + 1];

void (*CrashHook)();

void InstallCrashHook(void (*h)())
{
	CrashHook = h;
}

void    SetHomeDirectory(const char *dir)
{
	INTERLOCKED_(sHlock) {
		strcpy(sHomeDir, dir);
	}
}

String  GetHomeDirectory() {
	String r;
	INTERLOCKED_(sHlock) {
		if(!*sHomeDir)
			strcpy(sHomeDir,
			#ifdef PLATFORM_WIN32
				GetEnv("HOMEDRIVE") + GetEnv("HOMEPATH")
			#else
				Nvl(GetEnv("HOME"), "/root")
			#endif
			);
		r = sHomeDir;
	}
	return r;
}



#ifdef PLATFORM_WIN32

String GetEnv(const char *id)
{
	return WString(_wgetenv(ToSystemCharsetW(id))).ToString();
}

String GetExeFilePath()
{
	return GetModuleFileName();
}

#endif

#ifdef PLATFORM_POSIX

String GetEnv(const char *id)
{
	return FromSystemCharset(getenv(id));
}

static void sSetArgv0__(const char *title)
{
	strcpy(Argv0__, title);
}

const char *procexepath_() {
	static char h[_MAX_PATH + 1];
	ONCELOCK {
		char link[1024];
#ifdef PLATFORM_MACOS
		uint32_t sz = 1024;
		if(_NSGetExecutablePath(link, &sz))
			*link = 0;
#elif defined(PLATFORM_BSD)
		sprintf(link, "/proc/%d/file", getpid());
#else
		sprintf(link, "/proc/%d/exe", getpid());
#endif
		FindFile ff(link);
		if(ff) {
			if(ff.IsSymLink()) {
				int ret = readlink(link, h, _MAX_PATH);
				if(ret > 0 && ret < _MAX_PATH)
					h[ret] = '\0';
				else
					*h = '\0';
			}
			else
				strcpy(h, link);
		}
	}
	return h;
}

String GetExeFilePath()
{
	static char exepath[_MAX_PATH + 1];
	ONCELOCK {
		const char *exe = procexepath_();
		if(*exe)
			strcpy(exepath, exe);
		else {
			String x = Argv0__;
			if(IsFullPath(x) && FileExists(x))
				strcpy(exepath, x);
			else {
				strcpy(exepath, GetHomeDirFile("upp"));
				Vector<String> p = Split(FromSystemCharset(Environment().Get("PATH")), ':');
				if(x.Find('/') >= 0)
					p.Add(GetCurrentDirectory());
				for(int i = 0; i < p.GetCount(); i++) {
					String ep = NormalizePath(AppendFileName(p[i], x));
					if(FileExists(ep))
						strcpy(exepath, ep);
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

String GetExeFolder()
{
	return GetFileFolder(GetExeFilePath());
}

String GetExeTitle()
{
	return GetFileTitle(GetExeFilePath());
}

#ifdef PLATFORM_COCOA
String GetAppFolder()
{
	String p = GetExeFolder();
	return p.TrimEnd("/Contents/MacOS") && GetFileExt(p) == ".app" ? p : String();
}
#endif

void SyncLogPath__();

static char sAppName[256];

String GetAppName()
{
	return Nvl(String(sAppName), GetExeTitle());
}

void SetAppName(const String& name)
{
	strcpy(sAppName, name);
	SyncLogPath__();
}

static char sConfigGroup[256] = "u++";

void SetConfigGroup(const char *group)
{
	strcpy(sConfigGroup, group);
	SyncLogPath__();
}

String GetConfigGroup()
{
	return sConfigGroup;
}

String GetTempDirectory()
{
	return GetTempPath();
}

String TempFile(const char *filename)
{
	return AppendFileName(GetTempDirectory(), filename);
}

String  GetHomeDirFile(const char *fp) {
	return AppendFileName(GetHomeDirectory(), fp);
}

static bool sHomecfg;

void UseHomeDirectoryConfig(bool b)
{
	sHomecfg = b;
}

static char sConfigFolder[_MAX_PATH + 1];

void SetConfigDirectory(const String& s)
{
	strcpy(sConfigFolder, s);
	SyncLogPath__();
}

void CopyFolder(const char *dst, const char *src)
{
	RealizeDirectory(dst);
	FindFile ff(String(src) + "/*.*");
	while(ff) {
		String s = AppendFileName(src, ff.GetName());
		String d = AppendFileName(dst, ff.GetName());
		if(ff.IsFile()) {
			FileIn in(s);
			FileOut out(d);
			CopyStream(out, in);
		}
		else
		if(ff.IsFolder() && *ff.GetName() != '.')
			CopyFolder(d, s);
		ff.Next();
	}
}

String  ConfigFile(const char *file) {
	if(*sConfigFolder)
		return AppendFileName(sConfigFolder, file);
#if defined(PLATFORM_WIN32)
	if(sHomecfg) {
		String p = GetHomeDirFile(GetAppName());
		ONCELOCK
			RealizeDirectory(p);
		return AppendFileName(p, file);
	}
	return GetExeDirFile(file);
#elif defined(PLATFORM_POSIX)
	static char cfgd[_MAX_PATH + 1];
	static bool sandboxed = true;
	ONCELOCK {
		String cfgdir;
		String h = GetExeFolder();
		if(!sHomecfg)
			while(h.GetCount() > 1 && DirectoryExists(h)) {
				String pp = AppendFileName(h, ".config");
				FindFile ff(pp);
				if(ff && ff.IsFolder() && ff.CanWrite()) {
					cfgdir = pp;
					break;
				}
				h = GetFileFolder(h);
			}
		if(IsNull(cfgdir)) {
			sandboxed = false;
			cfgdir = GetEnv("XDG_CONFIG_HOME");
		}
		if(IsNull(cfgdir) || !DirectoryExists(cfgdir))
			cfgdir = GetHomeDirFile(".config");
		if(*sConfigGroup)
			cfgdir = AppendFileName(cfgdir, GetConfigGroup());
		strcpy(cfgd, cfgdir);
	}
	String pp = AppendFileName(cfgd, GetAppName());
	bool exists = DirectoryExists(pp);
	RealizeDirectory(pp);
	if(!exists && !sandboxed) { // migrate config files from the old path
		String old = GetHomeDirFile(".upp/" + GetExeTitle());
		if(DirectoryExists(old))
			CopyFolder(pp, old);
	}
	return AppendFileName(pp, file);
#else
	NEVER();
	return GetExeDirFile(file);
#endif//PLATFORM
}

String GetConfigFolder()
{
	return GetFileFolder(ConfigFile("x"));
}

String  ConfigFile() {
	return ConfigFile(GetAppName() + ".cfg");
}

String argv0;

Vector<WString>& coreCmdLine__()
{
	static Vector<WString> h;
	return h;
}

static Mutex sCmdMutex;
static Vector<String> sCmd;

const Vector<String>& CommandLine()
{
	Mutex::Lock __(sCmdMutex);
	ONCELOCK {
		for(WString s : coreCmdLine__())
			sCmd.Add(s.ToString());
	}
	return sCmd;
}

void CommandLineRemove(int i, int count)
{
	CommandLine();
	sCmd.Remove(i, count);
}

String GetArgv0()
{
	return Argv0__;
}

VectorMap<WString, WString>& EnvMap()
{
	static VectorMap<WString, WString> x;
	return x;
}

const VectorMap<String, String>& Environment()
{
	VectorMap<String, String> *ptr;
	INTERLOCKED {
		static ArrayMap< byte, VectorMap<String, String> > charset_env;
		byte cs = GetDefaultCharset();
		int f = charset_env.Find(cs);
		if(f >= 0)
			ptr = &charset_env[f];
		else {
			ptr = &charset_env.Add(cs);
			const VectorMap<WString, WString>& env_map = EnvMap();
			for(int i = 0; i < env_map.GetCount(); i++)
				ptr->Add(env_map.GetKey(i).ToString(), env_map[i].ToString());
		}
	}
	return *ptr;
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

	Vector<WString>& cmd = coreCmdLine__();
	static WString exp_cmd = "--export-tr";
	static WString brk_cmd = "--memory-breakpoint__";
	
	for(int i = 0; i < cmd.GetCount();) {
		if(cmd[i] == exp_cmd) {
			{
				i++;
				int lang = 0;
				int lang2 = 0;
				byte charset = CHARSET_UTF8;
				String fn = "all";
				if(i < cmd.GetCount())
					if(cmd[i].GetLength() == 4 || cmd[i].GetLength() == 5) {
						lang = LNGFromText(cmd[i].ToString());
						fn = cmd[i].ToString();
						int c = cmd[i][4];
						if(c >= '0' && c <= '8')
							charset = c - '0' + CHARSET_WIN1250;
						if(c >= 'A' && c <= 'J')
							charset = c - 'A' + CHARSET_ISO8859_1;
					}
				fn << ".tr";
				if(++i < cmd.GetCount() && (cmd[i].GetLength() == 4 || cmd[i].GetLength() == 5))
					lang2 = LNGFromText(cmd[i].ToString());
			#ifdef PLATFORM_WIN32
				FileOut out(GetExeDirFile(fn));
			#else
				FileOut out(GetHomeDirFile(fn));
			#endif
				if(lang) {
					if(lang2)
						SaveLngFile(out, SetLNGCharset(lang, charset), SetLNGCharset(lang2, charset));
					else
						SaveLngFile(out, SetLNGCharset(lang, charset));
				}
				else {
					Index<int> l = GetLngSet();
					for(int i = 0; i < l.GetCount(); i++)
						SaveLngFile(out, SetLNGCharset(l[i], charset));
				}
			}
			exit(0);
		}
	#if defined(_DEBUG) && defined(UPP_HEAP)
		if(cmd[i] == brk_cmd && i + 1 < cmd.GetCount()) {
			MemoryBreakpoint(atoi(cmd[i + 1].ToString()));
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

void Exit(int code)
{
	SetExitCode(code);
	throw ExitExc();
}

void MemorySetMainBegin__();
void MemorySetMainEnd__();

void AppExecute__(void (*app)())
{
	try {
		MemorySetMainBegin__();
		(*app)();
		MemorySetMainEnd__();
	}
	catch(ExitExc) {
		return;
	}
}

#ifdef PLATFORM_POSIX

void s_ill_handler(int)
{
	CrashHook();
	Panic("Illegal instruction!");
}

void s_segv_handler(int)
{
	CrashHook();
	Panic("Invalid memory access!");
}

void s_fpe_handler(int)
{
	CrashHook();
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
		EnvMap().Add(varname.ToWString(), String(var).ToWString());
	}
	Vector<WString>& cmd = coreCmdLine__();
	for(int i = 1; i < argc; i++)
		cmd.Add(FromSystemCharset(argv[i]).ToWString());
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
	SetLanguage(LNG_('E', 'N', 'U', 'S'));
	int nArgs;
    LPWSTR *szArglist = CommandLineToArgvW(GetCommandLineW(), &nArgs);
	if(szArglist) {
		strcpy(Argv0__, FromSystemCharsetW(szArglist[0]));
		for(int i = 1; i < nArgs; i++)
			coreCmdLine__().Add(FromSystemCharsetW(szArglist[i]).ToWString());
		LocalFree(szArglist);
    }
		
	WCHAR *env = GetEnvironmentStringsW();
	for(WCHAR *ptr = env; *ptr; ptr++)
	{
		const WCHAR *b = ptr;
		if(*ptr)
			ptr++;
		while(*ptr && *ptr != '=')
			ptr++;
		WString varname = ToUtf32(b, int(ptr - b));
		if(*ptr)
			ptr++;
		b = ptr;
		while(*ptr)
			ptr++;
		EnvMap().GetAdd(ToUpper(varname)) = ToUtf32(b, int(ptr - b));
	}
	FreeEnvironmentStringsW(env);

	CommonInit();
}

void AppInit__(int argc, const char **argv)
{
	AppInitEnvironment__();
}
#endif

void AppExit__()
{
	Thread::ShutdownThreads();
	sMainRunning = false;
#ifdef PLATFORM_POSIX
	MemoryIgnoreLeaksBegin(); // Qt leaks on app exit...
#endif
}

#ifdef flagTURTLE

void Turtle_PutLink(const String& link);

void LaunchWebBrowser(const String& url)
{
	Turtle_PutLink(url);
}

#else

#if defined(PLATFORM_WIN32) && !defined(PLATFORM_WINCE)
static auxthread_t auxthread__ sShellExecuteOpen(void *str)
{
	ShellExecuteW(NULL, L"open", (WCHAR *)str, NULL, L".", SW_SHOWDEFAULT);
	free(str);
	return 0;
}

void LaunchWebBrowser(const String& url)
{
	Vector<WCHAR> wurl = ToSystemCharsetW(url);
	if ((int64)(ShellExecuteW(NULL, L"open", wurl, NULL, L".", SW_SHOWDEFAULT)) <= 32) {
		int l = sizeof(wchar) * wurl.GetCount() + 1;
		char *curl = (char *)malloc(l);
		memcpy(curl, wurl, l);
		StartAuxThread(sShellExecuteOpen, curl);
	}
}
#endif

#ifdef PLATFORM_POSIX
void    LaunchWebBrowser(const String& url)
{
#ifdef PLATFORM_MACOS
	IGNORE_RESULT(system("open " + url));
#else
	const char * browser[] = {
		"htmlview", "xdg-open", "x-www-browser", "firefox", "konqueror", "opera", "epiphany", "galeon", "netscape"
	};
	for(int i = 0; i < __countof(browser); i++)
		if(system("which " + String(browser[i])) == 0) {
			String u = url;
			u.Replace("'", "'\\''");
			IGNORE_RESULT(
				system(String(browser[i]) + " '" + u + "' &")
			);
			break;
		}
#endif
}
#endif

#endif

String sDataPath;

void SetDataPath(const char *path)
{
	sDataPath = path;
}

String GetDataFile(const char *filename)
{
	if(sDataPath.GetCount())
		return AppendFileName(sDataPath, filename);
	String s = GetEnv("UPP_MAIN__");
	return s.GetCount() ? AppendFileName(s, filename) : GetExeDirFile(filename);
}

String LoadDataFile(const char *filename)
{
	return LoadFile(GetDataFile(filename));
}

String GetComputerName()
{
#if defined(PLATFORM_WIN32)
	WCHAR temp[256];
	*temp = 0;
	dword w = 255;
	::GetComputerNameW(temp, &w);
#else
	char temp[256];
	gethostname(temp, sizeof(temp));
#endif
	return temp;
}

String GetUserName()
{
#if defined(PLATFORM_WIN32)
	WCHAR temp[256];
	*temp = 0;
	dword w = 255;
	::GetUserNameW(temp, &w);
	return temp;
#else
	return Nvl(GetEnv("USER"), "root");
#endif
}

String GetDesktopManager()
{
#if defined(PLATFORM_WIN32) && !defined(PLATFORM_WINCE)
	return "windows";
#endif
#ifdef PLATFORM_POSIX
    if(GetEnv("GNOME_DESKTOP_SESSION_ID").GetCount())
		return "gnome";
	if(GetEnv("KDE_FULL_SESSION").GetCount() || GetEnv("KDEDIR").GetCount())
        return "kde";
	return GetEnv("DESKTOP_SESSION");
#endif	
}

#if defined(PLATFORM_WIN32)

String GetShellFolder(int clsid) 
{
	WCHAR path[MAX_PATH];
	if(SHGetFolderPathW(NULL, clsid, NULL, /*SHGFP_TYPE_CURRENT*/0, path) == S_OK)
		return FromSystemCharsetW(path);
	return Null;
}

String GetDesktopFolder()	  { return GetShellFolder(CSIDL_DESKTOP); }
String GetProgramsFolder()	  { return GetShellFolder(CSIDL_PROGRAM_FILES); }
String GetProgramsFolderX86() { return GetShellFolder(0x2a); }
String GetAppDataFolder()	  { return GetShellFolder(CSIDL_APPDATA);}
String GetMusicFolder()		  { return GetShellFolder(CSIDL_MYMUSIC);}
String GetPicturesFolder()	  { return GetShellFolder(CSIDL_MYPICTURES);}
String GetVideoFolder()		  { return GetShellFolder(CSIDL_MYVIDEO);}
String GetDocumentsFolder()	  { return GetShellFolder(/*CSIDL_MYDOCUMENTS*/0x0005);}
String GetTemplatesFolder()	  { return GetShellFolder(CSIDL_TEMPLATES);}
String GetProgramDataFolder() { return GetShellFolder(CSIDL_COMMON_APPDATA); }

#define MY_DEFINE_KNOWN_FOLDER(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
static const GUID name = { l, w1, w2, { b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } }

MY_DEFINE_KNOWN_FOLDER(MY_FOLDERID_Downloads, 0x374de290, 0x123f, 0x4565, 0x91, 0x64, 0x39, 0xc4, 0x92, 0x5e, 0x46, 0x7b);

String GetDownloadFolder()	
{
	static HRESULT (STDAPICALLTYPE * SHGetKnownFolderPath)(const void *rfid, DWORD dwFlags, HANDLE hToken, PWSTR *ppszPath);
	ONCELOCK {
		DllFn(SHGetKnownFolderPath, "shell32.dll", "SHGetKnownFolderPath");
	}
	if(SHGetKnownFolderPath) {
		PWSTR path = NULL;
		if(SHGetKnownFolderPath(&MY_FOLDERID_Downloads, 0, NULL, &path) == S_OK && path) {
			String s = FromSystemCharsetW(path);
			CoTaskMemFree(path);
			return s;
		}
	}
	return Null;
};
#endif

#ifdef PLATFORM_POSIX

String GetPathXdg(String xdgConfigHome, String xdgConfigDirs)
{
	String ret;
	if(FileExists(ret = AppendFileName(xdgConfigHome, "user-dirs.dirs")))
		return ret;
	if(FileExists(ret = AppendFileName(xdgConfigDirs, "user-dirs.defaults")))
		return ret;
	if(FileExists(ret = AppendFileName(xdgConfigDirs, "user-dirs.dirs")))
		return ret;
	return Null;
}

String GetPathDataXdg(String fileConfig, const char *folder) 
{
	TextSettings settings;
	settings.Load(fileConfig);
	String v = settings.Get(folder);
	if(*v == '\"')
		v = v.Mid(1);
	if(*v.Last() == '\"')
		v.Trim(v.GetCount() - 1);
	String r;
	const char *s = v;
	while(*s) {
		if(*s == '$') {
			CParser p(s + 1);
			p.NoSkipSpaces();
			p.Spaces();
			if(p.IsId()) {
				String id = p.ReadId();
				r.Cat(GetEnv(id));
				s = p.GetPtr();
			}
			else {
				r.Cat('$');
				s++;
			}
		}
		else
			r.Cat(*s++);
	}
	return r;
}

String GetShellFolder(const char *local, const char *users) 
{
	String xdgConfigHome = GetEnv("XDG_CONFIG_HOME");
	if (xdgConfigHome == "")		// By default
		xdgConfigHome = AppendFileName(GetHomeDirectory(), ".config");
	String xdgConfigDirs = GetEnv("XDG_CONFIG_DIRS");
	if (xdgConfigDirs == "")			// By default
		xdgConfigDirs = "/etc/xdg";
	String xdgFileConfigData = GetPathXdg(xdgConfigHome, xdgConfigDirs);
	String ret = GetPathDataXdg(xdgFileConfigData, local);
	if (ret == "" && *users != '\0')
		return GetPathDataXdg(xdgFileConfigData, users);
	else
		return ret;
}

String GetDesktopFolder()
{
	String ret = GetShellFolder("XDG_DESKTOP_DIR", "DESKTOP");
	if (ret.IsEmpty())
		return AppendFileName(GetHomeDirectory(), "Desktop");
	else
		return ret;
}

String GetProgramsFolder()    { return String("/usr/bin"); }
String GetAppDataFolder()     { return GetHomeDirectory(); }
String GetMusicFolder()	      { return GetShellFolder("XDG_MUSIC_DIR", "MUSIC"); }
String GetPicturesFolder()    { return GetShellFolder("XDG_PICTURES_DIR", "PICTURES"); }
String GetVideoFolder()       { return GetShellFolder("XDG_VIDEOS_DIR", "VIDEOS"); }
String GetDocumentsFolder()   { return GetShellFolder("XDG_DOCUMENTS_DIR", "DOCUMENTS"); }
String GetTemplatesFolder()   { return GetShellFolder("XDG_TEMPLATES_DIR", "XDG_TEMPLATES_DIR"); }
String GetDownloadFolder()    { return GetShellFolder("XDG_DOWNLOAD_DIR", "XDG_DOWNLOAD_DIR"); }
String GetProgramDataFolder() { return String("/var/opt"); }

#endif


}