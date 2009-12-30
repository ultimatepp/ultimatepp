#include "Builders.h"

#include <coff/binobj/binobj.h>
#include <plugin/bz2/bz2.h>

void   GccBuilder::AddFlags(Index<String>& cfg)
{
}

String GccBuilder::CompilerName() const
{
	if(!IsNull(compiler)) return compiler;
	return HasFlag("GCC_ARM") ? "arm-wince-pe-c++" : "c++";
}

String GccBuilder::CmdLine(const String& package, const Package& pkg)
{
	String cc = CompilerName();
	cc << " -c ";
	cc << IncludesDefinesTargetTime(package, pkg);
	if(HasFlag("GCC32"))
		cc << " -m32";
	return cc;
}

void GccBuilder::BinaryToObject(String objfile, CParser& binscript, String basedir,
                                const String& package, const Package& pkg)
{
	BinObjInfo info;
	info.Parse(binscript, basedir);
	String fo;
	for(int i = 0; i < info.blocks.GetCount(); i++) {
		String ident = info.blocks.GetKey(i);
		ArrayMap<int, BinObjInfo::Block>& belem = info.blocks[i];
		if(belem[0].flags & (BinObjInfo::Block::FLG_ARRAY | BinObjInfo::Block::FLG_MASK)) {
			int count = Max(belem.GetKeys()) + 1;
			Vector<BinObjInfo::Block *> blockref;
			blockref.SetCount(count, 0);
			for(int a = 0; a < belem.GetCount(); a++) {
				BinObjInfo::Block& b = belem[a];
				blockref[b.index] = &b;
			}
			for(int i = 0; i < blockref.GetCount(); i++)
				if(blockref[i]) {
					BinObjInfo::Block& b = *blockref[i];
					fo << "static char " << ident << "_" << i << "[] =\n";
					String data = ::LoadFile(b.file);
					if(data.IsVoid())
						throw Exc(NFormat("Error reading file '%s'", b.file));
					if(data.GetLength() != b.length)
						throw Exc(NFormat("length of file '%s' changed (%d -> %d) during object creation",
							b.file, b.length, data.GetLength()));
					switch(b.encoding) {
						case BinObjInfo::Block::ENC_BZ2: data = BZ2Compress(data); break;
						case BinObjInfo::Block::ENC_ZIP: data = ZCompress(data); break;
					}
					b.length = data.GetLength();
					data.Cat('\0');
					fo << AsCString(data, 70) << ";\n\n";
				}

			fo << "int " << ident << "_count = " << blockref.GetCount() << ";\n\n"
			"int " << ident << "_length[] = {\n";
			for(int i = 0; i < blockref.GetCount(); i++)
				fo << '\t' << (blockref[i] ? blockref[i]->length : -1) << ",\n";
			fo << "};\n\n"
			"char *" << ident << "[] = {\n";
			for(int i = 0; i < blockref.GetCount(); i++)
				if(blockref[i])
					fo << '\t' << ident << '_' << i << ",\n";
				else
					fo << "\t0,\n";
			fo << "};\n\n";
			if(belem[0].flags & BinObjInfo::Block::FLG_MASK) {
				fo << "char *" << ident << "_files[] = {\n";
				for(int i = 0; i < blockref.GetCount(); i++)
					fo << '\t' << AsCString(blockref[i] ? GetFileName(blockref[i]->file) : String(Null)) << ",\n";
				fo << "};\n\n";
			}
		}
		else {
			BinObjInfo::Block& b = belem[0];
			fo << "char *" << ident << " =\n";
			String data = ::LoadFile(b.file);
			if(data.IsVoid())
				throw Exc(NFormat("Error reading file '%s'", b.file));
			if(data.GetLength() != b.length)
				throw Exc(NFormat("length of file '%s' changed (%d -> %d) during object creation",
					b.file, b.length, data.GetLength()));
			switch(b.encoding) {
				case BinObjInfo::Block::ENC_BZ2: data = BZ2Compress(data); break;
				case BinObjInfo::Block::ENC_ZIP: data = ZCompress(data); break;
			}
			int b_length = data.GetLength();
			data.Cat('\0');
			fo << AsCString(data, 70) << ";\n\n"
			"int " << ident << "_length = " << b_length << ";\n\n";
		}
	}

	String tmpfile = ForceExt(objfile, ".c");
	SaveFile(tmpfile, fo);
	String cc = CmdLine(package, pkg);
	cc << " -c -o " << GetHostPathQ(objfile) << " -x c " << GetHostPathQ(tmpfile);
	int slot = AllocSlot();
	if(slot < 0 || !Run(cc, slot, objfile, 1))
		throw Exc(NFormat("Error compiling binary object '%s'.", objfile));
}

