#include "Postgres.h"

#include <Sql/sch_schema.h>
#include <Sql/sch_source.h>

using namespace Upp;

PostgreSQLTest::PostgreSQLTest()
{
	CtrlLayout(*this, "PostgreSQL Test");
	m_refresh <<= THISBACK(ShowTestData);
	m_array.SetTable(TESTPARTNER);
	m_array.AddKey(TESTPARTNER_ID);
	m_array.AddColumn(TESTPARTNER_NAME, "Name");
	m_array.AddColumn(TESTPARTNER_ADDRESS, "Address");
}

bool PostgreSQLTest::OpenDB()
{
	if(!m_session.Open("host=localhost dbname=test user=postgres password=pepicek12A"))
	{
		Exclamation(Format("Error in open: %s", DeQtf(m_session.GetLastError())));
		return false;
	}
	m_array.SetSession(m_session);
	
	SQLCommander(m_session);
	return false;

#ifdef _DEBUG
	m_session.SetTrace();
#endif

	SQL = m_session;

	//schema
	Progress p;
	p.SetText(t_("Creating _DEBUG database"));
	SqlSchema sch(PGSQL);
	StdStatementExecutor se(m_session);
	All_Tables(sch);

	PostgreSQLPerformScript(sch.Upgrade(),se, p);
	PostgreSQLPerformScript(sch.Attributes(),se, p);

	sch.SaveNormal();

	return true;
}

bool PostgreSQLTest::InsertTestData()
{
	Sql sql(m_session);
	sql.ClearError();
	sql.Begin();
	sql * Delete(TESTPARTNER);
	sql * Insert(TESTPARTNER)
		(TESTPARTNER_NAME, ToCharset(CHARSET_DEFAULT, "Žáček Pepíček je šikula", CHARSET_UTF8))
		(TESTPARTNER_ADDRESS, "First Street")
	;
	sql * Insert(TESTPARTNER)
		(TESTPARTNER_NAME, "Second Partner")
		(TESTPARTNER_ADDRESS, "Second Street")
	;
	sql * Insert(TESTPARTNER)
		(TESTPARTNER_NAME, "Third Partner")
		(TESTPARTNER_ADDRESS, "Third Street")
	;
	if(!OkCommit(m_session, "Unable to insert test data."))
		return false;

	return true;
}

void PostgreSQLTest::ShowTestData()
{
	m_array.SetOrderBy(TESTPARTNER_NAME);
	m_array.Query();
}




GUI_APP_MAIN
{
//	SetDefaultCharset(CHARSET_WIN1250);
	PostgreSQLTest dlg;
	if(dlg.OpenDB() && dlg.InsertTestData())
	{
		dlg.ShowTestData();
		dlg.Run();
	}
}
