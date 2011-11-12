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

inline void WriteLines(String& r, int count)
{
	for (int i = 0; i < count; ++i)
		r << '\n';
}

void ScanLayFile(const char *fn)
{
	LTIMING("Lay file");
	String s = LoadFile(fn);
	CParser p(s);
	String r;
	try {
		int line = p.GetLine();
		if(p.Char('#') && p.Id("ifdef")) {
			if(!p.Id("LAYOUTFILE"))
				p.Id("LAYOUT");
			WriteLines(r, p.GetLine() - line);
		}
		while(!p.IsEof()) {
			line = p.GetLine();
			p.PassId("LAYOUT");
			p.PassChar('(');
			r << "template <class T> struct With" << p.ReadId() << " : public T {"
			  << "\tstatic Size GetLayoutSize();";
			LaySkipRest(p);
			WriteLines(r, p.GetLine() - line);
			for(;;) {
				line = p.GetLine();
				if(p.Id("ITEM")) {
					p.PassChar('(');
					if(p.IsId()) {
						String type = p.ReadIdt();
						if(strncmp(type, "dv___", 5)) {
							r << '\t' << type;
							p.PassChar(',');
							r << ' ' << p.ReadId() << ";";
						}
					}
				}
				else
				if(p.Id("UNTYPED"))
					p.Char('(');
				else
					break;
				LaySkipRest(p);
				WriteLines(r, p.GetLine() - line);
			}
			line = p.GetLine();
			p.PassId("END_LAYOUT");
			if(p.Char('#'))
				p.Id("endif");
			r << "};";
			WriteLines(r, p.GetLine() - line);
		}
	}
	catch(CParser::Error) {}
	LDUMP(r);
	StringStream ss(r);
	CppBase& base = CodeBase();
	Parse(ss, IgnoreList(), base, fn, CNULL);
}
