#include "Updater.h"

#include <Draw/Draw.h>
#include <Web/Web.h>

#include <SysExecGui/SysExecGui.h>

#define IMAGECLASS UpdaterImg
#define IMAGEFILE <Updater/Updater.iml>
#include <Draw/iml.h>

NAMESPACE_UPP

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

///////////////////////////////////////////////////////////////////////////////////////
// constructor
Updater::Updater()
{
	// fetches and stores environment, we need to change it later
	environ <<= Environment();
	
	// accepts development versions by default
	acceptDevelVersions = true;

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
	if(state != InsideUpdater && state != UninstallSucceeded)
		appName = GetExeTitle();
	else if(environ.Find("UPDATER_APPNAME") >= 0)
		appName = environ.Get("UPDATER_APPNAME");
	else
		appName = GetExeTitle(); // fallout -- should NEVER get here
	
	// setup appName depending fields
	SetAppName(appName);
	
	// sets default confirm for install and uninstall
	confirmInstall = true;
	confirmUninstall = true;
	
	// no desktop icon installation by default
	desktopIcon = false;
}

// sets applicaton name (defaults with current executable title if not used)
// allows easy separation of installer from main application code
// if you want to deploy a small installer
Updater &Updater::SetAppName(String const &_appName)
{
	appName = _appName;
	
	// gets app's user config path
#ifdef PLATFORM_POSIX
	userConfigPath = "/home/" + user + "/." + appName + "/";
#else
	userConfigPath = GetAppDataFolder() + "/" + appName + "/";
#endif

	// gets app's system config path
#ifdef PLATFORM_POSIX
	systemConfigPath = "/usr/share/" + appName + "/";
#else
	systemConfigPath = GetProgramsFolder() + "/" + appName + "/";
#endif

	// checks wether app is already installed and gather its version
	installedVersion.Clear();
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
			installedVersion = verStr;
	}
	
	return *this;

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
			environ.Add("UPDATER_APPNAME", appName);
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
			environ.Add("UPDATER_APPNAME", appName);
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
// returns true if app must continue execution, false otherwise
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
#ifdef PLATFORM_WIN32
	tempName += ".exe";
#endif
	if(!FileCopy(exePath, tempName))
		return true;
	
#ifdef PLATFORM_POSIX
	// for posix, change temp file permission
	// allowing its execution and executes it as a superuser
	if(chmod(~tempName, 0755) != 0)
		return true;
#endif
	
	// executes the file asking for password
	bool res = !SysStartAdmin(tempName, "", environ);
	return res;

}

// uninstall app
// returns true if app must continue execution, false otherwise
bool Updater::START_Uninstall(void)
{
	if(confirmUninstall && !PromptYesNo(t_("This will remove '") + appName + t_("' application&Continue ?")))
	{
		state = UninstallAborted;
		return true;
	}
	if(START_Updater("UNINSTALL"))
	{
		state = UninstallFailed;
		return true;
	}
	return false;
}

// install app
// returns true if app must continue execution, false otherwise
bool Updater::START_Install(void)
{
	if(confirmInstall && !PromptYesNo(t_("Install '") + appName + t_("' application?")))
	{
		state = InstallAborted;
		return true;
	}
	if(START_Updater("INSTALL"))
	{
		state = InstallFailed;
		return true;
	}
	return false;
}

// update app
// returns true if app must continue execution, false otherwise
bool Updater::START_Update(void)
{
	if(START_Updater("UPDATE"))
	{
		state = UpdateFailed;
		return true;
	}
	return false;
}

// app started, must check for update/install requests
// returns true if app must continue execution, false otherwise
bool Updater::DO_NormalRun(void)
{
	// we now check if we just want to uninstall app
	// it can be done by command line --UNINSTALL option
	if(CommandLine().GetCount() && CommandLine()[0] == "--UNINSTALL")
		return START_Uninstall();
	
	// if app not installed, we shall install it
	if(!appInstalled)
		return START_Install();
	
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
	ProgramVersion maxVer;
	if( (maxVer = FetchMaxValidVersion(acceptDevelVersions)) <= installedVersion)
		return true;
	
	// if we want manual updates, just ask
	if(updateMode == "ASK")
		if(!PromptYesNo(t_("New version ") + maxVer.ToString() + t_(" is available&Install it ?")))
			return true;
	
	// updater enabled, start it
	// if update failed, resume normal run, otherwise exit
	// as the app will be launched again by updater itself
	return START_Update();
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
	
	// unlink application from shess
	ShellUnlink();

	// signal op success and leave
	// as it was uninstalling, it will restart
	// launching app (uninstaller, possibily)
	SucceedUpdate();
	RestartApp(RestartTemp);
}

