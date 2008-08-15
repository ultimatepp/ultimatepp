#include <SqlCtrl/SqlCtrl.h>
#include <plugin/sqlite3/Sqlite3.h>

using namespace Upp;

GUI_APP_MAIN
{
	FileSel fs;
	fs.Type("Sqlite3 database (*.db)", "*.db");
	fs.AllFilesType();
	LoadFromFile(fs);
	bool b = fs.ExecuteOpen("Open sqlite3 database");
	StoreToFile(fs);
	if(!b) return;
	Sqlite3Session sqlite3;
	if(!sqlite3.Open(~fs)) {
		Exclamation("Can't open database file\n");
		return;
	}
	SQL = sqlite3;
	SQLCommander();
}
