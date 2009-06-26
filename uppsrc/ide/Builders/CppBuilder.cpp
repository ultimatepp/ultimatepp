#include "Builders.h"

void PutCompileTime(int time, int count)
{
	PutConsole(String().Cat() << count << " file(s) compiled in " << GetPrintTime(time)
	           << " " << int(GetTickCount() - time) / count << " msec/file");
}

String CppBuilder::GetTargetExt() const
{
	if(!HasFlag("WIN32") && !HasFlag("WINCE"))
		return HasFlag("DLL") ? ".so" : "";
	else
		return HasFlag("DLL") ? ".dll" : ".exe";
}

String CppBuilder::GetSharedLibPath(const String& package) const
{
	String outfn;
	for(const char *p = package; *p; p++)
		outfn.Cat(IsAlNum(*p) || *p == '-' ? *p : '_');
	if(!IsNull(version))
		outfn << version;
	outfn << (HasFlag("WIN32") || HasFlag("WINCE") ? ".dll" : ".so");
	return CatAnyPath(GetFileFolder(target), outfn);
}

String CppBuilder::GetHostPath(const String& path) const
{
	return host->GetHostPath(path);
}

String CppBuilder::GetHostPathShort(const String& path) const
{
#ifdef PLATFORM_WIN32
	const dword SHORT_PATH_LENGTH = 2048;
	char short_path[SHORT_PATH_LENGTH];
	dword length = ::GetShortPathName((LPCTSTR) path, (LPTSTR) short_path, SHORT_PATH_LENGTH);
	if(length > 0)
		return String(short_path, length);
#endif
	return path;
}

String CppBuilder::GetHostPathQ(const String& path) const
{
	return '\"' + GetHostPath(path) + '\"';
}

String CppBuilder::GetHostPathShortQ(const String& path) const
{
	return '\"' + GetHostPathShort(path) + '\"';
}

String CppBuilder::GetLocalPath(const String& path) const
{
	return host->GetLocalPath(path);
}

Vector<Host::FileInfo> CppBuilder::GetFileInfo(const Vector<String>& path) const
{
	return host->GetFileInfo(path);
}

Host::FileInfo CppBuilder::GetFileInfo(const String& path) const
{
	return GetFileInfo(Vector<String>() << path)[0];
}

Time CppBuilder::GetFileTime(const String& path) const
{
	return GetFileInfo(path);
}

void CppBuilder::DeleteFile(const Vector<String>& path)
{
	host->DeleteFile(path);
}

void CppBuilder::DeleteFile(const String& path)
{
	host->DeleteFile(Vector<String>() << path);
}

int CppBuilder::Execute(const char *cmdline)
{
	return host->Execute(cmdline);
}

int CppBuilder::Execute(const char *cl, Stream& out)
{
	return host->Execute(cl, out);
}

int CppBuilder::AllocSlot()
{
	return host->AllocSlot();
}

bool CppBuilder::Run(const char *cmdline, int slot, String key, int blitz_count)
{
	return host->Run(cmdline, slot, key, blitz_count);
}

bool CppBuilder::Run(const char *cmdline, Stream& out, int slot, String key, int blitz_count)
{
	return host->Run(cmdline, out, slot, key, blitz_count);
}

bool CppBuilder::Wait()
{
	return host->Wait();
}

void CppBuilder::ChDir(const String& path)
{
	host->ChDir(path);
}

void CppBuilder::SaveFile(const String& path, const String& data)
{
	host->SaveFile(path, data);
}

String CppBuilder::LoadFile(const String& path)
{
	return host->LoadFile(path);
}

bool CppBuilder::FileExists(const String& path) const
{
	return !IsNull(GetFileInfo(path).length);
}

int CasFilter(int c) {
	return c == '\n' ? '\n' : c == '\t' ? ' ' : c >= ' ' ? c : 0;
}

int CharFilterEol(int c) {
	return c == '\n' || c == '\r' ? c : 0;
}

