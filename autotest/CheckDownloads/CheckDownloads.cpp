#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	Date d;
	String downloads = HttpRequest("https://www.ultimatepp.org/www$uppweb$download$en-us.html").Execute();
	
	LOG(downloads);
	
	int q = downloads.FindAfter("U++ for Linux/MacOS/BSD/Solaris</a></p>");
	ASSERT(q > 0);
	q = downloads.Find(">202", q); // should be good until 2030...:)
	ASSERT(q > 0);
	
	CParser p(~downloads + ++q);

	try {
		d.year = p.ReadInt();
		p.PassChar('-');
		d.month = p.ReadInt();
		p.PassChar('-');
		d.day = p.ReadInt();
	}
	catch(...) {
		NEVER();
	}

	LOG("Downloads last date: " << d);
	ASSERT(d >= GetSysDate() - 1);
	
	int e = downloads.Find("</tr>", q);
	ASSERT(e > 0);
	
	auto Check = [&](String s) {
		LOG("Checking " << s);
		int q = downloads.Find("downloads/upp-" + s);
		ASSERT(q > 0 && q < e);
	};

	Check("win");
	Check("posix");
//	Check("macos");
	
	LOG("============== OK");
}
