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

class SqlRaw : public String, AssignValueTypeNo<SqlRaw, SQLRAW_V> {
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
};


class Sql {
	SqlConnection  *cn;
	Vector<Value>   param;

	friend class SqlSession;
	friend struct AppSql;

	Value       Select0(const String& what);
	void        Assign(SqlSource& src);
	void        Detach();
	enum _NULLSQL { NULLSQL };
	Sql(_NULLSQL) { cn = NULL; }

public:
	String Compile(const SqlStatement& s);

	void   Clear();

	void   SetParam(int i, const Value& val);
	void   SetStatement(const String& s);
	void   SetStatement(const SqlStatement& s)         { SetStatement(Compile(s)); }

	bool   Execute();
	void   ExecuteX();
	bool   Run()                                       { return Execute(); }
	void   RunX()                                      { ExecuteX(); }

	bool   Execute(const String& s);
	void   ExecuteX(const String& s);

	bool   Execute(const SqlStatement& s)              { return Execute(Compile(s)); }
	void   ExecuteX(const SqlStatement& s)             { ExecuteX(Compile(s)); }
//$-
#define  E__Run(I)       bool Run(__List##I(E__Value));
	__Expand(E__Run)
//$ bool Run(const Value& v1 [, const Value& v2 ...]);

#define  E__RunX(I)      void RunX(__List##I(E__Value));
	__Expand(E__RunX)

#define  E__Execute(I)   bool Execute(const String& s, __List##I(E__Value));
	__Expand(E__Execute)

#define  E__ExecuteX(I)  void ExecuteX(const String& s, __List##I(E__Value));
	__Expand(E__ExecuteX)

	bool   Fetch();

#define  E__Fetch(I)    bool Fetch(__List##I(E__Ref));
	__Expand(E__Fetch)
//$+
	bool   Fetch(Vector<Value>& row);

	bool   Fetch(Fields fields);

	int    GetRowsProcessed() const                    { return cn->GetRowsProcessed(); }

	int    GetColumns() const;

	void   GetColumn(int i, Ref r) const;
	void   GetColumn(SqlId colid, Ref r) const;
	Value  operator[](int i) const;
	Value  operator[](SqlId colid) const;
	const SqlColumnInfo& GetColumnInfo(int i) const    { return cn->info[i]; }
	Vector<Value> GetRow() const;
	operator Vector<Value>() const                     { return GetRow(); }
	void   Get(Fields fields);

	void        SetFetchRows(int nrows)                { cn->fetchrows = nrows; }
	void        SetLongSize(int lsz)                   { cn->longsize = lsz; }

	void        Cancel()                               { cn->Cancel(); }

	Value       Select(const String& what);

#define  E__Select(I)   Value Select(const String& what, __List##I(E__Value));
	__Expand(E__Select)

#define  E__Insert(I)  bool Insert(const char *tb, const char *c0, const Value& v0, __List##I(E__ColVal));
	__Expand(E__Insert)

#define  E__InsertId(I)  bool Insert(SqlId tb, SqlId c0, const Value& v0, __List##I(E__IdVal));
	__Expand(E__InsertId)

#define  E__Update(I)  bool Update(const char *tb, const char *k, const Value& kv, __List##I(E__ColVal));
	__Expand(E__Update)

#define  E__UpdateId(I)  bool Update(SqlId tb, SqlId k, const Value& kv, __List##I(E__IdVal));
	__Expand(E__UpdateId)

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

	bool   operator*(const SqlStatement& q)            { return Execute(q); }
	Sql&   operator&(const SqlStatement& q)            { ExecuteX(q); return *this; }
	Value  operator%(const SqlStatement& q)            { return Select0(Compile(q)); }

	SqlSession& GetSession() const                     { return cn->GetSession(); }
	int    GetDialect() const;

	Value  GetInsertedId() const                       { return cn->GetInsertedId(); }

	String GetUser() const                             { return cn->GetUser(); }

	enum ERRORCLASS {
		ERROR_UNSPECIFIED,
		CONNECTION_BROKEN,
	};

	void   SetError(String error, String stmt, int code = 0, const char *scode = NULL, ERRORCLASS clss = ERROR_UNSPECIFIED);
	String GetLastError() const;
	String GetErrorStatement() const;
	int    GetErrorCode() const;
	String GetErrorCodeString() const;
	ERRORCLASS GetErrorClass() const;
	void   ClearError();

	void   Begin();
	void   Commit();
	void   Rollback();
	int    GetTransactionLevel();

	String Savepoint();
	void   RollbackTo(const String& savepoint);

	bool   IsOpen();

	bool   WasError() const;

	Sql(SqlSource&);
#ifndef NOAPPSQL
	Sql();
	Sql(const char *stmt);
	Sql(const SqlStatement& s);
#endif
	Sql(const char *stmt, SqlSource& session);
	Sql(const SqlStatement& s, SqlSource& session);
	Sql(SqlConnection *connection);
	~Sql();
};

#ifndef NOAPPSQL

