#include "Builders.h"

#include <coff/binobj/binobj.h>

void OwcBuilder::AddFlags(Index<String>& cfg)
{

}

bool OwcBuilder::BuildPackage(const String& package, Vector<String>& linkfile, String& linkoptions, const Vector<String>& all_uses, const Vector<String>& all_libraries, int opt)
{
	String packagepath = PackagePath(package);
	Package pkg;
	pkg.Load(packagepath);
	String packagedir = GetFileFolder(packagepath);
	ChDir(packagedir);
	PutVerbose("cd " + packagedir);
	IdeConsoleBeginGroup(package);
	Vector<String> obj;

	/*bool is_shared = HasFlag("SO");
	String shared_ext = (HasFlag("WIN32") ? ".dll" : ".so");*/

	String cc = CmdLine(package, pkg),
		clc = CompilerName(false), clcpp = CompilerName(true);

	// <..> Flags configuration
	if (HasFlag("MT"))
	    cc << " -bm";
#ifdef PLATFORM_WIN32
	cc << " -bt=nt";
#endif
	String cc_speed = cc;
	bool   release = false;

	if(HasFlag("DEBUG"))
		cc << " -d_DEBUG " << debug_options;
	else {
		release = true;
		cc << ' ' << release_size_options;
		cc_speed << ' ' << release_options;
		if(opt == R_SPEED || pkg.optimize_speed)
			cc = cc_speed;
	}
	// <..>

	Vector<String> sfile, isfile;
	Vector<String> soptions, isoptions;
	Vector<bool>   optimize, ioptimize;
	bool           error = false;

	for(int i = 0; i < pkg.GetCount(); i++) {
		if(!IdeIsBuilding())
			return false;
		if(!pkg[i].separator) {
			String gop = Gather(pkg[i].option, config.GetKeys());
			Vector<String> srcfile = CustomStep(SourcePath(package, pkg[i]));
			if(srcfile.GetCount() == 0)
				error = true;
			for(int j = 0; j < srcfile.GetCount(); j++) {
				String fn = srcfile[j];
				String ext = ToLower(GetFileExt(fn));
				if(ext == ".c" || ext == ".cpp" || ext == ".cc" || ext == ".cxx" || ext == ".rc" || ext == ".brc") {
					sfile.Add(fn);
					soptions.Add(gop);
					optimize.Add(release && pkg[i].optimize_speed && opt == R_OPTIMAL);
				}
				else
				if(ext == ".icpp") {
					isfile.Add(fn);
					isoptions.Add(gop);
					ioptimize.Add(release && pkg[i].optimize_speed && opt == R_OPTIMAL);
				}
				else
				if(ext == ".obj")
					obj.Add(fn);
				else
				if(ext == ".lib")
					linkfile.Add(fn);
			}
		}
	}

	if(HasFlag("BLITZ")) {
		Blitz b = BlitzStep(sfile, soptions, obj, ".obj", optimize);
		if(b.build) {
			PutConsole("BLITZ:" + b.info);
			int slot = AllocSlot();
			if(slot < 0 || !Run(String().Cat() << clcpp << cc << ' '
			<< GetHostPathQ(b.path) << " -fo=" << GetHostPathQ(b.object), slot, GetHostPath(b.object), b.count))
				error = true;
		}
	}

	int first_ifile = sfile.GetCount();
	sfile.AppendPick(isfile);
	soptions.AppendPick(isoptions);
	optimize.AppendPick(ioptimize);

	int ccount = 0;

	for(int i = 0; i < sfile.GetCount(); i++) {
		if(!IdeIsBuilding())
			return false;
		String fn = sfile[i];
		String ext = ToLower(GetFileExt(fn));
		bool rc = (ext == ".rc");
		bool brc = (ext == ".brc");
		bool init = (i >= first_ifile);
		String objfile = CatAnyPath(outdir, GetFileName(fn) + ".obj");
		if(HdependFileTime(fn) > GetFileTime(objfile)) {
			int time = GetTickCount();
			bool execerr = false;
			if(rc) {
				PutConsole(GetFileNamePos(fn));
				int slot = AllocSlot();
				if(slot < 0 || !Run("wrc -q -fo=" + GetHostPathQ(objfile) + Includes(" -i=", package, pkg)
					+ ' ' + GetHostPathQ(fn), slot, GetHostPath(objfile), 1))
					execerr = true;
			}
			else
			if(brc) {
				PutConsole(GetFileNamePos(fn));
				try {
					String hfn = GetHostPath(fn);
					String brcdata = LoadFile(hfn);
					if(brcdata.IsVoid())
						throw Exc(NFormat("error reading file '%s'", hfn));
					CParser parser(brcdata, hfn);
					BinaryToObject(GetHostPath(objfile), parser, GetFileDirectory(hfn), THISBACK(BinObjConsole));
				}
				catch(Exc e) {
					PutConsole(e);
					execerr = true;
				}
			}
			else {
				String c = cc;
				if(optimize[i])
					c = cc_speed;
				int slot = AllocSlot();

				if(slot < 0 || !Run((ext == ".c" ? clc : clcpp + " -xr -xs") + c + soptions[i] + ' '
					+ GetHostPathQ(fn) + " -fo=" + GetHostPathQ(objfile), slot, GetHostPath(objfile), 1))
					execerr = true;
			}

			if(execerr)
				DeleteFile(objfile);
			error |= execerr;
			PutVerbose("compiled in " + GetPrintTime(time));
			ccount++;
		}
		if(init)
			linkfile.Add(objfile);
		else
			obj.Add(objfile);
	}

	if(error) {
		IdeConsoleEndGroup();
		return false;
	}

	obj.Append(linkfile);
	linkfile = obj;
	return true;
}

