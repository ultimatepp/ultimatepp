#ifndef _ProtectClient_h_
#define _ProtectClient_h_

#include <Cypher/Cypher.h>

#include<ProductVersion/ProductVersion.h>

#include "ProtectStatus.h"

NAMESPACE_UPP

class ProtectClient
{
	private:
	
		// the HTTP client
		HttpRequest client;
		
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
		String userSurname;
		String userZone;
		String userCountry;
		String userZIP;
		String userTown;
		String userAddress;
		String userPhone;
		String userFax;
		String userCell;
		
		// license expire time and number of them
		Time expireTime;
		int numLicenses;
		
		// activation key -- given once from server
		// upon activation request on this machine
		String activationKey;
		
		// version of current app installed
		ProductVersion currentVersion;
		
		// maximum allowable version for this client
		ProductVersion maxAllowedVersion;
		
		// key for encrypted communication
		String communicationKey;
	
		// user configuration path
		String userConfigPath;
		
		// flag indicating that config has been loaded
		// (we must delay loda because of settable userConfigPath)
		bool configLoaded;
		
		// get MAC of client adapter(s), each separated by comma
		String GetMACs(void);
		
	protected:
	
		// sends a VectorMap to server in encrypted form
		// and gets its response also in VectorMap form
		VectorMap<String, Value> SendMap(VectorMap<String, Value> const &v);

	public:
		
		// constructor
		ProtectClient();
		
		// destructor
		~ProtectClient();
		
		// stores/retrieve config data from stream
		void Xmlize(XmlIO xml);
		bool StoreConfig(void);
		bool LoadConfig(void);
		
		// sets user configuration path
		ProtectClient &SetUserConfigPath(String const &path) { userConfigPath = path; return *this; }
		
		// sets the encrypting engine
		// default Cypher at startup is Snow2
		// WARNING -- takes Cypher ownership
		void SetCypher(Cypher *c);
		
		// sets server's URL
		ProtectClient &SetURL(String const &url) { client.Url(url); return *this; }
		
		// sets communication key
		ProtectClient &SetCommunicationKey(String const &_key) { communicationKey = _key; return *this; }

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
		String GetLicenseKey(void);
		
		// gets license info
		bool GetLicenseInfo(void);
		
		// updates user data on server
		bool UpdateUserData(void);
		
		// register app
		bool Register(void);
	
		// set user data -- for registration
		// filled automatically by GETLICENSEINFO request
		ProtectClient &SetUserEMail(String const &mail)				{ userEMail = mail; return *this; }
		ProtectClient &SetUserName(String const &name)				{ userName = name; return *this; }
		ProtectClient &SetUserSurname(String const &surname)		{ userSurname = surname; return *this; }
		ProtectClient &SetUserZone(String zone)						{ userZone = zone; return *this; }
		ProtectClient &SetUserCountry(String const &country)		{ userCountry = country; return *this; }
		ProtectClient &SetUserZip(String const &zip)				{ userZIP = zip; return *this; }
		ProtectClient &SetUserTown(String const &town)				{ userTown = town; return *this; }
		ProtectClient &SetUserAddress(String const &address)		{ userAddress = address; return *this; }
		ProtectClient &SetUserPhone(String const &phone)			{ userPhone = phone; return *this; }
		ProtectClient &SetUserFax(String const &fax)				{ userFax = fax; return *this; }
		ProtectClient &SetUserCell(String const &cell)				{ userCell = cell; return *this; }
		ProtectClient &SetCurrentVersion(ProductVersion const &v)	{ currentVersion = v; return *this; }
		ProtectClient &SetActivationKey(String const &key)			{ activationKey = key; StoreConfig(); return *this; }

		String GetUserEMail(void)					{ return userEMail; }
		String GetUserName(void)					{ return userName; }
		String GetUserSurname(void)					{ return userSurname; }
		String GetUserZone(void)					{ return userZone; }
		String GetUserCountry(void)					{ return userCountry; }
		String GetUserZip(void)						{ return userZIP; }
		String GetUserTown(void)					{ return userTown; }
		String GetUserAddress(void)					{ return userAddress; }
		String GetUserPhone(void)					{ return userPhone; }
		String GetUserFax(void)						{ return userFax; }
		String GetUserCell(void)					{ return userCell; }
		ProductVersion GetCurrentVersion(void)		{ return currentVersion; }
		ProductVersion GetMaxAllowedVersion(void)	{ return maxAllowedVersion; }
		String GetActivationKey(void)				{ LoadConfig(); return activationKey; }
		Time GetExpireTime(void)					{ return expireTime; }
};

END_UPP_NAMESPACE

#endif
