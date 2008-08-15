// When running this application and the help button is pressed the program crashes.
// Using the Topic++ editor the help file is edited fine.
// Using the Topic browser the table fails to show the left column of the table.
// Note it is important to use the full path as provided. ie MyApp should contain directory Bugs with HelpBug under it.
// If not then it will not crash, although it will still fail to show anything.

#include <CtrlLib/CtrlLib.h>

#define TOPICFILE <Bugs/HelpBug/Manual.tpp/all.i>
#include <Core/topic_group.h>


// ============================================================================
class APP : public TopWindow {
	MenuBar    Menu;
	ToolBar    Toolbar;
	HelpWindow Help;

	void ShowManual();
	void SetToolBar();
	void MainMenu(Bar &bar);
	void HelpMenu(Bar &bar);
	void MakeToolbar(Bar &bar);

public:
	typedef APP CLASSNAME;
	APP();
}*TheApp;

// ----------------------------------------------------------------------------
APP::APP() {
	AddFrame(Menu);
	AddFrame(TopSeparatorFrame());
	AddFrame(Toolbar);
	Menu.Set(callback(this,&CLASSNAME::MainMenu));
	Menu.WhenHelp = Toolbar.WhenHelp;
	SetToolBar();
	OpenMain();
}

// ----------------------------------------------------------------------------
void APP::HelpMenu(Bar &bar) {
	bar.Add(t_("Manual.."),CtrlImg::help(),callback(this,&CLASSNAME::ShowManual)).Key(K_F1).Help(t_("Provides a manual"));
}

// ----------------------------------------------------------------------------
void APP::MainMenu(Bar &bar) {
	bar.Add(t_("Help"),  callback(this,&CLASSNAME::HelpMenu));
}

// ----------------------------------------------------------------------------
void APP::MakeToolbar(Bar &bar) {
	HelpMenu(bar);
}

// ----------------------------------------------------------------------------
void APP::SetToolBar() {
	Toolbar.Set(callback(this,&CLASSNAME::MakeToolbar));
}

// ----------------------------------------------------------------------------
void APP::ShowManual() {
	Help.GoTo("topic://Bugs/HelpBug/Manual/Overview$en-us");
	Help.Execute();
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
GUI_APP_MAIN {
	SetLanguage(LNG_ENGLISH);
	TheApp = new APP;
	Ctrl::EventLoop();
	delete TheApp; // To stop heap leaks warning
}
