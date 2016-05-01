#include <Core/Core.h>
#include <plugin/zip/zip.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	FileZip zip(GetHomeDirFile("test.zip"));
	String s = LoadDataFile("Zip.cpp");
	zip.WriteFile(s, "zip.cpp");
	
	zip.WriteFile(s, "zip2.cpp", Gate2<int, int>(), GetSysTime(), false);
	
	zip.BeginFile("test.txt", GetSysTime(), false);
	String h = "Just a test here!";
	h.Cat(' ', 1000);
	zip.Put(~h, h.GetCount());
	zip.EndFile();
}
