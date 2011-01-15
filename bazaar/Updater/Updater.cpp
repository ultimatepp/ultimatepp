#include "Updater.h"

#include <Web/Web.h>

#include <SysExec/SysExec.h>

// gets platform root folder
String Updater::GetPlatformRoot(void)
{
#if defined(WIN64)
	return webRoot + "/WINDOWS64/";
#elif defined(WIN32)
	return webRoot + "/WINDOWS32/";
#elif defined(PLATFORM_POSIX)
#ifdef __x86_64
	return webRoot + "/LINUX64/";
#else
	return webRoot + "/LINUX32/";
#endif
#else
#error "PLATFORM NOT SUPPORTED"
#endif
}

// constructor
Updater::Updater()
{
	// fetches and stores environment, we need to change it later
	environ <<= Environment();

	// gets state variable, if not present assume starting in normal mode
	String stateStr;
	if(environ.Find("UPDATER_STATE") < 0)
		state = NormalRun;
	else
	{
		stateStr = environ.Get("UPDATER_STATE");
		if(stateStr == "INSIDEUPDATER")
			state = InsideUpdater;
		else if(stateStr == "UNINSTALLFAILED")
			state = UninstallFailed;
		else if(stateStr == "INSTALLFAILED")
			state = InstallFailed;
		else if(stateStr == "UPDATEFAILED")
			state = UpdateFailed;
		else if(stateStr == "UNINSTALLSUCCEEDED")
			state = UninstallSucceeded;
		else if(stateStr == "INSTALLSUCCEEDED")
			state = InstallSucceeded;
		else if(stateStr == "UPDATESUCCEEDED")
			state = UpdateSucceeded;
		else
			NEVER();
	}

	// gets user name
	if(state != InsideUpdater)
		user = GetUserName();
	else if(environ.Find("UPDATER_USER") < 0)
		user = "unknown";
	else
		user = environ.Get("UPDATER_USER");

	// get application name -- either from GetExeTitle() if in normal mode
	// or from environment if in superuser mode
	if(state != InsideUpdater)
		appName = GetExeTitle();
	else if(environ.Find("UPDATER_APPNAME") >= 0)
		appName = environ.Get("UPDATER_APPNAME");
	else
		appName = GetExeTitle(); // fallout -- should NEVER get here
	
	// gets app's user config path
#ifdef PLATFORM_POSIX
	userConfigPath = "/home/" + user + "./" + appName + "/";
#else
	userConfigPath = GetHomeDirectory() + "./" + appName + "/";
#endif

	// gets app's system config path
#ifdef PLATFORM_POSIX
	systemConfigPath = "/usr/share/" + appName + "/";
#else
	systemConfigPath = GetProgramsFolder() + "/" + appName + "/";
#endif

	// checks wether app is already installed and gather its version
	installedVersion = -1;
#ifdef PLATFORM_POSIX
	appInstalled = FileExists(GetProgramsFolder() + "/" + appName);
#else
	appInstalled = FileExists(GetProgramsFolder() + "/" + appName + "/" + appName + ".exe");
#endif
	appInstalled &= FileExists(systemConfigPath + "version");
	String verStr;
	if(appInstalled)
	{
		verStr = LoadFile(systemConfigPath + "version");
		if(verStr != "")
			installedVersion = ScanDouble(verStr);
	}

	// create user config path only on normal run
	if(state == NormalRun)
		RealizePath(userConfigPath);
	
	// creates system config path on superuser mode
	if(state == InsideUpdater)
		RealizePath(systemConfigPath);
}

///////////////////////////////////////////////////////////////////////////////////////
// installer/updater subfunctions

// setup environment for failing update/install
void Updater::FailUpdate()
{
	switch(updaterOp)
	{
		case Install:
			environ.Add("UPDATER_STATE", "INSTALLFAILED");
			break;
			
		case Uninstall:
			environ.Add("UPDATER_STATE", "UNINSTALLFAILED");
			break;
			
		case Update:
		environ.Add("UPDATER_STATE", "UPDATEFAILED");
			break;
			
		default:
			NEVER();
	}
}

