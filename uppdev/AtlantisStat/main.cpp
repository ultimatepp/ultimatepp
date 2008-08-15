#include "AtlantisStat.h"
#include <plugin/Sqlite3/Sqlite3.h>
//#include <iostream>
//#include "SQLiteWrapper.h"
#include <CtrlLib/CtrlLib.h>

AtlantisStat::AtlantisStat()
{
	CtrlLayout(*this, "Window title");
	select <<= THISBACK(DoQuery);
	game.AddColumn("1");
	game.AddColumn("2");
	game.AddColumn("3");
	game.AddColumn("4");
	game.AddColumn("5");
	game.AddColumn("6");
	tables.Add("player", "Player");
	tables.Add("settlements", "Settlements");
	tables.Add("inventory", "Inventory");
	tables.Add("building", "Building");
	menu.Set(THISBACK(MainMenu));
}

void AtlantisStat::MainMenu(Bar& bar)
{
	bar.Add("File", THISBACK(FileMenu));
}

void AtlantisStat::FileMenu(Bar& bar)
{
	bar.Add("Open", THISBACK(Open));
	bar.Add("Quit", THISBACK(Quit));
}

void AtlantisStat::DoQuery()
{
	String selectedtable;
	selectedtable = tables.GetData();
	//sql...

	PromptOK(AsString(selectedtable));

}

void AtlantisStat::Open()
{
// open a file

//	FileSelector fs;
	String fn;
	FileSel fs;
	fs.dir <<= GetCurrentDirectory();
	fs.Type( "RoA Stat files(.db)", "*.db, *.txt").DefaultExt("bmp").ExecuteOpen();
	fn = ~fs; // fn contains the file name
	if( fn != Null) { PromptOK(NFormat(t_("opening file [* \1%s\1]. Continue?"), fn));
	}

	Sqlite3Session session;

	if(!session.Open(fn)) {
        PromptOK("Can't connect to database file");
        return;
    }
    session.SetTrace();

	// Get the list of tables

	Vector<String> table_list = session.EnumTables("");

	DUMPC(table_list);

	game.Add(table_list.GetCount());

	//Sql sql(session);
	//game.Reset();|| game.Clear();
	//for (int i = 0; i < table_list.GetCount(); ++i)
	//    game.AddColoumn(table_list[i]);
	/*
	sql.Execute("select max(roundid) from Unit");
	sql.fetch();
	rounds.SetText(sql[0]);*/
	fitness.SetText("41");
	//default fillings to the table (.db info or players,killedunits,goods harvested, average hints by micromanager, average priorised hints by overmind class, executed hints by micro per round)

	//genom.SetText(gen);
	//session.Close();
}

void AtlantisStat::Quit()
{
	Break();
}

GUI_APP_MAIN
{
	AtlantisStat().Run();
}
