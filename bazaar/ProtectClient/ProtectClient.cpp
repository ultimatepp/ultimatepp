#include "ProtectClient.h"

#include "NetworkAdapter.h"

NAMESPACE_UPP

ProtectClient::ProtectClient()
{
	// generates client id
	clientID = -1;
	
	// defaults to Snow2 Cypher
	cypher = new Snow2;
	
	// initializes user configuration path
	// (can be reset by SetUserConfigPath() )
#ifdef PLATFORM_POSIX
	userConfigPath = "/home/" + Upp::GetUserName() + "/." + GetExeTitle() + "/";
#else
	userConfigPath = GetAppDataFolder() + "/" + GetExeTitle() + "/";
#endif

	// must load config before connection
	configLoaded = false;
	
	// setup timeout for http server
	// a couple of seconds should be enough !
//	client.Timeout(200);
	client.RequestTimeout(2000);
	client.MaxRetries(1);
}

ProtectClient::~ProtectClient()
{
	// if connected, disconnect from server
	Disconnect();
}

// stores/retrieve config data from stream
void ProtectClient::Xmlize(XmlIO xml)
{
	xml
		("EMail",			userEMail)
		("ClientID",		clientID)
		("ActivationKey",	activationKey)
	;
}

bool ProtectClient::StoreConfig(void)
{
	RealizePath(userConfigPath);
	String path = AppendFileName(userConfigPath, "Protect.xml");
	return StoreAsXMLFile(*this, "", path);
}

bool ProtectClient::LoadConfig(void)
{
	// don't load if already done
	if(configLoaded)
		return true;
	
	String path = AppendFileName(userConfigPath, "Protect.xml");
	if(FileExists(path))
		configLoaded = LoadFromXMLFile(*this, path);
	else
		configLoaded = false;
	
	return configLoaded;
}
		
// sets the encrypting engine
// default Cypher at startup is Snow2
// WARNING -- takes Cypher ownership
void ProtectClient::SetCypher(Cypher *c)
{
	// sets the new cypher freeing previous one
	cypher = c;
	
}

// get MAC of client adapter(s), each separated by comma
String ProtectClient::GetMACs(void)
{
	Vector<NetworkAdapter> adapters = GetNetworkAdaptersInfo();
	String res = "";
	for(int i = 0; i < adapters.GetCount(); i++)
	{
		NetworkAdapter const &adapter = adapters[i];
		// just grab cable and wireless adapters
		if(adapter.type == "ETHERNET" || adapter.type == "IEEE80211")
		{
			String mac = adapter.mac;
			if(!mac.IsEmpty())
				res += mac + ",";
		}
	}
	// strip trailing comma
	if(!res.IsEmpty())
		res = res.Left(res.GetCount() - 1);
	return res;
}
		
// sends a VectorMap to server in encrypted form
// and gets its response
VectorMap<String, Value> ProtectClient::SendMap(VectorMap<String, Value> const &v)
{
	// copy vectormap adding client id and a magic number
	VectorMap<String, Value>dataMap(v, 1);
	dataMap.Add("APPID", "ProtectClient");
	
	// add current locale
	dataMap.Add("LOCALE", LNGAsText(GetCurrentLanguage()).Left(5));
	
	// add client's adapter MAC addresses
	dataMap.Add("MAC", GetMACs());
	
	// sets cypher key (and create a random IV)
	cypher->SetKey(communicationKey);
	
	String postData;
	postData += "IV=" + HexString(cypher->GetNonce()) + "&";
	postData += "DATA=";
	postData += HexString((*cypher)(StoreAsXML(dataMap, "ProtectClient")));

	client.Post(postData);
	
	// WARNING -- PREVIOUSLY WAS ExecuteRedirect -- check on remote server!!!
	String contents = client.Execute();

	// if contents start with "ERROR", just fetch the error desc
	// and put it in result map
	VectorMap<String, Value> resMap;

	if(client.IsTimeout())
	{
		resMap.Add("ERROR", PROTECT_HTTP_ERROR);
		resMap.Add("ERRORMSG", ProtectMessage(PROTECT_HTTP_ERROR));
		return resMap;
	}

	// if contents don't start with IV field signals it
	if(!contents.StartsWith("IV="))
	{
		resMap.Add("ERROR", PROTECT_MISSING_IV);
		resMap.Add("ERRORMSG", ProtectMessage(PROTECT_MISSING_IV));
		return resMap;
	}
	
	// well, we've at least the IV, so we can fetch it and DATA field and decrypt
	StringStream s(contents);
	String line = s.GetLine();
	String IV = ScanHexString(line.Mid(3));
	line = s.GetLine();
	if(!line.StartsWith("DATA="))
	{
		resMap.Add("ERROR", PROTECT_BAD_DATA);
		resMap.Add("ERRORMSG", ProtectMessage(PROTECT_BAD_DATA));
		return resMap;
	}
	
	// decodes DATA field and read VectorMap from it
	cypher->SetKey(communicationKey, IV);
	String decoded = (*cypher)(ScanHexString(line.Mid(5)));
	try
	{
		LoadFromXML(resMap, decoded);
	}
	catch(...)
	{
		resMap.Clear();
		resMap.Add("ERROR", PROTECT_BAD_DATA);
		resMap.Add("ERRORMSG", ProtectMessage(PROTECT_BAD_DATA));
	}
	return resMap;
}

