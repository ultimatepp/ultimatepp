#include "multidb.h"

namespace DB2 {

#define SCHEMADIALECT <plugin/sqlite3/Sqlite3Schema.h>
#define MODEL <SQL_MultiDB/sqlite3_schema.sch>
#include <Sql/sch_source.h>
#include <Sql/sch_schema.h>
#undef MODEL
#undef SCHEMADIALECT

void DB2_All_Tables(SqlSchema& sch)
{ // we need a distinct name for second database schema creation function
	All_Tables(sch);
}

};
