#include "SQLApp.h"

#define SCHEMADIALECT <plugin/sqlite3/Sqlite3Schema.h>
#include "Sql/sch_source.h"

void SQLApp::MainMenu(Bar& bar)
{
	bar.Add("Book", THISBACK(BookMenu));
	bar.Add("Borrow record", THISBACK(BorrowMenu));
#ifdef _DEBUG
	bar.Add("SQL console", callback(SQLCommander));
#endif
}

SQLApp::SQLApp()
{
	CtrlLayout(*this, "Window title");
	menu.Set(THISBACK(MainMenu));

	book.SetTable(BOOK);
	book.AddKey(ID);
	book.AddColumn(AUTHOR, "Author");
	book.AddColumn(TITLE, "Title");
	book.SetOrderBy(TITLE, AUTHOR);
	book.WhenBar = THISBACK(BookMenu);
	book.WhenLeftDouble = THISBACK(EditBook);

	borrowed.SetTable(BORROW_RECORD);
	borrowed.AddKey(ID);
	borrowed.Join(BOOK_ID, book);
	borrowed.AddColumn(BORROWED, "Borrowed").SetConvert(DateIntConvert());
	borrowed.AddColumn(RETURNED, "Returned").SetConvert(DateIntConvert());
	borrowed.AddColumn(PERSON, "Who borrowed");
	borrowed.ColumnWidths("75 75 372");
	borrowed.SetOrderBy(BORROWED);
	borrowed.WhenBar = THISBACK(BorrowMenu);
	borrowed.WhenLeftDouble = THISBACK(EditBorrow);
	borrowed.GoEndPostQuery();

	CtrlLayoutOKCancel(query, "Select");
	query.status = 0;
	query.borrowed_from.SetConvert(DateIntConvert());
	query.borrowed_to.SetConvert(DateIntConvert());
	query.clear <<= THISBACK(ClearQuery);
}

#ifdef _DEBUG
#define SCHEMADIALECT <plugin/sqlite3/Sqlite3Schema.h>
#include "Sql/sch_schema.h"
#endif

GUI_APP_MAIN
{
	SQL;

	Sqlite3Session sqlite3;
	if(!sqlite3.Open(ConfigFile("simple.db"))) {
		Exclamation("Can't create or open database file\n");
		return;
	}

	SQL = sqlite3;

#ifdef _DEBUG
	SqlSchema sch(SQLITE3);
	sqlite3.SetTrace();
	All_Tables(sch);
	if(sch.ScriptChanged(SqlSchema::UPGRADE))
		Sqlite3PerformScript(sch.Upgrade());
	if(sch.ScriptChanged(SqlSchema::ATTRIBUTES))
		Sqlite3PerformScript(sch.Attributes());
	if(sch.ScriptChanged(SqlSchema::CONFIG)) {
		Sqlite3PerformScript(sch.ConfigDrop());
		Sqlite3PerformScript(sch.Config());
	}
	sch.SaveNormal();
	sqlite3.SetTrace();
#endif

	SQLApp app;
	LoadFromFile(app);
	app.Query();
	app.Run();
	StoreToFile(app);
}
