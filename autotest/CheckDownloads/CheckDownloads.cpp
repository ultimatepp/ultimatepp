#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	Date d;
	String downloads = HttpRequest("https://www.ultimatepp.org/www$uppweb$download$en-us.html").Execute();
	for(String s : { "posix", "macos", "win" }) {
		int q = downloads.Find("<a href=\"downloads/upp-" + s);
		ASSERT(q > 0);
		q = downloads.ReverseFind(">", q - 1);
		ASSERT(q > 0);
		
		CParser p(~downloads + q + 1);
	
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
	
		LOG(s << " downloads last date: " << d);
		ASSERT(d >= GetSysDate() - 1);
	}
	LOG("============== OK");
}
