#include <Core/SMTP/SMTP.h>

using namespace Upp;

// This example requires OpenSSL library installed in win32!

CONSOLE_APP_MAIN
{
//	Smtp::Trace();
	for(;;) {
		Cout() << "Send email to: ";
		String to = ReadStdIn();
		if(IsNull(to))
			break;
		Cout() << "Your full email address: ";
		String user = ReadStdIn();
		Cout() << "Your Gmail password: ";
		String pwd = ReadStdIn();
		Smtp mail;
		mail.Host("smtp.gmail.com")
		    .Port(465)
		    .SSL()
			.Auth(user, pwd)
			.To(to)
		    .Subject("Test message")
		    .Body("Hello world!")
		    .Attach("somedata.txt", "This is a test of String attachement")
		    .AttachFile(GetDataFile("SMTP.cpp"));
		if(mail.Send())
			Cout() << "OK\n";
		else
			Cout() << "Failed: " << mail.GetError() << '\n';
	}
}
