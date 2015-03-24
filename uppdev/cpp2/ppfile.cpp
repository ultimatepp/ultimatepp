#include "cpp.h"

void SetSpaces(String& l, int pos, int count)
{
	StringBuffer s = l;
	memset(~s + pos, ' ', count);
	l = s;
}

void RemoveComments(String& l, bool& incomment)
{
	int q = -1;
	int w = -1;
	if(incomment)
		q = w = 0;
	else {
		q = l.Find("/*");
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

void PPFile::CheckEndNamespace(Vector<int>& namespace_block, int level)
{
	if(namespace_block.GetCount() && namespace_block.Top() == level) {
		namespace_block.Drop();
		item.Add().type = PP_NAMESPACE_END;
	}
}

void PPFile::Parse(Stream& in)
{
	item.Clear();
	includes.Clear();
	bool was_using = false;
	bool was_namespace = false;
	int  level = 0;
	bool incomment = false;
	Vector<int> namespace_block;
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
					PPItem& m = item.Add();
					m.type = PP_DEFINE;
					m.id = m.macro.Define(p.GetPtr());
					if(IsNull(m.id))
						item.Drop();
				}
				else
				if(p.Id("include")) {
					PPItem& m = item.Add();
					m.type = PP_INCLUDE;
					m.id = TrimBoth(p.GetPtr());
					if(IsNull(m.id))
						item.Drop();
					else
						includes.FindAdd(m.id);
				}
			}
			else {
				while(!p.IsEof()) {
					if(was_namespace) {
						int type = was_using ? PP_USING : PP_NAMESPACE;
						String id;
						while(p.Char2(':', ':'))
							id = "::";
						id << p.ReadId();
						while(p.Char2(':', ':'))
							id << "::" << p.ReadId();
						if(!was_using)
							namespace_block.Add(level);
						PPItem& m = item.Add();
						m.type = type;
						m.id = id;
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
							static VectorMap<String, String> namespace_macro; // TODO: Make this ugly trick dynamic
							ONCELOCK {
								namespace_macro.Add("_STD_BEGIN", "std");
								namespace_macro.Add("_C_STD_BEGIN", "std");
								namespace_macro.Add("_STDEXT_BEGIN", "stdext");
								namespace_macro.Add("NAMESPACE_UPP", "Upp");
							}
							static Index<String> namespace_end_macro;
							ONCELOCK {
								namespace_end_macro.Add("_STD_END");
								namespace_end_macro.Add("_STDEXT_END");
								namespace_end_macro.Add("_C_STD_END");
								namespace_end_macro.Add("END_UPP_NAMESPACE");
							}

							String id = p.ReadId();
							int q = namespace_macro.Find(id);
							if(q > 0) {
								PPItem& m = item.Add();
								m.type = PP_NAMESPACE;
								m.id = namespace_macro[q];
								namespace_block.Add(level);
								level++;
							}
							else {
								q = namespace_end_macro.Find(id);
								if(q >= 0) {
									level--;
									CheckEndNamespace(namespace_block, level);
								}
							}
						}
						else
						if(p.Char('}')) {
							if(level > 0) {
								level--;
								CheckEndNamespace(namespace_block, level);
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
	}
}

void PPFile::Dump() const
{
	for(int i = 0; i < item.GetCount(); i++) {
		const PPItem& m = item[i];
		String ll;
		ll << decode(m.type, PP_DEFINE, "#define", PP_INCLUDE, "#include",
		                     PP_USING, "using namespace", PP_NAMESPACE, "namespace",
		                     PP_NAMESPACE_END, "}", "")
		   << ' ' << m.id;
		if(m.type == PP_DEFINE)
			ll << m.macro;
		if(m.type == PP_NAMESPACE)
			ll << " {";
		LOG(ll);
	}
	LOG("----- includes:");
	DUMPC(includes);
}

static ArrayMap<String, FileTime>  sPathFileTime;
static VectorMap<String, String>   sIncludePath;
static VectorMap<String, bool>     sIncludes;

void PPSyncPath()
{
	sPathFileTime.Clear();
	sIncludePath.Clear();
	sIncludes.Clear();
}

String GetIncludePath0(const char *s, const char *filedir, const String& include_path)
{
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
					String fn = NormalizePath(name, filedir);
					if(FileExists(fn))
						return fn;
				}
				return GetFileOnPath(name, include_path, false);
			}
			name.Cat(*s++);
		}
	}
	return Null;
}

FileTime GetFileTimeCached(const String& p)
{
	int q = sPathFileTime.Find(p);
	if(q >= 0)
		return sPathFileTime[q];
	FileTime m = GetFileTime(p);
	sPathFileTime.Add(p, m);
	return m;
}

String GetIncludePath(const String& s, const String& filedir, const String& include_path)
{
	RTIMING("GetIncludePath");
	String key;
	key << s << "#" << filedir << "#" << include_path;
	int q = sIncludePath.Find(key);
	if(q >= 0)
		return sIncludePath[q];
	String p = GetIncludePath0(s, filedir, include_path);
	sIncludePath.Add(key, p);
	return p;
}

const PPFile& GetPPFile(const char *path)
{
	static ArrayMap<String, PPFile> file;
	FileTime tm = GetFileTimeCached(path);
	PPFile& f = file.GetAdd(path);
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

bool IncludesFile(const String& parent_path, const String& path, const String& include_path, Index<String>& visited)
{
	if(visited.Find(parent_path) >= 0)
		return false;
	visited.Add(parent_path);
	if(IsSameFile(parent_path, path))
		return true;
	const PPFile& f = GetPPFile(parent_path);
	for(int i = 0; i < f.includes.GetCount(); i++) {
		String p = GetIncludePath(f.includes[i], GetFileFolder(parent_path), include_path);
		if(p.GetCount()) {
			String key = p + "#" + path;
			int q = sIncludes.Find(key);
			if(q >= 0) {
				if(sIncludes[q])
					return true;
			}
			else {
				bool b = IncludesFile(p, path, include_path, visited);
				sIncludes.Add(key, b);
				if(b)
					return true;
			}
		}
	}
	return false;
}

bool IncludesFile(const String& parent_path, const String& path, const String& include_path)
{
	Index<String> visited;
	return IncludesFile(parent_path, path, include_path, visited);
}
