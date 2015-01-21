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

// POSIX lib files has names in form of libXXXXXX.so.ver.minver(.rel)
// so we can't simply get file extension
String CppBuilder::GetSrcType(String fn) const
{
	fn = ToLower(fn);
	String ext = GetFileExt(fn);
	if(!HasFlag("POSIX") || ext == ".so")
		return ext;
	int soPos = fn.ReverseFind(".so");
	if(soPos < 0)
		return ext;
	fn = fn.Mid(soPos + 3);
	const char *c = ~fn;
	while(*c)
	{
		if(*c != '.' && !IsDigit(*c))
			return ext;
		c++;
	}
	return ".so";
}

// from complete lib name/path (libXXX.so.ver.minVer) gets the soname (libXXX.so.ver)
String CppBuilder::GetSoname(String libName) const
{
	
	String soname = GetFileName(libName);
	int soLen = soname.GetCount();
	int soPos = ToLower(soname).ReverseFind(".so");
	if(soPos < 0)
		soPos = soLen;
	else
		soPos += 3;
	if(soname.Mid(soPos, 1) == ".")
	{
		soPos++;
		while(soPos < soLen && IsDigit(soname[soPos]))
			soPos++;
	}
	return soname.Left(soPos);
}

// from complete lib name/path (libXXX.so.ver.minVer) gets the link name (libXXX.so)
String CppBuilder::GetSoLinkName(String libName) const
{
	
	String linkName = GetFileName(libName);
	int soPos = ToLower(linkName).ReverseFind(".so");
	if(soPos < 0)
		soPos = linkName.GetCount();
	else
		soPos += 3;
	return linkName.Left(soPos);
}

