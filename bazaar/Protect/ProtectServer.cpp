#include "ProtectServer.h"

NAMESPACE_UPP

// email validation helper
bool ValidateEMail(const char *address)
{
	int					count = 0;
	const char			*c, *domain;
	static const char	*rfc822_specials = "()<>@,;:\\\"[]";
	
	// first we validate the name portion (name@domain)
	for (c = address;  *c;  c++)
	{
		if (*c == '\"' && (c == address || *(c - 1) == '.' || *(c - 1) == '\"'))
		{
			while (*++c)
			{
				if (*c == '\"')
					break;
				if (*c == '\\' && (*++c == ' '))
					continue;
				if (*c < ' ' || *c >= 127)
					return false;
			}
			if (!*c++)
				return false;
			if (*c == '@')
				break;
			if (*c != '.')
				return false;
			continue;
		}
		if (*c == '@')
			break;
		if (*c <= ' ' || *c >= 127)
			return false;
		if (strchr(rfc822_specials, *c))
			return false;
	}
	if (c == address || *(c - 1) == '.')
		return false;
	
	// next we validate the domain portion (name@domain)
	if (!*(domain = ++c))
		return false;
	do
	{
		if (*c == '.')
		{
			if (c == domain || *(c - 1) == '.')
				return false;
			count++;
		}
		if (*c <= ' ' || *c >= 127)
			return false;
		if (strchr(rfc822_specials, *c))
			return false;
	}
	while (*++c);
	return (count >= 1);
}

// constructor - defaults to port 8787 and connection timeout of 5 minutes (300 sec.)
ProtectServer::ProtectServer(int port, int timeout) : ScgiServer(port)
{
	// setup default cypher
	cypher = new Snow2;
	
	// client timeout defaults to 300 seconds (5 minutes)
	clientTimeout = timeout;
}

// polls clients and remove those which connection has expired
void ProtectServer::CheckExpiredClients(void)
{
	Time curTime = GetSysTime();
	for(int iClient = clientLinks.GetCount() - 1; iClient >= 0; iClient--)
		if(clientLinks[iClient].time < curTime)
		{
			String eMail = clientLinks[iClient].eMail;
			clientLinks.Remove(iClient);
			registeredConnections.Get(eMail)--;
			if(registeredConnections.Get(eMail) <= 0)
				registeredConnections.RemoveKey(eMail);
		}
}
		
// checks client list to see if a client is connected
// and refresh its expire time on the way
bool ProtectServer::IsClientConnected(dword clientID)
{
	int iClient = clientLinks.Find(clientID);
	if(iClient >= 0)
	{
		clientLinks[iClient].time = GetSysTime() + clientTimeout;
		return true;
	}
	return false;
}

// connects a client -- returns a new clientID
// doesn't check anything, that one must be done elsewhere
dword ProtectServer::ConnectClient(String const &eMail)
{
	// generates an unique random clientID
	dword id;
	do
	{
		id = Random();
	}
	while(id == 0 || id == -1 || clientLinks.Find(id) >= 0);
	
	// adds to clientLinks and to registered emails count
	clientLinks.Add(id, TimeMail(GetSysTime() + clientTimeout, eMail));
	int i = registeredConnections.Find(eMail);
	if(i >= 0)
		registeredConnections[i]++;
	else
		registeredConnections.Add(eMail, 1);
	
	return id;
}
		
// disconnects a client or refresh its expiration time
void ProtectServer::DisconnectClient(dword clientID)
{
	int i = clientLinks.Find(clientID);
	if(i >= 0)
	{
		String eMail = clientLinks[i].eMail;
		clientLinks.Remove(i);
		i = registeredConnections.Find(eMail);
		if(i >= 0)
		{
			registeredConnections[i]--;
			if(registeredConnections[i] <= 0)
				registeredConnections.Remove(i);
		}
	}
}
		

void ProtectServer::OnAccepted()
{
	Cout() << "Accepted connection from client " << FormatIP(clientIP) << "\n";
}