// setup environment for succeeding update/install
void Updater::SucceedUpdate()
{
	switch(updaterOp)
	{
		case Install:
			environ.Add("UPDATER_STATE", "INSTALLSUCCEEDED");
			break;
			
		case Uninstall:
			environ.Add("UPDATER_STATE", "UNINSTALLSUCCEEDED");
			break;
			
		case Update:
			environ.Add("UPDATER_STATE", "UPDATESUCCEEDED");
			break;
			
		default:
			NEVER();
	}
}

// main updater proc inside normal mode
// prepares environment, copy app in temporary folder
// and restarts it in superuser mode
bool Updater::START_Updater(String const &operation)
{
	// prepare environment for updating
	environ.Add("UPDATER_USER", user);
	environ.Add("UPDATER_OP", operation);
	environ.Add("UPDATER_STATE", "INSIDEUPDATER");
	environ.Add("UPDATER_APPNAME", appName);
	environ.Add("UPDATER_EXEPATH", GetExeFilePath());
	
	if(CommandLine().GetCount() && CommandLine()[0] != "--UNINSTALL")
	{
		String s;
		for(int i = 0; i < CommandLine().GetCount(); i++)
			s += CommandLine()[i] + " ";
		s = s.Left(s.GetCount() - 1);
		environ.Add("UPDATER_CMDLINE", s);
	}

	// gets current executable path
	String exePath = GetExeFilePath();
	
	// creates a temporary filename and copies itself
	// in temporary folder
	String tempName = GetTempFileName();
	if(!FileCopy(exePath, tempName))
		return false;
	
#ifdef PLATFORM_POSIX
	// for posix, change temp file permission
	// allowing its execution and executes it as a superuser
	if(chmod(~tempName, 0755) != 0)
		return false;
	
	// note the -k to gksu -- it makes it preserve the environment
	String params = "-k -u root \"" + tempName + "\"";
	if(SysStart("gksu", params, environ) == -1)
		return false;
#else
	// for windows, simply execute the file
	if(SysStart(tempName, "", environ) == -1)
		return false;
#endif
	return true;
}

// uninstall app
void Updater::START_Uninstall(void)
{
	if(!START_Updater("UNINSTALL"))
		Exclamation(t_("Uninstall failed&Press OK to quit"));
}

// install app
bool Updater::START_Install(void)
{
	if(!START_Updater("INSTALL"))
		return !PromptYesNo(t_("Install failed&Continue running the application?"));
	return true;
}

// update app
bool Updater::START_Update(void)
{
	if(!START_Updater("UPDATE"))
		return !PromptYesNo(t_("Update failed&Continue running the application?"));
	return true;
}

// app started, must check for update/install requests
bool Updater::DO_NormalRun(void)
{
	// we now check if we just want to uninstall app
	// it can be done by command line --UNINSTALL option
	if(CommandLine().GetCount() && CommandLine()[0] == "--UNINSTALL")
	{
		START_Uninstall();
		return false;
	}
	
	// if app not installed, we shall install it
	if(!appInstalled)
		return !START_Install();
	
	// not installing nor uninstalling
	
	// we shall check which kind of update is enabled
	String updateMode;
	if(!FileExists(userConfigPath + "UPDATER_MODE"))
	{
		updateMode = "ASK";
		SaveFile(userConfigPath + "UPDATER_MODE", updateMode);
	}
	else
		updateMode = LoadFile(userConfigPath + "UPDATER_MODE");

	// if updates are disabled, just do nothing
	if(updateMode == "DISABLED")
		return true;
	
	// if we don't have a new version available, just do nothing
	double maxVer;
	if( (maxVer = FetchMaxValidVersion()) <= installedVersion)
		return true;
	
	// if we want manual updates, just ask
	if(updateMode == "ASK")
		if(!PromptYesNo(t_("New version ") + FormatDoubleFix(maxVer, 2, FD_ZERO) + t_(" is available&Install it ?")))
			return true;
	
	// updater enabled, start it
	// if update failed, resume normal run, otherwise exit
	// as the app will be launched again by updater itself
	return !START_Update();
}

