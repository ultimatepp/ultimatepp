#ifndef SQL_H
#define SQL_H

#include <Core/Core.h>

NAMESPACE_UPP

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

END_UPP_NAMESPACE

#endif
