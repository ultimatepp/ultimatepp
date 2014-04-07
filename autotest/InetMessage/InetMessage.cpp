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
		DLOG("== " << i << " ========== " << msg[i]["content-type"]);
		String body = msg[i].Decode();
		DDUMP(body.GetCount());
		if(body.GetCount() < 2000)
			DDUMP(body);
	}

	Test(msg, 0, 0);
	Test(msg, 1, 121, "Tato zpráva neobsahuje viry ani jiný škodlivý kód - avast! Antivirus je aktivní.");
	Test(msg, 3, 911, "Tato zpráva neobsahuje viry ani jiný škodlivý kód -<a href=");
	Test(msg, 4, 309891);
	
	LOG("============ OK");
}
