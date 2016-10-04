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

void Test(const char *sourcefile, const char *currentfile)
{
	DUMP(sourcefile);
	DUMP(currentfile);
	Cpp cpp;
	cpp.include_path = include_path;
	FileIn in(sourcefile);
	{ RTIMING("Preprocess");
		cpp.Preprocess(sourcefile, in, currentfile);
		DUMP(cpp.namespace_stack);
<<<<<<< .mine
		DUMP(cpp.output);
=======
>>>>>>> .r8434
		DUMP(cpp.usedmacro);
<<<<<<< .mine
		DUMP(cpp.definedmacro);
=======
		DUMP(cpp.macro.GetCount());
>>>>>>> .r8434
	}
	LOG("=================================");
<<<<<<< .mine
/*
=======
	LOG(cpp.output);

>>>>>>> .r8434
/*
	{
		Cpp cpp2;
		cpp2.include_path = include_path;
		cpp2.Preprocess(sourcefile, NilStream(), currentfile, &cpp.usedmacro);
		DUMP(cpp2.macro);
	}
*/
	for(int i = 0; i < 100; i++)
	{
		RTIMING("GetMacros");
		Cpp cpp2;
		cpp2.include_path = include_path;
		cpp2.Preprocess(sourcefile, NilStream(), currentfile, &cpp.usedmacro);
	}
*/
}

void TestC(const char *ln)
{
	for(int q = 0; q < 2; q++) {
		bool incomment = q;
		String l = ln;
		DUMP(incomment);
		LOG(l);
		RemoveComments(l, incomment);
		LOG(l);
		LOG(incomment);
		LOG("-----------------------");
	}
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

	GetPPFile("C:\\u\\upp.src\\uppsrc\\Core\\Core.h").Dump();
	DLOG("==============");
	Test("C:\\u\\upp.src\\uppsrc\\Core\\Format.h", "C:\\u\\upp.src\\uppsrc\\Core\\Format.cpp");

	return;
<<<<<<< .mine
#endif	
	{
		RTIMING("Pass1");
		Test("C:\\u\\upp.src\\uppsrc\\Core\\Topt.h", "C:\\u\\upp.src\\uppsrc\\Core\\Format.cpp");
	}
#ifndef _DEBUG
	for(int i = 0; i < 100; i++) {
		Test("C:\\u\\upp.src\\uppsrc\\Core\\Format.h", "C:\\u\\upp.src\\uppsrc\\Core\\Format.cpp");
	}
#endif
#if 0
	{
		RTIMING("Pass1");
		Index<String> visited;
//		RecursePP("c:/u/upp.src/uppsrc/CtrlLib/EditField.cpp", include_path, visited);
	}
	
	DDUMP(IncludesFile("c:/u/upp.src/uppsrc/CtrlLib/EditField.cpp", "c:/u/upp.src/uppsrc/Core/Core.h", include_path));
	DDUMP(IncludesFile("c:/u/upp.src/uppsrc/CtrlLib/EditField.cpp", "c:/u/upp.src/uppsrc/Core/Core2.h", include_path));
	
	for(int i = 0; i < 1000; i++) {
		RTIMING("IncludesFile true");
		IncludesFile("c:/u/upp.src/uppsrc/CtrlLib/EditField.cpp", "c:/u/upp.src/uppsrc/Core/Core.h", include_path);
	}

	for(int i = 0; i < 1000; i++) {
		RTIMING("IncludesFile false");
		IncludesFile("c:/u/upp.src/uppsrc/CtrlLib/EditField.cpp", "c:/u/upp.src/uppsrc/Core/Core1.h", include_path);
	}
#endif
//	GetPPFile(GetDataFile("test.h")).Dump();
	return;
=======
>>>>>>> .r8434
}

// TEST:a,b:|a|b|\n
