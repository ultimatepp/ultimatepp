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
	SeedRandom();
	for(int pass = 0; pass < 2; pass++) {
		NanoStrings ns;
		Vector<dword> ws;
		Vector<String> vs;
	
		for(int i = 0; i < 100000000; i++) {
			String s = pass ? "x" : RandomString();
			ws.Add(ns.Add(s));
			vs.Add(s);
		}
		
		for(int i = 0; i < ws.GetCount(); i++)
			if(ns.Get(ws[i]) != vs[i]) {
				DUMP(i);
				DUMP(ns.Get(ws[i]));
				DUMP(vs[i]);
				NEVER();
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
		
		ns.Profile();
	}
}