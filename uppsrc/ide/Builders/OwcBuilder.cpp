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

	bool is_shared = HasFlag("SO");

	String cc = CmdLine(package, pkg),
		clc = CompilerName(false), clcpp = CompilerName(true);

	if (HasFlag("MT"))
	    cc << " -bm";

	if (HasFlag("WIN32"))
		cc << " -bt=nt";

	String cc_speed = cc;
	bool   release = false;

	if (HasFlag("DEBUG"))
		cc << " -d_DEBUG " << debug_options;
	else {
		release = true;
		cc << ' ' << release_size_options;
		cc_speed << ' ' << release_options;
		if (opt == R_SPEED || pkg.optimize_speed)
			cc = cc_speed;
	}

	Vector<String> sfile, isfile;
	Vector<String> soptions, isoptions;
	Vector<bool>   optimize, ioptimize;
	bool           error = false;

	for (int i = 0; i < pkg.GetCount(); i++) {
		if (!IdeIsBuilding())
			return false;
		if (!pkg[i].separator) {
			String gop = Gather(pkg[i].option, config.GetKeys());
			Vector<String> srcfile = CustomStep(SourcePath(package, pkg[i]));
			if (srcfile.GetCount() == 0)
				error = true;
			for (int j = 0; j < srcfile.GetCount(); j++) {
				String fn = srcfile[j];
				String ext = ToLower(GetFileExt(fn));
				if (ext == ".c" || ext == ".cpp" || ext == ".cc" || ext == ".cxx" || ext == ".rc" || ext == ".brc") {
					sfile.Add(fn);
					soptions.Add(gop);
					optimize.Add(release && pkg[i].optimize_speed && opt == R_OPTIMAL);
				}
				else
				if (ext == ".icpp") {
					isfile.Add(fn);
					isoptions.Add(gop);
					ioptimize.Add(release && pkg[i].optimize_speed && opt == R_OPTIMAL);
				}
				else
				if (ext == ".obj")
					obj.Add(fn);
				else
				if (ext == ".lib")
					linkfile.Add(fn);
			}
		}
	}

	if (HasFlag("BLITZ")) {
		Blitz b = BlitzStep(sfile, soptions, obj, ".obj", optimize);
		if (b.build) {
			PutConsole("BLITZ:" + b.info);
			int slot = AllocSlot();
			if (slot < 0 || !Run(String().Cat() << clcpp << cc << ' '
			<< GetHostPathQ(b.path) << " -fo=" << GetHostPathQ(b.object), slot, GetHostPath(b.object), b.count))
				error = true;
		}
	}

	int first_ifile = sfile.GetCount();
	sfile.AppendPick(isfile);
	soptions.AppendPick(isoptions);
	optimize.AppendPick(ioptimize);

	int ccount = 0;

	for (int i = 0; i < sfile.GetCount(); ++i) {
		if (!IdeIsBuilding())
			return false;
		String fn = sfile[i];
		String ext = ToLower(GetFileExt(fn));
		bool rc = (ext == ".rc");
		bool brc = (ext == ".brc");
		bool init = (i >= first_ifile);
		String objfile = CatAnyPath(outdir, GetFileName(fn) + ".obj");
		if (HdependFileTime(fn) > GetFileTime(objfile)) {
			int time = GetTickCount();
			bool execerr = false;
			if (rc) {
				PutConsole(GetFileNamePos(fn));
				int slot = AllocSlot();
				if (slot < 0 || !Run("wrc -q -fo=" + GetHostPathQ(objfile) + Includes(" -i=", package, pkg)
					+ ' ' + GetHostPathQ(fn), slot, GetHostPath(objfile), 1))
					execerr = true;
			}
			else
			if (brc) {
				PutConsole(GetFileNamePos(fn));
				try {
					String hfn = GetHostPath(fn);
					String brcdata = LoadFile(hfn);
					if(brcdata.IsVoid())
						throw Exc(NFormat("error reading file '%s'", hfn));
					CParser parser(brcdata, hfn);
					BinaryToObject(GetHostPath(objfile), parser, GetFileDirectory(hfn), THISBACK(BinObjConsole));
				}
				catch (Exc e) {
					PutConsole(e);
					execerr = true;
				}
			}
			else {
				String c = cc;
				if (optimize[i])
					c = cc_speed;
				int slot = AllocSlot();

				if (slot < 0 || !Run((ext == ".c" ? clc : clcpp + " -xr -xs") + c + soptions[i] + ' '
					+ GetHostPathQ(fn) + " -fo=" + GetHostPathQ(objfile), slot, GetHostPath(objfile), 1))
					execerr = true;
			}

			if (execerr)
				DeleteFile(objfile);
			error |= execerr;
			PutVerbose("compiled in " + GetPrintTime(time));
			ccount++;
		}
		if (init)
			linkfile.Add(objfile);
		else
			obj.Add(objfile);
	}

	if (error) {
		IdeConsoleEndGroup();
		return false;
	}

	Vector<String> libs = Split(Gather(pkg.library, config.GetKeys()), ' ');
	for (int i = 0, n = libs.GetCount(); i < n; ++i)
		linkfile.Add(AppendExt(libs[i], ".lib"));

	linkoptions << Gather(pkg.link, config.GetKeys());
	if (linkoptions.GetCount() > 0)
		linkoptions << ' ';

	int linktime = GetTickCount();
	if (!HasFlag("MAIN")) {
		if (HasFlag("BLITZ") || HasFlag("NOLIB")) {
			obj.Append(linkfile);
			linkfile = obj;
			IdeConsoleEndGroup();
			return true;
		}

		String product;
		if (is_shared)
			product = GetSharedLibPath(package);
		else
			product = CatAnyPath(outdir, GetAnyFileName(package) + ".lib");

		Time producttime = GetFileTime(product);
		linkfile.Add(ForceExt(product, ".lib"));

		if (!Wait()) {
			IdeConsoleEndGroup();
			return false;
		}

		for (int i = 0, n = obj.GetCount(); i < n; ++i)
			if (GetFileTime(obj[i]) > producttime) {
				String lib;

				if (is_shared) {
					lib << LinkerName() << " option quiet";
		
					if (HasFlag("DEBUG"))
					    lib << " debug all option incremental";

					if (HasFlag("WIN32"))
						lib << " system nt_dll";

					const int libpathCount = libpath.GetCount();
					if (libpathCount > 0) {
						lib << " libpath ";
						for (int j = 0, k = libpathCount - 1; j < libpathCount; ++j) {
							lib << '\"' << libpath[j] << '\"';
							if (j < k)
								lib << ';';
						}
					}

					if (!linkoptions.IsEmpty())
						lib << ' ' << linkoptions;

					lib << " file ";

					for (int j = 0, k = n - 1; j < n; ++j) {
						lib << '\'' << obj[j] << '\'';
						if (j < k)
							lib << ", ";
					}

					const int all_usesCount = all_uses.GetCount(),
						all_librariesCount = all_libraries.GetCount();

					if (all_usesCount > 0 || all_librariesCount > 0) {
						lib << " library ";

						for (int j = 0, k = all_usesCount - 1; j < all_usesCount; ++j) {
							lib << '\'' << GetHostPath(ForceExt(GetSharedLibPath(all_uses[j]), ".lib")) << '\'';
							if (j < k)
								lib << ", ";
						}

						if (all_usesCount > 0 && all_librariesCount > 0)
							lib << ", ";

						for (int j = 0, k = all_librariesCount - 1; j < all_librariesCount; ++j) {
							String libfile(AppendExt(all_libraries[j], ".lib"));
							if (!IsFullPath(libfile)) {
								for (int p = 0; p < libpath.GetCount(); ++p) {
									String nf(NormalizePath(libfile, libpath[p]));

									if (FileExists(nf)) {
										libfile = nf;
										break;
									}
								}
							}

							lib << '\'' << GetHostPath(libfile) << '\'';

							if (j < k)
								lib << ", ";
						}
					}

					lib << " name '" << product << '\'';
				}
				else {
					lib << "wlib -q -b -n -pa ";

					if (!linkoptions.IsEmpty())
						lib << linkoptions;

					lib << '\'' << product << '\'';

					for (int j = 0; j < n; ++j)
						lib << " '" << GetHostPath(obj[j]) << '\'';
				}

				PutConsole("Creating library...");
				IdeConsoleEndGroup();
				DeleteFile(product);

				if (Execute(lib)) {
					DeleteFile(product);
					return false;
				}

				PutConsole(String().Cat() << product << " (" << GetFileInfo(product).length
				           << " B) created in " << GetPrintTime(linktime));

				break;
			}

		return true;
	}

	obj.Append(linkfile);
	linkfile = obj;
	return true;
}

