#include "MainWorkSpace.h"

namespace UA{

void MainWorkSpace::trayMenu(Bar& CurrentBar){
	if(isHidden)
		CurrentBar.Add(t_("Show application"), THISBACK(launch));
	else
		CurrentBar.Add(t_("Hide application"), THISBACK(hide));
	CurrentBar.Add(t_("Minimize"), THISBACK1(Minimize, true));
	CurrentBar.Add(t_("Maximize"), THISBACK1(Maximize, true));
	//CurrentBar.Add(t_("Restore"), THISBACK(Restore));
	CurrentBar.Add(AsString(t_("About")) + "...", THISBACK(About));
	CurrentBar.Add(t_("Exit"), THISBACK(Exit));
}
}