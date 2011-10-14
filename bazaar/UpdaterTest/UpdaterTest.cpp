//////////////////////////////////////////////////////////////////////////////////////////////////
// Updater demo application																		//
// Once run the first time, downloads latest allowed version and installs it,					//
// setting up icons and file associations.														//
// It then setup the control panel uninstaller (windows only).									//
// On next runs it checks for updates on web repository and downloads/installs them on request.	//
// On uninstall (either by control panel or running it with --UNINSTALL argument, it			//
// does a complete cleanup of system data leaving user app data unchanged						//
///////////////////////////////////////////////////////////////////////////////////////////////////
#include <Updater/Updater.h>

using namespace Upp;

GUI_APP_MAIN
{
	Updater updater;
	updater
		// setup application name
		// if you don't use this, it takes current executable file title
		// this can be handy if you want to deploy a single small installer
		// application that installs the true one from web
		.SetAppName("myapp")
		
		// accept 1.6 as maximum downloadable version
		.SetMaxVersion(ProductVersion("1.6"))
		
		// accept development versions too (those like MM.mm.dd)
		.AcceptDevelVersions()
		
		// setup web root for program downloads
		.SetWebRoot("www.timberstruct.com/webupdater/updatertest")
		
		// set LAN path if download is local
		//.SetLANRoot(GetDesktopFolder())
		
		// ask for updates on launch
		.UpdateManual()
		
		// program menu goes in "Office" cathegory
		// (Linux only by now, no harm on windows)
		.SetCathegory("Office")
		
		// helper comment for application
		// (shows in associated files properties)
		.SetComment("A sample test program")
		
		// associates .xxx and .yyy extensions to this application
		// and sets application icons for this files
		.SetExtensions(Vector<String>() << "*.xxx" << "*.yyy")
		
		// setup the icon for application
		// (here Updater default icon)
		.SetIcon(updater.DefaultIcon())
		
		// we want a desktop icon too
		.DesktopIcon();
	
	// run updater -- DON'T change this !
	if(!updater.Run())
		return;
	
	// run the default prompts -- see Updater source code
	// if you need some fine-grained control
	if(!updater.DefaultPrompts())
		return;
	
	PromptOK("App version is 1.0");

}
