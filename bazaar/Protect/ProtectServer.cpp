#include "ProtectServer.h"

NAMESPACE_UPP

// constructor - defaults to port 8787 and connection timeout of 5 minutes (300 sec.)
ProtectServer::ProtectServer(int port, int timeout) : ScgiServer(port)
{
	// setup default cypher
	cypher = new Snow2;
	
	// client timeout defaults to 300 seconds (5 minutes)
	clientTimeout = timeout;
}

void ProtectServer::SendError(int msg)
{
	clientSock.Write("ERROR=" + FormatInt(msg) + "\r\n");
}

// polls clients and remove those which connection has expired
void ProtectServer::CheckExpiredClients(void)
{
	int64 curTime = GetSysTime().Get();
	for(int iClient = clients.GetCount() - 1; iClient >= 0; iClient--)
		if(clients[iClient].Get() < curTime)
			clients.Remove(iClient);
}
		
// checks client list to see if a client is connected
bool ProtectServer::IsClientConnected(String const &clientID)
{
	int iClient = clients.Find(clientID);
	if(iClient >= 0)
	{
		clients[iClient] = GetSysTime() + clientTimeout;
		return true;
	}
	return false;
}

// connects a client or refresh its expiration time
void ProtectServer::ConnectClient(String const &clientID)
{
	int i = clients.FindAdd(clientID);
	clients[i] = GetSysTime() + clientTimeout;
}
		
// disconnects a client or refresh its expiration time
void ProtectServer::DisconnectClient(String const &clientID)
{
	int i = clients.Find(clientID);
	if(i >= 0)
		clients.Remove(i);
}
		

void ProtectServer::OnAccepted()
{
	Cout() << "Accepted connection from client " << FormatIP(clientIP) << "\n";
}

void ProtectServer::OnRequest()
{
	clientSock.Write("Content-Type: text/plain\r\n\r\n");
	
	// ignore requests without post data
	if(!HasPostData())
	{
		SendError(PROTECT_BAD_REQUEST);
		return;
	}
	
	// post data should contain at least an IV field and a DATA field
	if(post.Find("IV") < 0 || post.Find("DATA") < 0)
	{
		SendError(PROTECT_MISSING_IV);
		return;
	}

	// gets IV and setup the cypher to decrypt DATA field
	String IV = ScanHexString(post.GetString("IV"));
	cypher->SetKey(key, IV);
	
	// decrypts DATA field and build its vectormap
	VectorMap<String, Value> data;
	String decoded;
	try
	{
		decoded = (*cypher)(ScanHexString(post.GetString("DATA")));
		LoadFromXML(data, decoded);
	}
	catch(...)
	{
		SendError(PROTECT_BAD_DATA);
		return;
	}
		
	// Get request reason and process it
	// supports following reasons :
	//		CONNECT			establish connection to server
	//		DISCONNECT		frees server connection
	//		REFRESH			refreshes server connection (to restart timeout)
	//		GETKEY			gets application key
	//		REGISTER		registers app for timed demo
	//		GETLICENSEINFO	gets info about license (name, expiration date, app version....)
	VectorMap<String, Value> results = ProcessRequest(data);

	// encodes results and send back to client
	cypher->SetKey(key);
	clientSock.Write("IV=" + HexString(cypher->GetNonce()) + "\r\n");
	String encoded = HexString((*cypher)(StoreAsXML(results, "ProtectServer")));
	clientSock.Write("DATA=" + encoded + "\r\n");
}

void ProtectServer::OnClosed()
{
	Cout() << "Connection with " << FormatIP(clientIP) << " closed\n";
}

// process client request
// takes a VectorMap<String, Value> on input from client
// produces a response VectorMap<String, Value> to be returned
VectorMap<String, Value> ProtectServer::ProcessRequest(VectorMap<String, Value> const &v)
{
	// @@@@ TO DO - BY NOW JUST RETURN INPUT DATA
	return VectorMap<String, Value>(v, 1);
}

END_UPP_NAMESPACE
