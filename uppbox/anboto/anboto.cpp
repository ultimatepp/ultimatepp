#include <Core/Core.h>

using namespace Upp;

String Anboto = "D:/anboto/Anboto";
String UppHub = "D:/anboto/UppHub";

static
void CopyFolder(const char *dst, const char *src)
{
	RealizeDirectory(dst);
	FindFile ff(String(src) + "/*.*");
	while(ff) {
		String s = AppendFileName(src, ff.GetName());
		String d = AppendFileName(dst, ff.GetName());
		if(ff.IsFile()) {
			FileIn in(s);
			FileOut out(d);
			CopyStream(out, in);
		}
		else
		if(ff.IsFolder())
			CopyFolder(d, s);
		ff.Next();
	}
}

void Copy(const char *src, const char *tgt)
{
	LOG(src << " -> " << tgt);
	CopyFolder(UppHub + "/" + tgt, Anboto + "/" + src);
}

Index<String> modules;

void Main(String pkg, String tgt)
{
	modules.FindAdd(tgt);
	Copy(pkg, tgt + '/' + pkg);
}

void Main(const char *pkg)
{
	Main(pkg, pkg);
}

void Plugin(String pkg, String tgt)
{
	modules.FindAdd(tgt);
	Copy("plugin/" + pkg, tgt + "/plugin/" + pkg);
}

void Plugin(String pkg)
{
	Plugin(pkg, pkg);
}

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	DeleteFolderDeep(UppHub);
	RealizeDirectory(UppHub);
	
	Main("SysInfo");
	Main("Functions4U");
	Main("Controls4U");
	Main("Surface");
	Main("OfficeAutomation");
	Main("ScatterCtrl", "Scatter");
	Main("ScatterDraw", "Scatter");
	Plugin("assimp");
	Plugin("sundials");
	Plugin("matio");
	Main("Matio", "matio");
	Plugin("eigen");
	Main("Eigen", "eigen");
	Main("GLCanvas");
	Main("STEM4U");
	
	for(FindFile ff(Anboto + "/examples/*.*"); ff; ff++) {
		if(ff.IsFolder()) {
			String title = GetFileTitle(ff.GetPath());
			for(String m : modules) {
				if(title.StartsWith(m))
					Copy(ff.GetPath(), m + "/examples");
			}
		}
	}
}
