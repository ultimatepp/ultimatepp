#include "Core.h"

#define LTIMING(x) // RTIMING(x)

using namespace Upp;

void SetSpaces(String& l, int pos, int count)
{
	StringBuffer s(l);
	memset(~s + pos, ' ', count);
	l = s;
}

const char *SkipString(const char *s)
{
	CParser p(s);
	try {
		p.ReadOneString(*s);
	}
	catch(CParser::Error) {}
	s = p.GetPtr();
	while((byte)*(s - 1) <= ' ')
		s--;
	return s;
}

void RemoveComments(String& l, bool& incomment)
{
	int q = -1;
	int w = -1;
	if(incomment)
		q = w = 0;
	else {
		const char *s = l;
		while(*s) {
			if(*s == '\"')
				s = SkipString(s);
			else
			if(s[0] == '/' && s[1] == '/') {
				q = int(s - ~l);
				SetSpaces(l, q, l.GetCount() - q);
				return;
			}
			else
			if(s[0] == '/' && s[1] == '*') {
				q = int(s - ~l);
				break;
			}
			else
				s++;
		}
		if(q >= 0)
			w = q + 2;
	}
	while(q >= 0) {
		int eq = l.Find("*/", w);
		if(eq < 0) {
			incomment = true;
			SetSpaces(l, q, l.GetCount() - q);
			return;
		}
		SetSpaces(l, q, eq + 2 - q);
		incomment = false;
		q = l.Find("/*");
		w = q + 2;
	}
}

void PPInfo::PPFile::Parse(Stream& in)
{
	LTIMING("PPInfo::Parse");

	flags.Clear();
	defines[0].Clear();
	defines[1].Clear();
	includes[0].Clear();
	includes[1].Clear();
	define_includes[0].Clear();
	define_includes[1].Clear();
	guarded = false;
	blitz = AUTO;

	int  linei = 0;
	bool incomment = false;
	
	String guard_id;
	bool first = true;
	int speculative = 0;
	
	auto Blitz = [&](const char *s) {
		try {
			CParser p(s);
			if(p.Id("BLITZ_APPROVE"))
				blitz = APPROVED;
			else
			if(p.Id("BLITZ_PROHIBIT"))
				blitz = PROHIBITED;
			else
			if(p.Id("once"))
				guarded = true;
		}
		catch(CParser::Error) {}
	};
	
	while(!in.IsEof()) {
		String l = in.GetLine();
		String l0 = l;

		auto Flag = [&](const String& id) {
			String comment;
			int w = l0.Find("//");
			if(w >= 0)
				comment = TrimBoth(l0.Mid(w + 2));
			if(id.StartsWith("flag")) {
				String& c = flags.GetAdd(id);
				if(comment.GetLength() > c.GetLength())
					c = comment;
			}
		};
	
		while(*l.Last() == '\\' && !in.IsEof()) {
			l.TrimLast();
			linei++;
			l.Cat(in.GetLine());
		}
	
		if(!incomment && l[0] == '/' && l[1] == '/' && l[2] == '#')
			Blitz(~l + 3);
		RemoveComments(l, incomment);
		try {
			CParser p(l);
			if(p.Char('#')) {
				if(p.Id("define") && p.IsId()) {
					p.NoSkipSpaces().NoSkipComments(); // '#define TEST(x)' is different form '#define TEST (x)' - later is parameterless
					String id = p.ReadId();
					if(id == guard_id) {
						guarded = true;
						speculative = 0;
					}
					if(p.Char('(')) { // skip argument list
						id << "(";
						p.SkipSpaces();
						p.Spaces();
						bool was = false;
						while(p.IsId()) {
							if(was)
								id << ", ";
							id << p.ReadId();
							was = true;
						}
						if(p.Char3('.', '.', '.'))
							id << "...";
						p.Char(')');
						id << ")";
					}
					p.Spaces();
					String h = p.GetPtr();
					defines[!!speculative].Add(id, h);
					all_defines.Add(id, h);
				}
				else
				if(p.Id("ifndef") && p.IsId()) {
					String id = p.ReadId();
					Flag(id);
					if(first)
						guard_id = id;
					speculative++;
				}
				else
				if(p.Id("ifdef") && p.IsId()) {
					Flag(p.ReadId());
					speculative++;
				}
				else
				if(p.Id("if")) {
					while(!p.IsEof()) {
						if(p.IsId())
							Flag(p.ReadId());
						else
							p.Skip();
					}
					speculative++;
				}
				else
				if(p.Id("endif"))
					speculative--;
				else
				if(p.Id("include")) {
					if(p.IsId())
						define_includes[!!speculative].Add(p.ReadId());
					else
						includes[!!speculative].FindAdd(TrimBoth(p.GetPtr()));
				}
				else
				if(p.Id("pragma"))
					Blitz(p.GetPtr());
			}
		}
		catch(...) {}
		if(first)
			for(char s : l)
				if(s != ' ' && s != '\t') {
					first = false;
					break;
				}
		
		linei++;
	}
}