bool Updater::DO_InsideUpdater(void)
{
	String operation;

	// gets variables from environment
	// fails update if no operation found (shoudn't happen...)
	bool failed = false;
	
	if(environ.Find("UPDATER_USER") < 0)
		failed = true;
	else
		environ.RemoveKey("UPDATER_USER");

	if(environ.Find("UPDATER_OP") < 0)
		failed = true;
	else
	{
		operation = environ.Get("UPDATER_OP");
		environ.RemoveKey("UPDATER_OP");

		// setup update operation code
		if(operation == "INSTALL")
			updaterOp = Install;
		else if(operation == "UNINSTALL")
			updaterOp = Uninstall;
		else if(operation == "UPDATE")
			updaterOp = Update;
		else
			NEVER();
	
	}
	
	if(environ.Find("UPDATER_APPNAME") < 0)
		failed = true;
	else
	{
		appName = environ.Get("UPDATER_APPNAME");
		environ.RemoveKey("UPDATER_APPNAME");
	}
	environ.RemoveKey("UPDATER_STATE");

	if(environ.Find("UPDATER_CMDLINE") >= 0)
	{
		commandLine = environ.Get("UPDATER_CMDLINE");
		environ.RemoveKey("UPDATER_CMDLINE");
	}

	if(environ.Find("UPDATER_EXEPATH") < 0)
		failed = true;
	else
	{
		exePath = environ.Get("UPDATER_EXEPATH");
		environ.RemoveKey("UPDATER_EXEPATH");
	}

	// if failed getting environment data, signals and restarts in normal mode
	if(failed)
	{
		FailUpdate();
		RestartApp(RestartOrig);
		return false;
	}
	
	// execute the requested operation
	switch(updaterOp)
	{
		case Install:
			DO_Install();
			break;
			
		case Uninstall:
			DO_Uninstall();
			break;
			
		case Update:
			DO_Update();
			break;
			
		default:
			NEVER();
	}
	
	// returns false anyways, it shouldn't continue in superuser mode !
	return false;
}

void Updater::DO_Uninstall(void)
{
	// removes executable
	String path;
#ifdef PLATFORM_POSIX
			path = GetProgramsFolder() + "/" + appName;
#else
			path = GetProgramsFolder() + "/" + appName + "/" + appName + ".exe";
#endif
	FileDelete(path);
	
	// removes system config folder
	// leaves user one alone, it can contain user data
	// (on win32, that's also the program folder)
	DeleteFolderDeep(systemConfigPath);

	// signal op success and leave
	// as it was uninstalling, it will restart
	// launching app (uninstaller, possibily)
	SucceedUpdate();
	RestartApp(RestartTemp);
}

void Updater::DO_Install(void)
{
	// fetch the new app version and install it
	if(!FetchApp())
	{
		FailUpdate();
		RestartApp(RestartOrig);
	}
	else
	{
		SucceedUpdate();
		RestartApp(RestartNew);
	}
}

void Updater::DO_Update(void)
{
	// fetch the new app version and replace old one
	if(!FetchApp())
	{
		FailUpdate();
		RestartApp(RestartOrig);
	}
	else
	{
		SucceedUpdate();
		RestartApp(RestartNew);
	}
}

bool Updater::DO_UninstallFailed(void)
{
	Exclamation("Uninstall failed!&Aborting....");
	return false;
}

bool Updater::DO_InstallFailed(void)
{
	return PromptYesNo("Install failed&Do you want to run the app anyways?");
}

bool Updater::DO_UpdateFailed(void)
{
	return PromptYesNo("Update failed&Do you want to run the app anyways?");
}

bool Updater::DO_UninstallSucceeded(void)
{
	PromptOK("Uninstall succeeded!");
	return false;
}

bool Updater::DO_InstallSucceeded(void)
{
	PromptOK("Install succeeded!");
	return true;
}

bool Updater::DO_UpdateSucceeded(void)
{
	PromptOK("Update succeeded!");
	return true;
}

// restarts app in normal mode after updater mode
// either it re-launches the original one if update didn't succeed
// or the new one if it did OR the temporary app if uninstalling
void Updater::RestartApp(RestartModes restartMode)
{
	String path;
	switch(restartMode)
	{
		case RestartTemp:
			path = GetExeFilePath();
			break;
		case RestartOrig:
		default:
			path = exePath;
			break;
		case RestartNew:
#ifdef PLATFORM_POSIX
			path = GetProgramsFolder() + "/" + appName;
#else
			path = GetProgramsFolder() + "/" + appName + "/" + appName + ".exe";
#endif
			break;
	}
	SysStart(path, commandLine, environ);
}

