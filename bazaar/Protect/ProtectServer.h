#ifndef _ProtectServer_h_
#define _ProtectServer_h_

#include <Core/Core.h>
#include <ScgiServer/ScgiServer.h>
#include <Cypher/Cypher.h>

#include "ProtectStatus.h"

NAMESPACE_UPP

class ProtectServer : public ScgiServer
{
	private:
	
		// clients timeout value -- defaults to 5 minutes (300 seconds)
		int clientTimeout;
	
		// list of all connected clients with their expiration dates
		VectorMap<String, Time> clients;
		
		// encryption engine
		One<Cypher> cypher;
	
		// key used to en/decrypt http data
		String key;
	
		void OnAccepted();
		void OnRequest();
		void OnClosed();
		
		// polls clients and remove those which connection has expired
		void CheckExpiredClients(void);
		
		// checks client list to see if a client is connected
		// as a side effect, refresh its connection time
		bool IsClientConnected(String const &clientID);
		
		// connects a client or refresh its expiration time
		void ConnectClient(String const &clientID);
		
		// disconnects a client or refresh its expiration time
		void DisconnectClient(String const &clientID);
		
		// sends error message
		void SendError(int msg);
		
		// process client request
		// takes a VectorMap<String, Value> on input from client
		// produces a response VectorMap<String, Value> to be returned
		virtual VectorMap<String, Value> ProcessRequest(VectorMap<String, Value> const &v);

public:

	// constructor - defaults to port 8787 and connection timeout of 5 minutes (300 sec.)
	ProtectServer(int port = 8787, int timeout = 300);
	
	// setup encryption cypher
	// WARNING : takes ownership of it
	ProtectServer &SetCypher(Cypher *c) { cypher = c; return *this;  }
	
	// sets encryption key
	ProtectServer &SetKey(String const &_key) { key = _key; return *this; }
};

END_UPP_NAMESPACE

#endif