void PPInfo::PPFile::Serialize(Stream& s)
{
	s % time
	  % flags
	  % defines[0]
	  % defines[1]
	  % all_defines
	  % includes[0]
	  % includes[1]
	  % define_includes[0]
	  % define_includes[1]
	  % guarded
	  % blitz
	;
}


void PPInfo::SetIncludes(Vector<String>&& incs)
{
	inc_cache.Clear();
	includes = pick(incs);
	includes_base_count = includes.GetCount();
}

void PPInfo::SetIncludes(const String& incs)
{
	SetIncludes(Split(incs, ';'));
}

void PPInfo::BaseIncludes()
{
	if(includes.GetCount() > includes_base_count) {
		inc_cache.Clear();
		includes.Trim(includes_base_count);
	}
}

void PPInfo::AddInclude(const String& include)
{
	inc_cache.Clear();
	includes << include;
}

void PPInfo::Dir::Load(const String& dir)
{
	for(FindFile ff(dir + "/*.*"); ff; ff.Next()) {
		String n = ff.GetName();
	#ifdef PLATFORM_WIN32
		n = ToLower(n); // in Win32 case mismatch is possible
	#endif
		if(ff.IsFile())
			files.Add(n, ff.GetLastWriteTime());
		if(ff.IsFolder())
			subdirs.FindAdd(n);
	}
}

Time PPInfo::GetFileTime(const String& path)
{
	String dir = GetFileFolder(path);
	String name = GetFileName(path);
#ifdef PLATFORM_WIN32
	dir = ToLower(dir);
	name = ToLower(name);
#endif
	int q = dir_cache.Find(dir);
	if(q < 0) {
		String pdir = GetFileFolder(dir); // check that dir does make some sense...
		bool valid_dir = true;
		if(pdir.GetCount() > 3) {
			int dq = dir_cache.Find(pdir);
			if(dq < 0) {
				dq = dir_cache.GetCount();
				dir_cache.Add(pdir).Load(pdir);
			}
			valid_dir = dir_cache[dq].subdirs.Find(GetFileName(dir)) >= 0;
		}
		q = dir_cache.GetCount();
		Dir& d = dir_cache.Add(dir);
		if(valid_dir)
			d.Load(dir);
	}
	return dir_cache[q].files.Get(name, Null);
}

String PPInfo::NormalizePath(const String& path, const String& curr_dir)
{
	String key = path + "|" + curr_dir;
	int q = normalize_path_cache.Find(key);
	if(q < 0) {
		String p = Upp::NormalizePath(path, curr_dir);
		normalize_path_cache.Add(key, p);
		return p;
	}
	return normalize_path_cache[q];
}

String PPInfo::FindIncludeFile(const char *s, const String& filedir, const Vector<String>& incdirs)
{
	while(*s == ' ' || *s == '\t')
		s++;
	int type = *s;
	if(type == '<' || type == '\"' || type == '?') {
		s++;
		String name;
		if(type == '<') type = '>';
		while(*s != '\r' && *s != '\n') {
			if(*s == type) {
				if(type == '\"') {
					String fn = NormalizePath(name, filedir);
					if(FileExists(fn))
						return fn;
				}
				for(int i = 0; i < incdirs.GetCount(); i++) {
					String fn = NormalizePath(CatAnyPath(incdirs[i], name));
					if(FileExists(fn))
						return fn;
				}
				break;
			}
			name.Cat(*s++);
		}
	}
	return String();
}

String PPInfo::FindIncludeFile(const char *s, const String& filedir)
{
	String key = filedir + "|" + s;
	int q = inc_cache.Find(key);
	if(q >= 0)
		return inc_cache[q];
	String r = FindIncludeFile(s, filedir, includes);
	inc_cache.Add(key, r);
	return r;
}

void PPInfo::Dirty()
{
	for(PPFile& f : files)
		f.dirty = true;
	inc_cache.Clear();
	dir_cache.Clear();
	String cd = GetCurrentDirectory();
	if(cd != current_dir) {
		current_dir = cd;
		normalize_path_cache.Clear();
	}
}

std::atomic<int> PPInfo::scan_serial;

PPInfo::PPFile& PPInfo::File(const String& path)
{
	PPFile& f = files.GetAdd(path);
	if(f.dirty) {
		Time tm = GetFileTime(path);
		f.dirty = false;
		if(IsNull(tm)) // not a file
			return f;
		if(!IsNull(tm) || tm != f.time || scan_serial != f.scan_serial) {
			String cache_path = CacheFile(GetFileTitle(path) + "$" + SHA1String(path) + ".ppi");
			if(IsNull(f.time))
				LoadFromFile(f, cache_path);
			if(tm != f.time || scan_serial) {
				int retry = 0;
			again:
				FileIn in(path);
				if(!in && ++retry < 6) { // in case other thread is doing the same (e.g. Indexer)
					Sleep(retry * 100);
					goto again;
				}
				f.Parse(in);
				f.time = tm;
				f.scan_serial = scan_serial;
				StoreToFile(f, cache_path);
			}
		}
	}
	return f;
}

