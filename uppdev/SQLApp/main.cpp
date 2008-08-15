#include "SQLApp.h"



SQLApp::SQLApp()
{
	CtrlLayout(*this, "SQLApp");



	TableList.Hide();
	TableList.AddColumn("Table Name");

	CreateTables();

	BTAdd<<=THISBACK(OnAddBT);
	BTUpdate<<=THISBACK(OnUpdateBT);
	BTDelete<<=THISBACK(OnDeleteBT);
	BTFind<<=THISBACK(OnFindBT);
	BTClear<<=THISBACK(OnClearBT);
	BTShow<<=THISBACK(OnShowBT);

	MySqlData.SetTable(TEST);
	MySqlData.AddKey(ID);
	MySqlData.AddColumn(DATA1,"Data1");
	MySqlData.AddColumn(DATA2,"Data2");
	MySqlData.AddColumn(DATA3,"Data3");
	MySqlData.SetOrderBy(ID);

}

SQLApp::~SQLApp()
{
}

void SQLApp::CreateTables()
{
}

void SQLApp::OnAddBT()
{
	Sql sql(session);
	sql.Execute("use mysql");
	sql * Insert((SqlId)TEST)((SqlId)DATA1,(SqlVal)Data1.GetText()) \
						((SqlId)DATA2,(SqlVal)Data2.GetText()) \
						((SqlId)DATA3,(SqlVal)Data3.GetText());
}

void SQLApp::OnUpdateBT()
{
	Sql sql(session);
	sql.Execute("use mysql");
	sql * SqlUpdate((SqlId)TEST)((SqlId)DATA1,(SqlVal)Data1.GetText()) \
						((SqlId)DATA2,(SqlVal)Data2.GetText()) \
						((SqlId)DATA3,(SqlVal)Data3.GetText()).Where(ID == key);
}

void SQLApp::OnDeleteBT()
{
	Sql sql(session);
	sql.Execute("use mysql");
	sql * SqlDelete((SqlId)TEST).Where(ID == key);
}

void SQLApp::OnFindBT()
{
	String tmpString;
	String SQLString;
	SqlSet FindData;
	int keyval;
	Sql sql(session);
	sql.Execute("use mysql");

	if (Data1.IsModified())
	{
		tmpString = (String)~Data1;
		sql * Select(DATA1).From(TEST).Where(DATA1 == tmpString);
	}
	if (Data2.IsModified())
	{
//		tmpString = Data2;
//		sql * Select(DATA2).From(TEST).Where(DATA2 == tmpString);
	}
	if (Data3.IsModified())
	{
//		tmpString = Data3;
//		sql * Select(DATA3).From(TEST).Where(DATA3 == tmpString);
	}

	sql.Fetch();
	STKey.SetText((String)sql[0]);
	sql.Fetch();
	Data1.SetText((String)sql[0]);
	sql.Fetch();
	Data2.SetText((String)sql[0]);
	sql.Fetch();
	Data3.SetText((String)sql[0]);

}

void SQLApp::OnClearBT()
{
	Data1.Clear();
	Data2.Clear();
	Data3.Clear();
	STKey.SetText("");
	TableList.Clear();
	TableList.Hide();
}

void SQLApp::OnShowBT()
{
	Sql sql(session);
	sql.Execute("use mysql");
	sql.Execute("show tables");
	while(sql.Fetch())
		TableList.Add((String)sql[0]);
	TableList.Show();
	return;
}

GUI_APP_MAIN
{
	MySqlSession session;
	if(!session.Connect("test", "welcome1", "example", "localhost")) {
		Exclamation("Can't connect with MySql\n");
		exit (1);
	}

	SQL = session; // Use as global session

	SqlSchema sch;
	All_Tables(sch);
	if(sch.ScriptChanged(SqlSchema::UPGRADE))
		MySqlPerformScript(sch.Upgrade()); // Create schema without indexes, constraints etc...
	if(sch.ScriptChanged(SqlSchema::ATTRIBUTES)) {
		MySqlPerformScript(sch.Attributes()); // Add indexes, constraints...
	}
	if(sch.ScriptChanged(SqlSchema::CONFIG)) { // Fill with initial data if any
		MySqlPerformScript(sch.ConfigDrop());
		MySqlPerformScript(sch.Config());
	}
	sch.SaveNormal(); // Save as files for later use

	SQLApp().Run();
}