void Updater::DO_Install(void)
{
	// fetch the new app version and install it
	if(!FetchApp(ProgramVersion(), acceptDevelVersions))
	{
		FailUpdate();
		RestartApp(RestartOrig);
	}
	else
	{
		// link app to shell
		ShellLink();
		
		SucceedUpdate();
		RestartApp(RestartNew);
	}
}

void Updater::DO_Update(void)
{
	// fetch the new app version and replace old one
	if(!FetchApp(ProgramVersion(), acceptDevelVersions))
	{
		FailUpdate();
		RestartApp(RestartOrig);
	}
	else
	{
		// link app to shell -- we do it also for updates
		// in case of icon changes or others
		ShellLink();
		
		SucceedUpdate();
		RestartApp(RestartNew);
	}
}

bool Updater::DO_UninstallFailed(void)
{
	state = UninstallFailed;
	return true;
}

bool Updater::DO_InstallFailed(void)
{
	state = InstallFailed;
	return true;
}

bool Updater::DO_UpdateFailed(void)
{
	state = UpdateFailed;
	return true;
}

bool Updater::DO_UninstallSucceeded(void)
{
	state = UninstallSucceeded;
	return true;
}

bool Updater::DO_InstallSucceeded(void)
{
	state = InstallSucceeded;
	return true;
}

bool Updater::DO_UpdateSucceeded(void)
{
	state = UpdateSucceeded;
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
	// restart app in user mode, no gui, no password should be needed
	SysStartUser(user, "", path, commandLine, environ);
}

// fetch list of available app versions
ProgramVersions Updater::FetchVersions(void)
{
	HttpClient http;
	http.TimeoutMsecs(1000);
	http.URL(GetPlatformRoot() + "versions");

	// fetch version file from server
	String verStr = http.Execute();
	int err = http.GetStatusCode();
	if(verStr == "" || err != 200)
		return ProgramVersions();

	return ProgramVersions(verStr);
}

// fetch MAX valid version, i.e. the greatest among all available
// but smaller or equal than maxVersion
ProgramVersion Updater::FetchMaxValidVersion(bool devel)
{
	ProgramVersions versions = FetchVersions();
	if(!versions.GetCount())
		return ProgramVersion();
	
	return versions.FindMax(ProgramVersion(), maxVersion, devel);
}

// fetch the new app version from web server
// and replaces older one
// if ver is not specified, fetches the maximum available
// one but which is less than or equal maxVersion
bool Updater::FetchApp(ProgramVersion ver, bool devel)
{
	int err = 0;

	if(!ver)
		ver = FetchMaxValidVersion(acceptDevelVersions);
	if(!ver)
		return false;

	HttpClient http;
#ifdef PLATFORM_POSIX
	http.URL(GetPlatformRoot() + ver.ToString() + "/" + appName);
#else
	http.URL(GetPlatformRoot() + ver.ToString() + "/" + appName + ".exe");
#endif
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
	if(!SaveFile(systemConfigPath + "version", ver.ToString()))
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
	// create user config path only on normal run
	if(state == NormalRun)
		RealizePath(userConfigPath);
	
	// creates system config path on superuser mode
	if(state == InsideUpdater)
		RealizePath(systemConfigPath);
	
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
	// dummy
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
		
// default set of prompts for installer result
// just an handy shortcut good for most cases
bool Updater::DefaultPrompts(void)
{
	// fine grained behaviour depending on update state
	switch(GetState())
	{
		case UninstallFailed:
			Exclamation(t_("Uninstall of '") + appName + t_("' failed&Press OK to quit"));
			return false;
	
		case UninstallSucceeded:
			PromptOK(t_("Uninstall of '") + appName + t_("' complete&Press OK to quit"));
			return false;
			
		case UninstallAborted:
			return false ;

		case InstallFailed:			
			if(!PromptYesNo(t_("Install of '") + appName + t_("' failed&Run without installing?")))
				return false;
			return true;
			
		case InstallSucceeded:
			return true;
		
		case InstallAborted:
			return false;
			
		case UpdateFailed:
			if(!PromptYesNo(t_("Update of '") + appName + t_("' failed&Run anyways?")))
				return false;
			return true;

		case UpdateSucceeded:
			return true;
		
		// here we're on normal run, no install/uninstall/update process happened
		// we should continue normal execution
		default:
			return true;
	}
}

// default icon
Image Updater::DefaultIcon(void)
{
	return UpdaterImg::DefaultIcon();
}

END_UPP_NAMESPACE
