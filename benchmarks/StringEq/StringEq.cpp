#include <Core/Core.h>

using namespace Upp;

int found;

void Test(const char *name, const Vector<String>& patterns)
{
	Vector<String> p = clone(patterns);

	Vector<String> data;
	for(int i = 0; i < 1000; i++)
		data << String(~p[Random(p.GetCount())]);
	
	while(p.GetCount() < 1000)
		p.Append(clone(p));

	int rfound = 0;
	for(int i = 0; i < p.GetCount(); i++)
		for(int j = 0; j < data.GetCount(); j++)
			if(strcmp(p[i], data[j]) == 0)
				rfound++;

	found = 0;
	double tm1 = usecs();
	for(int q = 0; q < 1000; q++)
		for(int i = 0; i < p.GetCount(); i++)
			for(int j = 0; j < data.GetCount(); j++)
				if(p[i] == data[j])
					found++;
	double tm2 = usecs();
	
	if(1000 * rfound != found)
		Panic("Failed");

	RLOG(name << (tm2 - tm1) / p.GetCount() / data.GetCount() << " ns");
}

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_FILE|LOG_COUT);

	SeedRandom(0);
	
	Test("10 bytes ", { "0123456789", "012345678A" });
	Test("20 bytes ", { "01234567890123456789", "0123456789012345678A" });
	Test("40 bytes ", { "0123456789012345678901234567890123456789", "012345678901234567890123456789012345678A" });
	Test("80 bytes ", { "01234567890123456789012345678901234567890123456789012345678901234567890123456789",
	                    "0123456789012345678901234567890123456789012345678901234567890123456789012345678A", });
	Test("Mixed ", { "01234567890123456789", "0123456789A", "0123456789012345678A2", "012345678901", "012345678901234567890123456789012345678A" });
}