struct AppSql : Sql {
	void   operator=(SqlSource& s) { Assign(s); }
	void   Detach()                { Sql::Detach(); }
	AppSql() : Sql(NULLSQL) {}
};

AppSql& AppCursor();
//$-
#define SQL AppCursor()
//$+
//  Assist++ cheat:
//$ AppSql SQL;

#endif


struct StatementExecutor {
	virtual bool Execute(const String& stmt) = 0;
	virtual ~StatementExecutor() {}
};

typedef bool (*RunScript)(const String& text, StatementExecutor& executor, Gate2<int, int> progress_canceled);

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

	Stream                       *trace;
	bool                          logerrors;
	bool                          tracetime;
	bool                          usrlog;
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

	int                           status;
	
	One<Sql>                      sql;

public:
	virtual void                  Begin();
	virtual void                  Commit();
	virtual void                  Rollback();
	virtual int                   GetTransactionLevel() const;

	virtual String                Savepoint();
	virtual void                  RollbackTo(const String& savepoint);

	virtual bool                  IsOpen() const;

	virtual RunScript             GetRunScript() const;

	virtual Vector<String>        EnumUsers();
	virtual Vector<String>        EnumDatabases();
	virtual Vector<String>        EnumTables(String database);
	virtual Vector<String>        EnumViews(String database);
	virtual Vector<String>        EnumSequences(String database);
	virtual Vector<SqlColumnInfo> EnumColumns(String database, String table);
	virtual Vector<String>        EnumPrimaryKey(String database, String table);
	virtual String                EnumRowID(String database, String table);
	virtual Vector<String>        EnumReservedWords();

	SqlSession&                   Dialect(int q)                          { dialect = q; return *this; }
	int                           GetDialect() const                      { ASSERT(dialect != 255); return dialect; }

	void                          SetTrace(Stream& s = VppLog())          { trace = &s; }
	Stream                       *GetTrace() const                        { return trace; }
	void                          KillTrace()                             { trace = NULL; }

	void                          TraceTime(bool b = true)                { tracetime = b; }
	bool                          IsTraceTime() const                     { return tracetime; }

	SqlSession&                   LogErrors(bool b = true)                { logerrors = true; return *this; }
	SqlSession&                   UsrLog(bool b = true)                   { usrlog = true; return *this; }
	SqlSession&                   TraceSlow(int ms = 5000)                { traceslow = ms; return *this; }

	bool                          WasError() const                        { return !GetLastError().IsEmpty(); }

	void                          SetError(String error, String stmt, int code = 0, const char * scode = NULL, Sql::ERRORCLASS clss = Sql::ERROR_UNSPECIFIED);
	String                        GetLastError() const                    { return lasterror; }
	String                        GetErrorStatement() const               { return errorstatement; }
	int                           GetErrorCode() const                    { return errorcode_number; }
	String                        GetErrorCodeString() const              { return errorcode_string; }
	Sql::ERRORCLASS               GetErrorClass() const                   { return errorclass; }
	void                          ClearError();
	void                          InstallErrorHandler(bool (*handler)(String error, String stmt, int code, const char *scode, Sql::ERRORCLASS clss));

	String                        GetStatement() const                    { return statement; }
	void                          SetStatement(const String& s)           { statement = s; }

	void                          SetTime(int t)                          { exectime = t; }
	int                           GetTime() const                         { return exectime; }

	String                        GetUser()                               { return Sql(*this).GetUser(); }
	
	Sql&                          GetSessionSql();

	operator                      bool() const                            { return IsOpen(); }

	int                           GetStatus()                             { return status; }
	void                          SetStatus(int s)                        { status = s; WhenDatabaseActivity(*this); }
	bool                          operator == (int s) const               { return status == s; }
	bool                          operator != (int s) const               { return status != s; }

	Callback1<const SqlSession&>  WhenDatabaseActivity;

	SqlSession();
	virtual ~SqlSession();

};

class OciConnection;

bool SqlPerformScript(SqlSession& session, Stream& script,
                      Gate2<int, int> progress_canceled = false, bool stoponerror = false);
bool SqlPerformScript(Stream& script,
                      Gate2<int, int> progress_canceled = false, bool stoponerror = false);
bool SqlPerformScript(SqlSession& session, const String& script,
                      Gate2<int, int> progress_canceled = false, bool stoponerror = false);
bool SqlPerformScript(const String& script,
                      Gate2<int, int> progress_canceled = false, bool stoponerror = false);

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

class SqlMassInsert {
	struct Row : Moveable<Row> {
		dword          nulls;
		Vector <Value> value;
	};

	Sql&            sql;
	String          table;
	Vector<String>  column;
	Vector<Row>     cache;
	int             pos;
	
	void            NewRow();
	

public:
	SqlMassInsert& operator()(SqlId col, const Value& val);
	SqlMassInsert& EndRow();
	void           Flush();
	
	SqlMassInsert(Sql& sql, SqlId table) : sql(sql), table(~table) { pos = 0; }
#ifndef NOAPPSQL
	SqlMassInsert(SqlId table) : sql(SQL), table(~table) { pos = 0; }
#endif
	~SqlMassInsert();
};