void ProtectServer::OnRequest()
{
	int err = 0;
	VectorMap<String, Value> data;
	VectorMap<String, Value> results;
	String IV;
	
	// POST rwquest ?
	if(HasPostData())
	{
		// all requests besides PROTECT_ACTIVATE go through post
		if(post.Find("IV") < 0)
			err = PROTECT_MISSING_IV;
		else
		{
			IV = ScanHexString(post.GetString("IV"));
			if(post.Find("DATA") < 0)
				err = PROTECT_MISSING_DATA;
			else
			{
				cypher->SetKey(key, IV);
				String decoded = (*cypher)(ScanHexString(post.GetString("DATA")));
				LoadFromXML(data, decoded);
			}
		}
	}
	// GET rwquest ?
	else if(query.GetCount())
	{
		// here should come just PROTECT_ACTIVATE requests
		if(query.Find("IV") < 0)
			err = PROTECT_MISSING_IV;
		else
		{
			IV = ScanHexString(query.GetString("IV"));
			if(query.Find("DATA") < 0)
				err = PROTECT_MISSING_DATA;
			else
			{
				cypher->SetKey(key, IV);
				String decoded = (*cypher)(ScanHexString(query.GetString("DATA")));
				LoadFromXML(data, decoded);
			}
		}
	}
	else
		err = PROTECT_BAD_REQUEST;


	// Get request reason and process it
	// supports following reasons :
	//		CONNECT			establish connection to server
	//		DISCONNECT		frees server connection
	//		REFRESH			refreshes server connection (to restart timeout)
	//		GETKEY			gets application key
	//		REGISTER		registers app for timed demo
	//		GETLICENSEINFO	gets info about license (name, expiration date, app version....)
	if(!err && data.Find("REASON") < 0)
		err = PROTECT_MISSING_REASON;
	if(!err)
		results = ProcessRequest(data.Get("REASON"), data);
	
	// if previous error, sets it into result packet
	if(err)
	{
		results.Add("ERROR", err);
		results.Add("ERRORMSG", ProtectMessage(err));
	}
	else if(results.Find("ERROR") >= 0)
		results.Add("ERRORMSG", ProtectMessage(results.Get("ERROR")));

	// if not ACTIVATION REQUEST encodes results and send back to client
	if(data.Get("REASON") != PROTECT_ACTIVATE)
	{
		clientSock.Write("Content-Type: text/plain\r\n\r\n");
		cypher->SetKey(key);
		clientSock.Write("IV=" + HexString(cypher->GetNonce()) + "\r\n");
		String encoded = HexString((*cypher)(StoreAsXML(results, "ProtectServer")));
		clientSock.Write("DATA=" + encoded + "\r\n");
	}
	// otherwise send a fancy welcome html -- replacing %NAME% with user name
	else
	{
		String w;
		clientSock.Write("Content-Type: text/html\r\n\r\n");
		if(results.Find("ERROR") < 0)
			w = welcome;
		else
			w = activationFailed;
		VectorMap<String, Value> userData = db.Get(data.Get("EMAIL"));
		if(userData.GetCount())
			w.Replace("%USER%", userData.Get("NAME"));
		else
			w.Replace("%USER%", data.Get("EMAIL"));
		clientSock.Write(w);
	}
}

void ProtectServer::OnClosed()
{
	Cout() << "Connection with " << FormatIP(clientIP) << " closed\n";
}

