bool SqlToBool(const String& s);
bool SqlToBool(const Value& v);
const String& BoolToSql(bool b);

class SqlSession;

class SqlExc : public Exc {
public:
#ifndef NOAPPSQL
	SqlExc();
#endif
	SqlExc(const SqlSession& session);
	SqlExc(const Sql& sql);
	SqlExc(const String& desc) : Exc(desc) {}
	SqlExc(const char *desc) : Exc(desc) {}

	void SetSessionError(const SqlSession& session);
};

enum { SQLRAW_V = 34 };

class SqlRaw : public String, public ValueType<SqlRaw, SQLRAW_V> {
public:
	operator Value() const              { return RawValue<SqlRaw>(*this); }
	SqlRaw(const Value& q)
		: String(IsNull(q) ? String() :
		         IsString(q) ? String(q) :
		         String(RawValue<SqlRaw>::Extract(q))) {}
	SqlRaw(const String& s) : String(s) {}
	SqlRaw() {}
};

struct SqlColumnInfo : Moveable<SqlColumnInfo> {
	String      name;
	int         type;
	int         width;
	int         precision; //number of total digits in numeric types
	int         scale;     //number of digits after comma in numeric types
	bool        nullable;  //true - column can hold null values
	bool        binary;    //column holds binary data
};

class SqlConnection {
protected:
	friend class Sql;
	friend class SqlSession;

	virtual void        SetParam(int i, const Value& r) = 0;
	virtual bool        Execute() = 0;
	virtual int         GetRowsProcessed() const;
	virtual Value       GetInsertedId() const;
	virtual bool        Fetch() = 0;
	virtual void        GetColumn(int i, Ref r) const = 0;
	virtual void        Cancel() = 0;
	virtual SqlSession& GetSession() const = 0;
	virtual String      GetUser() const;
	virtual String      ToString() const = 0;

	static void Attach(Sql& sql, SqlConnection *con);

	SqlConnection();
	virtual            ~SqlConnection();

	int                    starttime;
	String                 statement;
	Vector<SqlColumnInfo>  info;
	int                    fetchrows;
	int                    longsize;
	bool                   parse;
};

#define E__ColVal(I)  const char *c##I, const Value& v##I
#define E__IdVal(I)   SqlId c##I, const Value& v##I

class SqlSource {
protected:
	virtual SqlConnection *CreateConnection() = 0;
	virtual ~SqlSource() {}
	friend class Sql;
	SqlSource() {}

private:
	void operator=(const SqlSource&);
	SqlSource(const SqlSource&);
};

class Sql {
	SqlConnection  *cn;
	Vector<Value>   param;

	friend class SqlSession;
	friend class SqlConnection;
	friend Sql& AppCursor();
	friend Sql& AppCursorR();

	Value       Select0(const String& what);

	void   SetSession(SqlSource& src);
	void   Attach(SqlConnection *connection);
	void   Detach();

protected:
	Sql(SqlConnection *connection);

public:
	String Compile(const SqlStatement& s);

	void   Clear();

	void   SetParam(int i, const Value& val);
	void   SetStatement(const String& s);
	void   SetStatement(const SqlStatement& s)         { SetStatement(Compile(s)); }

	bool   Execute();
	void   ExecuteX(); // Deprecated
	bool   Run()                                       { return Execute(); }
	void   RunX()                                      { ExecuteX(); } // Deprecated

	bool   Execute(const String& s);
	void   ExecuteX(const String& s); // Deprecated

	bool   Execute(const SqlStatement& s)              { return Execute(Compile(s)); }
	void   ExecuteX(const SqlStatement& s)             { ExecuteX(Compile(s)); }  // Deprecated


//$-
#define  E__Run(I)       bool Run(__List##I(E__Value));
	__Expand(E__Run)
//$+ bool Run(const Value& v1 [, const Value& v2 ...]);

//$-
#define  E__RunX(I)      void RunX(__List##I(E__Value)); // Deprecated
	__Expand(E__RunX)
//$+

//$-
#define  E__Execute(I)   bool Execute(const String& s, __List##I(E__Value));
	__Expand(E__Execute)
//$+ bool Execute(const String& s, const Value& v1 [, const Value& v2 ...]);

//$-
#define  E__ExecuteX(I)  void ExecuteX(const String& s, __List##I(E__Value)); // Deprecated
	__Expand(E__ExecuteX)
//$+


	bool   Fetch();

//$-
#define  E__Fetch(I)    bool Fetch(__List##I(E__Ref));
	__Expand(E__Fetch)
//$+ bool Fetch(Ref v1 [, Ref v2 ...]);

	bool   Fetch(Vector<Value>& row);
	bool   Fetch(ValueMap& row);
	bool   Fetch(Fields fields);

