String  GetEnv(const char *id);
bool    SetEnv(const char *name, const char *value);

String  GetExeFilePath();
String  GetExeTitle();
String  GetExeFolder();
String  GetExeDirFile(const char *fp);

#ifdef PLATFORM_COCOA
String GetAppFolder();
#endif

String GetAppName();
void SetAppName(const String& name);

String  GetHomeDirFile(const char *fp);
String  GetHomeDirectory();

void    SetHomeDirectory(const char *dir);

void    UseHomeDirectoryConfig(bool b = true);

String  GetTempDirectory();
String  TempFile(const char *filename);

#ifdef PLATFORM_POSIX
String GetUserConfigDir(bool *sandboxed = NULL);
#endif

String  GetConfigFolder();
String  ConfigFile(const char *file);
String  ConfigFile();

void    SetConfigName(const String& s);

void    SetConfigGroup(const char *group);
String  GetConfigGroup();


const Vector<String>& CommandLine();
void   CommandLineRemove(int i, int count = 1);
String GetArgv0();
const VectorMap<String, String>& Environment();

void    SetExitCode(int code);
int     GetExitCode();

bool    IsMainRunning();

struct ExitExc {};

void Exit(int code = 1);

void AppExit__();
void AppExecute__(void (*app)());

#ifdef PLATFORM_WIN32

void AppInit__(int argc, const char **argv);
void AppInitEnvironment__();

#define CONSOLE_APP_MAIN \
void ConsoleMainFn_(); \
 \
int main(int argc, char *argv[]) { \
	UPP::AppInit__(argc, (const char **)argv); \
	UPP::AppExecute__(ConsoleMainFn_); \
	UPP::AppExit__(); \
	return UPP::GetExitCode(); \
} \
 \
void ConsoleMainFn_()

#endif

#ifdef PLATFORM_POSIX

void AppInit__(int argc, const char **argv, const char **envptr);

#define CONSOLE_APP_MAIN \
void ConsoleMainFn_(); \
 \
int main(int argc, const char **argv, const char **envptr) { \
	UPP::AppInit__(argc, argv, envptr); \
	UPP::AppExecute__(ConsoleMainFn_); \
	UPP::AppExit__(); \
	return UPP::GetExitCode(); \
} \
 \
void ConsoleMainFn_()

#endif

String  GetDataFile(const char *filename);
String  LoadDataFile(const char *filename);
void    SetDataPath(const char *path);

void    LaunchWebBrowser(const String& url);

String GetComputerName();
String GetUserName();
String GetDesktopManager();

String GetDesktopFolder();
String GetProgramsFolder();
#ifdef PLATFORM_WIN32
String GetProgramsFolderX86();
#endif
String GetAppDataFolder();
String GetMusicFolder();
String GetPicturesFolder();
String GetVideoFolder();
String GetDocumentsFolder();
String GetTemplatesFolder();
String GetDownloadFolder();
String GetProgramDataFolder();

void   InstallCrashHook(void (*h)());