// process client request
// takes a VectorMap<String, Value> on input from client
// produces a response VectorMap<String, Value> to be returned
VectorMap<String, Value> ProtectServer::ProcessRequest(int reason, VectorMap<String, Value> const &v)
{
	VectorMap<String, Value> res;
	dword clientID;
	VectorMap<String, Value> userRec;
	String eMail;
	String activationKey;
	int numConnections;
	
	switch(reason)
	{
		case PROTECT_REGISTER:
			{
				// we need an eMail to register
				if(v.Find("EMAIL") < 0)
				{
					res.Add("ERROR", PROTECT_MISSING_EMAIL);
					return res;
				}
				eMail = v.Get("EMAIL");
				if(!ValidateEMail(~eMail))
				{
					res.Add("ERROR", PROTECT_INVALID_EMAIL);
					return res;
				}
				
				// avoid hacking of source packet stripping eventually
				// added licenseinfo, expiration, etc
				VectorMap<String, Value>vs(v, 1);
				vs.RemoveKey("LICENSES");
				vs.RemoveKey("EXPIRATION");
				vs.RemoveKey("ACTIVATIONKEY");
				vs.RemoveKey("ACTIVATIONSENT");
				vs.RemoveKey("ACTIVATED");
		
				// try to get user record from database, if any
				userRec = db.Get(eMail);
				
				// if not found, new registration
				if(!userRec.GetCount())
				{
					// new registration
					db.Set(vs);
					userRec = db.Get(eMail);
					if(SendActivationMail(userRec))
						res.Add("STATUS", "ACTIVATION RESENT");
					else
						res.Add("ERROR", PROTECT_MAIL_SEND_ERROR);
				}
				// otherwise, we shall check if activated
				else if(!userRec.Get("ACTIVATED"))
				{
					// already registered but still not activated
					// resend activation mail
					if(SendActivationMail(userRec))
						res.Add("STATUS", "ACTIVATION RESENT");
					else
						res.Add("ERROR", PROTECT_MAIL_SEND_ERROR);
				}
				// otherwise we shall check if license expired
				else if(userRec.Get("EXPIRATION") <= GetSysTime())
				{
					// signals expiration and leave
					res.Add("ERROR", PROTECT_MAIL_ALREADY_USED);
				}
				// otherwise, just tell user that's already registered
				else
				{
					res.Add("STATUS", "ALREADY REGISTERED");
				}
			}
			break;
		case PROTECT_ACTIVATE:
			// we need email to activate
			if(v.Find("EMAIL") < 0)
			{
				res.Add("ERROR", PROTECT_MISSING_EMAIL);
				return res;
			}
			eMail = v.Get("EMAIL");

			// we need ACTIVATIONKEY too
			if(v.Find("ACTIVATIONKEY") < 0)
			{
				res.Add("ERROR", PROTECT_MISSING_ACTIVATIONKEY);
				return res;
			}
			activationKey = v.Get("ACTIVATIONKEY");
			
			// get user record by email
			userRec = db.Get(eMail);

			// if no mail data, product is unregistered
			if(!userRec.GetCount())
			{
				res.Add("ERROR", PROTECT_UNREGISTERED);
				return res;
			}
			
			// sets registered flag and update record
			userRec.Get("ACTIVATED") = true;
			db.Set(userRec);
			
			res.Add("STATUS", "ACTIVATION SUCCESSFUL");
			break;
		case PROTECT_CONNECT:
			// we need email to connect
			if(v.Find("EMAIL") < 0)
			{
				res.Add("ERROR", PROTECT_MISSING_EMAIL);
				return res;
			}
			eMail = v.Get("EMAIL");
			// get the user data from database
			userRec = db.Get(eMail);

			// if no mail data, product is unregistered
			if(!userRec.GetCount())
			{
				res.Add("ERROR", PROTECT_UNREGISTERED);
				return res;
			}
			
			// product is registered, now we check if activation mail was sent
			if(!userRec.Get("ACTIVATED"))
			{
				res.Add("ERROR", PROTECT_LICENSE_NOT_ACTIVATED);
				SendActivationMail(userRec);
				return res;
			}
			
			// ok, product registered and activated... is license expired ?
			if(userRec.Get("EXPIRATION") < GetSysTime())
			{
				res.Add("ERROR", PROTECT_LICENSE_EXPIRED);
				return res;
			}
			
			// all ok, we shall finally check if number of licenses has overrun
			// first, we get number of current connections with same email
			numConnections = registeredConnections.Find(eMail);
			if(numConnections < 0)
				numConnections = 0;
			else
				numConnections = registeredConnections[numConnections];
			if(numConnections >= (int)userRec.Get("LICENSES"))
			{
				res.Add("ERROR", PROTECT_LICENSES_NUMBER_EXCEEDED);
				return res;
			}
			
			// all OK, connect client and return connection ID
			clientID = ConnectClient(eMail);
			res.Add("CLIENTID", (int)clientID);
			
			break;
			
		case PROTECT_DISCONNECT :
			// we need the conneciton ID
			if(v.Find("CLIENTID") < 0)
			{
				res.Add("ERROR", PROTECT_MISSING_CLIENTID);
				return res;
			}
			clientID = (int)v.Get("CLIENTID");
			DisconnectClient(clientID);
			break;
			
		case PROTECT_REFRESH :
			// we need the conneciton ID
			if(v.Find("CLIENTID") < 0)
			{
				res.Add("ERROR", PROTECT_MISSING_CLIENTID);
				return res;
			}
			clientID = (int)v.Get("CLIENTID");
			if(!IsClientConnected(clientID))
			{
				res.Add("ERROR", PROTECT_NOT_CONNECTED);
				return res;
			}
			break;
			
		case PROTECT_GETKEY:
			// we need the conneciton ID
			if(v.Find("CLIENTID") < 0)
			{
				res.Add("ERROR", PROTECT_MISSING_CLIENTID);
				return res;
			}
			// we shall be connected
			clientID = (int)v.Get("CLIENTID");
			if(!IsClientConnected(clientID))
			{
				res.Add("ERROR", PROTECT_NOT_CONNECTED);
				return res;
			}
			// all ok, return the key
			res.Add("KEY", appKey);
			break;
			
		case PROTECT_GETLICENSEINFO :
			// we need the conneciton ID
			if(v.Find("CLIENTID") < 0)
			{
				res.Add("ERROR", PROTECT_MISSING_CLIENTID);
				return res;
			}
			// we shall be connected
			clientID = (int)v.Get("CLIENTID");
			if(!IsClientConnected(clientID))
			{
				res.Add("ERROR", PROTECT_NOT_CONNECTED);
				return res;
			}
			// get user data from database
			res = db.Get(clientLinks.Get(clientID).eMail);
			// remove unnecessary data
			res.RemoveKey("ACTIVATIONKEY");
			res.RemoveKey("ACTIVATIONSENT");
			break;
			
		case PROTECT_UPDATEUSERDATA :
			{
				// we need the conneciton ID
				if(v.Find("CLIENTID") < 0)
				{
					res.Add("ERROR", PROTECT_MISSING_CLIENTID);
					return res;
				}
				// we shall be connected
				clientID = (int)v.Get("CLIENTID");
				if(!IsClientConnected(clientID))
				{
					res.Add("ERROR", PROTECT_NOT_CONNECTED);
					return res;
				}
				// get user data from database
				userRec = db.Get(clientLinks.Get(clientID).eMail);
			
				// avoid hacking of source packet stripping
				// data non-updateable by user
				VectorMap<String, Value>vs(v, 1);
				vs.RemoveKey("LICENSES");
				vs.RemoveKey("EXPIRATION");
				vs.RemoveKey("ACTIVATIONKEY");
				vs.RemoveKey("ACTIVATIONSENT");
				vs.RemoveKey("ACTIVATED");
				vs.RemoveKey("EMAIL");
				
				// update user record
				for(int i = 0; i < vs.GetCount(); i++)
				{
					int j = userRec.Find(vs.GetKey(i));
					if(j >= 0)
						userRec[j] = vs[i];
				}
				db.Set(userRec);
			}
			break;

		default:
			// disconnect me anyways
			if(v.Find("CLIENTID") >= 0)
			{
				clientID = (int)v.Get("CLIENTID");
				DisconnectClient(clientID);
			}
			res.Add("ERROR", PROTECT_UNKNOWN_REASON);
			break;
	}
	return res;
}

