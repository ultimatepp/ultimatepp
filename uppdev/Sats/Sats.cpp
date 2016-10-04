#include "Sats.h"

void Sats::Setup()
{
	CtrlLayout(*this, "Soldier Accession Tracking System");
}

GUI_APP_MAIN
{
	Sats s;
	s.Setup();
	Sqlite3Session db;
	db.LogErrors(true);
	
	FileIn fi("satsII.db");
	if(fi.IsError() || fi.GetSize() <= 0) {
		Exclamation(t_("Database file is corrupted"));
		return;
	}
	fi.Close();
	
	if(!db.Open(ConfigFile("satsII.db"))) {
		Exclamation(t_("Can't open database"));
		return;
	}
	
	SQL = db;
	
	SQL * Select(Code).From(Race);
	
	while(SQL.Fetch()) {
		s.d_race.Add(SQL[0]);
	}
	
	s.Run();
}