	int    GetRowsProcessed() const                    { return cn->GetRowsProcessed(); }

	int    GetColumns() const;
	int    GetColumnCount() const;

	void                 GetColumn(int i, Ref r) const;
	void                 GetColumn(SqlId colid, Ref r) const;
	Value                operator[](int i) const;
	Value                operator[](SqlId colid) const;
	const SqlColumnInfo& GetColumnInfo(int i) const    { return cn->info[i]; }
	Vector<Value>        GetRow() const;
	operator             Vector<Value>() const         { return GetRow(); }
	ValueMap             GetRowMap() const;
	ValueMap             operator~() const             { return GetRowMap(); }
	void                 Get(Fields fields);

	void        SetFetchRows(int nrows)                { cn->fetchrows = nrows; } // deprecated
	void        SetLongSize(int lsz)                   { cn->longsize = lsz; } // deprecated

	void        Cancel()                               { if(cn) cn->Cancel(); }

	Value       Select(const String& what); // Deprecated

//$-
#define  E__Select(I)   Value Select(const String& what, __List##I(E__Value)); // Deprecated
	__Expand(E__Select)

#define  E__Insert(I)  bool Insert(const char *tb, const char *c0, const Value& v0, __List##I(E__ColVal)); // Deprecated
	__Expand(E__Insert)

#define  E__InsertId(I)  bool Insert(SqlId tb, SqlId c0, const Value& v0, __List##I(E__IdVal)); // Deprecated
	__Expand(E__InsertId)

#define  E__Update(I)  bool Update(const char *tb, const char *k, const Value& kv, __List##I(E__ColVal)); // Deprecated
	__Expand(E__Update)

#define  E__UpdateId(I)  bool Update(SqlId tb, SqlId k, const Value& kv, __List##I(E__IdVal)); // Deprecated
	__Expand(E__UpdateId)
//$+

	bool        Insert(Fields nf);
	bool        Insert(Fields nf, const char *table);
	bool        Insert(Fields nf, SqlId table);

	bool        InsertNoKey(Fields nf, const char *table);
	bool        InsertNoKey(Fields nf);
	bool        InsertNoKey(Fields nf, SqlId table);

	bool        InsertNoNulls(Fields nf, const char *table);
	bool        InsertNoNulls(Fields nf);
	bool        InsertNoNulls(Fields nf, SqlId table);

	bool        Update(Fields nf);
	bool        Update(Fields nf, const char *table);
	bool        Update(Fields nf, SqlId table);

	bool        Delete(const char *table, const char *key, const Value& keyval);
	bool        Delete(SqlId table, SqlId key, const Value& keyval);

	String      ToString() const                       { return cn->ToString(); }

	bool       operator*(const SqlStatement& q)        { return Execute(q); }
	Sql&       operator&(const SqlStatement& q)        { ExecuteX(q); return *this; } // Deprecated
	Value      operator%(const SqlStatement& q);
	ValueMap   operator^(const SqlStatement& q);
	ValueArray operator/(const SqlStatement& q);

	SqlSession& GetSession() const                     { return cn->GetSession(); }
	int    GetDialect() const;

	Value  GetInsertedId() const                       { return cn->GetInsertedId(); }

	String GetUser() const                             { return cn->GetUser(); } // Deprecated

	enum ERRORCLASS {
		ERROR_UNSPECIFIED,
		CONNECTION_BROKEN,
	};

	// following block deprecated, use SqlSession for error handling
	void   SetError(String error, String stmt, int code = 0, const char *scode = NULL, ERRORCLASS clss = ERROR_UNSPECIFIED);
	String GetLastError() const;
	String GetErrorStatement() const;
	int    GetErrorCode() const;
	String GetErrorCodeString() const;
	ERRORCLASS GetErrorClass() const;
	void   ClearError();

	void   Begin(); // deprecated: use SqlSession::Begin instead
	void   Commit(); // deprecated: use SqlSession::Commit instead
	void   Rollback(); // deprecated: use SqlSession::Rollback instead
	int    GetTransactionLevel(); // deprecated: only single level of transactions generally supported

	String Savepoint(); // deprecated
	void   RollbackTo(const String& savepoint); // deprecated

	bool   IsOpen();

	bool   WasError() const; // deprecated, use SqlSession::WasError

	Sql(SqlSource& src);
#ifndef NOAPPSQL
	Sql();
	Sql(const char *stmt);
	Sql(const SqlStatement& s);
#endif
	Sql(const char *stmt, SqlSource& session);
	Sql(const SqlStatement& s, SqlSource& session);
	~Sql();
	
	void operator=(SqlSession& s); // this only works with SQL and SQLR...
	static void PerThread(bool b = true); // Activates thread local SQL/SQLR

private:
	void operator=(const Sql&);
	Sql(const Sql&);
};