// create a persistent link to server
bool ProtectClient::Connect(void)
{
	lastError = 0;
	
	// disconnect first, im case we're already connected
	if(!Disconnect())
		return false;
	
	// load config, if not already done so
	// this loads previous clientID (reused if still active)
	// and activationKey
	LoadConfig();
	
	// send a connect packet to server
	VectorMap<String, Value>v;
	v.Add("REASON", PROTECT_CONNECT);
	v.Add("EMAIL", userEMail);
	v.Add("CLIENTID", (int)clientID);
	v.Add("ACTIVATIONKEY", activationKey);
	VectorMap<String, Value> res = SendMap(v);
	
	// check for errors
	if(res.Find("ERROR") >= 0)
	{
		lastError = res.Get("ERROR");
		clientID = -1;
		return false;
	}
	
	int i = res.Find("CLIENTID");
	if(i < 0)
	{
		clientID = -1;
		return false;
	}
	else
	{
		// if got a new client id (previous was missing/expired)
		// replaces old one and store on file
		// clientid is used to maintain a persistent client/server relationship
		// on every connection, clientID timeout is updated on server side
		if(clientID != (dword)(int)res[i])
		{
			clientID = (dword)(int)res[i];
			return StoreConfig();
		}
	}
	return true;
}

// disconnect from server
bool ProtectClient::Disconnect(void)
{
	lastError = 0;
	
	// sends a disconnect packet to server
	VectorMap<String, Value>v;
	v.Add("REASON", PROTECT_DISCONNECT);
	v.Add("EMAIL", userEMail);
	v.Add("CLIENTID", (int)clientID);
	VectorMap<String, Value> res = SendMap(v);

	// check for errors
	if(res.Find("ERROR") >= 0)
	{
		lastError = res.Get("ERROR");
		return false;
	}

	// stores config on server; it will try to reuse clientID
	// on next server connection
	StoreConfig();

	return true;
}

// refresh server connection
bool ProtectClient::Refresh(void)
{
	lastError = 0;

	// sends a refresh packet to server
	VectorMap<String, Value>v;
	v.Add("REASON", PROTECT_REFRESH);
	v.Add("EMAIL", userEMail);
	v.Add("CLIENTID", (int)clientID);
	VectorMap<String, Value> res = SendMap(v);

	// check for errors
	if(res.Find("ERROR") >= 0)
	{
		lastError = res.Get("ERROR");
		
		// resets clientID, it'll be re-generated
		// on next server connection request
		clientID = -1;
		StoreConfig();

		return false;
	}
	
	return true;
}

