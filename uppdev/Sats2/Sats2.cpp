#include "Sats2.h"



Sats::Sats()
{
	CtrlLayout(*this, "Soldier Accession Tracking System");
	CtrlLayout(ISTAdd);
	tab.Add(ISTAdd, "Add Soldier");
	
	SQL * Select(Code).From(Race);
	while(SQL.Fetch()) 
		ISTAdd.txtRace.Add(SQL[0]);
}


GUI_APP_MAIN
{
	// Start a SQLite3 session
	Sqlite3Session db;
	db.LogErrors(true);
	FileIn fi("satsII.db");
	if (fi.IsError() || fi.GetSize() <= 0) {
		Exclamation(t_("Database file is corrupted"));
		return;
	}
	fi.Close();
	
	SQL = db;
	
	Sats().Run();
}
