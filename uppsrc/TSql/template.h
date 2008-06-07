//////////////////////////////////////////////////////////////////////
// sql/template: SQL templates.

NAMESPACE_UPP

void __sqltempl__instantiation__(Gate2<int, int>); // a weird compiler bug

template <class C>
C& FetchContainer(C& container, Sql& cursor, Gate2<int, int> progress = false)
{
	int done = 0;
	typedef typename C::ValueType VT;
	VT T;
	while(cursor.Fetch(T))
	{
		container.Add(T);
		if(progress(++done, 0))
			throw AbortExc();
	}
	return container;
}

template <class C>
C& FetchContainerExc(C& container, const SqlSelect& set, SqlSession& session APPSQLSESSION, Gate2<int, int> progress = false)
{
	Sql cursor(session);
	set.Force(cursor);
	return FetchContainer(container, cursor, progress);
}

template <class C>
C& FetchContainer(C& container, const SqlSelect& set, SqlSession& session, Gate2<int, int> progress = false)
{
	try
	{
		FetchContainerExc(container, set, session, progress);
	}
	catch(Exc e)
	{
//		ShowExc(e);
	}
	return container;
}

#ifndef NOAPPSQL
template <class C>
C& FetchContainer(C& container, const SqlSelect& set, Gate2<int, int> progress = false)
{
	return FetchContainer(container, set, SQL.GetSession(), progress);
}
#endif//NOAPPSQL

template <class I>
SqlSet SetOf(I begin, I end)
{
	SqlSet set;
	while(begin != end)
	{
		set |= SqlVal(*begin);
		++begin;
	}
	return set;
}

template <class I>
SqlSet ColumnSetOf(I begin, I end)
{
	SqlSet set;
	while(begin != end)
	{
		set |= SqlCol(*begin);
		++begin;
	}
	return set;
}

template <class C>
inline SqlSet SetOf(const C& container)
{
	return SetOf(container.Begin(), container.End());
}

template <class C>
inline SqlSet ColumnSetOf(const C& container)
{
	return ColumnSetOf(container.Begin(), container.End());
}

extern const char *txtFnTSqlTemplateFetchSeqKeyNull();
extern const char *txtFnTSqlTemplateFetchSeqNotFound();

template <class T>
T& FetchSeq(T& data, SqlId key, Value value, SqlId table, Sql& cursor APPSQLCURSOR)
{
	if(IsNull(value))
		throw SqlExc(NFormat(s_(FnTSqlTemplateFetchSeqKeyNull()), ~table, ~key));
	Select(data).From(table).Where(key == value).Force(cursor);
	if(!cursor.Fetch(data))
		throw SqlExc(NFormat(s_(FnTSqlTemplateFetchSeqNotFound()), StdFormat(value), ~table, ~key));
	return data;
}

template <class T>
inline T& FetchSeq(T& data, SqlId key, Value value, Sql& cursor APPSQLCURSOR)
{
	return FetchSeq<T>(data, key, value, SqlId(T::TableName), cursor);
}

template <class T>
inline T FetchSeq(SqlId key, Value value, SqlId table, Sql& cursor APPSQLCURSOR, T * = 0)
{
	T temp;
	return FetchSeq(temp, key, value, table, cursor);
}

template <class T>
inline T FetchSeq(SqlId key, Value value, Sql& cursor APPSQLCURSOR, T * = 0)
{
	return FetchSeq<T>(key, value, SqlId(T::TableName), cursor);
}

template <class T>
T& FetchSchemaSeq(T& data, SqlId key, Value value, SqlId table, Sql& cursor APPSQLCURSOR)
{
	if(IsNull(value))
		throw SqlExc(NFormat(s_(FnTSqlTemplateFetchSeqKeyNull()), ~table, ~key));
	Select(data).FromSchema(table).Where(key == value).Force(cursor);
	if(!cursor.Fetch(data))
		throw SqlExc(NFormat(s_(FnTSqlTemplateFetchSeqNotFound()), StdFormat(value), ~table));
	return data;
}

template <class T>
inline T& FetchSchemaSeq(T& data, SqlId key, Value value, Sql& cursor APPSQLCURSOR)
{
	return FetchSchemaSeq<T>(data, key, value, SqlId(T::TableName), cursor);
}

template <class T>
inline T FetchSchemaSeq(SqlId key, Value value, SqlId table, Sql& cursor APPSQLCURSOR, T * = 0)
{
	T temp;
	return FetchSchemaSeq(temp, key, value, table, cursor);
}

template <class T>
inline T FetchSchemaSeq(SqlId key, Value value, Sql& cursor APPSQLCURSOR, T * = 0)
{
	return FetchSeq<T>(key, value, SqlId(T::TableName), cursor);
}

template <class T>
SqlSelect SelectWhere(SqlBool cond = true, T * = 0)
{
	return Select(T::ColumnSet()).From(SqlCol(T::TableName)).Where(cond);
}

template <class T>
SqlSelect SelectSchemaWhere(SqlBool cond = true, T * = 0)
{
	return Select(T::ColumnSet()).FromSchema(SqlCol(T::TableName)).Where(cond);
}

END_UPP_NAMESPACE
