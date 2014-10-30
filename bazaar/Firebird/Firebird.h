#ifndef _Firebird_Firebird_h
#define _Firebird_Firebird_h

#include "fb.h"

NAMESPACE_UPP

class TransAuto
{
public:
	TransAuto(SqlSession& s);
	TransAuto(Sql& s);
	~TransAuto();

public:
	operator bool() const { return abort; }
	void Finish() { abort = false; }

protected:
	bool abort:1;
	bool isSession:1;
	union {
		SqlSession*	session;
		Sql*		sql;
	};
};

#define TRANSACTION(trans) \
for(TransAuto auto_trans_##__LINE__(trans); \
    auto_trans_##__LINE__; \
    auto_trans_##__LINE__.Finish() )


#ifndef NOAPPSQL
bool FirebirdPerformScript(const String& text, StatementExecutor& se = SQLStatementExecutor(), Gate2<int, int> progress_canceled = false);
#else
bool FirebirdPerformScript(const String& text, StatementExecutor& se, Gate2<int, int> progress_canceled = false);
#endif


class FBSession : public SqlSession {
	friend class FBConnection;
	friend class TransRetain;
	friend struct FBBlob;
		
public:
	virtual SqlConnection *CreateConnection();
	virtual Vector<SqlColumnInfo> EnumColumns(String database, String table);
	virtual Vector<String> EnumDatabases();
	virtual Vector<String> EnumPrimaryKey(String database, String table);
	virtual Vector<String> EnumReservedWords();
//	virtual String EnumRowID(String database, String table);
	virtual Vector<String> EnumSequences(String database);
	virtual Vector<String> EnumTables(String database);
	virtual Vector<String> EnumUsers();
	virtual Vector<String> EnumViews(String database);
	virtual RunScript GetRunScript() const;
//	virtual int GetTransactionLevel() const;
	virtual void RollbackTo(const String& savepoint);
	virtual String Savepoint();

public:
	FBSession(const String& dllName = Null);
	virtual ~FBSession();
	
public:
	void Connect(
		const char* dbname,
		const char* host = NULL,
		const char* user = NULL,
		const char* pswd = NULL,
		const String& dllName = Null, // In case of empty value default dll will be used.
		ibpp::network_protocol_t protocol = ibpp::np_local
	);

	virtual void Begin();
	virtual void Commit();
	virtual void Rollback();

	virtual bool IsOpen() const;

	bool IsTransStarted() const { return TrStarted; }
	bool IsExplicitTrans() const { return TrExplicit; }
	
protected:
	void BeginInternal();
	void CommitInternal();
	void RollbackInternal();

	//	Commit a transaction, and start a new one using the original
	//	transaction’s context
	void CommitRetaining();
	void RollbackRetaining();

	bool CursorIsClosed() const { return CursIsClosed; }
	void SetCursorIsClosed(bool flag = true) { CursIsClosed = flag; }
	
private:
	bool SvcConnected:1;
	bool DbConnected:1;
	bool TrStarted:1;
	bool CursIsClosed:1;
	bool TrExplicit:1;

	String				DbName;
	T_FB&               dll;
	ibpp::Error         ib_error;
	ibpp::Service		svc; 
	ibpp::DataBase		db;
	ibpp::Transaction	tr;
	ibpp::SQLDataArray  tmpDataArray;
};

class FBSequence : public ValueGen {
	SqlId       ssq;
	SqlId&      seq;
	SqlSession *session;

public:
	virtual Value  Get();

	Value operator++()                                                  { return Get(); }

	void Set(SqlId id, SqlSession& s)                                   { ssq = id; session = &s; }

#ifndef NOAPPSQL
	void Set(SqlId id)                                                  { ssq = id; session = NULL; }
	FBSequence(const char *name) : ssq(name), seq(ssq)                  { session = NULL; }
	FBSequence(SqlId& seq) : seq(seq)                                   { session = NULL; }
#endif
	FBSequence(const char *name, SqlSession& s) : ssq(name), seq(ssq)   { session = &s; }
	FBSequence(SqlId& seq, SqlSession& s) : seq(seq)                    { session = &s; }
	FBSequence() : seq(ssq)                                             { session = NULL; }
};

END_UPP_NAMESPACE

#endif