Vector<String> Cuprep(const String& m, const VectorMap<String, String>& mac,
                      const Vector<String>& inc) {
	String r;
	String q = Filter(m, CasFilter);
	const char *s = q;
	while(*s)
		if(*s == '$') {
			String v;
			s++;
			if(*s == '$') {
				r.Cat('$');
				s++;
			}
			else
			if(*s == '(') {
				s++;
				bool pinc = false;
				if(*s == '!') {
					pinc = true;
					s++;
				}
				for(;;) {
					if(*s == ')') {
						s++;
						break;
					}
					if(*s == '\0')
						break;
					v.Cat(*s++);
				}
				if(pinc)
					for(int i = 0; i < inc.GetCount(); i++)
						if(inc[i].Find(' '))
							r << v << '"' << inc[i] << "\" ";
						else
							r << v << inc[i] << ' ';
				else
					r.Cat(mac.Get(v, Null));
			}
			else
				r.Cat('$');
		}
		else
			r.Cat(*s++);
	return Split(r, CharFilterTextTest(CharFilterEol));
}

bool IsCd(const String& cmd) {
	if(cmd.GetLength() > 2 && ToLower(cmd.Mid(0, 3)) == "cd ") {
	#ifdef PLATFOTM_POSIX
		chdir(path);
	#endif
	#ifdef PLATFORM_WIN32
		SetCurrentDirectory(cmd.Mid(3));
	#endif
		return true;
	}
	return false;
}

static void AddPath(VectorMap<String, String>& out, String key, String path)
{
	out.Add(key, path);
	out.Add(key + "_WIN", WinPath(path));
	out.Add(key + "_UNIX", UnixPath(path));
}

Vector<String> CppBuilder::CustomStep(const String& path)
{
	String file = GetHostPath(path);
	String ext = ToLower(GetFileExt(file));
	for(int i = 0; i < wspc.GetCount(); i++) {
		const Array< ::CustomStep >& mv = wspc.GetPackage(i).custom;
		for(int j = 0; j < mv.GetCount(); j++) {
			const ::CustomStep& m = mv[j];
			if(MatchWhen(m.when, config.GetKeys()) && m.MatchExt(ext)) {
				VectorMap<String, String> mac;
				AddPath(mac, "PATH", file);
				AddPath(mac, "DIR", GetFileFolder(file));
				mac.Add("FILE", GetFileName(file));
				mac.Add("TITLE", GetFileTitle(file));
				AddPath(mac, "OUTPATH", GetHostPath(target));
				AddPath(mac, "OUTDIR", GetHostPath(outdir));
				mac.Add("OUTFILE", GetFileName(target));
				mac.Add("OUTTITLE", GetFileTitle(target));
				mac.Add("INCLUDE", Join(include, ";"));
				Vector<String> out = Cuprep(m.output, mac, include);
				bool dirty = out.IsEmpty();
				for(int i = 0; !dirty && i < out.GetCount(); i++)
					dirty = (GetFileTime(file) > GetFileTime(out[i]));
				if(dirty) {
					HdependTimeDirty();
					PutConsole(GetFileName(file));
					Vector<String> cmd = Cuprep(m.command, mac, include);
					String cmdtext;
					for(int c = 0; c < cmd.GetCount(); c++)
						if(!IsCd(cmd[c])) {
							String ctext = cmd[c];
							const char *cm = ctext;
							if(*cm == '?')
								cm++;
							if(*ctext != '?' && Execute(cm)) {
								for(int t = 0; t < out.GetCount(); t++)
									DeleteFile(out[t]);
								return Vector<String>();
							}
						}
				}
				return out;
			}
		}
	}
	Vector<String> out;
	out.Add(path);
	return out;
}

static Time s_bb = Null;

Time BlitzBaseTime()
{
	if(IsNull(s_bb))
		s_bb = GetSysTime();
	return max(GetSysTime() - 3600, s_bb);
}

