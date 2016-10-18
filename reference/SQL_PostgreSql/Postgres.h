#ifndef _Postgres_Postgres_h
#define _Postgres_Postgres_h

#include <CtrlLib/CtrlLib.h>
#include <SqlCtrl/SqlCtrl.h>

using namespace Upp;

#define LAYOUTFILE <SQL_PostgreSql/Postgres.lay>
#include <CtrlCore/lay.h>

#include <PostgreSQL/PostgreSQL.h>
#define SCHEMADIALECT <PostgreSQL/PostgreSQLSchema.h>
#define MODEL <SQL_PostgreSql/database.sch>
#include "Sql/sch_header.h"

// debian: sudo apt-get install libpq-dev

class PostgreSQLTest : public WithPostgreSQLTestLayout<TopWindow> {
public:
	typedef PostgreSQLTest CLASSNAME;
	PostgreSQLTest();
	bool OpenDB();
	bool InsertTestData();
	void ShowTestData();
protected:
	PostgreSQLSession m_session;
};

#endif
