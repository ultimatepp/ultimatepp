NAMESPACE_UPP

void __sqltempl__instantiation__(Gate2<int, int>); // a weird compiler bug

#define DEFID(x)    const Id x(#x);
#define _DEFID(x)   const Id _##x(#x);
#define _SQLID(x)   const SqlId _##x(#x);

const SqlId& ALL(); // <*>
const SqlId& QUE(); // <?>

class SqlBlock
{
public:
#ifndef NOAPPSQL
	SqlBlock(SqlSession& session = SQL.GetSession())
		: session(session), done(false)      { session.ClearError(); session.Begin(); }
#else
	SqlBlock(SqlSession& session)
		: session(session), done(false)      { session.ClearError(); session.Begin(); }
#endif
	~SqlBlock()                              { if(!done) session.Rollback(); }

	void        Commit()                     { ASSERT(!done); done = true; session.Commit(); }
	void        Rollback()                   { ASSERT(!done); done = true; session.Rollback(); }
	void        Flush()                      { ASSERT(!done); session.Commit(); session.Begin(); }

private:
	SqlSession& session;
	bool        done;
};

void                  SetSchema(const String& schema);
String                GetSchema();
String                SchemaTableName(const String& table);
SqlId                 SchemaTable(const SqlId& table);

String                GetInsertString(Fields nf, bool optimize = true);
String                GetSchemaInsertString(Fields nf, bool optimize = true);
void                  InsertScript(SqlSchema& schema, Fields nf, bool optimize = true);
void                  InsertSchemaScript(SqlSchema& schema, Fields nf, bool optimize = true);

SqlSelect             SelectTable(Fields nf);
SqlSelect             SelectSchemaTable(Fields nf);
SqlSelect             SelectColumns(Fields nf);

Value                 GetValue(Fields nf, SqlId field);
Ref                   GetRef(Fields nf, SqlId field);
String                GetTableName(Fields nf);

VectorMap<Id, Ref>    GetRefMap(Fields nf, String *tablename = NULL);
VectorMap<Id, Value>  GetValueMap(Fields nf, String *tablename = NULL);

String                ForceInsertRowid(const SqlInsert& insert, Sql& cursor APPSQLCURSOR);
String                ForceInsertRowid(Fields nf, Sql& cursor APPSQLCURSOR);
String                ForceInsertRowid(SqlId table, Fields nf, Sql& cursor APPSQLCURSOR);
String                ForceSchemaInsertRowid(Fields nf, Sql& cursor APPSQLCURSOR);
String                ForceSchemaInsertRowid(SqlId table, Fields nf, Sql& cursor APPSQLCURSOR);

void                  ForceInsert(Fields nf, Sql& cursor APPSQLCURSOR);
void                  ForceInsert(SqlId table, Fields nf, Sql& cursor APPSQLCURSOR);
void                  ForceUpdate(Fields nf, SqlId key, const Value& keyval, Sql& cursor APPSQLCURSOR);
void                  ForceUpdate(SqlId table, Fields nf, SqlId key, const Value& keyval, Sql& cursor APPSQLCURSOR);
void                  ForceUpdate(Fields nf, Fields of, SqlId key, const Value& keyval, Sql& cursor APPSQLCURSOR);
void                  ForceUpdate(SqlId table, Fields nf, Fields of, SqlId key, const Value& keyval, Sql& cursor APPSQLCURSOR);
void                  ForceDelete(SqlId table, SqlId key, const Value& keyval, Sql& cursor APPSQLCURSOR);
void                  ForceExecute(const String& s, Sql& cursor APPSQLCURSOR);
void                  ForceSchemaInsert(Fields nf, Sql& cursor APPSQLCURSOR);
void                  ForceSchemaInsert(SqlId table, Fields nf, Sql& cursor APPSQLCURSOR);
void                  ForceSchemaUpdate(Fields nf, SqlId key, const Value& keyval, Sql& cursor APPSQLCURSOR);
void                  ForceSchemaUpdate(SqlId table, Fields nf, SqlId key, const Value& keyval, Sql& cursor APPSQLCURSOR);
void                  ForceSchemaUpdate(Fields nf, Fields of, SqlId key, const Value& keyval, Sql& cursor APPSQLCURSOR);
void                  ForceSchemaUpdate(SqlId table, Fields nf, Fields of, SqlId key, const Value& keyval, Sql& cursor APPSQLCURSOR);
void                  ForceSchemaDelete(SqlId table, SqlId key, const Value& keyval, Sql& cursor APPSQLCURSOR);

