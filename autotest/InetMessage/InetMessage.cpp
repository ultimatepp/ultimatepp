#include <Core/POP3/POP3.h>
#include <Core/SMTP/SMTP.h>

using namespace Upp;

void Test(const InetMessage& msg, int ii, int count, const char *txt = NULL, const char *mime = NULL)
{
	LOG("Testing part " << ii);
	String body = msg[ii].Decode();
	ASSERT(body.GetCount() == count);
	ASSERT(body.Find(txt) >= 0);
	if(mime) {
		MIMEHeader h(msg[ii]["content-type"]);
		ASSERT(~h == mime);
	}
}

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	InetMessage msg;
	String h = LoadFile(GetDataFile("msg1.txt"));
	msg.Read(h);
	for(int i = 0; i < msg.GetCount(); i++) {
		LOG("== " << i << " ========== " << msg[i]["content-type"]);
		String body = msg[i].Decode();
		DUMP(body.GetCount());
		if(body.GetCount() < 2000)
			DUMP(body);
	}

	Test(msg, 0, 0);
	Test(msg, 1, 119, "Tato zpráva neobsahuje viry ani jiný škodlivý kód - avast! Antivirus je aktivní.");
	Test(msg, 3, 909, "Tato zpráva neobsahuje viry ani jiný škodlivý kód -<a href=");
	Test(msg, 4, 309891);

	DLOG("=========================================================");
	ASSERT(msg.Read(LoadFile(GetDataFile("msg2.txt"))));
	Vector< Tuple2<String, int> > test;
	for(int i = 0; i < msg.GetCount(); i++) {
		String ct = msg[i]["content-type"];
		int len = msg[i].Decode().GetCount();
		LOG(ct << ", LEN: " << len);
		if(len)
			test.Add(MakeTuple(ct, len));
		if(ct == "message/rfc822") {
			InetMessage nmsg;
			nmsg.Read(msg[i].Decode());
			for(int i = 0; i < nmsg.GetCount(); i++) {
				String ct = nmsg[i]["content-type"];
				int len = nmsg[i].Decode().GetCount();
				LOG("   " << ct << ", LEN: " << len);
				if(len)
					test.Add(MakeTuple(ct, len));
			}
		}
	}
	LOG(test);
	ASSERT(AsString(test) == "[(text/plain; charset=\"iso-8859-2\", 797), (text/html; charset=\"iso-8859-2\", 5897), (image/gif; name=\"image001.gif\", 5094), (message/rfc822, 272268), (text/plain; charset=\"windows-1250\", 7819), (text/html; charset=\"windows-1250\", 36234), (image/gif; name=\"image001.gif\", 5094), (application/pdf; name=\"BRN_928C13_003175.pdf\", 156223), (message/rfc822, 20438), (text/plain; charset=\"iso-8859-2\", 1150), (text/html; charset=\"iso-8859-2\", 9551), (image/gif; name=\"image001.gif\", 5094)]");

	DLOG("=========================================================");

	Smtp mail;
	mail
		.To("cxl@ntllib.org")
	    .Subject("Test message")
	    .Body("Hello world!\r\n")
		.Attach("test.txt", String("Attachment"));
	
	msg.Read(mail.GetMessage());
	
	LOG(mail.GetMessage());
	DLOG("=========================================================");
	
	for(int i = 0; i < msg.GetCount(); i++) {
		String ct = msg[i]["content-type"];
		String d = msg[i].Decode();
		LOG("-- " << ct << ", LEN: " << d.GetCount());
		LOGHEX(d);
	}
	
	Test(msg, 0, 0, NULL, "multipart/mixed");
	Test(msg, 1, 12, "Hello world!", "text/plain");
	Test(msg, 2, 10, "Attachment", "application/octet-stream");

	LOG("============ OK");
}
