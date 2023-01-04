#include "Core.h"

void Hdepend::Info::Serialize(Stream& s)
{
	s %	time
	  % depend
	  % bydefine
	  % macroinclude
	  % define
	  % flag
	  % macroflag
	  % timedirty
	  % guarded
	  % blitzprohibit
	;
}

String Hdepend::FindIncludeFile(const char *s, const String& filedir, const Vector<String>& incdirs)
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
					String fn = CatAnyPath(incdirs[i], name);
					if(FileExists(fn))
						return NormalizePath(fn);
				}
				break;
			}
			name.Cat(*s++);
		}
	}
	return String();
}

void Hdepend::AddDependency(const String& file, const String& dep)
{
	depends.GetAdd(NormalizePath(file)).FindAdd(NormalizePath(dep));
}

void Hdepend::Include(const char *s, Hdepend::Info& info, const String& filedir, bool bydefine) {
	while(*s == ' ' || *s == '\t')
		s++;
	if(iscib(*s)) { // #include MACRO
		const char *macid = s;
		while(iscid(*s))
			s++;
		info.macroinclude.FindAdd(String(macid, s));
	}
	else { // normal include
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

void Hdepend::ScanFile(const String& path, int map_index)
{
	Info& info = map[map_index];
	info.depend.Clear();
	info.bydefine.Clear();
	info.macroinclude.Clear();
	info.define.Clear();
	info.guarded = false;
	info.blitzprohibit = false;

	String src;
	if(GetFileLength(path) < 10000000)
		src = LoadFile(path);
	const char *term = src;

	auto Id = [&](const char *id) {
		int n = (int)strlen(id);
		if(memcmp(term, id, n) == 0 && findarg(term[n], ' ', '\t') >= 0) {
			term += n + 1;
			return true;
		}
		return false;
	};

	auto SkipSpc = [&] {
		while(*term == '\t' || *term == ' ')
			term++;
	};
	
	auto RestOfLine = [&] {
		SkipSpc();
		const char *b = term;
		while(*term && *term != '\r' && *term != '\n')
			term++;
		return TrimRight(String(b, term));
	};

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
				if(Id("include")) {
					SkipSpc();
					Include(RestOfLine(), info, filedir, false);
				}
				else
				if(testg && Id("ifndef")) {
					testg = false;
					try {
						CParser p(term);
						if(p.IsId()) {
							String id = p.ReadId();
							if(p.Char('#') && p.Id("define") && p.IsId() && id == p.ReadId())
								info.guarded = true;
						}
						term = p.GetPtr();
						goto begin;
					}
					catch(CParser::Error) {}
				}
				else
				if(defines && Id("define")) {
			       try {
				       CParser p(term);
				       if(p.IsId())
				          info.define.Add(p.ReadId());
				       term = p.GetPtr();
				       goto begin;
			       }
			       catch(CParser::Error) {}
				}
				else
				if(Id("pragma")) {
				    try {
						CParser p(term + 6);
						if(p.Id("BLITZ_APPROVE") || p.Id("once"))
							info.guarded = true;
						else
						if(p.Id("BLITZ_PROHIBIT"))
							info.blitzprohibit = true;
						term = p.GetPtr();
						goto begin;
				    }
				    catch(CParser::Error) {}
				}
			}
			else
			if(IsAlpha(*term) || *term == '_') { // .brc file
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
				if(Id("define")) {
					SkipSpc();
					while(*term && *term != '\n' && (byte)*term <= ' ')
						term++;
					const char *id = term;
					if(iscib(*term))
						while(iscid(*term))
							term++;
					String ident(id, term);
					if(minc.Find(ident) >= 0)
						Include(RestOfLine(), info, filedir, true);
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

int Hdepend::File(const String& f)
{
	String path = NormalizePath(f);
	int ii = map.FindAdd(path);
	Info& info = map[ii];
	if(info.flag) return ii;
	info.flag = true;
	Time file_time = FileGetTime(path);
	if(IsNull(file_time) || file_time == info.time) {
		if(info.timedirty) {
			for(int i = 0; i < info.depend.GetCount(); i++)
				File(map.GetKey(info.depend[i]));
			info.timedirty = false;
		}
	}
	else {
		info.time = file_time;
		if(console && info.time > GetSysTime() + 10)
			PutConsole(String().Cat() << "WARNING: " << path << " has invalid (future) time " << info.time);
		ScanFile(path, ii);
	}
	return ii;
}

void Hdepend::GetMacroIndex(Index<String>& dest, int ix)
{ // gather all macro includes in dependent files
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

Vector<String> Hdepend::GetDependencies(const String& path, bool bydefine_too)
{
	ClearFlag();
	Index<int> out;
	out.Add(File(path));
	for(int i = 0; i < out.GetCount(); i++)
		if(out[i] >= 0) {
			Info& f = map[out[i]];
			for(int i = 0; i < f.depend.GetCount(); i++)
				if(bydefine_too || !f.bydefine[i])
					out.FindAdd(f.depend[i]);
		}
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

void Hdepend::SetDirs(Vector<String>&& id)
{
	if(incdir != id) {
		incdir = pick(id);
		map.Clear();
	}
}

void Hdepend::SetDirs(const String& includes)
{
	SetDirs(pick(Split(includes, ';')));
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
				if(console && tm > GetSysTime() + 10)
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
			if(console)
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

Vector<String> HdependGetDependencies(const String& file, bool bydefine_too)
{
	return Single<Hdepend>().GetDependencies(file, bydefine_too);
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
