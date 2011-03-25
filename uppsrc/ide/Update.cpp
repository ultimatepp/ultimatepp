#include "ide.h"

#ifdef PLATFORM_POSIX

GLOBAL_VAR(UpdaterConfig,UpdaterCfg);

void Ide::CheckUpdates(bool verbose){
	RLOG("CheckUpdates verbose: "<<verbose);
	if(verbose){
		RDUMP(su.NeedsUpdate(true));
		SetBar();
	}else{
		RLOG("checkupdates");
		su.WhenUpdateAvailable=THISBACK(SetBar);
		su.CheckUpdates();
	}
}

void Ide::CheckUpdatesManual(){
	int tmp=UpdaterCfg().ignored;
	UpdaterCfg().ignored=0;
	if(su.NeedsUpdate(true)){
		su.Execute();
	}else{
		String err=su.GetError();
		if(!err.IsEmpty()){
			Exclamation("Unable to check for updates. "+err);
		}else{
			PromptOK("No update found. You are using version "+su.GetLocal()+".");
		}
		UpdaterCfg().ignored=tmp;
	}
	SetBar();
}

#endif
