#include <SqlCtrl/SqlCtrl.h>
#include <plugin/sqlite3/Sqlite3.h>

using namespace Upp;

#define MODEL <SqlArray/db.sch>

#define SCHEMADIALECT <plugin/sqlite3/Sqlite3Schema.h>
#include "Sql/sch_header.h"

#define SCHEMADIALECT <plugin/sqlite3/Sqlite3Schema.h>
#include "Sql/sch_source.h"

#define SCHEMADIALECT <plugin/sqlite3/Sqlite3Schema.h>
#include "Sql/sch_schema.h"

#define LAYOUTFILE <SqlArray/app.lay>
#include <CtrlCore/lay.h>


struct App : public WithAppLayout<TopWindow> {
	EditString author;
	EditString title;
	EditString person;

	App() {
		CtrlLayout(*this, "SqlArray SQL table editing example");
		master.SetTable(BOOK);
		master.AddKey(ID);
		master.AddColumn(AUTHOR, "Author").Edit(author);
		master.AddColumn(TITLE, "Title").Edit(title);
		master.Appending().Removing();
		master.SetOrderBy(AUTHOR, TITLE);
		
		detail.SetTable(BORROW_RECORD);
		detail.AddKey(ID);
		detail.Join(BOOK_ID, master);
		detail.AddColumn(PERSON, "Borrowed by").Edit(person);
		detail.AddCtrl(BORROWED, borrowed);
		detail.AddCtrl(RETURNED, returned);
		detail.Appending().Removing();
		detail.SetOrderBy(PERSON);
		
		master.Query();
	}
};

GUI_APP_MAIN
{
	Sqlite3Session sqlite3;
	if(!sqlite3.Open(ConfigFile("simple.db"))) {
		Exclamation("Can't create or open database file\n");
		return;
	}
	SQL = sqlite3;
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
	sqlite3.SetTrace();
	
	App().Run();
}
