#include <Core/Core.h>

using namespace Upp;

#include "Spreadsheet.h"


static bool end = false;

void TestGetchar()
{
	if (end)
		getchar();
}

void Puts(String s)
{
	puts(s);
	String file = AppendFileName(GetDesktopFolder(), "log");
	SaveFile(file, LoadFile(file) + "\n" + s);
}

void SpreadsheetDemo(Spreadsheet &spreadsheet) {
	spreadsheet.Open("myfile.xls");
	spreadsheet.SetData(4, 6, "Hello world");
}

void PluginDemo() {
	Spreadsheet spreadsheet;
	
	if (!PluginInit(spreadsheet, "Open")) {
		Puts("Impossible to init OpenOffice/LibreOffice");
		return;
	}
	SpreadsheetDemo(spreadsheet);
	if (!PluginInit(spreadsheet, "Excel")) {
		Puts("Impossible to init Excel");
		return;
	}
	SpreadsheetDemo(spreadsheet);
}

void FilesDemo() {
	String filename1 = AppendFileName(GetDesktopFolder(), "Demo", "file1.txt");
	RealizePath(filename1);
	String str1 = "This is the First string";
	SaveFile(filename1, str1);
	String filename2 = AppendFileName(GetDesktopFolder(), "Demo", "file2.txt");
	String str2 = "This is the Second string";
	SaveFile(filename2, str2);
	
	FileCat(filename1, filename2);
	Puts(Format("\nFileCat(%s, %s)", filename1, filename2));

//	LaunchFile(filename1);	
//	Puts(Format("LaunchFile(%s)", filename1));		

	int intres = FileCompare(filename1, filename2);
	Puts(Format("\nFileCompare(%s, %s) = %d (has to be -1)", filename1, filename2, intres));

	int64 pos = FindStringInFile(filename1, "Second");
	Puts(Format("\nFindStringInFile(%s, %s) = %d (has to be 35)", filename1, filename2, pos));

	bool boolres = FileStrAppend(filename1, ". Text appended at the end.");
	Puts(Format("\nFileStrAppend(%s, \". Text appended at the end.\") = %s (has to be true)", 
				filename1, boolres ? "true" : "false"));
	
	//boolres = UpperFolder(GetFileDirectory(filename1));
	//Puts(Format("\nUpperFolder(%s) = %s (has to be true)", filename1, boolres ? "true" : "false"));
	
	String upperFolder = GetUpperFolder(GetFileDirectory(filename1));
	Puts(Format("\nGetUpperFolder(%s) = %s (has to be %s)", filename1, upperFolder, GetDesktopFolder()));
	
	String stringres = GetNextFolder(GetUpperFolder(GetDesktopFolder()), GetFileDirectory(filename1));
	Puts(Format("\nGetNextFolder(%s, %s) = %s (has to be %s)", GetUpperFolder(GetDesktopFolder()), 
										GetFileDirectory(filename1), stringres, GetDesktopFolder()));
	
	String lfilename1 = ToLower(filename1);
	filename1 = FileRealName(lfilename1);
	Puts(Format("\nFileRealName(%s) = %s", lfilename1, filename1));
}

void NonReentrantDemo() {
	Puts("Trying to enter Non reentrant.");
	NON_REENTRANT_V;
	
	Puts("Entered in Non reentrant. It has to be once.");
	NonReentrantDemo();	
	NonReentrantDemo();
}

void Test() {
	Puts("Files demo");
	FilesDemo();
	
	Puts("\nPlugin demo");
	PluginDemo();	

	Puts("\nNonReentrant demo");
	NonReentrantDemo();	
	
	Puts("\nPress enter to end...");	TestGetchar();
}

CONSOLE_APP_MAIN
{	
	FileDelete(AppendFileName(GetDesktopFolder(), "log"));

	Puts("Introduce number of test cycles or just type enter to run it once: ");
	char str[50];
	fgets(str, 49, stdin);
	int numTests = atoi(str);
	for (int i = 0; i < numTests; ++i)
		Test();
	end = true;
	Test();
}

