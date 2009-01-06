#include <Web/Web.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	SmtpMail mail;
	mail.To("cxl@ntllib.org");
	mail.Host("out.smtp.cz");
	mail.Auth("cxl@ntllib.org", LoadFile("U:/smtp.txt"));
	mail.Text("<h2>Hello!</h2><p>This is test of SMTP mail capabilities!", "text/html");
	mail.From("cxl@ntllib.org");
	mail.Subject("SmtpMail test");
	mail.Send();
	DDUMP(mail.GetError());
	DDUMP(mail.GetTranscript());
}