bool OwcBuilder::Link(const Vector<String>& linkfile, const String& linkoptions, bool createmap)
{
	int time = GetTickCount();
	if(!Wait())
		return false;

	const int linkfileCount = linkfile.GetCount();

	for(int i = 0; i < linkfileCount; i++)
		if(GetFileTime(linkfile[i]) >= targettime) {
			String link;
			link << LinkerName() << " option quiet";

			if (HasFlag("DEBUG"))
			    link << " debug all option incremental";
#ifdef PLATFORM_WIN32
			if (HasFlag("GUI"))
				link << " system nt_win";
			else if (HasFlag("DLL"))
				link << " system nt_dll";
			else link << " system nt";
#endif
			if(createmap)
				link << " option map";

			const int libpathCount = libpath.GetCount();
			if (libpathCount > 0)
			{
				link << " libpath ";
				for (int j = 0, k = libpathCount - 1; j < libpathCount; ++j)
				{
					link << '\"' << libpath[j] << '\"';
					if (j < k)
						link << ';';
				}
			}

			if (!linkoptions.IsEmpty())
				link << ' ' << linkoptions;

			Vector<String> files, libs;

			for (int j = 0; j < linkfileCount; ++j)
			{
				const String ext = GetFileExt(linkfile[j]);
				if (ext == ".obj")
					files.Add(linkfile[j]);
				else if (ext == ".lib")
					libs.Add(linkfile[j]);
			}

			const int filesCount = files.GetCount(),
				libsCount = libs.GetCount();

			if (filesCount > 0)
			{
				link << " file ";

				for (int j = 0, k = filesCount - 1; j < filesCount; ++j)
				{
					link << '\'' << files[j] << '\'';
					if (j < k)
						link << ", ";
				}
			}

			if (libsCount > 0)
			{
				link << " library ";

				for (int j = 0, k = libsCount - 1; j < libsCount; ++j)
				{
					link << '\'' << GetHostPath(libs[i]) << '\'';
					if (j < k)
						link << ", ";
				}
			}

			link << " name '" << GetHostPath(target) << '\'';

			PutConsole("Linking...");
			CustomStep(".pre-link");
			if(Execute(link) == 0) {
				CustomStep(".post-link");

				PutConsole(String().Cat() << GetHostPath(target) << " (" << GetFileInfo(target).length
				           << " B) linked in " << GetPrintTime(time));
				return true;
			}
			else {
				DeleteFile(target);
				return false;
			}
		}

	PutConsole(String().Cat() << GetHostPath(target) << " (" << GetFileInfo(target).length
	           << " B) is up to date.");
	return true;
}

bool OwcBuilder::Preprocess(const String& package, const String& file, const String& target, bool asmout)
{
	FileOut out(target);
	const String packagepath = PackagePath(package);
	Package pkg;
	pkg.Load(packagepath);

	return Execute((GetFileExt(file) == ".c" ? CompilerName(false) : CompilerName(true)) + CmdLine(package, pkg) + " -pl " + file, out);
}

String OwcBuilder::CompilerName(bool isCpp) const
{
	if(!IsNull(compiler)) return compiler;
	return isCpp ? "wpp386" : "wcc386"; // C++ or C compiler
}

String OwcBuilder::LinkerName() const
{
	return "wlink";
}

String OwcBuilder::CmdLine(const String& package, const Package& pkg)
{
	return String().Cat() << " -zq -fr=" << Includes(" -i=", package, pkg);
	// "-fr=" - do not create error files
}

Builder *CreateOwcBuilder()
{
	return new OwcBuilder;
}

void RegisterOwcBuilder()
{
	RegisterBuilder("OWC", CreateOwcBuilder);
	//RegisterBuilder("OWC32", CreateOwcBuilder);
}
