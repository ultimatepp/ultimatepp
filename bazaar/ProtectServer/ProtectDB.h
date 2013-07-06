#ifndef _Protect_ProtectDB_h_
#define _Protect_ProtectDB_h_

#include <Core/Core.h>

#ifndef flagUSE_SQLITE
#include <MySql/MySql.h>
#else
#include <plugin/sqlite3/Sqlite3.h>
#endif

NAMESPACE_UPP

#ifndef flagUSE_SQLITE
#define SCHEMADIALECT <MySql/MySqlSchema.h>
#else
#define SCHEMADIALECT <plugin/sqlite3/Sqlite3Schema.h>
#endif
#define MODEL <ProtectServer/ProtectDB.sch>
#include "Sql/sch_header.h"

// encapsulation of Protect database
class ProtectDB
{
	private:
	
		// host, db name, username and password
		String host;
		String dbName;
		String userName;
		String password;
		
		// connected flag
		bool connected;
		
		// database upgraded flag
		bool upgraded;
		
		// database session
#ifndef flagUSE_SQLITE
		MySqlSession session;
#else
		Sqlite3Session session;
#endif
	
	protected:
	
	public:
	
		ProtectDB();
		~ProtectDB();
	
		// setting of connection data
		ProtectDB &SetHost(String const &_host)		{ host = _host; return *this; }
		ProtectDB &SetDBName(String const &_dbName)	{ dbName = _dbName; return *this; }
		ProtectDB &SetUserName(String const &_name)	{ userName = _name; return *this; }
		ProtectDB &SetPassword(String const &_pwd)	{ password = _pwd; return *this; }
		
		// connect/disconnect
		bool Connect(void);
		bool Disconnect(void);
		bool IsConnected(void) { return connected; }
		
		// refresh connection
		bool RefreshConnection(void);
		
		// get data -- email is the key -- non repeatable
		VectorMap<String, Value>Get(String const &mail);
		
		// set/append data
		bool Set(VectorMap<String, Value> const &data);
		
		// get default record
		VectorMap<String, Value> Default(VectorMap<String, Value> const &base = VectorMap<String, Value>());
		
		// network adapter MAC checkings -- if enabled, avoid multiple demo
		// registrations with different mails on same machine(s)
		Vector<String> GetMACEMails(String const &mac);
		bool UpdateMac(String const &mac, Vector<String> const &eMails);
};

END_UPP_NAMESPACE
#endif
