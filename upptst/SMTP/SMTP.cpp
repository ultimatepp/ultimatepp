#include <Core/SMTP/SMTP.h>

using namespace Upp;

// This example requires OpenSSL library installed in win32!

CONSOLE_APP_MAIN
{
	Smtp::Trace();
	String to = "to@test";
	String user = "test";
	String pwd = "test";
	Smtp mail;
	mail.Host("smtp.gmail.com")
	    .Port(465)
	    .SSL()
		.Auth(user, pwd)
		.AddHeader("Test", String().Cat() << "<ua.aa>")
		.AddHeader("Message-ID", String().Cat() << "<ua.aa>")
		.To(to)
	    .Subject("Test message")
	    .Body("Hello world!")
	    .Send();
}
