#include <Updater/Updater.h>

double GetMaxVersion(void)
{
	return 2.0;
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
	
//	PromptOK("After update call -- should be in normal mode&User is : '" + GetUserName() + "'");

}

