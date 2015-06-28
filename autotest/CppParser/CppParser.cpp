#include <CppBase/CppBase.h>

using namespace Upp;

Vector<String> errs;

void AddError(int ln, const String& s)
{
	errs.Add(AsString(ln) + ": " + s);
}

void CleanAnonymous(String& s)
{
	for(;;) {
		int q = s.Find('@');
		if(q < 0)
			return;
		int qq = s.Find("/title", q);
		if(qq < 0)
			return;
		s.Remove(q, qq - q - 1);
	}
}

void Test(const char *path, int filetype)
{
	CppBase base;
	
	LOG("**** " << GetFileName(path));

	FileIn in(path);
	errs.Clear();

	Index<String> hh;

	Parser p;
	p.Do(in, base, 0, filetype, "title", callback(AddError),
	     Vector<String>(), Vector<String>(), hh);

	if(errs.GetCount())
		DUMPC(errs);
	Qualify(base);
	String out;
	if(errs.GetCount())
		out << "ERRORS: " << errs << '\n';
	for(int i = 0; i < base.GetCount(); i++) {
		out << Nvl(base.GetKey(i), "<globals>") << " {\n";
		const Array<CppItem>& ma = base[i];
		for(int j = 0; j < ma.GetCount(); j++) {
			const CppItem& m = ma[j];
			out << '\t' << CppItemKindAsString(m.kind) << ", name: " << m.name << ", qitem: " << m.qitem 
			            << ", qtype: " << m.qtype
			            << ", qptype: " << m.qptype
			            << ", natural: " << m.natural
			            << ", line " << m.line
			            << ", using " << m.using_namespaces;
			if(m.isptr)
				out << ", pointer";
		 	out << "\n";
		}
		out << "}\n";
	}
	
	p.dobody = true;
	in.Seek(0);
	p.Do(in, base, 0, filetype, "title", callback(AddError),
	     Vector<String>(), Vector<String>(), hh);
	
	out << "<locals> {\n";
	for(int i = 0; i < p.local.GetCount(); i++) {
		out << p.local.GetKey(i) << " " << p.local[i].type;
		if(p.local[i].isptr)
			out << " pointer";
		out << ", line: " << p.local[i].line << "\n";
	}
	out << "}";
	CleanAnonymous(out);
	LOG("====");
	LOG(out);
	LOG("-------------------------------------------------------------------------------");
#ifdef flagSAVE
	SaveFile(ForceExt(path, ".out"), out);
#else
	String h = LoadFile(ForceExt(path, ".out"));
	h.Replace("\r", "");
	ASSERT(out == h);
#endif
}

CONSOLE_APP_MAIN {
	StdLogSetup(LOG_COUT|LOG_FILE);

	for(int i = 0; i < 10000; i++) {
		String p = GetDataFile("test" + AsString(i) + ".in");
		if(FileExists(p))
			Test(p, FILE_H);
		p << 'c';
		if(FileExists(p))
			Test(p, FILE_C);
	}
	LOG("=========== OK");
}
