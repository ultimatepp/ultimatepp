#include <Updater/Updater.h>

GUI_APP_MAIN
{
	Updater updater;
	updater
		.SetMaxVersion(1.60)
		.SetWebRoot("www.timberstruct.com/webupdater/updatertest")
		.UpdateManual();
	
	// run updater -- DON'T change this !
	if(!updater.Run())
		return;
	
	// run the default prompts -- see Updater source code
	// if you need some fine-grained control
	if(!updater.DefaultPrompts())
		return;
	
	PromptOK("App version is 1.00");

}

