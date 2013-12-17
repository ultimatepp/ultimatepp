#include "NanoStrings.h"

const char *xmlfile = "/home/cxl/20131106_ST_ZZSZ.xml";

String RandomString(int maxlen = 70)
{
	int len = Random(maxlen);
	String h;
	for(int i = 0; i < len; i++)
		h.Cat(Random(26) + 'a');
	return h;
}

void Test()
{
	MemoryLimitKb(8000000);
	for(int sz = 0; sz < 2; sz++) {
		for(int pass = 0; pass < 2; pass++) {
			RLOG("--------------------");
			DUMP(sz);
			DUMP(pass);
			{
				NanoStrings ns;
				Vector<dword> ws;
				
				ns.ZeroTerminated(sz);
			
				SeedRandom();
				for(int i = 0; i < 140000000; i++) {
					if(i % 10000000 == 0)
						RLOG("Created " << i);
					String s = pass ? "x" : RandomString(Random(4) ? 5 : 50);
					ws.Add(ns.Add(s));
				}
				
//				ns.DumpProfile();
				RLOG("---- Strings " << MemoryUsedKb() << " KB used -------");
				RLOG(MemoryProfile());
				
				SeedRandom();
				for(int i = 0; i < ws.GetCount(); i++) {	
					if(i % 10000000 == 0)
						RLOG("Tested " << i);
					String s = pass ? "x" : RandomString(Random(4) ? 5 : 50);
					if((sz ? String(ns.GetPtr(ws[i])) : ns.Get(ws[i])) != s) {
						DUMP(i);
						DUMP(ns.Get(ws[i]));
						DUMP(s);
						NEVER();
					}
				}
				RLOG("Test OK");
			}
			RLOG("===== EMPTY " << MemoryUsedKb() << " KB used -------");
			RLOG(MemoryProfile());		
		}
	}
}

Vector<String> ss;

NanoStrings ns;
Vector<dword> ws;

int total_len = 0;

void Scan(const XmlNode& n, bool nano)
{
	for(int i = 0; i < n.GetCount(); i++) {
		total_len += n.GetText().GetCount();
		if(nano)
			ws.Add(ns.Add(n.GetText()));
		else
			ss.Add(n.GetText());
		if(n[i].IsTag()) {
			Scan(n[i], nano);
		}
	}
}

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);
	Test(); return;
	
	RLOG(MemoryProfile());
	
	{
		RTIMING("Parse all!");
		XmlNode n = ParseXMLFile(xmlfile);
		int used = MemoryUsedKb();
		{
			RTIMING("Scan");
			Scan(n, false);
		}
		RLOG("Text: " << (total_len >> 10) << " KB");
		RLOG("---- Strings " << MemoryUsedKb() - used << " KB used -------");
		RLOG(MemoryProfile());
		
		ns.DumpProfile();
	}
}