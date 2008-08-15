#include "VegaMain.h"

void VegaMain::VegaMainMenu(Bar& menu)
{
	menu.Add("File", THISBACK(File) ).Key(K_CTRL_F);
	menu.Add("Extras", THISBACK(Extras) ).Key(K_CTRL_X);
	menu.Add("Help", THISBACK(Help) ).Key(K_CTRL_H);
}

void VegaMain::File(Bar& bar)
{
	bar.Add("New Tournament", THISBACK(NewTournament)).Key(K_CTRL_N);
	bar.Add("Open Tournament", CtrlImg::open(), THISBACK(OpenTournament)).Key(K_CTRL_O);
	bar.Add("Save Tournament", CtrlImg::save(), THISBACK(SaveTournament)).Key(K_CTRL_S);
	bar.Separator();
	bar.Add("Export", THISBACK(Export));
	bar.Add("Connect Database", THISBACK(ConnectDatabase));
	bar.Separator();
	bar.Add("Exit", THISBACK(Exit)).Key(K_CTRL_E);
}

void VegaMain::Export(Bar& bar)
{
	bar.Add("All Player", THISBACK1(ShowNumber, 1));
	bar.Add("Tournament", THISBACK1(ShowNumber, 2));
	bar.Add("Selected Player", THISBACK1(ShowNumber, 3));
}

void VegaMain::ConnectDatabase(Bar& bar)
{
	 bar.Add("Fide", THISBACK1(ShowNumber, 1));
	 bar.Add("VegaFSI (Italy)", THISBACK1(ShowNumber, 2));
	 bar.Add("Fixed Lenght", THISBACK1(ShowNumber, 3));
	 bar.Add("Vega", THISBACK1(ShowNumber, 4));
}

// from Extra
void VegaMain::FideReport(Bar& bar)
{
	bar.Add("Rating Report to FIDE", THISBACK1(ShowNumber, 1));
	bar.Add("Rating variation", THISBACK1(ShowNumber, 2));
	bar.Add("History card", THISBACK1(ShowNumber, 3));
	bar.Add("Find Norm", THISBACK1(ShowNumber, 4));
	bar.Add("FIDE norm certificate", THISBACK1(ShowNumber, 5));
	bar.Add("Statistics", THISBACK1(ShowNumber, 6));
}

void VegaMain::NationalReport(Bar& bar)
{
	bar.Add("FSI", THISBACK1(ShowNumber, 1));
	bar.Add("USCF", THISBACK1(ShowNumber, 2));
}

/*
void VegaMain::PlayerList(Bar& bar)
{
	bar.Add("sorte by name", THISBACK1(ShowNumber, 1));
	bar.Add("sorted by rating", THISBACK1(ShowNumber, 2));
}
*/

void VegaMain::Help(Bar& bar)
{
	bar.Add("Content", THISBACK(HelpContent));
	bar.Separator();
	bar.Add("Registered to ...", THISBACK(HelpRegistered));
	bar.Separator();
	bar.Add("About", THISBACK(HelpAbout));
}


//not finished...
void VegaMain::NewTournament()
{
	PromptOK( "New T NewTournament");
}

void VegaMain::OpenTournament()
{

	FileSel fs;
	fs.ExecuteOpen();
/*		if(filename.IsEmpty() && !editor.IsModified())
			Load(fs);
		else
			(new UWord)->Load(fs);
*/
//	else
//		statusbar.Temporary("Loading aborted.");
//	PromptOK( fs );
}

void VegaMain::SaveTournament()
{
	PromptOK( "Save T SaveTournament" );
}

void VegaMain::SortCrossTable()
{
	PromptOK( "c T SortCrossTable" );
}

void VegaMain::RankDateGroup()
{
	PromptOK( "c T RankDateGroup" );
}

void VegaMain::RankRatedGroup()
{
	PromptOK( "c T RankRatedGroup" );
}

void VegaMain::RoundRobinCalendar()
{
	PromptOK( "c T RoundRobinCalendar" );
}

void VegaMain::MPScoreSystem()
{
	PromptOK( "c T MPScoreSystem" );
}

void VegaMain::BackupVegFile()
{
	PromptOK( "c T BackupVegFile" );
}

void VegaMain::SelectLanguage()
{
	PromptOK( "c T SelectLanguage" );
}

void VegaMain::DBConversion(Bar& bar)
{
	bar.Add("USCF to fixed lenght", THISBACK1(ShowNumber, 1));
}
//end not finished

