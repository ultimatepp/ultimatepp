#include "Browser.h"

#define LDUMP(x) // DDUMP(x)

String PreprocessImlFile(const char *fn)
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
				i_ml << "\tI_" << id << ",\n";
				iml << "\tstatic Image " << id << "();\n";
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
#ifdef PLATFORM_WIN32
	FindFile ff(fn);
	r << "struct " << GetFileTitle(ff.GetPath()) << "Img {\n";
#else
	r << "struct " << GetFileTitle(fn) << "Img {\n";
#endif
	if(iml.GetCount())
		r << "\tenum {" << i_ml << "\t};\n" << iml;
	r <<
		"\n\tstatic Iml&   Iml();"
		"static int    Find(const Upp::String& s);"
		"static int    Find(const char *s);"
		"static int    GetCount();"
		"static String GetId(int i);"
		"static Image  Get(int i);"
		"static Image  Get(const char *s);"
		"static Image  Get(const Upp::String& s);"
		"static void   Set(int i, const Upp::Image& m);"
		"static void   Set(const char *s, const Upp::Image& m);"
		"static void   Reset();"
	"};\n";
	LDUMP(r);
	return r;
}
