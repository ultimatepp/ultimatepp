#ifndef _app_app_h
#define _app_app_h

#include <Sql/Sql.h>

using namespace Upp;

// debian: sudo apt-get install unixodbc-dev

#include <MSSQL/MSSQL.h>
#define SCHEMADIALECT <MSSQL/MSSQLSchema.h>
#define MODEL <SQL_MSSQL/database.sch>
#include "Sql/sch_header.h"

#endif
