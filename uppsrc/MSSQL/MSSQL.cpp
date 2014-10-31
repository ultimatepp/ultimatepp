#include "MSSQL.h"

namespace Upp {

String MsSqlTextType(int width)
{
	if(width <= 4000)
		return NFormat("varchar(%d)", width);
	return "text";
}

void IdentityInsert(Sql& sql, const SqlInsert& ins)
{
	sql.Execute("SET IDENTITY_INSERT " + ins.GetTable().ToString() + " ON;" +
	            ((SqlStatement)ins).Get(MSSQL));
}

void IdentityInsert(const SqlInsert& ins)
{
	IdentityInsert(SQL, ins);
}

Value MsSqlSequence::Get()
{
	ASSERT(seq);
#ifndef NOAPPSQL
	Sql sql(session ? *session : SQL.GetSession());
#else
	ASSERT(session);
	Sql sql(*session);
#endif
	if(!sql.Execute("select next value for " + ~*seq) || !sql.Fetch())
		return ErrorValue();
	return sql[0];
}

MsSqlSequence::MsSqlSequence(const char *seq_name)
{
	INTERLOCKED {
		static ArrayMap<String, SqlId> ids;
		seq = &ids.GetAdd(seq_name, SqlId(seq_name));
	};
}

};
