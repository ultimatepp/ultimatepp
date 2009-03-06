#include <SqlCtrl/SqlCtrl.h>
#include <plugin/sqlite3/Sqlite3.h>

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <SqlProblem/SqlProblem.lay>
#include <CtrlCore/lay.h>

#define SCHEMADIALECT <plugin/sqlite3/Sqlite3Schema.h>
#define MODEL <SqlProblem/mysql.sch>
#include "Sql/sch_header.h"



#include <Sql/sch_schema.h>
#include <Sql/sch_source.h>


#include <stdio.h>
#include <iostream>

class ClubsDlg : public WithClubsLayout<TopWindow> {
	typedef ClubsDlg CLASSNAME;

public:
	ClubsDlg()
	{
		CtrlLayoutOKCancel(*this, "");
		clubsList.SetTable(CLUBS);
		clubsList.AddKey(CLUB_ID);
		clubsList.AddColumn(CLUB_NAME, "NAME");
		clubsList.AddCtrl(CLUB_CITY, city);
		clubsList.Query();
	}
};



// ==========================================================
class CompetiteurDlg : public WithSaisieCompetiteursLayout<TopWindow> {
public:
	typedef CompetiteurDlg CLASSNAME;
	SqlCtrls ctrls;
	
public:
	CompetiteurDlg()
	{
		CtrlLayoutOKCancel(*this, "Competiteurs");
		ctrls(COMPH_NAME, name) (COMPH_REF_CLUB_ID, clubId);
		openClubsButton  << THISBACK(openClubs);
	}
	
	void openClubs()
	{
		ClubsDlg dlg;
		dlg.Run();
	}
};

// ==========================================================
class SqlProblem : public WithSqlProblemLayout<TopWindow> {
public:
	typedef SqlProblem CLASSNAME;
	SqlProblem()
	{
		CtrlLayout(*this, "Window title");
		openCompetiterButton << THISBACK(openCompetiter);
		openClubsButton << THISBACK(openClubs);
		openCommander << callback(SQLCommander);
	}
	
	void openCompetiter()
	{
		CompetiteurDlg dlg;
		Sql sql;
		sql * Select( dlg.ctrls ).From(COMPETITERS).Where(COMPH_ID == 1);
		if (dlg.ctrls.Fetch(sql))
		{
			dlg.RunAppModal();
		}
	}
	
	void openClubs()
	{
		ClubsDlg dlg;
		dlg.Run();
	}
};


// ==========================================================
GUI_APP_MAIN
{
	SQL;
	Sqlite3Session sqlite3;
	sqlite3.SetTrace();

	// DATA BASE
	// Gestion de la DB
	String statsusMsg = "Ouverture de la base de donnees";
	//statusBar.Set(statsusMsg);
	if(!sqlite3.Open(ConfigFile("FNPSA.db"))) {
		Exclamation("Can't create or open database file\n");
		return;
	}
	SQL = sqlite3;

	// if DB is empty ==> create structure
	{
		Vector<String> table_list = sqlite3.EnumTables("");
		if (table_list.GetCount() == 0)
		{
			statsusMsg = "Initialisation de la base de donnees en cours ...";
			//statusBar.Set(statsusMsg);

			// Update the schema to match the schema described in MODEL
			SqlSchema sch(SQLITE3);
			StdStatementExecutor se(sqlite3);
			All_Tables(sch);
			if(sch.ScriptChanged(SqlSchema::UPGRADE))
				Sqlite3PerformScript(sch.Upgrade(),se);
			if(sch.ScriptChanged(SqlSchema::ATTRIBUTES)) {
				Sqlite3PerformScript(sch.Attributes(),se);
			}
			if(sch.ScriptChanged(SqlSchema::CONFIG)) {
				Sqlite3PerformScript(sch.ConfigDrop(),se);
				Sqlite3PerformScript(sch.Config(),se);
			}
			sch.SaveNormal();

			statsusMsg = "Initialisation de la base de donnees termin�e";
			//statusBar.Set(statsusMsg);
		}
	}
	{
		// Get the list of tables
		Vector<String> table_list = sqlite3.EnumTables("");
		std::cout << "Tables: " << table_list.GetCount() << std::flush;
		LOG(Format("Tables: (%d)",table_list.GetCount()));
		for (int i = 0; i < table_list.GetCount(); ++i)
			LOG(Format("  #%d: %s",i+1,table_list[i]));
	}


	Sql sql(sqlite3);
	sql.ClearError();

	#define NEW_DB
	#ifdef NEW_DB

	sql.Begin();
	sql * Delete(CLUBS);
	sql * Delete(COMPETITERS);

	sql * Insert(CLUBS) (CLUB_NAME,"CLUB 1") (CLUB_CITY,"NY");
	sql * Insert(CLUBS) (CLUB_NAME,"CLUB 2") (CLUB_CITY,"PARIS");
	sql * Insert(CLUBS) (CLUB_NAME,"CLUB 3") (CLUB_CITY,"ROME");
	sql * Insert(CLUBS) (CLUB_NAME,"CLUB 4") (CLUB_CITY,"LONDON");

	sql * Insert(COMPETITERS) (COMPH_REF_CLUB_ID, 1)(COMPH_NAME, "Didier");
	sql * Insert(COMPETITERS) (COMPH_REF_CLUB_ID, 1)(COMPH_NAME, "Alex");
	sql * Insert(COMPETITERS) (COMPH_REF_CLUB_ID, 2)(COMPH_NAME, "Mathieu");
	sql * Insert(COMPETITERS) (COMPH_REF_CLUB_ID, 2)(COMPH_NAME, "Fred");

	// enregistrement de la DB
	sql.Commit();

	#endif
	// ======================== COMPETITION ==========================

	SqlProblem().Run();

}

