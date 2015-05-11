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
	
	Index<String> hh;
	hh.Add("test");

	Parser p;
//	p.Do(in, base, 0, 0, GetFileTitle(path), callback(AddError), Vector<String>(), Vector<String>(), Index<String>());
	p.Do(in, base, 0, 0, GetFileTitle(path), callback(AddError),
	     Vector<String>(), Vector<String>(), hh);

	if(errs.GetCount())
		DUMPC(errs);
	Qualify(base);
	String out;
	for(int i = 0; i < base.GetCount(); i++) {
		out << Nvl(base.GetKey(i), "<globals>") << " {\n";
		const Array<CppItem>& ma = base[i];
		for(int j = 0; j < ma.GetCount(); j++) {
			const CppItem& m = ma[j];
			out << '\t' << CppItemKindAsString(m.kind) << ", name: " << m.name << ", qitem: " << m.qitem
			            << ", qtype " << m.qtype << ", qptype: " << m.qptype << ", line " << m.line
			            << ", using " << m.using_namespaces
			            << ", item " << m.item
			            << ", natural " << m.natural
			            << "\n";
		}
		out << "}\n";
	}
	
//	base.Dump(VppLog());
	
	p.dobody = true;
	in.Seek(0);
	p.Do(in, base, 0, 0, GetFileTitle(path), callback(AddError),
	     Vector<String>(), Vector<String>(), hh);
	
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
}

CONSOLE_APP_MAIN {
	StdLogSetup(LOG_COUT|LOG_FILE);
//	Test("C:/xxx/cpp/math");
	Test(GetDataFile("test1.in"));
	/*
	FindFile ff(GetDataFile("test1.in"));
	while(ff) {
		Test(ff.GetPath());
		ff.Next();
	}
	*/
}
