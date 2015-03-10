#include <CppBase/CppBase.h>

using namespace Upp;

Vector<String> errs;

void AddError(int ln, const String& s)
{
	errs.Add(AsString(ln) + ": " + s);
}

void Test(const char *path)
{
	CppBase base;
	
	LOG("**** " << GetFileName(path));

	FileIn in(path);
	errs.Clear();

	Parser p;
	p.Do(in, Vector<String>(), base, path, callback(AddError));

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
			out << '\t' << CppItemKindAsString(m.kind) << ' ' << m.qitem << ", line " << m.line << "\n";
		}
		out << "}\n";
	}
	
	p.dobody = true;
	in.Seek(0);
	p.Do(in, Vector<String>(), base, path, callback(AddError));
	
	out << "<locals> {\n";
	for(int i = 0; i < p.local.GetCount(); i++) {
		out << p.local.GetKey(i) << " " << p.local[i].type;
		if(p.local[i].isptr)
			out << " pointer";
		out << ", line: " << p.local[i].line << "\n";
	}
	out << "}";
	LOG("====");
	LOG(out);
	LOG("-------------------------------------------------------------------------------");
	String h = LoadFile(ForceExt(path, ".out"));
	h.Replace("\r", "");
	ASSERT(out == h);
}

CONSOLE_APP_MAIN {
	StdLogSetup(LOG_COUT|LOG_FILE);
	
#ifdef flagSINGLE
	FindFile ff(GetDataFile("test5.in"));
#else
	FindFile ff(GetDataFile("*.in"));
#endif
	while(ff) {
		Test(ff.GetPath());
		ff.Next();
	}
	LOG("=========== OK");
}
