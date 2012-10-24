#include <Core/Core.h>

using namespace Upp;

// This is diagnostic package:
// It compiles all U++ examples using MSC8 and MINGW build methods
// or methods listed on commandline

String input = "c:/u/upp.src";
String output = "c:/upp/all";
String umk = "c:\\upp\\umk.exe ";
Vector<String> bm;

Vector<String> failed;

void Build(const char *nest, const char *bm, bool release)
{
	String flags = release ? "r" : "b";
	String mn = release ? "R" : "D";
	String n = String().Cat() << nest << '-' << bm << '-' << mn;
	Cout() << n << '\n';
	String outdir = AppendFileName(output, n);
	DeleteFolderDeep(outdir);
	RealizeDirectory(outdir);
	FindFile ff(AppendFileName(AppendFileName(input, nest), "*.*"));
	bool first = true;
	while(ff) {
		if(ff.IsFolder() && !ff.IsHidden()) {
			String txt;
			txt << nest << ' ' << ff.GetName() << ' ' << bm << ' ' << mn;
			Cout() << "  Building " << txt;
			String c;
			c << umk << nest << ' ' << ff.GetName() << ' ' << bm << " -" << flags;
			if(first)
				c << 'a';
			c << ' ' << outdir;
			String out;
			if(Sys(c, out)) {
				Cout() << " *** FAILED *** !\n";
				LOG("FAILED: " << txt);
				LOG(out);
				failed.Add() << txt;
			}
			else {
				Cout() << " ok\n";
				first = false;
			}
		}
		DeleteFile(AppendFileName(outdir, ff.GetName() + ".ilk"));
		DeleteFile(AppendFileName(outdir, ff.GetName() + ".pdb"));
		ff.Next();
	}
}

void Build(const char *nest, bool release)
{
	for(int i = 0; i < bm.GetCount(); i++)
		Build(nest, bm[i], release);
}

void Build(const char *nest)
{
	Build(nest, false);
	Build(nest, true);
}

CONSOLE_APP_MAIN
{
	const Vector<String>& arg = CommandLine();
	input = GetFileFolder(GetFileFolder(GetFileFolder(GetDataFile("BuildAll.cpp"))));
	output = "C:\\out";
	for(int i = 0; i < arg.GetCount(); i++)
		bm.Add(arg[i]);
	if(bm.GetCount() == 0) {
//		bm.Add("MSC71cdb");
		bm.Add("MSC9");
//		bm.Add("MINGWI2");
	}
	Build("examples");
	Build("reference");
	Build("tutorial");
	Build("upptst");
	if(failed.GetCount()) {
		Cout() << "***** Failed builds: \n";
		for(int i = 0; i < failed.GetCount(); i++)
			Cout() << "  " << failed[i] << '\n';
	}
//	RDUMPC(failed);
}
