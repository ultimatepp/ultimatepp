#ifndef _ODBC_ODBC_h
#define _ODBC_ODBC_h

#include <Sql/Sql.h>
#include <sql.h>
#include <sqlext.h>

// debian: sudo apt-get install unixodbc-dev

namespace Upp {

bool   ODBCPerformScript(const String& text, StatementExecutor& executor, Gate<int, int> progress_canceled = Null);

class ODBCConnection;

class ODBCSession : public SqlSession {
public:
	virtual void           Begin();
	virtual void           Commit();
	virtual void           Rollback();

	virtual String         Savepoint();
	virtual void           RollbackTo(const String& savepoint);

	virtual bool           IsOpen() const;

	virtual Vector<String> EnumUsers();
	virtual Vector<String> EnumDatabases();
	virtual Vector<String> EnumTables(String database);
	virtual Vector<String> EnumViews(String database);
	virtual Vector<String> EnumSequences(String database);
	virtual Vector<String> EnumPrimaryKeys(String database, String table);
	virtual String         EnumRowID(String database, String table);

	virtual RunScript      GetRunScript() const                   { return &ODBCPerformScript; }

protected:
	virtual SqlConnection *CreateConnection();

private:
	friend class ODBCConnection;
	HENV                  henv;
	HDBC                  hdbc;
	HSTMT                 hstmt;
	String                user;
	int                   tlevel;
	int                   tmode;
	int                   charset = -1;

	void   FlushConnections();
	bool   IsOk(SQLRETURN ret);

public:
	static Array< Tuple2<String, String> > EnumDSN();

	bool Connect(const char *cs);
	void Close();
	
	enum TransactionMode {
		NORMAL,              // autocommit at level 0, no Commit or Rollback allowed at level 0 (default)
		IMPLICIT             // Oracle-style Commit and Rollback at level 0
	};

	void    SetTransactionMode(int mode);
	void    Charset(int cs)                              { charset = cs; }

	ODBCSession();
	~ODBCSession();
};

}

#endif
