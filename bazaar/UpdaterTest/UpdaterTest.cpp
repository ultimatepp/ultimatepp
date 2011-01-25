#include <Updater/Updater.h>

using namespace Upp;

GUI_APP_MAIN
{
	Updater updater;
	updater
		.SetMaxVersion(1.6)
		.SetWebRoot("www.timberstruct.com/webupdater/updatertest")
		.UpdateManual()
		.SetCathegory("Office")
		.SetComment("A sample test program")
		.SetExtensions(Vector<String>() << "*.xxx" << "*.yyy")
		.SetIcon(updater.DefaultIcon())
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

