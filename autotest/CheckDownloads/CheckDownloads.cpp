#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	Date d;
	String downloads = HttpRequest("http://www.ultimatepp.org/www$uppweb$nightly$en-us.html").Execute();
	int q = downloads.Find("<a href=\"downloads/upp-x11-src-");
	ASSERT(q > 0);
	q = downloads.ReverseFind(">", q - 1);
	ASSERT(q > 0);
	
	CParser p(~downloads + q + 1);

	try {
		d.month = p.ReadInt();
		p.PassChar('/');
		d.day = p.ReadInt();
		p.PassChar('/');
		d.year = p.ReadInt();
	}
	catch(...) {
		NEVER();
	}

	LOG("downloads last date: " << d);
	ASSERT(d >= GetSysDate() - 1);
}
