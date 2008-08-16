String  GetExeFilePath();
String  GetExeDirFile(const char *fp);

String  GetHomeDirFile(const char *fp);
String  GetHomeDirectory();

#ifdef PLATFORM_POSIX

void    SetHomeDirectory(const char *dir);

#endif

String  GetExeTitle();

void    UseHomeDirectoryConfig(bool b = true);

String  ConfigFile(const char *file);
String  ConfigFile();

const Vector<String>& CommandLine();
const VectorMap<String, String>& Environment();

void    SetExitCode(int code);
int     GetExitCode();

bool    IsMainRunning();

#ifndef flagSO
//void    Main(); // By console application
#endif

void AppExit__();

#ifdef PLATFORM_WIN32

void AppInit__(int argc, const char **argv);
void AppInitEnvironment__();

#define CONSOLE_APP_MAIN \
void ConsoleMainFn_(); \
 \
int main(int argc, char *argv[]) { \
	UPP::AppInit__(argc, (const char **)argv); \
	ConsoleMainFn_(); \
	UPP::DeleteUsrLog(); \
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
	ConsoleMainFn_(); \
	UPP::DeleteUsrLog(); \
	UPP::AppExit__(); \
	return UPP::GetExitCode(); \
} \
 \
void ConsoleMainFn_()

#endif

String  GetDataFile(const char *filename);

void    LaunchWebBrowser(const String& url);
