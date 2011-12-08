//////////////////////////////////////////////////////////////////////
// dict: user privileges, relocation, index removal.

#include "TSql.h"
#pragma hdrstop

NAMESPACE_UPP

#ifndef NOAPPSQL
int SqlUserGetRightsTo(const char* table)
{
	return SqlUser().GetRightsTo(table);
}
#endif

#ifndef NOAPPSQL
bool SqlUserCanWrite(const char* table)
{
	return SqlUser().CanWrite(table);
}
#endif

#ifndef NOAPPSQL
bool SqlUserCanRead(const char* table)
{
	return SqlUser().CanRead(table);
}
#endif

SqlAnyTable::SqlAnyTable()
{
}

SqlAnyTable::SqlAnyTable(const char *table_name, SqlSession& session)
{
	const char *n = strchr(table_name, '.');
	if(n)
	{ // full specification
		owner = ToUpper(String(table, n - table_name));
		table = ToUpper(n + 1);
	}
	else
	{ // check table existence
		static VectorMap<String, SqlAnyTable> table_map;
		static String user;

		if(session.GetUser() != user)
		{ // another user - clear cache
			user = session.GetUser();
			table_map.Clear();
		}

		int ix = table_map.Find(table_name);
		if(ix >= 0)
		{
			*this = table_map[ix];
			return;
		}

		table = ToUpper(table_name);
		Sql cursor(session);
		owner = cursor.Select("OWNER from ALL_TABLES "
			"where OWNER = USER and TABLE_NAME = ?", table);
		if(IsNull(owner))
		{ // table not found - look up synonym table
			if(!cursor.Execute("select TABLE_OWNER, TABLE_NAME from ALL_SYNONYMS "
				"where SYNONYM_NAME = ? "
				"order by DECODE(OWNER, USER, 0, 'PUBLIC', 1, 2)", table))
				throw SqlExc(session);
			if(!cursor.Fetch(owner, table))
				owner = table = String();
		}
		if(!IsNull(owner) && !IsNull(table))
			table_map.FindAdd(table_name, *this);
	}
}

SqlAnyTable::SqlAnyTable(const char* owner, const char* table)
	: owner(owner), table(table)
{
}

String SqlAnyTable::Dot() const
{
	String result;
	if(!IsNull(owner))
		result << owner << '.';
	result += table;
	return result;
}

bool SqlAnyTable::operator == (const SqlAnyTable& t) const
{
	return owner == t.owner && table == t.table;
}

SqlAnyColumn::SqlAnyColumn()
{
}

SqlAnyColumn::SqlAnyColumn(const char* column, const char* table_name, SqlSession& session)
: SqlAnyTable(table_name, session)
, column(ToUpper(column))
{
}

SqlAnyColumn::SqlAnyColumn(const char* column, const char* owner, const char* table)
: SqlAnyTable(owner, table)
, column(ToUpper(column))
{
}

SqlAnyColumn::SqlAnyColumn(const char* column, const SqlAnyTable& table)
: SqlAnyTable(table)
, column(ToUpper(column))
{
}

String SqlAnyColumn::DotColumn() const
{
	return Dot() + '.' + column;
}

bool SqlAnyColumn::operator == (const SqlAnyColumn& c) const
{
	return owner == c.owner && table == c.table && column == c.column;
}

#ifndef NOAPPSQL
GLOBAL_VAR(SqlUserRights, SqlUser)
#endif

void SqlUserRights::Clear()
{
	user = Null;
	roles.Clear();
	rights.Clear();
	error = Null;
}

void SqlUserRights::Sync()
{
	try
	{
		SqlSession& ss = GetSession();
		if(!ss)
			throw SqlExc(t_("User is not connected to the database."));

		String new_user = ss.GetUser();
		if(user == new_user) // user has not changed, rights are up to date
			return;

		Clear();

		user = new_user;
		Sql cursor(ss);
		(Select(SqlId("GRANTED_ROLE")).From(SqlId("USER_ROLE_PRIVS"))
			| Select(SqlId("GRANTED_ROLE")).From(SqlId("ROLE_ROLE_PRIVS"))).Force(cursor);

		String temp;
		while(cursor.Fetch(temp))
			roles.FindAdd(temp);

		// fetch rights
		SqlId OWNER("OWNER");
		SqlId TABLE_NAME("TABLE_NAME");
		SqlId PRIVILEGE("PRIVILEGE");
		Select(SqlId("OWNER, TABLE_NAME, "
			"SUM(distinct DECODE(PRIVILEGE, 'SELECT', 1, 'UPDATE', 2, "
			"'INSERT', 4, 'DELETE', 8, 0))"))
			.From(Select(OWNER, TABLE_NAME, PRIVILEGE).From(SqlId("USER_TAB_PRIVS")) |
			      Select(OWNER, TABLE_NAME, PRIVILEGE).From(SqlId("ROLE_TAB_PRIVS")))
//			"(select * from USER_TAB_PRIVS union select * from ROLE_TAB_PRIVS)")
			.GroupBy(SqlSet(SqlId("OWNER"), SqlId("TABLE_NAME")))
			.Force(cursor);

		SqlAnyTable t;
		int privs;
		while(cursor.Fetch(t.owner, t.table, privs))
			rights.Add(t.Dot(), privs);
	}
	catch(Exc e)
	{
		error = e;
	}
}

