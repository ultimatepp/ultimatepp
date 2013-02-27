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
#ifndef flagUSE_SQLITE
	if(!session.Connect(userName, password, dbName, host))
#else
	if(!session.Open(dbName)) 
#endif
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

// refresh connection
bool ProtectDB::RefreshConnection(void)
{
	Disconnect();
	return Connect();
}

// get data -- email is the key -- non repeatable
VectorMap<String, Value> ProtectDB::Get(String const &mail)
{
	VectorMap<String, Value> res = Default();
	
	if(!RefreshConnection())
	{
		Cerr() << "SQL Error -- Session expired\n";
		res.Clear();
		return res;
	}
	
	for(int i = 0; i < 2; i++)
	{
		SQL * Select(SqlAll()).From(USERS).Where(EMAIL == mail);
		if(SQL.WasError())
		{
			if(!RefreshConnection())
			{
				Cerr() << "SQL Error : " << session.GetErrorCodeString() << "\n";
				res.Clear();
				return res;
			}
			else
				continue;
		}
		else
			break;
	}
	// we've unique key, so...
	if(SQL.Fetch())
		for(int i = 0; i < res.GetCount(); i++)
			res[i] = SQL[res.GetKey(i)];
	else
	{
		res.Clear();
		return res;
	}
		
	return res;
}

