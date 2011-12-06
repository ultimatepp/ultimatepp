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
	String path = *pf == '.' ? target : SourcePath(package, pf);
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
	return cc;
}

String CppBuilder::IncludesShort(const char *sep, const String& package, const Package& pkg)
{
	String cc;
	for(int i = 0; i < include.GetCount(); i++)
		cc << sep << GetHostPathShortQ(include[i]);
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

static void WriteByteArray(StringBuffer& fo, const String& data)
{
	int pos = 0;
	for(int p = 0; p < data.GetLength(); p++) {
		if(pos >= 70) {
			fo << '\n';
			pos = 0;
		}
		if(pos == 0)
			fo << '\t';
		String part = FormatInt((byte)data[p]);
		fo << part << ", ";
		pos += part.GetLength() + 2;
	}
}

String CppBuilder::BrcToC(String objfile, CParser& binscript, String basedir,
                          const String& package, const Package& pkg)
{
	BinObjInfo info;
	info.Parse(binscript, basedir);
	StringBuffer fo;
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
					fo << "static char " << ident << "_" << i << "[] = {\n";
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
					WriteByteArray(fo, data);
					fo << "\n};\n\n";
//					fo << AsCString(data, 70, "\t", ASCSTRING_OCTALHI | ASCSTRING_SMART) << ";\n\n";
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
				fo << "\n};\n\n";
			}
		}
		else {
			BinObjInfo::Block& b = belem[0];
			fo << "static char " << ident << "_[] = {\n";
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
			WriteByteArray(fo, data);
			fo << "\n};\n\n"
			"char *" << ident << " = " << ident << "_;\n\n"
//			fo << AsCString(data, 70) << ";\n\n"
			"int " << ident << "_length = " << b_length << ";\n\n";
		}
	}
	return fo;
}