// get license key
String ProtectClient::GetLicenseKey(void)
{
	lastError = 0;

	// sends a getkey packet to server
	VectorMap<String, Value>v;
	v.Add("REASON", PROTECT_GETLICENSEKEY);
	v.Add("EMAIL", userEMail);
	v.Add("CLIENTID", (int)clientID);
	VectorMap<String, Value> res = SendMap(v);

	// check for errors
	if(res.Find("ERROR") >= 0)
	{
		lastError = res.Get("ERROR");
		return "";
	}
	if(res.Find("KEY") < 0)
	{
		lastError = 999;
		return "";
	}
	return res.Get("KEY");
}

// gets license info
bool ProtectClient::GetLicenseInfo(void)
{
	lastError = 0;

	// sends a getinfo packet to server
	VectorMap<String, Value>v;
	v.Add("REASON", PROTECT_GETLICENSEINFO);
	v.Add("EMAIL", userEMail);
	v.Add("CLIENTID", (int)clientID);
	VectorMap<String, Value> res = SendMap(v);

	// check for errors
	if(res.Find("ERROR") >= 0)
	{
		lastError = res.Get("ERROR");
		return false;
	}
	if(res.Find("EMAIL") >= 0)			userEMail			= res.Get("EMAIL");
	if(res.Find("NAME") >= 0)			userName			= res.Get("NAME");
	if(res.Find("SURNAME") >= 0)		userSurname			= res.Get("SURNAME");
	if(res.Find("ZONE") >= 0)			userZone			= res.Get("ZONE");
	if(res.Find("COUNTRY") >= 0)		userCountry			= res.Get("COUNTRY");
	if(res.Find("ZIP") >= 0)			userZIP				= res.Get("ZIP");
	if(res.Find("TOWN") >= 0)			userTown			= res.Get("TOWN");
	if(res.Find("ADDRESS") >= 0)		userAddress			= res.Get("ADDRESS");
	if(res.Find("PHONE") >= 0)			userPhone			= res.Get("PHONE");
	if(res.Find("FAX") >= 0)			userFax				= res.Get("FAX");
	if(res.Find("CELL") >= 0)			userCell			= res.Get("CELL");
	if(res.Find("EXPIRATION") >= 0)		expireTime			= res.Get("EXPIRATION");
	if(res.Find("NUMLICENSES") >= 0)	numLicenses			= res.Get("NUMLICENSES");
	if(res.Find("ALLOWEDVERSION") >=0)	maxAllowedVersion	= res.Get("ALLOWEDVERSION");

	return true;
}

// updates user data on server
bool ProtectClient::UpdateUserData(void)
{
	lastError = 0;

	// sends a register packet to server
	VectorMap<String, Value>v;
	v.Add("REASON", PROTECT_UPDATEUSERDATA);
	v.Add("CLIENTID", (int)clientID);
	v.Add("EMAIL", userEMail);
	v.Add("NAME", userName);
	v.Add("SURNAME", userSurname);
	v.Add("ZONE", userZone);
	v.Add("COUNTRY", userCountry);
	v.Add("ZIP", userZIP);
	v.Add("TOWN", userTown);
	v.Add("ADDRESS", userAddress);
	v.Add("PHONE", userPhone);
	v.Add("FAX", userFax);
	v.Add("CELL", userCell);
	VectorMap<String, Value> res = SendMap(v);

	// check for errors
	if(res.Find("ERROR") >= 0)
	{
		lastError = res.Get("ERROR");
		return false;
	}
	return true;
}
		
// register app
bool ProtectClient::Register(void)
{
	lastError = 0;

	// sends a register packet to server
	VectorMap<String, Value>v;
	v.Add("REASON", PROTECT_REGISTER);
	v.Add("EMAIL", userEMail);
	v.Add("NAME", userName);
	v.Add("SURNAME", userSurname);
	v.Add("ZONE", userZone);
	v.Add("COUNTRY", userCountry);
	v.Add("ZIP", userZIP);
	v.Add("TOWN", userTown);
	v.Add("ADDRESS", userAddress);
	v.Add("PHONE", userPhone);
	v.Add("FAX", userFax);
	v.Add("CELL", userCell);
	VectorMap<String, Value> res = SendMap(v);

	// check for errors
	if(res.Find("ERROR") >= 0)
	{
		lastError = res.Get("ERROR");
		return false;
	}
	return true;
}

END_UPP_NAMESPACE
