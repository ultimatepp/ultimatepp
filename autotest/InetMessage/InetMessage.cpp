#include <Core/POP3/POP3.h>

using namespace Upp;

void Test(const InetMessage& msg, int ii, int count, const char *txt = NULL)
{
	LOG("Testing part " << ii);
	String body = msg[ii].Decode();
	ASSERT(body.GetCount() == count);
	ASSERT(body.Find(txt) >= 0);
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
	Test(msg, 1, 121, "Tato zpráva neobsahuje viry ani jiný škodlivý kód - avast! Antivirus je aktivní.");
	Test(msg, 3, 911, "Tato zpráva neobsahuje viry ani jiný škodlivý kód -<a href=");
	Test(msg, 4, 309891);

	DLOG("---------------------------------------------------");
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
	ASSERT(AsString(test) == "[(text/plain; charset=\"iso-8859-2\", 799), (text/html; charset=\"iso-8859-2\", 5899), (image/gif; name=\"image001.gif\", 5094), (message/rfc822, 272270), (text/plain; charset=\"windows-1250\", 7821), (text/html; charset=\"windows-1250\", 36236), (image/gif; name=\"image001.gif\", 5094), (application/pdf; name=\"BRN_928C13_003175.pdf\", 156223), (message/rfc822, 20440), (text/plain; charset=\"iso-8859-2\", 1152), (text/html; charset=\"iso-8859-2\", 9553), (image/gif; name=\"image001.gif\", 5094)]");
	
	LOG("============ OK");
}
