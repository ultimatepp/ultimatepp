#include "ide.h"

#define LLOG(x)  //RLOG(x)
#define LDUMP(x) //RDUMP(x)

#ifdef PLATFORM_POSIX

GLOBAL_VAR(UpdaterConfig,UpdaterCfg);

void Ide::CheckUpdates(bool verbose){
	LLOG("CheckUpdates, verbose="<<verbose);
	if(verbose){
		su.NeedsUpdate(true);
		SetBar();
	}else{
		su.WhenUpdateAvailable=THISBACK(SetBar);
		su.CheckUpdates();
	}
}

void Ide::CheckUpdatesManual(){
	int tmp=UpdaterCfg().ignored;
	UpdaterCfg().ignored=0;
	su.ClearError();
	if(su.NeedsUpdate(true)){
		su.Execute();
	}else{
		String err=su.GetError();
		if(err=="CANCEL") return;
		if(!err.IsEmpty()){
			Exclamation("Unable to check for updates. "+err);
		}else{
			PromptOK("No update found. You are using version "+su.GetLocal()+".");
		}
		UpdaterCfg().ignored=tmp;
	}
	SetBar();
}

void Ide::SetUpdateTimer(int period){
	LLOG("SetUpdateTimer, period="<<period);
	PostCallback(THISBACK1(CheckUpdates,false));
	SetTimeCallback(-60000*period,THISBACK1(CheckUpdates,false));
}
#endif