String CppBuilder::GetSharedLibPath(const String& package) const
{
	String outfn;
	if(HasFlag("POSIX"))
	   outfn << "lib";
	for(const char *p = package; *p; p++)
		outfn.Cat(IsAlNum(*p) || *p == '-' ? *p : '_');
	if(!IsNull(version) && !HasFlag("POSIX"))
		outfn << version;
	outfn << (HasFlag("WIN32") || HasFlag("WINCE") ? ".dll" : ".so");
	if(HasFlag("POSIX"))
	{
		Point p = ExtractVersion();
		int ver = IsNull(p.x) ? 1 : p.x;
		int minver = IsNull(p.y) ? 0 : p.y;
		outfn << '.' << ver << '.' << minver;
	}
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

String TrimSlash(String s)
{
	while(findarg(*s.Last(), '/', '\\') >= 0)
		s.Trim(s.GetCount() - 1);
	return s;
}

String CppBuilder::GetHostPathQ(const String& path) const
{
	return '\"' + TrimSlash(GetHostPath(path)) + '\"';
}

String CppBuilder::GetHostPathShortQ(const String& path) const
{
	return '\"' + TrimSlash(GetHostPathShort(path)) + '\"';
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

bool CppBuilder::Wait(int slot)
{
	return host->Wait(slot);
}

void CppBuilder::OnFinish(Callback cb)
{
	host->OnFinish(cb);
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
	return Split(r, CharFilterEol);
}

bool CppBuilder::Cd(const String& cmd) {
	if(cmd.GetLength() > 2 && ToLower(cmd.Mid(0, 3)) == "cd ") {
		String path = cmd.Mid(3);
	#ifdef PLATFOTM_POSIX
		chdir(path);
	#endif
	#ifdef PLATFORM_WIN32
		SetCurrentDirectory(path);
	#endif
		return true;
	}
	return false;
}

bool CppBuilder::Cp(const String& cmd, const String& package, bool& error) {
	if(cmd.GetLength() > 2 && ToLower(cmd.Mid(0, 3)) == "cp ") {
		Vector<String> path = Split(cmd.Mid(3), ' ');
		if(path.GetCount() == 2) {
			String p = GetFileFolder(PackagePath(package));
			String p1 = NormalizePath(path[0], p);
			String p2 = NormalizePath(path[1], p);
			RealizePath(p2);
			if(!FileExists(p1)) {
				PutConsole("FAILED: " + cmd);
				error = true;
			}
			SaveFile(p2, LoadFile(p1));
		}
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

Vector<String> CppBuilder::CustomStep(const String& pf, const String& package_, bool& error)
{
	String package = Nvl(package_, mainpackage);
	String path = (*pf == '.' && pf[1] != '.') ? target : SourcePath(package, pf);
	String file = GetHostPath(path);
	String ext = ToLower(GetFileExt(pf));
	for(int i = 0; i < wspc.GetCount(); i++) {
		const Array< ::CustomStep >& mv = wspc.GetPackage(i).custom;
		for(int j = 0; j < mv.GetCount(); j++) {
			const ::CustomStep& m = mv[j];
			if(MatchWhen(m.when, config.GetKeys()) && m.MatchExt(ext)) {
				VectorMap<String, String> mac;
				AddPath(mac, "PATH", file);
				AddPath(mac, "RELPATH", pf);
				AddPath(mac, "DIR", GetFileFolder(PackagePath(package)));
				AddPath(mac, "FILEDIR", GetFileFolder(file));
				AddPath(mac, "PACKAGE", package);
				mac.Add("FILE", GetFileName(file));
				mac.Add("TITLE", GetFileTitle(file));
				AddPath(mac, "EXEPATH", GetHostPath(target));
				AddPath(mac, "EXEDIR", GetHostPath(GetFileFolder(target)));
				mac.Add("EXEFILE", GetFileName(target));
				mac.Add("EXETITLE", GetFileTitle(target));
				AddPath(mac, "OUTDIR", GetHostPath(outdir));
				//BW
				AddPath(mac, "OUTDIR", GetHostPath(GetFileFolder(target)));
				AddPath(mac, "OUTFILE", GetHostPath(GetFileName(target)));
				AddPath(mac, "OUTTITLE", GetHostPath(GetFileTitle(target)));

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
					for(int c = 0; c < cmd.GetCount(); c++) {
						PutVerbose(cmd[c]);
						if(!Cd(cmd[c]) && !Cp(cmd[c], package, error)) {
							String ctext = cmd[c];
							const char *cm = ctext;
							if(*cm == '?')
								cm++;
							if(*ctext != '?' && Execute(cm)) {
								for(int t = 0; t < out.GetCount(); t++)
									DeleteFile(out[t]);
								PutConsole("FAILED: " + ctext);
								error = true;
								return Vector<String>();
							}
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

String BlitzBaseFile()
{
	return ConfigFile("blitzbase");
}

void ResetBlitz()
{
	SaveFile(BlitzBaseFile(), "");
}

Time BlitzBaseTime()
{
	return max(GetSysTime() - 3600, Time(GetFileTime(BlitzBaseFile())));
}

Blitz CppBuilder::BlitzStep(Vector<String>& sfile, Vector<String>& soptions,
                            Vector<String>& obj, Vector<String>& immfile,
                            const char *objext, Vector<bool>& optimize,
                            const Index<String>& noblitz)
{
	Blitz b;
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
		if((ext == ".cpp" || ext == ".cc" || ext == ".cxx" || ext == ".icpp")
		   && HdependBlitzApproved(fn) && IsNull(soptions[i]) && !optimize[i]
		   && fntime < BlitzBaseTime()
		   && noblitz.Find(fn) < 0) {
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
		sfile = pick(excluded);
		soptions = pick(excludedoptions);
		optimize = pick(excludedoptimize);
		if(LoadFile(b.path) != blitz) {
			SaveFile(b.path, blitz);
			b.build = true;
		}
		obj.Add(b.object);
		immfile.Add(b.object);
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
	cc << sep << GetHostPathQ(outdir);
	return cc;
}

String CppBuilder::IncludesShort(const char *sep, const String& package, const Package& pkg)
{
	String cc;
	for(int i = 0; i < include.GetCount(); i++)
		cc << sep << GetHostPathShortQ(include[i]);
	cc << sep << GetHostPathShortQ(outdir);
	return cc;
}

String CppBuilder::DefinesTargetTime(const char *sep, const String& package, const Package& pkg)
{
	String cc;
	for(int i = 0; i < config.GetCount(); i++)
		cc << sep << "flag" << config[i];
	if(main_conf)
		cc << sep << "MAIN_CONF";
	targettime = GetFileTime(target);

	FileOut info(AppendFileName(outdir, "build_info.h"));
	Time t = GetSysTime();
	info << "#define bmYEAR   " << (int)t.year << "\r\n";
	info << "#define bmMONTH  " << (int)t.month << "\r\n";
	info << "#define bmDAY    " << (int)t.day << "\r\n";
	info << "#define bmHOUR   " << (int)t.hour << "\r\n";
	info << "#define bmMINUTE " << (int)t.minute << "\r\n";
	info << "#define bmSECOND " << (int)t.second << "\r\n";
	info << Format("#define bmTIME   Time(%d, %d, %d, %d, %d, %d)\r\n",
	        (int)t.year, (int)t.month, (int)t.day, (int)t.hour, (int)t.minute, (int)t.second);
	info << "#define bmMACHINE " << AsCString(GetComputerName()) << "\r\n";
	info << "#define bmUSER    " << AsCString(GetUserName()) << "\r\n";
	return cc;
}

String CppBuilder::IncludesDefinesTargetTime(const String& package, const Package& pkg)
{
	String cc = Includes(" -I", package, pkg);
	cc << DefinesTargetTime(" -D", package, pkg);
	return cc;
}

bool CppBuilder::HasAnyDebug() const
{
	return HasFlag("DEBUG") || HasFlag("DEBUG_MINIMAL") || HasFlag("DEBUG_FULL");
}