Blitz CppBuilder::BlitzStep(Vector<String>& sfile, Vector<String>& soptions,
                            Vector<String>& obj, const char *objext,
                            Vector<bool>& optimize)
{
	Blitz b;
	Time now = GetSysTime();
	Vector<String> excluded;
	Vector<String> excludedoptions;
	Vector<bool>   excludedoptimize;
	b.object = CatAnyPath(outdir, "$blitz" + String(objext));
	Time blitztime = GetFileTime(b.object);
	String blitz;
	b.count = 0;
	b.build = false;
	if(!IdeGetOneFile().IsEmpty())
		return b;
	for(int i = 0; i < sfile.GetCount(); i++) {
		String fn = sfile[i];
		String ext = ToLower(GetFileExt(fn));
		String objfile = CatAnyPath(outdir, GetFileTitle(fn) + objext);
		Time fntime = GetFileTime(fn);
		if((ext == ".cpp" || ext == ".cc" || ext == ".cxx")
		   && HdependBlitzApproved(fn) && IsNull(soptions[i]) && !optimize[i]
//		   && (fntime < blitztime || !blitzexists)
//		   && (!FileExists(objfile) || now - fntime > 3600)) { // Causes a strage oscillation
		   && fntime < BlitzBaseTime()) {
			if(HdependFileTime(fn) > blitztime)
				b.build = true;
			blitz << "\r\n"
			      << "#define BLITZ_INDEX__ F" << i << "\r\n"
			      << "#include \"" << GetHostPath(fn) << "\"\r\n";
			b.info << ' ' << GetFileName(fn);
			const Vector<String>& d = HdependGetDefines(fn);
			for(int i = 0; i < d.GetCount(); i++)
				blitz << "#ifdef " << d[i] << "\r\n"
				      << "#undef " << d[i] << "\r\n"
				      << "#endif\r\n";
			blitz << "#undef BLITZ_INDEX__\r\n";
			b.count++;
		}
		else {
			excluded.Add(fn);
			excludedoptions.Add(soptions[i]);
			excludedoptimize.Add(optimize[i]);
		}
	}
	b.path = CatAnyPath(outdir, "$blitz.cpp");
	if(b.count > 1) {
		sfile = excluded;
		soptions = excludedoptions;
		optimize = excludedoptimize;
		if(LoadFile(b.path) != blitz) {
			SaveFile(b.path, blitz);
			b.build = true;
		}
		obj.Add(b.object);
	}
	else {
		DeleteFile(b.path);
		b.build = false;
	}
	return b;
}

String CppBuilder::Includes(const char *sep, const String& package, const Package& pkg)
{
	String cc;
	for(int i = 0; i < include.GetCount(); i++)
		cc << sep << GetHostPathQ(include[i]);
	for(int i = 0; i < pkg.include.GetCount(); i++) {
		const OptItem& m = pkg.include[i];
		if(MatchWhen(m.when, config.GetKeys()))
			cc << sep << GetHostPathQ(SourcePath(package, m.text));
	}	
	return cc;
}

String CppBuilder::IncludesShort(const char *sep, const String& package, const Package& pkg)
{
	String cc;
	for(int i = 0; i < include.GetCount(); i++)
		cc << sep << GetHostPathShortQ(include[i]);
	for(int i = 0; i < pkg.include.GetCount(); i++) {
		const OptItem& m = pkg.include[i];
		if(MatchWhen(m.when, config.GetKeys()))
			cc << sep << GetHostPathShortQ(SourcePath(package, m.text));
	}
	return cc;
}

