#include "cpp.h"

Vector<String> errs;

void AddError(const String& path, int ln, const String& s)
{
	errs.Add(path + " " + AsString(ln) + ": " + s);
}

String include_path =
	"C:\\Program Files (x86)\\Microsoft Visual Studio 10.0\\Vc\\Include;C:\\Program Files\\Microsoft SDKs\\Windows\\v7.1\\Include;C:\\OpenSSL-Win32\\include;C:\\u\\pgsql\\include;C:\\u\\OpenSSL-Win32\\include;"
	"C:\\Program Files (x86)\\Microsoft Visual Studio 9.0\\Vc\\Include;C:\\Program Files\\Microsoft SDKs\\Windows\\v7.0\\Include;C:\\u\\OpenSSL-Win32\\include;C:\\u\\pgsql\\include;C:\\Program Files (x86)\\MySQL\\MySQL Connector C 6.1\\include;"
	";c:/u/upp.src/uppsrc";

void Test(const char *path)
{
	DDUMP(sizeof(CppItem));
	Cpp cpp;
	cpp.WhenError = callback(AddError);
	cpp.path = path;
	cpp.filedir = GetFileFolder(path);
//	cpp.include_path = cpp.filedir;//"C:\\Program Files (x86)\\Microsoft Visual Studio 10.0\\Vc\\Include;C:\\Program Files\\Microsoft SDKs\\Windows\\v7.1\\Include;C:\\OpenSSL-Win32\\include;C:\\u\\pgsql\\include;C:\\u\\OpenSSL-Win32\\include";
	cpp.include_path << "C:\\Program Files (x86)\\Microsoft Visual Studio 10.0\\Vc\\Include;C:\\Program Files\\Microsoft SDKs\\Windows\\v7.1\\Include;C:\\OpenSSL-Win32\\include;C:\\u\\pgsql\\include;C:\\u\\OpenSSL-Win32\\include;";
	cpp.include_path << "C:\\Program Files (x86)\\Microsoft Visual Studio 9.0\\Vc\\Include;C:\\Program Files\\Microsoft SDKs\\Windows\\v7.0\\Include;C:\\u\\OpenSSL-Win32\\include;C:\\u\\pgsql\\include;C:\\Program Files (x86)\\MySQL\\MySQL Connector C 6.1\\include;";
	cpp.include_path << ";c:/u/upp.src/uppsrc";
	FileIn in(path);
	Index<String> inc;
	cpp.DoCpp(in, inc);
//	StringStream ss(pp);
//	Parse(ss, Vector<String>() << "__cdecl", base, path, callback(AddError));
	DLOG("=======================");
	DUMPC(cpp.macro.GetKeys());
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

void RecursePP(const char *path, const char *include_path, Index<String>& visited)
{
	const PPFile& p = GetPPFile(path);
	String filedir = GetFileFolder(path);
	for(int i = 0; i < p.includes.GetCount(); i++) {
		String ip = GetIncludePath(p.includes[i], filedir, include_path);
		if(visited.Find(ip) < 0) {
			visited.Add(ip);
			RecursePP(ip, include_path, visited);
		}
	}
	LOG("-------------------------");
	LOG(path);
	p.Dump();
}

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_FILE, NULL, 150000000);
//	Test(GetDataFile("testfile"));
//	Test("c:/u/upp.src/uppsrc/CtrlLib/EditField.cpp");
//	getchar();

//	PPFile f;
//	FileIn in(GetDataFile("test.h"));
//	f.Parse(in);
//	f.Dump();

	{
		RTIMING("Pass1");
		Index<String> visited;
//		RecursePP("c:/u/upp.src/uppsrc/CtrlLib/EditField.cpp", include_path, visited);
	}
	
	DDUMP(IncludesFile("c:/u/upp.src/uppsrc/CtrlLib/EditField.cpp", "c:/u/upp.src/uppsrc/Core/Core.h", include_path));
	
	for(int i = 0; i < 1000; i++) {
		RTIMING("IncludesFile true");
		IncludesFile("c:/u/upp.src/uppsrc/CtrlLib/EditField.cpp", "c:/u/upp.src/uppsrc/Core/Core.h", include_path);
	}

	for(int i = 0; i < 1000; i++) {
		RTIMING("IncludesFile false");
		IncludesFile("c:/u/upp.src/uppsrc/CtrlLib/EditField.cpp", "c:/u/upp.src/uppsrc/Core/Core1.h", include_path);
	}

//	GetPPFile(GetDataFile("test.h")).Dump();
	return;
}

// TEST:a,b:|a|b|\n
