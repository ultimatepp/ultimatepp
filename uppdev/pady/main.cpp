#include <CtrlLib/CtrlLib.h>
#include <Oracle/Oracle7.h>

SqlId PRIJM("PRIJM");
SqlId JMENO("JMENO");
SqlId OSOBA("OSOBA");
SqlId TYP("TYP");

Index<String> txt;

void FetchData()
{
	while(SQL.Fetch()) {
		String s = SQL[0];
		if(s.Find(' ') < 0)
			txt.FindAdd(s);
	}
}

void FetchDatabase()
{
	SQL * Select(PRIJM).From(OSOBA).Where(TYP == "M" || TYP == "Z");
	FetchData();
	SQL * Select(JMENO).From(OSOBA).Where(TYP == "M" || TYP == "Z");
	FetchData();
	String s;
}

struct ReversedCompare {
	bool operator()(const String& a, const String& b) const {
		const char *ap = a.End();
		const char *bp = b.End();
		for(;;) {
			if(ap == a.Begin())
				return bp != b.Begin();
			if(bp == b.Begin())
				return false;
			if(*--ap != *--bp) {
				int c1 = ToAscii(*ap, CHARSET_WIN1250);
				int c2 = ToAscii(*bp, CHARSET_WIN1250);
				if(c1 != c2)
					return c1 < c2;
				return *ap < *bp;
			}
		}
	}
};

GUI_APP_MAIN
{
	SetDefaultCharset(CHARSET_WIN1250);
	Oracle7 oracle;
	oracle.Open("mb/idismain@cxl");
	SQL = oracle;
/*	FetchDatabase();
	oracle.Open("idis_main/main_idis@cxl");
	SQL = oracle;
	FetchDatabase();
	Vector<String> x = txt.PickKeys();
	Sort(x, ReversedCompare());*/
	SQL * Select(JMENO, PRIJM, TYP).From(OSOBA).Where(TYP == "Z" || TYP == "M");
	while(SQL.Fetch()) {
		LOG(SklonJmeno(SQL[0], SQL[1], (String)SQL[2] == "Z", 1) <<
		    ", bez " << SklonJmeno(SQL[0], SQL[1], (String)SQL[2] == "Z", 2) <<
		    ", k " << SklonJmeno(SQL[0], SQL[1], (String)SQL[2] == "Z", 3) <<
		    ", vidím " << SklonJmeno(SQL[0], SQL[1], (String)SQL[2] == "Z", 4) <<
		    ", volám " << SklonJmeno(SQL[0], SQL[1], (String)SQL[2] == "Z", 5) <<
		    ", o " << SklonJmeno(SQL[0], SQL[1], (String)SQL[2] == "Z", 6) <<
		    ", s " << SklonJmeno(SQL[0], SQL[1], (String)SQL[2] == "Z", 7));
	}
}