String CppBuilder::IncludesDefinesTargetTime(const String& package, const Package& pkg)
{
	String cc = Includes(" -I", package, pkg);
	for(int i = 0; i < config.GetCount(); i++)
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

static String MakeIdent(const char *name)
{
	String out;
	for(; *name; name++)
		out << (iscid(*name) ? *name : '_');
	return out;
}

static String MakeSourcePath(const Vector<String>& dirs, String fn, bool raw, bool exporting)
{
	fn = UnixPath(fn);
	for(int i = 0; i < dirs.GetCount(); i++)
	{
		int dl = dirs[i].GetLength();
		if(fn.GetLength() >= dl + 2 && !memcmp(fn, dirs[i], dl) && fn[dl] == '/') {
			String s;
			if(!exporting)
				s << "$(UPPDIR" << (i + 1) << ")";
			s << AdjustMakePath(fn.GetIter(dl + 1));
			return s;
		}
	}
	return raw ? String() : AdjustMakePath(fn);
}

String CppBuilder::GetMakePath(String fn) const
{
	return ::GetMakePath(fn, HasFlag("WIN32"));
}

void CppBuilder::AddMakeFile(MakeFile& makefile, String package,
	const Vector<String>& all_uses, const Vector<String>& all_libraries,
	const Index<String>& common_config, bool exporting)
{
	String packagepath = PackagePath(package);
	Package pkg;
	pkg.Load(packagepath);
	String packagedir = GetFileFolder(packagepath);
	Vector<String> src = GetUppDirs();
	for(int i = 0; i < src.GetCount(); i++)
		src[i] = UnixPath(src[i]);

	bool main = HasFlag("MAIN");
	bool is_shared = HasFlag("SO");
	bool libout = !main && !HasFlag("NOLIB");
	bool win32 = HasFlag("WIN32");

	String pack_ident = MakeIdent(package);
	String outdir = "OutDir_" + pack_ident;
	String macros = "Macro_" + pack_ident;
	String macdef = "$(Macro)";
	String objext = (HasFlag("MSC") || HasFlag("EVC") ? ".obj" : ".o");

	Vector<String> x(config.GetKeys(), 1);
	Sort(x);
	for(int i = 0; i < x.GetCount(); i++) {
		if(common_config.Find(x[i]) < 0)
			macdef << " -Dflag" << x[i];
		x[i] = InitCaps(x[i]);
	}

	makefile.outdir << "$(" << outdir << ")";
	makefile.outfile << AdjustMakePath(GetFileTitle(NativePath(package)));
	if(main)
		makefile.outfile << GetTargetExt();
	else if(is_shared)
		makefile.outfile << (win32 ? ".dll" : ".so");
	else
		makefile.outfile << (win32 && HasFlag("MSC") ? ".lib" : ".a");
	makefile.output << (main ? String("$(OutDir)") : makefile.outdir) << makefile.outfile;

	if(main) {
		String cc = "c++ -c";
		if(HasFlag("DEBUG"))
			cc << " -D_DEBUG " << debug_options;
		else
			cc << ' ' << release_options;
		if(HasFlag("DEBUG_MINIMAL"))
			cc << " -ggdb -g1";
		if(HasFlag("DEBUG_FULL"))
			cc << " -ggdb -g2";
		if(is_shared && !win32)
			cc << " -fPIC ";
		cc << ' ' << Gather(pkg.option, config.GetKeys());
		makefile.config << "CC = " << cc << "\n"
			"CFLAGS = $(CC) -x c\n"
			"CPPFLAGS = $(CC) -x c++ \n"
			"LIBPATH = ";
		for(int i = 0; i < libpath.GetCount(); i++)
			makefile.config << " -L" << GetMakePath(AdjustMakePath(GetHostPathQ(libpath[i])));
		makefile.config << "\n"
			"AR = ar -sr\n";

		makefile.install << "\t-mkdir -p $(OutDir)\n";

		Vector<String> lib;
		String lnk;
		lnk << "c++";
		if(!HasFlag("SHARED"))
			lnk << " -static";
		if(HasFlag("WIN32")) {
			lnk << " -mwindows";
			if(!HasFlag("GUI"))
				makefile.linkfiles << " -mconsole";
		}
		lnk << " -o $(OutFile)";
		if(HasFlag("DEBUG") || HasFlag("DEBUG_MINIMAL") || HasFlag("DEBUG_FULL"))
			lnk << " -ggdb";
		else
			lnk << (!HasFlag("OSX11") ? " -Wl,-s" : "");

		lnk << " $(LIBPATH)";
		if (!HasFlag("OSX11"))
		  lnk << " -Wl,-O,2";
		lnk << " $(LINKOPTIONS)";

		makefile.linkfiles = lnk;
	}

	makefile.config << outdir << " = $(UPPOUT)"
		<< GetMakePath(AdjustMakePath(String().Cat() << package << '/' << method << '-' << Join(x, "-") << '/')) << "\n"
		<< macros << " = " << macdef << "\n";

	makefile.install << "\t-mkdir -p $(" << outdir << ")\n";

	String libdep, libfiles;

	libdep << makefile.output << ":";
	if(is_shared)
	{
		libfiles = "c++ -shared -fPIC"; // -v";
		Point p = ExtractVersion();
		if(!IsNull(p.x)) {
			libfiles << " -Xlinker --major-image-version -Xlinker " << p.x;
			if(!IsNull(p.y))
				libfiles << " -Xlinker --minor-image-version -Xlinker " << p.y;
		}
		libfiles << " -o ";
	}
	else
		libfiles = "$(AR) ";
	libfiles << makefile.output;

	for(int i = 0; i < pkg.GetCount(); i++)
		if(!pkg[i].separator) {
			String gop = Gather(pkg[i].option, config.GetKeys());
			String fn = SourcePath(package, pkg[i]);
			String ext = ToLower(GetFileExt(fn));
			bool isc = ext == ".c";
			bool isrc = (ext == ".rc" && HasFlag("WIN32"));
			bool iscpp = (ext == ".cpp" || ext == ".cc" || ext == ".cxx");
			bool isicpp = (ext == ".icpp");
			if(isc || isrc || iscpp || isicpp) {
				String outfile;
				outfile << makefile.outdir << AdjustMakePath(GetFileTitle(fn)) << (isrc ? "_rc" : "") << objext;
				String srcfile = GetMakePath(MakeSourcePath(src, fn, false, exporting));
				makefile.rules << outfile << ": " << srcfile;
				Vector<String> dep = HdependGetDependencies(fn);
				Sort(dep, GetLanguageInfo());
				for(int d = 0; d < dep.GetCount(); d++) {
					String dfn = MakeSourcePath(src, dep[d], true, exporting);
					if(!IsNull(dfn))
						makefile.rules << " \\\n\t" << GetMakePath(dfn);
				}
				makefile.rules << "\n"
					"\t" << (isc ? "$(CFLAGS)" : "$(CPPFLAGS)") << " $(CINC) $(" << macros << ") "
						<< gop << " " << srcfile << " -o " << outfile << "\n\n";
				if(!libout || isicpp) {
					makefile.linkdep << " \\\n\t" << outfile;
					makefile.linkfiles << " \\\n\t\t" << outfile;
				}
				else {
					libdep << " \\\n\t" << outfile;
					libfiles << " \\\n\t\t" << outfile;
				}
			}
			else
			if(ext == ".o" || ext == ".obj" || ext == ".a" || ext == ".so" || ext == ".lib" || ext == ".dll") {
				makefile.linkdep << " \\\n\t" << fn;
				makefile.linkfiles << ' ' << fn;
			}
		}

	if(libout) {
		makefile.rules << libdep << "\n\t" << libfiles << "\n\n";
		makefile.linkdep << " \\\n\t" << makefile.output;
		makefile.linkfiles << " \\\n\t\t\t" << makefile.output;
	}

	if(main) {
		if(!HasFlag("SOLARIS")&&!HasFlag("OSX11"))
			makefile.linkfiles << " \\\n\t\t-Wl,--start-group ";
		for(int i = 0; i < all_libraries.GetCount(); i++) {
			String ln = all_libraries[i];
			String ext = ToLower(GetFileExt(ln));
			if(ext == ".a" || ext == ".so" || ext == ".dll")
				makefile.linkfileend << " \\\n\t\t\t" << GetHostPathQ(FindInDirs(libpath, ln));
			else
				makefile.linkfileend << " \\\n\t\t\t-l" << ln;
		}
		if(!HasFlag("SOLARIS")&&!HasFlag("OSX11"))
			makefile.linkfileend << " \\\n\t\t-Wl,--end-group\n\n";
	}
}

Point CppBuilder::ExtractVersion()
{
	Point v = Point(Null, Null);
	CParser p(version);
	while(!p.IsEof()) {
		if(p.IsNumber()) {
			v.x = p.ReadNumber();
			break;
		}
		p.GetChar();
		p.Spaces();
	}
	while(!p.IsEof()) {
		if(p.IsNumber()) {
			v.y = p.ReadNumber();
			break;
		}
		p.GetChar();
		p.Spaces();
	}
	return v;
}

void CppBuilder::ShowTime(int count, int start_time)
{
	if(count)
		PutConsole(NFormat("%d file(s) compiled in %s %d msec/file",
			count, GetPrintTime(start_time), msecs(start_time) / count));
}
