#include "umake.h"

bool SilentMode;

String GetUmkFile(const char *fn)
{
	return GetFileOnPath(fn,
	                     GetHomeDirFile(".upp/umk") + ';' +
	                     GetHomeDirFile(".upp/theide") + ';' +
	                     GetHomeDirFile(".upp/ide") + ';' +
	                     GetHomeDirectory());
}

String GetBuildMethodPath(String method)
{
	if(GetFileExt(method) != ".bm")
		method << ".bm";
	return GetUmkFile(method);
}

String Ide::GetDefaultMethod()
{
	return "GCC";
}

VectorMap<String, String> Ide::GetMethodVars(const String& method)
{
	VectorMap<String, String> map;
	LoadVarFile(GetBuildMethodPath(method), map);
	return map;
}

void Puts(const char *s)
{
	if(!SilentMode)
		Cout() << s;
}

int CommaSpace(int c)
{
	return c == ',' ? ' ' : c;
}

CONSOLE_APP_MAIN
{
	Ide ide;
	TheIde(&ide);
	ide.console.SetSlots(CPU_Cores());
	bool clset = false;
	const Vector<String>& arg = CommandLine();
	if(arg.GetCount() >= 2 && IsAlpha(arg[0][0]) && IsAlpha(arg[1][0])) {
		bool build = arg.GetCount() >= 3 && IsAlpha(arg[2][0]);
		for(int i = 3; i < arg.GetCount(); i++)
			if(arg[i][0] == '-') {
				String x = arg[i];
				for(int i = 1; i < x.GetCount(); i++)
					if(x[i] == 'l')
						SilentMode = true;
			}
		if(!FileExists(GetUmkFile(arg[0] + ".var"))) {
			Vector<String> h = SplitDirs(arg[0]);
			for(int i = 0; i < h.GetCount(); i++)
				h[i] = GetFullPath(h[i]);
			SetVar("UPP", Join(h, ";"), false);
			String outdir = ConfigFile("_out");
			RealizeDirectory(outdir);
			SetVar("OUTPUT", outdir, false);
		}
		else
			if(!LoadVars(arg[0])) {
				Puts("Invalid assembly\n");
				SetExitCode(2);
				return;
			}
		if(!FileExists(SourcePath(arg[1], GetFileTitle(arg[1]) + ".upp"))) {
			Puts("Package does not exist\n");
			SetExitCode(2);
			return;
		}
		if(build) {
			ide.main = arg[1];
			ide.wspc.Scan(ide.main);
			clset = true;
			bool stoponerror = false;
			const Workspace& wspc = ide.IdeWorkspace();
			if(!wspc.GetCount()) {
				Puts("Empty assembly\n");
				SetExitCode(4);
				return;
			}
			const Array<Package::Config>& f = wspc.GetPackage(0).config;
			if(f.GetCount())
				ide.mainconfigparam = f[0].param;
			String m = arg[2];
			if(GetBuildMethodPath(m).GetCount() == 0) {
				SilentMode = false;
				Puts("Invalid build method\n");
				SetExitCode(3);
				return;
			}
			ide.method <<= m;
			ide.debug.def.blitz = ide.release.def.blitz = 0;
			ide.debug.def.debug = 2;
			ide.release.def.debug = 0;
			ide.debug.package.Clear();
			ide.release.package.Clear();
			ide.debug.linkmode = ide.release.linkmode = 0;
			ide.release.createmap = ide.debug.createmap = false;
			ide.targetmode = 0;
			ide.console.console = true;
			bool clean = false;
			bool makefile = false;
			int  exporting = 0;
			String mkf;
			for(int i = 3; i < arg.GetCount(); i++)
				if(arg[i][0] == '>')
					ide.mainconfigparam = Filter(~arg[i] + 1, CommaSpace);
				else
				if(arg[i][0] == '-') {
					String x = arg[i];
					for(int i = 1; i < x.GetCount(); i++)
						switch(x[i]) {
						case 'a':
							clean = true;
							break;
						case 'r':
							ide.targetmode = 1;
							break;
						case '1':
							ide.targetmode = 2;
							break;
						case '2':
							ide.targetmode = 3;
							break;
						case 'm':
							ide.release.createmap = ide.debug.createmap = true;
							break;
						case 'b':
							ide.release.def.blitz = ide.debug.def.blitz = 1;
							break;
						case 's':
							ide.debug.linkmode = ide.release.linkmode = 1;
							break;
						case 'd':
							ide.debug.def.debug = 0;
							break;
						case 'S':
							ide.debug.linkmode = ide.release.linkmode = 2;
							break;
						case 'e':
							stoponerror = true;
							break;
						case 'M':
							makefile = true;
							break;
						case 'v':
							ide.console.verbosebuild = true;
							break;
						case 'l':
							break;
						case 'x':
							exporting = 1;
							break;
						case 'X':
							exporting = 2;
							break;
						case 'H':
							if(i + 1 < x.GetCount() && x[i + 1] >= '1' && x[i + 1] <= '9')
								ide.console.SetSlots(x[++i] - '0');
							else
								ide.console.SetSlots(1);
							break;
						default:
							SilentMode = false;
							Puts("Invalid build option(s)");
							SetExitCode(3);
							return;
						}
				}
				else {
					ide.debug.target_override = ide.release.target_override = true;
					ide.debug.target = ide.release.target = mkf = arg[i];
				}
			if(clean)
				ide.Clean();
			if(exporting) {
				mkf = GetFullPath(mkf);
				Cout() << mkf << '\n';
				RealizeDirectory(mkf);
				if(makefile)
					ide.ExportMakefile(mkf);
				else
					ide.ExportProject(mkf, exporting == 2);
			}
			else
			if(makefile) {
				ide.SaveMakeFile(IsNull(mkf) ? "Makefile" : mkf, false);
				SetExitCode(0);
			}
			else
			if(ide.Build())
				SetExitCode(0);
			else
				SetExitCode(1);
		}
	}
	else
		Puts("Invalid commandline.\n");
}