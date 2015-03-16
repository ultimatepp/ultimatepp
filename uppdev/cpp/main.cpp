#include "cpp.h"

Vector<String> errs;

void AddError(const String& path, int ln, const String& s)
{
	errs.Add(path + " " + AsString(ln) + ": " + s);
}

void Test(const char *path)
{
	Cpp cpp;
	cpp.WhenError = callback(AddError);
	cpp.path = path;
	cpp.filedir = GetFileFolder(path);
//	cpp.include_path = cpp.filedir;//"C:\\Program Files (x86)\\Microsoft Visual Studio 10.0\\Vc\\Include;C:\\Program Files\\Microsoft SDKs\\Windows\\v7.1\\Include;C:\\OpenSSL-Win32\\include;C:\\u\\pgsql\\include;C:\\u\\OpenSSL-Win32\\include";
	cpp.include_path = "C:\\Program Files (x86)\\Microsoft Visual Studio 10.0\\Vc\\Include;C:\\Program Files\\Microsoft SDKs\\Windows\\v7.1\\Include;C:\\OpenSSL-Win32\\include;C:\\u\\pgsql\\include;C:\\u\\OpenSSL-Win32\\include";
	FileIn in(path);
	Index<String> inc;
	cpp.DoCpp(in);
//	StringStream ss(pp);
//	Parse(ss, Vector<String>() << "__cdecl", base, path, callback(AddError));
	DLOG("=======================");
//	DUMPC(inc);
//	DLOG("=======================");
	DUMPC(errs);
	DLOG("=======================");
	Qualify(cpp.base);
	String out;
	for(int i = 0; i < cpp.base.GetCount(); i++) {
		out << Nvl(cpp.base.GetKey(i), "<globals>") << " {\n";
		const Array<CppItem>& ma = cpp.base[i];
		for(int j = 0; j < ma.GetCount(); j++) {
			const CppItem& m = ma[j];
			out << '\t' << CppItemKindAsString(m.kind) << ' ' << m.qitem << ' ' << m.line << "\n";
			// DDUMP(StoreAsString(const_cast<CppItem&>(m)).GetCount());
		}
		out << "}\n";
	}
	LOG(out);
}

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_FILE, NULL, 150000000);
	Test(GetDataFile("testfile"));
	return;
}

// TEST:a,b:|a|b|\n
