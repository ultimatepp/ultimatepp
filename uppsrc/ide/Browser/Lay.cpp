#include "Browser.h"

#define LTIMING(x) // RTIMING(x)
#define LDUMP(x)   // DUMP(x)

void LaySkipRest(CParser& p)
{
	int lvl = 1;
	while(!p.IsEof()) {
		if(p.Char('('))
			lvl++;
		else
		if(p.Char(')')) {
			if(--lvl == 0)
				break;
		}
		else
			p.SkipTerm();
	}
}

void ScanLayFile(const char *fn)
{
	LTIMING("Lay file");
	String s = LoadFile(fn);
	CParser p(s);
	String r;
	try {
		if(p.Char('#') && p.Id("ifdef")) {
			if(!p.Id("LAYOUTFILE"))
				p.Id("LAYOUT");
		}
		while(!p.IsEof()) {
			p.PassId("LAYOUT");
			p.PassChar('(');
			r << "template <class T> struct With" << p.ReadId() << " : public T {\n"
			  << "\tstatic Size GetLayoutSize();\n";
			LaySkipRest(p);
			for(;;) {
				if(p.Id("ITEM")) {
					p.PassChar('(');
					if(p.IsId()) {
						String type = p.ReadIdt();
						if(strncmp(type, "dv___", 5)) {
							r << '\t' << type;
							p.PassChar(',');
							r << ' ' << p.ReadId() << ";\n";
						}
					}
				}
				else
				if(p.Id("UNTYPED"))
					p.Char('(');
				else
					break;
				LaySkipRest(p);
			}
			p.PassId("END_LAYOUT");
			if(p.Char('#'))
				p.Id("endif");
			r << "};\n\n";
		}
	}
	catch(CParser::Error) {}
	LDUMP(r);
	StringStream ss(r);
	CppBase& base = BrowserBase();
	Parse(ss, IgnoreList(), base, fn, CNULL);
}
