#include "ProtectClientDemo.h"

static String DumpSpecial(String s)
{
	String out;
	for(const char *p = s.Begin(), *e = s.End(); p < e; p++)
		if((byte)*p >= ' ' && *p != '\xFF')
			out.Cat(*p);
		else {
			switch(*p) {
				case '\a': out.Cat("[\\a]"); break;
				case '\b': out.Cat("[\\b]"); break;
				case '\f': out.Cat("[\\f]"); break;
				case '\v': out.Cat("[\\v]"); break;
				case '\t': out.Cat("[\\t]"); break;
				case '\r': out.Cat("[\\r]"); break;
				case '\n': out.Cat("[\\n]\n"); break;
				default:   out.Cat(NFormat("[\\x%02x", (byte)*p)); break;
			}
		}
	return out;
}

ProtectClientDemo::ProtectClientDemo()
{
	CtrlLayout(*this, "Window title");
	
	registerButton		<<= THISBACK1(onAction, PROTECT_REGISTER);
	connectButton		<<= THISBACK1(onAction, PROTECT_CONNECT);
	refreshButton		<<= THISBACK1(onAction, PROTECT_REFRESH);
	disconnectButton	<<= THISBACK1(onAction, PROTECT_DISCONNECT);
	getKeyButton		<<= THISBACK1(onAction, PROTECT_GETKEY);
	getInfoButton		<<= THISBACK1(onAction, PROTECT_GETLICENSEINFO);
	
	quitButton <<= Breaker();
	
	emailEdit		<<= "test@testme.net";
}

ProtectClientDemo::~ProtectClientDemo()
{
}

void ProtectClientDemo::onAction(int reason)
{
	// clear response text
	responseText <<= "";
	
	// setup client url and key
	client.SetURL("localhost/scgi/testing");
//	client.SetURL("www.timberstruct.it/scgi/testing");
	client.SetKey(ScanHexString("aabbccddeeff00112233445566778899"));

	// sets user data
	client.SetUserName(~nameEdit);
	client.SetUserAddress(~addressEdit);
	client.SetUserCountry(~countryEdit);
	client.SetUserZip(~zipEdit);
	client.SetUserPhone(~phoneEdit);
	client.SetUserFax(~faxEdit);
	client.SetUserCell(cellEdit);
	client.SetUserEMail(~emailEdit);

	String res;
	String key;
	switch(reason)
	{
		case PROTECT_CONNECT :			// establish connection to server
			if(!client.Connect())
				res += "CONNECT ERROR\n";
			break;
		case PROTECT_DISCONNECT :		// frees server connection
			if(!client.Disconnect())
				res += "DISCONNECT ERROR\n";
			break;
		case PROTECT_REFRESH :			// refreshes server connection (to restart timeout)
			if(!client.Refresh())
				res += "REFRESH ERROR\n";
			break;
		case PROTECT_GETKEY :			// gets application key
			key = client.GetKey();
			if(key == "")
				res += "GETKEY ERROR\n";
			else
				res += "GOT LICENSE KEY : '" + key + "'\n";
			break;
		case PROTECT_REGISTER :			// registers app for timed demo
			if(!client.Register())
				res += "REGISTER ERROR\n";
			break;
		case PROTECT_GETLICENSEINFO :	// gets info about license (name, expiration date, app version....)
			if(!client.GetLicenseInfo())
				res += "GETLICENSEINFO ERROR\n";
			break;
		default:
			break;
	}
	res += "Last server message : " + client.GetLastErrorMsg();
	
	responseText <<= res; 
	
}

GUI_APP_MAIN
{
	ProtectClientDemo().Run();
}

