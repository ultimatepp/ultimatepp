#ifndef _TimberStarter_Updater_h_
#define _TimberStarter_Updater_h_

#include <CtrlLib/CtrlLib.h>

#include <ProductVersion/ProductVersion.h>

NAMESPACE_UPP

class Updater
{
	public:
		typedef Updater CLASSNAME;
		
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
		
		// choices on error conditions
		typedef enum { AbortExecution, AskUser, ContinueExecution } ErrorBehaviours;

	private:
	
		// progress bar, used to show/abort download
		Progress progress;
		
		// progress callback
		void doProgress(void);
		
		// http request object used to fetch file when on server
		HttpRequest http;

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
		ProductVersions FetchVersions(void);
		ProductVersion FetchMaxValidVersion(bool devel);
		
		// fetch the new app version from web server
		// and replaces older one
		String appBuffer;
		bool FetchApp(ProductVersion ver, bool devel);
		
		// running copy of environment
		VectorMap<String, String>environment;
		
		// user name (which launched app in normal mode
		String user;
		
		// user configuration path
		String userConfigPath;
		
		// sistem configuration path (to keep installed version number)
		String systemConfigPath;
		
		// flag stating if app is installed
		bool appInstalled;
		
		// current installed version, if any
		ProductVersion installedVersion;

		// copy of starting command line -- needed to restart app correctly
		String commandLine;

		// running application name
		String appName;
		
		// copy of running executable path
		// when app was first launched in normal mode
		String exePath;

		// maximum allowable version on install/update
		bool acceptDevelVersions;
		ProductVersion maxVersion;
		
		// root of web server repository
		String serverRoot;
		
		// selects between web server mode or local mode
		bool isWebServer;
		
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
		
		// user's choices on how to handle error contidions
		ErrorBehaviours installBehaviour;
		ErrorBehaviours noInstallBehaviour;
		ErrorBehaviours updateBehaviour;
		
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
		
		Updater &SetMaxVersion(ProductVersion const &mv) { maxVersion = mv; return *this; }
		Updater &SetWebRoot(String const &wr) { serverRoot = wr; isWebServer = true; return *this; }
		Updater &SetLANRoot(String const &wr) { serverRoot = wr; isWebServer = false; return *this; }
		
		// gets user config folder (/home/user/.appname in Linux, c:\Users\user\Application data\appname in windows)
		String const &GetUserConfigPath(void) { return userConfigPath; }
		
		// gets system config path (/usr/share/appname in Linux, c:\Program Files\appname in windows)
		String const &GetSystemConfigPath(void) { return systemConfigPath; }
		
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
		ProductVersion const &GetInstalledVersion(void) { return installedVersion; }
		ProductVersion const &GetMaxVersion(void) { return maxVersion; }
		String const &GetWebRoot(void) { return serverRoot; }
		String const &GetServerRoot(void) { return serverRoot; }
		bool IsWebServer(void) { return isWebServer; }
	
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
		
		// sets behaviours on failures
		Updater &SetInstallBehaviour(ErrorBehaviours b) { installBehaviour = b; return *this; }
		Updater &SetNoInstallBehaviour(ErrorBehaviours b) { noInstallBehaviour = b; return *this; }
		Updater &SetUpdateBehaviour(ErrorBehaviours b) { updateBehaviour = b; return *this; }
		
		// checks wether we're in ROOT mode
		bool IsRoot(void) { return state == InsideUpdater; }

};

END_UPP_NAMESPACE

#endif
