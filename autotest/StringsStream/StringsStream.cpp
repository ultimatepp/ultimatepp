#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);
	for(int pass = 0; pass < 10; pass++) {
		for(int part = 7; part < 200; part += (part < 20 ? 1 : 33)) {
			String eta;
			StringsStreamOut sso(part);
			for(int i = 0; i < 25773 + pass; i++) {
				String s = AsString(pass ? Random() : i);
				sso << s;
				eta << s;
			}
			
			Vector<String> s = sso.PickResult();
			ASSERT(Join(s, "") == eta);
			
			StringsStreamIn ssi(s);
			
			ASSERT(LoadStream(ssi) == eta);
			DDUMP(eta.GetCount());
		}
	}
	
	Vector<int> data;
	for(int i = 0; i < 256*1024*1024; i++)
		data << Random();
	
	Vector<String> s = StoreAsStrings(data);
	
	DDUMP(s.GetCount());
	
	Vector<int> data2;
	
	LoadFromStrings(data2, s);
	
	DDUMP(data2.GetCount());
	
	ASSERT(data == data2);
	
	LOG("=============== OK");
}