bool GccBuilder::BuildPackage(const String& package, Vector<String>& linkfile,
	String& linkoptions, const Vector<String>& all_uses, const Vector<String>& all_libraries,
	int opt)
{
	int i;
	String packagepath = PackagePath(package);
	Package pkg;
	pkg.Load(packagepath);
	String packagedir = GetFileFolder(packagepath);
	ChDir(packagedir);
	PutVerbose("cd " + packagedir);
	IdeConsoleBeginGroup(package);
	Vector<String> obj;

	bool is_shared = HasFlag("SO");
	String shared_ext = (HasFlag("WIN32") ? ".dll" : ".so");

	String cc = CmdLine(package, pkg);
	if(HasFlag("WIN32") && HasFlag("MT"))
		cc << " -mthreads";
	if(HasFlag("DEBUG_MINIMAL"))
		cc << (HasFlag("WIN32") ? " -g1" : " -ggdb -g1");
	if(HasFlag("DEBUG_FULL"))
		cc << (HasFlag("WIN32") ? " -g2" : " -ggdb -g2");
	String fuse_cxa_atexit;
	if(is_shared /*&& !HasFlag("MAIN")*/) {
		cc << " -shared -fPIC";
		fuse_cxa_atexit = " -fuse-cxa-atexit";
	}
	if(!HasFlag("SHARED") && !is_shared)
		cc << " -static ";
//	else if(!HasFlag("WIN32")) // TRC 05/03/08: dynamic fPIC doesn't seem to work in MinGW
//		cc << " -dynamic -fPIC "; // TRC 05/03/30: dynamic fPIC doesn't seem to work in GCC either :-)
	cc << ' ' << Gather(pkg.option, config.GetKeys());
	cc << " -fexceptions ";

	if (HasFlag("OSX11")) {
	  if (HasFlag("POWERPC"))
		cc << " -arch ppc";
	  if (HasFlag("X86"))
		cc << " -arch i386";
	}

	if(HasFlag("SSE2"))
		cc << " -msse2 -mfpmath=sse";

	String cc_speed = cc;
	bool   release = false;

	if(HasFlag("DEBUG"))
		cc << " -D_DEBUG " << debug_options;
	else {
		release = true;
		cc << ' ' << release_size_options;
		cc_speed << ' ' << release_options;
		if(opt == R_SPEED || pkg.optimize_speed)
			cc = cc_speed;
	}

	Vector<String> sfile, isfile;
	Vector<String> soptions, isoptions;
	Vector<bool>   optimize, ioptimize;
	bool           error = false;

	for(i = 0; i < pkg.GetCount(); i++) {
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
				if(ext == ".c" || ext == ".cpp" || ext == ".cc" || ext == ".cxx"
				|| ext == ".s" || ext == ".S"
				|| ext == ".brc" || (ext == ".rc" && HasFlag("WIN32"))) {
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
				if(ext == ".o")
					obj.Add(fn);
				else
				if(ext == ".a" || ext == ".so")
					linkfile.Add(fn);
			}
		}
	}

	if(HasFlag("BLITZ")) {
		Blitz b = BlitzStep(sfile, soptions, obj, ".o", optimize);
		if(b.build) {
			PutConsole("BLITZ:" + b.info);
			int slot = AllocSlot();
			if(slot < 0 || !Run(String().Cat() << cc << ' '
			<< GetHostPathQ(b.path) << " -o " << GetHostPathQ(b.object), slot, GetHostPath(b.object), b.count))
				error = true;
		}
	}

	int first_ifile = sfile.GetCount();
	sfile.AppendPick(isfile);
	soptions.AppendPick(isoptions);
	optimize.AppendPick(ioptimize);

	int ccount = 0;
	for(i = 0; i < sfile.GetCount(); i++) {
		if(!IdeIsBuilding())
			return false;
		String fn = sfile[i];
		String ext = ToLower(GetFileExt(fn));
		bool rc = (ext == ".rc");
		bool brc = (ext == ".brc");
		bool init = (i >= first_ifile);
		String objfile = CatAnyPath(outdir, GetFileTitle(fn) + (rc ? "$rc.o" : brc ? "$brc.o" : ".o"));
		if(HdependFileTime(fn) > GetFileTime(GetHostPath(objfile))) {
			PutConsole(GetFileName(fn));
			int time = GetTickCount();
			bool execerr = false;
			if(rc) {
				String exec;
				exec << "windres -i " << GetHostPathQ(fn)
					<< " -o " << GetHostPathQ(objfile) << IncludesShort(" --include-dir=", package, pkg);
				PutVerbose(exec);
				int slot = AllocSlot();
				execerr = (slot < 0 || !Run(exec, slot, GetHostPath(objfile), 1));
			}
			else if(brc) {
				try {
//					String hfn = GetHostPath(fn);
					String brcdata = LoadFile(fn);
					if(brcdata.IsVoid())
						throw Exc(NFormat("error reading file '%s'", fn));
					CParser parser(brcdata, fn);
					BinaryToObject(GetHostPath(objfile), parser, GetFileDirectory(fn), package, pkg);
				}
				catch(Exc e) {
					PutConsole(e);
					execerr = true;
				}
			}
			else {
				String exec = optimize[i] ? cc_speed : cc;
				if(ext == ".c")
					exec << " -x c ";
				else if(ext == ".s" || ext == ".S")
					exec << " -x assembler-with-cpp ";
				else
					exec << fuse_cxa_atexit << " -x c++ ";
				exec << GetHostPathQ(fn) << " -o " << GetHostPathQ(objfile);
				PutVerbose(exec);
				int slot = AllocSlot();
				execerr = (slot < 0 || !Run(exec, slot, GetHostPath(objfile), 1));
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
//		if(ccount)
//			PutCompileTime(time, ccount);
		IdeConsoleEndGroup();
		return false;
	}

	linkoptions << Gather(pkg.link, config.GetKeys());
	if(linkoptions.GetCount())
		linkoptions << ' ';

	Vector<String> libs = Split(Gather(pkg.library, config.GetKeys()), ' ');
	linkfile.Append(libs);

	int libtime = GetTickCount();
	if(!HasFlag("MAIN")) {
		if(HasFlag("BLITZ") || HasFlag("NOLIB")) {
			linkfile.Append(obj);
			IdeConsoleEndGroup();
//			if(ccount)
//				PutCompileTime(time, ccount);
			return true;
		}
		IdeConsoleEndGroup();
		if(!Wait())
			return false;
		String product;
		if(is_shared)
			product = GetSharedLibPath(package);
		else
			product = CatAnyPath(outdir, GetAnyFileName(package) + ".a");
		String hproduct = GetHostPath(product);
		Time producttime = GetFileTime(hproduct);
//		LOG("hproduct = " << hproduct << ", time = " << producttime);
		linkfile.Add(GetHostPath(product));
		for(int i = 0; i < obj.GetCount(); i++)
			if(GetFileTime(obj[i]) > producttime) {
				String lib;
				if(is_shared) {
					lib = CompilerName();
					lib << " -shared -fPIC -fuse-cxa-atexit";
					if(!HasFlag("SHARED") && !is_shared)
						lib << " -static";
//					else if(!HasFlag("WIN32")) // TRC 05/03/08: dynamic fPIC causes trouble in MinGW
//						lib << " -dynamic -fPIC"; // TRC 05/03/30: dynamic fPIC doesn't seem to work in GCC either :-)
					if(HasFlag("GCC32"))
						lib << " -m32";
					Point p = ExtractVersion();
					if(!IsNull(p.x)) {
						lib << " -Xlinker --major-image-version -Xlinker " << p.x;
						if(!IsNull(p.y))
							lib << " -Xlinker --minor-image-version -Xlinker " << p.y;
					}
					lib << ' ' << Gather(pkg.link, config.GetKeys());
					lib << " -o ";
				}
				else
					lib = "ar -sr ";
				lib << GetHostPathQ(product);
				for(int i = 0; i < obj.GetCount(); i++)
					lib << ' ' << GetHostPathQ(obj[i]);
				PutConsole("Creating library...");
				DeleteFile(hproduct);
				if(is_shared) {
					for(int i = 0; i < libpath.GetCount(); i++)
						lib << " -L" << GetHostPathQ(libpath[i]);
					for(int i = 0; i < all_uses.GetCount(); i++)
						lib << ' ' << GetHostPathQ(GetSharedLibPath(all_uses[i]));
					for(int i = 0; i < all_libraries.GetCount(); i++)
						lib << " -l" << GetHostPathQ(all_libraries[i]);
				}
				if(!Execute(lib) == 0) {
					DeleteFile(hproduct);
					return false;
				}
				PutConsole(String().Cat() << hproduct << " (" << GetFileInfo(hproduct).length
				           << " B) created in " << GetPrintTime(libtime));
				break;
			}
		return true;
	}

	IdeConsoleEndGroup();
	obj.Append(linkfile);
	linkfile = obj;
	return true;
}


bool GccBuilder::Link(const Vector<String>& linkfile, const String& linkoptions, bool createmap)
{
	if(!Wait())
		return false;
	int time = GetTickCount();
	for(int i = 0; i < linkfile.GetCount(); i++)
		if(GetFileTime(linkfile[i]) >= targettime) {
			Vector<String> lib;
			String lnk = CompilerName();
			if(HasFlag("GCC32"))
				lnk << " -m32";
			if(HasFlag("DLL"))
				lnk << " -shared";
			if(!HasFlag("SHARED") && !HasFlag("SO"))
				lnk << " -static";
//			else if(!HasFlag("WIN32")) // TRC 05/03/08: see above
//				lnk << " -dynamic -fPIC"; // TRC 05/03/30: dynamic fPIC doesn't seem to work in GCC either :-)
			if(HasFlag("WINCE"))
				lnk << " -mwindowsce";
			else if(HasFlag("WIN32")) {
				lnk << " -mwindows";
				if(HasFlag("MT"))
					lnk << " -mthreads";
				if(!HasFlag("GUI"))
					lnk << " -mconsole";
			}
			lnk << " -o " << GetHostPathQ(target);
			if(createmap)
				lnk << " -Wl,-Map," << GetHostPathQ(GetFileDirectory(target) + GetFileTitle(target) + ".map");
			if(HasFlag("DEBUG") || HasFlag("DEBUG_MINIMAL") || HasFlag("DEBUG_FULL"))
				lnk << " -ggdb";
			else
				lnk << (!HasFlag("OSX11") ? " -Wl,-s" : "");
			for(i = 0; i < libpath.GetCount(); i++)
				lnk << " -L" << GetHostPathQ(libpath[i]);
//			lnk << " -Wl,--gc-sections,-O,2 ";
			if (!HasFlag("OSX11"))
			  lnk << " -Wl,-O,2 "; // CXL 05/11/14 --gc-sections causing trouble on ubuntu
			lnk << linkoptions;

			if (HasFlag("OSX11")) {
			  if (HasFlag("POWERPC"))
				lnk << " -arch ppc";
			  if (HasFlag("X86"))
				lnk << " -arch i386";
			}

			for(i = 0; i < linkfile.GetCount(); i++)
				if(ToLower(GetFileExt(linkfile[i])) == ".o")
					lnk  << ' ' << GetHostPathQ(linkfile[i]);
				else
					lib.Add(linkfile[i]);
			if(!HasFlag("SOLARIS")&&!HasFlag("OSX11"))
				lnk << " -Wl,--start-group ";
			for(i = 0; i < lib.GetCount(); i++) {
				String ln = lib[i];
				String ext = ToLower(GetFileExt(ln));
				if(ext == ".a" || ext == ".so" || ext == ".dll" || ext == ".lib")
					lnk << ' ' << GetHostPathQ(FindInDirs(libpath, lib[i]));
				else
					lnk << " -l" << ln;
			}
			if(!HasFlag("SOLARIS")&&!HasFlag("OSX11"))
				lnk << " -Wl,--end-group";
			PutConsole("Linking...");
			CustomStep(".pre-link");
			if(Execute(lnk) == 0) {
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

bool GccBuilder::Preprocess(const String& package, const String& file, const String& target, bool asmout)
{
	Package pkg;
	String packagepath = PackagePath(package);
	pkg.Load(packagepath);
	String packagedir = GetFileFolder(packagepath);
	ChDir(packagedir);
	PutVerbose("cd " + packagedir);

	String cmd = CmdLine(package, pkg);
	cmd << " " << Gather(pkg.option, config.GetKeys());
	cmd << " -o " << target;
	cmd << (asmout ? " -S " : " -E ") << GetHostPathQ(file);
	return Execute(cmd);
}

Builder *CreateGccBuilder()
{
	return new GccBuilder;
}

void RegisterGccBuilder()
{
	RegisterBuilder("GCC", CreateGccBuilder);
	RegisterBuilder("GCC32", CreateGccBuilder);
	RegisterBuilder("GCC_ARM", CreateGccBuilder);
}