bool OwcBuilder::Link(const Vector<String>& linkfile, const String& linkoptions, bool createmap)
{
	int time = GetTickCount();
	if (!Wait())
		return false;

	const int linkfileCount = linkfile.GetCount();

	for (int i = 0; i < linkfileCount; i++)
		if (GetFileTime(linkfile[i]) >= targettime) {
			String link;
			link << LinkerName() << " option quiet";

			if (HasFlag("DEBUG"))
			    link << " debug all option incremental";

			if (HasFlag("WIN32"))
				if (HasFlag("GUI"))
					link << " system nt_win";
				else if (HasFlag("DLL"))
					link << " system nt_dll";
				else link << " system nt";

			if (createmap)
				link << " option map";

			const int libpathCount = libpath.GetCount();
			if (libpathCount > 0) {
				link << " libpath ";
				for (int j = 0, k = libpathCount - 1; j < libpathCount; ++j) {
					link << '\"' << libpath[j] << '\"';
					if (j < k)
						link << ';';
				}
			}

			if (!linkoptions.IsEmpty())
				link << ' ' << linkoptions;

			int b = -1;
			for (int j = linkfileCount - 1; j >= 0; --j)
				if (GetFileExt(linkfile[j]) != ".lib") { b = j; break; }

			if (b >= 0) {
				link << " file ";

				for (int j = 0, k = b - 1; j <= b; ++j) {
					link << '\'' << linkfile[j] << '\'';
					if (j <= k)
						link << ", ";
				}
			}

			if (b < linkfileCount - 1) {
				link << " library ";

				for (int j = b + 1, k = linkfileCount - 1; j < linkfileCount; ++j) {
					link << '\'' << GetHostPath(linkfile[j]) << '\'';
					if (j < k)
						link << ", ";
				}
			}

			link << " name '" << GetHostPath(target) << '\'';

			PutConsole("Linking...");
			CustomStep(".pre-link");
			if (Execute(link) == 0) {
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

String OwcBuilder::IncludesDefinesTargetTime(const String& package, const Package& pkg)
{
	String cc = Includes(" -i=", package, pkg);

	for (int i = 0; i < config.GetCount(); i++)
		cc << " -Dflag" << config[i];

	Time t = GetSysTime();
	cc << " -DbmYEAR=" << (int)t.year;
	cc << " -DbmMONTH=" << (int)t.month;
	cc << " -DbmDAY=" << (int)t.day;
	cc << " -DbmHOUR=" << (int)t.hour;
	cc << " -DbmMINUTE=" << (int)t.minute;
	cc << " -DbmSECOND=" << (int)t.second;
	targettime = GetFileTime(target);

	return cc;
}

String OwcBuilder::CompilerName(bool isCpp) const
{
	if (!IsNull(compiler)) return compiler;
	return isCpp ? "wpp386" : "wcc386"; // C++ or C compiler
}

String OwcBuilder::LinkerName() const
{
	return "wlink";
}

String OwcBuilder::CmdLine(const String& package, const Package& pkg)
{
	return String().Cat() << " -zq -fr=" << IncludesDefinesTargetTime(package, pkg);
	// "-fr=" - do not create error files
}

Builder *CreateOwcBuilder()
{
	return new OwcBuilder;
}

void RegisterOwcBuilder()
{
	RegisterBuilder("OWC", CreateOwcBuilder);
}
