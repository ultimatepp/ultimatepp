#ifndef _ProtectServer_h_
#define _ProtectServer_h_

#include <Core/Core.h>
#include <Core/SMTP/SMTP.h>

#include <Cypher/Cypher.h>
#include <Scgi/Scgi.h>

#include <ProtectClient/ProtectStatus.h>
#include "ProtectDB.h"

NAMESPACE_UPP

class ProtectServer : public Scgi
{
	private:
	
		struct TimeMail : Moveable<TimeMail>
		{
			Time time;
			String eMail;
			bool connected;
			TimeMail(Time const &t, String const &m, bool conn) : time(t), eMail(m) { connected = conn; }
			TimeMail() { connected = false; }
		};
	
		// database for user auth
		ProtectDB db;
		
		// mailer for activation mails
		Smtp smtp;
		
		// last ID cleanup time
		Time lastIDCleanupTime;
		
		// last database cleanup time
		Time lastDBCleanupTime;
	
		// clients id timeout value -- defaults to 5 minutes (300 seconds)
		int idCleanupTime;
		
		// database cleanup time -- default to 30 days (2592000 seconds)
		// used to clean up registrations with no activation on database
		int dbCleanupTime;
	
		// list of all connected clients with their expiration dates and emails
		VectorMap<dword, TimeMail> clientLinks;
		
		// number of succesful connections per registered email
		VectorMap<String, int> registeredConnections;
		
		// encryption engine
		One<Cypher> cypher;
	
		// key used to en/decrypt http data
		String communicationKey;
		
		// smtp resets the From field on each New()
		// so we shall store it here :-(
		String fromMail;
		String fromName;
		
		// welcome message sent on key activation e-mail
		// a %ACTIVATIONKEY% string inside body will be
		// replaced by current activation key
		String welcomeSubject;
		String welcomeBody;
	
		void OnAccepted();
		void OnRequest();
		void OnClosed();
		
		// generates an unique activation key composed by 20 random uppercase chars
		String GenerateActivationKey(void);
		
		// executes database cleanup
		void DBCleanup(void);
		
		// polls clients and remove those which connection has expired
		void CheckExpiredClients(void);
		
		// checks client list to see if a client is connected
		// as a side effect, refresh its connection time
		bool IsClientConnected(dword clientID, String const &eMail);
		
		// connects a client -- return an ID
		dword ConnectClient(String const &eMail, dword id);
		
		// disconnects a client or refresh its expiration time
		void DisconnectClient(dword clientID);
		
		// process client request
		// takes a VectorMap<String, Value> on input from client
		// produces a response VectorMap<String, Value> to be returned
		virtual VectorMap<String, Value> ProcessRequest(int reason, VectorMap<String, Value> const &v);
		
		// Application key to be returned on auth success
		String appKey;
		
		// sends activation mail to user
		bool SendActivationMail(VectorMap<String, Value> const &userData, String const &locale);

		// MAC recording enabling
		// records ALL user's network adapter MACs on each access
		bool recordMACs;
		
		// check if user want to register for demo with an already
		// used MAC
		bool checkExistingMAC;
		
		// enforce MAC data from client -- to be used in transition
		// phases when some clients still don't send MAC data
		// defaults to TRUE, set to FALSE in transition time
		bool enforceMAC;
		
		// alert by email if user want to register for demo with an
		// already used MAC
		String alertMACEMail;
		bool SendMACAlert(String const &eMail, Vector<String> const &mails);
		
		// process MAC list in data field
		// if 'registering' is true, returns false if MAC already there
		bool ProcessMACList(String const &mail, String const &MACList, bool registering = false);

public:

	// constructor - defaults to port 8787
	ProtectServer(int port = 8787);
	
	// setup encryption cypher
	// WARNING : takes ownership of it
	ProtectServer &SetCypher(Cypher *c) { cypher = c; return *this;  }
	
	// sets encryption key
	ProtectServer &SetCommunicationKey(String const &_key) { communicationKey = _key; return *this; }
	
	// sets welcome (HTML) message
	ProtectServer &SetWelcomeSubject(String const &w) { welcomeSubject = w; return *this; }
	ProtectServer &SetWelcomeBody(String const &w) { welcomeBody = w; return *this; }
	
	// gets database
	ProtectDB &GetDB(void) { return db; }
	
	// gets mailer
	Smtp &GetSmtp(void) { return smtp; }
	
	// sets smtp 'From' fields
	ProtectServer &From(String const &mail, String const &name = Null) { fromMail = mail; fromName = name; return *this; }
	
	// sets application key (to be returned on auth success)
	ProtectServer &SetAppKey(String const &k) { appKey = k; return *this; }
	String GetAppKey(void) { return appKey; }
	
	// sets client ids timeout value
	ProtectServer &SetIDCleanupTime(int t);
		
	// sets database cleanup time
	// used to clean up registrations with no activation on database
	ProtectServer &SetDBCleanupTime(int t);
	
	// MAC handling
	ProtectServer &SetRecordMACs(bool rec = true);
	ProtectServer &SetNoRecordMACs(void) { return SetRecordMACs(false); }
	
	ProtectServer &SetCheckExistingMAC(double check = true);
	ProtectServer &SetNoCheckExistingMAC(void) { return SetCheckExistingMAC(false); }
	
	ProtectServer &SetMACAlertEMail(String const &m);
	ProtectServer &ClearMACAlertEMail(void);
	
	ProtectServer &SetEnforceMAC(bool e);
	ProtectServer &SetNoEnforceMAC(void) { return SetEnforceMAC(false); }

	// runs the server
	void Run(void);
};

END_UPP_NAMESPACE

#endif