// sends activation mail to user
bool ProtectServer::SendActivationMail(VectorMap<String, Value> const &userData)
{
	// build request map
	VectorMap<String, Value>req;
	req.Add("REASON", PROTECT_ACTIVATE);
	req.Add("EMAIL", userData.Get("EMAIL"));
	req.Add("ACTIVATIONKEY", userData.Get("ACTIVATIONKEY"));
	
	// builds activation link
	String link = "http://" + serverVars.Get("SERVER_NAME") + serverVars.Get("REQUEST_URI") + "?";
	
	// encodes results and send back to client
	cypher->SetKey(key);
	link += "IV=" + HexString(cypher->GetNonce()) + "&";
	String encoded = HexString((*cypher)(StoreAsXML(req, "ProtectServer")));
	link += "DATA=" + encoded;

	smtp.To(userData.Get("EMAIL"));
	smtp.Subject("APPLICATION ACTIVATION");
	smtp.From(serverVars.Get("SERVER_NAME"));
	smtp.Text(link);
	return smtp.Send();
}

// runs the server
void ProtectServer::Run(void)
{
	// try to connect to db
	if(!db.Connect())
	{
		Cout() << "Error connecting to database\n";
		return;
	}
	
	// runs the SCGI server
	ScgiServer::Run();
}

END_UPP_NAMESPACE
