#include "ProtectClient.h"

NAMESPACE_UPP

ProtectClient::ProtectClient()
{
	// unconnected at startup
	connected = false;
	
	// generates client id
	dword rnd = Random();
	clientID = HexString((byte const *)&rnd, sizeof(dword));
	
	// defaults to Snow2 Cypher
	cypher = new Snow2;
}

ProtectClient::~ProtectClient()
{
	// if connected, disconnect from server
	Disconnect();
}

// sets the encrypting engine
// default Cypher at startup is Snow2
// WARNING -- takes Cypher ownership
void ProtectClient::SetCypher(Cypher *c)
{
	// sets the new cypher freeing previous one
	cypher = c;
	
}
		
// sends a VectorMap to server in encrypted form
// and gets its response
VectorMap<String, Value> ProtectClient::SendMap(VectorMap<String, Value> const &v)
{
	// copy vectormap adding client id and a magic number
	VectorMap<String, Value>dataMap(v, 1);
	dataMap.Add("APPID", "ProtectClient");
	dataMap.Add("CLIENTID", clientID);
	
	// sets cypher key (and create a random IV)
	cypher->SetKey(key);
	
	String postData;
	postData += "IV=" + HexString(cypher->GetNonce()) + "&";
	postData += "DATA=";
	postData += HexString((*cypher)(StoreAsXML(dataMap, "ProtectClient")));

	client.Post(postData);
	String contents = client.ExecuteRedirect();

	// if contents start with "ERROR", just fetch the error desc
	// and put it in result map
	VectorMap<String, Value> resMap;
	if(contents.StartsWith("ERROR="))
	{
		int i = contents.Find("\r");
		String s;
		if(i >= 0)
			s = contents.Mid(6, i - 6);
		else
			s = contents.Mid(6);
		int errCode = atoi(~s);
		resMap.Add("ERROR", errCode);
		resMap.Add("ERRORMSG", ProtectMessage(errCode));
		return resMap;
	}
	// otherwise, if lastContents don't start with IV field
	// signals it
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
	cypher->SetKey(key, IV);
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
	
	// if already connected, disconnect first
	if(!Disconnect())
		return false;
	
	// send a connect packet to server
	VectorMap<String, Value>v;
	v.Add("REASON", ProtectReasonStr(PROTECT_CONNECT));
	VectorMap<String, Value> res = SendMap(v);
	
	// check for errors
	if(res.Find("ERROR") >= 0)
	{
		lastError = res.Get("ERROR");
		return false;
	}
	
	connected = true;
	return true;
}

// disconnect from server
bool ProtectClient::Disconnect(void)
{
	lastError = 0;
	
	if(!connected)
		return true;
	
	// sends a disconnect packet to server
	VectorMap<String, Value>v;
	v.Add("REASON", ProtectReasonStr(PROTECT_DISCONNECT));
	VectorMap<String, Value> res = SendMap(v);

	// check for errors
	if(res.Find("ERROR") >= 0)
	{
		lastError = res.Get("ERROR");
		return false;
	}
	
	connected = false;
	return true;
}

// refresh server connection
bool ProtectClient::Refresh(void)
{
	lastError = 0;

	// sends a refresh packet to server
	VectorMap<String, Value>v;
	v.Add("REASON", ProtectReasonStr(PROTECT_REFRESH));
	VectorMap<String, Value> res = SendMap(v);

	// check for errors
	if(res.Find("ERROR") >= 0)
	{
		lastError = res.Get("ERROR");
		return false;
	}
	
	return true;
}

// get license key
String ProtectClient::GetKey(void)
{
	lastError = 0;

	// sends a getkey packet to server
	VectorMap<String, Value>v;
	v.Add("REASON", ProtectReasonStr(PROTECT_GETKEY));
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
	v.Add("REASON", ProtectReasonStr(PROTECT_GETLICENSEINFO));
	VectorMap<String, Value> res = SendMap(v);

	// check for errors
	if(res.Find("ERROR") >= 0)
	{
		lastError = res.Get("ERROR");
		return false;
	}
	
	if(res.Find("EMAIL") >= 0)			userEMail	= res.Get("USEREMAIL");
	if(res.Find("USERNAME") >= 0)		userName	= res.Get("USERNAME");
	if(res.Find("USERADDRESS") >= 0)	userAddress	= res.Get("USERADDRESS");
	if(res.Find("USERCOUNTRY") >= 0)	userCountry	= res.Get("USERCOUNTRY");
	if(res.Find("USERPHONE") >= 0)		userPhone	= res.Get("USERPHONE");
	if(res.Find("USERFAX") >= 0)		userFax		= res.Get("USERFAX");
	if(res.Find("USERCELL") >= 0)		userCell	= res.Get("USERCELL");
	if(res.Find("EXPIRETIME") >= 0)		expireTime	= res.Get("EXPIRETIME");
	if(res.Find("NUMLICENSES") >= 0)	numLicenses	= res.Get("NUMLICENSES");

	return true;
}

// register app
bool ProtectClient::Register(void)
{
	lastError = 0;

	// sends a register packet to server
	VectorMap<String, Value>v;
	v.Add("REASON", ProtectReasonStr(PROTECT_REGISTER));
	v.Add("EMAIL", userEMail);
	v.Add("USERNAME", userName);
	v.Add("USERADDRESS", userAddress);
	v.Add("USERCOUNTRY", userCountry);
	v.Add("USERPHONE", userPhone);
	v.Add("USERFAX", userFax);
	v.Add("USERCELL", userCell);
	VectorMap<String, Value> res = SendMap(v);

	// check for errors
	if(res.Find("ERROR") >= 0)
	{
		lastError = res.Get("ERROR");
		return false;
	}
}

END_UPP_NAMESPACE