int SqlUserRights::GetRightsTo(const char* table)
{
	try
	{
		SqlSession& session = GetSession();
		SqlAnyTable tbl(table, session);
		if(tbl.owner == session.GetUser())
			return 15; // user has always full access to his/her own schema
		Sync();
		int ix = rights.Find(tbl.Dot());
		return (ix >= 0 ? rights[ix] : 0);
	}
	catch(Exc e)
	{
		error = e;
		return 0;
	}
}

bool SqlUserRights::HasRole(const char* role)
{
	Sync();
	return roles.Find(role) >= 0;
}

SqlIndex::SqlIndex()
{
	session = NULL;
}

void SqlIndex::Open(String ind, String own, SqlSession& session)
{
	SqlBool exp = SqlId("INDEX_NAME") == ind;
	if(!IsNull(own))
		exp &= SqlId("OWNER") == own;
	Sql cursor(session);
	Select(SqlId("OWNER"), SqlId("INDEX_NAME"), SqlId("TABLE_OWNER"), SqlId("TABLE_NAME"))
		.From(SqlId("ALL_INDEXES"))
		.Where(exp).Force(cursor);
	if(cursor.Fetch(owner, index, table.owner, table.table))
		FetchContainerExc(columns, Select(SqlId("COLUMN_NAME"))
		.From(SqlId("ALL_IND_COLUMNS"))
		.Where(SqlId("INDEX_OWNER") == owner && SqlId("INDEX_NAME") == index),
		session);
	dropped = false;
}

SqlIndex::SqlIndex(const SqlIndex& another, int)
{
	table     = another.table;
	index     = another.index;
	columns <<= another.columns;
	dropped   = another.dropped;
	session   = another.session;
}

SqlIndex::~SqlIndex()
{
}

String SqlIndex::Dot() const
{
	if(!owner.IsEmpty())
		return owner + '.' + index;
	else
		return index;
}

void SqlIndex::Drop()
{
	ASSERT(session);
	Sql cursor(*session);
	if(!index.IsEmpty() && cursor.Execute("drop index " + Dot()))
		dropped = true;
}

void SqlIndex::Create()
{
	if(index.IsEmpty() || columns.IsEmpty() || !dropped)
		return;

	String s;
	s << "create index " << Dot() << " on " << table.Dot()
		<< '(' << columns[0];
	for(int i = 1; i < columns.GetCount(); i++)
		s << ", " << columns[i];
	s << ')';
	ASSERT(session);
	Sql cursor(*session);
	if(cursor.Execute(s))
		dropped = false;
}

SqlIndexMap::SqlIndexMap()
{
}

SqlIndexMap::~SqlIndexMap()
{
}

void SqlIndexMap::Add(const char* _table, SqlSession& session)
{
	SqlAnyTable table(_table, session);

	Sql cursor(session);
	if(!cursor.Execute("select OWNER, INDEX_NAME from ALL_INDEXES "
		"where UNIQUENESS = 'NONUNIQUE' "
		"and TABLE_OWNER = ? and TABLE_NAME = ?", table.owner, table.table))
		throw SqlExc(session);

	String owner, index;
	while(cursor.Fetch(owner, index))
		AddPick(SqlIndex(index, owner, session));
}

void SqlIndexMap::Drop(Gate2<int, int> progress)
{
	for(int i = 0; i < map.GetCount(); i++)
	{
		map[i].Drop();
		if(progress(i, map.GetCount()))
			throw AbortExc();
	}
}

void SqlIndexMap::Create(Gate2<int, int> progress)
{
	for(int i = 0; i < map.GetCount(); i++)
	{
		map[i].Create();
		if(progress(i, map.GetCount()))
			throw AbortExc();
	}
}

SqlRelocate::SqlRelocate()
{
	session = NULL;
}

SqlRelocate::~SqlRelocate()
{
}

void SqlRelocate::Find(SqlId table, SqlId column, SqlSession& session)
{
	Find(~table.ToString(), ~column.ToString(), session);
}

