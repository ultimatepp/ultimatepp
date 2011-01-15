#include <Updater/Updater.h>

double GetMaxVersion(void)
{
	return 1.6;
}

GUI_APP_MAIN
{
	Updater updater;
	updater
		.SetMaxVersion(GetMaxVersion())
		.SetWebRoot("www.timberstruct.com/webupdater/updatertest")
		.UpdateManual();
	
	if(!updater.Run())
		return;
	
	PromptOK("App version is 1.00");

}