bool                  IsNotEmpty(const SqlSelect& select, Sql& cursor APPSQLCURSOR);
bool                  IsNotEmpty(const SqlVal& table, const SqlBool& cond, Sql& cursor APPSQLCURSOR);
bool                  IsNotEmpty(const SqlVal& table, SqlId column, const Value& value, Sql& cursor APPSQLCURSOR);
bool                  IsNotSchemaEmpty(const SqlVal& table, const SqlBool& cond, Sql& cursor APPSQLCURSOR);
bool                  IsNotSchemaEmpty(const SqlVal& table, SqlId column, const Value& value, Sql& cursor APPSQLCURSOR);

#ifdef PLATFORM_WIN32 // bw, move to tsql/ctrl
//bool                  LockSql(Sql& cursor APPSQLCURSOR);
//bool                  LockSql(const SqlVal& table, SqlBool where, Sql& cursor APPSQLCURSOR);
//bool                  LockSql(const SqlVal& table, SqlId column, const Value& value, Sql& cursor APPSQLCURSOR);
//bool                  LockSchemaSql(const SqlVal& table, SqlBool where, Sql& cursor APPSQLCURSOR);
//bool                  LockSchemaSql(const SqlVal& table, SqlId column, const Value& value, Sql& cursor APPSQLCURSOR);

//Mirek 06/12/22 - not defined anywhere in uppsrc, clashes with idis

#endif

SqlVal                Alias(const SqlId& value, const SqlId& alias);
SqlVal                SchemaAlias(const SqlId& table, const SqlId& alias);
SqlVal                SchemaAlias(const SqlId& table);
SqlId                 SchemaId(const SqlId& table, const SqlId& alias);
SqlId                 SchemaId(const SqlId& table);

inline SqlInsert      InsertSchema(SqlId table) { return SqlInsert(SqlId(SchemaTableName(~table))); }
inline SqlUpdate      UpdateSchema(SqlId table) { return SqlUpdate(SqlId(SchemaTableName(~table))); }
inline SqlDelete      DeleteSchema(SqlId table) { return Delete(SqlId(SchemaTableName(~table))); }
/* use .Hint instead...
SqlSelect             SelectHint(const char *hint, SqlSet set);

#define E__SelectHint(I)\
SqlSelect             SelectHint(const char *hint, __List##I(E__SqlVal));
__Expand(E__SelectHint);
*/
SqlSet                DeleteHint(const char *hint, const SqlVal& table);
SqlSet                DeleteSchemaHint(const char *hint, const SqlId& table);

VectorMap<String, SqlColumnInfo> Describe(const SqlVal& table, Sql& cursor APPSQLCURSOR);
VectorMap<String, SqlColumnInfo> DescribeSchema(const SqlId& table, Sql& cursor APPSQLCURSOR);

int64                 SelectCount(const SqlVal& table, const SqlBool& cond, Sql& cursor);
int64                 SelectSchemaCount(const SqlId& table, const SqlBool& cond, Sql& cursor);

#ifndef NOAPPSQL
inline int64          SelectCount(const SqlVal& table, const SqlBool& cond = SqlBool::True())       { return SelectCount(table, cond, SQL); }
inline int64          SelectSchemaCount(const SqlId& table, const SqlBool& cond = SqlBool::True()) { return SelectSchemaCount(table, cond, SQL); }
#endif

Vector<String>        LoadStrColumn(SqlId column, const SqlVal& table, SqlSession& session APPSQLSESSION, bool order = false);
Vector<String>&       SyncStrColumn(Vector<String>& dest, SqlId col, const SqlVal& table, SqlSession& session APPSQLSESSION, bool order = false);
Vector<String>        LoadSchemaStrColumn(SqlId column, const SqlId& table, SqlSession& session APPSQLSESSION, bool order = false);
Vector<String>&       SyncSchemaStrColumn(Vector<String>& dest, SqlId col, const SqlId& table, SqlSession& session APPSQLSESSION);

SqlVal                GetCsVal(const SqlVal& val); // replaces CH for sorts
SqlVal                GetCs(const char *col);
SqlVal                GetCsAsciiVal(const SqlVal& val); // replaces CH and removes accents
SqlVal                GetCsAscii(const char *col);
SqlBool               LikeSmartWild(const SqlVal& exp, const String& s);

SqlBool               GetTextRange(const String& s1, const String& s2, const SqlVal& exp);
SqlBool               GetNumRange(double min, double max, const SqlVal& exp);
SqlBool               GetDateRange(Date d1, Date d2, const SqlVal& exp, bool force_range = false);
SqlBool               GetTimeRange(Time d1, Time d2, const SqlVal& exp, bool force_range = false);

String                GetYearDayIndex(Date date);
SqlVal                GetYearDayIndex(const SqlVal& date);
SqlBool               GetYearDayRange(const SqlVal& date, Date min, Date max);

END_UPP_NAMESPACE
