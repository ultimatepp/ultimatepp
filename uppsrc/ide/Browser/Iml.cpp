#include "Browser.h"

#define LDUMP(x) // DDUMP(x)

void ScanImlFile(const char *fn)
{
	String s = LoadFile(fn);
	
	CParser p(s);
	String iml, i_ml;
	try {
		p.PassId("PREMULTIPLIED");
		for(;;) {
			if(p.Id("IMAGE_ID")) {
				p.Char('(');
				String id = p.ReadId();
				i_ml << "I_" << id << ",\n";
				iml << "static Image " << id << "();\n";
				p.Char(')');
			}
			else
			if(p.Id("IMAGE_META")) {
				p.Char('(');
				p.ReadString();
				p.Char(',');
				p.ReadString();
				p.Char(')');
			}
			else
				break;
		}
	}
	catch(CParser::Error) {}

	String r;
	r << "struct " << GetFileTitle(fn) << "Img {\n";
	if(iml.GetCount())
		r << "enum {" << i_ml << "};\n" << iml;
	r <<
		"static Iml&   Iml();"
		"static int         Find(const UPP::String& s);"
		"static int         Find(const char *s);"
		"static int         GetCount();"
		"static String GetId(int i);"
		"static Image  Get(int i);"
		"static Image  Get(const char *s);"
		"static Image  Get(const UPP::String& s);"
		"static void   Set(int i, const UPP::Image& m);"
		"static void   Set(const char *s, const UPP::Image& m);"
		"static void   Reset();"
	"};\n";
	LDUMP(r);
	StringStream ss(r);
	CppBase& base = CodeBase();
	Parse(ss, IgnoreList(), base, fn, CNULL);
}