void VegaMain::DangerousStuff(Bar& bar)
{
	bar.Add("Load Round", THISBACK1(ShowNumber, 1));
	bar.Add("modify tournament", THISBACK1(ShowNumber, 2));
	bar.Add("reset tournament", THISBACK1(ShowNumber, 3));
	bar.Add("delete round", THISBACK1(ShowNumber, 4));
	bar.Add("reopen registration", THISBACK1(ShowNumber, 5));
}

// from Help
void VegaMain::HelpContent()
{
	PromptOK( "c T HelpContent" );
}

void VegaMain::HelpRegistered()
{
	PromptOK( "c T HelpRegistered" );
}

void VegaMain::HelpAbout()
{
	PromptOK( "c T HelpAbout" );
}

//from extras
void VegaMain::Extras(Bar& bar)
{
	bar.Add("Fide Report", THISBACK(FideReport));
	bar.Add("National Report", THISBACK(NationalReport));
	//bar.Add("Player List", THISBACK(PlayerList));
	bar.Add("Sort Cross Table", THISBACK(SortCrossTable));
	bar.Add("Rank Rating Group", THISBACK(RankRatedGroup));
	bar.Add("Rank Date Group", THISBACK(RankDateGroup));
	bar.Add("Round Robin Calendar", THISBACK(RoundRobinCalendar));
	bar.Add("Missing Point Score system", THISBACK(MPScoreSystem));
	bar.Separator();
	bar.Add("Backup *.veg file", THISBACK(BackupVegFile));
	bar.Add("Select Language", THISBACK(SelectLanguage));
	bar.Add("DB conversion utility", THISBACK(DBConversion));
	bar.Separator();
	bar.Add("Dangerous Stuff!", THISBACK(DangerousStuff));
}


void VegaMain::ShowNumber(int n)
{
	PromptOK(AsString(n));
}

//the last things...
void VegaMain::SetBars()
{
	mainmenu.Set(THISBACK(VegaMainMenu));
	//add toolbar if you have one
}

void VegaMain::Exit()
{
	Close();
}

void VegaMain::SetTabs()
{
	tabs.Add(tab1,"Players");
	tabs.Add(tab2,"Pairing");
	tabs.Add(tab3,"InsertResult");
	tabs.Add(tab4,"Position");
}

//we might move some from here
VegaConsoleFrame::VegaConsoleFrame()

{
	CtrlLayout(*this, "");
	AddFrame(ThinOutsetFrame());


	console.AddColumn("Time", "Time", 4);
	console.AddColumn("Who", "Who", 2);
	console.AddColumn("Action", "Action", 2);
	console.AddColumn("Info", "Info", 11);
	console.HeaderTab(0).SetInk(SLtRed);
	console.HeaderTab(1).SetInk(SLtRed);
	console.HeaderTab(2).SetInk(SLtRed);
	console.HeaderTab(3).SetInk(SLtRed);

	console.Add(AsString(GetSysTime()), "System" , "intilializing", "Ready...");

}

void VegaMain::UpdateInfo(const String& _who, const String& _action, const String& _what)
{
	int nTotal = tab1.arr.GetCount();
	int nAvail = tab1.GetAvail();
	String	info = NFormat("Total players: %d - Available: %d ... %s", nTotal, nAvail, _what);
	String time = AsString(GetSysTime());
	consoleFrame.console.Add(time, _who, _action, info);
	consoleFrame.console.ScrollInto(consoleFrame.console.GetCount());

}


String VegaMain::InfoCurrentPlayerAndStatus()
{
//	Animate(); //for animated status effects!
	int m = tab1.arr.GetCursor();
	String  player = AsString(tab1.arr.Get( m, "Name"));
	String s;
	Value v = tab1.arr.Get( m, "Status");
	if (IsNull(v))
		s = " got a BYE!";
	else
		s = v? " is AVAILable!" : " is NOT AVAILable!";

	return (" and now "+ player + s);
}


void VegaMain::arrCtrlsAction()
{
	UpdateInfo("optCtrl", "made", InfoCurrentPlayerAndStatus());
}


void VegaMain::btnAddAction()
{
	tab1.AddPlayer();
	UpdateInfo("btnAdd", "added", InfoCurrentPlayerAndStatus());
}

void VegaMain::Init()
{
	UpdateInfo("Luigi", "logged in", "");

	tab1.btnAdd.WhenAction = THISBACK(btnAddAction);
	//need for other layout buttons...
	tab1.arr.WhenCtrlsAction = THISBACK(arrCtrlsAction);
}
