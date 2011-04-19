#ifndef __mysql_h__
#define __mysql_h__

#ifndef flagNOMYSQL

#include <Sql/Sql.h>

#ifdef PLATFORM_WIN32
#ifndef IPPROTO_IP
#include <winsock2.h>
#endif
#include <MySql.h>
#endif

#ifdef PLATFORM_POSIX
#include </usr/include/mysql/mysql.h>
#endif

NAMESPACE_UPP

const char *MySqlReadString(const char *s, String& stmt);

bool MySqlPerformScript(const String& text, StatementExecutor& se, Gate2<int, int> progress_canceled = false);
#ifdef NOAPPSQL
bool MySqlUpdateSchema(const SqlSchema& sch, int i, StatementExecutor& se);
#else
bool MySqlUpdateSchema(const SqlSchema& sch, int i, StatementExecutor& se = SQLStatementExecutor());
#endif

String MySqlTextType(int n);

class MySqlSession : public SqlSession {
public:
	virtual bool           IsOpen() const;
	virtual RunScript      GetRunScript() const             { return &MySqlPerformScript; }
	virtual Vector<String> EnumUsers();
	virtual Vector<String> EnumDatabases();
	virtual Vector<String> EnumTables(String database);

protected:
	virtual SqlConnection *CreateConnection();

private:
	MYSQL *mysql;
	String username;
	double lastid;
	int level;

public:
	bool Connect(const char *user = NULL, const char *password = NULL, const char *database = NULL,
		         const char *host = NULL, int port = MYSQL_PORT, const char *socket = NULL);
	bool Open(const char *connect);
	void Close();

	String   GetUser()   { return username; }

	operator MYSQL *()   { return mysql; }

	virtual void   Begin();
	virtual void   Commit();
	virtual void   Rollback();
	virtual int    GetTransactionLevel() const;

	MySqlSession()       { mysql = NULL; Dialect(MY_SQL); }
	~MySqlSession()      { Close(); }
};

END_UPP_NAMESPACE

#endif

#endif
