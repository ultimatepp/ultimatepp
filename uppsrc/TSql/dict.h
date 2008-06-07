//////////////////////////////////////////////////////////////////////
// SqlDict: DICTIONARY - based functions.
// Utilities:            AnyTable, AnyColumn
// User rights:          UserGetRightsTo, UserCanRead, UserCanWrite
//                       UserRights
// Key relocation:       Relocate
// Index autodrop:       Index, IndexMap
// usage:
// if(UserCanRead[Write]("ADRESA"[ADRESA]))
//    ...

NAMESPACE_UPP

enum // rights
{
	SEL = 0x01,
	UPD = 0x02,
	INS = 0x04,
	DEL = 0x08,
};

#ifndef NOAPPSQL
int         SqlUserGetRightsTo(const char* table);
bool        SqlUserCanWrite(const char* table);
bool        SqlUserCanRead(const char* table);

inline int  SqlUserGetRightsTo(const String& s) { return SqlUserGetRightsTo(~s); }
inline bool SqlUserCanWrite(const String& s)    { return SqlUserCanWrite(~s); }
inline bool SqlUserCanRead(const String& s)     { return SqlUserCanRead(~s); }

inline int  SqlUserGetRightsTo(Id id)           { return SqlUserGetRightsTo(~id); }
inline bool SqlUserCanWrite(Id id)              { return SqlUserCanWrite(~id); }
inline bool SqlUserCanRead(Id id)               { return SqlUserCanRead(~id); }
#endif

//////////////////////////////////////////////////////////////////////

struct SqlAnyTable : Moveable<SqlAnyTable>
{
	String       owner;
	String       table;

	SqlAnyTable();
	SqlAnyTable(const char* owner, const char* table);
#ifndef NOAPPSQL
	SqlAnyTable(const char* table, SqlSession& session = SQL.GetSession());
#else
	SqlAnyTable(const char* table, SqlSession& session);
#endif

	bool operator == (const SqlAnyTable& t) const;
	bool operator != (const SqlAnyTable& t) const { return !(*this == t); }

	String       Dot() const;
};

//////////////////////////////////////////////////////////////////////

struct SqlAnyColumn : public SqlAnyTable, Moveable<SqlAnyColumn>
{
	String column;

	SqlAnyColumn();
#ifndef NOAPPSQL
	SqlAnyColumn(const char* column, const char* table_name, SqlSession& session = SQL.GetSession());
#else
	SqlAnyColumn(const char* column, const char* table_name, SqlSession& session);
#endif
	SqlAnyColumn(const char* column, const char* owner, const char* table);
	SqlAnyColumn(const char* column, const SqlAnyTable& table);

	bool operator == (const SqlAnyColumn& c) const;
	bool operator != (const SqlAnyColumn& c) const { return !(*this == c); }

	String DotColumn() const;

};

//////////////////////////////////////////////////////////////////////

class SqlUserRights
{
public:
	SqlUserRights(SqlSession& session) : session(&session) {}
#ifndef NOAPPSQL
	SqlUserRights() : session(NULL) {}
#endif

	// it is valid to call members even when SQL is not connected,
	// in which case there are no rights

#ifndef NOAPPSQL
	SqlSession&             GetSession() const { return session ? *session : SQL.GetSession(); }
#else
	SqlSession&             GetSession() const { ASSERT(session); return *session; }
#endif

	void                    Clear(); // clear everything (forces Sync() on next call)

	void                    Sync();

	bool                    HasRole(const char* role);

	int                     GetRightsTo(const char* table);
	bool                    CanRead(const char* table)  { return GetRightsTo(table) & SEL; }
	bool                    CanWrite(const char* table) { return (~GetRightsTo(table) & (SEL | UPD | INS | DEL)) == 0; }

public:
	SqlSession             *session;
	String                  user;
	Index<String>           roles;
	VectorMap<String, char> rights; // table names
	String                  error; // last error message
};

//////////////////////////////////////////////////////////////////////

#ifndef NOAPPSQL
extern SqlUserRights& SqlUser();
#endif

//////////////////////////////////////////////////////////////////////
// Relocate::

class SqlRelocate
{
public:
	enum OPTIONS
	{
		TRY_ALL   = 0x01, // try all tables even on errors
		NO_CANCEL = 0x02, // don't allow cancelling the operation (only with PROGRESS)
	};

	SqlRelocate();
#ifndef NOAPPSQL
	SqlRelocate(const char *table, const char *column)                      { Find(table, column, SQL.GetSession()); }
	SqlRelocate(SqlId table, SqlId column)                                  { Find(table, column, SQL.GetSession()); }
#endif
	SqlRelocate(const char *table, const char *column, SqlSession& session) { Find(table, column, session); }
	SqlRelocate(SqlId table, SqlId column, SqlSession& session)             { Find(table, column, session); }
	~SqlRelocate();

	void                 Find(const char *table, const char *column, SqlSession& session);
	void                 Find(SqlId table, SqlId column, SqlSession& session);

	void                 Move(int old_seq, int new_seq, int options = 0, Gate2<int, int> progress = false);
	void                 Move(const Vector<int>& old_seq, const Vector<int>& new_seq, int options = 0, Gate2<int, int> progress = false);

	Vector<SqlAnyColumn> GetReferences(int seq) const; // columns referencing this seq

	bool                 IsEmpty() const     { return reference.IsEmpty(); }
	int                  GetCount() const    { return reference.GetCount(); }

	bool                 IsCanceled() const { return canceled; }
	bool                 IsError() const     { return !IsNull(error); }

	const String&        GetError() const    { return error; }

public:
	Vector<SqlAnyColumn> reference;
	Vector<bool>         result; // true = update succeeded
	String               error; // last error message; Null when Find() / Go() succeeded
	bool                 canceled;
	SqlSession          *session;

protected:
	void                 ClearResult();
};

//////////////////////////////////////////////////////////////////////
// SqlIndex::

struct SqlIndex : DeepCopyOption<SqlIndex>
{
	String         owner;
	String         index;
	SqlAnyTable    table;
	Vector<String> columns;
	bool           dropped;
	SqlSession    *session;

	SqlIndex();
	SqlIndex(String index, String owner, SqlSession& session) { Open(index, owner, session); }
#ifndef NOAPPSQL
	SqlIndex(String index, String owner)                      { Open(index, owner, SQL.GetSession()); }
#endif
	SqlIndex(const SqlIndex& another, int);
	~SqlIndex();

	void   Open(String index, String owner, SqlSession& session);

	String Dot() const;

	void   Drop();
	void   Create();
};

//////////////////////////////////////////////////////////////////////
// SqlIndexMap::

class SqlIndexMap
{
public:
	SqlIndexMap();
	~SqlIndexMap();

	void AddPick(pick_ SqlIndex& index) { map.Add() = index; }
	void Add(const SqlIndex& index) { map.Add(index); }
	void Add(const char* table, SqlSession& session);
#ifndef NOAPPSQL
	void Add(const char* table) { Add(table, SQL.GetSession()); }
#endif

	void Drop(Gate2<int, int> progress = false);
	void Create(Gate2<int, int> progress = false);

public:
	Array<SqlIndex> map;
};

#ifndef NOAPPSQL
inline SqlIndexMap& operator << (SqlIndexMap& map, const char* table) { map.Add(table); return map; }
#endif

//////////////////////////////////////////////////////////////////////

unsigned GetHashValue(const SqlAnyTable& table);
unsigned GetHashValue(const SqlAnyColumn& column);

END_UPP_NAMESPACE
