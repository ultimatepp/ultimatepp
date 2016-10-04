#include "MainWorkSpace.h"

namespace UA{

void MainWorkSpace::mainMenu(Bar& CurrentBar){
	CurrentBar.Add(t_("File"), THISBACK(menuFile));
	CurrentBar.Add(t_("Tools"), THISBACK(menuTools));
	CurrentBar.Add(t_("Help"), THISBACK(menuHelp));
};

void MainWorkSpace::menuFile(Bar& CurrentBar){
	CurrentBar.Add(t_("Exit"), UAImg::UAExitImg(), THISBACK(Exit));
};

void MainWorkSpace::menuTools(Bar& CurrentBar){
	CurrentBar.Add(t_("Hide"), THISBACK(Hide));
	CurrentBar.Add(t_("Minimize"), THISBACK1(Minimize, true));
	CurrentBar.Add(t_("Options"), THISBACK(Options));
};

void MainWorkSpace::menuHelp(Bar& CurrentBar){
	CurrentBar.Add(AsString(t_("About")) + "...", THISBACK(About));
};

};
