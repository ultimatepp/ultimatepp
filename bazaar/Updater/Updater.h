#ifndef _TimberStarter_Updater_h_
#define _TimberStarter_Updater_h_

#include <CtrlLib/CtrlLib.h>

#include "ProgramVersion.h"

NAMESPACE_UPP

/*
#define SELFUPDATE_OK				0
#define SELFUPDATE_NO_NETWORK		1
#define SELFUPDATE_NO_VERSIONFILE	2
#define SELFUPDATE_NO_APPFILE		3
#define SELFUPDATE_NO_PERMISSION	4

extern int SELFUPDATE_RESULT;

#define SELFUPDATE(remoteRoot, appName, currentVersion, maxVersion) \
	SELFUPDATE_RESULT = __SelfUpdate(remoteRoot, appName, currentVersion, maxVersion); \
	if(SELFUPDATE_RESULT == -1) \
		return;

// UPDATE HELPER USED BY SELFUPDATE MACRO

// IF APP STARTED NORMALLY :
//		stores current command line on a temporary file
//		restarts current applcation copying to temporary
//			folder and acting as a superuser
//		parameters passed to application :
//			--UPDATE			update command
//			exepath				complete path of itself
//			cmdlinepath			path of the command line file
//		returns 0 if failed, 1 if success
// IF APP STARTED BY UPDATER (as a superuser):
//		fetches the update file in a temporary folder
//		overwrite the old executable with the new one
//		restarts as a normal user the executable with original command line
//		returns 2 if failed, 3 if success
//		if failed, adds --UPDATEFAILED as last cmdline parameter
//		if success, adds --UPDATEOK as last cmdline parameter
//
// IF APP STARTED WITH --UPDATEFAILED AS LAST PARAMETER
//		returns 4
//
// IF APP STARTED WITH --INSTALLFAILED AS LAST PARAMETER
//		returns 5
//
// IF APP STARTED WITH --UPDATEOK AS LAST PARAMETER
//		just return 1 and skip further processing
int __SelfUpdate(String const &remoteRoot, String const &appName, String const &currentVersion, String const &maxVersion);

*/

class Updater
{
	public:
		// state of updater engine
		typedef enum
		{
			// internal states
			NormalRun, InsideUpdater,
			
			// public states
			UninstallFailed, InstallFailed, UpdateFailed,
			UninstallSucceeded, InstallSucceeded, UpdateSucceeded,
			UninstallAborted, InstallAborted
		} UpdaterState;

	private:
		// state of updater engine
		UpdaterState state;
		
		// requested operation
		typedef enum { Install, Update, Uninstall } UpdaterOp;
		UpdaterOp updaterOp;
		
		// setup environment for failing update/install
		void FailUpdate(void);

		// setup environment for succeeding update/install
		void SucceedUpdate(void);

		// main updater proc inside normal mode
		// prepares environment, copy app in temporary folder
		// and restarts it in superuser mode
		bool START_Updater(String const &operation);

		// operations procedures
		bool START_Uninstall(void);
		void DO_Uninstall(void);
		bool START_Install(void);
		void DO_Install(void);
		bool START_Update(void);
		void DO_Update(void);

		// installer/updater subfunctions
		bool DO_NormalRun(void);
		bool DO_InsideUpdater(void);
		bool DO_UninstallFailed(void);
		bool DO_InstallFailed(void);
		bool DO_UpdateFailed(void);
		bool DO_UninstallSucceeded(void);
		bool DO_InstallSucceeded(void);
		bool DO_UpdateSucceeded(void);
		
		// gets platform root folder
		String GetPlatformRoot(void);

		// restarts app in normal mode after updater mode
		typedef enum { RestartOrig, RestartNew, RestartTemp } RestartModes;
		void RestartApp(RestartModes restartMode);
		
		// fetch list of available app versions
		ProgramVersions FetchVersions(void);
		ProgramVersion FetchMaxValidVersion(bool devel);
		
		// fetch the new app version from web server
		// and replaces older one
		String appBuffer;
		bool FetchApp(ProgramVersion ver, bool devel);
		
		// running copy of environment
		VectorMap<String, String>environ;
		
		// user name (which launched app in normal mode
		String user;
		
		// user configuration path
		String userConfigPath;
		
		// sistem configuration path (to keep installed version number)
		String systemConfigPath;
		
		// flag stating if app is installed
		bool appInstalled;
		
		// current installed version, if any
		ProgramVersion installedVersion;

		// copy of starting command line -- needed to restart app correctly
		String commandLine;

		// running application name
		String appName;
		
		// copy of running executable path
		// when app was first launched in normal mode
		String exePath;

