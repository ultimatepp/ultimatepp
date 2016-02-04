#include "Core.h"

const char *SkipSpc(const char *term) {
	while(*term == '\t' || *term == ' ')
		term++;
	return term;
}

String FindIncludeFile(const char *s, const String& filedir, const Vector<String>& incdir)
{
	s = SkipSpc(s);
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
				for(int i = 0; i < incdir.GetCount(); i++) {
					String fn = CatAnyPath(incdir[i], name);
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

class Hdepend {
	struct Info {
		Time                      time;
		Vector<int>               depend;
		Vector<bool>              bydefine;
		Index<String>             macroinclude;
		Vector<String>            define;
		bool                      flag;
		bool                      macroflag;
		bool                      timedirty;
		bool                      guarded;
		bool                      blitzprohibit;

		bool                      CanBlitz() { return guarded && !blitzprohibit; }

		Info()                   { time = Null; flag = false; timedirty = true; guarded = false; }
	};

	ArrayMap<String, Info>            map;
	Vector<String>                    incdir;
	VectorMap<String, Index<String> > depends;

	void   Include(const char *trm, Info& info, const String& filedir, bool bydefine);
	void   ScanFile(const String& path, int map_index);
	int    File(const String& path);
	Time   FileTime(int i);
	void   ClearFlag();
	void   ClearMacroFlag();
	void   GetMacroIndex(Index<String>& dest, int ix);

public:
	void  SetDirs(Vector<String> pick_ id)  { incdir = pick(id); map.Clear(); }
	void  TimeDirty();

	void  ClearDependencies()  { depends.Clear(); }
	void  AddDependency(const String& file, const String& depends);

	Time                  FileTime(const String& path);
	bool                  BlitzApproved(const String& path);
	String                FindIncludeFile(const char *s, const String& filedir) { return ::FindIncludeFile(s, filedir, incdir); }
	const Vector<String>& GetDefines(const String& path);
	Vector<String>        GetDependencies(const String& path);
	const Vector<String>& GetAllFiles()                           { return map.GetKeys(); }
};

void Hdepend::AddDependency(const String& file, const String& dep)
{
	depends.GetAdd(NormalizePath(file)).FindAdd(NormalizePath(dep));
}

const char *RestOfLine(const char *term, String& val) {
	while(*term && *term != '\r' && *term != '\n')
		val.Cat(*term++);
	return term;
}

void Hdepend::Include(const char *s, Hdepend::Info& info, const String& filedir, bool bydefine) {
	s = SkipSpc(s);
	if(IsAlpha(*s) || *s == '_') {
		const char *macid = s;
		while(IsAlNum(*++s) || *s == '_')
			;
		info.macroinclude.FindAdd(String(macid, s));
	}
	else {
		String fn = FindIncludeFile(s, filedir);
		if(!IsNull(fn)) {
			info.depend.Add(File(fn));
			info.bydefine.Add(bydefine);
		}
	}
}

static const char *SkipComment(const char *s) {
	if(*s == '/') {
		if(s[1] == '/')
			for(s += 2; *s && *s != '\n';)
				s++;
		else if(s[1] == '*') {
			for(s += 2; *s && (*s != '*' || s[1] != '/'); s++)
				;
			if(*s)
				s += 2;
		}
	}
	return s;
}

void Hdepend::ScanFile(const String& path, int map_index) {
	Info& info = map[map_index];
	String src = LoadFile(path);
	const char *term = src;
	info.depend.Clear();
	info.bydefine.Clear();
	info.macroinclude.Clear();;
	info.define.Clear();;
	info.guarded = false;
	info.blitzprohibit = false;
	String filedir = GetFileDirectory(path);
	bool testg = true;
	bool defines = IsCSourceFile(path);
	goto begin;
	while(*term) {
		if(term[0] == '/' && term[1] == '*') {
			while(*term) {
				if(term[0] == '*' && term[1] == '/') {
					term += 2;
					break;
				}
				term++;
			}
		}
		else
		if(term[0] == '/' && term[1] == '/') {
			if(term[2] == '#') {
				try {
					CParser p(term + 3);
					if(p.Id("BLITZ_APPROVE") || p.Id("once"))
						info.guarded = true;
					else
					if(p.Id("BLITZ_PROHIBIT"))
						info.blitzprohibit = true;
				}
				catch(CParser::Error) {}
			}
			while(*term) {
				if(*term == '\n') break;
				term++;
			}
		}
		else
		if(*term == '\n') {
			term++;
		begin:
			while(*term == '\r' || *term == ' ' || *term == '\t') term++;
			if(*term == '#') {
				term++;
				while(*term == ' ' || *term == '\t') term++;
				if(term[0] == 'i' && term[1] == 'n' && term[2] == 'c' && term[3] == 'l' &&
				   term[4] == 'u' && term[5] == 'd' && term[6] == 'e' &&
				   (term[7] == ' ' || term[7] == '\t')) {
					term = SkipSpc(term + 7);
					String val;
					term = RestOfLine(term, val);
					val = TrimRight(val);
					Include(val, info, filedir, false);
				}
				else
				if(testg && term[0] == 'i' && term[1] == 'f' && term[2] == 'n' &&
				   term[3] == 'd' && term[4] == 'e' && term[5] == 'f' &&
				   (term[6] == ' ' || term[6] == '\t')) {
					testg = false;
					try {
						CParser p(term + 6);
						if(p.IsId()) {
							String id = p.ReadId();
							if(p.Char('#') && p.Id("define") && p.IsId() && id == p.ReadId())
								info.guarded = true;
						}
					}
					catch(CParser::Error) {}
				}
				else
				if(defines && term[0] == 'd' && term[1] == 'e' && term[2] == 'f' &&
				   term[3] == 'i' && term[4] == 'n' && term[5] == 'e' &&
				   (term[6] == ' ' || term[6] == '\t')) {
				       try {
					       CParser p(term + 6);
					       if(p.IsId())
					          info.define.Add(p.ReadId());
					       term = p.GetPtr();
				       }
				       catch(CParser::Error) {}
				}
				else
				if(term[0] == 'p' && term[1] == 'r' && term[2] == 'a' &&
				   term[3] == 'g' && term[4] == 'm' && term[5] == 'a' &&
				   (term[6] == ' ' || term[6] == '\t')) {
				    try {
						CParser p(term + 6);
						if(p.Id("BLITZ_APPROVE") || p.Id("once"))
							info.guarded = true;
						else
						if(p.Id("BLITZ_PROHIBIT"))
							info.blitzprohibit = true;
						term = p.GetPtr();
				    }
				    catch(CParser::Error) {}
				}
			}
			else if(IsAlpha(*term) || *term == '_') {
				const char *id = term;
				while(IsAlNum(*++term) || *term == '_')
					;
				bool is_bin = (term - id == 6 && !memcmp(id, "BINARY", 6));
				bool is_mask = (term - id == 11 && !memcmp(id, "BINARY_MASK", 11));
				bool is_array = (term - id == 12 && !memcmp(id, "BINARY_ARRAY", 12));
				if(is_bin || is_mask || is_array) {
					while(*term && (byte)*term <= ' ')
						term++;
					if(*term == '(') {
						while(*term && *term != ')' && *term != ',')
							term++;
						if(is_array && *term == ',') {
							while(*++term && *term != ')' && *term != ',')
								;
						}
						if(*term == ',') {
							while(*++term && *term != ')' && (byte)*term <= ' ')
								;
							if(*term == '\"')
								Include(term, info, filedir, false);
						}
					}
				}
			}
		}
		else {
			if((byte)*term > ' ')
				testg = false;
			term++;
		}
	}
	Index<String> minc;
	for(;;) {
		ClearMacroFlag();
		Index<String> new_minc;
		GetMacroIndex(new_minc, map_index);
		if(new_minc.GetCount() <= minc.GetCount())
			return;
		minc = pick(new_minc);
		term = src;
		while(*term) {
			while(*term && (byte)*term <= ' ')
				term++;
			if(*term == '#') {
				term++;
				while(*term && *term != '\n' && (byte)*term <= ' ')
					term++;
				if(Peek32le(term) == 'd' + ('e' << 8) + ('f' << 16) + ('i' << 24)
				&& Peek16le(term + 4) == 'n' + ('e' << 8)) {
					term += 6;
					while(*term && *term != '\n' && (byte)*term <= ' ')
						term++;
					const char *id = term;
					if(IsAlpha(*term) || *term == '_')
						while(IsAlNum(*++term) || *term == '_')
							;
					String ident(id, term);
					if(minc.Find(ident) >= 0) {
						term = SkipSpc(term);
						String incfn;
						id = term;
						while(*term && *term != '\n')
							term++;
						const char *e = term;
						while(e > id && (byte)e[-1] <= ' ')
							e--;
						Include(String(id, e), info, filedir, true);
					}
				}
			}
			while(*term && *term != '\n')
				if(*term == '/' && (term[1] == '*' || term[1] == '/'))
					term = SkipComment(term);
				else
					term++;
			if(*term)
				term++;
		}
	}
}

int Hdepend::File(const String& f) {
	String path = NormalizePath(f);
	int ii = map.FindAdd(path);
	Info& info = map[ii];
	if(info.flag) return ii;
	info.flag = true;
	FindFile ff(path);
	if(!ff || ff.GetLastWriteTime() == info.time) {
		if(info.timedirty) {
			for(int i = 0; i < info.depend.GetCount(); i++)
				File(map.GetKey(info.depend[i]));
			info.timedirty = false;
		}
	}
	else {
		info.time = ff.GetLastWriteTime();
		if(info.time > GetSysTime() + 10)
			PutConsole(String().Cat() << "WARNING: " << path << " has invalid (future) time " << info.time);
		ScanFile(path, ii);
	}
	return ii;
}

void Hdepend::GetMacroIndex(Index<String>& dest, int ix) {
	Info& info = map[ix];
	if(!info.macroflag) {
		info.macroflag = true;
		int i;
		for(i = 0; i < info.macroinclude.GetCount(); i++)
			dest.FindAdd(info.macroinclude[i]);
		for(i = 0; i < info.depend.GetCount(); i++)
			GetMacroIndex(dest, info.depend[i]);
	}
}

Time Hdepend::FileTime(int ii)
{
//	LOG("FileTime " << map.GetKey(ii));
//	LOGBEGIN();
	Info& info = map[ii];
//	DUMP(info.time);
	Time time = info.time;
	if(!info.flag) {
		info.flag = true;
		for(int i = 0; i < info.depend.GetCount(); i++)
			time = max(time, FileTime(info.depend[i]));
	}
//	LOGEND();
	return time;
}

Vector<String> Hdepend::GetDependencies(const String& path)
{
	ClearFlag();
	Index<int> out;
	out.Add(File(path));
	for(int i = 0; i < out.GetCount(); i++)
		if(out[i] >= 0)
			FindAppend(out, map[out[i]].depend);
	Vector<String> outnames;
	for(int i = 1; i < out.GetCount(); i++)
		if(out[i] >= 0)
			outnames.Add(map.GetKey(out[i]));
	return outnames;
}

void Hdepend::ClearFlag()
{
	for(int i = 0; i < map.GetCount(); i++)
		map[i].flag = false;
}

void Hdepend::ClearMacroFlag()
{
	for(int i = 0; i < map.GetCount(); i++)
		map[i].macroflag = false;
}

void Hdepend::TimeDirty()
{
	for(int i = 0; i < map.GetCount(); i++)
		map[i].timedirty = true;
}


Time Hdepend::FileTime(const String& path)
{
	String p = NormalizePath(path);
	ClearFlag();
	int d = File(p);
	ClearFlag();
	Time h = FileTime(d);
	d = depends.Find(p);
	if(d >= 0) {
		const Index<String>& dep = depends[d];
		for(int i = 0; i < dep.GetCount(); i++) {
			FindFile ff(dep[i]);
			if(ff) {
				Time tm = ff.GetLastWriteTime();
				if(tm > GetSysTime() + 10)
					PutConsole(String().Cat() << "WARNING: " << dep[i] << " has invalid (future) time " << tm);
				if(tm > h)
					h = tm;
			}
		}
	}
	return h;
}

bool Hdepend::BlitzApproved(const String& path)
{
	ClearFlag();
	int d = File(path);
	ClearFlag();
	Info& info = map[d];
	if(info.blitzprohibit)
		return false;
	if(info.guarded)
		return true;
	for(int i = 0; i < info.depend.GetCount(); i++)
		if(!info.bydefine[i] && !map[info.depend[i]].CanBlitz()) {
			PutVerbose(String().Cat() << map.GetKey(info.depend[i])
			           << "(1) : blocks BLITZ of " << path);
			return false;
		}
	return true;
}

const Vector<String>& Hdepend::GetDefines(const String& path)
{
	ClearFlag();
	int d = File(path);
	ClearFlag();
	return map[d].define;
}

void HdependSetDirs(Vector<String> pick_ id)
{
	Single<Hdepend>().SetDirs(pick(id));
}

void HdependTimeDirty()
{
	Single<Hdepend>().TimeDirty();
}

Time HdependFileTime(const String& path)
{
	String of = IdeGetOneFile();
	if(IsNull(of))
		return Single<Hdepend>().FileTime(path);
	else
		return of == path ? GetSysTime() : Time::Low();
}

String FindIncludeFile(const char *s, const String& filedir)
{
	return Single<Hdepend>().FindIncludeFile(s, filedir);
}

bool HdependBlitzApproved(const String& path)
{
	return Single<Hdepend>().BlitzApproved(path);
}

const Vector<String>& HdependGetDefines(const String& path)
{
	return Single<Hdepend>().GetDefines(path);
}

Vector<String> HdependGetDependencies(const String& file)
{
	return Single<Hdepend>().GetDependencies(file);
}

void  HdependClearDependencies()
{
	Single<Hdepend>().ClearDependencies();
}

void  HdependAddDependency(const String& file, const String& depends)
{
	Single<Hdepend>().AddDependency(file, depends);
}

const Vector<String>& HdependGetAllFiles()
{
	return Single<Hdepend>().GetAllFiles();
}
