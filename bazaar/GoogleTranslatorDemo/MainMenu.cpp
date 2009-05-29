#include "main.h"

void GoogleTranslatorDemo::mainMenu(Bar& CurrentBar){
	CurrentBar.Add(t_("File"), THISBACK(menuFile));
	CurrentBar.Add(t_("Tools"), THISBACK(menuTools));
	CurrentBar.Add(t_("Help"), THISBACK(menuHelp));
};

void GoogleTranslatorDemo::menuFile(Bar& CurrentBar){
	CurrentBar.Add(t_("Exit"), GoogleTranslatorDemoImg::ExitImg(), THISBACK(Exit));
};


void GoogleTranslatorDemo::menuTools(Bar& CurrentBar){
	CurrentBar.Add(t_("Hide"), THISBACK(hide));
	CurrentBar.Add(t_("Options"), THISBACK(Options));
};

void GoogleTranslatorDemo::menuHelp(Bar& CurrentBar){
	CurrentBar.Add(AsString(t_("About")) + "...", THISBACK(About));
};