		// maximum allowable version on install/update
		bool acceptDevelVersions;
		ProgramVersion maxVersion;
		
		// root ow web server repository
		String webRoot;
		
		// install mode -- manual (true) or auto (false)
		bool confirmInstall;
		
		// uninstall mode -- manual (true) or auto (false)
		bool confirmUninstall;
		
		// application details for installation
		Image icon;
		String cathegory;
		Vector<String> extensions;
		String comment;
		
		// flag for desktop icon install
		bool desktopIcon;
		
#ifdef PLATFORM_POSIX
		// scans for theme folders on which put/delete the mimetype icons
		// that's needed because if themed icons aren't available, the system
		// uses fallback ones instead of going through hicolor ones
		// btw...that must be some ill-brained people there
		// the routine builds a list of folders containing 32x32 icons
		// it would be of course better with svg, but Upp don't support them
		Vector<String>ScanTheme(void);
		
		// save an image as png inside multiple theme folders
		// retrieved with ScanTheme
		void InstallThemeIcons(Image const &img, String const &name);
		
		// deletes all icons with given name inside folders retrieved with ScanTheme
		void RemoveThemeIcons(String const &name);
#else
		String GetShellFolder(const char *name, HKEY type);
		void DelKey(const char *dir, const char *key);
		bool CreateShellLink(const char *filepath, const char *linkpath, const char *desc, const char *iconPath);
		bool RegisterApplication(String const &appName, String const &appPath);
		bool UnregisterApplication(String const &appName);
		bool SetFileAssociation(String const &appName, String const &ext, Image const &icon);
		bool RemoveFileAssociation(String const &ext);
#endif

		// links application to OS shell
		// (i.e., add icon, menu entry, mimetype....)
		bool ShellLink(void);
		
		// unlinks application
		bool ShellUnlink(void);
		
	protected:

	public:
	
		Updater();
		
		Updater &SetMaxVersion(ProgramVersion const &mv) { maxVersion = mv; return *this; }
		Updater &SetWebRoot(String const &wr) { webRoot = wr; return *this; }
		
		// sets updater to manual mode -- if update is available, asks user
		Updater &UpdateManual(void);

		// sets updater to auto -- updates app on launch without asking user
		Updater &UpdateAuto(void);

		// disable updater -- app will not update on launch
		// re-enable it to restart updating system on next run
		Updater &UpdateDisable(void);
		
		// sets installer to auto mode (don't ask user)
		Updater &NoConfirmInstall(void) { confirmInstall = false; return *this; }

		// sets installer to manual mode (ask user)
		Updater &ConfirmInstall(void) { confirmInstall = true; return *this; }

		// sets uninstaller to auto mode (don't ask user)
		Updater &NoConfirmUninstall(void) { confirmUninstall = false; return *this; }

		// sets uninstaller to manual mode (ask user)
		Updater &ConfirmUninstall(void) { confirmUninstall = true; return *this; }
		
		// accept/reject development versions
		Updater &AcceptDevelVersions(void) { acceptDevelVersions = true; return *this; }
		Updater &NoAcceptDevelVersions(void) { acceptDevelVersions = false; return *this; }

		bool IsAppInstalled(void) { return appInstalled; }
		ProgramVersion const &GetInstalledVersion(void) { return installedVersion; }
		ProgramVersion const &GetMaxVersion(void) { return maxVersion; }
		String const &GetWebRoot(void) { return webRoot; }
	
		// executes updater
		// return true if app should continue execution
		// return false if app should exit
		// BEWARE -- you NEED to test this and leave application if false !
		bool Run(void);

		// get updater result status
		UpdaterState GetState(void) { return state; }
		
		// default set of prompts for installer result
		// just an handy shortcut good for most cases
		bool DefaultPrompts(void);
		
		// default icon
		Image DefaultIcon(void);
		
		// sets applicaton name (defaults with current executable title if not used)
		// allows easy separation of installer from main application code
		// if you want to deploy a small installer
		Updater &SetAppName(String const &appName);
		
		// setup application icon
		Updater &SetIcon(Image const &i) { icon = i; return *this; }
		
		// setup application cathegory
		Updater &SetCathegory(String const &cat) { cathegory = cat; return *this; }

		// setup extensions associated with app
		Updater &SetExtensions(Vector<String> const &ext) { extensions <<= ext; return *this; }
		
		// setup application comment
		Updater &SetComment(String const &c) { comment = c; return *this; }
		
		// desktop icon
		Updater &DesktopIcon(void) { desktopIcon = true; return *this; }
		Updater &NoDesktopIcon(void) { desktopIcon = false; return *this; }

};

END_UPP_NAMESPACE

#endif
