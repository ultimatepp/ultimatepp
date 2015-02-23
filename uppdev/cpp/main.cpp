#include "cpp.h"

Vector<String> errs;

void AddError(int ln, const String& s)
{
	errs.Add(AsString(ln) + ": " + s);
}

void Test(const char *path)
{
	CppBase base;

	Cpp cpp;
	cpp.filedir = GetFileFolder(path);
	cpp.include_path = "C:\\Program Files (x86)\\Microsoft Visual Studio 10.0\\Vc\\Include;C:\\Program Files\\Microsoft SDKs\\Windows\\v7.1\\Include;C:\\OpenSSL-Win32\\include;C:\\u\\pgsql\\include;C:\\u\\OpenSSL-Win32\\include";
	FileIn in(path);
	Index<String> inc;
	String pp = cpp.Do(in, inc);
	SaveFile(GetHomeDirFile(GetFileTitle(path) + ".pp"), pp);
	StringStream ss(pp);
	Parse(ss, Vector<String>() << "__cdecl", base, path, callback(AddError));
	DLOG("=======================");
	DUMPC(inc);
	DLOG("=======================");
	DUMPC(errs);
	DLOG("=======================");
	Qualify(base);
	String out;
	for(int i = 0; i < base.GetCount(); i++) {
		out << Nvl(base.GetKey(i), "<globals>") << " {\n";
		const Array<CppItem>& ma = base[i];
		for(int j = 0; j < ma.GetCount(); j++) {
			const CppItem& m = ma[j];
			out << '\t' << CppItemKindAsString(m.kind) << ' ' << m.qitem << ' ' << m.line << "\n";
		}
		out << "}\n";
	}
	LOG(out);
}

CONSOLE_APP_MAIN
{
/*
	{
		CppMacro m;
		m.body = "a + b * 123 - alfa";
		m.param.Add("a");
		m.param.Add("b");
		Vector<String> x;
		x.Add("one");
		x.Add("two");
		DUMP(m.Expand(x));
	}

	Cpp x;
	x.Define("TEST(a, b) |a|b|");
	DUMP(x.Expand("This is: TEST(('\\1'   , \"2\"    \"4\" '4' ',' \",\"), 3)!"));
	DUMP(x.Expand("This is: TEST(1, 2)!"));
	DUMP(x.Expand("This is: TEST(1   +   a, 2)!"));
	DUMP(x.Expand("This is: TEST((1   , 2), 3)!"));
	DUMP(x.Expand("This is: TEST(  \"1\" \"2\" \",\", ',')!"));
*/
//	Test("C:/OpenSSL-Win32/include/openssl/ssl.h");
//	Test("C:/Program Files/Microsoft SDKs/Windows/v7.1/Include/WinBase.h");
//	Test("C:\\Program Files (x86)\\Microsoft Visual Studio 10.0\\Vc\\Include\\string.h");
	Test("C:/Program Files (x86)/Microsoft Visual Studio 10.0/VC/include/vector");
	return;
	Cpp x;
	x.filedir = GetFileFolder(GetDataFile("x"));
	x.include_path = "C:\\Program Files (x86)\\Microsoft Visual Studio 10.0\\Vc\\Include;C:\\Program Files\\Microsoft SDKs\\Windows\\v7.1\\Include;C:\\OpenSSL-Win32\\include;C:\\u\\pgsql\\include;C:\\u\\OpenSSL-Win32\\include";
//	FileIn in0("C:\\Program Files (x86)\\Microsoft Visual Studio 10.0\\Vc\\Include\\crtdefs.h");
//	x.Preprocess(in0);
//	FileIn in("C:\\Program Files (x86)\\Microsoft Visual Studio 10.0\\Vc\\Include\\string.h");
//	FileIn in(GetDataFile("test.h"));
	FileIn in("C:/OpenSSL-Win32/include/openssl/ssl.h");
//	FileIn in();
	Index<String> inc;
	LOG(x.Do(in, inc));
	LOG("---------------------------");
	DUMPM(x.macro);
	LOG("---------------------------");
	DUMPC(x.usedmacro);
	LOG("---------------------------");
	DUMPC(x.notmacro);
	LOG("---------------------------");
	DUMPC(inc);
}

// TEST:a,b:|a|b|\n
