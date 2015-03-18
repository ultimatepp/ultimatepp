#include "cpp.h"

Vector<String> errs;

void AddError(const String& path, int ln, const String& s)
{
	errs.Add(path + " " + AsString(ln) + ": " + s);
}

void Test(const char *path)
{
	DDUMP(sizeof(CppItem));
	Cpp cpp;
	cpp.WhenError = callback(AddError);
	cpp.path = path;
	cpp.filedir = GetFileFolder(path);
//	cpp.include_path = cpp.filedir;//"C:\\Program Files (x86)\\Microsoft Visual Studio 10.0\\Vc\\Include;C:\\Program Files\\Microsoft SDKs\\Windows\\v7.1\\Include;C:\\OpenSSL-Win32\\include;C:\\u\\pgsql\\include;C:\\u\\OpenSSL-Win32\\include";
//	cpp.include_path = "C:\\Program Files (x86)\\Microsoft Visual Studio 10.0\\Vc\\Include;C:\\Program Files\\Microsoft SDKs\\Windows\\v7.1\\Include;C:\\OpenSSL-Win32\\include;C:\\u\\pgsql\\include;C:\\u\\OpenSSL-Win32\\include";
	cpp.include_path = "C:\\Program Files (x86)\\Microsoft Visual Studio 9.0\\Vc\\Include;C:\\Program Files\\Microsoft SDKs\\Windows\\v7.0\\Include;C:\\u\\OpenSSL-Win32\\include;C:\\u\\pgsql\\include;C:\\Program Files (x86)\\MySQL\\MySQL Connector C 6.1\\include";
	cpp.include_path << ";c:/u/upp.src/uppsrc";
	FileIn in(path);
	Index<String> inc;
	cpp.DoCpp(in, inc);
//	StringStream ss(pp);
//	Parse(ss, Vector<String>() << "__cdecl", base, path, callback(AddError));
	DLOG("=======================");
	DUMPC(inc);
	DLOG("=======================");
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
//	Test(GetDataFile("testfile"));
	Test("c:/u/upp.src/uppsrc/CtrlLib/EditField.cpp");
	getchar();
	return;
}

// TEST:a,b:|a|b|\n
