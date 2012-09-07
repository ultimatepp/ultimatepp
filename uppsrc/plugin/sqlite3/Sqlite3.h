#ifndef __Plugin_Sqlite3__
#define __Plugin_Sqlite3__

#include <Sql/Sql.h>

NAMESPACE_UPP


// Deprecated, use SqlPerformScript instead
bool Sqlite3PerformScript(const String& text,
#ifdef NOAPPSQL
	StatementExecutor& se,
#else
	StatementExecutor& se = SQLStatementExecutor(),
#endif
	Gate2<int, int> progress_canceled = false
);

class Sqlite3Connection;

class Sqlite3Session : public SqlSession {
public:
	virtual bool           IsOpen() const               { return NULL != db; }
	virtual RunScript      GetRunScript() const         { return &Sqlite3PerformScript; }
	virtual Vector<String> EnumUsers()                  { NEVER(); return Vector<String>(); } // No users in sqlite!
	virtual Vector<String> EnumDatabases();	
	virtual Vector<String> EnumTables(String database);
	virtual Vector<String> EnumViews(String database);
	virtual Vector<SqlColumnInfo> EnumColumns(String database, String table);
	virtual int            GetTransactionLevel() const;
	
	// Some opaque structures used by the sqlite3 library
	typedef struct sqlite3 sqlite3;
	typedef struct sqlite3_stmt sqlite3_stmt;

protected:
	virtual SqlConnection *CreateConnection();

private:
	friend class Sqlite3Connection;

	sqlite3 *db;
	String current_filename;
	String current_dbname;
	Link<Sqlite3Connection> clink;
	
	int busy_timeout;
	
	int SqlExecRetry(const char *sql);
	
	void Reset();
	void Cancel();

public:
	bool Open(const char *filename);
	void Close();

	operator sqlite3 *()   { return db; }

	virtual void   Begin();
	virtual void   Commit();
	virtual void   Rollback();
	
	void SetBusyTimeout(int ms)   { busy_timeout = ms; } //infinite if less than 0

	Sqlite3Session();
	~Sqlite3Session();
};

END_UPP_NAMESPACE

#endif
