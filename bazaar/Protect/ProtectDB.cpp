#include "ProtectDB.h"

NAMESPACE_UPP

#include <Sql/sch_schema.h>
#include <Sql/sch_source.h>

ProtectDB::ProtectDB()
{
	connected = false;
	upgraded = false;
}

ProtectDB::~ProtectDB()
{
	if(connected)
		Disconnect();
}

// connect/disconnect
bool ProtectDB::Connect(void)
{
	if(!session.Connect(userName, password, dbName, host))
		return false;
	connected = true;
	SQL = session;

	// upgrade database on first connect
	if(!upgraded)
	{
		SqlSchema sch(MY_SQL);
		All_Tables(sch);
		SqlPerformScript(sch.Upgrade());
		SqlPerformScript(sch.Attributes());
		SQL.ClearError();
		upgraded = true;
	}
		
	return true;
}

bool ProtectDB::Disconnect(void)
{
	if(!connected)
		return true;
	session.Close();
	connected = false;
	return true;
}

// get data -- email is the key -- non repeatable
VectorMap<String, Value> ProtectDB::Get(String const &mail)
{
	VectorMap<String, Value> res = Default();
	
	SQL * Select(SqlAll()).From(USERS).Where(EMAIL == mail);
	// we've unique key, so...
	if(SQL.Fetch())
		for(int i = 0; i < res.GetCount(); i++)
			res[i] = SQL[res.GetKey(i)];
	else
		res.Clear();
		
	return res;
}

// set/append data
bool ProtectDB::Set(VectorMap<String, Value> const &d)
{
	String eMail = d.Get("EMAIL");
	
	// fill missing fields with default values
	VectorMap<String, Value>data = Default(d);
	
	SQL * Select(SqlAll()).From(USERS).Where(EMAIL == eMail);
	if(SQL.Fetch())
	{
		SQL * Update(USERS)
			(NAME			, data.Get("NAME"))
			(ADDRESS		, data.Get("ADDRESS"))
			(COUNTRY		, data.Get("COUNTRY"))
			(ZIP			, data.Get("ZIP"))
			(PHONE			, data.Get("PHONE"))
			(FAX			, data.Get("FAX"))
			(CELL			, data.Get("CELL"))
			(LICENSES		, data.Get("LICENSES"))
			(EXPIRATION		, data.Get("EXPIRATION"))
			(ACTIVATIONKEY	, data.Get("ACTIVATIONKEY"))
			(ACTIVATIONSENT	, data.Get("ACTIVATIONSENT"))
			(ACTIVATED		, data.Get("ACTIVATED"))
			.Where(EMAIL == eMail);
	}
	else
	{
		SQL * Insert(USERS)
			(EMAIL			, eMail)
			(NAME			, data.Get("NAME"))
			(ADDRESS		, data.Get("ADDRESS"))
			(COUNTRY		, data.Get("COUNTRY"))
			(ZIP			, data.Get("ZIP"))
			(PHONE			, data.Get("PHONE"))
			(FAX			, data.Get("FAX"))
			(CELL			, data.Get("CELL"))
			(LICENSES		, data.Get("LICENSES"))
			(EXPIRATION		, data.Get("EXPIRATION"))
			(ACTIVATIONKEY	, data.Get("ACTIVATIONKEY"))
			(ACTIVATIONSENT	, data.Get("ACTIVATIONSENT"))
			(ACTIVATED		, data.Get("ACTIVATED"))
		;
	}
	return true;
}

// get default record
VectorMap<String, Value> ProtectDB::Default(VectorMap<String, Value> const &base)
{
	VectorMap<String, Value> res(base, 1);
	res.FindAdd("EMAIL", "");
	res.FindAdd("NAME", "");
	res.FindAdd("ADDRESS", "");
	res.FindAdd("COUNTRY", "");
	res.FindAdd("ZIP", "");
	res.FindAdd("PHONE", "");
	res.FindAdd("FAX", "");
	res.FindAdd("CELL", "");
	res.FindAdd("LICENSES", 1);
	res.FindAdd("EXPIRATION", GetSysTime() + 24*60*60*30);
	dword r = Random();
	res.FindAdd("ACTIVATIONKEY", HexString((byte *)&r, sizeof(r)));
	res.FindAdd("ACTIVATIONSENT", GetSysTime());
	res.FindAdd("ACTIVATED", false);
	res.FindAdd("ALLOWEDVERSION", 0);

	return res;
}

/*
	MySqlSession session;
	// substitute your 'username' and 'password' here:
	if(!session.Connect("username", "password", "mysql", "localhost")) {
		printf("Can't connect with MySql\n");
		return;
	}
	Sql sql(session);
	sql.Execute("use mysql");
	sql.Execute("show tables");
	while(sql.Fetch())
		Cout() << (String)sql[0] << '\n';
*/

END_UPP_NAMESPACE