struct Sql0 : Sql {
	Sql0() : Sql((SqlConnection *)NULL) {}
};

#ifndef NOAPPSQL
struct SqlR : Sql {
	SqlR();
	SqlR(const char *stmt);
	SqlR(const SqlStatement& s);
};
#endif

struct StatementExecutor { // Deprecated, use SqlPerformScript
	virtual bool Execute(const String& stmt) = 0;
	virtual ~StatementExecutor() {}
};

 // Deprecated, use SqlPerformScript
typedef bool (*RunScript)(const String& text, StatementExecutor& executor, Gate<int, int> progress_canceled);

class AppSql;
class AppSqlR;

class SqlSession : public SqlSource {
public:
	enum {
		START_FETCHING,
		END_FETCHING,
		END_FETCHING_MANY,
		START_EXECUTING,
		END_EXECUTING,
		EXECUTING_ERROR,
		CONNECTION_ERROR,
		BEFORE_EXECUTING,
		AFTER_EXECUTING
	};

protected:
	virtual SqlConnection        *CreateConnection();

	friend class Sql;

	Stream                       *trace, *error_log;
	bool                          tracetime;
	bool                          trace_compression;
	int                           traceslow;
	int                           dialect;
	int                           exectime;

	String                        statement;

	String                        lasterror;
	String                        errorstatement;
	int                           errorcode_number;
	String                        errorcode_string;
	Sql::ERRORCLASS               errorclass;
	bool                        (*error_handler)(String error, String stmt, int code, const char *scode, Sql::ERRORCLASS clss);
	bool                          throwonerror;

	int                           status;
	
	bool                          use_realcase = false;
	
	One<Sql>                      sql;
	One<Sql>                      sqlr;
	
	void                          SessionClose();

	static void Attach(Sql& sql, SqlConnection *con);

protected:
	SqlSession&                   Dialect(int q)                          { dialect = q; return *this; }

public:
	virtual void                  Begin();
	virtual void                  Commit();
	virtual void                  Rollback();
	virtual int                   GetTransactionLevel() const;

	virtual String                Savepoint(); // Deprecated
	virtual void                  RollbackTo(const String& savepoint); // Deprecated

	virtual bool                  IsOpen() const;

	virtual RunScript             GetRunScript() const; // Deprecated

	virtual Vector<String>        EnumUsers();
	virtual Vector<String>        EnumDatabases();
	virtual Vector<String>        EnumTables(String database);
	virtual Vector<String>        EnumViews(String database);
	virtual Vector<String>        EnumSequences(String database);
	virtual Vector<SqlColumnInfo> EnumColumns(String database, String table);
	virtual Vector<String>        EnumPrimaryKey(String database, String table);
	virtual String                EnumRowID(String database, String table); // deprecated
	virtual Vector<String>        EnumReservedWords(); // deprecated

	int                           GetDialect() const                      { ASSERT(dialect != 255); return dialect; }

	void                          SetTrace(Stream& s = VppLog())          { trace = &s; }
	Stream                       *GetTrace() const                        { return trace; }
	void                          KillTrace()                             { trace = NULL; }
	bool                          IsTraceCompression() const              { return trace_compression; }
	void                          SetTraceCompression(bool b)             { trace_compression = b; }

	void                          LogErrors(Stream& s = VppLog())         { error_log = &s; }
	void                          LogErrors(bool b)                       { error_log = b ? &VppLog() : NULL; }

	void                          TraceTime(bool b = true)                { tracetime = b; }
	bool                          IsTraceTime() const                     { return tracetime; }

	SqlSession&                   TraceSlow(int ms = 5000)                { traceslow = ms; return *this; }
	
	SqlSession&                   ThrowOnError(bool b = true)             { throwonerror = b; return *this; }
	bool                          IsThrowOnError() const                  { return throwonerror; }

	bool                          WasError() const                        { return !GetLastError().IsEmpty(); }

	void                          SetError(String error, String stmt, int code = 0, const char * scode = NULL, Sql::ERRORCLASS clss = Sql::ERROR_UNSPECIFIED);
	String                        GetLastError() const                    { return lasterror; }
	String                        GetErrorStatement() const               { return errorstatement; }
	int                           GetErrorCode() const                    { return errorcode_number; }
	String                        GetErrorCodeString() const              { return errorcode_string; }
	Sql::ERRORCLASS               GetErrorClass() const                   { return errorclass; }
	void                          ClearError();
	void                          InstallErrorHandler(bool (*handler)(String error, String stmt, int code, const char *scode, Sql::ERRORCLASS clss));

	String                        GetStatement() const                    { return statement; } // deprecated
	void                          SetStatement(const String& s)           { statement = s; } // deprecated

