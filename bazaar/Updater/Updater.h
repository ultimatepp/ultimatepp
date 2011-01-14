#ifndef _TimberStarter_Updater_h_
#define _TimberStarter_Updater_h_

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

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


class Updater
{
	private:
		// state of updater engine
		typedef enum
		{
			NormalRun, InsideUpdater,
			UninstallFailed, InstallFailed, UpdateFailed,
			UninstallSucceeded, InstallSucceeded, UpdateSucceeded
		} UpdaterState;
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
		void START_Uninstall(void);
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
		Vector<double>FetchVersions(void);
		double FetchMaxValidVersion(void);
		
		// fetch the new app version from web server
		// and replaces older one
		String appBuffer;
		bool FetchApp(double ver = -1);
		
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
		double installedVersion;

		// copy of starting command line -- needed to restart app correctly
		String commandLine;

		// running application name
		String appName;
		
		// copy of running executable path
		// when app was first launched in normal mode
		String exePath;

		// user variables
		double maxVersion;
		String webRoot;
		
	protected:

	public:
	
		Updater();
		
		Updater &SetMaxVersion(double mv) { maxVersion = mv; return *this; }
		Updater &SetWebRoot(String const &wr) { webRoot = wr; return *this; }
		
		// sets updater to manual mode -- if update is available, asks user
		Updater &UpdateManual(void);

		// sets updater to auto -- updates app on launch without asking user
		Updater &UpdateAuto(void);

		// disable updater -- app will not update on launch
		// re-enable it to restart updating system on next run
		Updater &UpdateDisable(void);
		
		bool IsAppInstalled(void) { return appInstalled; }
		double GetInstalledVersion(void) { return installedVersion; }
		double GetMaxVersion(void) { return maxVersion; }
		String const &GetWebRoot(void) { return webRoot; }
	
		bool Run(void);
};

#endif
