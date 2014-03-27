#include <Core/POP3/POP3.h>

using namespace Upp;

// WIN32: This example requires OpenSSL .dlls to be available

CONSOLE_APP_MAIN
{
	Cout() << "Your GMail username: ";
	String user = ReadStdIn();
	Cout() << "Your GMail password: ";
	String pass = ReadStdIn();
	Pop3 pop3;
	pop3.Host("pop.gmail.com")
              .Port(995)
              .User(user, pass)
              .SSL()              
              .Trace(); // Activate logging of Pop3.
	Cout() 	<< "Connecting to GMail POP3 server... ";
	if(!pop3.Login()) {
		Cout() << "Failed: " << pop3.GetLastError() << "\n";
		return;
	}
	int msgcount = pop3.GetMessageCount();
	if(msgcount < 0) {
		Cout() << "\nFailed: " << pop3.GetLastError() << "\n";
		return;
	}
	Cout() << "There are " << msgcount << " messages in your inbox.\n";
	for(;;) {
		Cout()	<< "Message number to retrieve (1.." << msgcount << ") or anything else to QUIT: ";
		int i = StrInt(ReadStdIn());
		if(i >= 1 && i <= msgcount) {
			String message = pop3.GetMessage(i);
			if(IsNull(message))
				Cout() << "An error encountered: " << pop3.GetLastError() << "\n";
			else {
				InetMessage m;
				if(m.Read(message)) {
					Cout() << "Subject: " << m["subject"] << "\n"
					       << "From: " << m["from"] << "\n"
					       << "Date: " << m["date"] << "\n";
					for(int i = 0; i < m.GetCount(); i++)
						Cout() << "= Part " << i << ", type " << m[i]["content-type"]
						       << ", decoded size " << m[i].Decode().GetLength() << '\n';
					Cout() << "-------------------------------------\n";
				}
				else
					Cout() << "Error parsing the message\n";
			}
		}
		else
			break;
	}
	Cout() << "Logging out!..\n";
}