// fetch list of available app versions
Vector<double>Updater::FetchVersions(void)
{
	Vector<double> res;
	
	HttpClient http;
	http.TimeoutMsecs(1000);
	http.URL(GetPlatformRoot() + "versions");

	// fetch version file from server
	String verStr = http.Execute();
	int err = http.GetStatusCode();
	if(verStr == "" || err != 200)
		return res;
	StringStream ss(verStr);
	while(!ss.IsEof())
	{
		String s = ss.GetLine();
		if(s != "")
			res.Add(ScanDouble(s));
	}
	return res;
}

// fetch MAX valid version, i.e. the greatest among all available
// but smaller or equal than maxVersion
double Updater::FetchMaxValidVersion(void)
{
	Vector<double>versions = FetchVersions();
	if(!versions.GetCount())
		return -1;
	double maxV = -1;
	for(int i = 0; i < versions.GetCount(); i++)
		if(versions[i] <= maxVersion && versions[i] > maxV)
			maxV = versions[i];
	return maxV;
}

// fetch the new app version from web server
// and replaces older one
// if ver is not specified, fetches the maximum available
// one but which is less than or equal maxVersion
bool Updater::FetchApp(double ver)
{
	int err = 0;

	if(ver < 0)
		ver = FetchMaxValidVersion();
	if(ver < 0)
		return false;

	HttpClient http;
	http.URL(GetPlatformRoot() + FormatDoubleFix(ver, 2, FD_ZERO) + "/" + appName);
	http.TimeoutMsecs(1000*60*30);
	http.MaxContentSize(100000000);

	// fetch version file from server
	Progress progress(t_("Downloading application.... please wait"));
	appBuffer = http.Execute(progress);
	err = http.GetStatusCode();
	if(err != 200 || http.IsAborted() || http.IsError())
	{
		appBuffer = "";
		return false;
	}
	
	// replaces/installs app
	String destPath;
#ifdef PLATFORM_POSIX
	destPath = GetProgramsFolder() + "/" + appName;
#else
	destPath = GetProgramsFolder() + "/" + appName + "/" + appName + ".exe";
#endif
	if(!SaveFile(destPath, appBuffer))
		return false;
#ifdef PLATFORM_POSIX
	if(chmod(~destPath, 0755) != 0)
		return false;
#endif

	// stores current version inside system config path
	String verStr = FormatDoubleFix(ver, 2, FD_ZERO);
	if(!SaveFile(systemConfigPath + "version", verStr))
		return false;
	installedVersion = ver;

	return true;
}


///////////////////////////////////////////////////////////////////////////////////////
// main updater call
// returns TRUE if app should continue, FALSE if should terminate
// BEWARE, app MUST check for return value AND behave as needed
// te SELF_UPDATE() macro does all what is needed
bool Updater::Run()
{
	switch(state)
	{
		case NormalRun :
			return DO_NormalRun();
		case InsideUpdater :
			return DO_InsideUpdater();
		case UninstallFailed :
			return DO_UninstallFailed();
		case InstallFailed :
			return DO_InstallFailed();
		case UpdateFailed :
			return DO_UpdateFailed();
		case UninstallSucceeded :
			return DO_UninstallSucceeded();
		case InstallSucceeded :
			return DO_InstallSucceeded();
		case UpdateSucceeded :
			return DO_UpdateSucceeded();
		default:
			NEVER();
			break;
	}
	return false;
}

// sets updater to manual mode -- if update is available, asks user
Updater &Updater::UpdateManual(void)
{
	SaveFile(userConfigPath + "UPDATER_MODE", "ASK");
	return *this;
}

// sets updater to auto -- updates app on launch without asking user
Updater &Updater::UpdateAuto(void)
{
	SaveFile(userConfigPath + "UPDATER_MODE", "AUTO");
	return *this;
}

// disable updater -- app will not update on launch
// re-enable it to restart updating system on next run
Updater &Updater::UpdateDisable(void)
{
	SaveFile(userConfigPath + "UPDATER_MODE", "DISABLE");
	return *this;
}
		