	void                          SetTime(int t)                          { exectime = t; } // deprecated
	int                           GetTime() const                         { return exectime; } // deprecated

	String                        GetUser()                               { return Sql(*this).GetUser(); } // deprecated
	
	Sql&                          GetSessionSql(); // "private" - only to make SQL work
	Sql&                          GetSessionSqlR(); // "private" - only to make SQLR work

	operator                      bool() const                            { return IsOpen(); }

	int                           GetStatus()                             { return status; }
	void                          SetStatus(int s)                        { status = s; WhenDatabaseActivity(*this); }
	bool                          operator == (int s) const               { return status == s; }
	bool                          operator != (int s) const               { return status != s; }

	void                          UseRealcase()                           { use_realcase = true; }
	bool                          IsUseRealcase() const                   { return use_realcase; }

	Callback1<const SqlSession&>  WhenDatabaseActivity;

	static void PerThread(bool b = true); // Activates thread local SQL/SQLR

	SqlSession();
	virtual ~SqlSession();
};


#ifndef NOAPPSQL

Sql& AppCursor();
Sql& AppCursorR();

//$-
#define SQL  AppCursor()
#define SQLR AppCursorR()
//$+
//  Assist++ cheat:
//$ Sql SQL;

#endif

class OciConnection;

bool SqlPerformScript(SqlSession& session, Stream& script,
                      Gate<int, int> progress_canceled = Null, bool stoponerror = false);
bool SqlPerformScript(Stream& script,
                      Gate<int, int> progress_canceled = Null, bool stoponerror = false);
bool SqlPerformScript(SqlSession& session, const String& script,
                      Gate<int, int> progress_canceled = Null, bool stoponerror = false);
bool SqlPerformScript(const String& script,
                      Gate<int, int> progress_canceled = Null, bool stoponerror = false);

class SqlMassInsert {
	struct Row : Moveable<Row> {
		uint64         nulls;
		Vector <Value> value;
		SqlBool        remove;
		
		rval_default(Row);
		Row() {}
	};

	Sql&            sql;
	SqlId           table;
	Vector<String>  column;
	Vector<Row>     cache;
	int             pos;
	bool            error;
	bool            use_transaction;
	
	void            NewRow();

public:
	SqlMassInsert& operator()(SqlId col, const Value& val);
	SqlMassInsert& operator()(const ValueMap& data);
	SqlMassInsert& EndRow(SqlBool remove = SqlBool());
	void           Flush();
	bool           IsError() const                                 { return error; }
	SqlMassInsert& UseTransaction(bool b = true)                   { use_transaction = b; return *this; }
	SqlMassInsert& NoUseTransaction()                              { return UseTransaction(false); }
	
	SqlMassInsert(Sql& sql, SqlId table) : sql(sql), table(table)  { pos = 0; error = false; use_transaction = true; }
#ifndef NOAPPSQL
	SqlMassInsert(SqlId table) : sql(SQL), table(table)            { pos = 0; error = false; use_transaction = true; }
#endif
	~SqlMassInsert();
};

#ifndef NOAPPSQL

template <class T>
void SqlLoadTable(T& t, SqlId table, SqlId key = SqlId("ID"))
{
	Sql sql;
	sql * Select(SqlAll()).From(table);
	while(sql.Fetch())
		sql.Get(t.Add(sql[key]));
}

template <class T>
void SqlLoadColumn(T& t, SqlId table, SqlId column, SqlId key = SqlId("ID"))
{
	Sql sql;
	sql * Select(key, column).From(table);
	while(sql.Fetch())
		t.Add(sql[key], sql[column]);
}

template <class T>
void SqlLoadTable(T& t, SqlSelect select, SqlId key = SqlId("ID"))
{
	Sql sql;
	sql * select;
	while(sql.Fetch())
		sql.Get(t.Add(sql[key]));
}

template <class T>
void SqlLoadColumn(T& t, SqlSelect select)
{
	Sql sql;
	sql * select;
	while(sql.Fetch())
		t.Add(sql[0], sql[1]);
}

void operator*=(ValueMap& map, SqlSelect select);

template<class K, class V>
void operator*=(VectorMap<K, V>& map, SqlSelect select)
{
	map.Clear();
	Sql sql;
	sql * select;
	while(sql.Fetch())
		map.Add(sql[0], sql[1]);
}

#endif

// Deprecated, use SqlPerformScript instead
struct StdStatementExecutor : StatementExecutor {
	StdStatementExecutor(SqlSession& session) : cursor(session) {}
	virtual bool Execute(const String& stmt);
	Sql cursor;
};

#ifndef NOAPPSQL
StatementExecutor& SQLStatementExecutor();
#endif

#ifdef BackwardCompatibility
	typedef Sql        QSql;
	typedef SqlSession QSession;
#endif
