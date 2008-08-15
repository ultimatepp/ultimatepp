#ifndef _PostgreSQL_PostgeSQL_h_
#define _PostgreSQL_PostgeSQL_h_

#ifndef flagNOPOSTGRESQL

#include <Sql/Sql.h>

#include "lib/libpq-fe.h"

NAMESPACE_UPP

const char *PostgreSQLReadString(const char *s, String& stmt);

bool PostgreSQLPerformScript(const String& text,
#ifdef NOAPPSQL
	StatementExecutor& se,
#else
	StatementExecutor& se = SQLStatementExecutor(),
#endif
	Gate2<int, int> progress_canceled = false
);


String PostgreSQLTextType(int n);

class PostgreSQLSession : public SqlSession {
public:
	virtual bool                  IsOpen() const                   { return conn; }
	virtual RunScript             GetRunScript() const             { return &PostgreSQLPerformScript; }

	virtual Vector<String>        EnumUsers();
	virtual Vector<String>        EnumDatabases();
	virtual Vector<String>        EnumTables(String database);
	virtual Vector<String>        EnumViews(String database);
	virtual Vector<String>        EnumSequences(String database);
	virtual Vector<SqlColumnInfo> EnumColumns(String database, String table);
	virtual Vector<String>        EnumPrimaryKey(String database, String table);
	virtual String                EnumRowID(String database, String table);
	virtual Vector<String>        EnumReservedWords();

protected:
	virtual SqlConnection *CreateConnection();

private:
	PGconn               *conn;
	VectorMap<Oid, int>   oid_type_map; //pg has an oid for every type

	void                  StoreInOidTypeMap(const char *typname, int type_id, const VectorMap<String, int64> &typname_oid_map);
	bool                  InitOidTypeMap();
	void                  ExecTrans(const char * statement);

	Vector<String>        EnumData(char type, const char *schema = NULL);
	String                ErrorMessage();

public:
	int                   OidToType(Oid oid); ///< default is STRING_V
	bool                  Open(const char *connect);
	void                  Close();

	String                GetUser()   { return PQuser(conn); }
	operator PGconn *     ()   { return conn; }

	virtual void          Begin();
	virtual void          Commit();
	virtual void          Rollback();

	PostgreSQLSession()       { conn = NULL; Dialect(POSTGRESS); }
	~PostgreSQLSession()      { Close(); }
};

END_UPP_NAMESPACE

#endif

#endif
