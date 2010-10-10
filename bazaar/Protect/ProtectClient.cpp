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
	dataMap.Add("CLIENTID=", clientID);
	
	// sets cypher key (and create a random IV)
	cypher->SetKey(key);
	
	String postData;
	postData += "IV=" + HexString(cypher->GetNonce()) + "&";
	postData += "DATA=";
	postData += HexString((*cypher)(StoreAsXML(dataMap, "ProtectClient")));

	client.Post(postData);
	lastContents = client.ExecuteRedirect();

	// if contents start with "ERROR", just fetch the error desc
	// and put it in result map
	VectorMap<String, Value> resMap;
	if(lastContents.StartsWith("ERROR="))
	{
		int i = lastContents.Find("\r");
		String s;
		if(i >= 0)
			s = lastContents.Mid(6, i - 6);
		else
			s = lastContents.Mid(6);
		resMap.Add("ERROR", s);
		return resMap;
	}
	// otherwise, if lastContents don't start with IV field
	// fetch the error from http client
	if(!lastContents.StartsWith("IV="))
	{
		resMap.Add("ERROR", client.GetStatusLine());
		return resMap;
	}
	
	// well, we've at least the IV, so we can fetch it and DATA field and decrypt
	StringStream s(lastContents);
	String line = s.GetLine();
	String IV = ScanHexString(line.Mid(3));
	line = s.GetLine();
	if(!line.StartsWith("DATA="))
	{
		resMap.Add("ERROR", "Missing DATA field");
		return resMap;
	}
	
	// decodes DATA field and read VectorMap from it
	cypher->SetKey(key, IV);
	String decoded = (*cypher)(line.Mid(5));
	try
	{
		LoadFromXML(resMap, decoded);
	}
	catch(...)
	{
		resMap.Clear();
		resMap.Add("ERROR", "Bad DATA received");
	}
	return resMap;
}

// create a persistent link to server
bool ProtectClient::Connect(int persistTime)
{
	// if already connected, disconnect first
	Disconnect();
	
	// send a connect packet to server
	VectorMap<String, Value>v;
	v.Add("CONNECT", persistTime);
	VectorMap<String, Value> res = SendMap(v);
	
	// check for errors
	if(res.Find("CONNECTED") < 0)
		return false;
	
	connected = true;
	return true;
}

// disconnect from server
bool ProtectClient::Disconnect(void)
{
	if(!connected)
		return true;
	
	// sends a disconnect packet to server
	VectorMap<String, Value>v;
	v.Add("DISCONNECT", "");
	VectorMap<String, Value> res = SendMap(v);

	// check for errors
	if(res.Find("DISCONNECTED") < 0)
		return false;
	
	connected = false;
	return true;
}

// sends data and get response data
VectorMap<String, Value> ProtectClient::Send(VectorMap<String, Value> const &v)
{
	VectorMap<String, Value> res;
	
	// checks if connection is established
	if(!connected)
	{
		res.Add("ERROR", "NOT CONNECTED TO SERVER");
		return res;
	}
	
	// send the map and get results
	res = SendMap(v);
	
	// return result map
	return res;
}

END_UPP_NAMESPACE
