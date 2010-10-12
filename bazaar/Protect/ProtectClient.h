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
		
		// last error code for failed operations
		int lastError;
		
		// the cypher -- can be changed between available
		// in Cypher package
		One<Cypher>cypher;
	
		// client id -- got from server upon connection
		dword clientID;
		
		// user data
		String userEMail;
		String userName;
		String userAddress;
		String userCountry;
		String userZIP;
		String userPhone;
		String userFax;
		String userCell;
		
		// license expire time and number of them
		Time expireTime;
		int numLicenses;
		
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

		// read last error code and message
		int GetLastError(void) { return lastError; }
		String GetLastErrorMsg(void) { return ProtectMessage(lastError); }
		
		// create a persistent link to server
		bool Connect(void);

		// disconnect from server
		bool Disconnect(void);
		
		// refresh server connection
		bool Refresh(void);
		
		// get license key
		String GetKey(void);
		
		// gets license info
		bool GetLicenseInfo(void);
		
		// register app
		bool Register(void);
	
		// set user data -- for registration
		// filled automatically by GETLICENSEINFO request
		ProtectClient &SetUserEMail(String const &mail)			{ userEMail = mail; return *this; }
		ProtectClient &SetUserName(String const &name)			{ userName = name; return *this; }
		ProtectClient &SetUserAddress(String const &address)	{ userAddress = address; return *this; }
		ProtectClient &SetUserCountry(String const &country)	{ userCountry = country; return *this; }
		ProtectClient &SetUserZip(String const &zip)			{ userZIP = zip; return *this; }
		ProtectClient &SetUserPhone(String const &phone)		{ userPhone = phone; return *this; }
		ProtectClient &SetUserFax(String const &fax)			{ userFax = fax; return *this; }
		ProtectClient &SetUserCell(String const &cell)			{ userCell = cell; return *this; }
};

END_UPP_NAMESPACE

#endif