// set/append data
bool ProtectDB::Set(VectorMap<String, Value> const &d)
{
	String eMail = d.Get("EMAIL");
	
	// fill missing fields with default values
	VectorMap<String, Value>data = Default(d);
	

	for(int i = 0; i < 2; i++)
	{
		SQL * Select(SqlAll()).From(USERS).Where(EMAIL == eMail);
		if(SQL.WasError())
		{
			Cerr() << "Trying to refresh connection...\n";
			if(!RefreshConnection())
			{
				Cerr() << "SQL Error : " << session.GetErrorCodeString() << "\n";
				return false;
			}
			else
				continue;
		}
		else
			break;
	}
	if(SQL.Fetch())
	{
		SQL * Update(USERS)
			(NAME					, data.Get("NAME"))
			(SURNAME				, data.Get("SURNAME"))
			(ADDRESS				, data.Get("ADDRESS"))
			(TOWN					, data.Get("TOWN"))
			(ZONE					, data.Get("ZONE"))
			(COUNTRY				, data.Get("COUNTRY"))
			(ZIP					, data.Get("ZIP"))
			(CATHEGORY				, data.Get("CATHEGORY"))
			(CF						, data.Get("CF"))
			(PIVA					, data.Get("PIVA"))
			(PHONE					, data.Get("PHONE"))
			(FAX					, data.Get("FAX"))
			(CELL					, data.Get("CELL"))
			(LICENSES				, data.Get("LICENSES"))
			(EXPIRATION				, data.Get("EXPIRATION"))
			(ALLOWEDVERSION			, data.Get("ALLOWEDVERSION"))
			(ACTIVATIONKEY			, data.Get("ACTIVATIONKEY"))
			(REGISTRATIONDATE		, data.Get("REGISTRATIONDATE"))
			(ACTIVATED				, data.Get("ACTIVATED"))
			(ACTIVATIONDATE			, data.Get("ACTIVATIONDATE"))
			(LASTCONNECTIONDATE		, data.Get("LASTCONNECTIONDATE"))
			(TOTALCONNECTIONS		, data.Get("TOTALCONNECTIONS"))
			(TOTALCONNECTIONTIME	, data.Get("TOTALCONNECTIONTIME"))
			.Where(EMAIL == eMail);
	}
	else
	{
		SQL * Insert(USERS)
			(EMAIL					, eMail)
			(NAME					, data.Get("NAME"))
			(SURNAME				, data.Get("SURNAME"))
			(ADDRESS				, data.Get("ADDRESS"))
			(TOWN					, data.Get("TOWN"))
			(ZONE					, data.Get("ZONE"))
			(COUNTRY				, data.Get("COUNTRY"))
			(ZIP					, data.Get("ZIP"))
			(CATHEGORY				, data.Get("CATHEGORY"))
			(CF						, data.Get("CF"))
			(PIVA					, data.Get("PIVA"))
			(PHONE					, data.Get("PHONE"))
			(FAX					, data.Get("FAX"))
			(CELL					, data.Get("CELL"))
			(LICENSES				, data.Get("LICENSES"))
			(EXPIRATION				, data.Get("EXPIRATION"))
			(ALLOWEDVERSION			, data.Get("ALLOWEDVERSION"))
			(ACTIVATIONKEY			, data.Get("ACTIVATIONKEY"))
			(REGISTRATIONDATE		, data.Get("REGISTRATIONDATE"))
			(ACTIVATED				, data.Get("ACTIVATED"))
			(ACTIVATIONDATE			, data.Get("ACTIVATIONDATE"))
			(LASTCONNECTIONDATE		, data.Get("LASTCONNECTIONDATE"))
			(TOTALCONNECTIONS		, data.Get("TOTALCONNECTIONS"))
			(TOTALCONNECTIONTIME	, data.Get("TOTALCONNECTIONTIME"))
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
	res.FindAdd("SURNAME", "");
	res.FindAdd("ADDRESS", "");
	res.FindAdd("TOWN", "");
	res.FindAdd("ZONE", "");
	res.FindAdd("COUNTRY", "");
	res.FindAdd("ZIP", "");
	res.FindAdd("CATHEGORY", 1);
	res.FindAdd("CF", "");
	res.FindAdd("PIVA", "");
	res.FindAdd("PHONE", "");
	res.FindAdd("FAX", "");
	res.FindAdd("CELL", "");
	res.FindAdd("LICENSES", 1);
	res.FindAdd("EXPIRATION", GetSysTime() + 24*60*60*30);
	res.FindAdd("ALLOWEDVERSION", "9999.9999.9999");
	res.FindAdd("ACTIVATIONKEY", "");
	res.FindAdd("REGISTRATIONDATE", GetSysTime());
	res.FindAdd("ACTIVATED", false);
	res.FindAdd("ACTIVATIONDATE", GetSysTime());
	res.FindAdd("LASTCONNECTIONDATE", GetSysTime());
	res.FindAdd("TOTALCONNECTIONS", 0);
	res.FindAdd("TOTALCONNECTIONTIME", 0);

	return res;
}

// gets emails connected to e given MAC
// empty Vector if still none
Vector<String> ProtectDB::GetMACEMails(String const &mac)
{
	Vector<String> res;
	
	if(!RefreshConnection())
	{
		Cerr() << "SQL Error -- Session expired\n";
		res.Clear();
		return res;
	}
	
	for(int i = 0; i < 2; i++)
	{
		SQL * Select(SqlAll()).From(MACS).Where(MAC == mac);
		if(SQL.WasError())
		{
			if(!RefreshConnection())
			{
				Cerr() << "SQL Error : " << session.GetErrorCodeString() << "\n";
				res.Clear();
				return res;
			}
			else
				continue;
		}
		else
			break;
	}
	// we've unique key, so...
	if(SQL.Fetch())
	{
		String mailStr = SQL["EMAILS"];
		res = Split(mailStr, ",", true);
	}
	else
		res.Clear();

	return res;
}

// update (or creates) given MAC record with connected mail
bool ProtectDB::UpdateMac(String const &mac, Vector<String> const &eMails)
{
	// get all emails connected to this MAC
	Index<String> prevMails(GetMACEMails(mac));
	
	
	// check if mail is already there
	bool mustUpdate = false;
	for(int i = 0; i < eMails.GetCount(); i++)
		if(prevMails.Find(ToLower(eMails[i])) < 0)
		{
			mustUpdate = true;
			prevMails.Add(ToLower(eMails[i]));
		}

	// if eMail was already there, just return true
	if(!mustUpdate)
		return true;

	String mailList = Join(prevMails.PickKeys(), ",");
	for(int i = 0; i < 2; i++)
	{
		SQL * Select(SqlAll()).From(MACS).Where(MAC == mac);
		if(SQL.WasError())
		{
			Cerr() << "Trying to refresh connection...\n";
			if(!RefreshConnection())
			{
				Cerr() << "SQL Error : " << session.GetErrorCodeString() << "\n";
				return false;
			}
			else
				continue;
		}
		else
			break;
	}
	if(SQL.Fetch())
	{
		SQL * Update(MACS)
			(EMAILS					, mailList)
			.Where(MAC == mac);
	}
	else
	{
		SQL * Insert(MACS)
			(MAC					, mac)
			(EMAILS					, mailList)
		;
	}
	
	return true;
}

END_UPP_NAMESPACE
