#ifndef _ProtectServer_h_
#define _ProtectServer_h_

#include <Core/Core.h>
#include <Web/Web.h>
#include <Cypher/Cypher.h>

#include "ProtectStatus.h"
#include "ProtectDB.h"

NAMESPACE_UPP

class ProtectServer : public ScgiServer
{
	private:
	
		struct TimeMail : Moveable<TimeMail>
		{
			Time time;
			String eMail;
			TimeMail(Time const &t, String const &m) : time(t), eMail(m) {};
			TimeMail() {};
		};
	
		// database for user auth
		ProtectDB db;
		
		// mailer for activation mails
		SmtpMail smtp;
	
		// clients timeout value -- defaults to 5 minutes (300 seconds)
		int clientTimeout;
	
		// list of all connected clients with their expiration dates and emails
		VectorMap<dword, TimeMail> clientLinks;
		
		// number of succesful connections per registered email
		VectorMap<String, int> registeredConnections;
		
		// encryption engine
		One<Cypher> cypher;
	
		// key used to en/decrypt http data
		String key;
		
		// welcome and activation failed messages sent on key activation
		String welcome;
		String activationFailed;
	
		void OnAccepted();
		void OnRequest();
		void OnClosed();
		
		// polls clients and remove those which connection has expired
		void CheckExpiredClients(void);
		
		// checks client list to see if a client is connected
		// as a side effect, refresh its connection time
		bool IsClientConnected(dword clientID);
		
		// connects a client -- return an ID
		dword ConnectClient(String const &eMail);
		
		// disconnects a client or refresh its expiration time
		void DisconnectClient(dword clientID);
		
		// process client request
		// takes a VectorMap<String, Value> on input from client
		// produces a response VectorMap<String, Value> to be returned
		virtual VectorMap<String, Value> ProcessRequest(int reason, VectorMap<String, Value> const &v);
		
		// Application key to be returned on auth success
		String appKey;
		
		// sends activation mail to user
		bool SendActivationMail(VectorMap<String, Value> const &userData);

public:

	// constructor - defaults to port 8787 and connection timeout of 5 minutes (300 sec.)
	ProtectServer(int port = 8787, int timeout = 300);
	
	// setup encryption cypher
	// WARNING : takes ownership of it
	ProtectServer &SetCypher(Cypher *c) { cypher = c; return *this;  }
	
	// sets encryption key
	ProtectServer &SetKey(String const &_key) { key = _key; return *this; }
	
	// sets welcome and activation failed (HTML) messages
	ProtectServer &SetWelcome(String const &w) { welcome = w; return *this; }
	ProtectServer &SetActivationFailed(String const &a) { activationFailed = a; return *this; }
	
	// gets database
	ProtectDB &GetDB(void) { return db; }
	
	// gets mailer
	SmtpMail &GetSmtp(void) { return smtp; }
	
	// sets application key (to be returned on auth success)
	ProtectServer &SetAppKey(String const &k) { appKey = k; return *this; }
	String GetAppKey(void) { return appKey; }
	
	// runs the server
	void Run(void);
};

END_UPP_NAMESPACE

#endif
