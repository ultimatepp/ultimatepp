#include "CppBase.h"

NAMESPACE_UPP

#define LTIMING(x)  // RTIMING(x)
#define LLOG(x)     // LOG(x)

bool IsCPPFile(const String& path)
{
	return findarg(ToLower(GetFileExt(path)) , ".c", ".cpp", ".cc" , ".cxx", ".icpp") >= 0;
}

bool IsHFile(const String& path)
{
	return findarg(ToLower(GetFileExt(path)) , ".h", ".hpp", ".hxx" , ".hh") >= 0;
}

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

static VectorMap<String, PPMacro> sAllMacros;
static ArrayMap<String, PPFile>   sPPfile;
static int                        sPPserial;

void SweepPPFiles(const Index<String>& keep)
{
	for(int i = 0; i < sPPfile.GetCount(); i++)
		if(keep.Find(sPPfile.GetKey(i)) < 0 && !sPPfile.IsUnlinked(i))
			sPPfile.Unlink(i);
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

	for(int pass = 0; pass < 2; pass++) {
		r = NULL;
		int best = segmenti;
		int line = -1;
		int q = sAllMacros.Find(id);
		while(q >= 0) {
			PPMacro& m = sAllMacros[q];
			if(m.macro.IsUndef()) {
				if(pass == 0 && segment_id.Find(m.segment_id) >= 0)
					undef.FindAdd(m.segment_id); // cancel out undefined macro...
			}
			else
			if(pass == 0 || undef.Find(m.undef_segment_id) < 0) {
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
	Vector<int> namespace_block;
	bool next_segment = true;
	Index<int> local_segments;
	keywords.Clear();
	int linei = 0;
	Md5Stream md5;
	while(!in.IsEof()) {
		String l = in.GetLine();
		while(*l.Last() == '\\' && !in.IsEof()) {
			l.Trim(l.GetLength() - 1);
			l.Cat(in.GetLine());
		}
		RemoveComments(l, incomment);
		try {
			CParser p(l);
			if(p.Char('#')) {
				if(p.Id("define")) {
					if(next_segment) {
						PPItem& m = item.Add();
						m.type = PP_DEFINES;
						m.segment_id = ++sPPserial;
						next_segment = false;
						local_segments.Add(sPPserial);
					}
					CppMacro def;
					String   id = def.Define(p.GetPtr());
					if(id.GetCount()) {
						PPMacro m;
						m.segment_id = sPPserial;
						m.line = linei;
						m.macro = def;
						ppmacro.Add(sAllMacros.Put(id, m));
						md5.Put("#", 1);
						md5.Put(id);
						md5.Put(0);
						md5.Put(m.macro.md5, 16);						
					}
				}
				else
				if(p.Id("undef")) {
					if(p.IsId()) {
						String id = p.ReadId();
						md5.Put("#", 1);
						md5.Put(id);
						md5.Put(1);
						int segmenti = -1;
						PPMacro *um = FindPPMacro(id, local_segments, segmenti);
						if(um) { // heuristic: only local undefs are allowed
							PPItem& m = item.Add();
							m.type = PP_DEFINES;
							m.segment_id = ++sPPserial;
							um->undef_segment_id = m.segment_id;
							next_segment = true;
							local_segments.Add(sPPserial);
							if(id.GetCount()) {
								PPMacro m;
								m.segment_id = sPPserial;
								m.line = linei;
								m.macro.SetUndef();
								ppmacro.Add(sAllMacros.Put(id, m));
							}
						}
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

static VectorMap<String, String>   sIncludePath;
static ArrayMap<String, FlatPP>    sFlatPP;
static String                      sInclude_Path;

void PPSync(const String& include_path)
{
	LLOG("* PPSync");
	sIncludePath.Clear();
	sFlatPP.Clear();
	sInclude_Path = include_path;
}

String GetIncludePath()
{
	return sInclude_Path;
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

static VectorMap<String, Time>     sPathFileTime;

void InvalidateFileTimeCache()
{
	sPathFileTime.Clear();
}

void InvalidateFileTimeCache(const String& path)
{
	sPathFileTime.UnlinkKey(path);
}

Time GetFileTimeCached(const String& p)
{
	LTIMING("GetFileTimeCached");
	int q = sPathFileTime.Find(p);
	if(q >= 0)
		return sPathFileTime[q];
	Time m = FileGetTime(p);
	sPathFileTime.Put(p, m);
	return m;
}

String GetIncludePath(const String& s, const String& filedir)
{
	LTIMING("GetIncludePath");
	String key;
	key << s << "#" << filedir;
	int q = sIncludePath.Find(key);
	if(q >= 0)
		return sIncludePath[q];
	LTIMING("GetIncludePath 2");
	String p = GetIncludePath0(s, filedir);
	sIncludePath.Add(key, p);
	return p;
}

const PPFile& GetPPFile(const char *path)
{
	LTIMING("GetPPFile");
	Time tm = GetFileTimeCached(path);
	PPFile& f = sPPfile.GetPut(path);
	LLOG("GetPPFile " << path << ", " << f.filetime << ", " << tm);
	if(f.filetime != tm) {
		f.filetime = tm;
		FileIn in(path);
		f.Parse(in);
	}
	return f;
}

bool IsSameFile(const String& f1, const String& f2)
{
	return NormalizePath(f1) == NormalizePath(f2);
}

const FlatPP& GetFlatPPFile(const char *path, Index<String>& visited)
{
	LTIMING("GetFlatPPFile");
	LLOG("GetFlatPPFile " << path);
	int q = sFlatPP.Find(path);
	if(q >= 0) {
		LLOG("From cache");
		return sFlatPP[q];
	}
	FlatPP& fp = sFlatPP.Add(path);
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
	sDefs = defs;
	LoadPPConfig();
}

void CleanPP()
{
	sAllMacros.Clear();
	sPPfile.Clear();
	sPPserial = 0;
	LoadPPConfig();
}

void SerializePPFiles(Stream& s)
{
	s % sAllMacros % sPPfile % sPPserial;
	if(s.IsLoading())
		LoadPPConfig();
}

END_UPP_NAMESPACE
