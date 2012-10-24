#include <Core/Core.h>

using namespace Upp;

// This is diagnostic package:
// It compiles all U++ examples using MSC8 and MINGW build methods
// or methods listed on commandline

String input = "c:/u/upp.src";
String output = "c:/upp/all";
String umk = "c:\\upp\\umk.exe ";
Vector<String> bm;

bool failed;

char *exclude[] = {
	"SQL_MYSQL", "SDLEXAMPLE", "OLECALC", // REACTIVATE LATER
	"LOG:R",
	"WINFB", "LINUXFB", "FRAMEBUFFER", "COUNTER",
};

bool IsIgnored(const String& name)
{
	return Find(exclude, exclude + __countof(exclude), ToUpper(name));
}

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
		String name = ff.GetName();
		if(ff.IsFolder() && !ff.IsHidden() &&
		   !IsIgnored(name) && !IsIgnored(name + ":" + mn) && !IsIgnored(String(nest) + ':' + name + ':' + mn)) {
			String txt;
			txt << nest << ' ' << name << ' ' << bm << ' ' << mn;
			Cout() << "  Building " << txt;
			String c;
			c << umk << nest << ' ' << name << ' ' << bm << " -" << flags;
			if(first)
				c << 'a';
			c << ' ' << outdir;
			String out;
			if(Sys(c, out)) {
				Cout() << " *** FAILED *** !\n";
				failed = true;
				LOG("FAILED: " << txt);
				LOG(out);
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
	Build("upptst", false);
	if(failed)
		Cout() << "THERE WERE ERRORS!\n";
	else
		Cout() << "OK.\n";
//	RDUMPC(failed);
}
