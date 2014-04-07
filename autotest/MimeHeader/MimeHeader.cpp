#include <Core/Core.h>
#include <Core/POP3/POP3.h>

using namespace Upp;

String etalon;

void Test(const String& ct, const String& val = Null)
{
	MIMEHeader h;
	h.Parse(ct);
	String s = AsString(h);
	LOG(ct);
	LOG("  " << h);
	if(val.GetCount())
		ASSERT(s == val);
	etalon << "Test(" << AsCString(ct) << ",\r\n     " << AsCString(s) << ");\r\n";
}

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	Test("Content-Type: text/plain",
	     "Content-Type: text/plain");
	Test("Content-Type: multipart/mixed; boundary=001a11c12a1ca56b2b04f66f1aca",
	     "Content-Type: multipart/mixed; boundary=\"001a11c12a1ca56b2b04f66f1aca\"");
	Test("Content-Disposition: attachment; filename=genome.jpeg;modification-date=\"Wed, 12 Feb 1997 16:29:51 -0500\";",
	     "Content-Disposition: attachment; filename=\"genome.jpeg\"; modification-date=\"Wed, 12 Feb 1997 16:29:51 -0500\"");
	Test("Content-Type: application/rtf; name=\"\\\";.rtf\"",
	     "Content-Type: application/rtf; name=\"\\\";.rtf\"");
	Test("Content-Disposition: attachment; filename=\"\\\";.rtf\"",
	     "Content-Disposition: attachment; filename=\"\\\";.rtf\"");
	Test("Content-Type: application/rtf; name=\"\\\";.rtf\";test=b",
	     "Content-Type: application/rtf; name=\"\\\";.rtf\"; test=\"b\"");
	Test("Content-Transfer-Encoding: base64",
	     "Content-Transfer-Encoding: base64");
	Test("Content-Type: text/plain; charset=\"iso-8859-1\"",
	     "Content-Type: text/plain; charset=\"iso-8859-1\"");
	Test("Content-Type: text/plain; charset=iso-8859-1",
	     "Content-Type: text/plain; charset=\"iso-8859-1\"");
	
	LOG("--- ETALONS ------------------");
	LOG(etalon);
	
	LOG("================ OK");
}