void SqlRelocate::Find(const char* table, const char* column, SqlSession& sess)
{
	session = &sess;

// todo: CWaitCursor without GUI / MFC?
//	CWaitCursor wait;

	error = Null;
	reference.Clear();
	canceled = false;

	try
	{
		SqlAnyColumn col(column, table, *session);
		static VectorMap<SqlAnyColumn, Vector<SqlAnyColumn> > reference_map;
		int i = reference_map.Find(col);
		if(i >= 0)
		{
			reference <<= reference_map[i];
			return;
		}

		Vector<int> positions;

		Sql cursor(*session);
		if(!cursor.Execute("select distinct POSITION from ALL_CONS_COLUMNS "
			"where OWNER = ? and TABLE_NAME = ? "
			"and COLUMN_NAME = ? and POSITION is not NULL",
			col.owner, col.table, col.column))
			throw SqlExc(*session);

		int temp;
		if(cursor.Fetch(temp))
		{ // positions exist
			do
				positions.Add(temp);
			while(cursor.Fetch(temp));
			for(int pos = 0; pos < positions.GetCount(); pos++)
			{
				if(!cursor.Execute(
					"select OWNER, TABLE_NAME, COLUMN_NAME from ALL_CONS_COLUMNS "
					"where (OWNER, CONSTRAINT_NAME, POSITION) in "
					"(select OWNER, CONSTRAINT_NAME, ? from ALL_CONSTRAINTS "
					"where CONSTRAINT_TYPE = 'R' and (OWNER, R_CONSTRAINT_NAME) in "
					"(select OWNER, CONSTRAINT_NAME from ALL_CONS_COLUMNS "
					"where OWNER = ? and TABLE_NAME = ? "
					"and COLUMN_NAME = ? and POSITION = ?))",
					positions[pos], col.owner, col.table, col.column, positions[pos]))
					throw SqlExc(*session);

				SqlAnyColumn rcol;
				while(cursor.Fetch(rcol.owner, rcol.table, rcol.column))
					reference.Add(rcol);
			}
		}
		reference_map.Add(col, reference);
	}
	catch(Exc e)
	{
		error = e;
	}
	ClearResult();
}

void SqlRelocate::Move(const Vector<int>& old_seq, const Vector<int>& new_seq, int options, Gate2<int, int> progress)
{
	ASSERT(session);

	if(!error.IsEmpty()) // don't do anything when Find() failed
		return;

	ASSERT(old_seq.GetCount() == new_seq.GetCount());
	Vector<SqlSet> old_sub, new_sub, dec_sub;
	int c = 0;
	for(int i = 0; i < old_seq.GetCount(); i++)
		if(!IsNull(old_seq[i]) && new_seq[i] != old_seq[i])
		{
			if(c >= 50 || old_sub.IsEmpty())
			{
				old_sub.Add();
				new_sub.Add();
				dec_sub.Add();
				c = 0;
			}
			c++;
			old_sub.Top().Cat(old_seq[i]);
			new_sub.Top().Cat(new_seq[i]);
			dec_sub.Top().Cat(old_seq[i]).Cat(new_seq[i]);
		}

	if(old_sub.IsEmpty())
		return;

	canceled = false;
	ClearResult();
// todo: CWaitCursor without GUI / MFC?
//	CWaitCursor wait;

	try
	{
		SqlBlock block(*session);
		Sql cursor(*session);
		if(progress(0, GetCount()) && !(options & NO_CANCEL))
		{
			canceled = true;
			throw AbortExc();
		}
		for(int i = 0; i < GetCount(); i++)
		{
			const SqlAnyColumn& column = reference[i];
			SqlId tbl(column.Dot());
			SqlId col(column.column);
			for(int s = 0; s < old_sub.GetCount(); s++)
			{
				if(!IsNull(cursor % Select(1).From(tbl).Where(col == old_sub[s] && SqlId("ROWNUM") == 1)))
				{
					SqlSet dec_set = dec_sub[s];
					dec_set.Cat(col);
					if(!Update(tbl)(col, Decode(col, dec_set))
						.Where(col == old_sub[s]).Execute(cursor))
					{ // update failed
						if(!(options & TRY_ALL))
							throw SqlExc(*session);
						else if(error.IsEmpty())
							error = SqlExc(*session); // just store error & continue trying other tables
					}
					else
						result[i] = true;
				}
				else
					result[i] = true;
			}
//				progress.SetText(
//					Format("Upravuji %s", column.column), false);
			if(progress(i, GetCount()) && !(options & NO_CANCEL))
			{ // set 'canceled' flag & exit
				canceled = true;
				throw AbortExc();
			}
		}
		block.Commit();
	}
	catch(Exc e)
	{
		error = e;
	}
}

void SqlRelocate::Move(int old_seq, int new_seq, int options, Gate2<int, int> progress)
{
	Vector<int> src, dest;
	src << old_seq;
	dest << new_seq;
	Move(src, dest, options, progress);
}

void SqlRelocate::ClearResult()
{
	result.SetCount(reference.GetCount());
	Fill(result.Begin(), result.End(), false);
}

Vector<SqlAnyColumn> SqlRelocate::GetReferences(int seq) const
{
	ASSERT(session);

	Vector<SqlAnyColumn> result;
	if(IsNull(seq))
		return result;

	Sql cursor(*session);

	for(int i = 0; i < GetCount(); i++)
	{
		const SqlAnyColumn& column = reference[i];
		if(seq == (int)cursor.Select(column.column + " from " + column.Dot()
			+ " where " + column.column + " = ? and ROWNUM = 1", seq))
			result.Add(column);
	}

	return result;
}

unsigned GetHashValue(const SqlAnyTable& t)
{
	return GetHashValue(t.owner) ^ GetHashValue(t.table);
}

unsigned GetHashValue(const SqlAnyColumn& t)
{
	return GetHashValue(static_cast<const SqlAnyTable&>(t)) ^ GetHashValue(t.column);
}

END_UPP_NAMESPACE
