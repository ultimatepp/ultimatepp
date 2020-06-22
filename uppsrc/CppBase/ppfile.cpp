#include "CppBase.h"
#include "Internal.h"

namespace Upp {

#define LTIMING(x)  // DTIMING(x)
#define LLOG(x)     // DLOG(x)

static std::atomic<int> s_PPserial;
static VectorMap<String, PPMacro>  sAllMacros; // Only MakePP can write to this
static ArrayMap<String, PPFile>    sPPfile; // Only MakePP can write to this

static VectorMap<String, Time>     s_PathFileTime;
static StaticMutex                 s_PathFileTimeMutex;

static VectorMap<String, String>   s_IncludePath;
static String                      s_Include_Path;
static StaticMutex                 s_IncludePathMutex;

static StaticMutex                 s_FlatPPMutex;
static ArrayMap<String, FlatPP>    s_FlatPP; // ArrayMap to allow read access

int  NextPPSerial()
{
	return ++s_PPserial;
}

void SweepPPFiles(const Index<String>& keep)
{
	CppBaseLock __;
	Index<int> pp_segment_id;
	int unlinked_count = 0;
	for(int i = 0; i < sPPfile.GetCount(); i++)
		if(sPPfile.IsUnlinked(i))
			unlinked_count++;
		else
			if(keep.Find(sPPfile.GetKey(i)) < 0) {
				unlinked_count++;
				sPPfile.Unlink(i);
			}
			else {
				const PPFile& p = sPPfile[i];
				for(int j = 0; j < p.item.GetCount(); j++)
					pp_segment_id.FindAdd(p.item[j].segment_id);
			}
	if(unlinked_count > sPPfile.GetCount() / 2) {
		CleanPP();
		return;
	}
	unlinked_count = 0;
	for(int i = 0; i < sAllMacros.GetCount(); i++) {
		if(sAllMacros.IsUnlinked(i))
			unlinked_count++;
		else
		if(sAllMacros[i].segment_id && pp_segment_id.Find(sAllMacros[i].segment_id) < 0) {
			sAllMacros.Unlink(i);
			unlinked_count++;
		}
		if(unlinked_count > sAllMacros.GetCount() / 2) {
			CleanPP();
			return;
		}
	}
}

String GetSegmentFile(int segment_id)
{
	for(int i = 0; i < sPPfile.GetCount(); i++) {
		const Array<PPItem>& m = sPPfile[i].item;
		for(int j = 0; j < m.GetCount(); j++)
			if(m[j].type == PP_DEFINES && m[j].segment_id == segment_id)
				return sPPfile.GetKey(i);
	}
	return "<not found>";
}

PPMacro *FindPPMacro(const String& id, Index<int>& segment_id, int& segmenti)
{
	Index<int> undef;
	PPMacro *r;
	int best;
	for(int pass = 0; pass < 2; pass++) {
		r = NULL;
		best = segmenti;
		int line = -1;
		int q = sAllMacros.Find(id);
		while(q >= 0) {
			PPMacro& m = sAllMacros[q];
			if(m.macro.IsUndef()) {
				if(pass == 0 && segment_id.Find(m.segment_id) >= 0)
					undef.FindAdd(m.segment_id); // cancel out undefined macro...
			}
			else
			if(pass == 0 || m.segment_id == 0 || undef.Find(m.undef_segment_id) < 0) {
				int si = m.segment_id == 0 ? INT_MAX : segment_id.Find(m.segment_id); // defs macros always override
				if(si > best || si >= 0 && si == best && m.line > line) {
					best = si;
					line = m.line;
					r = &m;
				}
			}
			q = sAllMacros.FindNext(q);
		}
		if(undef.GetCount() == 0)
			break;
	}
	segmenti = best;
	return r;
}

const CppMacro *FindMacro(const String& id, Index<int>& segment_id, int& segmenti)
{
	PPMacro *m = FindPPMacro(id, segment_id, segmenti);
	return m ? &m->macro : NULL;
}

void PPFile::CheckEndNamespace(Vector<int>& namespace_block, int level, Md5Stream& md5)
{
	if(namespace_block.GetCount() && namespace_block.Top() == level) {
		namespace_block.Drop();
		item.Add().type = PP_NAMESPACE_END;
		md5.Put('.');
	}
}

void PPFile::Parse(Stream& in)
{
	LTIMING("PPFile::Parse");
	for(int i = 0; i < ppmacro.GetCount(); i++)
		sAllMacros.Unlink(ppmacro[i]);
	ppmacro.Clear();
	item.Clear();
	includes.Clear();
	bool was_using = false;
	bool was_namespace = false;
	int  level = 0;
	bool incomment = false;
	bool do_pp = true;
	Vector<int> namespace_block;
	bool next_segment = true;
	Index<int> local_segments;
	keywords.Clear();
	int linei = 0;
	Md5Stream md5;
	int current_serial = 0;
	
	VectorMap<String, PPMacro> local_macro; // gather all macros first to reduce locking
	
	while(!in.IsEof()) {
		String l = in.GetLine();
		const char *ll = l;
		while(*ll == ' ' || *ll == '\t')
			ll++;
		if(ll[0] == '/' && ll[1] == '/' && ll[2] == '$')
			do_pp = decode(ll[3], '+', true, '-', false, do_pp);
		while(*l.Last() == '\\' && !in.IsEof()) {
			l.Trim(l.GetLength() - 1);
			l.Cat(in.GetLine());
		}
		RemoveComments(l, incomment);
		try {
			CParser p(l);
			if(p.Char('#')) {
				if(do_pp) {
					if(p.Id("define")) {
						if(next_segment) {
							PPItem& m = item.Add();
							m.type = PP_DEFINES;
							m.segment_id = current_serial = NextPPSerial();
							next_segment = false;
							local_segments.Add(current_serial);
						}
						CppMacro def;
						String   id = def.Define(p.GetPtr());
						if(id.GetCount()) {
							PPMacro& l = local_macro.Add(id);
							l.segment_id = current_serial;
							l.line = linei;
							l.macro = def;
						/*
							PPMacro m;
							m.segment_id = current_serial;
							m.line = linei;
							m.macro = def;
							ppmacro.Add(sAllMacros.Put(id, m));
						*/
							md5.Put("#", 1);
							md5.Put(id);
							md5.Put(0);
							md5.Put(l.macro.md5, 16);
						}
					}
					else
					if(p.Id("undef")) {
						if(p.IsId()) {
							String id = p.ReadId();
							if(id.GetCount()) {
								md5.Put("#", 1);
								md5.Put(id);
								md5.Put(1);
								int q = local_macro.FindLast(id); // heuristic: only local undefs are allowed
								while(q >= 0) {
									PPMacro& um = local_macro[q];
									if(!um.macro.IsUndef()) { // found corresponding macro to undef
										PPItem& m = item.Add();
										m.type = PP_DEFINES;
										m.segment_id = current_serial = NextPPSerial();
										um.undef_segment_id = m.segment_id;
										next_segment = true;
										local_segments.Add(current_serial);
										PPMacro& l = local_macro.Add(id);
										l.segment_id = current_serial;
										l.line = linei;
										l.macro.SetUndef();
									}
									q = local_macro.FindPrev(q);
								}
							}
						/*
							int segmenti = -1;
							PPMacro *um = FindPPMacro(id, local_segments, segmenti);
							if(um && segmenti) {
								PPItem& m = item.Add();
								m.type = PP_DEFINES;
								m.segment_id = current_serial = NextPPSerial();
								um->undef_segment_id = m.segment_id;
								next_segment = true;
								local_segments.Add(current_serial);
								if(id.GetCount()) {
									PPMacro m;
									m.segment_id = current_serial;
									m.line = linei;
									m.macro.SetUndef();
									ppmacro.Add(sAllMacros.Put(id, m));
								}
							}
						*/
						}
					}
					else
					if(p.Id("include")) {
						PPItem& m = item.Add();
						next_segment = true;
						m.type = PP_INCLUDE;
						m.text = TrimBoth(p.GetPtr());
						if(IsNull(m.text))
							item.Drop();
						else
							includes.FindAdd(m.text);
						md5.Put('@');
						md5.Put(m.text);
					}
				}
			}
			else {
				while(!p.IsEof()) {
					if(was_namespace) {
						int type = was_using ? PP_USING : PP_NAMESPACE;
						String id;
						while(p.Char2(':', ':'))
							id = "::";
						if(p.IsId()) {
							id << p.ReadId();
							while(p.Char2(':', ':') && p.IsId())
								id << "::" << p.ReadId();
							if(!was_using)
								namespace_block.Add(level);
							if(!was_using || level == 0) {
								PPItem& m = item.Add();
								next_segment = true;
								m.type = type;
								m.text = id;
							}
							md5.Put('$');
							md5.Put(type);
							md5.Put(id);
						}
						was_namespace = was_using = false;
					}
					else
					if(p.Id("using"))
						was_using = true;
					else
					if(p.Id("namespace"))
						was_namespace = true;
					else {
						was_using = was_namespace = false;
						if(p.IsId()) {
							static const VectorMap<String, String>& namespace_macro = GetNamespaceMacros();
							static const Index<String>& namespace_end_macro = GetNamespaceEndMacros();

							String id = p.ReadId();
							int q = namespace_macro.Find(id);
							if(q > 0) {
								PPItem& m = item.Add();
								next_segment = true;
								m.type = PP_NAMESPACE;
								m.text = namespace_macro[q];
								namespace_block.Add(level);
								level++;
								md5.Put('%');
								md5.Put(id);
							}
							else {
								q = namespace_end_macro.Find(id);
								if(q >= 0) {
									level--;
									CheckEndNamespace(namespace_block, level, md5);
								}
							}
							keywords.Add(id);
						}
						else
						if(p.Char('}')) {
							if(level > 0) {
								level--;
								CheckEndNamespace(namespace_block, level, md5);
							}
						}
						else
						if(p.Char('{'))
							level++;
						else
							p.SkipTerm();
					}
				}
			}
		}
		catch(...) {}
		linei++;
	}
	md5sum = md5.FinishString();
	Sort(keywords);
	Vector<int> remove;
	int i = 0;
	while(i < keywords.GetCount()) { // Remove identical items
		int ii = i;
		i++;
		while(i < keywords.GetCount() && keywords[ii] == keywords[i])
			remove.Add(i++);
	}
	keywords.Remove(remove);
	INTERLOCKED { // this is the only place that is allowed to write to sAllMacros
		for(int i = 0; i < local_macro.GetCount(); i++)
			ppmacro.Add(sAllMacros.Put(local_macro.GetKey(i), local_macro[i]));
	}
}

void PPFile::Dump() const
{
	for(int i = 0; i < item.GetCount(); i++) {
		const PPItem& m = item[i];
		String ll;
		ll << decode(m.type, PP_DEFINES, "#defines ", PP_INCLUDE, "#include ",
		                     PP_USING, "using namespace ", PP_NAMESPACE, "namespace ",
		                     PP_NAMESPACE_END, "}", "");
		if(m.type == PP_DEFINES)
			ll << m.segment_id;
		else
			ll << m.text;
		if(m.type == PP_NAMESPACE)
			ll << " {";
		LOG(ll);
	}
	LOG("----- includes:");
	DUMPC(includes);
}

void PPSync(const String& include_path)
{
	CppBaseLock __;
	LLOG("* PPSync");
	{
		Mutex::Lock __(s_IncludePathMutex);
		s_IncludePath.Clear();
		s_Include_Path = include_path;
	}
	{
		Mutex::Lock __(s_FlatPPMutex);
		s_FlatPP.Clear();
	}
}

void InvalidateFileTimeCache()
{
	Mutex::Lock __(s_PathFileTimeMutex);
	s_PathFileTime.Clear();
}

void InvalidateFileTimeCache(const String& path)
{
	LLOG("InvalidateFileTimeCache " << path);
	Mutex::Lock __(s_PathFileTimeMutex);
	s_PathFileTime.UnlinkKey(path);
}

Time GetFileTimeCached(const String& p)
{
	LTIMING("GetFileTimeCached");
	Mutex::Lock __(s_PathFileTimeMutex);
	int q = s_PathFileTime.Find(p);
	if(q >= 0)
		return s_PathFileTime[q];
	Time m = FileGetTime(p);
	s_PathFileTime.Put(p, m);
	return m;
}

String GetIncludePath()
{
	Mutex::Lock __(s_IncludePathMutex);
	return s_Include_Path;
}

String GetIncludePath0(const char *s, const char *filedir)
{
	LTIMING("GetIncludePath0");
	while(IsSpace(*s))
		s++;
	int type = *s;
	if(type == '<' || type == '\"' || type == '?') {
		s++;
		String name;
		if(type == '<') type = '>';
		while(*s != '\r' && *s != '\n') {
			if(*s == type) {
				if(type == '\"') {
					String fn = NormalizeSourcePath(name, filedir);
					if(FileExists(fn))
						return fn;
				}
				String p = GetFileOnPath(name, GetIncludePath(), false);
				if(p.GetCount())
					return NormalizeSourcePath(p);
				return Null;
			}
			name.Cat(*s++);
		}
	}
	return Null;
}

String GetIncludePath(const String& s, const String& filedir)
{
	LTIMING("GetIncludePath");
	Mutex::Lock __(s_IncludePathMutex);
	String key;
	key << s << "#" << filedir;
	int q = s_IncludePath.Find(key);
	if(q >= 0)
		return s_IncludePath[q];
	LTIMING("GetIncludePath 2");
	String p = GetIncludePath0(s, filedir);
	s_IncludePath.Add(key, p);
	LLOG("GetIncludePath " << s << " " << filedir << ": " << p);
	return p;
}

void MakePP(const Index<String>& paths)
{
	Vector<String> todo;
	Vector<PPFile *> pp;
	for(int i = 0; i < paths.GetCount(); i++) {
		String path = paths[i];
		PPFile& f = sPPfile.GetPut(path);
		Time tm = GetFileTimeCached(path);
		if(f.filetime != tm) {
			f.filetime = tm;
			pp.Add(&f);
			todo.Add(path);
		}
	}
	CoFor(todo.GetCount(), [&](int i) {
		FileIn in(todo[i]);
		pp[i]->Parse(in);
	});
}

const PPFile& GetPPFile(const char *path)
{
	LTIMING("GetPPFile");
	static PPFile zero;
	return sPPfile.Get(path, zero);
}

bool IsSameFile(const String& f1, const String& f2)
{
	return NormalizePath(f1) == NormalizePath(f2);
}

const FlatPP& GetFlatPPFile(const char *path, Index<String>& visited)
{
	LTIMING("GetFlatPPFile");
	LLOG("GetFlatPPFile " << path);
	Mutex::Lock __(s_FlatPPMutex);
	int q = s_FlatPP.Find(path);
	if(q >= 0) {
		LLOG("From cache");
		return s_FlatPP[q];
	}
	FlatPP& fp = s_FlatPP.Add(path);
	const PPFile& pp = GetPPFile(path);
	int n = visited.GetCount();
	visited.FindAdd(path);
	for(int i = 0; i < pp.item.GetCount(); i++) {
		const PPItem& m = pp.item[i];
		if(m.type == PP_INCLUDE) {
			String s = GetIncludePath(m.text, GetFileFolder(path));
			LLOG("#include " << m.text << " -> " << s);
			if(s.GetCount() && visited.Find(s) < 0) {
				visited.Add(s);
				const FlatPP& pp = GetFlatPPFile(s, visited);
				for(int i = 0; i < pp.segment_id.GetCount(); i++)
					fp.segment_id.FindAdd(pp.segment_id[i]);
				for(int i = 0; i < pp.usings.GetCount(); i++)
					fp.usings.FindAdd(pp.usings[i]);
			}
		}
		else
		if(m.type == PP_DEFINES)
			fp.segment_id.FindAdd(m.segment_id);
		else
		if(m.type == PP_USING)
			fp.usings.FindAdd(m.text);
	}
	visited.Trim(n);
	return fp;
}

const FlatPP& GetFlatPPFile(const char *path)
{
	Index<String> visited;
	visited.Add(path);
	return GetFlatPPFile(path, visited);
}

void GetAllMacros(Md5Stream& md5, const String& id, Index<int>& segment_id)
{
	Vector< Tuple2<int, int> > pos;
	Vector<const CppMacro *> def;
	String r;
	int q = sAllMacros.Find(id);
	while(q >= 0) {
		const PPMacro& m = sAllMacros[q];
		int si = segment_id.Find(m.segment_id);
		if(si >= 0) {
			pos.Add(MakeTuple(si, m.line));
			def.Add(&m.macro);
		}
		q = sAllMacros.FindNext(q);
	}
	IndexSort(pos, def);
	int n = def.GetCount();
	if(n) {
		md5.Put(&n, sizeof(int));
		md5.Put(id);
		for(int i = 0; i < n; i++)
			md5.Put(def[i]->md5, 16);
	}
}

static VectorMap<String, String> s_namespace_macro;
static Index<String> s_namespace_end_macro;

static String sDefs;

void LoadPPConfig()
{
	for(int i = 0; i < sAllMacros.GetCount(); i++)
		if(sAllMacros[i].segment_id == 0 && !sAllMacros.IsUnlinked(i))
			sAllMacros.Unlink(i);

	s_namespace_macro.Clear();
	s_namespace_end_macro.Clear();

	StringStream ss(sDefs);
	int linei = 0;
	while(!ss.IsEof()) {
		String l = ss.GetLine();
		try {
			CParser p(l);
			if(p.Char('#')) {
				if(p.Id("define")) {
					CppMacro def;
					String   id = def.Define(p.GetPtr());
					if(id.GetCount()) {
						PPMacro m;
						m.segment_id = 0;
						m.line = linei;
						m.macro = def;
						sAllMacros.Put(id, m);
						if(findarg(TrimBoth(def.body), "}", "};") >= 0)
							s_namespace_end_macro.Add(id);
						try {
							CParser p(def.body);
							if(p.Id("namespace") && p.IsId()) {
								String n = p.ReadId();
								if(p.Char('{') && p.IsEof())
									s_namespace_macro.Add(id, n);
							}
						}
						catch(CParser::Error) {}
					}
				}
			}
		}
		catch(CParser::Error) {}
		linei++;
	}
}

const VectorMap<String, String>& GetNamespaceMacros()
{
	return s_namespace_macro;
}

const Index<String>& GetNamespaceEndMacros()
{
	return s_namespace_end_macro;
}

void SetPPDefs(const String& defs)
{
	CppBaseLock __;
	sDefs = defs;
	LoadPPConfig();
}

void CleanPP()
{
	CppBaseLock __;
	sAllMacros.Clear();
	sPPfile.Clear();
	s_PPserial = 0;
	LoadPPConfig();
}

void SerializePPFiles(Stream& s)
{
	CppBaseLock __;
	int sPPserial = s_PPserial;
	s % sAllMacros % sPPfile % sPPserial;
	s_PPserial = sPPserial;
	if(s.IsLoading())
		LoadPPConfig();

#if 0
	if(s.IsLoading()) { _DBG_
		DDUMP(sPPfile.GetCount());
		DDUMP(sAllMacros.GetCount());
		DDUMP(sPPserial);
		
		Index<int> psegment;
		for(int i = 0; i < sPPfile.GetCount(); i++) {
			const PPFile& p = sPPfile[i];
			for(int j = 0; j < p.item.GetCount(); j++)
				psegment.FindAdd(p.item[j].segment_id);
		}
		DDUMP(psegment.GetCount());
			
		int n = 0; _DBG_
		Index<int> msegment;
		for(int i = 0; i < sAllMacros.GetCount(); i++) { _DBG_
			if(sAllMacros.IsUnlinked(i))
				n++;
			else
				msegment.FindAdd(sAllMacros[i].segment_id);
		}
		DLOG("UNLINKED " << n);
		DLOG("Segments " << msegment.GetCount());
	}
#endif
}

}
