#ifndef SQL_H
#define SQL_H

#include <Core/Core.h>

namespace Upp {

#ifdef flagNOAPPSQL
	#define NOAPPSQL
	#define APPSQLCURSOR
	#define APPSQLSESSION
#else
	#define APPSQLCURSOR = SQL
	#define APPSQLSESSION = SQL.GetSession()
#endif

#include "Sqlexp.h"
#include "Sqls.h"
#include "SqlSchema.h"

}

#endif
