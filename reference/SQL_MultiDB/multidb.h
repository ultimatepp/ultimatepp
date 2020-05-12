#ifndef _SQL_MultiDB_multidb_h_
#define _SQL_MultiDB_multidb_h_

#define SCHEMADIALECT <MySql/MySqlSchema.h>
#define MODEL <SQL_MultiDB/mysql_schema.sch>
#include <Sql/sch_header.h>
#undef SCHEMADIALECT

namespace DB2 {
#define SCHEMADIALECT <plugin/sqlite3/Sqlite3Schema.h>
#define MODEL <SQL_MultiDB/sqlite3_schema.sch>
#include <Sql/sch_header.h>
#undef SCHEMADIALECT
};

#endif
