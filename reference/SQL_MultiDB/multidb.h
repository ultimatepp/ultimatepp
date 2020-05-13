#ifndef _SQL_MultiDB_multidb_h_
#define _SQL_MultiDB_multidb_h_

#include <MySql/MySql.h>
#include <plugin/sqlite3/Sqlite3.h>

using namespace Upp;

#define SCHEMADIALECT <MySql/MySqlSchema.h>
#define MODEL <SQL_MultiDB/mysql_schema.sch>
#include <Sql/sch_header.h>
#undef MODEL         // with multiple schemas, we need to do cleanup so that MODEL and
#undef SCHEMADIALECT // SCHEMDIALECT can be redefined to second schema values

namespace DB2 {
	
#define SCHEMADIALECT <plugin/sqlite3/Sqlite3Schema.h>
#define MODEL <SQL_MultiDB/sqlite3_schema.sch>
#include <Sql/sch_header.h>
#undef MODEL
#undef SCHEMADIALECT

void DB2_All_Tables(SqlSchema& sch);

};

#endif
