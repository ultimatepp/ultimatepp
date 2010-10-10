#ifndef _ProtectClient_h_
#define _ProtectClient_h_

#include <Web/Web.h>
#include <Cypher/Cypher.h>

#include "ProtectStatus.h"

NAMESPACE_UPP

class ProtectClient
{
	private:
	
		// the HTTP client
		HttpClient client;
		
		// last HTTP answer
		String lastContents;
		
		// the cypher -- can be changed between available
		// in Cypher package
		One<Cypher>cypher;
	
		// flag stating a succesfull connection
		bool connected;
	
		// client id -- generated as random dword on creation
		String clientID;
		
		// key and IV for encrypted communication
		String key;
		qword IV;
	
	protected:
	
		// sends a VectorMap to server in encrypted form
		// and gets its response also in VectorMap form
		VectorMap<String, Value> SendMap(VectorMap<String, Value> const &v);

	public:
		
		// constructor
		ProtectClient();
		
		// destructor
		~ProtectClient();
		
		// sets the encrypting engine
		// default Cypher at startup is Snow2
		// WARNING -- takes Cypher ownership
		void SetCypher(Cypher *c);
		
		// sets server's URL
		ProtectClient &SetURL(String const &url) { client.URL(url); return *this; }
		
		// sets communication key
		ProtectClient &SetKey(String const &_key) { key = _key; return *this; }

		// read last HTTP answer got from server
		// useful mostly for debugging purposes
		String const &GetLastContents(void) { return lastContents; }
		String GetLastHeaders(void) { return client.GetHeaders(); }
		String GetLastStatus(void) { return client.GetStatusLine(); }
		String GetLastError(void) { return client.GetError(); }
		int GetLastStatusCode(void) { return client.GetStatusCode(); }
		
		// create a persistent link to server
		bool Connect(int persistTime);

		// disconnect from server
		bool Disconnect(void);
	
		// sends data and get response data
		VectorMap<String, Value> Send(VectorMap<String, Value> const &v);
};

END_UPP_NAMESPACE

#endif