void PPInfo::AddDependency(const String& file, const String& dep)
{
	depends.GetAdd(NormalizePath(file)).FindAdd(NormalizePath(dep));
}

Time PPInfo::GatherDependencies(const String& path, VectorMap<String, Time>& result,
                                ArrayMap<String, Index<String>>& define_includes,
                                bool speculative, const String& include, Vector<String>& chain, bool& found)
{
	PPFile& f = File(path);
	String dir = GetFileFolder(path);
	Index<String>& dics = define_includes.GetAdd(path);
	for(int i = 0; i <= (int)speculative; i++) {
		for(const String& inc : f.define_includes[i])
			dics.FindAdd(inc);
	}
	
	Time ftm = GetFileTime(path);
	
	if(include.GetCount() && path == include)
		found = true;

	auto DoInclude = [&](const String& inc) {
		String ipath = FindIncludeFile(inc, dir);
		if(ipath.GetCount()) {
			int q = result.Find(ipath);
			if(q < 0) {
				q = result.GetCount();
				result.Add(ipath); // prevent infinite recursion
				result[q] = GetFileTime(ipath); // temporary
				result[q] = GatherDependencies(ipath, result, define_includes, speculative,
				                               include, chain, found);
			}
			ftm = max(result[q], ftm);
			q = define_includes.Find(ipath);
			if(q >= 0)
				for(const String& i : define_includes[q])
					dics.FindAdd(i);
		}
	};

	for(int i = 0; i <= (int)speculative; i++) {
		for(const String& inc : f.includes[i]) {
			DoInclude(inc);
			if(found)
				goto done;
		}
		for(int i = 0; i < dics.GetCount(); i++) { // cannot use range for as dics can change
			String id = dics[i];
			for(int j = 0; j <= (int)speculative; j++) // defines can be speculative too (CtrlCore.h, Turtle.h issue)
				for(int q = f.defines[j].Find(id); q >= 0; q = f.defines[j].FindNext(q)) {
					DoInclude(f.defines[j][q]);
					if(found)
						goto done;
				}
		}
	}

done:
	result.GetAdd(path) = ftm;
	if(found)
		chain.Add(path);
	return ftm;
}

Time PPInfo::GatherDependencies(const String& path,
                                VectorMap<String, Time>& result,
                                ArrayMap<String, Index<String>>& define_includes,
                                bool speculative)
{
	Vector<String> chain;
	bool found = false;
	return GatherDependencies(path, result, define_includes, speculative, String(), chain, found);
}

Time PPInfo::GetTime(const String& path, VectorMap<String, Time> *ret_result)
{
	String p = NormalizePath(path);

	VectorMap<String, Time> result;
	ArrayMap<String, Index<String>> define_includes;
	Time ftm = GatherDependencies(p, result, define_includes, true);

	int d = depends.Find(p);
	if(d >= 0) {
		const Index<String>& dep = depends[d];
		for(const String& d : dep) {
			Time tm = GetFileTime(d);
			if(tm > ftm)
				ftm = tm;
		}
	}

	if(ret_result)
		*ret_result = pick(result);

	return ftm;
}

bool PPInfo::BlitzApproved(const String& path)
{
	PPFile& f = File(path);
	if(f.blitz == APPROVED)
		return true;
	if(f.blitz == PROHIBITED)
		return false;
	String dir = GetFileFolder(path);
	for(int speculative = 0; speculative < 2; speculative++)
		for(const String& inc : f.includes[speculative]) {
			String ipath = FindIncludeFile(inc, dir);
			if(ipath.GetCount()) {
				PPFile& f = File(ipath);
				if(f.blitz == PROHIBITED)
					return false;
				if(f.blitz != APPROVED && !f.guarded) {
					WhenBlitzBlock(ipath, path);
					return false;
				}
			}
		}
	return true;
}

static PPInfo hdepend;

void HdependSetIncludes(Vector<String>&& id)
{
	hdepend.SetIncludes(pick(id));
}

void HdependBaseIncludes()
{
	hdepend.BaseIncludes();
}

void HdependAddInclude(const String& inc)
{
	hdepend.AddInclude(inc);
}

void HdependTimeDirty()
{
	hdepend.Dirty();
}

void HdependClearDependencies()
{
	hdepend.ClearDependencies();
}

void HdependAddDependency(const String& file, const String& depends)
{
	hdepend.AddDependency(file, depends);
}

Time HdependGetFileTime(const String& path, VectorMap<String, Time> *ret_result)
{
	return hdepend.GetTime(path, ret_result);
}

Vector<String> HdependGetDependencies(const String& path, bool bydefine_too)
{
	String p = NormalizePath(path);

	VectorMap<String, Time> result;
	ArrayMap<String, Index<String>> define_includes;

	hdepend.GatherDependencies(p, result, define_includes, bydefine_too);
	return result.PickKeys();
}

bool HdependBlitzApproved(const String& path)
{
	return hdepend.BlitzApproved(path);
}

const Vector<String>& HdependGetDefines(const String& path)
{
	return hdepend.GetFileDefines(path).GetKeys();
}
