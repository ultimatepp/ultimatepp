#ifndef ORACLE8_H
#define ORACLE8_H

#include <Sql/Sql.h>
#include "OraCommon.h"
#include "OciCommon.h"

namespace Upp {

#if defined(PLATFORM_WIN32) && defined(COMPILER_MSC)
#define __STDC__ 1
#endif

// selected definitions from OCI 8 headers
struct OCIEnv;
struct OCIServer;
struct OCISession;
struct OCISvcCtx;
struct OCIError;
struct OCILobLocator;

#define DLLFILENAME "oci.dll"
#define DLIMODULE   OCI8
#define DLIHEADER   <Oracle/Oci8.dli>
#include <Core/dli_header.h>

void OCI8SetDllPath(String oci8_path, T_OCI8& oci8 = OCI8_());

//#define dword _dword
//#include <oci.h>
//#undef dword

class OCI8Connection;

class Oracle8 : public SqlSession {
public:
	virtual void                  Begin();
	virtual void                  Commit();
	virtual void                  Rollback();

	virtual String                Savepoint();
	virtual void                  RollbackTo(const String& savepoint);

	virtual bool                  IsOpen() const;

	virtual RunScript             GetRunScript() const          { return &OraclePerformScript; }

	virtual Vector<String>        EnumUsers();
	virtual Vector<String>        EnumDatabases();
	virtual Vector<String>        EnumTables(String database);
	virtual Vector<String>        EnumViews(String database);
	virtual Vector<String>        EnumSequences(String database);
	virtual Vector<String>        EnumPrimaryKey(String database, String table);
	virtual String                EnumRowID(String database, String table);
	virtual Vector<String>        EnumReservedWords();

	bool                          BulkExecute(const char *stmt, const Vector< Vector<Value> >& param_rows);
	bool                          BulkExecute(const SqlStatement& stmt, const Vector< Vector<Value> >& param_rows);

public:
	T_OCI8&              oci8;

protected:
	virtual SqlConnection *CreateConnection();

private:
	bool                   AllocOciHandle(void *hp, int type);
	void                   FreeOciHandle(void *hp, int type);
	void                   SetOciError(String text, OCIError *errhp);

private:
	friend class OCI8Connection;
	friend class OracleBlob;
	friend class OracleClob;

	OCIEnv              *envhp;
	OCIError            *errhp;
	OCIServer           *srvhp;
	OCISession          *seshp;
	OCISvcCtx           *svchp;
	Link<>               clink;
	bool                 in_server;
	bool                 in_session;
	bool                 utf8_session;
	bool                 disable_utf8_mode;
	String               user;
	int                  level;
	int                  tmode;

	String         Spn();
	void           PostError();
	bool           StdMode()   { return  tmode == NORMAL; }

public:
	bool Login(const char *name, const char *password, const char *connect, bool use_objects, String *warn = NULL);
	bool Open(const String& connect_string, bool use_objects, String *warn = NULL);
	void Logoff();

	enum TransactionMode {
		NORMAL,              // autocommit at level 0, no Commit or Rollback allowed at level 0
		ORACLE               // Oracle-style Commit and Rollback at level 0
	};

	void    SetTransactionMode(int mode)            { tmode = mode; }
	void    DisableUtf8Mode(bool dutf8 = true)      { disable_utf8_mode = dutf8; }
	
	bool    IsUtf8Session() const                   { return utf8_session; }
	
	Oracle8(T_OCI8& oci8 = OCI8_());
	~Oracle8();
};

class OracleBlob : public BlockStream {
protected:
	virtual  void  SetStreamSize(int64 size);
	virtual  dword Read(int64 at, void *ptr, dword size);
	virtual  void  Write(int64 at, const void *data, dword size);

public:
	virtual  bool  IsOpen() const;
	virtual  void  Close();

protected:
	OCILobLocator   *locp;
	Oracle8         *session;

	void      Init(dword size);

public:
	operator  bool() const                 { return IsOpen(); }

	void      Assign(const Sql& sql, int64 blob);
	void      Assign(Oracle8& session, int64 blob);

	OracleBlob();
	OracleBlob(const Sql& sql, int64 blob);
	OracleBlob(Oracle8& session, int64 blob);
	~OracleBlob();
};

class OracleClob : public BlockStream {
protected:
	virtual  void  SetStreamSize(int64 size);
	virtual  dword Read(int64 at, void *ptr, dword size);
	virtual  void  Write(int64 at, const void *data, dword size);

public:
	virtual  bool  IsOpen() const;
	virtual  void  Close();

protected:
	OCILobLocator   *locp;
	Oracle8         *session;

	void      Init(dword size);

public:
	operator  bool() const                 { return IsOpen(); }

	void      Assign(const Sql& sql, int64 blob);
	void      Assign(Oracle8& session, int64 blob);

	WString   Read();
	void      Write(const WString& w);
	void      SetLength(int sz);

	OracleClob();
	OracleClob(const Sql& sql, int64 blob);
	OracleClob(Oracle8& session, int64 blob);
	~OracleClob();
};

#ifdef text
#undef text
#endif

}

#endif
